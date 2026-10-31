#!/usr/bin/env python3
"""Read class sizes out of GeometryDash.exe from its RTTI and vtables.

  pip install capstone pefile
  python tools/rtti_sizes.py <bindings> [--version 2.2091] [--show ClassName ...]

A second, independent source of sizeof(Class), next to `layout_check.py harvest`
(which reads the `mov ecx, size; call operator new` at the top of `X::create`).
This one covers every polymorphic class, including the many with no `create`:

  1. MSVC emits an RTTI type descriptor (".?AVName@@") for each polymorphic class.
  2. A Complete Object Locator points at it, and the class's primary vtable sits right
     after a pointer to that locator.
  3. One of the first four vtable slots is the deleting destructor (slot 0, or slot 1 for many
     classes derived from cocos2d, where slot 0 is an import thunk). Its tail is
         test  dil, 1 ; je skip ; mov edx, <sizeof> ; mov rcx, rbx ; call operator delete
     so the immediate loaded into edx is sizeof(Class).

Only classes where the shape matches, and every match agrees, are reported. The result is
Tried and rejected: reading sizeof from `mov ecx, SIZE ; call operator new ; ... ; call X::X` call sites. A derived\nclass's constructor calls its base class's constructor right after its own allocation, so the base class is reported\nwith the derived class's size (GameManager came out as 0x20 against the real 0x6e8); it disagreed with the vtable\nsizes on 5 of the 23 classes both covered.\n\nDo not raise --slots: scanning 8 or 16 slots picks up unrelated functions with the same shape and\nreturns wrong sizes (four classes disagreed with `create`). The result is written to generated/class_sizes_rtti_<ver>.json and compared with (a) the sizes harvested
from `create` and (b) the sizes the bindings imply, so a disagreement is reported instead
of trusted.
"""
import argparse, collections, json, re, struct, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "codegen"))

try:
    import capstone, pefile
except ImportError:
    sys.exit("needs: pip install capstone pefile")


def pretty(mangled):
    """'CCNode@cocos2d' -> 'cocos2d::CCNode'. Template instances (with '?$') are skipped by the caller."""
    return "::".join(reversed([p for p in mangled.split("@") if p]))


class Image:
    def __init__(self, exe):
        pe = pefile.PE(str(exe))
        self.data = pe.get_memory_mapped_image()
        self.base = pe.OPTIONAL_HEADER.ImageBase
        self.secs = {s.Name.rstrip(b"\0").decode(): (s.VirtualAddress, s.VirtualAddress + s.Misc_VirtualSize)
                     for s in pe.sections}
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)

    def u32(self, rva):
        return struct.unpack_from("<I", self.data, rva)[0]

    def u64(self, rva):
        return struct.unpack_from("<Q", self.data, rva)[0]


def type_descriptors(img):
    """rva of TypeDescriptor -> mangled class name (without '.?AV' and '@@')."""
    out = {}
    for m in re.finditer(rb"\.\?AV([A-Za-z0-9_@]+)@@\x00", img.data):   # plain names; templates contain '?$'
        out[m.start() - 0x10] = m.group(1).decode()                     # name starts 0x10 into the descriptor
    return out


def object_locators(img, td):
    """(locator rva, this-offset) for every x64 Complete Object Locator that names a known descriptor."""
    lo, hi = img.secs[".rdata"]
    found = collections.defaultdict(list)
    for q in range(lo, hi - 24, 4):
        if img.u32(q) == 1 and img.u32(q + 20) == q:                    # signature 1, pSelf == own rva
            t = img.u32(q + 12)
            if t in td:
                found[t].append((q, img.u32(q + 4)))
    return found


def primary_vtables(img, td, locators):
    """class name -> rva of the first vtable entry of its primary (offset 0) vtable."""
    wanted = {}
    for t, lst in locators.items():
        for col, offset in lst:
            if offset == 0:
                wanted[col] = t
    lo, hi = img.secs[".rdata"]
    vt = {}
    for q in range(lo, hi - 8, 8):
        r = img.u64(q) - img.base
        t = wanted.get(r)
        if t is not None:
            vt.setdefault(td[t], q + 8)
    return vt


def deleting_dtor_size(img, fn_rva, operator_delete=None):
    """sizeof read from a vtable-slot-0 function, or None if it does not have the expected shape."""
    code = img.data[fn_rva:fn_rva + 0xA0]
    ins = list(img.md.disasm(code, fn_rva))
    if ins and ins[0].mnemonic == "jmp" and ins[0].op_str.startswith("0x"):       # thunk to the real one
        return deleting_dtor_size(img, int(ins[0].op_str, 16), operator_delete)
    sizes = set()
    for i, x in enumerate(ins):
        if x.mnemonic == "test" and re.fullmatch(r"\w+, 1", x.op_str):
            window = ins[i + 1:i + 6]
            for j, y in enumerate(window):
                m = re.fullmatch(r"edx, (0x[0-9a-f]+)", y.op_str) if y.mnemonic == "mov" else None
                if not m:
                    continue
                # `mov edx, SIZE ; mov rcx, this ; call operator delete`: the call target is what separates this
                # from any other function that happens to test a flag and load edx.
                if operator_delete is None or any(
                        c.mnemonic == "call" and c.op_str == hex(operator_delete) for c in window[j + 1:j + 4]):
                    sizes.add(int(m.group(1), 16))
    return sizes.pop() if len(sizes) == 1 else None


