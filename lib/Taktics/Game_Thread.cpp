#include "Game_Thread.h"
#include "RGB.h"
#include "LineCalculations.h"
#include "Cam.h"
GameC Game;

void GameC::Run(){
    //Serial.print("Distance: "); Serial.println(LineCalc.Distance);
    //Serial.print("VW: "); Serial.println(Line.Summe_VW);
    if (LineCalc.Distance != 0 || Line.Summe_VW != 0){ // Line Avoidance System (Linie auf irgendeinem VW oder Basic Sensor erkannt ?)
        Robot.Drive(LineCalc.DriveAngle,0,100);
        //Rand_Dir = random(16)*22.5; 
        //Serial.println(LineCalc.Distance);
    }
    else{
        //if (LDR.Aktiv()){
            //vlt attacker / defender
            //Cam.Decode();
        //}
        //else{
            //Taktics.GoalAttak(); 
        //}
    }
   //Robot.Kicker.Once();
}



