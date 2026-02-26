#include "Game_Thread.h"
#include "RGB.h"
#include "Mouse.h"
#include "Taktics.h"
#include "Debug.h"
GameC Game;

elapsedMillis Line_Timer;

void GameC::Run(){
    if ((Line.Summe > 0) )  {        
        if((Goal.inSight == true) && (LDR.Aktiv() == true)){
            Robot.Kicker.On(800);
        }
        else{
            Robot.Kicker.Off();
        }

        dead_diff = dead_zone-LineCalc.DriveAngle;

        U.Circel(dead_diff); // maping to circel

        if((dead_diff < -140)&&(once == false)){ // linien überquerung
            outside = true;
            once = true;
            temp3_Angle = LineCalc.DriveAngle;
            Line_Timer=0;
        }

        if (Line_Follow == true){
            if(outside == true){
                Robot.Drive(round((LineCalc.DriveAngle-180)/Front_Speed),0,50);
                Serial.println("test");
            }
            else{Robot.Drive(LineCalc.DriveAngle,0,20);}
        }
        else{
            if((outside == true) && (Line.Summe < 5)){
                Robot.Drive(((temp3_Angle-180)*-1),0,100);
            }
            else{
                Robot.Drive(LineCalc.DriveAngle,0,100);
            }
        }

        //Robot.Drive(LineCalc.DriveAngle,0,50);       
        
    }
    else if ((Line.VW_Summe > 0))  {  // VW only
        dead_zone = LineCalc.DriveAngle;
        U.Circel(dead_zone);

        if (Line_Follow == true){
            Robot.Break();
        }
        else{
            Robot.Drive(LineCalc.DriveAngle,0,100);
            
        }
    }
    else { 
        Line_Timer = 0;
        Robot.Kicker.Off();
        /*if(BL.Rolle == "A"){
            AttackerTactics.step();
        }
        else if(BL.Rolle == "D"){
            DefenderTactics.step();
        }
        else if(BL.Rolle == "N"){
            Elem_Taktics.Ballsearch();
        }*/
        //Robot.Drive(0,0,MainSpeed);
        Taktics.step();
        once = false;
        outside = false;
    }
}



