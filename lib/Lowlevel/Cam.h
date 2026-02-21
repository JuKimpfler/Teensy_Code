#pragma once
#include "Elementar.h"

class CamC{
    private:
        String Decode(String message);
    public:
        float x;
        float y;
        float area;                    
        String message;
        void init();
        void read();
};

extern CamC Cam;
 