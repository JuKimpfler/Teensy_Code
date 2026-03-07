#include "Defender.h"
#include "Taktics.h"

DefenderTacticsC DefenderTactics;

void DefenderTacticsC::step(){
    if (LDR.Aktiv()){
        Robot.Kicker.On();
        GoalTurn();
    }
    else{
        Robot.Kicker.Off();
        Homing();
    }
}

void DefenderTacticsC::Homing(){
    if (US.Distance_raw[0] < US.Distance_raw[2] + 10){
        Robot.Drive(155, 0, MainSpeed);
    }

    else if(US.Distance_raw[2] < US.Distance_raw[0] + 10){
        Robot.Drive(-155, 0, MainSpeed);
    }
    else{
        Robot.Drive(180,0,MainSpeed);
    }

  
    if (US.Distance_raw[1] < 45) { //wenn Abstand hinten kleiner 45

        //Defender Programm Felix

        if (Ball.Angle > 0) { //ist Ball links?

            if (US.Distance_Raw[0] > (-0.84*Ball.Angle+91.1)) { //ist Abstand nach links größer als er sein sollte?
     
               Robot.Drive(90, 0, 30)  //fahr nach links

            } else { 

          
             if (US.Distance_Raw[0] < (-0.84*Ball.Angle+91.1)) { //ist Abstand nach links kleiner als er sein sollte?
       
               Robot.Drive(-90, 0, 30)  //fahr nach rechts
      
            }
    
        }


        if (Ball.Angle < 0) { //ist Ball rechts?

           if (US.Distance_Raw[2] > (0.84*Ball.Angle+91.1)) { //ist Abstand nach rechts größer als er sein sollte?
      
              Robot.Drive(-90, 0, 30)  //fahr nach rechts

            } else { 

          
           if (US.Distance_Raw[2] < (0.84*Ball.Angle+91.1)) { //ist Abstand nach rechts kleiner als er sein sollte?
       
             Robot.Drive(90, 0, 30) //fahr nach links

           }
      
        }  


    }
}

void DefenderTacticsC::GoalTurn(){
    Robot.Turn(Goal.Angle,30);
}



