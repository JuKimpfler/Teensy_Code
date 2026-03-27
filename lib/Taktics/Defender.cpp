#include "Defender.h"
#include "US.h"


DefenderTacticsC DefenderTactics;

void DefenderTacticsC::step(){
    if (LDR.Aktiv()){
        Robot.Kicker.On();
        GoalTurn();
    }
    else{
        Homing();
    }
}

void DefenderTacticsC::Homing(){
    #ifdef Robo_s
    float diff = US.Distance_raw[1] - US.Distance_raw[2];

    if ((US.Distance_raw[0] < 20)){
        Robot.Drive(0,0);
    }  
    else{
        if ((diff < -20)){
            Robot.Drive(-90, 0, MainSpeed/2);
        }
        else if((diff > 20)){
            Robot.Drive(90, 0, MainSpeed/2);
        }
        else{
            if ((US.Distance_raw[0] < 20)){
                Robot.Drive(0,0);
            }

            if ((US.Distance_raw[0] > 40)){
                Robot.Drive(-180,0);
            }
        }
    }
    #endif
    #ifdef Robo_w
    float diff = US.Distance_raw[2] - US.Distance_raw[0];

    if ((US.Distance_raw[1] < 20)){
        Robot.Drive(0,0);
    }  
    else{
        if ((diff < -20)){
            Robot.Drive(-90, 0, MainSpeed/2);
        }
        else if((diff > 20)){
            Robot.Drive(90, 0, MainSpeed/2);
        }
        else{
            if ((US.Distance_raw[1] < 20)){
                Robot.Drive(0,0);
            }

            if ((US.Distance_raw[1] > 40)){
                Robot.Drive(-180,0);
            }
        }
    }  
    #endif
}


void DefenderTacticsC::Goal_Defens(){
    if (US.Distance_raw[1] < 45) { //wenn Abstand hinten kleiner 45

        //Defender Programm Felix

        if (Ball.Angle > 0) { //ist Ball links?

            if (US.Distance_raw[0] > (-0.84*Ball.Angle+91.1)) { //ist Abstand nach links größer als er sein sollte?
     
               Robot.Drive(90, 0, MainSpeed);  //fahr nach links

            }

          
            if (US.Distance_raw[0] < (-0.84*Ball.Angle+91.1)) { //ist Abstand nach links kleiner als er sein sollte?
       
               Robot.Drive(-90, 0, MainSpeed);  //fahr nach rechts
      
            }
    
        }


        if (Ball.Angle < 0) { //ist Ball rechts?

           if (US.Distance_raw[2] > (0.84*Ball.Angle+91.1)) { //ist Abstand nach rechts größer als er sein sollte?
      
              Robot.Drive(-90, 0, MainSpeed);  //fahr nach rechts

            } 

          
           if (US.Distance_raw[2] < (0.84*Ball.Angle+91.1)) { //ist Abstand nach rechts kleiner als er sein sollte?
       
             Robot.Drive(90, 0, MainSpeed); //fahr nach links

           }
      
        }  

    }
}

void DefenderTacticsC::GoalTurn(){
    Robot.Turn(999);
}



