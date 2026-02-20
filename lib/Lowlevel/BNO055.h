#pragma once
#include "Adafruit_BNO055.h"
#include "Elementar.h"

// Klasse zur Steuerung und Auswertung des BNO055 Sensors
class BNO055C {

    private:
        // Hier könnten interne Variablen für Kalibrierung etc. stehen
    public:

        double BNO_Cal; // Kalibrierungswert für den Kompass

        double TiltX;   // Neigung um X-Achse
        double TiltY;   // Neigung um Y-Achse
        double TiltZ;   // Neigung um Z-Achse

        // Initialisiert den Sensor
        void init();
        // Liest aktuelle Sensordaten und berechnet Tilt-Werte
        void read();
        // Führt Kalibrierung durch
        void Calibrate();
        // Zeigt Kalibrierungsstatus an
        void showCal();
        // Gibt TiltZ zurück
        double give_TiltZ();
        // Gibt TiltX zurück
        double give_TiltX();
        // Gibt TiltY zurück
        double give_TiltY();
};

// Globale Instanz des Sensors
extern BNO055C BNO055;