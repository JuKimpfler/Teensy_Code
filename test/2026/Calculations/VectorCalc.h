#include "Calculations.h"
#include "Param.h"
#include "Elementar.h"
#pragma once

class VectorCalcC{
public:
    Vec2 GetWantedVector(Vec2 current, Vec2 target);
};

extern VectorCalcC VectorCalc;