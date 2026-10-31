"""
C++ header generator.

Emits, per GD/cocos2d class, a header with:
  * the class declaration: bases, every member function declaration, every data
    member in order (declarations and layout only, no helper code);
  * static_asserts on sizeof and on the offset of every data member, taken from
    layout.py. The generated code is compiled with MSVC, so any disagreement
    between our model of the MSVC x64 object layout and the compiler's own fails
    the build instead of showing up in game;
  * a table mapping each hookable member function to its address signature, used
    by cadence::hook<&Class::fn>();
  * registration of the class's layout hash, so a mod reports which layouts it
    was built against and the loader can refuse it if they changed.

Output is a plain include tree:  gd/prelude.hpp, gd/enums.hpp, gd/fwd.hpp,
gd/<Class>.hpp (namespaced classes under gd/<ns>/), gd/gd.hpp (everything).
"""
import json
import re
from pathlib import Path

from layout import (Layouter, EXTERNAL_CLASSES, COCOS_VALUE_TYPES, COCOS_ENUMS, PRIMITIVES, parse_enums)
from broma_parser import MemberDecl

# Parser artifacts: access-specifier labels glued onto the next declaration.
JUNK_WORDS = {"protected", "private", "public", "virtual"}
# Plain words that can appear in a type but are not type names we need to resolve.
TYPE_WORDS = set(PRIMITIVES) | {
    "void", "const", "volatile", "unsigned", "signed", "int", "char", "short", "long", "float", "double",
    "bool", "size_t", "va_list", "TodoReturn", "uint8_t", "int8_t", "uint16_t", "int16_t", "uint32_t",
    "int32_t", "uint64_t", "int64_t", "FMOD_RESULT",
}

IDENT = re.compile(r"[A-Za-z_][A-Za-z0-9_]*(?:::[A-Za-z_][A-Za-z0-9_]*)*")
BUILTIN_TYPE_WORDS = {"unsigned", "signed", "int", "char", "short", "long", "float", "double", "bool", "void", "const", "volatile", "size_t"}

# Geode runtime types the bindings use by value but do not define. Sizes are
# derived from the binary: sizeof(GameManager) == 0x6e8 holds only for 12.
GEODE_RUNTIME_TYPES = {
    "geode::SeedValueRSV": (12, 4),
    "geode::SeedValueRS": (8, 4),
    "geode::SeedValueSR": (8, 4),
    "geode::SeedValueVSR": (12, 4),
}

COCOS_STRUCTS = """
namespace cocos2d {
struct CCPoint { float x, y; };
struct CCSize { float width, height; };
struct CCRect { CCPoint origin; CCSize size; };
struct ccColor3B { unsigned char r, g, b; };
struct ccColor4B { unsigned char r, g, b, a; };
struct ccColor4F { float r, g, b, a; };
struct ccVertex2F { float x, y; };
struct ccVertex3F { float x, y, z; };
struct ccTex2F { float u, v; };
struct ccBlendFunc { unsigned int src, dst; };
struct CCAffineTransform { float a, b, c, d, tx, ty; };
struct ccHSVValue { float h, s, v; bool sAdditive, vAdditive; };
struct ccV3F_C4B_T2F { ccVertex3F vertices; ccColor4B colors; ccTex2F texCoords; };
struct ccV3F_C4B_T2F_Quad { ccV3F_C4B_T2F tl, bl, tr, br; };
struct ccV2F_C4B_T2F { ccVertex2F vertices; ccColor4B colors; ccTex2F texCoords; };
struct ccV2F_C4B_T2F_Triangle { ccV2F_C4B_T2F a, b, c; };
struct cc_timeval { long tv_sec, tv_usec; };
using ccGLServerState = unsigned int;
// Member-function-pointer typedefs in cocos2d-x; 8 bytes with single inheritance.
struct SEL_MenuHandler { void* fn; };
struct SEL_CallFunc { void* fn; };
struct SEL_CallFuncN { void* fn; };
struct SEL_CallFuncND { void* fn; };
struct SEL_CallFuncO { void* fn; };
struct SEL_SCHEDULE { void* fn; };
struct SEL_EventHandler { void* fn; };
struct SEL_CompareHandler { void* fn; };
namespace extension { struct SEL_HttpResponse { void* fn; }; }
}
enum FMOD_RESULT : int;
"""


