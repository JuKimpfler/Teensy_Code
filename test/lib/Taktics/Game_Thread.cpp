#include "Game_Thread.h"
#include "RGB.h"
#include "LineCalculations.h"
#include "Cam.h"
#include "BL.h"
GameC Game;

void GameC::Run(){
    if ((LineCalc.Distance != 0 || Line.Summe_VW != 0)){ // Line Avoidance System (Linie auf irgendeinem VW oder Basic Sensor erkannt ?)
        if (Mouse.delta_dist > 0.1){
            Robot.Break();
        }
        else{
            Robot.Drive(LineCalc.DriveAngle,0,30);
        }
    }
    else{
        /*if(BL.Rolle == "A"){
            AttackerTactics.step();
        }
        else if(BL.Rolle == "D"){
            DefenderTactics.step();
        }
        else if(BL.Rolle == "N"){
            //Elem_Taktics.step();
            Elem_Taktics.Ballsearch();
        }*/
       Robot.Turn(0);
    }
}



