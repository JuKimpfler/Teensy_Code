#pragma once
#include "Elementar.h"

class BLC {

    private:
        String decode(String message);
    public:
        String Rolle;
        
        void init();
        void showRolle();
        void doRolle();
};

extern BLC BL;