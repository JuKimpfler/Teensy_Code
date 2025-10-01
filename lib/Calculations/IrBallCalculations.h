#pragma once
#include "Elementar.h"
#include "structs.h"
#include "IR.h"

class IrBallC{

    public:
        float getIrAngle();
        float getDriveAngle(float I);
        float getDistanceIr();
};

extern IrBallC IrBall;