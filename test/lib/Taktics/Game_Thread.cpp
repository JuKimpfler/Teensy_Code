#include "Game_Thread.h"
#include "RGB.h"
#include "LineCalculations.h"
#include "Cam.h"
#include "BL.h"
#include "Debug.h"
GameC Game;

//elapsedMillis Line_Timer;

void GameC::Run(){
    if ((Line.Summe != 0 || Line.VW_Summe != 0)){ // Line Avoidance System (Linie auf irgendeinem VW oder Basic Sensor erkannt ?)
        if(Goal.inSight == true && LDR.Aktiv() == true){
            Robot.Kicker.On(800);
        }
        else{
            Robot.Kicker.Off();
        }
        Robot.Kicker.On();
        if (Line_Timer < 200){
            Robot.Break();
            Debug.Plot("Mess",0);

        }
        else{
            Debug.Plot("Mess",0);
            Robot.Drive(LineCalc.DriveAngle,0,100);
        }
        Debug.Plot("s",0);
    }
    else{
        Robot.Kicker.Off();
        Line_Timer = 0;
        Debug.Plot("Mess",Line_Timer);
        /*if(BL.Rolle == "A"){
            AttackerTactics.step();
        }
        else if(BL.Rolle == "D"){
            DefenderTactics.step();
        }
        else if(BL.Rolle == "N"){
            Elem_Taktics.Ballsearch();
        }*/
       //Robot.Turn(0);
        Elem_Taktics.step();
    }
}



