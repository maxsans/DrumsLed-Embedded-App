#!/bin/bash

BUILD_DIR="${1:-build/DrumKit}"
APP_BIN="${2:-build/DrumKit.bin}"

BOOTLOADER_PATH="$BUILD_DIR/bootloader/bootloader.bin"
PARTITION_TABLE_PATH="$BUILD_DIR/partition_table/partition-table.bin"
APP_BIN_PATH="$BUILD_DIR/$APP_BIN"

/home/esp/ESP8266_RTOS_SDK/components/esptool_py/esptool/esptool.py --chip esp8266 -p /dev/ttyUSB0 -b 921600 \
--before default_reset --after hard_reset write_flash \
--flash_mode dio --flash_size 2MB --flash_freq 40m \
0x0 "$BOOTLOADER_PATH" \
0x8000 "$PARTITION_TABLE_PATH" \
0x10000 "$APP_BIN_PATH"
