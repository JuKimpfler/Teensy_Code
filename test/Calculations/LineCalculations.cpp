#include "LineCalculations.h"

LineCalcC LineCalc;

void LineCalcC::Calc(){
    int list[4] = {CS_LineA,CS_LineB,CS_LineC,CS_LineD};
    summex = 0;
    summey = 0;
    Distance = 0;
    for(int i = 0 ; i<32 ; i++){
        Distance = Distance + Line.Raw[i];
        summex = summex + (Line.Raw[i]*sinf(((i*11.25)*DEG_TO_RAD)));
        summey = summey + (Line.Raw[i]*cosf(((i*11.25)*DEG_TO_RAD)));
    }
    RawAngle = atan2f(summex,summey)*RAD_TO_DEG;

    DriveAngle = RawAngle-180;

    if (DriveAngle<0){DriveAngle = DriveAngle+360;}
    else if (DriveAngle>359){DriveAngle = DriveAngle-360;}
    if(DriveAngle<=180){DriveAngle=DriveAngle;}
    else if (DriveAngle>180){DriveAngle=DriveAngle-360;}
    if (DriveAngle<-180){DriveAngle = DriveAngle+360;}
    else if (DriveAngle>359){DriveAngle = DriveAngle-360;}

    DriveAngle = DriveAngle*-1;
}