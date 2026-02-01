#include "Defender.h"

DefenderTacticsC DefenderTactics;

void DefenderTacticsC::step(){
    if (LDR.Aktiv() == true){
        Elem_Taktics.BallCaught();
    }
    else{
        Defens();
    } 
}

void DefenderTacticsC::Defens(){
    // Do homing and defend the goal
}


