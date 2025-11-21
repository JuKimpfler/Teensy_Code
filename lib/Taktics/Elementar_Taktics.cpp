#include "Elementar_Taktics.h"
TakticsC Taktics;

void TakticsC::BallSearch(){
    Robot.Drive(-BallSearchCalculations.OutAngle,0,30);
}

void TakticsC::GoalAttak(){
    Robot.Drive(0, 0, 40);
    
}



