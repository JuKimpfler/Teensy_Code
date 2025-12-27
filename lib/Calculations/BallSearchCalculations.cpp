#include "BallSearchCalculations.h"

BallSearchCalculationsC BallSearchCalculations;

void BallSearchCalculationsC::getAngleCurvature(float inAngle, float dist){
    // Berechnung 2024/2025
    /*if(abs(inAngle) >35){
        float dist_p = (dist/100)*0.7; // 0.7
        float dir_pi = ((abs(inAngle)/1.8)/100); // 0.5
        float dir_p = (dir_pi)*0.3; // 0.15
        float temp = ((dir_p + dist_p) * 0.5)+1; // 1.32 //180-(3705.3/(pow(dist, 1.19)+34.38));
        OutAngle = temp * inAngle ; //
    }
    else{
        OutAngle = inAngle;
    }
    */

    //Berechnung 2026 Rafael
    if(dist<25 && dist>15){ // Wenn richtiger Abstand zum Ball 
        if(inAngle<-20){ // CW Kreis um Ball Fahren
            OutAngle=inAngle-90;
        }
        else if(inAngle>20){ // CCW Kreis um Ball Fahren
            OutAngle=inAngle+90;
        }
        else { // Mit leichter Kurve entanfahrt machen.
            OutAngle=inAngle*1.1;
        }
    }
    else{
        if(inAngle<0){
            OutAngle=inAngle-(atanf(20/dist)*RAD_TO_DEG); // Versuchter Treffpunkt des roboters auf Kreisförmiger Bahn um Ball mit Radius 13 
        }
        else{
            OutAngle=inAngle+(atanf(20/dist)*RAD_TO_DEG); // Versuchter Treffpunkt des roboters auf Kreisförmiger Bahn um Ball mit Radius 13
        }
    }
}