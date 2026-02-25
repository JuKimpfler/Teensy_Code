#pragma once
#include "Elementar.h"
#include "Wire.h" 


class ExpanderC {
    private:
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
};


extern ExpanderC Expander;