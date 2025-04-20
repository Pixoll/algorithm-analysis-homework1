# Min Distance - Algorithm Analysis class homework 1

## Preparation

### Linux

Nothing to do, all dependencies are automatically installed for you.

### Windows

Make sure to have installed:
- CMake >= 3.10
- Python >= 3.12
- Ninja build

## Executing

Arguments (all are optional):
- `-r`: Number of runs per test case. Should be >= 32. Default = 32.
- `-l`: Range of test cases. Should be > 0. Default = 8.
- `-u`: Range of test cases. Should be > 0. Default = 512.
- `-s`: Step of test cases. Should be > 0. Default = 1.
- `-gmin`: Lowest possible generated value. Default = 0.
- `-gmax`: Greatest possible generated value. Default = 99.

### Linux

```bash
chmod +x bin/run.sh
./bin/run.sh -r RUNS -l LOWER -u UPPER -s STEP -gmin GEN_MIN -gmax GEN_MAX
```

### Windows

Powershell:
```powershell
./bin/run.ps1 -r RUNS -l LOWER -u UPPER -s STEP -gmin GEN_MIN -gmax GEN_MAX
```

Command prompt:
```batch
call ./bin/run.bat -r RUNS -l LOWER -u UPPER -s STEP -gmin GEN_MIN -gmax GEN_MAX
```
