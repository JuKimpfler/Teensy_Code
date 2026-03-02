#pragma once
#include "Elementar.h"

class LineC {
    private :
        int ADC_Befehle[8] = {2048,4096,6144,8192,10240,12288,14336,16384};
    public:
        float min_schwelle=5000;
        float max_schwelle=10000;
        int VW_Summe;
        int Summe;
        bool line[32] ;
        bool lineVW[8];
        void read_Fast(); 
        void init();
        void Calibrate(int Modes);
};

extern LineC Line;