def cocos_enum_decls():
    """`namespace cocos2d { enum X : int; }` for every enum in COCOS_ENUMS."""
    by_ns = {}
    for q in sorted(COCOS_ENUMS):
        nss, short = split_ns(q)
        by_ns.setdefault(tuple(nss), []).append(short)
    out = []
    for nss, names in sorted(by_ns.items()):
        out.append(open_ns(list(nss)).rstrip() + " " + " ".join(f"enum {n} : int;" for n in names) + " " + close_ns(list(nss)).rstrip())
    return "\n".join(out)

GEODE_STRUCTS = """
namespace geode {
struct SeedValueRSV { int a, b, c; };
struct SeedValueRS { int a, b; };
struct SeedValueSR { int a, b; };
struct SeedValueVSR { int a, b, c; };
}
"""


def split_ns(qualified: str):
    parts = qualified.split("::")
    return parts[:-1], parts[-1]


def header_rel(qualified: str) -> str:
    return "gd/" + qualified.replace("::", "/") + ".hpp"


def open_ns(nss):
    return "".join(f"namespace {n} {{ " for n in nss) + ("\n" if nss else "")


def close_ns(nss):
    return ("} " * len(nss)).rstrip() + ("\n" if nss else "")


def clean(s: str) -> str:
    return re.sub(r"\s+", " ", s).strip()


def split_params(params: str):
    out, depth, cur = [], 0, ""
    for ch in params:
        if ch in "<(":
            depth += 1
        elif ch in ">)":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def param_type(p: str) -> str:
    """Drop the parameter name and any default value: 'cocos2d::CCObject* sender' -> 'cocos2d::CCObject*'."""
    p = clean(p.split("=")[0])
    m = re.match(r"^(.*[\s\*&>])([A-Za-z_]\w*)$", p)
    if m and m.group(2) not in BUILTIN_TYPE_WORDS and m.group(1).strip():
        return m.group(1).strip()
    return p


def by_value_deps(type_str: str, layouter: Layouter):
    """Classes that must be complete to declare a member of this type."""
    deps = set()
    for m in IDENT.finditer(type_str):
        cls = layouter.resolve(m.group(0))
        if not cls or cls in EXTERNAL_CLASSES:
            continue
        rest = type_str[m.end():]
        if re.match(r"^\s*(?:const\s*)*[*&]", rest):
            continue
        deps.add(cls)
    return deps


class Names:
    """Which identifiers in a signature the generated headers can express."""

    def __init__(self, classes, layouter, enums):
        self.classes = classes
        self.layouter = layouter
        self.enums = enums
        self.opaque = set()      # unknown types only ever used behind a pointer/reference
        self._find_opaque()

    def known(self, ident: str) -> bool:
        if ident in TYPE_WORDS or ident.startswith(("std::", "gd::")):
            return True
        if self.layouter.resolve(ident) or ident in GEODE_RUNTIME_TYPES or self.layouter.is_cocos_enum(ident):
            return True
        short = ident.split("::")[-1]
        return ident in self.enums or short in self.enums or short in COCOS_VALUE_TYPES

    @staticmethod
    def _behind_ptr(text: str, end: int) -> bool:
        return re.match(r"^\s*(?:const\s*)*[*&]", text[end:]) is not None

    def _all_type_strings(self):
        for c in self.classes.values():
            for f in c.functions:
                yield f.return_type or ""
                for p in split_params(f.params):
                    yield param_type(p)
            for m in c.members:
                if not m.is_pad:
                    yield m.type

    def _find_opaque(self):
        # Any unknown type used behind a pointer/reference at least once gets an
        # opaque declaration. A by-value use of it still leaves that function out.
        behind = set()
        for t in self._all_type_strings():
            for m in IDENT.finditer(t):
                ident = m.group(0)
                if ident in JUNK_WORDS or self.known(ident):
                    continue
                if self._behind_ptr(t, m.end()):
                    behind.add(ident)
        ns_only = namespace_only_names(self.classes)
        self.nested = {}         # class -> names of nested types, declared opaquely inside it
        for ident in behind:
            parent = "::".join(ident.split("::")[:-1])
            if parent and parent not in ns_only and self.layouter.resolve(parent):
                # A type nested in a class cannot be forward declared outside it.
                self.nested.setdefault(self.layouter.resolve(parent), set()).add(ident.split("::")[-1])
                continue
            self.opaque.add(ident)

    def type_ok(self, t: str) -> bool:
        for m in IDENT.finditer(t):
            ident = m.group(0)
            if self.known(ident):
                continue
            if ident in self.opaque and self._behind_ptr(t, m.end()):
                continue
            parent = "::".join(ident.split("::")[:-1])
            if self.layouter.resolve(parent) in self.nested and ident.split("::")[-1] in self.nested[self.layouter.resolve(parent)] \
                    and self._behind_ptr(t, m.end()):
                continue
            return False
        return True


