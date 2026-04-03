#include "Calculations.h"
#include "VectorCalc.h"

VectorCalcC VectorCalc;
Vec2 VectorCalcC::GetWantedVector(Vec2 current, Vec2 target){
    return target - current;
}