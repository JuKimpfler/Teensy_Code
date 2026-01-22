#include "Elementar_Taktics.h"
#include "BallSearchCalculations.h"
#include "Cam.h"
TakticsC Taktics;

void TakticsC::BallSearch(){
    Robot.Drive(BallSearchCalculations.OutAngle, 0, LowSpeed);
}

void TakticsC::GoalAttak(){
    Cam.
    GoalCalculations.calculateAngle();
    Robot.Drive(-BNO055.TiltZ, 0, LowSpeed);
}



