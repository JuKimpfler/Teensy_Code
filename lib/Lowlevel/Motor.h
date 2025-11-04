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
    void Break(int Motor);
};

extern MotorC Motor;