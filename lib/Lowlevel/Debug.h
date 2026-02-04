#pragma once
#include "Elementar.h"

class DebugC {
    private:
        int IR_Values[8] ;
        void Plot(double Value);
        void Plot_List(double Values[8]);

    public:
        void Send(bool BLE = true);
        
};

extern DebugC Debug;