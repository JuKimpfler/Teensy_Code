#include "Defender.h"
#include "US.h"

DefenderTacticsC DefenderTactics;

void DefenderTacticsC:: step(){
    bool ballcaught = true;
    if (ballcaught == true){
            Goaldirection();
    }
    else{
        Ballsearch();


        //if ( US.Distance_raw[0] > (91-(Ball.Distance*sin(Ball.Angle)))) {

          //   Robot.Drive(90, Ball.Angle, 20); 
      
       // } else {


        

           


    }
    
}



