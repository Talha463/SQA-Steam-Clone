#!/bin/bash
# =============================================================================
# run_all.sh - one command: build, run every test, check the results.
# Usage: ./run_all.sh ["run label"]      e.g.  ./run_all.sh "Run 1"
# =============================================================================
cd "$(dirname "$0")" || exit 1
./build.sh && ./run_tests.sh && ./check_tests.sh "$1"
