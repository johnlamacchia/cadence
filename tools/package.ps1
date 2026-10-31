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

$addr = Get-ChildItem generated\addresses_*.json | Select-Object -First 1
$ver = $addr.BaseName -replace "addresses_", ""
$stage = Join-Path $root "dist\Cadence-$ver"
if (Test-Path $stage) { Remove-Item $stage -Recurse -Force }
New-Item -ItemType Directory -Force "$stage\generated", "$stage\mods", "$stage\sdk\loader\include", "$stage\sdk\generated", "$stage\sdk\cmake", "$stage\sdk\template", "$stage\sdk\tools" | Out-Null

Copy-Item build\loader\Release\winmm.dll $stage
Copy-Item generated\addresses_*.json, generated\layout_hashes_*.json "$stage\generated"
Get-ChildItem build\mods -Recurse -Filter *.dll | Where-Object { $_.FullName -like "*\Release\*" } | Copy-Item -Destination "$stage\mods"

Copy-Item loader\include\sdk.h, loader\include\sdk_extras.h, loader\include\sdk_ui.h, loader\include\cadence_abi.h "$stage\sdk\loader\include"
Copy-Item generated\include "$stage\sdk\generated\include" -Recurse
Copy-Item cmake\CadenceMod.cmake "$stage\sdk\cmake"
Copy-Item template\* "$stage\sdk\template"
Copy-Item tools\offset_probe.py, tools\layout_check.py, tools\verify_addresses.py "$stage\sdk\tools"
Copy-Item README.md, THIRD_PARTY_LICENSES.txt $stage
Copy-Item docs "$stage\docs" -Recurse
Copy-Item tools\install.ps1 $stage

$zip = Join-Path $root "dist\Cadence-$ver.zip"
if (Test-Path $zip) { Remove-Item $zip }
Compress-Archive -Path "$stage\*" -DestinationPath $zip
Write-Host "`nPackaged: $zip" -ForegroundColor Green
