#!/usr/bin/env bash
# Installs Cadence into a Geometry Dash folder on Linux / Steam Deck (GD running under Proton).
# Run it from the unzipped release, next to winmm.dll:
#
#   bash install.sh                       find Steam's copy of Geometry Dash and install
#   bash install.sh --gd-dir "<folder>"   install somewhere specific
#   bash install.sh --uninstall           remove it again
#   options: --no-mods  --allow-unknown-version
#
# It writes the same files and the same cadence_install.txt as CadenceInstaller.exe, so either
# can uninstall what the other installed. A winmm.dll that is already there is moved to
# winmm.dll.pre-cadence, never overwritten, and put back on uninstall.
#
# IMPORTANT: Proton uses its own winmm.dll unless told otherwise, so Cadence never loads. Set
# Geometry Dash's Steam launch option (Properties > General > Launch Options) to:
#
#   WINEDLLOVERRIDES="winmm=n,b" %command%
set -u

KNOWN_STAMPS="6ad2d987"          # PE timestamp of GeometryDash.exe 2.2091
MANIFEST="cadence_install.txt"
BACKUP="winmm.dll.pre-cadence"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

gd_dir="" uninstall=0 with_mods=1 allow_unknown=0
while [ $# -gt 0 ]; do
    case "$1" in
        --gd-dir) gd_dir="${2:-}"; shift ;;
        --uninstall) uninstall=1 ;;
        --no-mods) with_mods=0 ;;
        --allow-unknown-version) allow_unknown=1 ;;
        -h|--help) sed -n '2,16p' "${BASH_SOURCE[0]}" | sed 's/^# \{0,1\}//'; exit 0 ;;
        *) echo "unknown option: $1 (try --help)" >&2; exit 2 ;;
    esac
    shift
done

say()  { printf '%s\n' "$*"; }
die()  { printf 'error: %s\n' "$*" >&2; exit 1; }

# ---- find Geometry Dash ----------------------------------------------------------------------
steam_roots() {
    printf '%s\n' \
        "$HOME/.steam/steam" \
        "$HOME/.steam/root" \
        "$HOME/.local/share/Steam" \
        "$HOME/.var/app/com.valvesoftware.Steam/.local/share/Steam" \
        "$HOME/.var/app/com.valvesoftware.Steam/data/Steam" \
        "/home/deck/.local/share/Steam"
}

# "path"  "/mnt/games/SteamLibrary" lines of libraryfolders.vdf
library_folders() {
    local root="$1" vdf="$1/steamapps/libraryfolders.vdf"
    printf '%s\n' "$root"
    [ -f "$vdf" ] || return 0
    sed -n 's/^[[:space:]]*"path"[[:space:]]*"\(.*\)"[[:space:]]*$/\1/p' "$vdf" | sed 's/\\\\/\\/g'
}

find_gd() {
    local root lib dir seen=""
    while IFS= read -r root; do
        [ -d "$root" ] || continue
        while IFS= read -r lib; do
            dir="$lib/steamapps/common/Geometry Dash"
            if [ -f "$dir/GeometryDash.exe" ]; then
                real="$(cd "$dir" && pwd -P)"
                case "$seen" in *"|$real|"*) continue ;; esac
                seen="$seen|$real|"
                printf '%s\n' "$dir"
            fi
        done < <(library_folders "$root")
    done < <(steam_roots)
}

# ---- checks ------------------------------------------------------------------------------------
pe_stamp() {     # PE TimeDateStamp of an exe as 8 hex digits, or nothing
    local exe="$1" lfanew
    [ "$(od -An -c -N2 "$exe" 2>/dev/null | tr -d ' ')" = "MZ" ] || return 0
    lfanew=$(od -An -tu4 -j60 -N4 "$exe" | tr -d ' ')
    [ -n "$lfanew" ] || return 0
    od -An -tx4 -j$((lfanew + 8)) -N4 "$exe" | tr -d ' '
}

gd_running() { command -v pgrep >/dev/null 2>&1 && pgrep -f 'GeometryDash\.exe' >/dev/null 2>&1; }

