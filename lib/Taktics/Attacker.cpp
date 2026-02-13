#include "Attacker.h"

AttackerTacticsC AttackerTactics;

void AttackerTacticsC::step(){
	if(LDR.Aktiv()){
		Elem_Taktics.BallCaught();
	}
	else{
		Elem_Taktics.Ballsearch();
	}
}

void AttackerTacticsC::BallCaught(){
	if(US.Distance > -180 && US.Distance < 0){
		Robot.Drive(20,10,30);
	}
	else if (US.Distance < 180 && US.Distance > 0){
		Robot.Drive(-20,-10,30);
	}
	else{
		Robot.Drive(0,0,30);
	}
}

void AttackerTacticsC::Ballsearch(){
    Robot.Drive(-BallCalc.DriveAngle,0,30);
}
