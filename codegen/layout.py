"""
Class layout calculator for GD (MSVC x64, Release).

Takes the member lists from the Broma bindings and computes the byte offset of
every member and the size of every class, using the MSVC x64 object model:
natural alignment, one vfptr per polymorphic hierarchy, bases in declaration
order, no reuse of a base's tail padding.

Nothing here is guessed silently: a type whose size is not known makes the
class (and anything derived from or containing it) "incomplete" with the reason
recorded, and no offsets are emitted for it.

Validate the output against the game binary with tools/layout_check.py.
"""
import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Optional


class Unknown(Exception):
    pass


# --- plain data types (public cocos2d-x / MSVC ABI facts) -------------------

def _t(size, align=None):
    return (size, align if align is not None else min(size, 8))

PRIMITIVES = {
    "bool": _t(1), "char": _t(1), "signed char": _t(1), "unsigned char": _t(1),
    "int8_t": _t(1), "uint8_t": _t(1),
    "short": _t(2), "unsigned short": _t(2), "int16_t": _t(2), "uint16_t": _t(2), "wchar_t": _t(2),
    "int": _t(4), "unsigned int": _t(4), "unsigned": _t(4), "int32_t": _t(4), "uint32_t": _t(4),
    "long": _t(4), "unsigned long": _t(4),  # LLP64
    "std::atomic_bool": _t(1),
    "float": _t(4),
    "long long": _t(8), "unsigned long long": _t(8), "int64_t": _t(8), "uint64_t": _t(8),
    "double": _t(8), "size_t": _t(8), "intptr_t": _t(8), "uintptr_t": _t(8), "ptrdiff_t": _t(8),
}

# MSVC x64 Release STL objects (_ITERATOR_DEBUG_LEVEL == 0)
STL_FIXED = {
    "std::string": _t(32, 8), "gd::string": _t(32, 8),
    "std::vector<bool>": _t(32, 8),                      # vector<bool> keeps an extra size_t
    "std::deque": _t(40, 8), "std::list": _t(16, 8),
    "std::set": _t(16, 8), "std::multiset": _t(16, 8),
    "std::map": _t(16, 8), "std::multimap": _t(16, 8),
    "std::unordered_map": _t(64, 8), "std::unordered_set": _t(64, 8),
    "std::unordered_multimap": _t(64, 8), "std::unordered_multiset": _t(64, 8),
    "std::vector": _t(24, 8),
    "std::function": _t(64, 8),
    "std::shared_ptr": _t(16, 8), "std::weak_ptr": _t(16, 8), "std::unique_ptr": _t(8, 8),
    "std::mutex": _t(80, 8),
}

# Plain cocos2d-x structs and enums (sizes of the public cocos2d-x ABI).
COCOS_VALUE_TYPES = {
    "CCPoint": _t(8, 4), "CCSize": _t(8, 4), "CCRect": _t(16, 4),
    "ccColor3B": _t(3, 1), "ccColor4B": _t(4, 1), "ccColor4F": _t(16, 4),
    "ccVertex2F": _t(8, 4), "ccVertex3F": _t(12, 4), "ccTex2F": _t(8, 4),
    "ccBlendFunc": _t(8, 4), "CCAffineTransform": _t(24, 4),
    "ccHSVValue": _t(16, 4),                   # float h,s,v; bool sAdd,vAdd
    "ccV3F_C4B_T2F": _t(24, 4), "ccV3F_C4B_T2F_Quad": _t(96, 4),
    "ccV2F_C4B_T2F": _t(20, 4), "ccV2F_C4B_T2F_Triangle": _t(60, 4),
    "ccGLServerState": _t(4),
    "cc_timeval": _t(8, 4),
    "SEL_MenuHandler": _t(8), "SEL_CallFunc": _t(8), "SEL_SCHEDULE": _t(8),
    "SEL_CallFuncN": _t(8), "SEL_CallFuncND": _t(8), "SEL_CallFuncO": _t(8),
    "SEL_EventHandler": _t(8), "SEL_CompareHandler": _t(8), "SEL_HttpResponse": _t(8),
    "FMOD_RESULT": _t(4),
}


# cocos2d-x enums the bindings use but do not define. All int-sized; declared
# opaquely (`enum X : int;`) in the generated prelude, which is enough for
# layout and signatures.
COCOS_ENUMS = {
    "cocos2d::CCObjectType", "cocos2d::ccTouchesMode", "cocos2d::ccScriptType", "cocos2d::TextureQuality",
    "cocos2d::ccTextAlignment", "cocos2d::CCTextAlignment", "cocos2d::tCCPositionType",
    "cocos2d::CCTexture2DPixelFormat", "cocos2d::ResolutionPolicy", "cocos2d::CCLabelType",
    "cocos2d::enumKeyCodes", "cocos2d::ccKeypadMSGType", "cocos2d::tOrientation",
    "cocos2d::CCVerticalTextAlignment", "cocos2d::BorderAlignment", "cocos2d::ccLanguageType",
    "cocos2d::TargetPlatform", "cocos2d::PopTransition", "cocos2d::ccDirectorProjection",
    "cocos2d::CCProgressTimerType", "cocos2d::eImageFormat", "cocos2d::tCCMenuState",
    "cocos2d::ccTMXTileFlags",
    "cocos2d::extension::CCControlEvent", "cocos2d::extension::EditBoxInputFlag",
    "cocos2d::extension::EditBoxInputMode", "cocos2d::extension::KeyboardReturnType",
    "cocos2d::extension::CCTableViewVerticalFillOrder",
}