# ---- payload (the files next to this script) -----------------------------------------------------
payload_files() {   # relative paths of what gets installed
    echo "winmm.dll"
    if [ "$with_mods" = 1 ] && [ -d "$HERE/mods" ]; then
        for f in "$HERE"/mods/*.dll; do [ -f "$f" ] && echo "mods/$(basename "$f")"; done
    fi
    for f in "$HERE"/generated/addresses_*.json "$HERE"/generated/layout_hashes_*.json; do
        [ -f "$f" ] || continue
        case "$(basename "$f")" in addresses_rejected_*) continue ;; esac    # a report, not an address file
        echo "generated/$(basename "$f")"
    done
}

# ---- do it ----------------------------------------------------------------------------------------
if [ "$uninstall" = 0 ]; then
    [ -f "$HERE/winmm.dll" ] || die "winmm.dll is not next to install.sh. Run it from the unzipped release."
fi
if [ -z "$gd_dir" ]; then
    found="$(find_gd)"
    [ -n "$found" ] || die "could not find Geometry Dash under Steam. Pass --gd-dir \"<folder with GeometryDash.exe>\"."
    gd_dir="$(printf '%s\n' "$found" | head -n 1)"
    [ "$(printf '%s\n' "$found" | wc -l)" -gt 1 ] && say "found several copies; using the first: $gd_dir"
fi
[ -f "$gd_dir/GeometryDash.exe" ] || die "GeometryDash.exe is not in: $gd_dir"
gd_dir="$(cd "$gd_dir" && pwd)"
say "Geometry Dash folder: $gd_dir"
gd_running && die "Geometry Dash is running. Close it first."

if [ "$uninstall" = 1 ]; then
    backed_up=0
    if [ -f "$gd_dir/$MANIFEST" ]; then
        files="$(sed -n 's/^file //p' "$gd_dir/$MANIFEST")"
        grep -q '^backup winmm.dll' "$gd_dir/$MANIFEST" && backed_up=1
    else
        files="$(payload_files)"          # an install made by hand: remove what this would have written
    fi
    printf '%s\n' "$files" | while IFS= read -r rel; do
        [ -n "$rel" ] && [ -f "$gd_dir/$rel" ] && rm -f "$gd_dir/$rel" && say "removed $rel"
    done
    rm -rf "$gd_dir/mods/.cadence-cache"
    rmdir "$gd_dir/generated" 2>/dev/null; rmdir "$gd_dir/mods" 2>/dev/null
    if [ "$backed_up" = 1 ] && [ -f "$gd_dir/$BACKUP" ] && [ ! -e "$gd_dir/winmm.dll" ]; then
        mv "$gd_dir/$BACKUP" "$gd_dir/winmm.dll" && say "restored the winmm.dll that was here before"
    fi
    rm -f "$gd_dir/$MANIFEST"
    say "Cadence has been removed. Your own mods, settings and logs were left alone."
    exit 0
fi

stamp="$(pe_stamp "$gd_dir/GeometryDash.exe")"
case " $KNOWN_STAMPS " in
    *" $stamp "*) ;;
    *) [ "$allow_unknown" = 1 ] || die "this does not look like Geometry Dash 2.2091 (build stamp '${stamp:-unreadable}'). Cadence only works with that exact version. Use --allow-unknown-version to install anyway." ;;
esac

backed_up=0
[ -f "$gd_dir/$MANIFEST" ] && grep -q '^backup winmm.dll' "$gd_dir/$MANIFEST" && backed_up=1
if [ -e "$gd_dir/winmm.dll" ] && [ ! -f "$gd_dir/$MANIFEST" ]; then
    [ -e "$gd_dir/$BACKUP" ] && die "winmm.dll already exists here and so does $BACKUP. Move one of them away first, so nothing gets overwritten."
    mv "$gd_dir/winmm.dll" "$gd_dir/$BACKUP" || die "could not move the existing winmm.dll aside"
    backed_up=1
    say "existing winmm.dll moved to $BACKUP"
fi

mkdir -p "$gd_dir/generated" "$gd_dir/mods" || die "could not create folders in $gd_dir"
written=""
while IFS= read -r rel; do
    cp -f "$HERE/$rel" "$gd_dir/$rel" || die "could not write $rel"
    written="$written$rel"$'\n'
    say "installed $rel"
done < <(payload_files)

{
    echo "cadence shell-installer"
    [ "$backed_up" = 1 ] && echo "backup winmm.dll"
    printf '%s' "$written" | sed 's/^/file /'
} > "$gd_dir/$MANIFEST"

[ -f "$gd_dir/winmm.dll" ] || die "the install did not complete: winmm.dll is not in $gd_dir"
say ""
say "Installed. One more step, or nothing will load:"
say "  Steam > Geometry Dash > Properties > General > Launch Options, set it to:"
say "      WINEDLLOVERRIDES=\"winmm=n,b\" %command%"
say "Then start the game. Press F6 in game to reload mods. loader.log appears in the game folder."
