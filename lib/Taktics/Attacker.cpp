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
	//TODO: include BallCaught for attacker
	Robot.Turn (0);
	Robot.Kicker.Once(); //ist nur ein kleiner Programmierversuch ;) ~Felix
    
}

void AttackerTacticsC::BallSearch(){
	//TODO: get ir values
	//int IrValues[8];//Insert values
	//Angle RelativeBalDir = IrBall.getWeightedIr(IrValues);
	//float BallDir = RelativeBalDir.theta - BNO055.give_TiltZ();
	//TODO: move motor towards RelativeBallDir;
	
}
