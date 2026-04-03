#include "System.h"

SystemC System;

void SystemC::begin(int CIndex){
    PFU.begin();
}

void SystemC::UpdateC::Interface(){
    
}

void SystemC::UpdateC::Sensors(){
    Line.read_Fast();
}

void SystemC::UpdateC::Calculations(){
    PFU.Update();
}
