#pragma once
#include "Elementar.h"

class LineC {
    private :
        int ADC_Befehle[8] = {2048,4096,6144,8192,10240,12288,14336,16384};
    public:
        int VW_Summe;
        int Summe;
        float calib_line[40];
        uint16_t line[32] ;
        uint16_t lineVW[8];
        uint16_t Values_raw[32];
        uint16_t Values_raw_VW[8];
        void read_Fast(); 
        void init();
        void Calibrate(int Modes);
};

extern LineC Line;