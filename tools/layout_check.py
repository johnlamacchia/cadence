#!/usr/bin/env python3
"""Check the class layouts computed from the bindings against the game binary.

  pip install capstone pefile

  python tools/layout_check.py harvest          # read sizeof(Class) out of GeometryDash.exe
  python tools/layout_check.py check <bindings> # compare computed layouts with the binary

`harvest` finds every `X::create(...)` in the exe, whose first act is
`mov ecx, sizeof(X); call operator new`, and records sizeof(X) in
generated/class_sizes_<ver>.json. codegen uses those sizes to pad classes whose
bindings are incomplete and to mark which layouts are verified.

`check` computes layouts from the Broma files and reports, per class that has a
harvested size, whether the computed size equals the binary's, plus a set of
member-offset anchors read from machine code (see ANCHORS).
"""
import argparse, collections, json, re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "codegen"))

try:
    import capstone, pefile
except ImportError:
    sys.exit("needs: pip install capstone pefile")

# (class, member, offset). Each offset was read from the game's machine code:
# a function that obviously uses the field loads/stores [this+offset].
ANCHORS = [
    ("GameManager", "m_playLayer", 0x230),              # set in PlayLayer::init, cleared in ~PlayLayer
    ("GJBaseGameLayer", "m_isPracticeMode", 0x32E0),    # tested by PlayLayer::togglePracticeMode / startMusic
    ("GJBaseGameLayer", "m_debugDrawNode", 0x3870),     # CCDrawNode::clear target in updateDebugDraw / toggleDebugDraw
    ("UILayer", "m_pauseBtn", 0x238),                   # UILayer::toggleMenuVisibility
    ("UILayer", "m_gameLayer", 0x258),                  # stored in UILayer::init
    ("UILayer", "m_initialized", 0x260),
    ("UILayer", "m_uiNodes", 0x268),
    ("UILayer", "m_controllerButtons", 0x278),
]


def newest(pattern_dir, pattern):
    return max(Path(pattern_dir).glob(pattern))


def harvest(args):
    exe = Path(args.exe) if args.exe else newest(ROOT / "binaries", "*/GeometryDash.exe")
    addr_file = Path(args.addresses) if args.addresses else newest(ROOT / "generated", "addresses_*.json")
    version = addr_file.stem.replace("addresses_", "")
    image = pefile.PE(str(exe)).get_memory_mapped_image()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
    addrs = json.load(open(addr_file))["addresses"]

    found = collections.defaultdict(list)
    for sig, a in addrs.items():
        m = re.match(r"^([\w:]+)::create\(", sig)
        if not m:
            continue
        rva = int(a, 16)
        ins = list(md.disasm(image[rva:rva + 0x60], rva))
        for i, x in enumerate(ins[:14]):
            if x.mnemonic == "mov" and x.op_str.startswith("ecx, 0x"):
                imm = int(x.op_str.split(",")[1].strip(), 16)
                for y in ins[i + 1:i + 3]:
                    if y.mnemonic == "call" and y.op_str.startswith("0x"):
                        found[m.group(1)].append((imm, int(y.op_str, 16)))
                        break
                break
    targets = collections.Counter(t for v in found.values() for _, t in v)
    operator_new = targets.most_common(1)[0][0]
    sizes = {c: next(imm for imm, t in v if t == operator_new) for c, v in found.items() if any(t == operator_new for _, t in v)}
    out = ROOT / "generated" / f"class_sizes_{version}.json"
    out.write_text(json.dumps({"version": version, "operator_new": hex(operator_new), "sizes": sizes}, indent=1, sort_keys=True))
    print(f"{len(sizes)} class sizes (operator new at {operator_new:#x}) -> {out}")


def check(args):
    import layout, headergen
    from broma_parser import parse_version
    bindings = Path(args.bindings)
    parser = parse_version(bindings, args.version)
    enums = layout.parse_enums(bindings / "include" / "Geode" / "Enums.hpp")
    sizes = json.load(open(ROOT / "generated" / f"class_sizes_{args.version}.json"))["sizes"]

    L = layout.Layouter(parser.classes, enums, headergen.GEODE_RUNTIME_TYPES)
    complete = sum(1 for c in parser.classes if L.layout(c).complete)
    print(f"layouts computed for {complete}/{len(parser.classes)} classes")

    ok = tail = conflict = skipped = 0
    for name, want in sorted(sizes.items()):
        lay = L.layout(name)
        if not lay.complete:
            skipped += 1
        elif lay.size == want:
            ok += 1
        elif lay.size < want:
            tail += 1
        else:
            conflict += 1
            print(f"  CONFLICT {name}: bindings describe {lay.size:#x} but the binary's class is {want:#x}")
    print(f"sizeof vs binary ({len(sizes)} classes): {ok} exact, {tail} smaller than the binary (bindings omit trailing members), "
          f"{conflict} larger, {skipped} not computable")

    bad = 0
    for cls, member, want in ANCHORS:
        got = L.offset_of(cls, member)
        flag = "OK " if got == want else "BAD"
        bad += got != want
        print(f"  {flag} {cls}::{member} binary {want:#x} computed {('%#x' % got) if got is not None else 'n/a'}")
    sys.exit(1 if bad or conflict else 0)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    h = sub.add_parser("harvest")
    h.add_argument("--exe"); h.add_argument("--addresses")
    h.set_defaults(fn=harvest)
    c = sub.add_parser("check")
    c.add_argument("bindings", help="path to geode-sdk/bindings' bindings/ folder")
    c.add_argument("--version", default="2.2091")
    c.set_defaults(fn=check)
    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
