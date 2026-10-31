# Scaffolds a standalone mod with tools/new_mod.py and builds it, so the first thing a new mod author runs cannot go stale.
#   powershell -File tests/run_new_mod_test.ps1 -Repo <repo root> -WorkDir <empty build folder, not under %TEMP%>
param([Parameter(Mandatory)][string]$Repo, [Parameter(Mandatory)][string]$WorkDir)
# Not "Stop": the refusals below print to stderr on purpose, and every check here looks at an exit code.
$ErrorActionPreference = "Continue"
$Repo = (Resolve-Path $Repo).Path
if (Test-Path $WorkDir) { Remove-Item $WorkDir -Recurse -Force }
New-Item -ItemType Directory $WorkDir | Out-Null
$failed = 0
function Check($ok, $what) { if ($ok) { Write-Host "  ok    $what" } else { Write-Host "  FAIL  $what" -ForegroundColor Red; $script:failed++ } }

$py = (Get-Command py -ErrorAction SilentlyContinue); $pyExe = if ($py) { "py" } else { "python" }
& $pyExe "$Repo\tools\new_mod.py" scaffold_probe --standalone $WorkDir --author tester --description "probe" --key J | Out-Null
Check ($LASTEXITCODE -eq 0) "new_mod.py --standalone succeeds"
$proj = Join-Path $WorkDir "scaffold_probe"
Check ((Test-Path "$proj\CMakeLists.txt") -and (Test-Path "$proj\main.cpp") -and (Test-Path "$proj\.gitignore")) "writes CMakeLists.txt, main.cpp and .gitignore"
$cm = Get-Content "$proj\CMakeLists.txt" -Raw
Check (($cm -match 'cmake_minimum_required') -and ($cm -match 'project\(scaffold_probe') -and ($cm -match 'CADENCE_SDK_DIR') -and ($cm -match 'cadence_add_mod\(scaffold_probe')) "the CMakeLists is complete (cmake_minimum_required, project, SDK path, cadence_add_mod)"
Check (($cm -match 'AUTHOR "tester"') -and ($cm -match 'KEY "J"')) "the metadata options are applied"
& $pyExe "$Repo\tools\new_mod.py" scaffold_probe --standalone $WorkDir 2>$null | Out-Null
Check ($LASTEXITCODE -ne 0) "refuses to overwrite an existing project"
& $pyExe "$Repo\tools\new_mod.py" "Bad Name" --standalone $WorkDir 2>$null | Out-Null
Check ($LASTEXITCODE -ne 0) "refuses an invalid name"

$cmake = (Get-Command cmake -ErrorAction SilentlyContinue).Source; if (-not $cmake) { $cmake = "C:\Program Files\CMake\bin\cmake.exe" }
& $cmake -S $proj -B "$proj\build" -G "Visual Studio 17 2022" -A x64 2>&1 | Out-Null
Check ($LASTEXITCODE -eq 0) "configures with no extra flags"
& $cmake --build "$proj\build" --config Release 2>&1 | Out-Null
Check ($LASTEXITCODE -eq 0) "builds"
$dll = "$proj\build\Release\scaffold_probe.dll"
Check (Test-Path $dll) "produces scaffold_probe.dll"
if (Test-Path $dll) {
    $text = [Text.Encoding]::ASCII.GetString([IO.File]::ReadAllBytes($dll))
    Check (($text -match 'mod_main') -and ($text -match 'mod_info')) "the DLL exports mod_main and mod_info"
}
if ($failed) { Write-Host "`n$failed check(s) failed" -ForegroundColor Red; exit 1 }
Write-Host "`nnew_mod test: all passed"
exit 0
