#include "Cam.h"
#include "Debug.h"
#include "Elementar.h"
#include "LineCalculations.h"
#include "RGB.h"
#include "System.h"

void setup() {
  SPI.begin();
  System.init.Sensors();
  System.init.Motors();
  System.init.Interface();
  Serial.begin(115200);
  UART_2.begin(115200);
  UART_Pixy.begin(115200);
}

void loop() {
  Debug.Start();
  Debug.Plot("Time", Cycletime);

  Cycle_Timer = 0;
  // Mouse.read();
  System.Update.Sensors();
  System.Update.Interface();
  System.Update.Calculations();

  BNO055.showCal();

  if (Robot.Start == true) {
    //Game.Run();
    Robot.Turn(0);
    //Motor.On(100,VL_Motor);  
    //Motor.On(100,HR_Motor);
  } else {
    Robot.Stop();
    // Game.Run();
  }

  delay(5);

  Debug.Plot("Pid",PID.Out);
  Debug.Plot("Pid_in",PID.ealt);
  Debug.Plot("bno",BNO055.TiltZ);

  // Mouse Sensor Test
  //Debug.Plot_List("line",Line.Raw,32);
  //Debug.Plot_List("line_VW",Line.Raw_VW,8);
  //Debug.Plot("lift", Mouse.lift);
  //Debug.Plot("move", Mouse.movement);
  //Debug.Plot("x", Mouse.deltaX);
  //Debug.Plot("y", Mouse.deltaY);
  //Debug.Plot("xpos", Mouse.xPos);
  //Debug.Plot("ypos", Mouse.yPos);
  //Debug.Plot("speed", Mouse.delta_dist);
  Debug.Send();

  if (Robot.Button[0]) {
    BNO055.Calibrate();
  }
  if (Robot.Button[1]) {
    IR.Calib_Offset();
  }
  if (Robot.Button[2]) {
    IR.DistCal = IR.Distance_raw;
  }
  if (Robot.Button[3]) {
    IR.Calib_Dist();
  }

  if (Robot.Switches[0]) {
    MainSpeed = HighSpeed;
  } else {
    MainSpeed = LowSpeed;
  }

  Cycletime = Cycle_Timer;
}
