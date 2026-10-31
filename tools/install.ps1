# Installs Cadence into your Geometry Dash folder.
#   powershell -File install.ps1                         (auto-detects the Steam install)
#   powershell -File install.ps1 -GdDir "D:\Games\Geometry Dash"
#   powershell -File install.ps1 -Uninstall
param(
    [string]$GdDir,
    [switch]$Uninstall
)
$ErrorActionPreference = "Stop"

if (-not $GdDir) {
    $candidates = @(
        "C:\Program Files (x86)\Steam\steamapps\common\Geometry Dash",
        "C:\Program Files\Steam\steamapps\common\Geometry Dash"
    )
    $GdDir = $candidates | Where-Object { Test-Path "$_\GeometryDash.exe" } | Select-Object -First 1
}
if (-not $GdDir -or -not (Test-Path "$GdDir\GeometryDash.exe")) {
    throw "GeometryDash.exe not found. Pass -GdDir <folder>."
}

if ($Uninstall) {
    Remove-Item "$GdDir\winmm.dll" -Force -ErrorAction SilentlyContinue
    Write-Host "Removed winmm.dll from $GdDir (mods/ and generated/ left in place)."
    return
}

# This script lives in the package root next to winmm.dll; in the repo it lives in tools/.
$src = if (Test-Path "$PSScriptRoot\winmm.dll") { $PSScriptRoot } else { Join-Path (Split-Path -Parent $PSScriptRoot) "build\loader\Release" }
if (-not (Test-Path "$src\winmm.dll")) { throw "winmm.dll not found next to this script" }

Copy-Item "$src\winmm.dll" $GdDir -Force
$pkg = if (Test-Path "$PSScriptRoot\generated") { $PSScriptRoot } else { Split-Path -Parent $PSScriptRoot }
New-Item -ItemType Directory -Force "$GdDir\generated", "$GdDir\mods" | Out-Null
Get-ChildItem "$pkg\generated\addresses_*.json" | Where-Object { $_.Name -notlike "addresses_rejected_*" } | Copy-Item -Destination "$GdDir\generated" -Force
Copy-Item "$pkg\generated\layout_hashes_*.json" "$GdDir\generated" -Force
if (Test-Path "$PSScriptRoot\mods") {
    Copy-Item "$PSScriptRoot\mods\*.dll" "$GdDir\mods" -Force
} else {
    # source checkout: take the freshly built mods
    Get-ChildItem "$pkg\build\mods" -Recurse -Filter *.dll -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -like "*\Release\*" } | Copy-Item -Destination "$GdDir\mods" -Force
}

Write-Host "Cadence installed to $GdDir" -ForegroundColor Green
Write-Host "Launch Geometry Dash. Mods live in $GdDir\mods; press F6 in game to reload them."
