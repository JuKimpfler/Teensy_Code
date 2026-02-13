#include "Defender.h"

DefenderTacticsC DefenderTactics;

void DefenderTacticsC::step(){
    if (LDR.Aktiv() == true){
        Elem_Taktics.BallCaught();
    }
    else{
        Defence();
    } 
}

void DefenderTacticsC::Defence(){
    // Do homing and defend the goal
    
}


