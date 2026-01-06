#include "Game_Thread.h"
#include "RGB.h"
#include "LineCalculations.h"
GameC Game;

void GameC::Run(){
    if (Line.Summe != 0 || Line.Summe_VW != 0){ // Line Avoidance System (Linie auf irgendeinem VW oder Basic Sensor erkannt ?)
        //Robot.Drive(LineCalc.DriveAngle,0,100);
        //Robot.Drive_Smoothed_set(LineCalc.RawAngle,0,0,MainSpeed);
        //Robot.Drive_Smoothed_set(LineCalc.RawAngle,0,0,MainSpeed);
        //Robot.Drive(LineCalc.RawAngle,0,1); 
        Robot.Break();
        
        //Robot.Stop();
    }
    else{
        Robot.Drive_Smoothed_Kill();
        Robot.Drive(0,0,MainSpeed);
        //if (LDR.Aktiv()){
            //vlt attacker / defender
            //Taktics.BallSearch();
        //}
        //else{
            //Taktics.GoalAttak();
        //}
    //    Robot.Stop();
    }
}



