#!/usr/bin/env bash

set -e

PRESET_FILE="CMakePresets.json"

if [[ ! -f "$PRESET_FILE" ]]; then
    echo "Error: $PRESET_FILE not found in current directory."
    exit 1
fi

# Extract all build preset names using jq
if ! command -v jq &> /dev/null; then
    echo "Error: jq is required but not installed. Install it with 'sudo apt install jq'."
    exit 1
fi

PRESETS=$(jq -r '.configurePresets[] | select(.hidden != true) | .name' "$PRESET_FILE")

if [[ -z "$PRESETS" ]]; then
    echo "No build presets found in $PRESET_FILE."
    exit 1
fi

PIDS=()
PRESET_NAMES=()

for PRESET in $PRESETS; do
    BUILD_DIR="build/$PRESET"
    LOG_FILE="$BUILD_DIR/build_${PRESET}.log"
    echo "=== Building with preset: $PRESET ==="
    (
        cmake --preset "$PRESET" &> "$LOG_FILE"
        cmake --build "$BUILD_DIR" &>> "$LOG_FILE"
    ) &
    PIDS+=($!)
    PRESET_NAMES+=("$PRESET")
done

# Wait for all builds to finish and print results
for i in "${!PIDS[@]}"; do
    PID=${PIDS[$i]}
    PRESET=${PRESET_NAMES[$i]}
    LOG_FILE="build_${PRESET}.log"
    if wait $PID; then
        echo "[SUCCESS] $PRESET"
    else
        echo "[FAILED]  $PRESET (see $LOG_FILE)"
    fi
done

echo "All builds completed."