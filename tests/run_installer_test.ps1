# Drives the real CadenceInstaller.exe against throwaway "Geometry Dash" folders (no GD needed).
#   powershell -File tests/run_installer_test.ps1 -Installer build/installer/Release/CadenceInstaller.exe
param([Parameter(Mandatory)][string]$Installer)
$ErrorActionPreference = "Stop"
$Installer = (Resolve-Path $Installer).Path

if (Get-Process GeometryDash -ErrorAction SilentlyContinue) {
    Write-Host "installer test skipped: Geometry Dash is running (the installer refuses to run then, as it should)"
    exit 0
}

$work = Join-Path ([IO.Path]::GetTempPath()) ("cadence_installer_test_" + [guid]::NewGuid().ToString("N").Substring(0, 8))
New-Item -ItemType Directory $work | Out-Null
$log = Join-Path ([IO.Path]::GetTempPath()) "cadence_installer.log"
$failed = 0

function Check($ok, $what) {
    if ($ok) { Write-Host "  ok    $what" } else { Write-Host "  FAIL  $what" -ForegroundColor Red; $script:failed++ }
}

function Run-Installer([string[]]$arguments) {
    Remove-Item $log -ErrorAction SilentlyContinue
    $p = Start-Process -FilePath $Installer -ArgumentList $arguments -Wait -PassThru
    return $p.ExitCode
}

function New-FakeGd([string]$name) {
    $dir = Join-Path $work $name
    New-Item -ItemType Directory $dir | Out-Null
    [IO.File]::WriteAllBytes((Join-Path $dir "GeometryDash.exe"), [byte[]](0x4D, 0x5A, 0, 0))   # not a real exe: unknown version
    return $dir
}

