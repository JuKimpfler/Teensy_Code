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
    if ((US.Distance_raw[0] < 40)){
        Robot.Drive(0,0,40);
    }  
    else{
        if ((Goal.Angle > 3)){
            Robot.Drive(-90, 0, MainSpeed);
        }
        else if((Goal.Angle < -3)){
            Robot.Drive(90, 0, MainSpeed);
        }
        else if ((US.Distance_raw[0] > 50)){
            Robot.Drive(-180,0,MainSpeed);
        }
        else{
            Robot.Turn(0);
        }
    }
    #endif
    #ifdef Robo_w
    if ((US.Distance_raw[1] < 40)){
        Robot.Drive(0,0);
    }  
    else{
        if ((Goal.Angle > 3)){
            Robot.Drive(-90, 0, MainSpeed);
        }
        else if((Goal.Angle < -3)){
            Robot.Drive(90, 0, MainSpeed);
        }
        else if ((US.Distance_raw[1] > 50)){
            Robot.Drive(-180,0,MainSpeed);
        }
        else{
            Robot.Turn(0);
        }
    }  
    #endif
}

<<<<<<< HEAD
=======
void DefenderTacticsC::Homing_Test(){
    #ifdef Robot_s
    if(US.Distance_raw[0] < 60 && US.Distance_raw[0] > 40){
        if(abs(Ball.Angle) > 10){
            Robot.Drive(Ball.Distance * cosf(Ball.Angle), 0, MainSpeed);
        }
        else{
            Robot.Turn(Ball.Angle);
        }
    }
    else{
        if (US.Distance_raw[0] < US.Distance_raw[2] + 10){
            Robot.Drive(110, 0, MainSpeed/3);
        }
        else if(US.Distance_raw[2] < US.Distance_raw[0] + 10){
            Robot.Drive(-110, 0, MainSpeed/3);
        }
        else if(US.Distance_raw[1] > 60){
            Robot.Drive(180,0,MainSpeed);
        }
        else if(US.Distance_raw[1] < 40){
            Robot.Drive(0,0,MainSpeed);
        }
        else{
            Robot.Turn(Ball.Angle);
        }
    }
    #endif
    #ifdef Robo_w
    if(US.Distance_raw[1] < 60 && US.Distance_raw[1] > 40){
        if(abs(Ball.Angle) > 10){
            Robot.Drive(Ball.Distance * cosf(Ball.Angle), 0, MainSpeed);
        }
        else{
            Robot.Turn(Ball.Angle);
        }
    }
    else{
        if (US.Distance_raw[0] < US.Distance_raw[2] + 10){
            Robot.Drive(110, 0, MainSpeed/3);
        }
        else if(US.Distance_raw[2] < US.Distance_raw[0] + 10){
            Robot.Drive(-110, 0, MainSpeed/3);
        }
        else if(US.Distance_raw[1] > 60){
            Robot.Drive(180,0,MainSpeed);
        }
        else if(US.Distance_raw[1] < 40){
            Robot.Drive(0,0,MainSpeed);
        }
        else{
            Robot.Turn(Ball.Angle);
        }
    }
    #endif
}
>>>>>>> 820ccfe06c50c9c550e2a8db90330c65b9e36d9a

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



