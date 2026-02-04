#include "US.h"

elapsedMillis US_Timer1 ;
USC US;

void USC::init(){
    US_Timer1 = 0;
}

void USC::read(){
    if(US_Timer1 > (1000/US_Frequency)){
        int reading;

        Wire1.beginTransmission(US_Left);
        Wire1.write(byte(0x02));  
        Wire1.endTransmission();   
        Wire1.requestFrom(US_Left, 2);   
        if (2 <= Wire1.available()) { 
        reading = Wire1.read(); 
        reading = reading << 8;    
        reading |= Wire1.read(); 
        Distance_raw[0]=reading;
        }

        // US Sensoren Anfragen
        Wire1.beginTransmission(US_Left); 
        Wire1.write(byte(0x00));      
        Wire1.write(byte(0x51));  
        Wire1.endTransmission();   

        Wire1.beginTransmission(US_Right);
        Wire1.write(byte(0x02));  
        Wire1.endTransmission();   
        Wire1.requestFrom(US_Right, 2);   
        if (2 <= Wire1.available()) { 
        reading = Wire1.read(); 
        reading = reading << 8;    
        reading |= Wire1.read(); 
        Distance_raw[1]=reading;
        }

        // US Sensoren Anfragen 
        Wire1.beginTransmission(US_Right); 
        Wire1.write(byte(0x00));  
        Wire1.write(byte(0x51));    
        Wire1.endTransmission();  

        US_Timer1 = 0;
    } 

    if (Distance_raw[0]<Distance_raw[1]){
        Distance = -Distance_raw[0];
    }
    else {
        Distance = Distance_raw[1];
    }
}



