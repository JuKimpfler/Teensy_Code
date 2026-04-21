"""
RoboCup Junior Soccer 2v2 – Feldvisualisierung
================================================
Zeigt das Spielfeld aus der Vogelperspektive und bewegt einen Kreis
(Ø 22 cm) anhand von Positionsdaten, die über einen seriellen Port im
Format  "x,y\n"  empfangen werden.

Koordinatenursprung: linke untere Feldecke
Tore: oben und unten (an den Schmalseiten)

Starten:
    python field_visualizer.py
Abhängigkeiten:
    pip install pyserial
"""

import tkinter as tk
from tkinter import ttk, messagebox
import threading
import serial
import serial.tools.list_ports

# ---------------------------------------------------------------------------
# Feldmaße in cm  (RoboCup Junior Soccer 2024 – 2 vs. 2)
# ---------------------------------------------------------------------------
FIELD_W_CM = 182.0          # Feldbreite  (x-Achse)
FIELD_H_CM = 243.0          # Feldhöhe    (y-Achse)
CENTER_CIRCLE_R_CM = 20.0   # Radius Mittelkreis

BALL_DIAM_CM = 22.0         # anzuzeigender Kreis-Durchmesser
BALL_R_CM = BALL_DIAM_CM / 2.0

GOAL_WIDTH_CM = 60.0        # Torbreite
GOAL_DEPTH_CM = 10.0        # Tortiefe (nur optisch)

# Padding um das Feld herum (Pixel)
CANVAS_PAD = 40
# Maximale Canvasgröße (Pixel) – wird auf Fenster angepasst
CANVAS_W_PX = 550
CANVAS_H_PX = int(CANVAS_W_PX * FIELD_H_CM / FIELD_W_CM)

# Farben
COLOR_FIELD = "#2e7d32"        # Feldgrün
COLOR_LINE = "white"           # Linien
COLOR_BALL = "#ff6600"         # Ball / Kreis
COLOR_GOAL_A = "#1565c0"       # Tor oben (blau)
COLOR_GOAL_B = "#b71c1c"       # Tor unten (rot)
COLOR_GOAL_FILL_A = "#1565c088"
COLOR_GOAL_FILL_B = "#b71c1c88"
COLOR_BG = "#212121"


