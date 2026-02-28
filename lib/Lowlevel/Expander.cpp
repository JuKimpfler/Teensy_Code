#include "Expander.h"
ExpanderC Expander;

void ExpanderC::I2CC::init(int Add , int Mode , int on_off){ // on_off -> write all Outputs to low or high
    //Wire1.begin();
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

    if(Add == I2C_ITF_Main){
        Switch[0] = (last & 1);
        Switch[1] = (last & (1 << 1));
        Switch[2] = (last & (1 << 2));
        Switch[3] = (last & (1 << 3));
        Switch[4] = (last & (1 << 4));
        Switch[5] = (last & (1 << 5));
        Switch[6] = (last & (1 << 6));
        Switch[7] = (last & (1 << 7));
    }
}

bool ExpanderC::I2CC::give(int Add , int Port){
    if (Add == I2C_ITF_Main){
        return Switch[Port];
    }
}


void ExpanderC::I2CC::write_Single(int Add , int Port , bool Zustand){
    if ( Zustand == true ){
        Wire1.beginTransmission(Add); //address second port expander
        Wire1.write(Output_Reg);             // select output register
        Wire1.write(00000001);            // update output ports Port_Seting
        Wire1.endTransmission();
    }
    else if ( Zustand == false ){
        Wire1.beginTransmission(Add); //address second port expander
        Wire1.write(Output_Reg);             // select output register
        Wire1.write(00000000);            // update output ports Port_Seting
        Wire1.endTransmission();
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