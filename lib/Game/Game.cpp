#include "Game.h"
GameC Game;

elapsedMillis Line_Timer;

void GameC::Run(){
    if(Ball.Distance<30){ESC.set(10);}
    else{ESC.stop();}

    if(!Game.LineInterrupt()){
        if(LDR.Aktiv()){
            if(Cam.isValid()){Robot.Drive(Cam.give_Angle()*1.3,Cam.give_Angle(),20);}
            else{
                Robot.Drive(180,0,20);
            }
        }
        else{
            int Distance2 = Ball.Distance-0;
            if(Distance2 < 0){Distance2 = 0;}
            int drive = U.Circel(((LUT.get_DriveAngle(U.Circel(Ball.Angle),Distance2))));
            if(abs(Ball.Angle) < 10){
                drive = Ball.Angle;
            }
            float turn=0;
            if(Cam.isValid() && abs(Cam.give_Angle()) < 60){turn = Cam.give_Angle();}
            else{turn = 0;}
            Robot.Drive(drive,turn,15);
        }
    }
    //if(Cam.isValid()){Robot.Turn(Cam.give_Angle());}
    //else{Robot.Turn(0);}
    //Robot.Turn(0);
    //Robot.Drive(45,0,10);
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



