#pragma once
#include "Elementar.h"
#include "Expander.h" 

class LineC {
    private :
        int list[4] = {CS_LineA,CS_LineB,CS_LineC,CS_LineD};
    public:
        void read();
        float Raw[32];
        float Raw_Out[8];
};

extern LineC Line;