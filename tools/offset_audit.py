#!/usr/bin/env python3
"""Check member offsets against how the game's own code uses them.

  pip install capstone pefile
  python tools/offset_audit.py <bindings> [--version 2.2091] [--worst 25] [--class Name]

A class's size can be right while a member sits at the wrong offset, so this is a second,
independent test. For every non-static member function of a class that has a Windows
address, the machine code is read and `this` (rcx on entry) is followed through the
registers it is copied into. Every `[this + offset]` access of a known width is then compared
with the layout computed from the bindings:

  consistent   the access falls inside one member (or the vtable pointer), as the layout says
  unmapped     the access falls in a gap the bindings leave unnamed (the `_unmapped_tail` pad
               or explicit pads): the layout cannot confirm or deny it
  beyond       the access is past the end of what the bindings describe, in a class with no known
               binary size: it has trailing members the bindings never named (not a contradiction)
  inconsistent the access starts in the middle of a scalar member, so the member is at the wrong offset

Each distinct (function, offset) is counted once. Only linear code is followed, `this` is
forgotten when its register is overwritten or at a call (for volatile registers), and a
function that begins by adjusting `this` (`lea reg, [rcx - N]`, a method of a secondary base)
is skipped, as is any function that sits directly in a secondary vtable (found through the RTTI). So this finds contradictions; it cannot prove a layout right. A class with many
consistent accesses and no inconsistent one is good evidence, not a proof.
"""
import argparse, bisect, collections, json, re, struct, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "codegen"))

try:
    import capstone, pefile
    from capstone import x86_const as X
except ImportError:
    sys.exit("needs: pip install capstone pefile")

RCX_FAMILY = {X.X86_REG_RCX, X.X86_REG_ECX, X.X86_REG_CX, X.X86_REG_CL}
VOLATILE = {X.X86_REG_RAX, X.X86_REG_RCX, X.X86_REG_RDX, X.X86_REG_R8, X.X86_REG_R9, X.X86_REG_R10, X.X86_REG_R11}
SKIP_WRITE_REGS = set()


def reg64(reg):
    """Canonical 64-bit register for a sub-register id, or the id itself."""
    names = {
        "al": "rax", "ax": "rax", "eax": "rax", "bl": "rbx", "bx": "rbx", "ebx": "rbx", "cl": "rcx", "cx": "rcx", "ecx": "rcx",
        "dl": "rdx", "dx": "rdx", "edx": "rdx", "sil": "rsi", "si": "rsi", "esi": "rsi", "dil": "rdi", "di": "rdi", "edi": "rdi",
        "bpl": "rbp", "bp": "rbp", "ebp": "rbp",
    }
    return names, None


class Audit:
    def __init__(self, exe):
        pe = pefile.PE(str(exe))
        self.data = pe.get_memory_mapped_image()
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
        self.md.detail = True
        self.func_ends = sorted((e.struct.BeginAddress, e.struct.EndAddress) for e in pe.DIRECTORY_ENTRY_EXCEPTION)
        self.starts = [b for b, _ in self.func_ends]

    def function_range(self, rva):
        i = bisect.bisect_right(self.starts, rva) - 1
        if i >= 0 and self.func_ends[i][0] <= rva < self.func_ends[i][1]:
            return self.func_ends[i][1]
        return rva + 0x200                                                       # leaf function without unwind data

    def this_accesses(self, rva):
        """[(offset, width, is_write)] for [this + offset] memory operands. Empty if `this` is adjusted."""
        end = min(self.function_range(rva), rva + 0x1000)
        names = self.md.reg_name
        alias = {"rcx"}
        out, first = [], True
        for ins in self.md.disasm(self.data[rva:end], rva):
            if first:
                first = False
                # a method of a secondary base starts by moving `this` back to the object's start
                if ins.mnemonic == "lea" and re.match(r"r\w+, \[rcx - 0x", ins.op_str):
                    return []
            read, written = ins.regs_access()
            for op in ins.operands:
                if op.type == X.X86_OP_MEM and op.mem.index == 0 and op.mem.base and names(op.mem.base) in alias \
                        and ins.mnemonic not in ("lea", "nop", "prefetcht0", "prefetchw"):
                    is_write = bool(op.access & capstone.CS_AC_WRITE)
                    out.append((op.mem.disp, op.size, is_write))
            if ins.mnemonic == "mov" and len(ins.operands) == 2 and ins.operands[0].type == X.X86_OP_REG \
                    and ins.operands[1].type == X.X86_OP_REG and ins.operands[0].size == 8 \
                    and names(ins.operands[1].reg) in alias:
                alias.add(names(ins.operands[0].reg))                           # mov rbx, rcx
                continue
            if ins.mnemonic == "call":
                alias -= {"rax", "rcx", "rdx", "r8", "r9", "r10", "r11"}        # volatile registers die at a call
            for r in written:
                alias.discard(names(r).replace("e", "r", 1) if names(r).startswith("e") else names(r))
                for n in list(alias):
                    if r and names(r) in (n, n.replace("r", "e", 1)):
                        alias.discard(n)
            if not alias:
                break
            if ins.mnemonic == "ret":
                break
        return out