def apply_binary_sizes(classes, layouter_factory, sizes: dict):
    """
    Reconcile computed sizes with sizes read from the game binary.
    Where the binary's class is larger than the bindings describe (members Geode
    never mapped), append an explicit tail pad so sizeof and every derived class
    are right. Where the bindings describe more than the binary has, give up on
    that class. Returns {class: status}.
    """
    status = {}
    for _ in range(10):
        L = layouter_factory()
        changed = False
        for name, want in sizes.items():
            if name not in classes:
                continue
            lay = L.layout(name)
            if not lay.complete:
                continue
            if name in status and status[name] == "tail-padded":
                continue
            if lay.size < want:
                pad = want - lay.size
                classes[name].members.append(MemberDecl(name="_unmapped_tail", type=f"uint8_t[{pad}]", is_pad=True, pad_size=pad))
                status[name] = "tail-padded"
                changed = True
        if not changed:
            break
    L = layouter_factory()
    for name, want in sizes.items():
        if name not in classes:
            continue
        lay = L.layout(name)
        if not lay.complete:
            continue
        if lay.size == want:
            status.setdefault(name, "verified")
        else:
            status[name] = "conflict"
    return status


def write_prelude(out: Path, classes, enums_src: Path):
    gd = out / "gd"
    gd.mkdir(parents=True, exist_ok=True)

    defined = set(classes)
    structs = COCOS_STRUCTS
    ext = []
    for name in sorted(EXTERNAL_CLASSES):
        nss, short = split_ns(name)
        ext.append(f"{open_ns(nss)}class {short} {{ public: virtual ~{short}(); }};\n{close_ns(nss)}")

    (gd / "prelude.hpp").write_text(f"""// Generated by codegen. Types every GD header needs; layout and nothing else.
#pragma once
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <array>
#include <atomic>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// The bindings list some interfaces both directly and through a base (as GD does).
#pragma warning(disable : 4584)

// The offsets in these headers assume the Release STL (no debug iterators).
#if defined(_ITERATOR_DEBUG_LEVEL) && _ITERATOR_DEBUG_LEVEL != 0
#error "Cadence GD headers need _ITERATOR_DEBUG_LEVEL=0 (build mods in Release, or define it)."
#endif
static_assert(sizeof(std::string) == 32 && sizeof(std::vector<int>) == 24 && sizeof(std::map<int, int>) == 16 &&
              sizeof(std::set<int>) == 16 && sizeof(std::list<int>) == 16 && sizeof(std::deque<int>) == 40 &&
              sizeof(std::unordered_map<int, int>) == 64 && sizeof(std::unordered_set<int>) == 64 &&
              sizeof(std::vector<bool>) == 32, "STL layout differs from the game's (MSVC x64 Release)");

namespace gd {{
// A library type whose size differs between the game's MSVC STL and the one you
// compile with (std::mutex is 80 bytes in the game's, 8 in current toolsets).
// Same size and alignment, no behaviour.
template <size_t Size, size_t Align> struct alignas(Align) opaque {{ unsigned char bytes[Size]; }};
using string = std::string;
template <class T> using vector = std::vector<T>;
template <class K, class V> using map = std::map<K, V>;
template <class K, class V> using unordered_map = std::unordered_map<K, V>;
template <class T> using set = std::set<T>;
template <class T> using unordered_set = std::unordered_set<T>;
}}

struct TodoReturnPlaceholder;
using TodoReturn = TodoReturnPlaceholder;

{structs}
{cocos_enum_decls()}
{GEODE_STRUCTS}
// Interfaces the bindings inherit from but do not define: one vfptr, no data.
{"".join(ext)}
""", encoding="utf-8")

    # enums.hpp: the bindings' Enums.hpp minus its Geode platform include.
    if enums_src.exists():
        text = enums_src.read_text(encoding="utf-8", errors="replace")
        text = re.sub(r'#include\s*<Geode/[^>]*>', '', text)
        text = re.sub(r'struct TodoReturnPlaceholder;\s*using TodoReturn = TodoReturnPlaceholder;', '', text)
        text = text.replace("#pragma once", "")
        (gd / "enums.hpp").write_text("// From geode-sdk/bindings include/Geode/Enums.hpp (BSL-1.0), minus platform includes.\n#pragma once\n#include <gd/prelude.hpp>\n" + text, encoding="utf-8")
    else:
        (gd / "enums.hpp").write_text("#pragma once\n#include <gd/prelude.hpp>\n", encoding="utf-8")


