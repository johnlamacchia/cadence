#!/usr/bin/env python3
"""Scaffold a new mod.

  python tools/new_mod.py my_mod                     in a repo checkout: creates mods/my_mod/
  python sdk/tools/new_mod.py my_mod                 from an unzipped release: a standalone project in ./my_mod/
  python tools/new_mod.py my_mod --standalone <dir>  force a standalone project (in <dir>/my_mod)

Options: --display-name "My Mod"  --author you  --description "What it does"  --key K

In a repo checkout the root CMakeLists.txt discovers mods/*/ by itself, so the mod only needs a
short CMakeLists.txt and `cmake -B build` picks it up.

A standalone project is complete on its own: it has the cmake_minimum_required / project()
boilerplate, points at the SDK this script came from (override with -DCADENCE_SDK_DIR=...), and
builds with the two commands this script prints. Nothing else is needed.
"""
import argparse, re, sys
from pathlib import Path

SDK = Path(__file__).resolve().parent.parent          # the repo root, or the release's sdk/ folder


def in_repo_checkout():
    return (SDK / "mods").is_dir() and (SDK / "loader" / "src").is_dir()


def cmake_string(value):
    return value.replace("\\", "\\\\").replace('"', '\\"')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("name", help="lowercase letters, digits and underscores; becomes <name>.dll")
    ap.add_argument("--standalone", nargs="?", const=".", metavar="DIR",
                    help="make a standalone project in DIR/<name> (the default outside a repo checkout)")
    ap.add_argument("--display-name"); ap.add_argument("--author", default="you")
    ap.add_argument("--description", default="What it does"); ap.add_argument("--key", default="K")
    args = ap.parse_args()

    name = args.name
    if not re.fullmatch(r"[a-z][a-z0-9_]*", name):
        sys.exit("error: the name must be lowercase letters, digits and underscores, starting with a letter")
    for needed in ("template/main.cpp", "cmake/CadenceMod.cmake"):
        if not (SDK / needed).is_file():
            sys.exit(f"error: {SDK / needed} is missing; run this script from a Cadence checkout or the release's sdk/ folder")

    display = args.display_name or name.replace("_", " ").title()
    standalone = args.standalone is not None or not in_repo_checkout()
    main_cpp = (SDK / "template" / "main.cpp").read_text(encoding="utf-8").replace("MY_MOD", name)
    metadata = (f'    NAME "{cmake_string(display)}" AUTHOR "{cmake_string(args.author)}" VERSION "1.0"\n'
                f'    DESCRIPTION "{cmake_string(args.description)}"\n'
                f'    KEY "{cmake_string(args.key)}")\n')

    if not standalone:
        dest = SDK / "mods" / name
        if dest.exists():
            sys.exit(f"error: mods/{name} already exists")
        dest.mkdir(parents=True)
        (dest / "main.cpp").write_text(main_cpp, encoding="utf-8")
        (dest / "CMakeLists.txt").write_text(
            f"# Found automatically by the root CMakeLists.txt; no project() or SDK path needed here.\n"
            f"cadence_add_mod({name} main.cpp\n{metadata}", encoding="utf-8")
        print(f"created mods/{name}/\n\nnext:\n  cmake -B build -G \"Visual Studio 17 2022\" -A x64\n"
              f"  cmake --build build --config Release --target {name}_mod\n"
              f"then copy build/mods/{name}/Release/{name}.dll into <Geometry Dash>/mods/ and press F6 in game.")
        return

    parent = Path(args.standalone if args.standalone not in (None, ".") else ".").resolve()
    dest = parent / name
    if dest.exists():
        sys.exit(f"error: {dest} already exists")
    dest.mkdir(parents=True)
    sdk_path = SDK.as_posix()
    (dest / "main.cpp").write_text(main_cpp, encoding="utf-8")
    (dest / "CMakeLists.txt").write_text(
        f"cmake_minimum_required(VERSION 3.20)\n"
        f"project({name} LANGUAGES CXX)\n\n"
        f"# The Cadence SDK this project was made from. Move the SDK, or build against another copy, with\n"
        f"#   cmake -B build -DCADENCE_SDK_DIR=<path to cadence or the release's sdk/ folder>\n"
        f'set(CADENCE_SDK_DIR "{sdk_path}" CACHE PATH "Path to the Cadence SDK")\n'
        f'if(NOT EXISTS "${{CADENCE_SDK_DIR}}/cmake/CadenceMod.cmake")\n'
        f'    message(FATAL_ERROR "No Cadence SDK at ${{CADENCE_SDK_DIR}}. Pass -DCADENCE_SDK_DIR=<path>.")\n'
        f"endif()\n\n"
        f"include(${{CADENCE_SDK_DIR}}/cmake/CadenceMod.cmake)\n\n"
        f"# NAME, AUTHOR, DESCRIPTION, KEY and VERSION become the mod's metadata (shown by the mod menu).\n"
        f"cadence_add_mod({name} main.cpp\n{metadata}", encoding="utf-8")
    (dest / ".gitignore").write_text("build/\n", encoding="utf-8")
    print(f"created {dest}\n\nnext:\n  cd {dest}\n  cmake -B build -G \"Visual Studio 17 2022\" -A x64\n"
          f"  cmake --build build --config Release\n"
          f"then copy build/Release/{name}.dll into <Geometry Dash>/mods/ and press F6 in game.\n"
          f"(Needs Windows with Visual Studio 2022 and CMake 3.20+. Read docs/GUIDE.md first.)")


if __name__ == "__main__":
    main()
