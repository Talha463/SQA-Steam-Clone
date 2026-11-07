# Phase 3 - Front End Requirements Testing (instructions for the TA)

Run all commands from a terminal (Git Bash on Windows, or any Linux/macOS shell) inside the `Phase3` folder.

## Folder contents
| Item | Purpose |
|---|---|
| `phase1_tests/<test name>/input.txt` | Lines typed into the program (stdin) |
| `phase1_tests/<test name>/output.txt` | Expected terminal output |
| `phase1_tests/<test name>/expected.atf` | Expected daily transaction file (optional; checked only if present) |
| `test_data/` | `currentaccounts.txt` and `availablegames.txt` used by every test |
| `test_outputs/<test name>/` | Created by the scripts: `actual.out`, `actual.atf`, `diff_*.txt` |
| `run_history.txt` | One line per test-suite run (pass/fail counts and failed tests) |

## Build the program
```
./build.sh
```
Compiles `../Phase2/src/*.cpp` into `../Phase2/steam_clone` (`.exe` on Windows).

## Run the program by hand
```
../Phase2/steam_clone test_data/currentaccounts.txt test_data/availablegames.txt transout.atf
```
Input is read from the keyboard (stdin), the log is printed to the terminal, and the daily transaction file is appended to `transout.atf` when the user types `logout`.

## Run all tests and check the results
```
./run_all.sh "Run 1"
```
This does three things:
1. `build.sh` - compiles the program.
2. `run_tests.sh` - runs the program on every `input.txt`, saving the terminal log (`actual.out`) and transaction file (`actual.atf`) in `test_outputs/<test name>/`.
3. `check_tests.sh` - compares them with the expected files, prints PASS/FAIL, saves differences in `test_outputs/<test name>/diff_*.txt` (`<` = actual, `>` = expected) and appends a line to `run_history.txt`.

Use a different label for each run (`"Run 2"`, ...). `IGNORE_WS=1 ./run_all.sh "Run 2"` ignores blank-line and spacing differences. To re-run without rebuilding, use `./run_tests.sh && ./check_tests.sh "Run 2"`.
