import sys
import serial
import serial.tools.list_ports
import argparse
import time


def find_serial_ports():
    """
    Trouve automatiquement les ports série disponibles.
    Retourne une liste des ports trouvés.
    """
    ports = serial.tools.list_ports.comports()
    available_ports = []
    
    for port in ports:
        # Prioriser les ports USB/ESP communs
        if any(keyword in port.device.lower() for keyword in ['usb', 'tty', 'com']):
            available_ports.append(port.device)
    
    return available_ports


def select_port(specified_port=None):
    """
    Sélectionne le port à utiliser.
    Si specified_port est fourni, l'utilise directement.
    Sinon, détecte automatiquement les ports disponibles.
    """
    if specified_port:
        return specified_port
    
    ports = find_serial_ports()
    
    if not ports:
        print("Aucun port série détecté.")
        return None
    
    if len(ports) == 1:
        print(f"Port détecté automatiquement: {ports[0]}")
        return ports[0]
    
    # Plusieurs ports disponibles, demander à l'utilisateur
    print("Plusieurs ports détectés:")
    for i, port in enumerate(ports):
        print(f"  {i+1}. {port}")
    
    try:
        choice = int(input("Choisissez un port (numéro): ")) - 1
        if 0 <= choice < len(ports):
            return ports[choice]
        else:
            print("Choix invalide.")
            return None
    except (ValueError, KeyboardInterrupt):
        print("\nAnnulé par l'utilisateur.")
        return None


def set_esp_reset(ser, bootloader=False):
    """
    Contrôle les lignes DTR/RTS pour resetter l'ESP.
    bootloader=True : mode flash
    bootloader=False : exécution normale (redémarrage)
    """
    print("Redémarrage de l'ESP8266...")
    if bootloader:
        # DTR=0, RTS=1 : bootloader
        ser.dtr = False
        ser.rts = True
    else:
        # DTR=1, RTS=0 puis DTR=0, RTS=0 : redémarrage normal
        ser.dtr = False
        ser.rts = True
    time.sleep(0.1)
    # Relâcher le reset
    ser.dtr = False
    ser.rts = False
    time.sleep(0.2)  # Attendre que l'ESP redémarre
    print("Redémarrage terminé.")


def main():
    parser = argparse.ArgumentParser(
        description="Moniteur série minimaliste avec contrôle DTR/RTS"
    )
    parser.add_argument(
        "-p", "--port", help="Port série (ex: /dev/ttyUSB0). Si non spécifié, détection automatique."
    )
    parser.add_argument(
        "-b",
        "--baud",
        type=int,
        default=115200,
        help="Vitesse (baudrate), défaut: 115200",
    )
    parser.add_argument(
        "--boot", action="store_true", help="Forcer le bootloader au reset"
    )
    parser.add_argument(
        "--no-reset", action="store_true", help="Ne pas redémarrer l'ESP au démarrage"
    )
    args = parser.parse_args()

    # Sélectionner le port
    port = select_port(args.port)
    if not port:
        sys.exit(1)

    try:
        with serial.Serial(port, args.baud, timeout=0.1) as ser:
            # Redémarrer l'ESP sauf si --no-reset est spécifié
            if not args.no_reset:
                set_esp_reset(ser, bootloader=args.boot)
            
            print(f"Connecté à {port} ({args.baud} bauds). Ctrl+C pour quitter.")
            while True:
                line = ser.readline()
                if line:
                    try:
                        print(line.decode("utf-8", errors="replace"), end="")
                    except Exception:
                        print(line)
    except KeyboardInterrupt:
        print("\nArrêté par l'utilisateur.")
    except Exception as e:
        print(f"Erreur: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
