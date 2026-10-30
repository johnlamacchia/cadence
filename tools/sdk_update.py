"""
sdk update — port addresses to a new GD version and regenerate headers.

Usage:
    python sdk_update.py --old 2.2091 --new 2.2092

Steps:
    1. Copy the new GeometryDash.exe to binaries/<new_version>/
    2. Run gdport to carry addresses from old → new
    3. Regenerate headers from the (possibly updated) bindings
    4. Show a diff of which class layouts changed
    5. List functions that couldn't be matched
"""
import argparse
import json
import shutil
import subprocess
import sys
from pathlib import Path

SDK_ROOT = Path(__file__).parent.parent
GD_DIR = Path(r"C:\Program Files (x86)\Steam\steamapps\common\Geometry Dash")


def main():
    parser = argparse.ArgumentParser(description="Update Cadence to a new GD version")
    parser.add_argument("--old", required=True, help="Old GD version (e.g. 2.2091)")
    parser.add_argument("--new", required=True, help="New GD version (e.g. 2.2092)")
    parser.add_argument("--gd-dir", default=str(GD_DIR), help="GD install directory")
    args = parser.parse_args()

    old_ver = args.old
    new_ver = args.new
    gd_dir = Path(args.gd_dir)

    binaries_dir = SDK_ROOT / "binaries"
    generated_dir = SDK_ROOT / "generated"
    gdport_script = SDK_ROOT / "gdport" / "gdport.py"
    codegen_script = SDK_ROOT / "codegen" / "generate.py"
    bindings_dir = SDK_ROOT.parent / "bindings" / "bindings"

    # 1. Copy new exe
    new_bin_dir = binaries_dir / new_ver
    new_bin_dir.mkdir(parents=True, exist_ok=True)
    new_exe = gd_dir / "GeometryDash.exe"
    if not new_exe.exists():
        print(f"ERROR: {new_exe} not found")
        sys.exit(1)

    dest_exe = new_bin_dir / "GeometryDash.exe"
    if not dest_exe.exists():
        print(f"Copying {new_exe} to {dest_exe}...")
        shutil.copy2(new_exe, dest_exe)
    else:
        print(f"Using existing {dest_exe}")

    # 2. Run gdport
    old_exe = binaries_dir / old_ver / "GeometryDash.exe"
    old_addresses = generated_dir / f"addresses_{old_ver}.json"
    new_addresses = generated_dir / f"addresses_{new_ver}.json"

    if not old_exe.exists():
        print(f"ERROR: {old_exe} not found")
        sys.exit(1)
    if not old_addresses.exists():
        print(f"ERROR: {old_addresses} not found")
        sys.exit(1)

    print(f"\nPorting addresses from {old_ver} → {new_ver}...")
    result = subprocess.run([
        sys.executable, str(gdport_script),
        str(old_exe), str(old_addresses),
        str(dest_exe),
        "-o", str(new_addresses),
    ])
    if result.returncode != 0:
        print("ERROR: gdport failed")
        sys.exit(1)

    # 3. Regenerate headers if bindings exist for new version
    if (bindings_dir / new_ver).exists():
        print(f"\nRegenerating headers for {new_ver}...")
        subprocess.run([
            sys.executable, str(codegen_script),
            str(bindings_dir), new_ver,
        ])
    else:
        print(f"\nNo bindings found for {new_ver}, skipping header regeneration")
        print(f"  (add bindings to {bindings_dir / new_ver}/ and re-run)")

    # 4. Diff layout hashes
    old_hashes_path = generated_dir / f"layout_hashes_{old_ver}.json"
    new_hashes_path = generated_dir / f"layout_hashes_{new_ver}.json"

    if old_hashes_path.exists() and new_hashes_path.exists():
        with open(old_hashes_path) as f:
            old_hashes = json.load(f).get("hashes", {})
        with open(new_hashes_path) as f:
            new_hashes = json.load(f).get("hashes", {})

        changed = []
        for cls in sorted(set(old_hashes) | set(new_hashes)):
            oh = old_hashes.get(cls)
            nh = new_hashes.get(cls)
            if oh != nh:
                if oh and nh:
                    changed.append(f"  CHANGED: {cls} ({oh} → {nh})")
                elif not oh:
                    changed.append(f"  NEW:     {cls}")
                else:
                    changed.append(f"  REMOVED: {cls}")

        if changed:
            print(f"\nLayout changes ({len(changed)} classes):")
            for line in changed:
                print(line)
        else:
            print("\nNo layout changes detected")

    # 5. Show unmatched summary
    if new_addresses.exists():
        with open(new_addresses) as f:
            data = json.load(f)
        unmatched = data.get("unmatched", [])
        if unmatched:
            print(f"\n{len(unmatched)} functions need manual address finding in IDA:")
            for sig in unmatched[:10]:
                print(f"  {sig}")
            if len(unmatched) > 10:
                print(f"  ... and {len(unmatched) - 10} more")
            print(f"\nAdd them to generated/addresses.local.json to unblock mod devs")

    print("\nDone!")


if __name__ == "__main__":
    main()
