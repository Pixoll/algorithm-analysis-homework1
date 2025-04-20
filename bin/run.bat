@echo off

cd /d "%~dp0\.."
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S . -B build
if errorlevel 1 exit /b %errorlevel%
cmake --build build --target algorithm_analysis_homework1 -j 4
if errorlevel 1 exit /b %errorlevel%
cd build
algorithm_analysis_homework1.exe %*
if errorlevel 1 (
  cd /d "%~dp0\.."
  exit /b %errorlevel%
)
cd ..
python -m venv .venv
if errorlevel 1 exit /b %errorlevel%
call .venv\Scripts\activate.bat
pip install matplotlib numpy pandas scipy
python plotter.py
deactivate
