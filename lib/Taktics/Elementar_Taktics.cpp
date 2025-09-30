#include "Elementar_Taktics.h"
TakticsC Taktics;

void TakticsC::BallSearch(){

}

void TakticsC::GoalAttak(){
    Robot.Drive(0, 180);
}

void TakticsC::CornerLeftEscape(){
    Robot.Drive(115, 0);
}

void TakticsC::CornerRightEscape(){
    Robot.Drive(-115, 0);
}

void TakticsC::CalculateAttackChances(){
    float BallDir = 0;
    float BallDist = 0;

    if(Utils.isinRange(BallDir,-60,60)){
        AttackChances = 5;
    }
    else{
        AttackChances = -(abs(BallDir)-60)/24;
    }
    AttackChances += BallDist / 20;
}



