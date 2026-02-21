#include "IrBallCalculations.h"
IrBallC IrBall;

float IrBallC::getIrAngle(){
    float totalx = 0;
    float totaly = 0;
    float totalAngle = 0;
    for(int i = 0; i < 8; i++){
        totalx = totalx + (IR.GetData(i)) * sinf(IR_Pos[i]);
        totaly = totaly + (IR.GetData(i)) * cosf(IR_Pos[i]);
    }
    totalAngle = atan2f(totaly,totalx);
    return totalAngle;
}

float IrBallC::getDriveAngle(float IRAngle){
    float totalAngle = 0;
    float Anfahrtsfaktor = 1.3;
    totalAngle = IRAngle * Anfahrtsfaktor;
    return totalAngle;
}

float IrBallC::getDistanceIr(){
    float totalDist = 0;
    for(int i = 0; i < 8; i++){
        totalDist += IR.GetData(i);
    }
    totalDist *= distFactor;
}