def harvest(img, slots=4, operator_delete=None):
    td = type_descriptors(img)
    locs = object_locators(img, td)
    vts = primary_vtables(img, td, locs)
    sizes, no_shape, ambiguous = {}, [], {}
    text_lo, text_hi = img.secs[".text"]
    for mangled, vt in vts.items():
        name = pretty(mangled)
        # The deleting destructor is slot 0 for plain classes, but slot 1 for many classes derived from
        # cocos2d (slot 0 is an import thunk), so look at the first few slots. Every match must agree.
        found = set()
        for slot in range(slots):
            fn = img.u64(vt + 8 * slot) - img.base
            if text_lo <= fn < text_hi:
                size = deleting_dtor_size(img, fn, operator_delete)
                if size is not None:
                    found.add(size)
        if len(found) == 1:
            sizes[name] = found.pop()
        elif found:
            ambiguous[name] = sorted(found)
        else:
            no_shape.append(name)
    return len(td), len(vts), sizes, no_shape, ambiguous


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("bindings", help="path to geode-sdk/bindings' bindings/ folder")
    ap.add_argument("--version", default="2.2091")
    ap.add_argument("--exe")
    ap.add_argument("--slots", type=int, default=4, help="how many vtable slots to look in (default 4)")
    ap.add_argument("--operator-delete", default="0", help="rva of operator delete(void*, size_t) to pin the call target to. Off by default: 'the address is specific to one GD version and gave no benefit at 4 slots'")
    ap.add_argument("--show", nargs="*", default=[], help="print what each of these classes resolved to")
    args = ap.parse_args()

    exe = Path(args.exe) if args.exe else ROOT / "binaries" / args.version / "GeometryDash.exe"
    img = Image(exe)
    n_td, n_vt, sizes, no_shape, ambiguous = harvest(img, args.slots, int(args.operator_delete, 16) or None)
    print(f"{n_td} RTTI type descriptors, {n_vt} primary vtables: {len(sizes)} sizes read from a deleting "
          f"destructor, {len(ambiguous)} ambiguous (ignored), {len(no_shape)} with none in the first slots")

    out = ROOT / "generated" / f"class_sizes_rtti_{args.version}.json"
    if args.slots == 4:
        out.write_text(json.dumps({"version": args.version, "sizes": sizes}, indent=1, sort_keys=True))
        print(f"-> {out}")
    else:
        print("(experiment: --slots / --operator-delete differ from the validated defaults, so nothing is written)")

    import layout, headergen
    from broma_parser import parse_version
    bindings = Path(args.bindings)
    parser = parse_version(bindings, args.version)
    enums = layout.parse_enums(bindings / "include" / "Geode" / "Enums.hpp")
    L = layout.Layouter(parser.classes, enums, headergen.GEODE_RUNTIME_TYPES)
    create = json.load(open(ROOT / "generated" / f"class_sizes_{args.version}.json"))["sizes"]

    # 1. Do the two binary sources agree? (If they do not, this method is not trustworthy.)
    both = [c for c in sizes if c in create]
    agree = [c for c in both if sizes[c] == create[c]]
    print(f"\nvs the sizes harvested from X::create: {len(both)} classes in both, {len(agree)} agree")
    for c in both:
        if sizes[c] != create[c]:
            print(f"  DISAGREE {c}: deleting destructor says {sizes[c]:#x}, create says {create[c]:#x}")

    # 2. What the bindings imply, for every class we have a binary size for.
    exact = smaller = larger = skipped = 0
    new_exact = []
    for c, want in sorted(sizes.items()):
        if c not in parser.classes:
            continue
        lay = L.layout(c)
        if not lay.complete:
            skipped += 1
        elif lay.size == want:
            exact += 1
            if c not in create:
                new_exact.append(c)
        elif lay.size < want:
            smaller += 1
        else:
            larger += 1
            print(f"  CONFLICT {c}: bindings describe {lay.size:#x} but the binary's class is {want:#x}")
    print(f"\nvs the bindings ({exact + smaller + larger + skipped} classes with a binary size): "
          f"{exact} exact, {smaller} smaller than the binary, {larger} larger, {skipped} not computable")
    print(f"  of the exact ones, {len(new_exact)} are classes not verified by the create() source")

    for name in args.show:
        print(f"\n{name}: rtti={sizes.get(name)} create={create.get(name)} "
              f"computed={L.layout(name).size if name in parser.classes and L.layout(name).complete else None}")


if __name__ == "__main__":
    main()
