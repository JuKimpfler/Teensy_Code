#include "ReflectionCalculations.h"

Vec2 ReflectionCalc::getReflection(Vec2 first, Vec2 second){
    Vec2 vector = Vec2{0,0};
    vector.y = first.y+(second.y-first.y)/(second.x/first.x + 1);
    return vector;
}