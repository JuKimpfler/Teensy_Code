#pragma once
#include "Elementar.h"

#include <vector>

class DebugC {
    private:
        // internal buffer that accumulates the formatted text for plotting
        String mainList;
    public:
        void Start(); // leert die mainliste und beginnt sie mit "> " als Kennzeichnung
            // Step 1: Initializes the debug output.
        void Plot(const String &Name, double Value); // hängt das Paar Name:Value an (format kompatibel mit VSC Serial Plotter)
        void Plot(const String &Name, String Value);
        
        // Step 2: Adds a name-value pair to the debug output.
        void Plot_List(String names, const double Values[], size_t count); // erstellt eine Zeile "Name1: v1, Name2: v2, ..."
        void Plot_List(String names, bool Values[], size_t count);
        void Plot_List(String names, String Values[], size_t count);
        // Step 3: Adds a list of name-value pairs to the debug output.
        void Send(bool BLE = true); // Sendet die aktuelle mainliste per Serial.println oder – wenn BLE==false – über UART_Pixy
            // Step 4: Sends the current main list via Serial or UART_Pixy.
        
};

extern DebugC Debug;