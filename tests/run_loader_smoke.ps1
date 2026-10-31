# Loader + mods integration test. Needs no Geometry Dash.
#   powershell -File tests/run_loader_smoke.ps1 -BuildDir build
param([string]$BuildDir = "build", [string]$Config = "Release")
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$build = if ([IO.Path]::IsPathRooted($BuildDir)) { $BuildDir } else { Join-Path $root $BuildDir }
$work = Join-Path $build "smoke"
$failures = @()
function Check($ok, $msg) { if ($ok) { Write-Host "  ok   $msg" } else { Write-Host "  FAIL $msg" -ForegroundColor Red; $script:failures += $msg } }

$modDlls = Get-ChildItem "$build\mods" -Recurse -Filter *.dll | Where-Object { $_.FullName -like "*\$Config\*" }
$expectedMods = @($modDlls | ForEach-Object { $_.Name } | Sort-Object)

function Stage($hashFile) {
    if (Test-Path $work) { Remove-Item $work -Recurse -Force }
    New-Item -ItemType Directory "$work\mods", "$work\generated" | Out-Null
    Copy-Item "$build\$Config\loader_smoke.exe" $work
    Copy-Item "$build\loader\$Config\winmm.dll" $work
    $modDlls | Copy-Item -Destination "$work\mods"
    # An empty address table: every function lookup fails, so no hook is ever installed.
    Set-Content "$work\generated\addresses_2.2091.json" '{"version":"2.2091","count":0,"addresses":{}}' -Encoding ascii
    Set-Content "$work\generated\layout_hashes_2.2091.json" $hashFile -Encoding ascii
}
function RunSmoke() {
    $p = Start-Process "$work\loader_smoke.exe" -WorkingDirectory $work -PassThru -Wait -NoNewWindow
    Check ($p.ExitCode -eq 0) "loader_smoke exited cleanly (exit code $($p.ExitCode))"
    return Get-Content "$work\loader.log"
}

Write-Host "Mod exports (metadata + layout hashes match the data file)"
$probe = Join-Path $build "smoke_mods_probe"
if (Test-Path $probe) { Remove-Item $probe -Recurse -Force }
New-Item -ItemType Directory $probe | Out-Null
$modDlls | Copy-Item -Destination $probe
& py -I "$root\tests\mod_exports_check.py" $probe | Select-Object -Last 1 | ForEach-Object { Write-Host "  $_" }
Check ($LASTEXITCODE -eq 0) "every mod exports mod_main / mod_info and current layout hashes"
Remove-Item $probe -Recurse -Force

Write-Host "Scenario 1: current layout hashes -> every mod loads"
$good = Get-Content "$root\generated\layout_hashes_2.2091.json" -Raw
Stage $good
$log = RunSmoke
$initialized = @($log | Select-String "mod '(.+?)' initialized" | ForEach-Object { $_.Matches[0].Groups[1].Value })
Check ($initialized.Count -eq $expectedMods.Count) "all $($expectedMods.Count) mods initialized (got $($initialized.Count))"
Check (($initialized -join ",") -eq (($initialized | Sort-Object) -join ",")) "mods loaded in alphabetical order"
Check (@($log | Select-String "address not found").Count -gt 0) "missing addresses are logged and skipped, not fatal"
Check (-not ($log | Select-String "refusing to load")) "no mod refused"
Check (@(Get-ChildItem "$work\mods\.cadence-cache" -Filter *.dll).Count -eq $expectedMods.Count) "mods were loaded from shadow copies"

Write-Host "Scenario 2: PlayLayer's layout changed -> mods that use it are refused, by class name"
$json = $good | ConvertFrom-Json
$json.hashes.PlayLayer = "0000000000000000"
Stage ($json | ConvertTo-Json -Depth 5)
$log = RunSmoke
$refused = @($log | Select-String "refusing to load")
Check ($refused.Count -gt 0) "stale mods are refused ($($refused.Count))"
Check (@($refused | Where-Object { $_.Line -notmatch "PlayLayer" }).Count -eq 0) "every refusal message names the class (PlayLayer)"
$loaded = @($log | Select-String "mod '(.+?)' initialized" | ForEach-Object { $_.Matches[0].Groups[1].Value })
foreach ($m in "anticheat.dll", "example.dll", "fpsdisplay.dll", "modmenu.dll") {
    Check ($loaded -contains $m) "$m does not use PlayLayer and still loads"
}
Check (-not ($loaded -contains "noclip.dll")) "noclip.dll (uses PlayLayer) was not initialized"

Write-Host "Scenario 3: an address that is not inside the exe's code is refused, and the hook is skipped"
Stage $good
Set-Content "$work\generated\addresses_2.2091.json" '{"version":"2.2091","count":1,"addresses":{"GJBaseGameLayer::checkSpawnAbuse()":"0x9999999"}}' -Encoding ascii
$log = RunSmoke
Check (@($log | Select-String "ignored 1 address.*not inside GeometryDash.exe").Count -eq 1) "the placeholder address is rejected at load, with an example in the log"
Check (@($log | Select-String "address not found: GJBaseGameLayer::checkSpawnAbuse").Count -eq 1) "the mod that asked for it is told 'not found' and skips the hook"
Check (@($log | Select-String "mod 'anticheat.dll' initialized").Count -eq 1) "the mod still loads and the game is not touched"

if ($failures.Count) { Write-Host "`n$($failures.Count) check(s) failed" -ForegroundColor Red; exit 1 }
Write-Host "`nloader smoke test passed" -ForegroundColor Green
