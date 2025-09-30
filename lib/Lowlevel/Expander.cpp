#include "Expander.h"
ExpanderC Expander;

void ExpanderC::I2CC::init(int Add , int Mode , int on_off = All_Off){ // on_off -> write all Outputs to low or high
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

void ExpanderC::I2CC::read(int Add){
    
    Wire1.beginTransmission(Add); //address second port expander
    Wire1.write(Input_Reg);  // select input register
    Wire1.endTransmission(false);        // send repeated start instead of stop
    Wire1.requestFrom(Add, 1);          // Start a read access and expect one byte
    byte last = Wire1.read();     // read Register
    Wire1.endTransmission(); // end of transmittion

    int rest=0;

    if (Add == I2C_ITF_1){
        ITF1[0] = (last & 1);
        ITF1[1] = (last & (1 << 1));
        ITF1[2] = (last & (1 << 2));
        ITF1[3] = (last & (1 << 3));
        ITF1[4] = (last & (1 << 4));
        ITF1[5] = (last & (1 << 5));
        ITF1[6] = (last & (1 << 6));
        ITF1[7] = (last & (1 << 7)); 
    }

    else if (Add == I2C_ITF_2){
        ITF2[0] = (last & 1);
        ITF2[1] = (last & (1 << 1));
        ITF2[2] = (last & (1 << 2));
        ITF2[3] = (last & (1 << 3));
        ITF2[4] = (last & (1 << 4));
        ITF2[5] = (last & (1 << 5));
        ITF2[6] = (last & (1 << 6));
        ITF2[7] = (last & (1 << 7));  
    }

    else if (Add == I2C_Ex){
        Ex[0] = (last & 1);
        Ex[1] = (last & (1 << 1));
        Ex[2] = (last & (1 << 2));
        Ex[3] = (last & (1 << 3));
        Ex[4] = (last & (1 << 4));
        Ex[5] = (last & (1 << 5));
        Ex[6] = (last & (1 << 6));
        Ex[7] = (last & (1 << 7)); 
    }

}

bool ExpanderC::I2CC::give(int Add , int Port){
    if(Add == I2C_Ex){
        return Ex[Port];
    }
    else if(Add == I2C_ITF_1){
        return ITF1[Port];
    }
    else if(Add == I2C_ITF_2){
        return ITF2[Port];
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
    #ifndef Spi0_Start
    #define Spi0_Start
        SPI.begin();
    #endif
    pinMode(Port, OUTPUT);  /* ADC chip select */
    digitalWrite(Port, HIGH);  /* prepare default state of ADC chip select */
}

void ExpanderC::ADCC::read(int Port){
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
        else if (Port == CS_Line_OUT){
            lineOUT[i] = SPI.transfer16(Expander.ADC_Befehle[i]);
        }
    }

    digitalWrite(Port, HIGH);  // prepare default state of ADC chip select 
    SPI.endTransaction(); 
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
    else if (Port == CS_Line_OUT){
        return lineOUT[Pin] ;
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
    else if (Port == CS_Line_OUT){
        if(lineOUT[Pin]<Line_Schwelle){
            return 0;
        }
        else {
            return 1;
        }
    }
}
