#include "Attacker.h"

AttackerTacticsC AttackerTactics;

void AttackerTacticsC::step(){
	if(LDR.Aktiv()){
		GoalAttak();
	}
	else{
		BallSearch();
	}
}

void AttackerTacticsC::GoalAttak(){
	if (abs(Goal.Angle) < 10){
		Robot.Kicker.On();
		Robot.Drive(0, 0, MainSpeed);
	}
	else{
		Robot.Kicker.Off();
		Robot.Drive(Goal.Angle, Goal.Angle, MainSpeed); 
	}
}

void AttackerTacticsC::BallSearch(){
	Robot.Drive(BallCalc.DriveAngle,0,MainSpeed);
}

void AttackerTacticsC::EckEscape(){
	
}


