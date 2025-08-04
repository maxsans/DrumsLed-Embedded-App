import sys
import serial
import argparse
import time
from find_port import select_port


def set_esp_reset(ser, bootloader=False):
    """
    Controls DTR/RTS lines to reset the ESP.
    bootloader=True : flash mode
    bootloader=False : normal execution (restart)
    """
    print("Restarting ESP8266...")
    if bootloader:
        # DTR=0, RTS=1 : bootloader
        ser.dtr = False
        ser.rts = True
    else:
        # DTR=1, RTS=0 then DTR=0, RTS=0 : normal restart
        ser.dtr = False
        ser.rts = True
    time.sleep(0.1)
    # Release reset
    ser.dtr = False
    ser.rts = False
    time.sleep(0.2)  # Wait for ESP to restart
    print("Restart completed.")


def main():
    parser = argparse.ArgumentParser(
        description="Minimalist serial monitor with DTR/RTS control"
    )
    parser.add_argument(
        "-p",
        "--port",
        help="Serial port (ex: /dev/ttyUSB0). If not specified, automatic detection.",
    )
    parser.add_argument(
        "-b",
        "--baud",
        type=int,
        default=115200,
        help="Speed (baudrate), default: 115200",
    )
    parser.add_argument("--boot", action="store_true", help="Force bootloader on reset")
    parser.add_argument(
        "--no-reset", action="store_true", help="Do not restart ESP on startup"
    )
    args = parser.parse_args()

    # Select port
    port = select_port(args.port, auto_select=False, silent=False)
    if not port:
        sys.exit(1)

    try:
        with serial.Serial(port, args.baud, timeout=0.1) as ser:
            # Restart ESP unless --no-reset is specified
            if not args.no_reset:
                set_esp_reset(ser, bootloader=args.boot)

            print(f"Connected to {port} ({args.baud} baud). Ctrl+C to quit.")
            while True:
                line = ser.readline()
                if line:
                    try:
                        print(line.decode("utf-8", errors="replace"), end="")
                    except Exception:
                        print(line)
    except KeyboardInterrupt:
        print("\nStopped by user.")
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
