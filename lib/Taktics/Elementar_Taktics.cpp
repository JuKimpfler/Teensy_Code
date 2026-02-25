#include "Elementar_Taktics.h"
TakticsC Taktics;

void TakticsC::BallSearch(){
    Robot.Drive(Ball.Angle,0,MainSpeed);
}

void TakticsC::GoalAttak(){
    Robot.Drive(Goal.Angle, 0, MainSpeed); 
}

void TakticsC::step(){
    if (LDR.Aktiv() == true){
        GoalAttak();
    }
    else{
        BallSearch();
    } 
}



