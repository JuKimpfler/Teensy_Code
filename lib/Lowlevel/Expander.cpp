#include "Expander.h"
ExpanderC Expander;

void ExpanderC::I2CC::init(int Add , int Mode , int on_off = All_Off){ // on_off -> write all Outputs to low or high
    if(Add == I2C_Button || Add == I2C_Dip_SW){
        Wire.begin(); 
        Wire.beginTransmission(Add);
        Wire.write(Config_Reg);        
        Wire.write(Mode);         
        Wire.endTransmission();
        Wire.beginTransmission(Add); 
        Wire.write(Output_Reg);         
        Wire.write(on_off);         
        Wire.endTransmission();
    }
    else{
        Wire1.begin(); 
        Wire1.beginTransmission(Add);
        Wire1.write(Config_Reg);        
        Wire1.write(Mode);         
        Wire1.endTransmission();
        Wire1.beginTransmission(Add); 
        Wire1.write(Output_Reg);         
        Wire1.write(on_off);         
        Wire1.endTransmission();
    }
}

void ExpanderC::I2CC::read(int Add){
    
    Wire.beginTransmission(Add); //address second port expander
    Wire.write(Input_Reg);  // select input register
    Wire.endTransmission(false);        // send repeated start instead of stop
    Wire.requestFrom(Add, 1);          // Start a read access and expect one byte
    byte last = Wire.read();     // read Register
    Wire.endTransmission(); // end of transmittion

    int rest=0;

    if(Add == I2C_Button){
        Switch[0] = (last & 1);
        Switch[1] = (last & (1 << 1));
        Switch[2] = (last & (1 << 2));
        Switch[3] = (last & (1 << 3));
        Switch[4] = (last & (1 << 4));
        Switch[5] = (last & (1 << 5));
        Switch[6] = (last & (1 << 6));
        Switch[7] = (last & (1 << 7));
    }
    else if (Add == I2C_Dip_SW){
        Dip[0] = (last & 1);
        Dip[1] = (last & (1 << 1));
        Dip[2] = (last & (1 << 2));
        Dip[3] = (last & (1 << 3));
        rest = (last & (1 << 4));
        rest = (last & (1 << 5));
        rest = (last & (1 << 6));
        rest = (last & (1 << 7));
    }
}

bool ExpanderC::I2CC::give(int Add , int Port){
    if (Add == I2C_Dip_SW){
        return Dip[Port];
    }
    else if (Add == I2C_Button){
        return Switch[Port];
    }
}

void ExpanderC::I2CC::write(int Add , int Port , bool Zustand){
    Wire1.beginTransmission(Add); //address second port expander
    Wire1.write(Output_Reg);             // select output register
    Wire1.endTransmission(false);  // send restart instead of stop
    Wire1.requestFrom(Add, 1);    // Start a read access and expect one byte
    byte last = Wire1.read();          // get current port state
    Wire1.endTransmission();

    byte out = 0;
    int Switch[8] = {0,0,0,0,0,0,0,0};

    if (int((last >> 7)) % 2 == 0) {Switch[0]=0;} else{Switch[0]=1;}
    if (int((last >> 6)) % 2 == 0) {Switch[1]=0;} else{Switch[1]=1;}
    if (int((last >> 5)) % 2 == 0) {Switch[2]=0;} else{Switch[2]=1;}
    if (int((last >> 4)) % 2 == 0) {Switch[3]=0;} else{Switch[3]=1;}
    if (int((last >> 3)) % 2 == 0) {Switch[4]=0;} else{Switch[4]=1;}
    if (int((last >> 2)) % 2 == 0) {Switch[5]=0;} else{Switch[5]=1;}
    if (int((last >> 1)) % 2 == 0) {Switch[6]=0;} else{Switch[6]=1;}
    if (int((last >> 0)) % 2 == 0) {Switch[7]=0;} else{Switch[7]=1;}

    Switch[Port] = int(Zustand);

    for (int i = 0; i < 8; i++) {
        if (Switch[i] == 1) {
            out |= (1 << (7 - i)); // Bit setzen, wenn der Wert 1 ist
        }
    }

    Wire1.beginTransmission(Add); //address second port expander
    Wire1.write(Output_Reg);             // select output register
    Wire1.write(out);            // update output ports Port_Seting
    Wire1.endTransmission();
}


void ExpanderC::ADCC::init(int Port){
    if (Port == CS_IR){
        #ifndef Spi1_Start
        #define Spi1_Start
            SPI1.begin();
        #endif
        pinMode(Port, OUTPUT);  /* ADC chip select */
        digitalWrite(Port, HIGH);  /* prepare default state of ADC chip select */
    }
    else{
        #ifndef Spi0_Start
        #define Spi0_Start
            SPI.begin();
        #endif
        pinMode(Port, OUTPUT);  /* ADC chip select */
        digitalWrite(Port, HIGH);  /* prepare default state of ADC chip select */
    }
}

void ExpanderC::ADCC::read(int Port){
    if (Port == CS_LineA || Port == CS_LineB || Port == CS_LineC || Port == CS_LineD ){
        SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE3)); //Start SPI und Einstellern der Übertragungsparameter

        digitalWrite(Port, LOW);  /* prepare default state of ADC chip select */

        for (int i = 0 ; i<8 ; i++){
            if (Port == CS_LineA){
                lineA[i] = SPI.transfer16(Expander.ADC_Befehle[i]);
            }
            else if (Port == CS_LineB){
                lineB[i] = SPI.transfer16(Expander.ADC_Befehle[i]);
            }
            else if (Port == CS_LineC){
                lineC[i] = SPI.transfer16(Expander.ADC_Befehle[i]);
            }
            else if (Port == CS_LineD){
                lineD[i] = SPI.transfer16(Expander.ADC_Befehle[i]);
            }
        }

        digitalWrite(Port, HIGH);  // prepare default state of ADC chip select 
        SPI.endTransaction(); 
    }
    else{
        SPI1.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE3)); //Start SPI und Einstellern der Übertragungsparameter

        digitalWrite(Port, LOW); /* prepare default state of ADC chip select */

        for (int i = 0 ; i<8 ; i++){
            IR[i] = SPI1.transfer16(Expander.ADC_Befehle[i]);
        }

        digitalWrite(Port, HIGH);
        SPI1.endTransaction(); 
    }
}

int ExpanderC::ADCC::give(int Port , int Pin){
    if (Port == CS_LineA){
        return lineA[Pin];
    }
    else if (Port == CS_LineB){
        return lineB[Pin];
    }
    else if (Port == CS_LineC){
        return lineC[Pin];
    }
    else if (Port == CS_LineD){
        return lineD[Pin];
    }
    else if (Port == CS_IR){
        return IR[Pin] ;
    }
}


int ExpanderC::ADCC::give_digital(int Port , int Pin){
    if (Port == CS_LineA){
        if(lineA[Pin]<Line_Schwelle){
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (Port == CS_LineB){
        if(lineB[Pin]<Line_Schwelle){
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (Port == CS_LineC){
        if(lineC[Pin]<Line_Schwelle){
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (Port == CS_LineD){
        if(lineD[Pin]<Line_Schwelle){
            return 0;
        }
        else {
            return 1;
        }
    }
}
