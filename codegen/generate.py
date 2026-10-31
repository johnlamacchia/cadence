"""
Code generator: takes the Broma bindings and outputs, under generated/:

  addresses_<ver>.json      function signature -> Windows address
  layout_hashes_<ver>.json  per-class hash of the exact byte layout
  layout_report_<ver>.json  per-class layout status (verified / tail-padded / unverified / incomplete)
  include/gd/**             C++ headers: declarations + member layouts + static_asserts

Usage: generate.py <bindings_dir> [version]
  bindings_dir: path to geode-sdk/bindings' bindings/ folder
"""
import json
import sys
from pathlib import Path

from broma_parser import parse_version
import headergen


def generate_address_json(parser, version: str, out_dir: Path):
    """Generate addresses_<version>.json mapping signatures to win addresses.
    Functions marked `win inline` have no address on Windows and are left out."""
    addresses = {}

    for cls in parser.classes.values():
        for func in cls.functions:
            if func.addrs.has_win():
                addresses[func.full_key()] = f"0x{func.addrs.win:x}"

    for func in parser.free_functions:
        if func.addrs.has_win():
            addresses[func.full_key()] = f"0x{func.addrs.win:x}"

    out_path = out_dir / f"addresses_{version}.json"
    data = {"version": version, "count": len(addresses), "addresses": addresses}
    out_path.write_text(json.dumps(data, indent=2), encoding="utf-8")
    print(f"  addresses: {len(addresses)} entries -> {out_path}")
    return addresses


def reject_bad_addresses(parser, version: str, out_dir: Path):
    """
    Check every Windows address against the real GeometryDash.exe and drop the ones that
    cannot be function starts: outside executable code, or strictly inside a function the
    exe's own unwind table (.pdata) says begins earlier. Hooking such an address would
    corrupt the game, and the bindings do contain a few. Dropped addresses are written to
    addresses_rejected_<version>.json so nothing disappears silently.
    """
    exe = out_dir.parent / "binaries" / version / "GeometryDash.exe"
    if not exe.exists():
        print(f"  address check: skipped ({exe} not found; addresses are taken from the bindings as they are)")
        return
    sys.path.insert(0, str(out_dir.parent / "gdport"))
    import bisect
    from gdport import Image, pdata_ranges

    img = Image(exe)
    ranges = pdata_ranges(img)
    begins = [b for b, _ in ranges]
    rejected = {}
    for cls in parser.classes.values():
        for f in cls.functions:
            if not f.addrs.has_win():
                continue
            rva = f.addrs.win
            reason = None
            if not img.exec_end(rva):
                reason = "outside executable code"
            else:
                i = bisect.bisect_right(begins, rva) - 1
                if i >= 0 and begins[i] < rva < ranges[i][1]:
                    reason = f"inside the function that starts at {begins[i]:#x}"
            if reason:
                rejected[f.full_key()] = {"address": f"0x{rva:x}", "reason": reason}
                f.addrs.win = None
    path = out_dir / f"addresses_rejected_{version}.json"
    path.write_text(json.dumps({"version": version, "rejected": rejected}, indent=2), encoding="utf-8")
    print(f"  address check: {len(rejected)} address(es) are not function starts in the exe and were dropped -> {path.name}")


def generate_layout_hashes(hashes: dict, version: str, out_dir: Path):
    out_path = out_dir / f"layout_hashes_{version}.json"
    data = {"version": version, "count": len(hashes), "hashes": hashes}
    out_path.write_text(json.dumps(data, indent=2, sort_keys=True), encoding="utf-8")
    print(f"  layout hashes: {len(hashes)} entries -> {out_path}")


def main():
    import argparse
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("bindings_dir", help="geode-sdk/bindings' bindings/ folder")
    ap.add_argument("version", nargs="?", default="2.2091", help="GD version to generate for")
    ap.add_argument("--bindings-version", help="parse this version's .bro files instead (when bindings for a new GD "
                                               "version do not exist yet: carries the old layouts over)")
    ap.add_argument("--keep-addresses", action="store_true", help="do not overwrite addresses_<version>.json "
                                                                  "(use after gdport has produced it)")
    args = ap.parse_args()

    bindings_dir = Path(args.bindings_dir)
    version = args.version
    out_dir = Path(__file__).parent.parent / "generated"
    out_dir.mkdir(parents=True, exist_ok=True)

    source_version = args.bindings_version or version
    print(f"Parsing bindings for GD {source_version}" + (f" (generating for {version})" if source_version != version else "") + "...")
    parser = parse_version(bindings_dir, source_version)
    print(f"  {len(parser.classes)} classes, {len(parser.free_functions)} free functions")

    sizes_path = out_dir / f"class_sizes_{version}.json"
    sizes = json.loads(sizes_path.read_text())["sizes"] if sizes_path.exists() else {}
    if sizes:
        print(f"  {len(sizes)} class sizes read from the game binary ({sizes_path.name})")
    else:
        print(f"  no {sizes_path.name}: layouts will be 'unverified' (run tools/layout_check.py harvest)")

    print("Generating...")
    if not args.keep_addresses:        # a ported address file was checked by gdport already
        reject_bad_addresses(parser, version, out_dir)
    if args.keep_addresses:
        print(f"  addresses: kept existing addresses_{version}.json")
    else:
        generate_address_json(parser, version, out_dir)
    hashes, _ = headergen.generate(parser, bindings_dir, version, out_dir, sizes)
    generate_layout_hashes(hashes, version, out_dir)

    hookable = sum(1 for c in parser.classes.values() for f in c.functions if f.addrs.has_win())
    hookable += sum(1 for f in parser.free_functions if f.addrs.has_win())
    inline = sum(1 for c in parser.classes.values() for f in c.functions if f.addrs.win_inline)
    print(f"\nDone. {hookable} hookable Windows functions ({inline} marked `win inline`: not hookable).")


if __name__ == "__main__":
    main()
