#include "Attacker.h"

AttackerTacticsC AttackerTactics;

void AttackerTacticsC::BallCaught(){
	//TODO: include BallCaught for attacker
}

void AttackerTacticsC::BallSearch(){
	//TODO: get ir values
	int RelativeBalDir = 0;
	float BallDir = RelativeBalDir - BNO055.give_TiltZ();
	float BallDist = 0;
	//TODO: move motor towards RelativeBallDir;
	if(Utils.isinRange(BallDir, -60, 60)){
		Robot.Drive(BallDir);
	}
	else{
		//Move To Ball
	}



	
}

void AttackerTacticsC::step(){
	
}

