#include "Game_Thread.h"
#include "RGB.h"
GameC Game;

void GameC::Run(){
    //if (LineCalc.Distance != 0){ // Line Avoidance System
        //Robot.Drive(LineCalc.DriveAngle,0,HighSpeed);
        //Rand_Dir = random(16)*22.5;
    //}
    //else{
        //Robot.Drive(Rand_Dir,0,20);
        //Robot.Turn(0);
        if (LDR.Aktiv()){
            //vlt attacker / defender
            Taktics.BallSearch();
        }
        else{
            Taktics.GoalAttak();
        }
    //}
    //Robot.Turn(0);

}



