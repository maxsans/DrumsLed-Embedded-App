import sys
import serial
import argparse
import time

def set_esp_reset(ser, bootloader=False):
    """
    Contrôle les lignes DTR/RTS pour resetter l'ESP.
    bootloader=True : mode flash
    bootloader=False : exécution normale
    """
    if bootloader:
        # DTR=0, RTS=1 : bootloader
        ser.dtr = False
        ser.rts = True
    else:
        # DTR=1, RTS=0 : run
        ser.dtr = True
        ser.rts = False
    time.sleep(0.1)
    # Relâcher le reset (laisser RTS à 0)
    if bootloader:
        ser.dtr = False
        ser.rts = False
    else:
        ser.dtr = True
        ser.rts = False
    time.sleep(0.05)

def main():
    parser = argparse.ArgumentParser(description="Moniteur série minimaliste avec contrôle DTR/RTS")
    parser.add_argument('-p', '--port', required=True, help='Port série (ex: /dev/ttyUSB0)')
    parser.add_argument('-b', '--baud', type=int, default=115200, help='Vitesse (baudrate), défaut: 115200')
    parser.add_argument('--boot', action='store_true', help='Forcer le bootloader au reset')
    args = parser.parse_args()

    try:
        with serial.Serial(args.port, args.baud, timeout=0.1) as ser:
            set_esp_reset(ser, bootloader=args.boot)
            print(f"Connecté à {args.port} ({args.baud} bauds). Ctrl+C pour quitter.")
            while True:
                line = ser.readline()
                if line:
                    try:
                        print(line.decode('utf-8', errors='replace'), end='')
                    except Exception:
                        print(line)
    except KeyboardInterrupt:
        print("\nArrêté par l'utilisateur.")
    except Exception as e:
        print(f"Erreur: {e}")
        sys.exit(1)

if __name__ == '__main__':
    main()