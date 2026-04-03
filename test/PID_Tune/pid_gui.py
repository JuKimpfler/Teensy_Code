import tkinter as tk
from tkinter import messagebox
import serial
import threading
import time
import os

SERIAL_PORT = 'COM7'  # Passe den Port an!
BAUDRATE = 115200
PARAM_FILE = 'pid_params.txt'

class PIDGui:
    def __init__(self, master):
        self.master = master
        master.title("PID Parameter Tool")

        self.serial = None
        self.connect_serial()

        self.kp = tk.DoubleVar()
        self.ki = tk.DoubleVar()
        self.kd = tk.DoubleVar()
        self.mult = tk.DoubleVar()

        self.load_params()

        tk.Label(master, text="Kp:").grid(row=0, column=0)
        tk.Entry(master, textvariable=self.kp).grid(row=0, column=1)

        tk.Label(master, text="Ki:").grid(row=1, column=0)
        tk.Entry(master, textvariable=self.ki).grid(row=1, column=1)

        tk.Label(master, text="Kd:").grid(row=2, column=0)
        tk.Entry(master, textvariable=self.kd).grid(row=2, column=1)

        tk.Label(master, text="PID_Mult:").grid(row=3, column=0)
        tk.Entry(master, textvariable=self.mult).grid(row=3, column=1)

        tk.Button(master, text="Senden", command=self.send_params).grid(row=4, column=0, columnspan=2)
        tk.Button(master, text="Speichern", command=self.save_params).grid(row=5, column=0, columnspan=2)

        self.status = tk.Label(master, text="", fg="green")
        self.status.grid(row=6, column=0, columnspan=2)

        # Start thread to read serial output
        self.serial_thread = threading.Thread(target=self.read_serial, daemon=True)
        self.serial_thread.start()

    def connect_serial(self):
        try:
            self.serial = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)
            time.sleep(2)  # Warten bis Arduino bereit
        except Exception as e:
            messagebox.showerror("Fehler", f"Serielle Verbindung fehlgeschlagen: {e}")

    def send_params(self):
        try:
            cmds = [
                f"kp {self.kp.get()}",
                f"ki {self.ki.get()}",
                f"kd {self.kd.get()}",
                f"mult {self.mult.get()}",
                "show"
            ]
            for cmd in cmds:
                self.serial.write((cmd + "\n").encode())
                time.sleep(0.1)
            self.status.config(text="Werte gesendet!")
        except Exception as e:
            self.status.config(text=f"Fehler: {e}", fg="red")

    def save_params(self):
        try:
            with open(PARAM_FILE, "w") as f:
                f.write(f"{self.kp.get()},{self.ki.get()},{self.kd.get()},{self.mult.get()}\n")
            self.status.config(text="Werte gespeichert!")
        except Exception as e:
            self.status.config(text=f"Fehler: {e}", fg="red")

    def load_params(self):
        if os.path.exists(PARAM_FILE):
            try:
                with open(PARAM_FILE, "r") as f:
                    line = f.readline()
                    kp, ki, kd, mult = map(float, line.strip().split(","))
                    self.kp.set(kp)
                    self.ki.set(ki)
                    self.kd.set(kd)
                    self.mult.set(mult)
            except Exception as e:
                self.status.config(text=f"Fehler beim Laden: {e}", fg="red")
        else:
            # Defaultwerte
            self.kp.set(1.5)
            self.ki.set(0.0005)
            self.kd.set(8.0)
            self.mult.set(0.1)

    def read_serial(self):
        while True:
            if self.serial and self.serial.in_waiting:
                try:
                    line = self.serial.readline().decode().strip()
                    if line:
                        self.status.config(text=line)
                except Exception:
                    pass
            time.sleep(0.1)

if __name__ == "__main__":
    root = tk.Tk()
    app = PIDGui(root)
    root.mainloop()