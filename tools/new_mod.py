#!/usr/bin/env python3
"""Scaffold a new mod:  python tools/new_mod.py my_mod

Creates mods/my_mod/{CMakeLists.txt,main.cpp} from template/. Mods are
auto-discovered, so just re-run `cmake -B build` and build.
"""
import re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def main():
    if len(sys.argv) != 2 or not re.fullmatch(r"[a-z][a-z0-9_]*", sys.argv[1]):
        sys.exit("usage: new_mod.py <name>   (lowercase letters, digits, underscore)")
    name = sys.argv[1]
    dest = ROOT / "mods" / name
    if dest.exists():
        sys.exit(f"mods/{name} already exists")
    dest.mkdir(parents=True)
    (dest / "main.cpp").write_text((ROOT / "template" / "main.cpp").read_text().replace("MY_MOD", name))
    (dest / "CMakeLists.txt").write_text(
        f'cadence_add_mod({name} main.cpp\n'
        f'    NAME "{name}" AUTHOR "you" VERSION "1.0"\n'
        f'    DESCRIPTION "What it does"\n'
        f'    KEY "K")\n')
    print(f"created mods/{name}/\nnext: cmake -B build -A x64 && cmake --build build --config Release --target {name}_mod")


if __name__ == "__main__":
    main()