# Base classes the bindings reference but do not define. All are pure interfaces
# in cocos2d-x: a single vfptr and no data.  name -> (size, align, polymorphic)
EXTERNAL_CLASSES = {name: (8, 8, True) for name in (
    "cocos2d::CCCopying",
    "cocos2d::CCApplicationProtocol",
    "cocos2d::TypeInfo",
    "cocos2d::CCAccelerometerDelegate",
    "cocos2d::CCRGBAProtocol",
    "cocos2d::CCTextureProtocol",
    "cocos2d::CCTransitionEaseScene",
    "cocos2d::CCSAXDelegator",
    "cocos2d::EGLTouchDelegate",
    "cocos2d::CCSceneDelegate",
    "cocos2d::CCKeypadDelegate",
    "cocos2d::CCKeyboardDelegate",
    "cocos2d::CCMouseDelegate",
    "cocos2d::CCBlendProtocol",
    "cocos2d::CCTextFieldDelegate",
    "cocos2d::CCSceneTransitionDelegate",
    "cocos2d::CCLabelProtocol",
    "cocos2d::extension::CCSortableObject",
    "cocos2d::extension::ColorPickerDelegate",
    "cocos2d::extension::CCScrollViewDelegate",
)}


def align_up(x: int, a: int) -> int:
    return (x + a - 1) // a * a


# --- type-string helpers -----------------------------------------------------

def split_template(t: str):
    """'std::map<int, std::vector<int>>' -> ('std::map', ['int', 'std::vector<int>'])"""
    lt = t.find("<")
    if lt == -1 or not t.endswith(">"):
        return t, []
    base, inner = t[:lt].strip(), t[lt + 1:-1]
    args, depth, cur = [], 0, ""
    for ch in inner:
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        if ch == "," and depth == 0:
            args.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        args.append(cur.strip())
    return base, args


def parse_enums(enums_hpp: Path) -> dict:
    """Enum name -> (size, align) from the bindings' Enums.hpp."""
    out = {}
    if not enums_hpp.exists():
        return out
    for m in re.finditer(r"^\s*enum\s+(?:class\s+)?(\w+)\s*(?::\s*([\w ]+?))?\s*\{", enums_hpp.read_text(errors="replace"), re.M):
        name, under = m.group(1), (m.group(2) or "int").strip()
        out[name] = PRIMITIVES.get(under, _t(4))
    return out


# --- results ------------------------------------------------------------------

@dataclass
class FieldLayout:
    name: str
    type: str
    offset: int
    size: int
    is_pad: bool = False


@dataclass
class Layout:
    name: str
    size: int = 0
    align: int = 1
    poly: bool = False
    own_vptr: bool = False                       # this class adds the vfptr at offset 0
    bases: list = field(default_factory=list)    # [(base name, offset)]
    fields: list = field(default_factory=list)   # [FieldLayout]
    complete: bool = True
    error: Optional[str] = None


