#include "IrBallCalculations.h"
IrBallC IrBall;

double IrBallC::getIrAngle(){
    double totalx = 0;
    double totaly = 0;
    double totalAngle = 0;
    for(int i = 0; i < 8; i++){
        totalx = totalx + (IR.GetData(i)) * sinf(IR_Pos[i]);
        totaly = totaly + (IR.GetData(i)) * cosf(IR_Pos[i]);
    }
    totalAngle = atan2f(totaly,totalx);
    return totalAngle;
}

double IrBallC::getDriveAngle(double IRAngle){
    double totalAngle = 0;
    double Anfahrtsfaktor = 1.3;
    totalAngle = IRAngle * Anfahrtsfaktor;
    return totalAngle;
}

double IrBallC::getDistanceIr(){
    double totalDist = 0;
    for(int i = 0; i < 8; i++){
        totalDist += IR.GetData(i);
    }
    totalDist *= distFactor;
}