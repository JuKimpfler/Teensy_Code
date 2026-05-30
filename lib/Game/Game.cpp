#include "Game.h"
#include "Defender_Tim.h"
GameC Game;

Defender_Tim T_Defender;

elapsedMillis Line_Timer;

void GameC::Run(){
    /*if(Ball.Distance<70){ESC.set(30);}
    else{ESC.stop();}*/
    ESC.set(30);

    if(BL.Rolle == "D"){
        bool LineDef[32];
        bool LineDef_invers[32];
        for(int i = 8 ; i<32 ; i++){if(Line.line[i]==1){LineDef[i-8] = true;}else{LineDef[i-8] = false;}}
        for(int i = 0 ; i<8  ; i++){if(Line.line[i]==1){LineDef[i+24] = true;}else{LineDef[i+24] = false;}}
        for(int i = 0 ; i<32 ; i++){LineDef_invers[i]=LineDef[31-i];}

        Drive_Data Data = T_Defender.follow_Line(LineDef_invers,T_Defender.Jto12(U.Circel(Ball.Angle-180)*-1),Cam.give_BlobH2(),Cam.isValid2(),Cam.give_Angle2(),T_Defender.Jto12(LineCalc.DriveAngle),Line.dep,(Line.Summe!=0),Ball.Distance+10);
        Data.direction = T_Defender.C12toJ(Data.direction);

        Robot.Drive(Data.direction,Data.turn,Data.speed);
    }
    else if(BL.Rolle=="A" || BL.Rolle =="N"){
        if(Game.LineInterrupt()==false){
            /*if(Ball.catched){
                if(Cam.isValid1()&&abs(Cam.give_Angle_Cam1())<20){Robot.Kicker.On();}
                if(Cam.isValid1()){Robot.Drive(Cam.give_Angle1()*1.3,Cam.give_Angle1()*-1,25);}
                else{
                    Robot.Drive(180,0,10);
                    ESC.set(20);
                    Robot.Kicker.Off();
                }
            }
            else{
                Robot.Kicker.Off();
                int Distance2 = Ball.Distance-0;
                if(Distance2 < 0){Distance2 = 0;}
                int drive = U.Circel(((LUT.get_DriveAngle(U.Circel(Ball.Angle),Distance2))));
                if(abs(Ball.Angle) < 10){
                    drive = Ball.Angle;
                }
                float turn=0;
                if(Cam.isValid1() && abs(Ball.Angle) < 40){turn = Ball.Angle*-1;}
                else{turn = 0;}
                Robot.Drive(drive,turn,20);
            }*/
           Robot.Drive(0,0,100);
        }
    }
}

void GameC::Stop(){
    Robot.Stop();
}

bool GameC::LineInterrupt(){
    if (Line.Summe > 0)  {   
        //if(false && Cam.isValid1() && abs(Cam.give_Angle1()) < 15 && (Cam.give_BlobH1()>Cam.give_BlobH2())){
        //    Robot.Drive(180,0,100);
       //}
        //else if(false &&  Cam.isValid2() && abs(Cam.give_Angle2()) < 15 && (Cam.give_BlobH2()>Cam.give_BlobH1())){
        //    Robot.Drive(0,0,100);
        //}
        //else{
            //dead_diff = dead_zone-LineCalc.DriveAngle;
            //U.Circel(dead_diff);
            //if(U.Ran(abs(dead_diff),135,225)){
            //    Robot.Drive(LineCalc.RawAngle,0,100);
            //}
            //else{
                Robot.Drive(LineCalc.DriveAngle,0,100);
            //}
        //}
        return true;
    }
    //else if ((Line.VW_Summe > 0))  {

        //dead_zone = LineCalc.DriveAngle;
        //U.Circel(dead_zone);
    //    Robot.Drive(LineCalc.DriveAngle,0,100);

    //    return true;
    //}
    else {return false;}
}



