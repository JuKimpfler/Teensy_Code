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
    if (US.Distance_raw[0] < US.Distance_raw[2] + 10){
        Robot.Drive(155, 0, MainSpeed);
    }
}

void DefenderTacticsC::GoalTurn(){
    Robot.Turn(Goal.Angle,30);
}



