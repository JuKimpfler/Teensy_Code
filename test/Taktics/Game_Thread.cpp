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
            Taktics.BallSearch();
        }
        else{
            Taktics.GoalAttak();
        }
    //}

    //Robot.Drive(-BallSearchCalculations.OutAngle,0,30);
    //Robot.Turn(0);

}



