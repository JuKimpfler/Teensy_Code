#include "Elementar.h"
#include "System.h"

void setup() {
    System.init.Sensors();
    System.init.Motors();
    System.init.Interface();
}

void loop() { 
    Cycle_Timer = 0 ;

    System.Update.Sensors();
    System.Update.Interface();
    System.Update.Calculations();
 
    if(System.Start){
        Game.Run();
        //for ( int i = 0 ; i<7 ; i++){
        //    Serial.print(Expander.ADC.IR[i]);
        //    Serial.print(" , ");
        //}
        //Serial.println(Expander.ADC.IR[7]);
    }
    else{
        Robot.Stop();
    }

    if(System.Button[0]){
        BNO055.Calibrate();
    }   
    
    Cycletime = Cycle_Timer;
    //Serial.println(Cycletime);
}

