#pragma once
#include "Elementar.h"

class DebugC {
    private:
        int IR_Values[8] ;
        void Plot(float Value);
        void Plot_List(float Values[8]);

    public:
        void Send(bool BLE = true);
        
};

extern DebugC Debug;