try {
    Write-Host "install into a fresh folder"
    $gd = New-FakeGd "fresh"
    Check ((Run-Installer @("--silent", "--gd-dir", $gd)) -ne 0) "refuses an exe that is not GD 2.2091"
    Check (-not (Test-Path "$gd\winmm.dll")) "  ...and wrote nothing"
    Check ((Run-Installer @("--silent", "--gd-dir", $gd, "--allow-unknown-version")) -eq 0) "installs with --allow-unknown-version"
    Check (Test-Path "$gd\winmm.dll") "winmm.dll is there"
    $mods = @(Get-ChildItem "$gd\mods" -Filter *.dll -ErrorAction SilentlyContinue)
    Check ($mods.Count -ge 15) "example mods are there ($($mods.Count))"
    Check (-not (Test-Path "$gd\mods\qolmod.dll")) "a NOT_BUNDLED mod is not bundled"
    Check (@(Get-ChildItem "$gd\generated" -Filter "addresses_*.json").Count -eq 1) "exactly one address file"
    Check (-not (Test-Path "$gd\generated\addresses_rejected_*.json")) "the 'rejected' report is not installed as an address file"
    Check (@(Get-ChildItem "$gd\generated" -Filter "layout_hashes_*.json").Count -eq 1) "layout hashes are there"
    Check (Test-Path "$gd\cadence_install.txt") "a manifest records what was written"
    Check ((Get-FileHash "$gd\winmm.dll").Hash -eq (Get-FileHash (Join-Path (Split-Path $Installer -Parent) "..\..\loader\Release\winmm.dll")).Hash) "installed winmm.dll is byte-identical to the built loader"

    Write-Host "install again (upgrade in place)"
    Check ((Run-Installer @("--silent", "--gd-dir", $gd, "--allow-unknown-version")) -eq 0) "second install succeeds"

    Write-Host "uninstall leaves the user's own files alone"
    "keep me" | Set-Content "$gd\mods\my_own_mod.dll"
    "keep me too" | Set-Content "$gd\qolmod.ini"
    Check ((Run-Installer @("--silent", "--uninstall", "--gd-dir", $gd)) -eq 0) "uninstall succeeds"
    Check (-not (Test-Path "$gd\winmm.dll")) "winmm.dll is gone"
    Check (-not (Test-Path "$gd\mods\hitboxes.dll")) "bundled mods are gone"
    Check (-not (Test-Path "$gd\cadence_install.txt")) "the manifest is gone"
    Check (Test-Path "$gd\mods\my_own_mod.dll") "the user's own mod is untouched"
    Check (Test-Path "$gd\qolmod.ini") "the user's settings file is untouched"
    Check (-not (Test-Path "$gd\generated")) "the empty generated/ folder was removed"

    Write-Host "a winmm.dll that was already there is never overwritten"
    $gd2 = New-FakeGd "foreign"
    "FOREIGN LOADER" | Set-Content "$gd2\winmm.dll"
    Check ((Run-Installer @("--silent", "--gd-dir", $gd2, "--allow-unknown-version")) -eq 0) "install succeeds"
    Check ((Get-Content "$gd2\winmm.dll.pre-cadence" -Raw).Trim() -eq "FOREIGN LOADER") "the other loader was moved aside intact"
    Check ((Get-Item "$gd2\winmm.dll").Length -gt 1000) "Cadence's winmm.dll took its place"
    Check ((Run-Installer @("--silent", "--uninstall", "--gd-dir", $gd2)) -eq 0) "uninstall succeeds"
    Check ((Get-Content "$gd2\winmm.dll" -Raw).Trim() -eq "FOREIGN LOADER") "the other loader is back in place"
    Check (-not (Test-Path "$gd2\winmm.dll.pre-cadence")) "no backup left over"

    Write-Host "refuses to clobber an existing backup"
    $gd3 = New-FakeGd "twice"
    "A" | Set-Content "$gd3\winmm.dll"; "B" | Set-Content "$gd3\winmm.dll.pre-cadence"
    Check ((Run-Installer @("--silent", "--gd-dir", $gd3, "--allow-unknown-version")) -ne 0) "fails instead of overwriting"
    Check ((Get-Content "$gd3\winmm.dll" -Raw).Trim() -eq "A" -and (Get-Content "$gd3\winmm.dll.pre-cadence" -Raw).Trim() -eq "B") "both files untouched"

    Write-Host "--no-mods"
    $gd4 = New-FakeGd "nomods"
    Check ((Run-Installer @("--silent", "--gd-dir", $gd4, "--allow-unknown-version", "--no-mods")) -eq 0) "installs"
    Check ((Test-Path "$gd4\winmm.dll") -and -not (Test-Path "$gd4\mods\hitboxes.dll")) "the loader, but no example mods"

    Write-Host "uninstall an install that has no manifest (made by hand or install.ps1)"
    $gd5 = New-FakeGd "nomanifest"
    Run-Installer @("--silent", "--gd-dir", $gd5, "--allow-unknown-version") | Out-Null
    Remove-Item "$gd5\cadence_install.txt"
    Check ((Run-Installer @("--silent", "--uninstall", "--gd-dir", $gd5)) -eq 0) "uninstall succeeds"
    Check (-not (Test-Path "$gd5\winmm.dll") -and -not (Test-Path "$gd5\mods\hitboxes.dll")) "what it would have installed is removed"

    Write-Host "not a Geometry Dash folder"
    $empty = Join-Path $work "empty"; New-Item -ItemType Directory $empty | Out-Null
    Check ((Run-Installer @("--silent", "--gd-dir", $empty, "--allow-unknown-version")) -ne 0) "refuses a folder with no GeometryDash.exe"
    Check (-not (Test-Path "$empty\winmm.dll")) "  ...and wrote nothing"

    $real = Join-Path (Split-Path $Installer -Parent) "..\..\..\binaries\2.2091\GeometryDash.exe"
    if (Test-Path $real) {
        Write-Host "the real 2.2091 exe is recognised (this machine only)"
        $gd6 = Join-Path $work "real"; New-Item -ItemType Directory $gd6 | Out-Null
        Copy-Item $real "$gd6\GeometryDash.exe"
        Check ((Run-Installer @("--silent", "--gd-dir", $gd6)) -eq 0) "installs with no override flag"
    }

    Write-Host "the window"
    $p = Start-Process -FilePath $Installer -ArgumentList "--gui-smoke" -Wait -PassThru
    Check ($p.ExitCode -eq 0) "opens, builds its controls and closes cleanly"
}
finally {
    Remove-Item $work -Recurse -Force -ErrorAction SilentlyContinue
}

if ($failed) {
    Write-Host "`n$failed check(s) failed. Last installer log:" -ForegroundColor Red
    if (Test-Path $log) { Get-Content $log -Tail 15 }
    exit 1
}
Write-Host "`ninstaller test: all passed"
exit 0
