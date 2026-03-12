#include "Attacker.h"
#include "Defender.h"
AttackerTacticsC AttackerTactics;

void AttackerTacticsC::step(){
	if(LDR.Aktiv()){
		GoalAttak();
		Robot.Kicker.On();
	}
	else{
		BallSearch();
	}
}

void AttackerTacticsC::GoalAttak(){
	if((Goal_Turn == true) && (U.Ran(Ball.Angle,-25,25)&&(Ball.Distance<30))){
        Robot.Drive(Goal.Angle,999,MainSpeed);
    }
    else{
        Robot.Drive(Goal.Angle,0,MainSpeed);
    } 
}

void AttackerTacticsC::BallSearch(){
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

void AttackerTacticsC::EckEscape(){
	
}


