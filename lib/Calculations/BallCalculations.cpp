#include "BallCalculations.h"
#include "IR.h"

BallCalcC BallCalc;

void BallCalcC::CalcAngle(){
    double x = 0; 
    double y = 0;

    for (uint_fast8_t i=0; i<16; i++)
    {
        x = x +  IR.IR_Values[i] * sinf(((i*22.5)*DEG_TO_RAD));
        y = y +  IR.IR_Values[i] * cosf(((i*22.5)*DEG_TO_RAD));
    }

    Ball.Angle = atan2f(x,y)*RAD_TO_DEG;

    if (Ball.Angle<0){Ball.Angle = Ball.Angle+360;}
    else if (Ball.Angle>359){Ball.Angle = Ball.Angle-360;}
    if(Ball.Angle<=180){Ball.Angle=Ball.Angle;}
    else if (Ball.Angle>180){Ball.Angle=Ball.Angle-360;}
    if (Ball.Angle<-180){Ball.Angle = Ball.Angle+360;}
    else if (Ball.Angle>359){Ball.Angle = Ball.Angle-360;}

    Ball.Angle = -Ball.Angle;

    if(Ball.Distance<5){Ball.Angle=0;}
}

void BallCalcC::CalcDist(){
    double sum = 0; 
    for (int i=0; i<16; i++)
    {
        sum = sum + IR.IR_Values[i];
    }

    sum = (sum/16)+1;

    IR.Distance_raw = sum;
    if(sum<30){
        Ball.Distance = 1000;
        Ball.inSight = false;
    }
    else{
        Ball.inSight = true;
        if(sum<130){
            Ball.Distance = 40+(100/sum)*80;
        }
        else{
            IR.Distance_raw2 = (sum - 780)*-1;
            Ball.Distance = IR.Distance_raw2 * 0.12;//pow(IR.DistCal / sum, 1 / IR.DistFaktor);//IR.DistCal * (1/sqrt(sum));
        }
        
        if(Ball.Distance< 0){
            Ball.Distance=0;
        }
    }
}

void BallCalcC::getAngle(double inAngle, double dist){
    // Berechnung 2024/2025
    /*if(abs(inAngle) >35){
        double dist_p = (dist/100)*0.7; // 0.7
        double dir_pi = ((abs(inAngle)/1.8)/100); // 0.5
        double dir_p = (dir_pi)*0.3; // 0.15
        double temp = ((dir_p + dist_p) * 0.5)+1; // 1.32 //180-(3705.3/(pow(dist, 1.19)+34.38));
        DriveAngle = temp * inAngle ; //
    }
    else{
        DriveAngle = inAngle;
    }
    */

    //Berechnung 2026 Rafael
    if(dist<25 && dist>15){ // Wenn richtiger Abstand zum Ball 
        if(inAngle<-20){ // CW Kreis um Ball Fahren
            DriveAngle=inAngle-90;
        }
        else if(inAngle>20){ // CCW Kreis um Ball Fahren
            DriveAngle=inAngle+90;
        }
        else { // Mit leichter Kurve entanfahrt machen.
            DriveAngle=inAngle*1.1;
        }
    }
    else if (inAngle>=25){
        if(inAngle>90){
            DriveAngle=inAngle-(atanf(20/dist)*RAD_TO_DEG); // Versuchter Treffpunkt des roboters auf Kreisförmiger Bahn um Ball mit Radius 13 
        }
        else if (inAngle<-90){
            DriveAngle=inAngle+(atanf(20/dist)*RAD_TO_DEG); // Versuchter Treffpunkt des roboters auf Kreisförmiger Bahn um Ball mit Radius 13
        }
        else if (inAngle<=90 && inAngle>=-90){
            DriveAngle=inAngle; // Versuchter Treffpunkt des roboters auf Kreisförmiger Bahn um Ball mit Radius 13
        }
    }
}

void BallCalcC::calcTorAngle(){
    //TODO
}