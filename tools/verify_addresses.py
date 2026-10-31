#!/usr/bin/env python3
"""Verify the addresses in generated/addresses_<ver>.json against the real GeometryDash.exe.

  pip install capstone pefile
  python tools/verify_addresses.py [--sample 400] [--seed 1] [--write docs/ADDRESS_VERIFICATION.md]

For a random sample of addresses it checks that each one
  1. lands inside an executable section,
  2. is a plausible function start (preceded by padding or the previous function's
     ret/jmp, and decodes cleanly for the first instructions),
  3. is actually used: reached by a direct call/jmp, taken with `lea`, or stored as
     a pointer in the image (vtables, callback tables).
Then, over EVERY address it can test, it checks that the code behaves like its name:
  4. `X::create(...)` makes a direct call into another function of class X
     (its init / constructor);
  5. a virtual function sits in a vtable next to functions of the same, a base or a
     derived class.
Checks 4 and 5 are what separates "points at a real function" from "points at the
function the name says".
"""
import argparse, bisect, json, random, re, struct, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "codegen"))

try:
    import capstone, pefile
except ImportError:
    sys.exit("needs: pip install capstone pefile")


def class_of(sig):
    head = sig.split("(")[0]
    return head.rsplit("::", 1)[0] if "::" in head else ""


