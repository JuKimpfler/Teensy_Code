#include "US.h"
USC US;

void USC::init(){

}

void USC::Read(){
    int reading;

    Wire.beginTransmission(0x71);
    Wire.write(byte(0x02));    
    Wire.endTransmission();    
    Wire.requestFrom(0x71, 2); 
    if (2 <= Wire.available()) { 
    reading = Wire.read(); 
    reading = reading << 8;   
    reading |= Wire.read();
    US_R = reading;
    }

    Wire.beginTransmission(0x74);
    Wire.write(byte(0x02));  
    Wire.endTransmission();   
    Wire.requestFrom(0x74, 2);   
    if (2 <= Wire.available()) { 
    reading = Wire.read(); 
    reading = reading << 8;    
    reading |= Wire.read(); 
    US_L=reading;
    }

    Wire.beginTransmission(0x70);
    Wire.write(byte(0x02));  
    Wire.endTransmission();   
    Wire.requestFrom(0x70, 2);   
    if (2 <= Wire.available()) { 
    reading = Wire.read(); 
    reading = reading << 8;    
    reading |= Wire.read(); 
    US_H=reading;
    }

    // US Sensoren Anfragen
    Wire.beginTransmission(0x71); 
    Wire.write(byte(0x00));  
    Wire.write(byte(0x51));    
    Wire.endTransmission();   
    Wire.beginTransmission(0x74); 
    Wire.write(byte(0x00));      
    Wire.write(byte(0x51));  
    Wire.endTransmission();   
    Wire.beginTransmission(0x70); 
    Wire.write(byte(0x00));  
    Wire.write(byte(0x51));    
    Wire.endTransmission();   
}



