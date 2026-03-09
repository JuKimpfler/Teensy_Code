#include "BallCalc.h"
#include "IR.h"

BallCalcC BallCalc;

void BallCalcC::CalcAngle(){
    Ball.Angle = Ball.Angle_raw-IR.Angle_Offset;

    /*
    double x = 0; 
    double y = 0;

    for (uint_fast8_t i=0; i<16; i++)
    {
        x = x +  IR.IR_Values[i] * sinf(((i*22.5)*DEG_TO_RAD));
        y = y +  IR.IR_Values[i] * cosf(((i*22.5)*DEG_TO_RAD));
    }

    Ball.Angle = atan2f(x,y)*RAD_TO_DEG;

    if(Ball.Angle<-180){Ball.Angle = Ball.Angle+ 360;}
    else if (Ball.Angle > 180){Ball.Angle = Ball.Angle - 360;}

    Ball.Angle = -Ball.Angle;

    //if(Ball.Distance<5){Ball.Angle=0;}*/
}

void BallCalcC::CalcDist(){
    
    Ball.Distance_raw2 = ((1/sqrt(Ball.Distance_raw)) * 2000);
    Ball.Distance = (Ball.Distance_raw2 -IR.Dist_Offset)*2;
}

void BallCalcC::getAngle(){

    Zone = "unread";
    
    if(U.Ran(Ball.Angle,-5,5)&&U.Ran(Ball.Distance,-30,Dist_Schwelle_min)){DriveAngle=Ball.Angle;Zone = "A";  /* Zone A*/ }
    else if(U.Ran(Ball.Angle,-5,5)&&U.Ran(Ball.Distance,Dist_Schwelle_min,Dist_Schwelle_max)){DriveAngle=Ball.Angle*B_Faktor; Zone = "B"; /* Zone B*/ }
    else if(U.Ran(Ball.Angle,5,90)&&U.Ran(Ball.Distance,-30,Dist_Schwelle_min)){DriveAngle=Ball.Angle*C_Faktor;Zone = "C1"; /* Zone C1*/ }
    else if(U.Ran(Ball.Angle,-5,-90)&&U.Ran(Ball.Distance,-30,Dist_Schwelle_min)){DriveAngle=Ball.Angle*C_Faktor; Zone = "C2";/* Zone C2*/ }
    else if(U.Ran(Ball.Angle,90,130)&&U.Ran(Ball.Distance,-30,Dist_Schwelle_min)){DriveAngle=Ball.Angle*H_Faktor;Zone = "H1"; /* Zone C1*/ }
    else if(U.Ran(Ball.Angle,-90,-130)&&U.Ran(Ball.Distance,-30,Dist_Schwelle_min)){DriveAngle=Ball.Angle*H_Faktor; Zone = "H2";/* Zone C2*/ }
    else if(U.Ran(Ball.Angle,130,180)&&U.Ran(Ball.Distance,-30,Dist_Schwelle_min)){DriveAngle=-90;Zone = "D1"; /* Zone D1*/ }
    else if(U.Ran(Ball.Angle,-130,-180)&&U.Ran(Ball.Distance,-30,Dist_Schwelle_min)){DriveAngle=90;Zone = "D2"; /* Zone D2*/ }
    else if(U.Ran(Ball.Angle,5,90)&&U.Ran(Ball.Distance,Dist_Schwelle_min,Dist_Schwelle_max)){DriveAngle=Ball.Angle*E_Faktor;Zone = "E1"; /* Zone E1*/ }
    else if(U.Ran(Ball.Angle,-5,-90)&&U.Ran(Ball.Distance,Dist_Schwelle_min,Dist_Schwelle_max)){DriveAngle=Ball.Angle*E_Faktor;Zone = "E2"; /* Zone E2*/ }
    else if(U.Ran(Ball.Angle,90,160)&&U.Ran(Ball.Distance,Dist_Schwelle_min,Dist_Schwelle_max)){DriveAngle=Ball.Angle*F_Faktor;Zone = "F1"; /* Zone F1*/ }
    else if(U.Ran(Ball.Angle,-90,-160)&&U.Ran(Ball.Distance,Dist_Schwelle_min,Dist_Schwelle_max)){DriveAngle=Ball.Angle*F_Faktor;Zone = "F2"; /* Zone F2*/ }
    else if(U.Ran(Ball.Angle,160,180)&&U.Ran(Ball.Distance,Dist_Schwelle_min,Dist_Schwelle_max)){DriveAngle=140;Zone = "G2"; /* Zone G1*/ }
    else if(U.Ran(Ball.Angle,-160,-180)&&U.Ran(Ball.Distance,Dist_Schwelle_min,Dist_Schwelle_max)){DriveAngle=-140;Zone = "G2"; /* Zone G2*/ }
    else{
        DriveAngle = Ball.Angle;
        Zone = "Error";
    }

    if(DriveAngle<-180){DriveAngle = DriveAngle+ 360;}
    else if (DriveAngle > 180){DriveAngle = DriveAngle - 360;}
}

void BallCalcC::calcTorAngle(){
    //TODO
}