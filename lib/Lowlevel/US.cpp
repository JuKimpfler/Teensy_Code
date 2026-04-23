#include "US.h"
#include "RGB.h"

elapsedMillis US_Timer ;
USC US;

void USC::read(){
    if(US_Timer > (1000/US_Frequency)){
        Wire1.setClock(I2C_SPEED_US);
        int reading;

        Wire1.beginTransmission(US_Left);
        Wire1.write(byte(0x02));  
        Wire1.endTransmission();   
        Wire1.requestFrom(US_Left, 2);   
        if (2 <= Wire1.available()) { 
        reading = Wire1.read(); 
        reading = reading << 8;    
        reading |= Wire1.read(); 
        Distance[3]=reading;}
        Wire1.beginTransmission(US_Left); 
        Wire1.write(byte(0x00));      
        Wire1.write(byte(0x51));  
        Wire1.endTransmission();   

        Wire1.beginTransmission(US_Back);
        Wire1.write(byte(0x02));  
        Wire1.endTransmission();   
        Wire1.requestFrom(US_Back, 2);   
        if (2 <= Wire1.available()) { 
        reading = Wire1.read(); 
        reading = reading << 8;    
        reading |= Wire1.read(); 
        Distance[2]=reading;}
        Wire1.beginTransmission(US_Back); 
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
        Distance[1]=reading;}
        Wire1.beginTransmission(US_Right); 
        Wire1.write(byte(0x00));  
        Wire1.write(byte(0x51));    
        Wire1.endTransmission();  

        Wire1.beginTransmission(US_Front);
        Wire1.write(byte(0x02));  
        Wire1.endTransmission();   
        Wire1.requestFrom(US_Front, 2);   
        if (2 <= Wire1.available()) { 
        reading = Wire1.read(); 
        reading = reading << 8;    
        reading |= Wire1.read(); 
        Distance[0]=reading;}
        Wire1.beginTransmission(US_Front); 
        Wire1.write(byte(0x00));  
        Wire1.write(byte(0x51));    
        Wire1.endTransmission();

        Wire1.setClock(I2C_SPEED);
        US_Timer = 0;
    }
}

int USC::giveNR(int NR){
    return Distance[NR];
}



