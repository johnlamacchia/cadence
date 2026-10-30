"""
Code generator: takes parsed Broma data and outputs:
1. C++ headers for GD classes
2. addresses_<version>.json
3. layout_hashes_<version>.json
"""
import json
import os
import sys
from pathlib import Path

from broma_parser import parse_version, ClassDecl, FuncDecl, MemberDecl


KNOWN_TYPE_SIZES_WIN64 = {
    "bool": 1,
    "char": 1,
    "unsigned char": 1,
    "int8_t": 1,
    "uint8_t": 1,
    "short": 2,
    "unsigned short": 2,
    "int16_t": 2,
    "uint16_t": 2,
    "int": 4,
    "unsigned int": 4,
    "int32_t": 4,
    "uint32_t": 4,
    "float": 4,
    "double": 8,
    "long long": 8,
    "unsigned long long": 8,
    "int64_t": 8,
    "uint64_t": 8,
    "size_t": 8,
    "uintptr_t": 8,
    "intptr_t": 8,
    "gd::string": 40,  # MSVC std::string with SSO
    "std::string": 40,
    "std::string const&": 8,
}


def format_return_type(rt: str) -> str:
    if rt == "TodoReturn":
        return "void /* TodoReturn */"
    return rt


def format_params_for_header(params: str) -> str:
    return params


def generate_address_json(parser, version: str, out_dir: Path):
    """Generate addresses_<version>.json mapping signatures to win addresses."""
    addresses = {}

    for cls in parser.classes.values():
        for func in cls.functions:
            if func.addrs.has_win():
                key = func.full_key()
                addresses[key] = f"0x{func.addrs.win:x}"

    for func in parser.free_functions:
        if func.addrs.has_win():
            key = func.full_key()
            addresses[key] = f"0x{func.addrs.win:x}"

    out_path = out_dir / f"addresses_{version}.json"
    data = {
        "version": version,
        "count": len(addresses),
        "addresses": addresses,
    }
    out_path.write_text(json.dumps(data, indent=2), encoding="utf-8")
    print(f"  addresses: {len(addresses)} entries -> {out_path}")
    return addresses


def generate_layout_hashes(parser, version: str, out_dir: Path):
    """Generate layout_hashes_<version>.json."""
    hashes = {}
    for cls in parser.classes.values():
        if cls.members:
            hashes[cls.name] = cls.layout_hash()

    out_path = out_dir / f"layout_hashes_{version}.json"
    data = {
        "version": version,
        "count": len(hashes),
        "hashes": hashes,
    }
    out_path.write_text(json.dumps(data, indent=2), encoding="utf-8")
    print(f"  layout hashes: {len(hashes)} entries -> {out_path}")
    return hashes


def generate_headers(parser, out_dir: Path):
    """Generate C++ headers for all GD classes."""
    include_dir = out_dir / "include" / "gd"
    include_dir.mkdir(parents=True, exist_ok=True)

    all_class_names = set(parser.classes.keys())
    generated_files = []

    for cls_name, cls in parser.classes.items():
        header_path = include_dir / f"{cls_name}.hpp"
        content = generate_class_header(cls, all_class_names)
        header_path.write_text(content, encoding="utf-8")
        generated_files.append(cls_name)

    # Generate umbrella header
    umbrella = include_dir.parent / "gd.hpp"
    lines = ["#pragma once\n"]
    for name in sorted(generated_files):
        lines.append(f'#include "gd/{name}.hpp"')
    lines.append("")
    umbrella.write_text("\n".join(lines), encoding="utf-8")
    print(f"  headers: {len(generated_files)} classes -> {include_dir}")


def generate_class_header(cls: ClassDecl, all_classes: set[str]) -> str:
    lines = []
    lines.append("#pragma once")
    lines.append("")

    # Collect forward declarations needed
    fwd = collect_forward_decls(cls, all_classes)
    for f in sorted(fwd):
        if "::" in f:
            # namespace::class
            ns, name = f.rsplit("::", 1)
            lines.append(f"namespace {ns} {{ class {name}; }}")
        else:
            lines.append(f"class {f};")
    if fwd:
        lines.append("")

    # Class declaration
    base_str = ""
    if cls.bases:
        bases = ", ".join(f"public {b}" for b in cls.bases)
        base_str = f" : {bases}"
    lines.append(f"class {cls.name}{base_str} {{")
    lines.append("public:")

    # Virtual functions first (order matters for vtable)
    virtuals = [f for f in cls.functions if f.is_virtual]
    if virtuals:
        for f in virtuals:
            lines.append(f"    {format_func_decl(f)};")
        lines.append("")

    # Static functions
    statics = [f for f in cls.functions if f.is_static]
    if statics:
        for f in statics:
            lines.append(f"    {format_func_decl(f)};")
        lines.append("")

    # Regular member functions
    regular = [f for f in cls.functions if not f.is_virtual and not f.is_static]
    if regular:
        for f in regular:
            lines.append(f"    {format_func_decl(f)};")
        lines.append("")

    # Member variables
    if cls.members:
        for m in cls.members:
            if m.is_pad:
                lines.append(f"    char {m.name}[{m.pad_size}];")
            else:
                lines.append(f"    {m.type} {m.name};")
        lines.append("")

    lines.append("};")
    lines.append("")
    return "\n".join(lines)


def format_func_decl(func: FuncDecl) -> str:
    parts = []
    if func.is_virtual:
        parts.append("virtual")
    if func.is_static:
        parts.append("static")

    ret = format_return_type(func.return_type)
    if ret:
        parts.append(ret)

    params = format_params_for_header(func.params)
    parts.append(f"{func.name}({params})")
    return " ".join(parts)


def collect_forward_decls(cls: ClassDecl, all_classes: set[str]) -> set[str]:
    """Collect types that need forward declarations."""
    fwd = set()
    # We only forward-declare GD classes referenced in function params/returns
    # and member types. Don't forward-declare bases (they need full defs).
    base_names = set()
    for b in cls.bases:
        # Extract the class name from possibly qualified name
        base_names.add(b.split("::")[-1])
        base_names.add(b)

    def check_type(t: str):
        # Look for class names in the type string
        for name in all_classes:
            if name in t and name != cls.name and name not in base_names:
                fwd.add(name)

    for f in cls.functions:
        check_type(f.return_type)
        check_type(f.params)
    for m in cls.members:
        if not m.is_pad:
            check_type(m.type)

    return fwd


def main():
    if len(sys.argv) < 2:
        print("Usage: generate.py <bindings_dir> [version]")
        print("  bindings_dir: path to bindings/bindings/")
        print("  version: GD version (default: 2.2091)")
        sys.exit(1)

    bindings_dir = Path(sys.argv[1])
    version = sys.argv[2] if len(sys.argv) > 2 else "2.2091"
    out_dir = Path(__file__).parent.parent / "generated"
    out_dir.mkdir(parents=True, exist_ok=True)

    print(f"Parsing bindings for GD {version}...")
    parser = parse_version(bindings_dir, version)
    print(f"  {len(parser.classes)} classes, {len(parser.free_functions)} free functions")

    print("Generating...")
    addresses = generate_address_json(parser, version, out_dir)
    hashes = generate_layout_hashes(parser, version, out_dir)
    generate_headers(parser, out_dir)

    # Summary
    win_hookable = sum(
        1 for cls in parser.classes.values()
        for f in cls.functions if f.addrs.has_win()
    )
    win_hookable += sum(1 for f in parser.free_functions if f.addrs.has_win())
    print(f"\nDone. {win_hookable} hookable Windows functions.")


if __name__ == "__main__":
    main()
