#pragma once
#include "Elementar.h"
#include "Expander.h"
#include "ir_ring.h"

class IRC {
    private:

    public:
        float Angle;
        float Distance;
        int IR_Values[16] ;

        void read();
        int GetData(int Port);
        void init();

};

extern IRC IR;