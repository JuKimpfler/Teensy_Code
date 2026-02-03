import serial
import threading
import time
import sys

# --- KONFIGURATION ---
PORT = 'COM15'        # Deinen Port hier anpassen (z.B. 'COM3' oder '/dev/ttyUSB0')
BAUDRATE = 115200    # Baudrate anpassen
MESSAGE = "1234"    # Die Nachricht, die gesendet werden soll
INTERVALL = 0.02     # 10 Millisekunden

# Globale Variable zur Steuerung
sending_active = False
run_program = True

def sender_thread(ser):
    """Loop, der die Nachricht sendet, wenn sending_active True ist."""
    global sending_active
    while run_program:
        if sending_active:
            try:
                ser.write(MESSAGE.encode('utf-8'))
                time.sleep(INTERVALL)
            except Exception as e:
                print(f"\n[Fehler beim Senden]: {e}")
                sending_active = False
        else:
            time.sleep(0.1) # CPU-Schonung, wenn inaktiv

def main():
    global sending_active, run_program

    print(f"--- COM-Port Sender ({PORT}) ---")
    
    # Port prüfen und öffnen
    try:
        ser = serial.Serial(PORT, BAUDRATE, timeout=1)
        print(f"[Info]: {PORT} erfolgreich geöffnet.")
    except serial.SerialException:
        print(f"[Fehler]: Port {PORT} ist nicht verfügbar oder belegt.")
        return

    # Sender-Thread starten
    thread = threading.Thread(target=sender_thread, args=(ser,), daemon=True)
    thread.start()

    print("\nSteuerung:")
    print(" '1' -> Senden START")
    print(" '0' -> Senden STOPP")
    print(" 'q' -> Programm BEENDEN")

    while run_program:
        cmd = input("Befehl: ").strip().lower()
        
        if cmd == '1':
            if not sending_active:
                sending_active = True
                print("[Status]: Sende alle 10ms...")
        elif cmd == '0':
            if sending_active:
                sending_active = False
                print("[Status]: Gestoppt.")
        elif cmd == 'q':
            print("[Info]: Beende Programm...")
            sending_active = False
            run_program = False
            break
        else:
            print("[?] Unbekannter Befehl.")

    ser.close()
    print("Port geschlossen. Tschüss!")

if __name__ == "__main__":
    main()