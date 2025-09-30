#pragma once
#include "Elementar.h"
#include "Expander.h"
#include "Arduino.h"

class MotorC
{
  private: 
  public: 
    // Variables
 
    // Funktions
    void init();
    void On(int Speed , int Motor); // +- Speed für Richtung
    void Off(int Motor);
    void OnFast(int MotorVR , int MotorVL , int MotorHR , int MotorHL);
    void OffFast();
    void BreakFast();

    int Iteration_list[8] = {VR_PortA,VR_PortB,VL_PortA,VL_PortB,HR_PortA,HR_PortB,HL_PortA,HL_PortB};
};

extern MotorC Motor;