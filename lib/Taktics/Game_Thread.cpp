#include "Game_Thread.h"
#include "RGB.h"
GameC Game;

void GameC::Run(){/*
    if (LineCalc.Distance != 0 || Line.Summe_VW != 0){ // Line Avoidance System (Linie auf irgendeinem VW oder Basic Sensor erkannt ?)
        Robot.Drive(LineCalc.DriveAngle,0,100);
        //Rand_Dir = random(16)*22.5; 
        //Serial.println(LineCalc.Distance);
    }
    else{
        //if (LDR.Aktiv()){
            //vlt attacker / defender
            AttackerTactics.step();
        //}
        //else{
            //Taktics.GoalAttak();
        //}
    }*/
   Robot.Kicker.Once();
}



