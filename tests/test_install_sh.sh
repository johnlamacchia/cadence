#!/usr/bin/env bash
# Tests tools/install.sh against throwaway Steam folder trees. Needs bash (on Windows: Git's bash.exe).
#   bash tests/test_install_sh.sh <build dir> [path to CadenceInstaller.exe]
# The release folder is assembled from the same list the installer packs (build/installer/payload_list_*.txt).
set -u
BUILD="${1:?usage: test_install_sh.sh <build dir> [CadenceInstaller.exe]}"
INSTALLER="${2:-}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
LIST="$BUILD/installer/payload_list_${CONFIG:-Release}.txt"
[ -f "$LIST" ] || { echo "no $LIST (build the CadenceInstaller target first)"; exit 2; }

T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT
failed=0
check() { if [ "$1" = 0 ]; then echo "  ok    $2"; else echo "  FAIL  $2"; failed=$((failed + 1)); fi; }
ok()    { "$@" >/dev/null 2>&1; }
no()    { ! "$@" >/dev/null 2>&1; }

# --- assemble a release folder: winmm.dll, mods/, generated/, install.sh ---------------------
REL="$T/release"
mkdir -p "$REL"
while IFS='|' read -r src dest; do
    dest="${dest%$'\r'}"              # CMake writes the list with Windows line endings on Windows
    [ -n "$src" ] || continue
    mkdir -p "$REL/$(dirname "$dest")"
    cp "$src" "$REL/$dest"
done < "$LIST"
cp "$ROOT/tools/install.sh" "$REL/install.sh"
# a report that must never be installed as an address file
echo '{}' > "$REL/generated/addresses_rejected_2.2091.json"

REAL_EXE="$ROOT/binaries/2.2091/GeometryDash.exe"
fake_exe() { printf 'MZ\0\0' > "$1/GeometryDash.exe"; }                  # not a real exe: unknown version
install_sh() { ( cd "$REL" && HOME="$1" bash install.sh "${@:2}" ); }

