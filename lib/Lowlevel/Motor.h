#pragma once
#include "enum.h"
#include "Param.h"

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
    bool Enable ;
};

extern MotorC Motor;