class Layouter:
    def __init__(self, classes: dict, enums: dict, overrides: Optional[dict] = None):
        self.classes = classes
        self.enums = enums
        self.overrides = overrides or {}         # type name -> (size, align), calibrated from the binary
        self._cache = {}
        self._active = set()

    # ---- name resolution ----
    def resolve(self, name: str) -> Optional[str]:
        name = name.strip()
        if name in EXTERNAL_CLASSES or name in self.classes:
            return name
        if not name.startswith("cocos2d::") and "cocos2d::" + name in self.classes:
            return "cocos2d::" + name
        if name.startswith("cocos2d::") and name[9:] in self.classes:
            return name[9:]
        return None

    @staticmethod
    def _short(name: str) -> str:
        return name.split("::")[-1]

    @staticmethod
    def is_cocos_enum(t: str) -> bool:
        return t in COCOS_ENUMS or ("cocos2d::" + t) in COCOS_ENUMS

    # ---- type sizes ----
    def type_info(self, t: str):
        t = re.sub(r"\b(const|volatile|struct|class|enum)\b", " ", t)
        t = re.sub(r"\s+", " ", t).strip()
        if t.endswith("*") or t.endswith("&"):
            return (8, 8)
        if t in self.overrides:
            return self.overrides[t]
        if t in PRIMITIVES:
            return PRIMITIVES[t]
        if t in STL_FIXED:
            return STL_FIXED[t]

        base, args = split_template(t)
        base = base.replace("gd::", "std::") if base.startswith("gd::") and base != "gd::string" else base
        if args:
            if base == "std::vector" and args[0] == "bool":
                return STL_FIXED["std::vector<bool>"]
            if base in STL_FIXED:
                return STL_FIXED[base]
            if base == "std::array":
                n = int(args[1], 0)
                sz, al = self.type_info(args[0])
                return (sz * n, al)
            if base == "std::pair":
                return self._struct_of([self.type_info(a) for a in args])
            if base == "std::atomic":
                return self.type_info(args[0])
            if base == "std::optional":
                sz, al = self.type_info(args[0])
                return (align_up(sz + 1, al), al)
            raise Unknown(t)

        short = self._short(t)
        if self.is_cocos_enum(t):
            return _t(4)
        if short in COCOS_VALUE_TYPES and (t.startswith("cocos2d::") or "::" not in t):
            return COCOS_VALUE_TYPES[short]
        if t in self.enums:
            return self.enums[t]
        if short in self.enums:
            return self.enums[short]
        cls = self.resolve(t)
        if cls:
            lay = self.layout(cls)
            if not lay.complete:
                raise Unknown(f"{t} ({lay.error})")
            return (lay.size, lay.align)
        raise Unknown(t)

    @staticmethod
    def _struct_of(infos):
        off, al = 0, 1
        for sz, a in infos:
            off = align_up(off, a) + sz
            al = max(al, a)
        return (align_up(off, al), al)

    # ---- class layout ----
    def layout(self, name: str) -> Layout:
        name = self.resolve(name) or name
        if name in self._cache:
            return self._cache[name]
        if name in EXTERNAL_CLASSES:
            size, align, poly = EXTERNAL_CLASSES[name]
            lay = Layout(name, size=size, align=align, poly=poly, own_vptr=poly)
            self._cache[name] = lay
            return lay
        lay = Layout(name)
        self._cache[name] = lay
        if name in self._active:
            lay.complete, lay.error = False, "inheritance/containment cycle"
            return lay
        self._active.add(name)
        try:
            self._compute(name, lay)
        except Unknown as e:
            lay.complete, lay.error = False, f"unknown type {e}"
        finally:
            self._active.discard(name)
        return lay

    def _compute(self, name: str, lay: Layout):
        cls = self.classes.get(name)
        if cls is None:
            raise Unknown(name)

        own_virtual = any(f.is_virtual for f in cls.functions)
        base_layouts = []
        for b in cls.bases:
            bn = self.resolve(b)
            if bn is None:
                raise Unknown(f"base {b}")
            bl = self.layout(bn)
            if not bl.complete:
                raise Unknown(f"base {b} ({bl.error})")
            base_layouts.append((bn, bl))

        any_poly_base = any(bl.poly for _, bl in base_layouts)
        lay.poly = own_virtual or any_poly_base

        # MSVC puts a fresh vfptr first only when no base provides one; a
        # non-polymorphic base ahead of a polymorphic one needs reordering
        # rules we do not model, so refuse rather than guess.
        seen_nonpoly = False
        for _, bl in base_layouts:
            if not bl.poly:
                seen_nonpoly = True
            elif seen_nonpoly:
                raise Unknown("polymorphic base after non-polymorphic base")

        off, align = 0, 1
        if own_virtual and not any_poly_base:
            lay.own_vptr = True
            off, align = 8, 8

        for bn, bl in base_layouts:
            if bl.size == 1 and not bl.fields and not bl.poly:
                raise Unknown(f"empty base {bn}")
            off = align_up(off, bl.align)
            lay.bases.append((bn, off))
            off += bl.size
            align = max(align, bl.align)

        for m in cls.members:
            if m.is_pad:
                lay.fields.append(FieldLayout(m.name, m.type, off, m.pad_size, True))
                off += m.pad_size
                continue
            sz, al = self.type_info(m.type)
            off = align_up(off, al)
            lay.fields.append(FieldLayout(m.name, m.type, off, sz))
            off += sz
            align = max(align, al)

        if off == 0:
            off = 1
        lay.align = align
        lay.size = align_up(off, align)

    # ---- queries ----
    def offset_of(self, cls: str, member: str) -> Optional[int]:
        lay = self.layout(cls)
        if not lay.complete:
            return None
        for f in lay.fields:
            if f.name == member:
                return f.offset
        return None

    def layout_hash(self, cls: str) -> Optional[str]:
        """Hash over the exact byte layout (size + every member's offset/type), so a
        change in any base class or member type changes the hash of everything built on it."""
        import hashlib
        lay = self.layout(cls)
        if not lay.complete:
            return None
        parts = [f"size={lay.size}"]
        parts += [f"base:{b}@{o}:{self.layout(b).size}" for b, o in lay.bases]
        parts += [f"{f.offset}:{f.size}:{f.type}:{f.name}" for f in lay.fields]
        return hashlib.sha256("|".join(parts).encode()).hexdigest()[:16]
