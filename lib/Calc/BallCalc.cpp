#include "BallCalc.h"
#include "IR.h"
#include "BNO055.h"

BallCalcC BallCalc;
elapsedMillis Stilltimer;

void BallCalcC::CalcAngle(){
    lastA = Ball.Angle;
    lastD = Ball.Distance;

    Ball.Distance = (Ball.Distance_raw2 - IR.Dist_Offset)*2;

    if(Ball.Distance>200){Ball.Distance = 200;}

    Ball.Angle = Ball.Angle_raw-IR.Angle_Offset;

    diffA = abs(abs(Ball.Angle)-abs(lastA));
    diffD = abs(abs(Ball.Distance)-abs(lastD));

    if((diffA>0.13) || (diffD>5)){
        Stilltimer = 0;
    }

    Ball.Stilltime = Stilltimer;
}

void BallCalcC::getAngle(){

    if (Ball.Distance < 30){
        if(abs(Ball.Angle) < 30){
            DriveAngle = Ball.Angle * 1.335;
        }
        //else if(Ball.Distance < 10){
        //    DriveAngle = Ball.Angle * 1.7;
        //}
        else {
            DriveAngle = Ball.Angle * 1.5;
        }
    }
    else{ 
        DriveAngle = Ball.Angle;
    }
    //(1+Ball.Distance_raw/(780*2));// (abs(Ball.Angle)/360)

    DriveAngle = DriveAngle*-1;

    if(DriveAngle<-180){DriveAngle = DriveAngle+ 360;}
    else if (DriveAngle > 180){DriveAngle = DriveAngle - 360;}
}