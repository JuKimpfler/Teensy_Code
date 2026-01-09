#include "Attacker.h"

AttackerTacticsC AttackerTactics;

void AttackerTacticsC::step(){
	bool ballCaught = true;
	if(ballCaught){
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
		//vielleicht halb nach Tor ausgerichtet fahren
		Robot.Drive(goalAngle.toDeg(), goalAngle.toDeg() /2, HighSpeed);

	}
}

void AttackerTacticsC::BallSearch(){
	Taktics.BallSearch();
	//vlt nur auf ball gehen falls er vor dem attacker ist sodass der defender den ball vorschießen kann?
	
}

