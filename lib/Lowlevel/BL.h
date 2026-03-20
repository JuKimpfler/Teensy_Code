#pragma once
#include "Elementar.h"

// Klasse zur Steuerung der Ballrolle und P2P-Kommunikation über das ESP-Modul.
//
// Protokoll-Übersicht (UART_Pixy ↔ ESP-Bridge):
//   TX: "DAT:0<aaa>0<ddd>0<i>0<i>0<eee>$"  – Nutzdaten an Peer-Roboter
//   TX: "DBG:<text>\r\n"                    – Debug an Monitor (nicht an Peer)
//   RX: Wie TX (mit oder ohne "DAT:"-Prefix) – Nutzdaten vom Peer-Roboter
//
// Empfang erfolgt nicht-blockierend byte-weise; Frames werden erst verarbeitet,
// wenn das '$'-Terminierungszeichen empfangen wurde.
class BLC {

    private:
        // Dekodiert eine mindestens 16 Zeichen lange Nutzlast eines RX-Frames.
        void decode(const char* payload);

    public:
        String Rolle = "N"; // Aktuelle Rolle: "A" (Angreifer), "D" (Verteidiger), "N" (Neutral)

        bool SinglePlayer = true; // true, wenn kein gültiger Peer-Frame empfangen wurde

        // Zeigt die aktuelle Rolle per RGB-LED an.
        void showRolle();

        // Nicht-blockierende Kommunikationslogik; muss jeden Loop-Zyklus aufgerufen werden.
        // Empfängt und verarbeitet RX-Frames und sendet zyklisch den eigenen TX-Frame.
        // Setzt Rolle="N" und SinglePlayer=true wenn seit >500 ms kein gültiger Frame kam.
        void doRolle();

        // Sendet eine Debug-Nachricht mit "DBG:"-Prefix über UART_Pixy.
        // Die ESP-Bridge erkennt den Prefix und leitet die Nachricht an den
        // Debug-Monitor weiter, ohne sie an den Peer-Roboter zu senden.
        // Hat keinen Effekt wenn Debug.debugEnabled == false.
        void sendDebug(String msg);

        // Konfiguriert das ESP-Modul für den Debug-Monitor-Betrieb.
        // Nur im Setup/Config-Zustand aufrufen – niemals im Spiel-Loop.
        void setDebugEnabled(bool enabled);
};

// Globale Instanz der Rollenklasse
extern BLC BL;