class FieldVisualizer(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("RoboCup Soccer 2v2 – Feldvisualisierung")
        self.resizable(False, False)
        self.configure(bg=COLOR_BG)

        # Skalierungsfaktor cm → Pixel
        self._scale = (CANVAS_W_PX - 2 * CANVAS_PAD) / FIELD_W_CM

        self._serial: serial.Serial | None = None
        self._read_thread: threading.Thread | None = None
        self._running = False

        # Aktuelle Ballposition in cm (Ursprung links unten)
        self._ball_x_cm = FIELD_W_CM / 2.0
        self._ball_y_cm = FIELD_H_CM / 2.0

        self._build_ui()
        self._draw_field()
        self._draw_ball()

    # ------------------------------------------------------------------
    # UI aufbauen
    # ------------------------------------------------------------------
    def _build_ui(self):
        # ---- Canvas ----
        self._canvas = tk.Canvas(
            self,
            width=CANVAS_W_PX,
            height=CANVAS_H_PX,
            bg=COLOR_BG,
            highlightthickness=0,
        )
        self._canvas.pack(padx=10, pady=10)

        # ---- Steuerleiste ----
        ctrl = tk.Frame(self, bg=COLOR_BG)
        ctrl.pack(fill="x", padx=10, pady=(0, 10))

        tk.Label(ctrl, text="COM-Port:", bg=COLOR_BG, fg="white").pack(
            side="left", padx=(0, 4)
        )

        self._port_var = tk.StringVar()
        self._port_combo = ttk.Combobox(
            ctrl,
            textvariable=self._port_var,
            width=14,
            state="readonly",
        )
        self._port_combo.pack(side="left")
        self._refresh_ports()

        tk.Button(
            ctrl,
            text="⟳",
            command=self._refresh_ports,
            bg="#424242",
            fg="white",
            relief="flat",
            padx=4,
        ).pack(side="left", padx=(2, 10))

        tk.Label(ctrl, text="Baud:", bg=COLOR_BG, fg="white").pack(
            side="left", padx=(0, 4)
        )
        self._baud_var = tk.StringVar(value="115200")
        baud_combo = ttk.Combobox(
            ctrl,
            textvariable=self._baud_var,
            values=["9600", "19200", "38400", "57600", "115200", "230400"],
            width=8,
            state="readonly",
        )
        baud_combo.pack(side="left", padx=(0, 10))

        self._start_btn = tk.Button(
            ctrl,
            text="▶  Start",
            command=self._start,
            bg="#388e3c",
            fg="white",
            relief="flat",
            padx=8,
        )
        self._start_btn.pack(side="left", padx=2)

        self._stop_btn = tk.Button(
            ctrl,
            text="■  Stop",
            command=self._stop,
            bg="#c62828",
            fg="white",
            relief="flat",
            padx=8,
            state="disabled",
        )
        self._stop_btn.pack(side="left", padx=2)

        # Statuszeile
        self._status_var = tk.StringVar(value="Bereit.")
        tk.Label(
            self,
            textvariable=self._status_var,
            bg=COLOR_BG,
            fg="#aaaaaa",
            anchor="w",
        ).pack(fill="x", padx=10, pady=(0, 6))

    # ------------------------------------------------------------------
    # Feld zeichnen (einmalig)
    # ------------------------------------------------------------------
    def _draw_field(self):
        c = self._canvas
        c.delete("field")

        def cm2px(x_cm, y_cm):
            """Konvertiert cm-Koordinaten (Ursprung links unten) in Canvas-Pixel."""
            px = CANVAS_PAD + x_cm * self._scale
            py = CANVAS_H_PX - CANVAS_PAD - y_cm * self._scale
            return px, py

        x0, y0 = cm2px(0, 0)
        x1, y1 = cm2px(FIELD_W_CM, FIELD_H_CM)

        # Feldfläche
        c.create_rectangle(x0, y0, x1, y1, fill=COLOR_FIELD, outline=COLOR_LINE,
                            width=2, tags="field")

        # Mittellinie
        mx0, my = cm2px(0, FIELD_H_CM / 2)
        mx1, _ = cm2px(FIELD_W_CM, FIELD_H_CM / 2)
        c.create_line(mx0, my, mx1, my, fill=COLOR_LINE, width=2, tags="field")

        # Mittelkreis
        cx, cy = cm2px(FIELD_W_CM / 2, FIELD_H_CM / 2)
        r = CENTER_CIRCLE_R_CM * self._scale
        c.create_oval(cx - r, cy - r, cx + r, cy + r,
                      outline=COLOR_LINE, width=2, tags="field")
        # Mittelpunkt
        c.create_oval(cx - 3, cy - 3, cx + 3, cy + 3,
                      fill=COLOR_LINE, outline=COLOR_LINE, tags="field")

        # Tor oben (y = FIELD_H_CM, blaue Mannschaft)
        gx0, gy0 = cm2px((FIELD_W_CM - GOAL_WIDTH_CM) / 2, FIELD_H_CM)
        gx1, gy1 = cm2px((FIELD_W_CM + GOAL_WIDTH_CM) / 2,
                         FIELD_H_CM + GOAL_DEPTH_CM)
        c.create_rectangle(gx0, gy0, gx1, gy1,
                           fill=COLOR_GOAL_A, outline=COLOR_LINE, width=2,
                           tags="field")
        c.create_text((gx0 + gx1) / 2, (gy0 + gy1) / 2,
                      text="TOR A", fill="white", font=("Arial", 8, "bold"),
                      tags="field")

        # Tor unten (y = 0, rote Mannschaft)
        gx0b, gy0b = cm2px((FIELD_W_CM - GOAL_WIDTH_CM) / 2, 0)
        gx1b, gy1b = cm2px((FIELD_W_CM + GOAL_WIDTH_CM) / 2, -GOAL_DEPTH_CM)
        c.create_rectangle(gx0b, gy0b, gx1b, gy1b,
                           fill=COLOR_GOAL_B, outline=COLOR_LINE, width=2,
                           tags="field")
        c.create_text((gx0b + gx1b) / 2, (gy0b + gy1b) / 2,
                      text="TOR B", fill="white", font=("Arial", 8, "bold"),
                      tags="field")

        # Koordinaten-Beschriftung (Ecken)
        pad = 5
        c.create_text(x0 + pad, y0 - pad, text="(0,0)", fill="#aaaaaa",
                      anchor="sw", font=("Arial", 7), tags="field")
        c.create_text(x1 - pad, y0 - pad,
                      text=f"({int(FIELD_W_CM)},0)", fill="#aaaaaa",
                      anchor="se", font=("Arial", 7), tags="field")

    # ------------------------------------------------------------------
    # Ball zeichnen / aktualisieren
    # ------------------------------------------------------------------
    def _draw_ball(self):
        c = self._canvas
        c.delete("ball")

        x_cm = max(0.0, min(self._ball_x_cm, FIELD_W_CM))
        y_cm = max(0.0, min(self._ball_y_cm, FIELD_H_CM))

        px = CANVAS_PAD + x_cm * self._scale
        py = CANVAS_H_PX - CANVAS_PAD - y_cm * self._scale
        r = BALL_R_CM * self._scale

        # Schatten
        c.create_oval(px - r + 2, py - r + 2, px + r + 2, py + r + 2,
                      fill="#00000066", outline="", tags="ball")
        # Ball
        c.create_oval(px - r, py - r, px + r, py + r,
                      fill=COLOR_BALL, outline="white", width=1, tags="ball")
        # Koordinatenanzeige
        c.create_text(px, py - r - 6,
                      text=f"{self._ball_x_cm:.1f} , {self._ball_y_cm:.1f}",
                      fill="white", font=("Arial", 8, "bold"), tags="ball")

    # ------------------------------------------------------------------
    # Serielle Ports ermitteln
    # ------------------------------------------------------------------
    def _refresh_ports(self):
        ports = [p.device for p in serial.tools.list_ports.comports()]
        self._port_combo["values"] = ports
        if ports and not self._port_var.get():
            self._port_combo.current(0)

    # ------------------------------------------------------------------
    # Start / Stop
    # ------------------------------------------------------------------
    def _start(self):
        port = self._port_var.get()
        if not port:
            messagebox.showwarning("Kein Port", "Bitte einen COM-Port auswählen.")
            return
        try:
            baud = int(self._baud_var.get())
            self._serial = serial.Serial(port, baud, timeout=1)
        except serial.SerialException as exc:
            messagebox.showerror("Verbindungsfehler", str(exc))
            return

        self._running = True
        self._start_btn.config(state="disabled")
        self._stop_btn.config(state="normal")
        self._port_combo.config(state="disabled")
        self._status_var.set(f"Verbunden mit {port} @ {baud} Baud …")

        self._read_thread = threading.Thread(
            target=self._read_serial, daemon=True
        )
        self._read_thread.start()

    def _stop(self):
        self._running = False
        if self._serial and self._serial.is_open:
            self._serial.close()
        self._serial = None
        self._start_btn.config(state="normal")
        self._stop_btn.config(state="disabled")
        self._port_combo.config(state="readonly")
        self._status_var.set("Getrennt.")

    # ------------------------------------------------------------------
    # Seriellen Port lesen (Hintergrundthread)
    # ------------------------------------------------------------------
    def _read_serial(self):
        buffer = ""
        while self._running and self._serial and self._serial.is_open:
            try:
                raw = self._serial.readline()
                if not raw:
                    continue
                line = raw.decode("ascii", errors="ignore").strip()
                if not line:
                    continue
                parts = line.split(",")
                if len(parts) >= 2:
                    x = float(parts[0].strip())
                    y = float(parts[1].strip())
                    # GUI-Update im Hauptthread planen
                    self.after(0, self._update_position, x, y, line)
            except (serial.SerialException, OSError):
                self.after(0, self._stop)
                self.after(0, lambda: messagebox.showerror(
                    "Verbindung getrennt", "Serielle Verbindung unterbrochen."))
                break
            except ValueError:
                # Ungültiges Paket – ignorieren
                pass

    # ------------------------------------------------------------------
    # Position aktualisieren (Hauptthread)
    # ------------------------------------------------------------------
    def _update_position(self, x: float, y: float, raw: str):
        self._ball_x_cm = x
        self._ball_y_cm = y
        self._status_var.set(f"Empfangen: {raw}  →  x={x:.2f} cm, y={y:.2f} cm")
        self._draw_ball()

    # ------------------------------------------------------------------
    # Fenster schließen
    # ------------------------------------------------------------------
    def on_close(self):
        self._stop()
        self.destroy()


if __name__ == "__main__":
    app = FieldVisualizer()
    app.protocol("WM_DELETE_WINDOW", app.on_close)
    app.mainloop()
