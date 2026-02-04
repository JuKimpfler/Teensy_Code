#pragma once
#include "Elementar.h"
#include "structs.h"
#include "IR.h"

class IrBallC{

    public:
        double getIrAngle();
        double getDriveAngle(double I);
        double getDistanceIr();
};

extern IrBallC IrBall;