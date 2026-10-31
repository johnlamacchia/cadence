#!/usr/bin/env python3
"""Check what every built mod DLL exports, the way the loader will read it.

For each mods/*.dll: it must export mod_main; its mod_info() must be valid JSON;
and every class hash from mod_layout_hashes() must equal the hash in
generated/layout_hashes_<ver>.json (otherwise the loader would refuse the mod).

  python tests/mod_exports_check.py <dir containing mod dlls> [version]
"""
import ctypes, json, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def main():
    mods_dir = Path(sys.argv[1])
    version = sys.argv[2] if len(sys.argv) > 2 else "2.2091"
    known = json.load(open(ROOT / "generated" / f"layout_hashes_{version}.json"))["hashes"]

    bad = 0
    dlls = sorted(mods_dir.glob("*.dll"))
    for dll in dlls:
        lib = ctypes.WinDLL(str(dll))     # runs static init: this is where class hashes register
        problems = []
        if not hasattr(lib, "mod_main"):
            problems.append("no mod_main")
        info = {}
        if hasattr(lib, "mod_info"):
            lib.mod_info.restype = ctypes.c_char_p
            try:
                info = json.loads(lib.mod_info())
            except Exception as e:
                problems.append(f"mod_info is not JSON ({e})")
        else:
            problems.append("no mod_info")
        hashes = {}
        if hasattr(lib, "mod_layout_hashes"):
            lib.mod_layout_hashes.restype = ctypes.c_char_p
            hashes = json.loads(lib.mod_layout_hashes())
        for cls, h in hashes.items():
            if cls in known and known[cls] != h:
                problems.append(f"{cls}: mod has {h}, current layout is {known[cls]}")
        status = "OK " if not problems else "BAD"
        bad += bool(problems)
        print(f"{status} {dll.name:22s} {info.get('name', '?'):22s} {len(hashes):4d} layout hashes  {'; '.join(problems)}")
    print(f"\n{len(dlls)} mods, {bad} problem(s)")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
