#!/usr/bin/env python3
"""Find which [reg+disp] operands a function uses: how to discover a class member's
offset straight from the game's machine code.

  pip install capstone pefile
  python tools/offset_probe.py 0x3000 0x3700 "PlayLayer::togglePracticeMode" "PlayLayer::startMusic"

Prints every memory operand with a displacement in [LO, HI] in each function whose
signature starts with the given text. A field that two functions both obviously use
(a setter and a getter, say) shows up as the same displacement in both.

You rarely need this: the generated headers give every member's offset, and
`tools/layout_check.py check` confirms them against the binary. Use this to
investigate a class the bindings do not describe fully (a `tail-padded` class in
generated/layout_report_<ver>.json) or to double-check a suspicious offset.
"""
import json, sys
from pathlib import Path

try:
    import capstone, pefile
except ImportError:
    sys.exit("needs: pip install capstone pefile")

ROOT = Path(__file__).resolve().parent.parent


def main():
    if len(sys.argv) < 4:
        sys.exit(__doc__)
    lo, hi, prefixes = int(sys.argv[1], 16), int(sys.argv[2], 16), sys.argv[3:]
    exe = max((ROOT / "binaries").glob("*/GeometryDash.exe"))
    addr_file = max((ROOT / "generated").glob("addresses_*.json"))
    image = pefile.PE(str(exe)).get_memory_mapped_image()
    addrs = json.load(open(addr_file))["addresses"]
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
    md.detail = True
    skip = (0, capstone.x86.X86_REG_RIP, capstone.x86.X86_REG_RSP, capstone.x86.X86_REG_RBP)

    for prefix in prefixes:
        for sig, a in sorted(addrs.items()):
            if not sig.startswith(prefix):
                continue
            rva = int(a, 16)
            print(f"== {sig} @ {rva:#x}")
            seen = {}
            for ins in md.disasm(image[rva:rva + 0x1800], rva):
                if ins.mnemonic == "int3":
                    break
                for op in ins.operands:
                    if op.type == capstone.x86.X86_OP_MEM and op.mem.base not in skip and lo <= op.mem.disp <= hi:
                        seen.setdefault(op.mem.disp, []).append(f"{ins.address:#x}: {ins.mnemonic} {ins.op_str}")
            for disp, uses in sorted(seen.items()):
                print(f"  {disp:#x} ({len(uses)}x)  {uses[0]}")


if __name__ == "__main__":
    main()
