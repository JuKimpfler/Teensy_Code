#include "Defender.h"
#include "Taktics.h"

DefenderTacticsC DefenderTactics;

void DefenderTacticsC::step(){
    if (LDR.Aktiv()){
        Robot.Kicker.On();
        GoalTurn();
    }
    else{
        Robot.Kicker.Off();
        Homing();
    }
}

void DefenderTacticsC::Homing(){

}

void DefenderTacticsC::GoalTurn(){
    Robot.Turn(Goal.Angle,30);
}