def namespace_only_names(classes):
    """Names the bindings use as a class to hold free functions but that are really
    namespaces (e.g. 'cocos2d', 'FMOD'): they must not become classes."""
    return {split_ns(n)[0][0] for n in classes if "::" in n} & set(classes)


def write_fwd(out: Path, classes, opaque):
    lines = ["// Generated by codegen. Forward declarations of every class, plus opaque",
             "// types the bindings only ever use through a pointer or reference.",
             "#pragma once", "#include <gd/prelude.hpp>", "#include <gd/enums.hpp>", ""]
    skip = namespace_only_names(classes)
    for name in sorted(set(classes) | set(opaque)):
        if name in EXTERNAL_CLASSES or name in skip:
            continue
        nss, short = split_ns(name)
        lines.append(f"{open_ns(nss)}class {short}; {close_ns(nss)}".rstrip())
    (out / "gd" / "fwd.hpp").write_text("\n".join(lines) + "\n", encoding="utf-8")


def usable(f, names):
    """A function is declared only if every type in its signature can be expressed."""
    if f.name in JUNK_WORDS or (f.return_type or "") in JUNK_WORDS:
        return False
    if not names.type_ok(f.return_type or ""):
        return False
    return all(names.type_ok(param_type(p)) for p in split_params(f.params))


def emit_class(name, cls, lay, layouter, status, hashes, names, stats):
    nss, short = split_ns(name)
    rel = header_rel(name)

    funcs = [f for f in cls.functions if usable(f, names)]
    stats["declared"] += len(funcs)
    stats["omitted"] += len(cls.functions) - len(funcs)
    stats["omitted_hookable"] += sum(1 for f in cls.functions if f.addrs.has_win() and not usable(f, names))

    includes = ["<sdk.h>", "<gd/fwd.hpp>"]
    deps = set()
    for b in cls.bases:
        r = layouter.resolve(b)
        if r and r not in EXTERNAL_CLASSES:
            deps.add(r)
    for m in cls.members:
        if not m.is_pad:
            deps |= by_value_deps(m.type, layouter)
    deps.discard(name)
    includes += [f"<{header_rel(d)}>" for d in sorted(deps)]

    L = ["// Generated by codegen from geode-sdk/bindings (BSL-1.0). Do not edit.", "#pragma once"]
    L += [f"#include {i}" for i in includes]
    L.append("")
    L.append(open_ns(nss).rstrip())
    bases = ", ".join(f"public {b}" for b in cls.bases)
    L.append(f"class {short}{' : ' + bases if bases else ''} {{")
    L.append("public:")
    for nested in sorted(names.nested.get(name, ())):
        L.append(f"    class {nested};")

    seen = set()
    for f in funcs:
        key = (f.name, clean(f.params), f.is_static, f.is_const)
        if key in seen:
            continue
        seen.add(key)
        is_ctor = f.is_ctor or (f.name == short and not f.return_type)
        is_dtor = f.is_dtor or (f.name == "~" + short and not f.return_type)
        if is_dtor:
            L.append(f"    {'virtual ' if f.is_virtual else ''}{f.name}();")
            continue
        ret = f.return_type or ""
        params = clean(f.params)
        if is_ctor:
            L.append(f"    {f.name}({params});")
            continue
        prefix = "static " if f.is_static else ("virtual " if f.is_virtual else "")
        L.append(f"    {prefix}{ret} {f.name}({params}){' const' if f.is_const else ''};")

    L.append("")
    for m in cls.members:
        if m.is_pad:
            L.append(f"    uint8_t {m.name}[{m.pad_size}];")
        else:
            t = clean(m.type)
            if t == "std::mutex":
                L.append(f"    gd::opaque<80, 8> {m.name};  // std::mutex")
            else:
                L.append(f"    {t} {m.name};")
    L.append("};")
    L.append(close_ns(nss).rstrip())
    L.append("")

    qn = name
    st = status.get(name, "unverified")
    if lay.complete:
        L.append(f"// layout: {st}")
        L.append(f"static_assert(sizeof({qn}) == {lay.size:#x}, \"{qn}: size differs from the layout computed from the bindings\");")
        for f in lay.fields:
            if f.is_pad:
                continue
            L.append(f"static_assert(offsetof({qn}, {f.name}) == {f.offset:#x}, \"{qn}::{f.name}\");")
        h = hashes.get(name)
        if h:
            ident = re.sub(r"\W", "_", name)
            L.append(f"inline const cadence::detail::LayoutRegistration cadence_layout_{ident}(\"{name}\", \"{h}\");")
    else:
        L.append(f"// layout: INCOMPLETE ({lay.error}); no offsets are asserted for this class")

    # hook table
    counts = {}
    for f in funcs:
        counts[f.name] = counts.get(f.name, 0) + 1
    table = []
    for f in funcs:
        if (not f.addrs.has_win() or f.is_ctor or f.is_dtor or f.name.startswith("operator")
                or f.name in (short, "~" + short)):
            continue
        key = f.full_key().replace("\\", "\\\\").replace('"', '\\"')
        if counts[f.name] == 1:
            expr = f"&{qn}::{f.name}"
        else:
            pts = ", ".join(param_type(p) for p in split_params(f.params))
            ret = f.return_type or "void"
            if f.is_static:
                expr = f"static_cast<{ret}(*)({pts})>(&{qn}::{f.name})"
            else:
                expr = f"static_cast<{ret}({qn}::*)({pts}){' const' if f.is_const else ''}>(&{qn}::{f.name})"
        table.append(f"template <> inline constexpr const char* fn_sig<{expr}> = \"{key}\";")
    if table:
        L.append("namespace cadence {")
        L += table
        L.append("}")
    return "\n".join(L) + "\n", rel


