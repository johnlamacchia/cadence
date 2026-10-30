"""
gdport — carry GD function addresses from one exe version to the next.

Fingerprints functions by strings, byte patterns (addresses masked),
opcode shapes, and call graph, then matches against the new binary.
"""
import argparse
import json
import struct
import re
import sys
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path


@dataclass
class Function:
    rva: int
    size: int
    strings: list[str] = field(default_factory=list)
    masked_bytes: bytes = b""
    opcodes: list[int] = field(default_factory=list)
    call_targets: list[int] = field(default_factory=list)
    callers: list[int] = field(default_factory=list)


def read_pe(path: str) -> tuple[bytes, int, list[tuple[int, int, int]]]:
    """Read PE file, return (data, image_base, sections[(rva, vsize, raw_off)])."""
    data = open(path, "rb").read()
    e_lfanew = struct.unpack_from("<I", data, 0x3c)[0]
    sig = e_lfanew
    file_hdr = sig + 4
    num_sections = struct.unpack_from("<H", data, file_hdr + 2)[0]
    opt_hdr_off = file_hdr + 20
    magic = struct.unpack_from("<H", data, opt_hdr_off)[0]
    if magic == 0x20b:  # PE32+
        image_base = struct.unpack_from("<Q", data, opt_hdr_off + 24)[0]
    else:
        image_base = struct.unpack_from("<I", data, opt_hdr_off + 28)[0]

    opt_hdr_size = struct.unpack_from("<H", data, file_hdr + 16)[0]
    sections_off = opt_hdr_off + opt_hdr_size
    sections = []
    for i in range(num_sections):
        s = sections_off + i * 40
        vsize = struct.unpack_from("<I", data, s + 8)[0]
        rva = struct.unpack_from("<I", data, s + 12)[0]
        raw_size = struct.unpack_from("<I", data, s + 16)[0]
        raw_off = struct.unpack_from("<I", data, s + 20)[0]
        sections.append((rva, vsize, raw_off))
    return data, image_base, sections


def rva_to_offset(rva: int, sections: list) -> int:
    for s_rva, s_vsize, s_raw in sections:
        if s_rva <= rva < s_rva + s_vsize:
            return s_raw + (rva - s_rva)
    return -1


def find_strings_at(data: bytes, sections: list, rva: int, size: int) -> list[str]:
    """Find string references in a function's bytes."""
    strings = []
    off = rva_to_offset(rva, sections)
    if off < 0:
        return strings

    func_bytes = data[off:off + size]
    # Look for LEA instructions with RIP-relative addressing that point to .rdata
    # Pattern: 48 8D xx xx xx xx xx (LEA reg, [rip+disp32])
    for i in range(len(func_bytes) - 6):
        # LEA with RIP-relative: 48 8D [0x05,0x0D,0x15,0x1D,0x25,0x2D,0x35,0x3D] + disp32
        if func_bytes[i] in (0x48, 0x4C) and func_bytes[i + 1] == 0x8D:
            modrm = func_bytes[i + 2]
            if (modrm & 0xC7) == 0x05:  # mod=00, rm=101 (RIP-relative)
                disp = struct.unpack_from("<i", func_bytes, i + 3)[0]
                target_rva = rva + i + 7 + disp
                target_off = rva_to_offset(target_rva, sections)
                if 0 <= target_off < len(data):
                    # Try to read a string
                    end = target_off
                    while end < len(data) and end - target_off < 256 and data[end] != 0:
                        if data[end] < 0x20 or data[end] > 0x7E:
                            break
                        end += 1
                    if end > target_off + 3 and data[end] == 0:
                        s = data[target_off:end].decode("ascii", errors="ignore")
                        strings.append(s)
    return strings


def mask_addresses(data: bytes, sections: list, rva: int, size: int) -> bytes:
    """Create masked byte pattern: replace RIP-relative displacements with wildcards."""
    off = rva_to_offset(rva, sections)
    if off < 0:
        return b""
    func_bytes = bytearray(data[off:off + size])

    # Mask call/jmp rel32 targets
    i = 0
    while i < len(func_bytes) - 4:
        if func_bytes[i] == 0xE8 or func_bytes[i] == 0xE9:  # CALL/JMP rel32
            func_bytes[i + 1:i + 5] = b'\x00\x00\x00\x00'
            i += 5
            continue
        # Mask RIP-relative displacements in LEA and MOV
        if i < len(func_bytes) - 6:
            if func_bytes[i] in (0x48, 0x4C, 0x44, 0x42) and func_bytes[i + 1] in (0x8D, 0x8B, 0x89, 0x3B, 0x39):
                modrm = func_bytes[i + 2]
                if (modrm & 0xC7) == 0x05:
                    func_bytes[i + 3:i + 7] = b'\x00\x00\x00\x00'
                    i += 7
                    continue
        # Conditional jumps: 0F 8x rel32
        if func_bytes[i] == 0x0F and i + 5 < len(func_bytes):
            if 0x80 <= func_bytes[i + 1] <= 0x8F:
                func_bytes[i + 2:i + 6] = b'\x00\x00\x00\x00'
                i += 6
                continue
        i += 1
    return bytes(func_bytes)


