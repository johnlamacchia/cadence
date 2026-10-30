#!/usr/bin/env python3
"""Check Cadence's hardcoded GD field offsets against the real binary.

The generated headers list fields in the right order but do NOT give reliable
byte offsets, so every offset in loader/include/sdk.h (cadence::off) is read
out of the game's machine code instead. This tool re-does that check:

    pip install capstone pefile
    python tools/offset_probe.py --verify
    python tools/offset_probe.py --scan 0x3000 0x3700 "PlayLayer::togglePracticeMode"

--verify  confirms each cadence::off value shows up as a [reg+disp] operand in
          functions that obviously use that field. Run it after a GD update.
--scan    prints every [reg+disp] in a displacement range for functions whose
          signature starts with the given text, to find a field's new offset.
"""
import argparse, json, sys
from pathlib import Path

try:
    import capstone, pefile
except ImportError:
    sys.exit("needs: pip install capstone pefile")

ROOT = Path(__file__).resolve().parent.parent

# (cadence::off name, offset, [functions that must reference it])
EXPECTED = [
    ("game_manager_play_layer",     0x230,  ["PlayLayer::~PlayLayer", "PlayLayer::init"]),
    ("play_layer_is_practice_mode", 0x32E0, ["PlayLayer::togglePracticeMode", "PlayLayer::startMusic"]),
    ("ui_layer_pause_btn",          0x238,  ["UILayer::toggleMenuVisibility", "UILayer::init"]),
    ("ui_layer_game_layer",         0x258,  ["UILayer::init"]),
]


class Binary:
    def __init__(self, exe, addresses):
        self.image = pefile.PE(str(exe)).get_memory_mapped_image()
        data = json.load(open(addresses))
        self.addrs = data.get("addresses", data)
        self.md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
        self.md.detail = True

    def find(self, prefix):
        return [(k, int(v, 16)) for k, v in self.addrs.items() if k.startswith(prefix)]

    def displacements(self, rva, size=0x1800):
        out = {}
        for ins in self.md.disasm(self.image[rva:rva + size], rva):
            if ins.mnemonic == "int3":
                break
            for op in ins.operands:
                if op.type == capstone.x86.X86_OP_MEM and op.mem.base not in (
                        0, capstone.x86.X86_REG_RIP, capstone.x86.X86_REG_RSP, capstone.x86.X86_REG_RBP):
                    out.setdefault(op.mem.disp, []).append(f"{ins.address:#x}: {ins.mnemonic} {ins.op_str}")
        return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--exe", help="GeometryDash.exe (default: newest under binaries/)")
    ap.add_argument("--addresses", help="addresses_<ver>.json (default: newest under generated/)")
    ap.add_argument("--verify", action="store_true")
    ap.add_argument("--scan", nargs="+", metavar=("LO", "HI"), help="LO HI SIGNATURE_PREFIX...")
    args = ap.parse_args()

    exe = Path(args.exe) if args.exe else max((ROOT / "binaries").glob("*/GeometryDash.exe"))
    addresses = Path(args.addresses) if args.addresses else max((ROOT / "generated").glob("addresses_*.json"))
    b = Binary(exe, addresses)
    print(f"binary: {exe}\naddresses: {addresses.name}")

    if args.scan:
        lo, hi, *sigs = args.scan
        lo, hi = int(lo, 16), int(hi, 16)
        for prefix in sigs:
            for name, rva in b.find(prefix):
                print(f"== {name} @ {rva:#x}")
                for disp, uses in sorted(b.displacements(rva).items()):
                    if lo <= disp <= hi:
                        print(f"  {disp:#x} ({len(uses)}x)  {uses[0]}")

    if args.verify:
        bad = 0
        for field, offset, funcs in EXPECTED:
            for prefix in funcs:
                hits = b.find(prefix)
                if not hits:
                    print(f"?? {field}: no address for '{prefix}'"); bad += 1; continue
                found = any(offset in b.displacements(rva) for _, rva in hits)
                print(f"{'OK ' if found else 'BAD'} {field} = {offset:#x}  in {prefix}")
                bad += not found
        print("all offsets verified" if not bad else f"{bad} check(s) failed: update cadence::off in sdk.h")
        sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
