#!/bin/bash

set -e
cd "$(dirname "$0")/.."
sudo apt install cmake ninja-build python3.12 python3.12-venv
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S . -B build
cmake --build build --target algorithm_analysis_homework1 -j 4
cd build
./algorithm_analysis_homework1 ${@:1}
cd ..
python3 -m venv .venv
source .venv/bin/activate
pip install matplotlib numpy pandas scipy
python3 plotter.py
deactivate
