
// robot_lut.h
// ===========
// Automatisch generiert von generate_lut.py
// Optimiert mit LUT_Simplifier.py  (Symmetrie-Seite: links, Glätt-Radius: 1)

// Modell      : roboter_rl_modell_128.pth
// Neuronen    : 128
// Max-Distanz : 424.2641 cm
// Winkel      : 0–359° (360 Werte, 1°-Schritte)
// Abstand     : 0–200 cm (201 Werte, 1 cm-Schritte)
// Einträge    : 72360 (uint8_t, 0–89)
//
// Zugriff:
//   uint8_t aktion = robot_lut_lookup(winkel_deg, abstand_cm);
//
// Jeder Aktionswert steht für eine Fahrtrichtung:
//   Aktion A → Winkel = A * 4°  (z.B. Aktion 0 = 0°, Aktion 45 = 180°, Aktion 89 = 356°)

#pragma once
#include <Arduino.h>
#include <stdint.h>
#include "Utils.h"

class LUTC{
  private:
    // Tabellen-Dimensionen
    #define LUT_WINKEL_MIN     0
    #define LUT_WINKEL_MAX     359
    #define LUT_ANZAHL_WINKEL  360
    #define LUT_ABSTAND_MIN    0
    #define LUT_ABSTAND_MAX    200
    #define LUT_ANZAHL_ABST    201
    #define LUT_GESAMT         72360
    #define LUT_AKTIONEN       90
    #define LUT_WINKEL_SCHRITT (360.0f / LUT_AKTIONEN)   // 4°

  public: 
    float robot_aktion_zu_winkel(uint8_t aktion);
    uint8_t robot_lut_lookup(int winkel_deg, int abstand_cm);
    float get_DriveAngle(int winkel, int abstand);
};

extern LUTC LUT;
