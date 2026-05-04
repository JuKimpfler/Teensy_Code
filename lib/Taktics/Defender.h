#pragma once
#include "Elementar.h"
#include "Wire.h"
#include "Calc.h"
#include "Lowlevel.h"

class DefenderC{
    private:
        void Bump_Ball();
        float Follow_Line();

        bool State_Def;
    public:
        void set_State(bool State);
        void init();

        void Update();
};

extern DefenderC Defender ;