#!/bin/bash
# =============================================================================
# check_tests.sh - compares ACTUAL results with EXPECTED results.
#
# For every test it checks:
#   1. terminal output : actual.out  vs  output.txt     (expected terminal)
#   2. transaction file: actual.atf  vs  expected.atf   (only if expected.atf exists)
#
# It prints PASS / FAIL per check, saves the differences to
# $RESULTS_DIR/<test>/diff_*.txt, and appends one line to $HISTORY_FILE so
# every run of the suite is recorded (handy for the multi-run failure table).
#
# Usage:   ./check_tests.sh [run label]
#   e.g.   ./check_tests.sh "Run 1"
# Set IGNORE_WS=1 to ignore blank lines / spacing differences:
#   IGNORE_WS=1 ./check_tests.sh "Run 2"
# =============================================================================

TESTS_DIR="${TESTS_DIR:-phase1_tests}"
RESULTS_DIR="${RESULTS_DIR:-test_outputs}"
HISTORY_FILE="${HISTORY_FILE:-run_history.txt}"
LABEL="${1:-$(date '+%Y-%m-%d %H:%M')}"

# diff options: -B ignores blank lines, -b ignores spacing changes
DIFF_OPTS=""
[ "$IGNORE_WS" = "1" ] && DIFF_OPTS="-B -b"

pass=0
fail=0
failed_tests=""

# compare_files <actual> <expected> <diff output file> <description>
compare_files() {
    if diff $DIFF_OPTS "$1" "$2" > "$3"; then
        echo "PASS ($4)"
        pass=$((pass+1))
        rm -f "$3"                      # no differences, no diff file needed
    else
        echo "FAIL ($4)  -> see $3"
        fail=$((fail+1))
        failed_tests="$failed_tests [$name: $4]"
    fi
}

for test_path in "$TESTS_DIR"/*/; do
    name=$(basename "$test_path")
    [ -f "$test_path/input.txt" ] || continue
    res="$RESULTS_DIR/$name"

    echo "--- $name"
    compare_files "$res/actual.out" "$test_path/output.txt" \
                  "$res/diff_terminal.txt" "terminal output"

    if [ -f "$test_path/expected.atf" ]; then
        compare_files "$res/actual.atf" "$test_path/expected.atf" \
                      "$res/diff_transactions.txt" "transaction file"
    else
        echo "SKIP (transaction file): no expected.atf for this test"
    fi
done

echo "=============================="
echo "Passed: $pass   Failed: $fail"

# Record this run so results from several runs can go into the failure table
echo "$LABEL | passed=$pass failed=$fail |$failed_tests" >> "$HISTORY_FILE"
echo "Run recorded in $HISTORY_FILE"
