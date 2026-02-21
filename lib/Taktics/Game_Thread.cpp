#include "Game_Thread.h"
#include "RGB.h"
#include "Mouse.h"
GameC Game;

void GameC::Run(){
    if ((LineCalc.Distance != 0 || Line.Summe_VW != 0)){ // Line Avoidance System (Linie auf irgendeinem VW oder Basic Sensor erkannt ?)
            if (Mouse.delta_dist > 0.2){
                Robot.Break();
            }
            else{
                Robot.Drive(LineCalc.DriveAngle,0,100);
            }
    }
    else{
        //if (LDR.Aktiv()){
            //vlt attacker / defender
            Taktics.BallSearch();
        //}
        //else{
            //Taktics.GoalAttak(); 
        //}
    }
   //Robot.Kicker.Once();
}



