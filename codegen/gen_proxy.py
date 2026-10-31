"""Generate winmm.dll proxy forwarding stubs (MASM + C++ + DEF)."""
import struct
import sys
from pathlib import Path

def get_winmm_exports():
    dll_path = r"C:\Windows\System32\winmm.dll"
    data = open(dll_path, "rb").read()
    e_lfanew = struct.unpack_from("<I", data, 0x3c)[0]
    opt_hdr = e_lfanew + 4 + 20
    export_rva = struct.unpack_from("<I", data, opt_hdr + 112)[0]
    num_names = struct.unpack_from("<I", data, export_rva + 24)[0]
    names_rva = struct.unpack_from("<I", data, export_rva + 32)[0]
    names = []
    for i in range(num_names):
        name_rva = struct.unpack_from("<I", data, names_rva + i * 4)[0]
        end = name_rva
        while data[end] != 0:
            end += 1
        names.append(data[name_rva:end].decode("ascii"))
    return sorted(names)


def generate(out_dir: Path):
    exports = get_winmm_exports()
    out_dir.mkdir(parents=True, exist_ok=True)

    # --- proxy.asm ---
    asm_lines = [
        "; Auto-generated winmm.dll forwarding stubs",
        "; Each export jumps through a function pointer table filled at runtime",
        "",
        ".data",
        f"public real_funcs",
        f"real_funcs dq {len(exports)} dup(0)",
        "",
        ".code",
        "",
    ]
    for i, name in enumerate(exports):
        asm_lines.append(f"proxy_{name} proc")
        asm_lines.append(f"    jmp qword ptr [real_funcs + {i * 8}]")
        asm_lines.append(f"proxy_{name} endp")
        asm_lines.append("")

    asm_lines.append("end")
    (out_dir / "proxy.asm").write_text("\n".join(asm_lines), encoding="utf-8")

    # --- proxy.def ---
    def_lines = [
        "; Auto-generated winmm.dll export definitions",
        "LIBRARY winmm",
        "EXPORTS",
    ]
    for name in exports:
        def_lines.append(f"    {name} = proxy_{name}")
    # Cadence's own exports, looked up by name with GetProcAddress (null on an older loader).
    def_lines += ["    cadence_reload_mods", "    cadence_loader_version"]
    (out_dir / "proxy.def").write_text("\n".join(def_lines), encoding="utf-8")

    # --- proxy_table.h (init code) ---
    h_lines = [
        "// Auto-generated: fills real_funcs[] from the real winmm.dll",
        "#pragma once",
        "#include <windows.h>",
        "",
        f"#define WINMM_EXPORT_COUNT {len(exports)}",
        "extern \"C\" UINT_PTR real_funcs[WINMM_EXPORT_COUNT];",
        "",
        "static const char* winmm_export_names[WINMM_EXPORT_COUNT] = {",
    ]
    for name in exports:
        h_lines.append(f'    "{name}",')
    h_lines.append("};")
    h_lines.append("")
    h_lines.append("inline bool proxy_init() {")
    h_lines.append('    HMODULE real = LoadLibraryW(L"C:\\\\Windows\\\\System32\\\\winmm.dll");')
    h_lines.append("    if (!real) return false;")
    h_lines.append("    for (int i = 0; i < WINMM_EXPORT_COUNT; i++) {")
    h_lines.append("        real_funcs[i] = (UINT_PTR)GetProcAddress(real, winmm_export_names[i]);")
    h_lines.append("    }")
    h_lines.append("    return true;")
    h_lines.append("}")
    (out_dir / "proxy_table.h").write_text("\n".join(h_lines), encoding="utf-8")

    print(f"Generated proxy files for {len(exports)} exports in {out_dir}")
    return exports


if __name__ == "__main__":
    out = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("../loader/src")
    generate(out)
