#include "Game_Thread.h"
#include "RGB.h"
#include "LineCalculations.h"
#include "Cam.h"
#include "BL.h"
GameC Game;

void GameC::Run(){
    if (LineCalc.Distance != 0 || Line.Summe_VW != 0){ // Line Avoidance System (Linie auf irgendeinem VW oder Basic Sensor erkannt ?)
        Robot.Drive(LineCalc.DriveAngle,0,100);
    }
    else{
        if(BL.Rolle == "A"){
            AttackerTactics.step();
        }
        else if(BL.Rolle == "D"){
            DefenderTactics.step();
        }
    }
}



