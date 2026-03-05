#include "US.h"
#include "RGB.h"

elapsedMillis US_Timer1 ;
USC US;

void USC::read(){
    #ifdef Robo_w
    if(US_Timer1 > (1000/US_Frequency)){
        Wire1.setClock(I2C_SPEED_US);
        int reading;

        Wire1.beginTransmission(US_Leftw);
        Wire1.write(byte(0x02));  
        Wire1.endTransmission();   
        Wire1.requestFrom(US_Leftw, 2);   
        if (2 <= Wire1.available()) { 
        reading = Wire1.read(); 
        reading = reading << 8;    
        reading |= Wire1.read(); 
        Distance_raw[0]=reading;}
        Wire1.beginTransmission(US_Leftw); 
        Wire1.write(byte(0x00));      
        Wire1.write(byte(0x51));  
        Wire1.endTransmission();   

        Wire1.beginTransmission(US_Backw);
        Wire1.write(byte(0x02));  
        Wire1.endTransmission();   
        Wire1.requestFrom(US_Backw, 2);   
        if (2 <= Wire1.available()) { 
        reading = Wire1.read(); 
        reading = reading << 8;    
        reading |= Wire1.read(); 
        Distance_raw[1]=reading;}
        Wire1.beginTransmission(US_Backw); 
        Wire1.write(byte(0x00));      
        Wire1.write(byte(0x51));  
        Wire1.endTransmission();   

        Wire1.beginTransmission(US_Rightw);
        Wire1.write(byte(0x02));  
        Wire1.endTransmission();   
        Wire1.requestFrom(US_Rightw, 2);   
        if (2 <= Wire1.available()) { 
        reading = Wire1.read(); 
        reading = reading << 8;    
        reading |= Wire1.read(); 
        Distance_raw[2]=reading;}
        Wire1.beginTransmission(US_Rightw); 
        Wire1.write(byte(0x00));  
        Wire1.write(byte(0x51));    
        Wire1.endTransmission();  

        Wire1.setClock(I2C_SPEED);
        US_Timer1 = 0;
    } 
    #endif
    #ifdef Robo_s
    if(US_Timer1 > (1000/US_Frequency)){
        Wire1.setClock(I2C_SPEED_US);
        int reading;

        Wire1.beginTransmission(US_Lefts);
        Wire1.write(byte(0x02));  
        Wire1.endTransmission();   
        Wire1.requestFrom(US_Lefts, 2);   
        if (2 <= Wire1.available()) { 
        reading = Wire1.read(); 
        reading = reading << 8;    
        reading |= Wire1.read(); 
        Distance_raw[0]=reading;}
        Wire1.beginTransmission(US_Lefts); 
        Wire1.write(byte(0x00));      
        Wire1.write(byte(0x51));  
        Wire1.endTransmission();   

        Wire1.beginTransmission(US_Backs);
        Wire1.write(byte(0x02));  
        Wire1.endTransmission();   
        Wire1.requestFrom(US_Backs, 2);   
        if (2 <= Wire1.available()) { 
        reading = Wire1.read(); 
        reading = reading << 8;    
        reading |= Wire1.read(); 
        Distance_raw[1]=reading;}
        Wire1.beginTransmission(US_Backs); 
        Wire1.write(byte(0x00));      
        Wire1.write(byte(0x51));  
        Wire1.endTransmission();   

        Wire1.beginTransmission(US_Rights);
        Wire1.write(byte(0x02));  
        Wire1.endTransmission();   
        Wire1.requestFrom(US_Rights, 2);   
        if (2 <= Wire1.available()) { 
        reading = Wire1.read(); 
        reading = reading << 8;    
        reading |= Wire1.read(); 
        Distance_raw[2]=reading;}
        Wire1.beginTransmission(US_Rights); 
        Wire1.write(byte(0x00));  
        Wire1.write(byte(0x51));    
        Wire1.endTransmission();  

        Wire1.setClock(I2C_SPEED);
        US_Timer1 = 0;
    } 
    #endif
}



