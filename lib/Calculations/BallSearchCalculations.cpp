#include "BallSearchCalculations.h"

BallSearchCalculationsC BallSearchCalculations;

void BallSearchCalculationsC::getAngleCurvature(float inAngle, float dist){
    if(abs(inAngle) >35){
        float dist_p = (dist/100)*0.7; // 0.7
        float dir_pi = ((abs(inAngle)/1.8)/100); // 0.5
        float dir_p = (dir_pi)*0.3; // 0.15
        float temp = ((dir_p + dist_p) * 0.5)+1; // 1.32 //180-(3705.3/(pow(dist, 1.19)+34.38));
        OutAngle = temp * inAngle ; //
    }
    else{
        OutAngle = inAngle;
    }
    
}