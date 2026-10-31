#!/usr/bin/env python3
"""Measure gdport against ground truth.

    python selftest.py <GeometryDash.exe> <addresses.json> [--mutate-percent 30] [--seed 1]

Two tests, both with a known right answer (every function maps to its own address):

  identity  the new exe is the old exe. Anything but "correct" or "left unmatched" is a bug,
            and "wrong" must be zero.
  mutated   the new exe is a copy where one instruction in N% of the known functions has been
            changed (an immediate operand, so sizes and layout stay put). Their exact hashes
            no longer match, so they have to be recovered by shape, strings, the call graph or
            layout order, and must still never be wrong.

Exit code 0 only if no function was placed at a wrong address in either test.
"""
import argparse
import json
import random
import struct
import sys
import tempfile
from pathlib import Path

import capstone

from gdport import BRANCHES, Image, port

MUTABLE = {"mov", "add", "sub", "cmp", "and", "or", "xor", "test", "imul", "shl", "shr", "sar"}


def section_map(raw):
    e = struct.unpack_from("<I", raw, 0x3C)[0]
    nsec, = struct.unpack_from("<H", raw, e + 6)
    opt_size, = struct.unpack_from("<H", raw, e + 20)
    sec = e + 24 + opt_size
    out = []
    for i in range(nsec):
        vsize, va, rawsize, rawoff = struct.unpack_from("<IIII", raw, sec + i * 40 + 8)
        out.append((va, vsize, rawsize, rawoff))
    return out


def mutate(exe_path, addresses, percent, seed, out_path):
    raw = bytearray(Path(exe_path).read_bytes())
    sections = section_map(raw)
    img = Image(exe_path)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_64)
    md.detail = True
    rng = random.Random(seed)
    rvas = sorted(set(int(a, 16) for a in addresses.values()))
    chosen = rng.sample(rvas, len(rvas) * percent // 100)
    nxt = {rvas[i]: rvas[i + 1] for i in range(len(rvas) - 1)}
    changed = set()
    for rva in chosen:
        end = min(nxt.get(rva, rva + 0x200), rva + 0x2000)
        code = bytes(img.mem[rva:end])
        for ins in md.disasm(code, rva):
            if ins.mnemonic in MUTABLE and ins.imm_size in (1, 4) and not (ins.disp_size == 4 and (ins.modrm & 0xC7) == 0x05) \
                    and ins.mnemonic not in BRANCHES:
                target_rva = ins.address + ins.imm_offset
                for va, vsize, rawsize, rawoff in sections:
                    if va <= target_rva < va + min(vsize, rawsize):
                        raw[rawoff + (target_rva - va)] ^= 0x01
                        changed.add(rva)
                        break
                break
    Path(out_path).write_bytes(bytes(raw))
    return changed


def score(name, addrs_old, result, unmatched, changed=None):
    right = sum(1 for s, r in result.items() if addrs_old[s] == r)
    wrong = [s for s, r in result.items() if addrs_old[s] != r]
    print(f"\n[{name}]  functions {len(addrs_old)}: correct {right}, WRONG {len(wrong)}, left unmatched {len(unmatched)}")
    if changed is not None:
        mut = [s for s, a in addrs_old.items() if a in changed]
        ok = sum(1 for s in mut if result.get(s) == addrs_old[s])
        print(f"    of the {len(mut)} functions that were altered: recovered {ok} ({100 * ok / max(1, len(mut)):.1f}%)")
    for s in wrong[:8]:
        print(f"    WRONG {s}: truth {addrs_old[s]:#x}, gdport {result[s]:#x}")
    return len(wrong)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("exe")
    ap.add_argument("addresses")
    ap.add_argument("--mutate-percent", type=int, default=30)
    ap.add_argument("--seed", type=int, default=1)
    args = ap.parse_args()

    old = json.load(open(args.addresses))["addresses"]
    truth = {s: int(a, 16) for s, a in old.items()}
    quiet = lambda *_: None

    print("identity test...")
    res, unm, methods, _ = port(args.exe, old, args.exe, quiet)
    bad = score("identity", truth, res, unm)
    print(f"    by method: {methods}")

    print("\nmutated test...")
    with tempfile.TemporaryDirectory() as d:
        mutated = Path(d) / "GeometryDash.mutated.exe"
        changed = mutate(args.exe, old, args.mutate_percent, args.seed, mutated)
        print(f"    altered one instruction in {len(changed)} functions")
        res, unm, methods, _ = port(args.exe, old, str(mutated), quiet)
        bad += score("mutated", truth, res, unm, changed)
        print(f"    by method: {methods}")

    print("\nPASS: nothing placed at a wrong address" if not bad else f"\nFAIL: {bad} wrong placements")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