def extract_opcodes(data: bytes, sections: list, rva: int, size: int) -> list[int]:
    """Extract just the opcode bytes (first byte of each instruction-ish)."""
    off = rva_to_offset(rva, sections)
    if off < 0:
        return []
    func_bytes = data[off:off + min(size, 256)]
    # Simplified: just take first 64 bytes as opcode signature
    return list(func_bytes[:64])


def find_call_targets(data: bytes, sections: list, rva: int, size: int) -> list[int]:
    """Find CALL rel32 targets."""
    off = rva_to_offset(rva, sections)
    if off < 0:
        return []
    func_bytes = data[off:off + size]
    targets = []
    for i in range(len(func_bytes) - 4):
        if func_bytes[i] == 0xE8:
            disp = struct.unpack_from("<i", func_bytes, i + 1)[0]
            target = rva + i + 5 + disp
            targets.append(target)
    return targets


def find_functions_heuristic(data: bytes, sections: list) -> list[tuple[int, int]]:
    """Find function boundaries heuristically (look for common prologues)."""
    # Find the .text section
    text_rva, text_vsize, text_raw = sections[0]  # assume first section is .text
    text_bytes = data[text_raw:text_raw + text_vsize]

    funcs = []
    prologues = [
        b'\x48\x89\x5C\x24',       # mov [rsp+...], rbx
        b'\x48\x83\xEC',             # sub rsp, imm8
        b'\x48\x81\xEC',             # sub rsp, imm32
        b'\x40\x53',                 # push rbx
        b'\x40\x55',                 # push rbp
        b'\x40\x56',                 # push rsi
        b'\x40\x57',                 # push rdi
        b'\x41\x54',                 # push r12
        b'\x41\x55',                 # push r13
        b'\x41\x56',                 # push r14
        b'\x41\x57',                 # push r15
        b'\x53',                     # push rbx
        b'\x55',                     # push rbp
        b'\x56',                     # push rsi
        b'\x57',                     # push rdi
    ]

    i = 0
    while i < len(text_bytes) - 4:
        for p in prologues:
            if text_bytes[i:i + len(p)] == p:
                rva = text_rva + i
                # Estimate size: scan forward for next prologue or INT3 padding
                end = i + 16
                while end < len(text_bytes) - 4:
                    if text_bytes[end] == 0xCC:  # INT3
                        break
                    # Check for next function prologue
                    is_prologue = False
                    for p2 in prologues:
                        if text_bytes[end:end + len(p2)] == p2:
                            is_prologue = True
                            break
                    if is_prologue and end - i > 8:
                        break
                    end += 1
                size = end - i
                funcs.append((rva, min(size, 0x10000)))
                i = end
                break
        else:
            i += 1
    return funcs


def fingerprint_known_functions(
    exe_path: str,
    addresses: dict[str, str]
) -> dict[str, Function]:
    """Fingerprint all known functions in the old exe."""
    data, image_base, sections = read_pe(exe_path)
    functions = {}

    # Build a reverse map: rva → signature
    rva_to_sig = {}
    for sig, addr_str in addresses.items():
        rva = int(addr_str, 16)
        rva_to_sig[rva] = sig

    # For each known function, estimate its size and fingerprint it
    rvas = sorted(rva_to_sig.keys())
    for idx, rva in enumerate(rvas):
        # Estimate size from next function
        if idx + 1 < len(rvas):
            size = min(rvas[idx + 1] - rva, 0x10000)
        else:
            size = 256
        if size <= 0:
            size = 16

        sig = rva_to_sig[rva]
        func = Function(rva=rva, size=size)
        func.strings = find_strings_at(data, sections, rva, size)
        func.masked_bytes = mask_addresses(data, sections, rva, size)
        func.opcodes = extract_opcodes(data, sections, rva, size)
        func.call_targets = find_call_targets(data, sections, rva, size)
        functions[sig] = func

    # Build caller info
    rva_set = set(rva_to_sig.keys())
    sig_by_rva = {v.rva: k for k, v in functions.items()}
    for sig, func in functions.items():
        for target in func.call_targets:
            if target in sig_by_rva:
                functions[sig_by_rva[target]].callers.append(func.rva)

    return functions


