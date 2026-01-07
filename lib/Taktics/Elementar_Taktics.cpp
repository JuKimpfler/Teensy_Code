#include "Elementar_Taktics.h"
TakticsC Taktics;

void TakticsC::BallSearch(){
    Robot.Drive(-BallCalc.OutAngle,0,MainSpeed);
}

void TakticsC::GoalAttak(){
    Robot.Drive(0, 0, MainSpeed); 
}



