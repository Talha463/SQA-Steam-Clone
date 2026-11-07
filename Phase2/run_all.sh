#!/bin/bash
# =============================================================================
# run_all.sh - one command that runs every test and checks the results.
# Usage: ./run_all.sh ["run label"]      e.g.  ./run_all.sh "Run 1"
# =============================================================================
./run_tests.sh && ./check_tests.sh "$1"