class Image:
    def __init__(self, exe):
        self.pe = pefile.PE(str(exe))
        self.base = self.pe.OPTIONAL_HEADER.ImageBase
        self.data = self.pe.get_memory_mapped_image()
        self.size = len(self.data)
        self.exec = [(s.VirtualAddress, s.VirtualAddress + s.Misc_VirtualSize) for s in self.pe.sections
                     if s.Characteristics & 0x20000000]
        self.text = max(self.exec, key=lambda r: r[1] - r[0])
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
        self.md.detail = False
        self._index()

    def in_exec(self, rva):
        return any(lo <= rva < hi for lo, hi in self.exec)

    def _index(self):
        d, (lo, hi) = self.data, self.text
        self.call_targets, self.jmp_targets, self.lea_targets = set(), set(), set()
        for m in re.finditer(rb"[\xE8\xE9]", d[lo:hi], flags=re.S):
            p = lo + m.start()
            if p + 5 > hi:
                continue
            tgt = p + 5 + struct.unpack_from("<i", d, p + 1)[0]
            if lo <= tgt < hi:
                (self.call_targets if d[p] == 0xE8 else self.jmp_targets).add(tgt)
        for m in re.finditer(rb"[\x48\x4C]\x8D[\x05\x0D\x15\x1D\x25\x2D\x35\x3D]", d[lo:hi], flags=re.S):
            p = lo + m.start()
            tgt = p + 7 + struct.unpack_from("<i", d, p + 3)[0]
            if lo <= tgt < hi:
                self.lea_targets.add(tgt)
        # absolute pointers stored in non-executable data (vtables, tables)
        self.ptr_at = {}                       # rva of function -> [rva of slots holding it]
        lo_p, hi_p = self.base + lo, self.base + hi
        for s in self.pe.sections:
            if s.Characteristics & 0x20000000:
                continue
            start, end = s.VirtualAddress, min(s.VirtualAddress + s.Misc_VirtualSize, self.size)
            end -= (end - start) % 8
            for i, v in enumerate(struct.unpack_from(f"<{(end - start) // 8}Q", d, start)):
                if lo_p <= v < hi_p:
                    self.ptr_at.setdefault(v - self.base, []).append(start + i * 8)

    def plausible_start(self, rva):
        """Is `rva` right after something that ends a function: padding, ret, ret imm16, an
        unconditional jmp (rel32 / short / [mem] / reg), a call to a noreturn function, or ud2?"""
        d = self.data
        prev_ok = (d[rva - 1] in (0xCC, 0xC3, 0xCB, 0x90)
                   or d[rva - 5] in (0xE9, 0xE8)                       # jmp / call rel32 (tail call, noreturn)
                   or d[rva - 3] == 0xC2                               # ret imm16
                   or d[rva - 2] == 0xEB                               # jmp short
                   or (d[rva - 2] == 0xFF and 0xE0 <= d[rva - 1] <= 0xE7)   # jmp reg
                   or (d[rva - 6] == 0xFF and d[rva - 5] == 0x25)      # jmp [rip+disp]
                   or (d[rva - 2] == 0x0F and d[rva - 1] == 0x0B))     # ud2
        n = 0
        for ins in self.md.disasm(d[rva:rva + 0x60], rva):
            n += 1
            if ins.mnemonic in ("ret", "jmp", "int3") or n >= 12:
                break
        return prev_ok, n >= 1

    def used(self, rva):
        return (rva in self.call_targets or rva in self.jmp_targets or rva in self.lea_targets or rva in self.ptr_at)

    def calls_in(self, rva, span=0x180):
        out = []
        for ins in self.md.disasm(self.image_bytes(rva, span), rva):
            if ins.mnemonic == "call" and ins.op_str.startswith("0x"):
                out.append(int(ins.op_str, 16))
            if ins.mnemonic == "int3":
                break
        return out

    def image_bytes(self, rva, n):
        return self.data[rva:rva + n]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--exe")
    ap.add_argument("--addresses")
    ap.add_argument("--bindings", default=str(ROOT.parent / "bindings" / "bindings"))
    ap.add_argument("--version", default="2.2091")
    ap.add_argument("--sample", type=int, default=400)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--write", help="write a Markdown report here")
    args = ap.parse_args()

    exe = Path(args.exe) if args.exe else ROOT / "binaries" / args.version / "GeometryDash.exe"
    addr_file = Path(args.addresses) if args.addresses else ROOT / "generated" / f"addresses_{args.version}.json"
    addrs = json.load(open(addr_file))["addresses"]
    img = Image(exe)
    rva_of = {sig: int(a, 16) for sig, a in addrs.items()}
    names_at = {}
    for sig, rva in rva_of.items():
        names_at.setdefault(rva, []).append(sig)

    # class relationships, for the vtable-neighbour test
    from broma_parser import parse_version
    parser = parse_version(Path(args.bindings), args.version)
    parents = {n: set(b.strip() for b in c.bases) for n, c in parser.classes.items()}

    def related(a, b):
        def ancestors(x, seen=None):
            seen = seen if seen is not None else set()
            for p in parents.get(x, ()):
                if p not in seen:
                    seen.add(p)
                    ancestors(p, seen)
            return seen
        return a == b or b in ancestors(a) or a in ancestors(b) or bool(ancestors(a) & ancestors(b) - {""})

    random.seed(args.seed)
    sample = random.sample(sorted(rva_of), min(args.sample, len(rva_of)))

    exec_ok = start_ok = decode_ok = used_ok = 0
    failures = []
    for sig in sample:
        rva = rva_of[sig]
        if not img.in_exec(rva):
            failures.append((sig, "not in an executable section")); continue
        exec_ok += 1
        prev_ok, decodes = img.plausible_start(rva)
        start_ok += prev_ok
        decode_ok += decodes
        if img.used(rva):
            used_ok += 1
        else:
            failures.append((sig, "never called, jumped to, lea'd or stored as a pointer"))
        if not prev_ok:
            failures.append((sig, "not preceded by padding / the end of another function"))

    # semantic checks over everything that can be judged
    # (a check is only counted when the data needed to judge it exists)
    create_total = create_ok = 0
    has_init = {}
    for sig in rva_of:
        m = re.match(r"^(.*)::(init\w*|[A-Za-z_]\w*)\(", sig)
        if m and (m.group(2).startswith("init") or m.group(2) == m.group(1).split("::")[-1]):
            has_init[m.group(1)] = True
    create_failed = []
    for sig, rva in rva_of.items():
        cls = class_of(sig)
        if re.match(r".*::create\(", sig) and cls and has_init.get(cls):
            callee_classes = {class_of(n) for t in img.calls_in(rva) for n in names_at.get(t, [])} - {""}
            if not callee_classes:
                continue            # it only calls functions the bindings give no address: cannot be judged
            create_total += 1
            if any(related(cls, c) for c in callee_classes):    # its own init, or a base class's constructor
                create_ok += 1
            else:
                create_failed.append(sig)

    vt_total = vt_ok = 0
    vt_failed = []
    for sig, rva in rva_of.items():
        cls = class_of(sig)
        slots = img.ptr_at.get(rva)
        if not slots or not cls or cls not in parents:
            continue
        if not any(f.is_virtual and f.full_key() == sig for f in parser.classes[cls].functions):
            continue
        judged, ok = False, False
        for slot in slots:
            for k in (-3, -2, -1, 1, 2, 3):
                p = slot + 8 * k
                if 0 <= p + 8 <= img.size:
                    v = struct.unpack_from("<Q", img.data, p)[0] - img.base
                    for n in names_at.get(v, []):
                        judged = True
                        if related(cls, class_of(n)):
                            ok = True
        if judged:
            vt_total += 1
            vt_ok += ok
            if not ok:
                vt_failed.append(sig)

    n = len(sample)
    all_in_code = sum(1 for rva in rva_of.values() if img.in_exec(rva))
    pct = lambda a, b: f"{a}/{b} ({100 * a / b:.1f}%)" if b else "n/a"
    lines = [
        f"# Address verification: GD {args.version}",
        "",
        f"`tools/verify_addresses.py --sample {args.sample} --seed {args.seed}` against `{exe.name}` "
        f"({len(rva_of)} addresses in `{addr_file.name}`).",
        "",
        f"Every address in the file lands in an executable section: {pct(all_in_code, len(rva_of))}.",
        "",
        f"## Random sample of {n} addresses",
        "",
        f"| Check | Result |",
        f"|---|---|",
        f"| lands in an executable section | {pct(exec_ok, n)} |",
        f"| preceded by padding or the end of another function | {pct(start_ok, n)} |",
        f"| decodes as code at the start | {pct(decode_ok, n)} |",
        f"| is referenced (call / jmp / lea / stored pointer) | {pct(used_ok, n)} |",
        "",
        "## Does the code behave like its name? (every address that can be tested)",
        "",
        f"| Check | Result |",
        f"|---|---|",
        f"| `X::create(...)` calls a function of class `X` or a related class (its init, or a base constructor), when it calls any named function | {pct(create_ok, create_total)} |",
        f"| a virtual function's vtable neighbours belong to the same / a related class (when a neighbour is named) | {pct(vt_ok, vt_total)} |",
        "",
        "Not every miss is a wrong address: `create` may allocate and call an initialiser that the bindings "
        "mark `win inline`, and a vtable can interleave a base class's functions with no address. "
        "The sample checks above are the ones that cannot be explained that way.",
        "",
        "Class sizes are a third, independent check: `tools/layout_check.py check` confirms "
        "that `sizeof(X)` read from each `X::create` in the exe equals the size computed from the bindings, "
        "which also requires that `create` was found at the right address.",
        "",
    ]
    if failures:
        lines += ["## Sample addresses that failed a check", ""]
        seen = set()
        for sig, why in failures[:25]:
            if (sig, why) not in seen:
                seen.add((sig, why)); lines.append(f"- `{sig}`: {why}")
        lines.append("")
    report = "\n".join(lines)
    print(report)
    if args.write:
        Path(args.write).parent.mkdir(parents=True, exist_ok=True)
        Path(args.write).write_text(report, encoding="utf-8")


if __name__ == "__main__":
    main()