echo "finding Geometry Dash"
H1="$T/home1"; mkdir -p "$H1/.steam/steam/steamapps"
LIB2="$T/games/SteamLibrary"; GD1="$LIB2/steamapps/common/Geometry Dash"; mkdir -p "$GD1"
printf '"libraryfolders"\n{\n\t"0"\n\t{\n\t\t"path"\t\t"%s"\n\t}\n}\n' "$LIB2" > "$H1/.steam/steam/steamapps/libraryfolders.vdf"
if [ -f "$REAL_EXE" ]; then cp "$REAL_EXE" "$GD1/GeometryDash.exe"; extra=""; else fake_exe "$GD1"; extra="--allow-unknown-version"; fi
out="$(install_sh "$H1" $extra 2>&1)"; rc=$?
check "$rc" "finds GD on a second library drive listed in libraryfolders.vdf, and installs"
echo "$out" | grep -q "$LIB2" ; check $? "  ...and says which folder it used"
[ -f "$GD1/winmm.dll" ]; check $? "winmm.dll installed"
n=$(ls "$GD1/mods"/*.dll 2>/dev/null | wc -l); [ "$n" -ge 15 ]; check $? "example mods installed ($n)"
[ "$(ls "$GD1/generated"/addresses_*.json | wc -l)" = 1 ] && [ ! -e "$GD1/generated/addresses_rejected_2.2091.json" ]; check $? "exactly one address file, the 'rejected' report is not installed"
[ -f "$GD1/cadence_install.txt" ] && grep -q '^file winmm.dll$' "$GD1/cadence_install.txt"; check $? "manifest written"
echo "$out" | grep -q 'WINEDLLOVERRIDES="winmm=n,b"'; check $? "tells the user the Proton launch option"
cmp -s "$GD1/winmm.dll" "$REL/winmm.dll"; check $? "installed winmm.dll is byte-identical to the release's"

if [ -f "$REAL_EXE" ]; then
    echo "version check"
    G="$T/dummy"; mkdir -p "$G"; fake_exe "$G"
    no install_sh "$T/nohome" --gd-dir "$G"; check $? "refuses an exe that is not 2.2091"
    [ ! -e "$G/winmm.dll" ]; check $? "  ...and wrote nothing"
fi

echo "a release folder with no winmm.dll"
EMPTY_REL="$T/emptyrel"; mkdir -p "$EMPTY_REL"; cp "$ROOT/tools/install.sh" "$EMPTY_REL/install.sh"; GD0="$T/gd0"; mkdir -p "$GD0"; fake_exe "$GD0"
( cd "$EMPTY_REL" && HOME="$T/nohome" bash install.sh --gd-dir "$GD0" --allow-unknown-version >/dev/null 2>&1 ); rc=$?
[ "$rc" != 0 ]; check $? "fails loudly instead of reporting success"
[ ! -e "$GD0/cadence_install.txt" ]; check $? "  ...and writes no manifest"

echo "Flatpak Steam layout"
H2="$T/home2"; GD2="$H2/.var/app/com.valvesoftware.Steam/.local/share/Steam/steamapps/common/Geometry Dash"; mkdir -p "$GD2"; fake_exe "$GD2"
ok install_sh "$H2" --allow-unknown-version; check $? "found and installed"
[ -f "$GD2/winmm.dll" ]; check $? "winmm.dll installed"

echo "native Steam in ~/.local/share/Steam"
H3="$T/home3"; GD3="$H3/.local/share/Steam/steamapps/common/Geometry Dash"; mkdir -p "$GD3"; fake_exe "$GD3"
ok install_sh "$H3" --allow-unknown-version; check $? "found and installed"

echo "Geometry Dash not installed"
no install_sh "$T/emptyhome" --allow-unknown-version; check $? "fails with a clear error instead of guessing"

echo "uninstall leaves the user's files alone"
echo "keep me" > "$GD1/mods/my_own_mod.dll"; echo "keep" > "$GD1/qolmod.ini"
ok install_sh "$H1" --uninstall; check $? "uninstall succeeds"
[ ! -e "$GD1/winmm.dll" ] && [ ! -e "$GD1/mods/hitboxes.dll" ] && [ ! -e "$GD1/cadence_install.txt" ]; check $? "loader, bundled mods and manifest are gone"
[ -f "$GD1/mods/my_own_mod.dll" ] && [ -f "$GD1/qolmod.ini" ]; check $? "the user's own mod and settings are untouched"
[ ! -d "$GD1/generated" ]; check $? "the empty generated/ folder was removed"

echo "another mod loader's winmm.dll is never overwritten"
G4="$T/foreign"; mkdir -p "$G4"; fake_exe "$G4"; echo "FOREIGN" > "$G4/winmm.dll"
ok install_sh "$T/nohome" --gd-dir "$G4" --allow-unknown-version; check $? "install succeeds"
[ "$(cat "$G4/winmm.dll.pre-cadence")" = "FOREIGN" ]; check $? "the other loader was moved aside intact"
[ "$(wc -c < "$G4/winmm.dll")" -gt 1000 ]; check $? "Cadence's winmm.dll took its place"
ok install_sh "$T/nohome" --gd-dir "$G4" --uninstall; check $? "uninstall succeeds"
[ "$(cat "$G4/winmm.dll")" = "FOREIGN" ] && [ ! -e "$G4/winmm.dll.pre-cadence" ]; check $? "the other loader is back, no backup left over"

echo "refuses to clobber an existing backup"
G5="$T/twice"; mkdir -p "$G5"; fake_exe "$G5"; echo A > "$G5/winmm.dll"; echo B > "$G5/winmm.dll.pre-cadence"
no install_sh "$T/nohome" --gd-dir "$G5" --allow-unknown-version; check $? "fails"
[ "$(cat "$G5/winmm.dll")" = A ] && [ "$(cat "$G5/winmm.dll.pre-cadence")" = B ]; check $? "both files untouched"

echo "--no-mods"
G6="$T/nomods"; mkdir -p "$G6"; fake_exe "$G6"
ok install_sh "$T/nohome" --gd-dir "$G6" --allow-unknown-version --no-mods; check $? "installs"
[ -f "$G6/winmm.dll" ] && [ ! -e "$G6/mods/hitboxes.dll" ]; check $? "the loader, but no example mods"

if [ -n "$INSTALLER" ] && command -v cygpath >/dev/null 2>&1; then
    echo "the two installers understand each other's manifest (Windows only)"
    G7="$T/interop"; mkdir -p "$G7"; fake_exe "$G7"
    ok install_sh "$T/nohome" --gd-dir "$G7" --allow-unknown-version; check $? "install.sh installs"
    "$INSTALLER" --silent --uninstall --gd-dir "$(cygpath -w "$G7")"; check $? "CadenceInstaller.exe uninstalls it"
    [ ! -e "$G7/winmm.dll" ] && [ ! -e "$G7/mods/hitboxes.dll" ] && [ ! -e "$G7/cadence_install.txt" ]; check $? "nothing of Cadence is left"
    "$INSTALLER" --silent --gd-dir "$(cygpath -w "$G7")" --allow-unknown-version; check $? "CadenceInstaller.exe installs"
    ok install_sh "$T/nohome" --gd-dir "$G7" --uninstall; check $? "install.sh uninstalls it"
    [ ! -e "$G7/winmm.dll" ] && [ ! -e "$G7/mods/hitboxes.dll" ]; check $? "nothing of Cadence is left"
fi

if [ "$failed" -gt 0 ]; then echo; echo "$failed check(s) failed"; exit 1; fi
echo; echo "install.sh test: all passed"
