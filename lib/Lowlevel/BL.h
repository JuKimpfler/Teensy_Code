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
        void decode(String message1);

    public:
        String Rolle = "N"; // Aktuelle Rolle: "A" (Angreifer), "D" (Verteidiger), "N" (Neutral)

        bool SinglePlayer = true; // true, wenn kein gültiger Peer-Frame empfangen wurde

        void showRolle();
        void doRolle();
};

// Globale Instanz der Rollenklasse
extern BLC BL;