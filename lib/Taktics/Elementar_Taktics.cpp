#include "Elementar_Taktics.h"
TakticsC Taktics;

void TakticsC::BallSearch(){
    Robot.Drive(BallCalc.DriveAngle,0,MainSpeed);
}

void TakticsC::GoalAttak(){
    Robot.Drive(0, Goal.Angle, MainSpeed); 
}

void TakticsC::step(){
    if (LDR.Aktiv() == true){
        GoalAttak();
        Robot.Kicker.On();
    }
    else{
        BallSearch();
        Robot.Kicker.Off();
    } 
}



