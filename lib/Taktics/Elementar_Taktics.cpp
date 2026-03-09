#include "Elementar_Taktics.h"
TakticsC Taktics;

void TakticsC::BallSearch(){
    Robot.Drive(BallCalc.DriveAngle,0,MainSpeed);
}

void TakticsC::GoalAttak(){
    Robot.Drive(Goal.Angle, 999, MainSpeed); 
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
            if (US.Distance_raw[0] < US.Distance_raw[2] + 10){
                Robot.Drive(-155, 0, MainSpeed);
            }

            else if(US.Distance_raw[2] < US.Distance_raw[0] + 10){
                Robot.Drive(155, 0, MainSpeed);
            }
            else{
                Robot.Drive(180,0,MainSpeed);
            }

            //Defender Programm Felix?
        }
        Robot.Kicker.Off();
    } 
}



