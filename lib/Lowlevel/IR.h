#pragma once
#include "Elementar.h"
#include "Expander.h"

class IRC {
    private:
        int IR_Values[8] ;

    public:
        void read();
        int GetData(int Port);
};

extern IRC IR;