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
	//TODO: include BallCaught for attacker
	//Robot.Turn (0);
}

void AttackerTacticsC::Ballsearch(){
	//TODO: get ir values
	//int IrValues[8];//Insert values
	//Angle RelativeBalDir = IrBall.getWeightedIr(IrValues);
	//double BallDir = RelativeBalDir.theta - BNO055.give_TiltZ();
	//TODO: move motor towards RelativeBallDir;
		
}
