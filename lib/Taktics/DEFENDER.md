# Defender – Dokumentation

> Dateien: `lib/Taktics/Defender.h` · `lib/Taktics/Defender.cpp`  
> Adapter: `lib/Taktics/DefenderAdapter.h` · `lib/Taktics/DefenderAdapter.cpp`

---

## Inhaltsverzeichnis

1. [Überblick & Spielprinzip](#1-überblick--spielprinzip)
2. [Koordinatensystem & Winkelkonventionen](#2-koordinatensystem--winkelkonventionen)
3. [Eingangsparameter von `follow_Line`](#3-eingangsparameter-von-follow_line)
4. [Situationen und Verhalten (Zustandsdiagramm)](#4-situationen-und-verhalten-zustandsdiagramm)
   - [4.1 Kamera nicht ausgerichtet / kein Tor sichtbar](#41-kamera-nicht-ausgerichtet--kein-tor-sichtbar)
   - [4.2 Kick-Manöver aktiv](#42-kick-manöver-aktiv)
   - [4.3 Linie verloren – Roboter Richtung eigenes Tor gefahren](#43-linie-verloren--roboter-richtung-eigenes-tor-gefahren)
   - [4.4 Linie verloren – Roboter vom Tor weggefahren](#44-linie-verloren--roboter-vom-tor-weggefahren)
   - [4.5 Normales Linien-Following – Ball hinter dem Roboter](#45-normales-linien-following--ball-hinter-dem-roboter)
   - [4.6 Normales Linien-Following – Ball seitlich](#46-normales-linien-following--ball-seitlich)
   - [4.7 Kick-Bedingung geprüft (alle 2 s)](#47-kick-bedingung-geprüft-alle-2-s)
5. [Kick-Logik im Detail](#5-kick-logik-im-detail)
6. [Linien-Sensor-Logik im Detail](#6-linien-sensor-logik-im-detail)
7. [Wichtige Konstanten](#7-wichtige-konstanten)
8. [DefenderAdapter – Kamera-Mapping & Hardware-Brücke](#8-defenderadapter--kamera-mapping--hardware-brücke)
   - [8.1 Problem: Nur Vorwärts-Kamera vorhanden](#81-problem-nur-vorwärts-kamera-vorhanden)
   - [8.2 Kamera-Mapping-Lösung](#82-kamera-mapping-lösung)
   - [8.3 Strafraum-Schutz (Penalty-Area Protection)](#83-strafraum-schutz-penalty-area-protection)
   - [8.4 Sensor-Reihenfolge (CW/CCW)](#84-sensor-reihenfolge-cwccw)
   - [8.5 Nutzung in `loop()`](#85-nutzung-in-loop)
9. [Datenstrukturen](#9-datenstrukturen)
10. [Bekannte Einschränkungen & Tuning-Hinweise](#10-bekannte-einschränkungen--tuning-hinweise)

---

## 1 Überblick & Spielprinzip

Der **Defender** ist die taktische Steuerungsklasse für die Verteidigerrolle im RoboCup Soccer 2vs2 LWL.

**Grundprinzip:**
Der Roboter fährt **nicht frei auf dem Feld**, sondern hält sich stets auf der weißen **Linie vor dem eigenen Strafraum** auf. Diese Linie bildet die Grenze des Strafraums und verhindert gleichzeitig, dass der Roboter in den Strafraum einfährt (nach RoboCup-Regeln verboten).

**Zwei Kernaufgaben:**

| Aufgabe | Beschreibung |
|---|---|
| **Angreifer abfangen** | Sobald der Ball von einem gegnerischen Roboter bewegt wird (Ball ist nicht still), positioniert sich der Defender zwischen Ball und eigenem Tor, indem er der Linie in Richtung des Balls folgt. |
| **Ball schießen** | Sobald der Ball still steht, in der richtigen Distanz und im richtigen Winkelbereich liegt, fährt der Roboter auf den Ball zu und aktiviert den Kicker. |

---

## 2 Koordinatensystem & Winkelkonventionen

```
           Gegnerisches Tor
               ↑ 0°
         270° ←  → 90°
               ↓ 180°
           Eigenes Tor
```

- Alle Winkel im Defender intern: **0 … 360°** (gegen den Uhrzeigersinn positiv)
- `Ball.Angle` (aus IR-Sensoren): **−180 … +180°** → wird im Adapter zu 0…360° umgewandelt
- `LineCalc.DriveAngle`: **−180 … +180°** (Away-Vektor von der Linie weg)
- `BNO055.TiltZ`: IMU-Gier-Winkel, kalibriert zu 0° wenn Roboter Richtung gegnerisches Tor zeigt
- `Goal_angle` im Defender: kamerarelativ, **positiv = Tor rechts** von der Kameraachse

### Tor-relativer Ballwinkel

Im Kern von `follow_Line` wird der Ballwinkel **goal-relativ** gemacht:

```cpp
ball_angle = fmod(ball_angle - Goal_angle + 360.0f, 360.0f);
```

Das bedeutet: `ball_angle = 0°` → Ball direkt in Richtung eigenes Tor, `ball_angle = 180°` → Ball auf der Seite hinter dem Roboter (zwischen Roboter und gegnerischem Tor).

---

## 3 Eingangsparameter von `follow_Line`

```cpp
Drive_Data follow_Line(
    bool     Line_sensors_active[32],  // aktive Liniensensoren (Index 0..31, je 11,25°)
    float    ball_angle,               // Ballwinkel 0..360° (absolut / feldrelativ)
    CamData  camData_Back,             // Kameradaten (Goal-Winkel, Sichtbarkeit, Höhe)
    float    LinieWinkel,              // Winkel des Liniensensors 0..360° (Away-Vektor)
    float    Line_distance,            // Liniendistanz 0..1 (normiert)
    bool     Line_Detected,            // ob aktuell eine Linie erkannt wird
    float    Last_Line_Angle,          // letzter gültiger Linienwinkel (für Lost-Logic)
    double   ball_distance,            // Ballabstand in cm
    bool     Cam_SW                    // true = Kamera vorhanden/aktiv
);
```

**Rückgabe:** `Drive_Data { float turn; float direction; float speed; }`
- `direction`: Fahrtrichtung 0…360°
- `speed`: Fahrgeschwindigkeit 0…100
- `turn`: Drehmenge (wird im Defender aktuell nicht aktiv genutzt)

---

## 4 Situationen und Verhalten (Zustandsdiagramm)

```
                    ┌─────────────────────────────────┐
                    │         follow_Line()            │
                    └──────────────┬──────────────────┘
                                   │
              ┌────────────────────▼─────────────────────┐
              │  |Goal_angle| > 47°                       │
              │  ODER Tor nicht sichtbar & Cam aktiv?     │
              └────────────────┬─────────────────────────┘
                  JA ──────────┘                │ NEIN
                  ↓                             │
         [4.1] Geradeaus fahren         ┌───────▼──────────────┐
         speed=30, dir=0°               │  Kick_Active && noch  │
                                        │  innerhalb KICK_DAUER?│
                                        └───────┬──────────────┘
                                    JA ─────────┘        │ NEIN
                                    ↓                     │
                              [4.2] Kick-                 │
                              Fahrt                ┌──────▼──────────┐
                              speed=70             │ Linie erkannt?   │
                              dir=ball_angle_raw   └──────┬──────────┘
                                              NEIN ───────┘    │ JA
                                              ↓                │
                                 ┌────────────▼──────┐  [normale Linie]
                                 │ Robot Richtung    │         │
                                 │ Tor gefahren?     │  ┌──────▼──────────────┐
                                 │ (diff < 90°)      │  │ Ball HINTER Roboter?│
                                 │  && GoalHeight>60?│  │ (ball_angle ≈ 180°) │
                                 └────┬──────────────┘  └──────┬──────────────┘
                              JA ─────┘     │ NEIN     JA ─────┘      │ NEIN
                              ↓             ↓          ↓              ↓
                           [4.3]          [4.4]      [4.5]          [4.6]
                       Weg vom Tor     Zurück zu   Linie folgen  Nächsten Sensor
                       fahren         Linie fahren  (seitlich)   zum Ball fahren
```

---

### 4.1 Kamera nicht ausgerichtet / kein Tor sichtbar

**Bedingung:**
```
|Goal_angle| > 47°  ODER  (kein Tor sichtbar UND Cam_SW == true)
```

**Verhalten:** Der Roboter fährt **geradeaus vorwärts** mit niedrigerer Geschwindigkeit.

```
speed = 30
direction = 0° (vorwärts)
```

**Taktische Begründung:** Wenn die Kamera das Tor nicht findet (Roboter zu stark gedreht oder Sicht versperrt), ist keine sinnvolle Torrichtungsberechnung möglich. Der Roboter hält einfach seine Grundposition bei, bis die Kamera wieder ein Tor sieht.

---

### 4.2 Kick-Manöver aktiv

**Bedingung:**  
`Kick_Active == true` UND Zeitstempel < `KICK_DURATION_MS` (500 ms) nach Kick-Start

**Verhalten:** Der Roboter fährt mit voller Kick-Geschwindigkeit direkt auf den Ball zu.

```
speed = 70  (KICK_SPEED)
direction = ball_angle_before  (absoluter Ballwinkel VOR goal-relativer Transformation)
```

**Taktische Begründung:** Der Roboter rammt den Ball mit maximaler Kraft weg, um ihn aus dem Gefahrenbereich vor dem eigenen Tor zu schießen. Der physische Kicker wird gleichzeitig durch den `DefenderAdapter` ausgelöst.

Nach Ablauf der 500 ms wird `Kick_Active = false` gesetzt und der Ball-Stillstands-Timer zurückgesetzt.

---

### 4.3 Linie verloren – Roboter Richtung eigenes Tor gefahren

**Bedingung:**
```
Line_Detected == false
UND letzter gültiger Linienwinkel zeigt Richtung Tor (diff < 90° oder diff > 270°)
UND GoalHeight > 60  (Roboter nah am Tor = im Strafraum-Bereich)
```

**Verhalten:** Der Roboter fährt **weg vom Tor** in die zuletzt bekannte Linienrichtung zurück.

```
speed = 30  (defender_out_speed)
direction = last_valid_line_angle  (Richtung in der die Linie zuletzt erkannt wurde)
```

**Taktische Begründung:** Der Roboter ist versehentlich in den Strafraum gefahren (nach RoboCup-Regeln verboten). Er muss den Strafraum verlassen, ohne direkt Richtung Tor zu fahren und tiefer hineinzufahren.

---

### 4.4 Linie verloren – Roboter vom Tor weggefahren

**Bedingung:**
```
Line_Detected == false
UND Roboter ist NICHT Richtung Tor gefahren (oder GoalHeight ≤ 60)
```

**Verhalten:** Der Roboter fährt zurück zur Linie, orientiert an der Torrichtung.

```
Wenn Tor sichtbar:
  speed = 30
  direction = GoalAngle + 180°  (entgegengesetzt zum Tor = Richtung Linie)

Wenn Tor NICHT sichtbar:
  speed = 30  (defender_out_speed)
  direction = 0° (vorwärts)
```

**Taktische Begründung:** Der Roboter hat die Linie verlassen (z.B. nach einem Ball-Kontakt oder Push), ist aber nicht im Strafraum. Er navigiert zurück zur Strafraumlinie, indem er sich entgegen der Torrichtung bewegt.

---

### 4.5 Normales Linien-Following – Ball hinter dem Roboter

**Bedingung:**
```
Line_Detected == true
UND ball_angle (goal-relativ) ≈ 180°  (genauer: 175° < ball_angle < 185°)
```

Das bedeutet: Der Ball befindet sich hinter dem Roboter, also zwischen Roboter und dem gegnerischen Tor – der Roboter hat das Tor und den Ball in einer Linie, steht also gut als Verteidiger.

**Verhalten:** Folgt der Linie seitlich, abhängig von der aktuellen Linienorientierung:

```
Wenn Line_distance > 0.5 UND LinieWinkel nahe 0° oder 360° (Linie quer vor dem Roboter):
  → speed = 100 * Line_distance
  → direction = LinieWinkel  (Linie-Richtungsvektor folgen)

Wenn Line_distance > 0.5 UND LinieWinkel nahe 180° (Linie hinter dem Roboter):
  → speed = 100 * Line_distance
  → direction = LinieWinkel
```

**Taktische Begründung:** Der Roboter steht optimal zwischen Ball und Tor. Er muss nur noch der Linie folgen, um auf der Grenzlinie zu bleiben, und wartet auf einen Schuss-Moment.

---

### 4.6 Normales Linien-Following – Ball seitlich

**Bedingung:**
```
Line_Detected == true
UND ball_angle (goal-relativ) NICHT ≈ 180°
```

**Verhalten (zweistufig):**

**Schritt 1 – Nächsten Sensor zum Ball suchen:**  
Der Defender sucht den aktiven Liniensensor, dessen Winkel am nächsten am Ballwinkel liegt.
```
Für alle aktiven Sensoren i (0..31):
  sensor_angle = i * 11.25°
  angle_diff = |sensor_angle - ball_angle|  (kürzeste Winkeldistanz)
  → Sensor mit kleinstem angle_diff wird ausgewählt

direction = Closest_sensor * 11.25°
speed = min(speed, speed * exp(ball_x_distance / 50 - 1) - 0.31)
```

**Schritt 2 – Linie nahe prüfen:**  
Wenn der Sensor zu nah an der Linie ist (Line_distance > 0,7), wird die Bewegung entlang der Linie verstärkt:
```
Wenn Line_distance > 0.7 UND LinieWinkel nahe 0°/360° oder 180°:
  direction = LinieWinkel
  speed = 100 * Line_distance
```

**Taktische Begründung:** Der Roboter bewegt sich entlang der Strafraumlinie seitlich in Richtung des Balls, um sich zwischen Ball und Tor zu positionieren. Die Geschwindigkeit wird durch `ball_x_distance` (horizontaler Abstand des Balls) gedämpft – je weiter der Ball seitlich weg ist, desto langsamer fährt der Roboter.

**Geschwindigkeitsdämpfung:**
```
ball_x_distance = |sin(ball_angle) * ball_distance|

speed_gedämpft = speed * exp(ball_x_distance / 50 - 1) - 0.31
```
Bei `ball_x_distance = 50 cm` → volle Geschwindigkeit. Bei kleinerem Abstand → Geschwindigkeit reduziert.

---

### 4.7 Kick-Bedingung geprüft (alle 2 s)

**Alle 2 Sekunden** prüft der Defender, ob ein Schuss ausgeführt werden soll.

**Alle folgenden Bedingungen müssen erfüllt sein:**

| Bedingung | Wert | Bedeutung |
|---|---|---|
| `angle_change_ok` | Winkeländerung < 20° seit letzter Prüfung | Ball hat sich nicht seitlich bewegt |
| `dist_change_ok` | Distanzänderung < 20 cm seit letzter Prüfung | Ball hat sich nicht vor/zurück bewegt |
| `ball_dist_min_ok` | ball_distance > 30 cm | Ball nicht zu nah (würde Sensor sättigen) |
| `ball_dist_max_ok` | ball_distance < 55 cm | Ball nicht zu weit weg |
| `ball_angle_min_ok` | |normierter Ballwinkel| > 15° | Ball nicht direkt vorne |
| `ball_angle_max_ok` | |normierter Ballwinkel| < 40° | Ball nicht zu weit seitlich |
| `line_detected_ok` | Linie erkannt | Roboter steht auf der Strafraumlinie |

**Normierter Ballwinkel** (zur Kick-Bedingungsprüfung):
```cpp
ball_angle_check = fabs(((ball_angle_before + 180) % 360) - 180)
```
Dieser Wert beschreibt, wie weit der Ball vom direkten Vorwärtsbereich des Roboters abweicht (0° = direkt vorne).

**Wenn alle Bedingungen erfüllt:**
```
Kick_Active = true
Kick_Starter = aktueller Zeitstempel
→ Nächster Loop-Durchlauf: Zustand [4.2] aktiv
```

---

## 5 Kick-Logik im Detail

Die Kick-Logik arbeitet intern mit einer `KickState`-Struktur (in `Defender.h` deklariert), welche aber in der aktuellen `follow_Line`-Implementierung **nur über die Klassen-Membervariablen** (`Kick_Active`, `Kick_Starter`) gesteuert wird. Die separate `KickState`-Struktur und die zugehörigen privaten Methoden (`is_Ball_Still`, `handle_Kick`, `check_Kick_Trigger`) sind im Code vorhanden, werden in `follow_Line` selbst aber über den 2-Sekunden-Timer gesteuert.

```
KickState (Struktur):
  ball_angle   – Ballwinkel beim letzten Check
  ball_dist    – Ballabstand beim letzten Check
  still_start  – Zeitpunkt seit dem der Ball still ist
  kick_start   – Zeitpunkt des Kick-Starts
  kicking      – ob Kick gerade aktiv

is_Ball_Still():
  → Ball gilt als still wenn:
     Distanzänderung  < 3 cm (MOVEMENT_DIST_THRESH)
     Winkeländerung   < 15°  (MOVEMENT_ANGLE_THRESH)
     UND dieser Zustand > 2000 ms anhält (STILL_DURATION_MS)

check_Kick_Trigger():
  → Nur aktiv wenn Ball in Range:
     KICK_DIST_MIN = 5 cm  ≤ ball_distance ≤  KICK_DIST_MAX = 15 cm
  → Erst dann: is_Ball_Still() prüfen

handle_Kick():
  → Fährt 500 ms (KICK_DURATION_MS) mit speed=100 Richtung Ball
  → Danach: kicking = false, Ball-Timer reset
```

---

## 6 Linien-Sensor-Logik im Detail

Der Defender nutzt ein Array von **32 Liniensensoren**, die gleichmäßig um den Roboter verteilt sind (je 11,25° Abstand). Jeder Sensor liefert 0 (kein Weiß erkannt) oder 1 (Weiß/Linie erkannt).

```
Sensor-Index  Winkel
     0        0°    (vorne)
     8        90°   (rechts)
    16        180°  (hinten)
    24        270°  (links)
```

**Sensor-Reihenfolge:** Standardmäßig nimmt der Defender an, dass die Indizes **gegen den Uhrzeigersinn** zunehmen. Falls die physische Anordnung **im Uhrzeigersinn** ist, muss `DEFENDER_SENSOR_CW true` gesetzt werden (im Adapter).

**`find_Closest_Sensor`:**
- Iteriert über alle 32 Sensoren
- Berechnet für jeden aktiven Sensor die Winkeldifferenz zum Ballwinkel (goal-relativ)
- Wählt den Sensor mit der kleinsten Differenz als Fahrziel
- Die Geschwindigkeit wird exponentiell durch `ball_x_distance` skaliert

**`handle_Ball_In_Front`:**
- Prüft ob der goal-relative Ballwinkel ≈ 180° ist (Ball auf der "sicheren" Seite)
- Wenn ja: Roboter folgt einfach der Linie (keine gezielte Bewegung Richtung Ball nötig)
- Wenn die Linie sehr nah ist (Line_distance > 0,5) wird entlang der Linie gefahren

---

## 7 Wichtige Konstanten

| Konstante | Wert | Bedeutung |
|---|---|---|
| `STILL_DURATION_MS` | 2000 ms | Wie lange der Ball still sein muss, damit er als "still" gilt |
| `KICK_DURATION_MS` | 500 ms | Wie lange der Kick-Vorwärtssprint dauert |
| `KICK_SPEED` | 70 | Geschwindigkeit während des Kick-Manövers |
| `KICK_DIST_MIN` | 5,0 cm | Minimale Balldistanz für Kick-Auslösung |
| `KICK_DIST_MAX` | 15,0 cm | Maximale Balldistanz für Kick-Auslösung |
| `MOVEMENT_DIST_THRESH` | 3,0 cm | Distanzänderung über der ein Ball als "bewegt" gilt |
| `MOVEMENT_ANGLE_THRESH` | 15° | Winkeländerung über der ein Ball als "bewegt" gilt |
| `speed` (Klasse) | 30 | Standard-Fahrgeschwindigkeit beim Linien-Following |
| `defender_out_speed` | 30 | Geschwindigkeit beim Zurückfahren zur Linie |

**Kick-Bedingungen** (geprüft alle 2 s in `follow_Line`):
| Bedingung | Schwellwert |
|---|---|
| Maximale Winkeländerung seit letzter Prüfung | < 20° |
| Maximale Distanzänderung seit letzter Prüfung | < 20 cm |
| Minimale Balldistanz | > 30 cm |
| Maximale Balldistanz | < 55 cm |
| Minimaler normierter Ballwinkel | > 15° |
| Maximaler normierter Ballwinkel | < 40° |

---

## 8 DefenderAdapter – Kamera-Mapping & Hardware-Brücke

### 8.1 Problem: Nur Vorwärts-Kamera vorhanden

Der `Defender` wurde für einen Roboter mit **rückwärts gerichteter Kamera** entwickelt, die das **eigene Tor** erkennt. Der reale Roboter hat jedoch nur eine **vorwärts gerichtete Kamera** auf das **gegnerische Tor**.

Der `DefenderAdapter` löst diese Inkompatibilität, indem er die fehlende "Rückkamera-Sicht" aus den vorhandenen Sensordaten synthetisiert.

### 8.2 Kamera-Mapping-Lösung

**Mathematische Grundlage:**  
Beide Tore liegen auf derselben Achse. Wenn der Roboter um N° dreht, ändert sich der kamerarelative Winkel **beider** Tore um N° in dieselbe Richtung:

```
Roboter gerade ausgerichtet:
  Vorwärts-Kamera sieht gegnerisches Tor bei 0° → Rückwärts-Kamera würde eigenes Tor bei 0° sehen ✓

Roboter dreht 20° nach rechts:
  Vorwärts-Kamera sieht gegnerisches Tor bei +20° → Rückwärts-Kamera würde eigenes Tor bei +20° sehen ✓
```

**Mapping:**
```
Kamera verfügbar:
  GoalAgnle = Cam.give_Angle_Cam()    ← kamerarelativ (nicht feldrelativ!)

Kamera nicht verfügbar (Fallback):
  GoalAgnle = BNO055.TiltZ            ← IMU-Gier (kalibriert zu 0° wenn Roboter vorwärts zeigt)
  BlueGoal_visible = false
  YellowGoal_visible = false
```

> ⚠️ **Wichtig:** Es muss `Cam.give_Angle_Cam()` (kamerarelativ) verwendet werden, **nicht** `Cam.give_Angle()` (feldrelativ). Feldrelative Winkel beinhalten bereits die IMU-Rotation und würden die goal-relative Berechnung im Defender korrumpieren.

### 8.3 Strafraum-Schutz (Penalty-Area Protection)

Der originale Defender erkennt Strafraum-Eintritte über `GoalHeight > 60` (Tor-Blob-Höhe aus der Rückkamera). Der Adapter ersetzt dies durch eine **Liniensensor-basierte Erkennung**:

```
Wenn Linie erkannt UND LineCalc.DriveAngle ≈ 0° (±45°):
  → Roboter berührt die Linie AUF der Torseite
  → _nearOwnGoal = true
  → GoalHeight = 100  (synthetisch, > 60 → Defender erkennt Strafraum-Nähe)

Wenn Linie erkannt UND DriveAngle NICHT ≈ 0°:
  → Roboter berührt eine andere Linie (Seitenlinie)
  → _nearOwnGoal = false
  → GoalHeight = 0

Wenn keine Linie erkannt UND _nearOwnGoal seit > 1200 ms:
  → Flag wird zurückgesetzt
```

**Konfiguration:**
```cpp
#define DEFENDER_GOAL_SIDE_TOLERANCE  45.0f   // Toleranz für "Tor-seitige Linie"
#define DEFENDER_NEAR_GOAL_HOLD_MS    1200u   // Wie lange das Flag gehalten wird
```

### 8.4 Sensor-Reihenfolge (CW/CCW)

**Problem:** Die 32 Liniensensoren können physisch im oder gegen den Uhrzeigersinn angeordnet sein. Der Defender geht intern von **gegen den Uhrzeigersinn** aus (Index 0 = vorne, Index 8 = rechts).

**Lösung:**
```cpp
#define DEFENDER_SENSOR_CW false  // false = CCW (Standard), true = CW (gespiegelt)
```

Bei `DEFENDER_SENSOR_CW true` wird die Sensor-Reihenfolge beim Übergeben gespiegelt:
```cpp
out[(32 - i) % 32] = Line.line[i];
```

> Überprüfe die physische Sensor-Anordnung deines Roboters und passe diesen Wert an!

### 8.5 Nutzung in `loop()`

```cpp
#include "DefenderAdapter.h"

void loop() {
    System.Update.Sensors();
    System.Update.Calculations();

    if (System.Start) {
        DefenderAdapter.update();  // erledigt alles automatisch
    } else {
        Game.Stop();
    }

    Robot.Kicker.Update_End();
    Cam.Update();
}
```

`DefenderAdapter.update()` übernimmt intern:
1. Liniensensoren aufbereiten (`Line.line[32]` → `bool[32]`)
2. Ballwinkel konvertieren (−180..+180 → 0..360)
3. Tor-Nähe erkennen (Strafraum-Schutz)
4. `CamData` synthetisieren
5. `Defender::follow_Line()` aufrufen
6. Fahrbefehl anwenden (`Robot.Drive()`)
7. Kicker auslösen wenn `isKicking() == true`

---

## 9 Datenstrukturen

### `CamData` (in `lib/Lowlevel/Cam.h`)

```cpp
struct CamData {
    float GoalAgnle          = 0.0f;   // kamerarelativ, -180..+180°
    bool  BlueGoal_visible   = false;  // blaues Tor erkannt
    bool  YellowGoal_visible = false;  // gelbes Tor erkannt
    int   GoalHeight         = 0;      // Blob-Höhe (Nähe-Indikator, >60 = nah)
};
```

### `Drive_Data` (in `lib/Taktics/Defender.h`)

```cpp
struct Drive_Data {
    float turn;       // Drehmenge (aktuell nicht aktiv genutzt)
    float direction;  // Fahrtrichtung 0..360°
    float speed;      // Geschwindigkeit 0..100
};
```

### `KickState` (in `lib/Taktics/Defender.h`)

```cpp
struct KickState {
    float    ball_angle   = 0.0f;   // Ballwinkel bei letztem Check
    double   ball_dist    = 0.0;    // Balldistanz bei letztem Check
    uint32_t still_start  = 0;      // Zeitstempel seit Ball still
    uint32_t kick_start   = 0;      // Zeitstempel Kick-Start
    bool     kicking      = false;  // Kick gerade aktiv
};
```

---

## 10 Bekannte Einschränkungen & Tuning-Hinweise

| Thema | Hinweis |
|---|---|
| **Sensor-Richtung** | `DEFENDER_SENSOR_CW` muss zur physischen Sensor-Anordnung passen – falsche Einstellung führt dazu, dass der Roboter in die falsche Richtung der Linie fährt |
| **Kamera-Vorzeichen** | Falls der Roboter bei Tor-Sicht in die falsche Richtung dreht, `Cam.setSign(false)` setzen und testen |
| **Kick-Distanz** | `KICK_DIST_MIN/MAX` in `Defender.h` beschreiben die Distanz für den `KickState`-Mechanismus; die eigentliche Kick-Auslösung in `follow_Line` nutzt andere Schwellen (30..55 cm) – beide prüfen |
| **2-Sekunden-Timer** | Der Kick wird nur alle 2 s geprüft; bei einem sehr schnell rollenden Ball kann das zu langsam sein. Den Timer auf 1 s reduzieren falls nötig |
| **Line_distance** | Aktuell liefert der Adapter immer 0,0 oder 1,0 (binär). Eine feiner abgestufte Distanz (z.B. `Summe / 32.0f`) könnte das Linien-Following weicher machen |
| **GoalHeight-Schwelle** | Der Strafraum-Schutz greift wenn `GoalHeight > 60`. Der Adapter setzt synthetisch 0 oder 100. Die ±45°-Toleranz für "Tor-seitige Linie" ist ein Startwert und muss auf dem echten Spielfeld verifiziert werden |
| **Debugging** | Der Defender gibt via `Serial.print` detaillierte Debug-Ausgaben jede 2 s aus. Im Normalbetrieb kann dies die Rechenzeit leicht erhöhen |
