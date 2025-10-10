#pragma once
#include <structs.h>
#include <Param.h>
#include <math.h>

class BallSearchCalculationsC{
    public:
        float getAngleCurvature(float inAngle, float dist);
};

extern BallSearchCalculationsC BallSearchCalculations;