# Builds everything and produces dist/Cadence-<gd version>.zip
#   powershell -File tools/package.ps1
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
Set-Location $root

$cmake = (Get-Command cmake -ErrorAction SilentlyContinue).Source
if (-not $cmake) { $cmake = "C:\Program Files\CMake\bin\cmake.exe" }

& $cmake -B build -G "Visual Studio 17 2022" -A x64
& $cmake --build build --config Release
if ($LASTEXITCODE -ne 0) { throw "build failed" }

& $cmake --build build --config Release --target gd_headers_check
if ($LASTEXITCODE -ne 0) { throw "the generated GD headers do not compile (a layout static_assert failed?)" }

& "$root\build\Release\hook_test.exe"
if ($LASTEXITCODE -ne 0) { throw "hook_test failed" }
powershell -NoProfile -File "$root\tests\run_loader_smoke.ps1" -BuildDir build
if ($LASTEXITCODE -ne 0) { throw "loader smoke test failed" }

& $cmake --build build --config Release --target CadenceInstaller
if ($LASTEXITCODE -ne 0) { throw "installer build failed" }
powershell -NoProfile -ExecutionPolicy Bypass -File "$root\tests\run_installer_test.ps1" -Installer "$root\build\installer\Release\CadenceInstaller.exe"
if ($LASTEXITCODE -ne 0) { throw "installer test failed" }

$addr = Get-ChildItem generated\addresses_*.json | Where-Object { $_.Name -notlike "addresses_rejected_*" } | Select-Object -First 1
$ver = $addr.BaseName -replace "addresses_", ""
$stage = Join-Path $root "dist\Cadence-$ver"
if (Test-Path $stage) { Remove-Item $stage -Recurse -Force }
New-Item -ItemType Directory -Force "$stage\generated", "$stage\mods", "$stage\sdk\loader\include", "$stage\sdk\generated", "$stage\sdk\cmake", "$stage\sdk\template", "$stage\sdk\tools" | Out-Null

Copy-Item build\loader\Release\winmm.dll $stage
Get-ChildItem generated\addresses_*.json | Where-Object { $_.Name -notlike "addresses_rejected_*" } | Copy-Item -Destination "$stage\generated"
Copy-Item generated\layout_hashes_*.json "$stage\generated"
Get-ChildItem build\mods -Recurse -Filter *.dll | Where-Object { $_.FullName -like "*\Release\*" } | Copy-Item -Destination "$stage\mods"

Copy-Item loader\include\sdk.h, loader\include\sdk_extras.h, loader\include\sdk_ui.h, loader\include\cadence_abi.h "$stage\sdk\loader\include"
Copy-Item generated\include "$stage\sdk\generated\include" -Recurse
Copy-Item cmake\CadenceMod.cmake "$stage\sdk\cmake"
Copy-Item template\* "$stage\sdk\template"
Copy-Item tools\offset_probe.py, tools\layout_check.py, tools\rtti_sizes.py, tools\offset_audit.py, tools\verify_addresses.py "$stage\sdk\tools"
Copy-Item README.md, LICENSE, THIRD_PARTY_LICENSES.txt $stage
Copy-Item docs "$stage\docs" -Recurse
Copy-Item tools\install.ps1 $stage

$zip = Join-Path $root "dist\Cadence-$ver.zip"
if (Test-Path $zip) { Remove-Item $zip }
# Not Compress-Archive or ZipFile.CreateFromDirectory: on Windows PowerShell 5.1 both write backslashes into
# the entry names, which 7-Zip, unzip and most other tools reject. Entries are added one by one so the
# names can be given with the forward slashes the zip format requires.
Add-Type -AssemblyName System.IO.Compression, System.IO.Compression.FileSystem
$stream = [System.IO.File]::Create($zip)
$archive = New-Object System.IO.Compression.ZipArchive($stream, [System.IO.Compression.ZipArchiveMode]::Create)
try {
    Get-ChildItem $stage -Recurse -File | ForEach-Object {
        $entryName = $_.FullName.Substring($stage.Length + 1).Replace('\', '/')
        [void][System.IO.Compression.ZipFileExtensions]::CreateEntryFromFile($archive, $_.FullName, $entryName, [System.IO.Compression.CompressionLevel]::Optimal)
    }
} finally {
    $archive.Dispose()
    $stream.Dispose()
}
# The installer is the download for players; the zip is for mod developers.
$installer = Join-Path $root "dist\CadenceInstaller.exe"
Copy-Item "$root\build\installer\Release\CadenceInstaller.exe" $installer -Force
Write-Host "`nPackaged: $zip" -ForegroundColor Green
Write-Host "Installer: $installer  (sha256 $((Get-FileHash $installer).Hash))" -ForegroundColor Green
