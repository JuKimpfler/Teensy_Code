#pragma once
#include "Elementar.h"

class CamC{
    private:
        void Decode(String message);
    public:
        float x;
        float y;
        float area;
        float rest;
        bool goal; // true = gelb , false = blau                    
        String message;
        void read();
};

extern CamC Cam;
 