#pragma once
#include "Elementar.h"
#include "Expander.h" 

class LineC {
    private :
        int list[4] = {CS_LineA,CS_LineB,CS_LineC,CS_LineD};
    public:
        double Summe; 
        void read();
        void read_VW();
        double Raw[32];
        double Raw_VW[8];
        double Summe_VW;
};

extern LineC Line;