#include "Elementar_Taktics.h"
Elem_TakticsC Elem_Taktics;

void Elem_TakticsC::Ballsearch(){
    Robot.Drive(-BallCalc.DriveAngle,0,30);
}

void Elem_TakticsC::BallCaught(){
    Robot.Drive(0, 0, MainSpeed);
}



