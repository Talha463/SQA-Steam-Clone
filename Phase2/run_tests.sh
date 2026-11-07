#!/bin/bash
# =============================================================================
# run_tests.sh - runs the front end once for every test case.
#
# Each test case is a folder inside $TESTS_DIR containing:
#     input.txt    - the lines typed into the program (stdin)
#     output.txt   - the EXPECTED terminal output
#     expected.atf - (optional) the EXPECTED daily transaction file
#
# For every test this script saves, in $RESULTS_DIR/<test name>/ :
#     actual.out   - what the program printed to the terminal
#     actual.atf   - the daily transaction file the program wrote
#
# Usage (from the project root):
#     ./run_tests.sh
#
# Settings can be overridden without editing the file, e.g.:
#     PROGRAM=./build/steam_clone ./run_tests.sh
# =============================================================================

PROGRAM="${PROGRAM:-./steam_clone}"            # compiled front end
TESTS_DIR="${TESTS_DIR:-phase1_tests}"         # one sub-folder per test
RESULTS_DIR="${RESULTS_DIR:-test_outputs}"     # actual results go here
ACCOUNTS="${ACCOUNTS:-test_data/currentaccounts.txt}"   # current user accounts
GAMES="${GAMES:-test_data/availablegames.txt}"          # available games

# Stop early with a clear message if something is missing
if [ ! -x "$PROGRAM" ]; then
    echo "ERROR: program '$PROGRAM' not found. Build it first (see README)."
    exit 1
fi
if [ ! -d "$TESTS_DIR" ]; then
    echo "ERROR: tests folder '$TESTS_DIR' not found."
    exit 1
fi

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
    "$PROGRAM" "$ACCOUNTS" "$GAMES" "$RESULTS_DIR/$name/actual.atf" \
        < "$test_path/input.txt" > "$RESULTS_DIR/$name/actual.out" 2>&1

    # A test with no transactions leaves no file; create an empty one so the
    # checker can still compare it.
    touch "$RESULTS_DIR/$name/actual.atf"

    count=$((count+1))
done

echo "Finished: ran $count tests. Results are in $RESULTS_DIR/"