def match_functions(
    old_funcs: dict[str, Function],
    new_exe_path: str
) -> tuple[dict[str, int], list[str]]:
    """Match old functions to addresses in the new exe."""
    data, image_base, sections = read_pe(new_exe_path)
    new_funcs = find_functions_heuristic(data, sections)

    print(f"  Found {len(new_funcs)} function candidates in new exe")

    matched = {}
    unmatched = []

    # Build string→new_rva index
    string_index: dict[str, list[int]] = defaultdict(list)
    for rva, size in new_funcs:
        strs = find_strings_at(data, sections, rva, size)
        for s in strs:
            string_index[s].append(rva)

    # Phase 1: String matching (highest confidence)
    for sig, func in old_funcs.items():
        if not func.strings:
            continue
        candidates = defaultdict(int)
        for s in func.strings:
            for rva in string_index.get(s, []):
                candidates[rva] += 1

        if candidates:
            best_rva = max(candidates, key=candidates.get)
            best_score = candidates[best_rva]
            if best_score >= len(func.strings) * 0.5 and best_score >= 1:
                matched[sig] = best_rva

    print(f"  Phase 1 (strings): {len(matched)} matched")

    # Phase 2: Exact masked bytes
    new_masked: dict[bytes, int] = {}
    for rva, size in new_funcs:
        if rva in set(matched.values()):
            continue
        mb = mask_addresses(data, sections, rva, size)
        if len(mb) > 16:
            new_masked[mb] = rva

    for sig, func in old_funcs.items():
        if sig in matched:
            continue
        if func.masked_bytes and func.masked_bytes in new_masked:
            matched[sig] = new_masked[func.masked_bytes]

    print(f"  Phase 2 (exact bytes): {len(matched)} matched")

    # Phase 3: Opcode shape
    new_opcodes: dict[tuple, list[int]] = defaultdict(list)
    for rva, size in new_funcs:
        if rva in set(matched.values()):
            continue
        ops = extract_opcodes(data, sections, rva, size)
        if ops:
            new_opcodes[tuple(ops)].append(rva)

    for sig, func in old_funcs.items():
        if sig in matched:
            continue
        key = tuple(func.opcodes)
        if key and key in new_opcodes and len(new_opcodes[key]) == 1:
            matched[sig] = new_opcodes[key][0]

    print(f"  Phase 3 (opcode shape): {len(matched)} matched")

    # Phase 4: Call graph neighbors
    old_rva_to_sig = {f.rva: s for s, f in old_funcs.items()}
    matched_old_to_new = {old_funcs[s].rva: new_rva for s, new_rva in matched.items()}

    for sig, func in old_funcs.items():
        if sig in matched:
            continue
        # Check if most call targets are matched
        if not func.call_targets:
            continue
        matched_targets = [matched_old_to_new.get(t) for t in func.call_targets if t in matched_old_to_new]
        if len(matched_targets) < 2:
            continue

        # Look for a new function that calls the same set of new addresses
        best_match = None
        best_score = 0
        for rva, size in new_funcs:
            if rva in set(matched.values()):
                continue
            new_targets = find_call_targets(data, sections, rva, size)
            overlap = len(set(new_targets) & set(matched_targets))
            if overlap > best_score:
                best_score = overlap
                best_match = rva

        if best_match and best_score >= len(matched_targets) * 0.5:
            matched[sig] = best_match

    print(f"  Phase 4 (call graph): {len(matched)} matched")

    # Collect unmatched
    for sig in old_funcs:
        if sig not in matched:
            unmatched.append(sig)

    return matched, unmatched


def main():
    parser = argparse.ArgumentParser(description="Port GD function addresses to a new exe version")
    parser.add_argument("old_exe", help="Path to old GeometryDash.exe")
    parser.add_argument("old_addresses", help="Path to addresses JSON for old version")
    parser.add_argument("new_exe", help="Path to new GeometryDash.exe")
    parser.add_argument("-o", "--output", help="Output addresses JSON", default="addresses_new.json")
    args = parser.parse_args()

    print("Loading old addresses...")
    with open(args.old_addresses) as f:
        old_data = json.load(f)
    old_addrs = old_data["addresses"]
    old_version = old_data.get("version", "unknown")
    print(f"  {len(old_addrs)} functions from GD {old_version}")

    print("Fingerprinting old exe...")
    old_funcs = fingerprint_known_functions(args.old_exe, old_addrs)
    funcs_with_strings = sum(1 for f in old_funcs.values() if f.strings)
    print(f"  {len(old_funcs)} fingerprinted ({funcs_with_strings} with string refs)")

    print("Matching against new exe...")
    matched, unmatched = match_functions(old_funcs, args.new_exe)

    print(f"\nResults:")
    print(f"  Matched: {len(matched)} / {len(old_funcs)} ({100*len(matched)/len(old_funcs):.1f}%)")
    print(f"  Unmatched: {len(unmatched)}")

    # Write output
    output = {
        "version": "new",
        "ported_from": old_version,
        "count": len(matched),
        "addresses": {sig: f"0x{rva:x}" for sig, rva in sorted(matched.items())},
        "unmatched": sorted(unmatched),
    }
    with open(args.output, "w") as f:
        json.dump(output, f, indent=2)
    print(f"  Written to {args.output}")

    if unmatched:
        print(f"\nUnmatched functions (find manually in IDA):")
        for sig in sorted(unmatched)[:20]:
            print(f"  {sig}")
        if len(unmatched) > 20:
            print(f"  ... and {len(unmatched) - 20} more")


if __name__ == "__main__":
    main()
