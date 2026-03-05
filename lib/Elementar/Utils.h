#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

class UtilsC {
public:
    bool Ran(float In, float min, float max);
    double Circel(double Angle);
    
    // Kopierfunktionen
    uint16_t* List_copy(const uint16_t source[], uint16_t dest[], int len);
    int16_t* List_copy(const int16_t source[], int16_t dest[], int len);
};

extern UtilsC U;

#endif

