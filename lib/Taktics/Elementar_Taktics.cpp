#include "Elementar_Taktics.h"
#include "Defender.h"
TakticsC Taktics;

void TakticsC::BallSearch(){
    if(Ball.inSight){
		if((Goal_Turn == true) && (U.Ran(Ball.Angle,-25,25)&&(Ball.Distance<30))){
			Robot.Drive(BallCalc.DriveAngle,999,MainSpeed);
		}
		else{
			Robot.Drive(BallCalc.DriveAngle,0,MainSpeed);
		} 
	}
	else{
		DefenderTactics.Homing();
	}
}

void TakticsC::GoalAttak(){
    if((Goal_Turn == true) && (U.Ran(Ball.Angle,-25,25)&&(Ball.Distance<30))){
        Robot.Drive(Goal.Angle, 999, MainSpeed); 
    }
    else{
        Robot.Drive(Goal.Angle, 0, MainSpeed); 
    }
}

void TakticsC::step(){
    if (LDR.Aktiv() == true){
        GoalAttak();
        Robot.Kicker.On();
    }
    else{
        BallSearch();
    } 
}



