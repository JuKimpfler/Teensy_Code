#include "Debug.h"
#include "Elementar.h" // for Serial/UART_Pixy definitions

// static instance defined at the bottom of this file

void DebugC::Start() {
    // clear buffer and add marker
    mainList = "> ";
}

void DebugC::Plot(const String &Name, double Value) {
    // ensure we have a start marker
    if (mainList.length() == 0) {
        Start();
    }
    mainList += Name;
    mainList += ":";
    mainList += String(Value);
    mainList += ", ";
}

void DebugC::Plot_List(const String names[], const double Values[], size_t count) {
    Start();
    for (size_t i = 0; i < count; ++i) {
        mainList += names[i];
        mainList += ":";
        mainList += String(Values[i]);
        if (i + 1 < count) {
            mainList += ", ";
        }
    }
}

void DebugC::Send(bool BLE) {
    if (BLE) {
        Serial.println(mainList);
    } else {
        UART_Pixy.println(mainList);
    }
}

// global instance
DebugC Debug;
