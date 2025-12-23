#!/bin/bash

BUILD_DIR="${1:-build/DrumKit}"
APP_BIN="${2:-build/DrumKit.bin}"

BOOTLOADER_PATH="$BUILD_DIR/bootloader/bootloader.bin"
PARTITION_TABLE_PATH="$BUILD_DIR/partition_table/partition-table.bin"
APP_BIN_PATH="$BUILD_DIR/$APP_BIN"

# Automatically detect serial port
SERIAL_PORT=$(python3 scripts/find_port.py --auto)
if [ $? -ne 0 ] || [ -z "$SERIAL_PORT" ]; then
    echo "Error: Unable to detect serial port"
    exit 1
fi

echo "Using port: $SERIAL_PORT"

/home/esp/ESP8266_RTOS_SDK/components/esptool_py/esptool/esptool.py --chip esp8266 -p "$SERIAL_PORT" -b 921600 \
--before default_reset --after hard_reset write_flash \
--flash_mode dio --flash_size 2MB --flash_freq 40m \
0x0 "$BOOTLOADER_PATH" \
0x8000 "$PARTITION_TABLE_PATH" \
0x10000 "$APP_BIN_PATH"

python3 scripts/serial_monitor.py --port "$SERIAL_PORT" --baud 115200
