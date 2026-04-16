#include "Game.h"
GameC Game;

elapsedMillis Line_Timer;

void GameC::Run(){
    if(BL.Rolle == "A"){
        // Attaker
    }
    else if(BL.Rolle == "D"){
        // Defender 
    }
    else if(BL.Rolle == "N"){
        // Neutral Game
    }
}

void GameC::Stop(){
    Robot.Stop();
}

bool GameC::LineInterrupt(){
    if ((Line.Summe > 0) )  {        

        dead_diff = dead_zone-LineCalc.DriveAngle;
        U.Circel(dead_diff);

        if(U.Ran(abs(dead_diff),135,225)){
            Robot.Drive(((LineCalc.DriveAngle-180)*-1),0,100);
        }
        else{
            Robot.Drive(LineCalc.DriveAngle,0,100);
        }

        return true;
    }
    else if ((Line.VW_Summe > 0))  {

        dead_zone = LineCalc.DriveAngle;
        U.Circel(dead_zone);
        Robot.Drive(LineCalc.DriveAngle,0,100);

        return true;
    }
    else {return false;}
}



