#pragma once
#include "Elementar.h"
#include "Wire.h" 
#include "SPI.h"


class ExpanderC {
    private:
        int ADC_Befehle[8] = {2048,4096,6144,8192,10240,12288,14336,16384};
    public:
        class I2CC {
            private :
                bool Switch[8] ;
                bool Dip[4];
            public:
                void init(int Add , int Mode , int on_off = All_Off);
                void read(int Add);
                bool give(int Add , int Port);
                void write(int Add , int Port , bool Zustand);
                void write_Single(int Add , int Port , bool Zustand);
                
        };
        I2CC I2C;
        
        class ADCC {
            private :
                int lineA[8];
                int lineB[8];
                int lineC[8];
                int lineD[8];
            public:
                int IR[8];
                void init(int Port);
                void read(int Port);
                int give_digital(int Port, int Pin);
                int give(int Port , int Pin);
        };
        ADCC ADC ;
};


extern ExpanderC Expander;