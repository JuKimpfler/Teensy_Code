# Projektdokumentation

Diese Dokumentation beschreibt die wichtigsten Dateien, Klassen und Funktionen im Projekt *TeensyCode_2026*. Sie richtet sich an Entwickler und bietet einen Überblick über die Struktur sowie eine detaillierte Tabellenübersicht aller relevanten Funktionen, Parameter und Enumerationen.

📁 **Dateiübersicht**
- `lib/Lowlevel` – Hardware-nahe Klassen (Motoren, Sensoren, Robotersteuerung, …)
- `lib/System` – Systemsteuerung, Initialisierung und Update‑Schleifen
- `lib/Elementar` – gemeinsame Definitionsdateien (Enums, Globals, Utils)
- `lib/Taktics` – Spieltaktiken und Algorithmen
- `src/main.cpp` – Hauptprogramm, das System-Updates aufruft


## Tabellenstruktur
In der Datei **`functions_and_enums.csv`** (im selben Ordner) befindet sich eine Excel-kompatible Tabelle mit folgenden Spalten:

1. **File** – Pfad zur Quelldatei
2. **Class/Enum** – Name der Klasse oder Aufzählung
3. **Symbol** – Funktionsname oder enum‑Element
4. **Type** – `function`, `enum` oder `data struct`
5. **Parameters** – Parameterliste (Typ und Namen)
6. **Description** – Kurzbeschreibung des Verwendungszwecks

Die Tabelle enthält sowohl alle öffentlichen Methoden der Klassen als auch die im Projekt definierten `enum`‑Typen und ihre Werte.

> 💡 Tipp: Die CSV lässt sich direkt in Excel oder LibreOffice calc öffnen; jede Zeile stellt ein Dokumentationselement dar. Verwenden Sie Filter, um z. B. nur Methoden einer bestimmten Klasse anzuzeigen.


## Modulübersicht (stichpunktartig)

### Robotik (`Robot.h` / `Robot.cpp`)
- **RobotC** – zentrale Steuerklasse für Bewegung
  - *Turn, Drive, Stop, Break* – Bewegungsbefehle
  - *Drive_Smoothed_* – sanfte Geschwindigkeitssignale
  - **KickerC** – verschachtelte Klasse zur Steuerung des Kickers
    - `Once`, `On`, `Off`, `Update` + Timer-Support

### Motoren (`Motor.h` / `Motor.cpp`)
- **MotorC** – PWM‑gesteuerte Motorobjekte
  - `init`, `On`, `Off`, `Break`

### System (`System.h` / `System.cpp`)
- **SystemC** – verwaltet Starttaster, Buttons, Switches
  - Unterklassen *initC* (Initialisierung) und *UpdateC* (laufende Updates)

### Sensoren und Peripherie
- **USC** – Ultraschall-System (`init`, `read`)
- **CamC** / **GoalC** – Kameradatenverarbeitung
- **BNO055C** – IMU (Tilt-Werte, Kalibrierung)
- **ExpanderC** – I2C‑/ADC‑Expander mit Unterklassen *I2CC* und *ADCC*
- **DebugC** – Logging und Plotting-Helper
- **PIDC** – PID-Regelungsberechnungen

### Gemeinsame Definitionen
- **enum.h** – Pin- und Port-Zuweisungen, I2C-Befehle, ADC-Ports
- **Param.h**, **Globals.h**, **Utils.h** – globale Konstanten und Hilfsfunktionen

### Taktiken (`lib/Taktics`)
Enthält Klassen wie `AttackerTacticsC`, `DefenderTacticsC` usw., die Strategiealgorithmen realisieren. Diese Klassen rufen intern Methoden aus den oben erwähnten Modulen auf.


## Gebrauchsanleitung
1. Öffnen Sie `Doku/functions_and_enums.csv` in einem Tabellenprogramm, um alle Funktionen und Enums einzusehen.
2. Verwenden Sie den Markdown-Text als Referenz für die Architektur; er wird bei Bedarf erweitert.
3. Fügen Sie neue Module oder Funktionen in der Tabelle hinzu, wenn das Projekt wächst.

---

📌 *Hinweis*: Falls bereits ähnliche Dokumente existieren, wurden sie überarbeitet und aktualisiert.

---

_Ende der initialen Dokumentation._
