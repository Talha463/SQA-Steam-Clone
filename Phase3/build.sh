#!/bin/bash
# =============================================================================
# build.sh - compiles the front end (Phase 2 source) into Phase2/steam_clone
# (Windows adds .exe automatically).
# Usage:  ./build.sh        (run from anywhere)
# =============================================================================
cd "$(dirname "$0")/../Phase2" || exit 1

g++ -std=c++17 -Wall -Wextra -g src/*.cpp -o steam_clone \
    && echo "Build OK -> Phase2/steam_clone" \
    || { echo "Build FAILED"; exit 1; }
