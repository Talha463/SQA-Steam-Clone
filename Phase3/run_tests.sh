#!/bin/bash
# =============================================================================
# run_tests.sh - runs the front end once for every test case.
#
# Each test case is a folder inside phase1_tests/ containing:
#     input.txt    - the lines typed into the program (stdin)
#     output.txt   - the EXPECTED terminal output
#     expected.atf - (optional) the EXPECTED daily transaction file
#
# For every test this script saves, in test_outputs/<test name>/ :
#     actual.out   - what the program printed to the terminal
#     actual.atf   - the daily transaction file the program wrote
#
# Usage:  ./run_tests.sh        (run from anywhere)
#
# Settings can be overridden without editing the file, e.g.:
#     PROGRAM=/path/to/steam_clone ./run_tests.sh
# =============================================================================

# Always work from the folder this script lives in, so paths never break
cd "$(dirname "$0")" || exit 1

# Find the compiled program (Windows builds end in .exe)
if [ -z "$PROGRAM" ]; then
    for candidate in ../Phase2/steam_clone.exe ../Phase2/steam_clone; do
        [ -x "$candidate" ] && PROGRAM="$candidate" && break
    done
fi
TESTS_DIR="${TESTS_DIR:-phase1_tests}"                 # one sub-folder per test
RESULTS_DIR="${RESULTS_DIR:-test_outputs}"             # actual results go here
ACCOUNTS="${ACCOUNTS:-test_data/currentaccounts.txt}"  # current user accounts
GAMES="${GAMES:-test_data/availablegames.txt}"         # available games
COLLECTION="${COLLECTION:-test_data/gamescollection.txt}"  # games each user owns

# Stop early with a clear message if something is missing
if [ -z "$PROGRAM" ] || [ ! -x "$PROGRAM" ]; then
    echo "ERROR: program not found. Build it first with ./build.sh"
    exit 1
fi
for needed in "$TESTS_DIR" "$ACCOUNTS" "$GAMES" "$COLLECTION"; do
    if [ ! -e "$needed" ]; then
        echo "ERROR: '$needed' not found."
        exit 1
    fi
done

# Start from a clean results folder so old runs never get mixed in
rm -rf "$RESULTS_DIR"
mkdir -p "$RESULTS_DIR"

count=0
for test_path in "$TESTS_DIR"/*/; do
    name=$(basename "$test_path")
    [ -f "$test_path/input.txt" ] || continue    # skip folders with no input

    echo "running test: $name"
    mkdir -p "$RESULTS_DIR/$name"

    # The program APPENDS to its transaction file, so each test starts with
    # no file at all. stdin <- input.txt, stdout -> actual.out
    rm -f "$RESULTS_DIR/$name/actual.atf"
    "$PROGRAM" "$ACCOUNTS" "$GAMES" "$COLLECTION" "$RESULTS_DIR/$name/actual.atf" \
        < "$test_path/input.txt" > "$RESULTS_DIR/$name/actual.out" 2>&1

    # A test with no transactions leaves no file; create an empty one so the
    # checker can still compare it.
    touch "$RESULTS_DIR/$name/actual.atf"

    count=$((count+1))
done

echo "Finished: ran $count tests. Results are in Phase3/$RESULTS_DIR/"
