#include "Attacker.h"

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
	if(Goal_Turn){
        Robot.Drive(BallCalc.DriveAngle,0,MainSpeed);
    }
    else{
        Robot.Drive(BallCalc.DriveAngle,999,MainSpeed);
    } 
}

void AttackerTacticsC::BallSearch(){
	if(Ball.inSight){
		Robot.Drive(BallCalc.DriveAngle,0,MainSpeed);
	}
	else{
		Robot.Drive(0, 0, MainSpeed);
	}
}

void AttackerTacticsC::EckEscape(){
	
}


