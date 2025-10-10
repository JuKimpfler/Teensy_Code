#include "BallSearchCalculations.h"

BallSearchCalculationsC BallSearchCalculations;

float BallSearchCalculationsC::getAngleCurvature(float inAngle, float dist){
    if(abs(inAngle) >45){
        //ich habe absolut keine Ahnung wovon 3705.3, 1.19 oder 34.38 kommen also nicht fragen
        float temp = 180-(3705.3/(pow(dist, 1.19)+34.38));
        return temp + inAngle;
    }
    else{
        return inAngle;
    }
}