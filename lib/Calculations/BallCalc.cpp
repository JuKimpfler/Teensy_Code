#include "BallCalc.h"
#include "IR.h"

BallCalcC BallCalc;

void BallCalcC::CalcAngle(){

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
    if(Ball.Distance_raw<30){
        Ball.Distance = 1000;
    }
    else{
        //if(Ball.Distance_raw<130){
        //    Ball.Distance = 40+(100/Ball.Distance_raw)*80;
        //}
        //else{
            //Ball.Distance = ((Ball.Distance_raw - IR.DistCal)*-1) * IR.DistFaktor;//pow(IR.DistCal / sum, 1 / IR.DistFaktor);//IR.DistCal * (1/sqrt(sum));
            Ball.Distance_raw2 = ((1/sqrt(Ball.Distance_raw)) * 2000);
            Ball.Distance = Ball.Distance_raw2-IR.DistCal;
            //}
        
        //if(Ball.Distance< 0){
        //    Ball.Distance=0;
        //}
    }
}

void BallCalcC::getAngle(){
    // Berechnung 2024/2025
    /*if(abs(Ball.Angle) >35){
        double Ball.Distance_p = (Ball.Distance/100)*0.7; // 0.7
        double dir_pi = ((abs(Ball.Angle)/1.8)/100); // 0.5
        double dir_p = (dir_pi)*0.3; // 0.15
        double temp = ((dir_p + Ball.Distance_p) * 0.5)+1; // 1.32 //180-(3705.3/(pow(Ball.Distance, 1.19)+34.38));
        DriveAngle = temp * Ball.Angle ; //
    }
    else{
        DriveAngle = Ball.Angle;
    }
    */

    int Dist_Schwelle_min = 5;
    int Dist_Schwelle_max = 60;
    Zone = "unread";
    
    if(U.Ran(Ball.Angle,-5,5)&&U.Ran(Ball.Distance,0,Dist_Schwelle_min)){DriveAngle=Ball.Angle;Zone = "A";  /* Zone A*/ }
    else if(U.Ran(Ball.Angle,-5,5)&&U.Ran(Ball.Distance,Dist_Schwelle_min,Dist_Schwelle_max)){DriveAngle=Ball.Angle*B_Faktor; Zone = "B"; /* Zone B*/ }
    else if(U.Ran(Ball.Angle,5,90)&&U.Ran(Ball.Distance,0,Dist_Schwelle_min)){DriveAngle=Ball.Angle*C_Faktor;Zone = "C1"; /* Zone C1*/ }
    else if(U.Ran(Ball.Angle,-5,-90)&&U.Ran(Ball.Distance,0,Dist_Schwelle_min)){DriveAngle=Ball.Angle*C_Faktor; Zone = "C2";/* Zone C2*/ }
    else if(U.Ran(Ball.Angle,90,130)&&U.Ran(Ball.Distance,0,Dist_Schwelle_min)){DriveAngle=Ball.Angle*H_Faktor;Zone = "H1"; /* Zone C1*/ }
    else if(U.Ran(Ball.Angle,-90,-130)&&U.Ran(Ball.Distance,0,Dist_Schwelle_min)){DriveAngle=Ball.Angle*H_Faktor; Zone = "H2";/* Zone C2*/ }
    else if(U.Ran(Ball.Angle,130,180)&&U.Ran(Ball.Distance,0,Dist_Schwelle_min)){DriveAngle=-90;Zone = "D1"; /* Zone D1*/ }
    else if(U.Ran(Ball.Angle,-130,-180)&&U.Ran(Ball.Distance,0,Dist_Schwelle_min)){DriveAngle=90;Zone = "D2"; /* Zone D2*/ }
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

    /*/Berechnung 2026 Rafael
    if(Ball.Distance<25 && Ball.Distance>15){ // Wenn richtiger Abstand zum Ball 
        if(Ball.Angle<-20){ // CW Kreis um Ball Fahren
            DriveAngle=Ball.Angle-90;
        }
        else if(Ball.Angle>20){ // CCW Kreis um Ball Fahren
            DriveAngle=Ball.Angle+90;
        }
        else { // Mit leichter Kurve entanfahrt machen.
            DriveAngle=Ball.Angle*1.1;
        }
    }
    else if (Ball.Angle>=25){
        if(Ball.Angle>90){
            DriveAngle=Ball.Angle-(atanf(20/Ball.Distance)*RAD_TO_DEG); // Versuchter Treffpunkt des roboters auf Kreisförmiger Bahn um Ball mit Radius 13 
        }
        else if (Ball.Angle<-90){
            DriveAngle=Ball.Angle+(atanf(20/Ball.Distance)*RAD_TO_DEG); // Versuchter Treffpunkt des roboters auf Kreisförmiger Bahn um Ball mit Radius 13
        }
        else if (Ball.Angle<=90 && Ball.Angle>=-90){
            DriveAngle=Ball.Angle; // Versuchter Treffpunkt des roboters auf Kreisförmiger Bahn um Ball mit Radius 13
        }
    }
    */

    if(DriveAngle<-180){DriveAngle = DriveAngle+ 360;}
    else if (DriveAngle > 180){DriveAngle = DriveAngle - 360;}
}

void BallCalcC::calcTorAngle(){
    //TODO
}