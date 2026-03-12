#include "Elementar_Taktics.h"
#include "Defender.h"
TakticsC Taktics;

void TakticsC::BallSearch(){
    if(Goal_Turn){
        Robot.Drive(BallCalc.DriveAngle,0,MainSpeed);
    }
    else{
        Robot.Drive(BallCalc.DriveAngle,999,MainSpeed);
    } 
}

void TakticsC::GoalAttak(){
    if(Goal_Turn){
        Robot.Drive(Goal.Angle, 999, MainSpeed); 
    }
    else{
        Robot.Drive(0, 0, MainSpeed); 
    }
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
            DefenderTactics.Homing();
        }
    } 
}