SCALAR = re.compile(r"^(?:const\s+)?(?:bool|char|signed char|unsigned char|short|unsigned short|int|unsigned int|unsigned|long|"
                    r"long long|float|double|u?int(?:8|16|32|64)_t|size_t|ptrdiff_t)$")


def is_scalar(type_name):
    t = type_name.strip()
    return t.endswith("*") or bool(SCALAR.match(t))


def build_map(L, name, base_off=0, out=None):
    """Flat list of (start, end, label, kind, scalar) for the class and its bases.
    kind: member | unmapped | vptr."""
    out = [] if out is None else out
    lay = L.layout(name)
    if lay.own_vptr:
        out.append((base_off, base_off + 8, "<vptr>", "vptr", True))
    for bn, off in lay.bases:
        build_map(L, bn, base_off + off, out)
    for f in lay.fields:
        kind = "unmapped" if (f.is_pad or f.name.startswith("_pad") or f.name == "_unmapped_tail") else "member"
        out.append((base_off + f.offset, base_off + f.offset + f.size, f"{name}::{f.name}", kind, is_scalar(f.type)))
    return sorted(out)


def classify(regions, size, off, width):
    """consistent | unmapped | inconsistent. See the module docstring."""
    if off + width > size:
        return "beyond"                                         # past what the bindings describe: unmapped trailing members
    hit = next((r for r in regions if r[0] <= off < r[1]), None)
    if hit is None or hit[3] == "unmapped":
        return "unmapped"                                       # a gap the bindings do not name
    start, end, _, _, scalar = hit
    if scalar and off != start:
        return "inconsistent"                                   # starts in the middle of a scalar member
    # the access may be wider than the member (a merged store of adjacent members); every byte of it
    # must still fall in named members that follow without a gap
    reach = end
    for r in regions:
        if r[0] == reach and r[3] in ("member", "vptr") and reach < off + width:
            reach = r[1]
    return "consistent" if reach >= off + width else "unmapped"


def secondary_virtuals(parser, cname):
    """Names of virtual functions that come from a base other than the primary chain. Their `this` points
    into the middle of the object, so their offsets cannot be checked against this class's layout."""
    names = set()

    def all_virtuals(c, seen):
        cls = parser.classes.get(c)
        if cls is None or c in seen:
            return
        seen.add(c)
        names.update(f.name for f in cls.functions if f.is_virtual)
        for b in cls.bases:
            all_virtuals(b.strip(), seen)

    def walk(c, primary):
        cls = parser.classes.get(c)
        if cls is None:
            return
        for i, b in enumerate(cls.bases):
            b = b.strip()
            if i == 0 and primary:
                walk(b, True)
            else:
                all_virtuals(b, set())
                walk(b, False)

    walk(cname, True)
    return names

def secondary_vtable_functions(exe):
    """RVAs of functions that sit directly in a secondary (non-zero this-offset) vtable. The game calls those
    with the secondary base's `this`, so [this + offset] there is relative to that base, not to the class."""
    import rtti_sizes as R
    img = R.Image(exe)
    td = R.type_descriptors(img)
    locs = R.object_locators(img, td)
    wanted = {col: t for t, lst in locs.items() for col, off in lst if off != 0}
    lo, hi = img.secs[".rdata"]
    text_lo, text_hi = img.secs[".text"]
    fns = set()
    for q in range(lo, hi - 8, 8):
        if (img.u64(q) - img.base) in wanted:
            slot = q + 8
            while slot + 8 <= hi:
                fn = img.u64(slot) - img.base
                if not (text_lo <= fn < text_hi):
                    break
                fns.add(fn)
                slot += 8
    return fns

