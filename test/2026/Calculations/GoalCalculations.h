#pragma once
#include <Param.h>
#include <structs.h>
#include <math.h>

class GoalCalculationsC{
    public:
        Angle calculateAngle(Vec2 goalPosition);
        Vec2 calculatedDistance(Vec2 goalSize);
};

extern GoalCalculationsC GoalCalculations;