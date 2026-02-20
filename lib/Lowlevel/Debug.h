#pragma once
#include "Elementar.h"

#include <vector>

class DebugC {
    private:
        // internal buffer that accumulates the formatted text for plotting
        String mainList;
    public:
        void Start(); // leert die mainliste und beginnt sie mit "> " als Kennzeichnung
        void Plot(const String &Name, double Value); // hängt das Paar Name:Value an (format kompatibel mit VSC Serial Plotter)
        void Plot_List(const String names[], const double Values[], size_t count); // erstellt eine Zeile "Name1: v1, Name2: v2, ..."
        void Send(bool BLE = true); // Sendet die aktuelle mainliste per Serial.println oder – wenn BLE==false – über UART_Pixy
        
};

extern DebugC Debug;