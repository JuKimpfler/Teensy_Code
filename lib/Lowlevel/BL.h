#pragma once
#include "Elementar.h"

class BLC {

    private:
        String decode(String message1);
    public:
        String Rolle="N";
        String message;

        bool SinglePlayer; 
        float Dist_Player2;
        
        void init();
        void showRolle();
        void doRolle();
};

extern BLC BL;