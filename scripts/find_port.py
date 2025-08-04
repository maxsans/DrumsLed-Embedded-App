import serial.tools.list_ports
import sys


def find_serial_ports():
    """
    Automatically finds available serial ports.
    Returns a list of found ports.
    """
    ports = serial.tools.list_ports.comports()
    available_ports = []

    for port in ports:
        # Prioritize common USB/ESP ports
        if any(keyword in port.device.lower() for keyword in ["usb", "tty", "com"]):
            available_ports.append(port.device)

    return available_ports


def select_port(specified_port=None, auto_select=False, silent=False):
    """
    Selects the port to use.
    If specified_port is provided, uses it directly.
    If auto_select=True, automatically takes the first port found.
    If silent=True, suppresses status messages (for script usage).
    Otherwise, automatically detects available ports and asks the user.
    """
    if specified_port:
        return specified_port

    ports = find_serial_ports()

    if not ports:
        if not silent:
            print("No serial port detected.", file=sys.stderr)
        return None

    if len(ports) == 1 or auto_select:
        selected_port = ports[0]
        if not silent:
            print(
                f"Port {'automatically selected' if auto_select else 'automatically detected'}: {selected_port}",
                file=sys.stderr,
            )
        return selected_port

    # Multiple ports available, ask the user
    if not silent:
        print("Multiple ports detected:", file=sys.stderr)
        for i, port in enumerate(ports):
            print(f"  {i+1}. {port}", file=sys.stderr)

    try:
        choice = int(input("Choose a port (number): ")) - 1
        if 0 <= choice < len(ports):
            return ports[choice]
        else:
            if not silent:
                print("Invalid choice.", file=sys.stderr)
            return None
    except (ValueError, KeyboardInterrupt):
        if not silent:
            print("\nCancelled by user.", file=sys.stderr)
        return None


def main():
    """
    Standalone script to find and display the serial port.
    Can be used by other bash scripts.
    """
    import argparse

    parser = argparse.ArgumentParser(description="Automatically find serial port")
    parser.add_argument(
        "--auto",
        action="store_true",
        help="Automatically select the first port found",
    )
    args = parser.parse_args()

    port = select_port(auto_select=args.auto, silent=True)
    if port:
        print(port)  # Only print the port to stdout
        sys.exit(0)
    else:
        print("No port found", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
