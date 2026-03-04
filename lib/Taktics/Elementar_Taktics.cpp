#include "Elementar_Taktics.h"
TakticsC Taktics;

void TakticsC::BallSearch(){
    Robot.Drive(BallCalc.DriveAngle,0,MainSpeed);
}

void TakticsC::GoalAttak(){
    Robot.Drive(Goal.Angle, Goal.Angle, MainSpeed); 
}

void TakticsC::step(){
    if (LDR.Aktiv() == true){
        GoalAttak();
        Robot.Kicker.On();
    }
    else{
        if (Ball.inSight){
            BallSearch();
        }
        else{
            Robot.Drive(180, 0 , 35);
        }
        Robot.Kicker.Off();
    } 
}