def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("bindings")
    ap.add_argument("--version", default="2.2091")
    ap.add_argument("--exe")
    ap.add_argument("--worst", type=int, default=25)
    ap.add_argument("--class", dest="only")
    ap.add_argument("--json", help="write per-class results here")
    args = ap.parse_args()

    import layout, headergen
    from broma_parser import parse_version
    bindings = Path(args.bindings)
    parser = parse_version(bindings, args.version)
    enums = layout.parse_enums(bindings / "include" / "Geode" / "Enums.hpp")
    def factory():
        return layout.Layouter(parser.classes, enums, headergen.GEODE_RUNTIME_TYPES)

    # Audit the layout that is actually emitted: with the binary sizes applied (tail pads, corrections).
    sizes = {}
    for name in (f"class_sizes_rtti_{args.version}.json", f"class_sizes_{args.version}.json"):
        p = ROOT / "generated" / name
        if p.exists():
            sizes.update(json.load(open(p))["sizes"])
    headergen.apply_binary_sizes(parser.classes, factory, sizes)
    L = factory()
    exe = Path(args.exe) if args.exe else ROOT / "binaries" / args.version / "GeometryDash.exe"
    audit = Audit(exe)
    secondary_fns = secondary_vtable_functions(exe)
    addrs = json.load(open(ROOT / "generated" / f"addresses_{args.version}.json"))["addresses"]
    report = json.load(open(ROOT / "generated" / f"layout_report_{args.version}.json"))["classes"]

    results = {}
    for cname, cls in parser.classes.items():
        if args.only and cname != args.only:
            continue
        lay = L.layout(cname)
        if not lay.complete:
            continue
        regions = build_map(L, cname)
        skip_names = secondary_virtuals(parser, cname)
        counts = collections.Counter()
        bad = collections.Counter()
        for f in cls.functions:
            if f.is_static or not f.addrs.has_win() or f.name in skip_names:
                continue
            key = f.full_key()
            if key not in addrs:
                continue
            if int(addrs[key], 16) in secondary_fns:
                continue                                        # called with a secondary base's `this`
            seen = set()
            accesses = audit.this_accesses(int(addrs[key], 16))
            if any(o < 0 for o, _, _ in accesses):
                continue                                        # `this` is adjusted: a secondary-base method
            for off, width, _ in accesses:
                if (off, width) in seen:
                    continue
                seen.add((off, width))
                kind = classify(regions, lay.size, off, width)
                counts[kind] += 1
                if kind == "inconsistent":
                    bad[(f.name, off, width)] += 1
        if counts:
            results[cname] = {"status": report.get(cname, {}).get("status", "?"), "counts": dict(counts),
                              "inconsistent": [[n, hex(o), w] for (n, o, w) in sorted(bad)]}

    total = collections.Counter()
    for r in results.values():
        total.update(r["counts"])
    n_bad_cls = sum(1 for r in results.values() if r["counts"].get("inconsistent"))
    print(f"{len(results)} classes with at least one traceable [this+offset] access")
    print("accesses:", ", ".join(f"{k}={v}" for k, v in sorted(total.items())))
    print(f"classes with an inconsistent access: {n_bad_cls}")
    by_status = collections.defaultdict(lambda: collections.Counter())
    for r in results.values():
        by_status[r["status"]].update(r["counts"])
    for st, c in sorted(by_status.items()):
        print(f"  {st:12} " + ", ".join(f"{k}={v}" for k, v in sorted(c.items())))

    worst = sorted(((r["counts"].get("inconsistent", 0), n) for n, r in results.items() if r["counts"].get("inconsistent")), reverse=True)
    print(f"\nclasses with the most inconsistent accesses (first {args.worst}):")
    for cnt, n in worst[:args.worst]:
        r = results[n]
        total_n = sum(r["counts"].values())
        sample = ", ".join(f"{fn}@{off}/{w}" for fn, off, w in r["inconsistent"][:3])
        print(f"  {n}: {cnt}/{total_n} inconsistent [{r['status']}]  e.g. {sample}")
    if args.json:
        Path(args.json).write_text(json.dumps(results, indent=1, sort_keys=True))


if __name__ == "__main__":
    main()
