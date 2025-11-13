#include <Arduino.h>
#include <Wire.h>



void setup()
{
  Serial.begin(115200);

  
}

void loop()
{
  Serial.print("Ball angle: "); Serial.println(ir_ring.read_ball_angle());
  Serial.print("Ball distance: "); Serial.println(ir_ring.read_ball_distance());

  uint16_t calib_data[16];
  ir_ring.read_calibrated_values(calib_data);
  
  Serial.print("Calibrated data: ");
  for (int i=0; i<16; i++)
  {
    Serial.print(i+1); Serial.print(": ");
    Serial.print(calib_data[i]); Serial.print("  ");
  }
  Serial.println();

  delayMicroseconds(500);
}
