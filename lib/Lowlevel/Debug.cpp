#include "Debug.h"
#include "Elementar.h" // for Serial/UART_Pixy definitions

DebugC Debug;

/**
 * Initializes the debug output by clearing the buffer and setting the start marker.
 * 
 * This function is used to prepare the debug output for the next set of data.
 * It clears the mainList string and sets the start marker to "> ".
 * The mainList string is used to store the debug output.
 * The start marker is used to indicate the beginning of the debug output.
 * 
 * This function should be called at the beginning of the debug output sequence.
 * It is typically called once per debug output sequence.
 * 
 * Example:
 * Debug.Start();
 * Debug.Plot("Name1", 1.0);
 * Debug.Plot("Name2", 2.0);
 * Debug.Send();
 */
void DebugC::Start() {
    // clear buffer and add marker
    mainList = "> ";
        // Step 1: Initialize the debug output by clearing the buffer and setting the start marker.
}

/**
 * Adds a name-value pair to the debug output.
 * This function is used to add a single name-value pair to the debug output.
 * It appends the name and value to the mainList string and adds a comma separator.
 * If the mainList string is empty, it calls Start() to initialize the string with a start marker.
 * The function should be called for each name-value pair to be added to the debug output.
 * It is typically called multiple times per debug output sequence.
 * Example:
 * Debug.Plot("Name1", 1.0);
 * Debug.Plot("Name2", 2.0);
 * Debug.Send();
 */
void DebugC::Plot(const String &Name, double Value) {
    // ensure we have a start marker
    if (mainList.length() == 0) {
        Start();
    }
        // Step 2: Add a name-value pair to the debug output.
    mainList += Name;
    mainList += ":";
    mainList += String(Value);
    mainList += ", ";
}

void DebugC::Plot(const String &Name, String Value) {
    // ensure we have a start marker
    if (mainList.length() == 0) {
        Start();
    }
        // Step 2: Add a name-value pair to the debug output.
    mainList += Name;
    mainList += ":";
    mainList += Value;
    mainList += ", ";
}

/**
 * Adds a list of name-value pairs to the debug output.
 * This function is used to add multiple name-value pairs to the debug output.
 * It appends each name and value to the mainList string and adds a comma separator.
 * If the mainList string is empty, it calls Start() to initialize the string with a start marker.
 * The function should be called once per debug output sequence.
 * Example:
 * Debug.Plot_List(names, values, count);
 * Debug.Send();
 */
void DebugC::Plot_List(String name, double Values[], size_t count) {
    for (size_t i = 0; i < count; ++i) {
        mainList += (name + String(i));
        mainList += ":";
        mainList += String(Values[i]);
        if (i + 1 < count) {
            mainList += ", ";
        }
    }
        // Step 3: Add a list of name-value pairs to the debug output.
    mainList += ", ";
}


void DebugC::Plot_List(String name, bool Values[], size_t count) {
    for (size_t i = 0; i < count; ++i) {
        mainList += (name + String(i));
        mainList += ":";
        mainList += String((Values[i])*300);
        if (i + 1 < count) {
            mainList += ", ";
        }
    }
        // Step 3: Add a list of name-value pairs to the debug output.
        mainList += ", ";
}

void DebugC::Plot_List(String name, uint16_t Values[], size_t count) {
    for (size_t i = 0; i < count; ++i) {
        mainList += (name + String(i));
        mainList += ":";
        mainList += String(Values[i]);
        if (i + 1 < count) {
            mainList += ", ";
        }
    }
        // Step 3: Add a list of name-value pairs to the debug output.
        mainList += ", ";
}


void DebugC::Plot_List(String name, String Values[], size_t count) {
    for (size_t i = 0; i < count; ++i) {
        mainList += (name + String(i));
        mainList += ":";
        mainList += Values[i];
        if (i + 1 < count) {
            mainList += ", ";
        }
    }
        // Step 3: Add a list of name-value pairs to the debug output.
        mainList += ", ";
}

void DebugC::Send(bool BLE) {
    if (!BLE) {
        Serial.println(mainList);
    } else {
        //UART_Pixy.println(mainList);
    }
}

