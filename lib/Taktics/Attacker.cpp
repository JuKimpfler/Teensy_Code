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
	if(Goal_Turn){
        Robot.Drive(Goal.Angle,0,MainSpeed);
    }
    else{
        Robot.Drive(Goal.Angle,999,MainSpeed);
    } 
}

void AttackerTacticsC::BallSearch(){
	if(Ball.inSight){
		Robot.Drive(BallCalc.DriveAngle,0,MainSpeed);
	}
	else{
		DefenderTactics.Homing();
	}
}

void AttackerTacticsC::EckEscape(){
	
}


