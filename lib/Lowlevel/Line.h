#pragma once
#include "Elementar.h"
#include "Expander.h" 

class LineC {
    private :
        int list[4] = {CS_LineA,CS_LineB,CS_LineC,CS_LineD};
    public:
        float Summe; 
        void read();
        void read_VW();
        float Raw[32];
        float Raw_VW[8];
        float Summe_VW;
};

extern LineC Line;