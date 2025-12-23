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
    mkdir -p "$BUILD_DIR"
    LOG_FILE="$BUILD_DIR/build_${PRESET}.log"
    echo "=== Building with preset: $PRESET ==="

    # Extract environment variables from preset if any
    ENV_VARS=$(jq -r --arg preset "$PRESET" '.configurePresets[] | select(.name == $preset) | .environment // {} | to_entries[] | "\(.key)=\(.value)"' "$PRESET_FILE")

    (
        # Export environment variables for slave builds
        if [[ -n "$ENV_VARS" ]]; then
            while IFS= read -r env_var; do
                if [[ -n "$env_var" ]]; then
                    export "$env_var"
                    echo "Exported: $env_var" >> "$LOG_FILE"
                fi
            done <<< "$ENV_VARS"
        fi

        # Ensure IDF_PATH is set for ESP8266 builds
        if [[ "$PRESET" == *"Kit" ]] || [[ "$PRESET" == *"slave"* ]]; then
            if [[ -z "$IDF_PATH" ]]; then
                export IDF_PATH="/home/esp/ESP8266_RTOS_SDK"
                echo "Set IDF_PATH=$IDF_PATH" >> "$LOG_FILE"
            fi

            # Ensure the toolchain is in PATH
            if [[ ":$PATH:" != *":/home/esp/xtensa-lx106-elf/bin:"* ]]; then
                export PATH="/home/esp/xtensa-lx106-elf/bin:$PATH"
                echo "Updated PATH to include xtensa toolchain" >> "$LOG_FILE"
            fi
        fi

        # Print environment for debugging
        echo "=== Environment Variables ===" >> "$LOG_FILE"
        env | grep -E "^(IDF_PATH|PATH)=" >> "$LOG_FILE" || true
        echo "============================" >> "$LOG_FILE"

        cmake --preset "$PRESET" &> "$LOG_FILE"
        cmake --build "$BUILD_DIR" &>> "$LOG_FILE"
    ) &
    PIDS+=($!)
    PRESET_NAMES+=("$PRESET")
done

# Wait for all builds to finish and print results
SUCCESS_PRESETS=()
for i in "${!PIDS[@]}"; do
    PID=${PIDS[$i]}
    PRESET=${PRESET_NAMES[$i]}
    LOG_FILE="build/${PRESET}/build_${PRESET}.log"
    if wait $PID; then
        echo "[SUCCESS] $PRESET"
        SUCCESS_PRESETS+=("$PRESET")
    else
        echo "[FAILED]  $PRESET (see ${LOG_FILE})"
    fi
done

# Collect binaries from successful builds
if [[ ${#SUCCESS_PRESETS[@]} -gt 0 ]]; then
    BINARIES_DIR="build/tunes-app"
    SLAVE_BINARIES_DIR="${BINARIES_DIR}/slave-bin"
    mkdir -p "$BINARIES_DIR"
    mkdir -p "$SLAVE_BINARIES_DIR"

    echo "=== Collecting binaries ==="
    BINARY_COUNT=0

    for PRESET in "${SUCCESS_PRESETS[@]}"; do
        BUILD_DIR="build/$PRESET"

        if [[ "$PRESET" == "master"* ]]; then
            DEST_DIR="$BINARIES_DIR"
        else
            # Create preset-specific subfolder for slave binaries
            PRESET_DIR="$SLAVE_BINARIES_DIR/$PRESET"
            mkdir -p "$PRESET_DIR"
            DEST_DIR="$PRESET_DIR"
        fi

        # For Kit presets, only copy .bin files
        if [[ "$PRESET" == *"Kit" ]]; then
            BINARY_FILES=$(find "$BUILD_DIR" -type f -name "*.bin" -not -path "*/CMakeFiles/*" 2>/dev/null || true)
        else
            # For other targets, copy common binary types and executables
            BINARY_FILES=$(find "$BUILD_DIR" -type f \( -name "*.bin" -o -name "*.elf" -o -name "*.hex" -o -name "*.uf2" \) -not -path "*/CMakeFiles/*" 2>/dev/null || true)
            EXECUTABLES=$(find "$BUILD_DIR" -type f -executable -not -path "*/CMakeFiles/*" -not -name "cmake*" -not -name "*.cmake" -not -name "Makefile*" 2>/dev/null || true)
            BINARY_FILES=$(echo -e "$BINARY_FILES\n$EXECUTABLES" | sort -u | grep -v '^$' || true)
        fi

        # Copy binaries to the destination directory
        for BINARY in $BINARY_FILES; do
            if [[ -f "$BINARY" && -s "$BINARY" ]]; then
                FILENAME=$(basename "$BINARY")

                # Ensure destination directory exists
                mkdir -p "$DEST_DIR"

                # Temporarily disable exit on error for the copy operation
                set +e
                cp "$BINARY" "$DEST_DIR/$FILENAME" 2>/dev/null
                if [[ $? -eq 0 ]]; then
                    ((BINARY_COUNT++))
                fi
                set -e
            fi
        done
    done

    if [[ $BINARY_COUNT -gt 0 ]]; then
        echo "Collected $BINARY_COUNT binaries in $BINARIES_DIR/"
    else
        echo "No binaries found in successful builds"
    fi
fi

echo "All builds completed."