def generate(parser, bindings_dir: Path, version: str, out_dir: Path, sizes: dict):
    classes = parser.classes
    enums_hpp = bindings_dir / "include" / "Geode" / "Enums.hpp"
    enums = parse_enums(enums_hpp)

    def factory():
        return Layouter(classes, enums, GEODE_RUNTIME_TYPES)

    status = apply_binary_sizes(classes, factory, sizes)
    layouter = factory()

    hashes = {}
    for name in classes:
        h = layouter.layout_hash(name)
        if h:
            hashes[name] = h

    # fresh include tree
    gd = out_dir / "include" / "gd"
    if gd.exists():
        for p in sorted(gd.rglob("*"), reverse=True):
            p.unlink() if p.is_file() else p.rmdir()
    names = Names(classes, layouter, enums)
    write_prelude(out_dir / "include", classes, enums_hpp)
    write_fwd(out_dir / "include", classes, names.opaque)

    written = []
    report = {"version": version, "classes": {}}
    stats = {"declared": 0, "omitted": 0, "omitted_hookable": 0}
    skip = namespace_only_names(classes)
    for name, cls in classes.items():
        if name in EXTERNAL_CLASSES or name in skip:
            continue
        lay = layouter.layout(name)
        if name.startswith("FMOD::") or not lay.complete:
            # FMOD is the audio library, not GD; classes whose layout cannot be
            # computed are forward-declared only (a wrong layout is worse than none).
            report["classes"][name] = {"status": "not generated" if name.startswith("FMOD::") else "incomplete",
                                       "size": None, "error": lay.error}
            continue
        text, rel = emit_class(name, cls, lay, layouter, status, hashes, names, stats)
        p = out_dir / "include" / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text, encoding="utf-8")
        written.append(rel)
        report["classes"][name] = {"status": status.get(name, "unverified"), "size": lay.size, "error": None}
    report["functions"] = stats

    umbrella = ["// Generated by codegen. Includes every class header (slow; include only what you use).", "#pragma once"]
    umbrella += [f"#include <{r}>" for r in sorted(written)]
    (out_dir / "include" / "gd" / "gd.hpp").write_text("\n".join(umbrella) + "\n", encoding="utf-8")

    (out_dir / f"layout_report_{version}.json").write_text(json.dumps(report, indent=1, sort_keys=True), encoding="utf-8")
    counts = {}
    for c in report["classes"].values():
        counts[c["status"]] = counts.get(c["status"], 0) + 1
    print(f"  headers: {len(written)} classes -> {out_dir / 'include' / 'gd'}")
    print("  layout status: " + ", ".join(f"{k}={v}" for k, v in sorted(counts.items())))
    print(f"  functions declared: {stats['declared']}; left out (type not expressible): {stats['omitted']}, "
          f"of which hookable by address: {stats['omitted_hookable']}")
    return hashes, report
