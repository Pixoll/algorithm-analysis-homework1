$ErrorActionPreference = "Stop"
$PSNativeCommandUseErrorActionPreference = $true
Set-StrictMode -Version Latest

Set-Location (Join-Path $PSScriptRoot "..")
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S . -B build
if ($LastExitCode -ne 0) { exit }
cmake --build build --target algorithm_analysis_homework1 -j 4
if ($LastExitCode -ne 0) { exit }
Set-Location build
.\algorithm_analysis_homework1.exe $args
if ($LastExitCode -ne 0) {
    Set-Location (Join-Path $PSScriptRoot "..")
    exit
}
Set-Location ..
python -m venv .venv
.\.venv\Scripts\Activate.ps1
pip install matplotlib numpy pandas scipy
python plotter.py
deactivate
