#include "Attacker.h"

AttackerTacticsC AttackerTactics;

void AttackerTacticsC::step(){
	if(/*LDR.Aktiv()*/false){
		BallCaught();
	}
	else{
		BallSearch();
	}
}

void AttackerTacticsC::BallCaught(){
	Angle goalAngle = GoalCalculations.calculateAngle(Vec2(Cam.x, Cam.y));
	if (abs(goalAngle.toDeg()) < 10){
		Robot.Kicker.Once();
		Robot.Drive(0, 0, HighSpeed);
	}
	else{
		//get us daten
		if(false) {
			Robot.Drive(-115, 0, HighSpeed);
		}
		else{
			//vielleicht halb nach Tor ausgerichtet fahren
			Robot.Drive(goalAngle.toDeg(), goalAngle.toDeg() /2, HighSpeed);
		}
	}
}

void AttackerTacticsC::BallSearch(){
	Taktics.BallSearch();

	//vlt für später nur auf ball gehen falls er vor dem attacker ist sodass der defender den ball vorschießen kann?
	
}

