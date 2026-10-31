"""
sdk update: carry Cadence to a new Geometry Dash version.

    python tools/sdk_update.py --old 2.2091 --new 2.2092 [--gd-dir <folder with the new GeometryDash.exe>]

Steps:
  1. Copy the new GeometryDash.exe to binaries/<new>/.
  2. Run gdport: fingerprint every function in the old exe and find it in the new
     one, writing generated/addresses_<new>.json (plus the functions it could not
     place).
  3. Read each class's sizeof out of the new exe (tools/layout_check.py harvest).
  4. Regenerate headers and layout hashes. If geode-sdk/bindings has a <new>
     folder, use it (its addresses are the official ones). If not, carry the old
     layouts over and keep the gdport addresses; step 5 tells you what to recheck.
  5. Report which class layouts changed (hash diff, plus sizeof changes seen
     directly in the binary) and which functions need a human in IDA.

Add addresses you find by hand to generated/addresses.local.json; the loader
merges that file at startup, so nobody has to wait for an official update.
"""
import argparse
import json
import shutil
import subprocess
import sys
from pathlib import Path

SDK_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_GD_DIR = Path(r"C:\Program Files (x86)\Steam\steamapps\common\Geometry Dash")


def run(cmd):
    print("  $", " ".join(str(c) for c in cmd))
    return subprocess.run([str(c) for c in cmd]).returncode


def load_json(path):
    return json.loads(path.read_text(encoding="utf-8")) if path.exists() else {}


def main():
    ap = argparse.ArgumentParser(description="Update Cadence to a new GD version")
    ap.add_argument("--old", required=True, help="old GD version, e.g. 2.2091")
    ap.add_argument("--new", required=True, help="new GD version, e.g. 2.2092")
    ap.add_argument("--gd-dir", default=str(DEFAULT_GD_DIR), help="folder containing the NEW GeometryDash.exe")
    ap.add_argument("--bindings", default=str(SDK_ROOT.parent / "bindings" / "bindings"),
                    help="geode-sdk/bindings' bindings/ folder")
    args = ap.parse_args()

    old_ver, new_ver = args.old, args.new
    binaries, generated = SDK_ROOT / "binaries", SDK_ROOT / "generated"
    bindings = Path(args.bindings)

    # 1. new exe
    new_exe = Path(args.gd_dir) / "GeometryDash.exe"
    dest_exe = binaries / new_ver / "GeometryDash.exe"
    dest_exe.parent.mkdir(parents=True, exist_ok=True)
    if not dest_exe.exists():
        if not new_exe.exists():
            sys.exit(f"ERROR: {new_exe} not found")
        print(f"Copying {new_exe} -> {dest_exe}")
        shutil.copy2(new_exe, dest_exe)
    else:
        print(f"Using existing {dest_exe}")

    old_exe = binaries / old_ver / "GeometryDash.exe"
    old_addr, new_addr = generated / f"addresses_{old_ver}.json", generated / f"addresses_{new_ver}.json"
    for p in (old_exe, old_addr):
        if not p.exists():
            sys.exit(f"ERROR: {p} not found")

    # 2. gdport
    print(f"\nPorting addresses {old_ver} -> {new_ver} with gdport...")
    if run([sys.executable, SDK_ROOT / "gdport" / "gdport.py", old_exe, old_addr, dest_exe, "-o", new_addr]):
        sys.exit("ERROR: gdport failed")

    # 3. class sizes straight from the new binary
    print("\nReading class sizes from the new exe...")
    if run([sys.executable, SDK_ROOT / "tools" / "layout_check.py", "harvest", "--exe", dest_exe, "--addresses", new_addr]):
        print("  (could not read class sizes; layouts will be marked unverified)")

    # 4. regenerate
    have_bindings = (bindings / new_ver).exists()
    print(f"\nRegenerating headers and layout hashes ({'from the new bindings' if have_bindings else 'carrying the ' + old_ver + ' layouts over'})...")
    cmd = [sys.executable, SDK_ROOT / "codegen" / "generate.py", bindings, new_ver]
    if not have_bindings:
        cmd += ["--bindings-version", old_ver, "--keep-addresses"]
    if run(cmd):
        sys.exit("ERROR: codegen failed")

    # 5. what changed
    print("\n=== Layout changes ===")
    old_h = load_json(generated / f"layout_hashes_{old_ver}.json").get("hashes", {})
    new_h = load_json(generated / f"layout_hashes_{new_ver}.json").get("hashes", {})
    changed = [c for c in sorted(set(old_h) | set(new_h)) if old_h.get(c) != new_h.get(c)]
    print(f"bindings: {len(changed)} class layout(s) differ" + ("" if changed else " (expected when the bindings were carried over)"))
    for c in changed[:40]:
        print(f"  {c}")

    old_s = load_json(generated / f"class_sizes_{old_ver}.json").get("sizes", {})
    new_s = load_json(generated / f"class_sizes_{new_ver}.json").get("sizes", {})
    resized = [(c, old_s[c], new_s[c]) for c in sorted(set(old_s) & set(new_s)) if old_s[c] != new_s[c]]
    print(f"binary:   {len(resized)} class(es) changed sizeof between {old_ver} and {new_ver}")
    for c, a, b in resized:
        print(f"  {c}: {a:#x} -> {b:#x}   (update its members in the bindings, or the layout is wrong)")
    if resized and not have_bindings:
        print("  These classes' headers were generated from the OLD layout and are now wrong.")

    print("\n=== Functions needing a human ===")
    unmatched = load_json(new_addr).get("unmatched", [])
    print(f"{len(unmatched)} function(s) gdport could not place")
    for sig in unmatched[:15]:
        print(f"  {sig}")
    if len(unmatched) > 15:
        print(f"  ... and {len(unmatched) - 15} more (see the 'unmatched' list in {new_addr.name})")
    if unmatched:
        print("Find each in IDA and add it to generated/addresses.local.json:")
        print('  { "Class::function(args)": "0xRVA" }')

    print("\nNext: python tools/layout_check.py check <bindings> --version " + new_ver + "; rebuild; run the mods.")


if __name__ == "__main__":
    main()
