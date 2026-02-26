#include <Arduino.h>
#include <Wire.h>

#include "ir_ring.h"

#define I2C_BUS Wire1     // IR-Ring on Wire1
#define I2C_SPEED 1000000 // 1 MHz

// IR-Ring setup
I2C_bus i2c(I2C_BUS);
Device_handle ir_ring_handle = {i2c, I2C_target{0x0A}};
IR_ring ir_ring(ir_ring_handle);

// Helper to print arrays easily
template <typename T>
void print_array(const char* label, T* data, size_t size)
{
  Serial.print(label);
  Serial.print(": [ ");
  for (size_t i = 0; i < size; i++)
  {
    Serial.print(data[i]);
    if (i < size - 1)
      Serial.print(", ");
  }
  Serial.println(" ]");
}

void print_diagnostics()
{
  Serial.println(F("--- SYSTEM DIAGNOSTICS ---"));

  // Read Sensor ID
  uint8_t id = ir_ring.read_sensor_id();
  Serial.print(F("Sensor ID: ")); Serial.println(id);

  // Read Firmware Version (Struct)
  IR_ring::Fw_version fw = ir_ring.read_fw_version();
  Serial.print(F("Firmware: v"));
  Serial.print(fw.major); Serial.print(".");
  Serial.print(fw.minor); Serial.print(".");
  Serial.println(fw.patch);

  // Read Hardware Version (Enum)
  IR_ring::Hw_version hw = ir_ring.read_hw_version();
  Serial.print(F("Hardware: "));
  switch (hw)
  {
    case IR_ring::Hw_version::v1_0: Serial.println(F("v1.0")); break;
    case IR_ring::Hw_version::v1_1: Serial.println(F("v1.1")); break;
    default: Serial.println(F("Unknown")); break;
  }

  // Read Error Code
  uint16_t error = ir_ring.read_error_code();
  Serial.print(F("Error Code: ")); Serial.println(error, HEX);

  // Power Supply Status
  uint8_t pwr = ir_ring.read_sensor_power_supply();
  Serial.print(F("Power Supply State: ")); Serial.println(pwr, BIN);

  // User Input
  uint8_t user_in = ir_ring.read_user_input();
  Serial.print(F("User Input (Buttons/Dip): ")); Serial.println(user_in, BIN);

  Serial.println(F("--------------------------"));
}

void demo_configuration()
{
  Serial.println(F("--- CONFIGURATION DEMO ---"));

  // --- GAINS ---
  // Read all gains
  uint16_t gains[16];
  ir_ring.read_gains(gains);
  print_array("Current Gains", gains, 16);

  // Read single gain
  uint16_t gain_1 = ir_ring.read_gain(1);
  Serial.print(F("Gain IR1: ")); Serial.println(gain_1);

  // Write all gains (Example: writing the array back)
  ir_ring.write_gains(gains);

  // Write single gain (Example: setting IR1 gain to 4000)
  //ir_ring.write_gain(1, 4000);

  // --- OFFSETS ---
  // Read all offsets
  int16_t offsets[16];
  ir_ring.read_offsets(offsets);
  print_array("Current Offsets", offsets, 16);

  // Read single offset
  int16_t offset_5 = ir_ring.read_offset(5);
  Serial.print(F("Offset Ch5: ")); Serial.println(offset_5);

  // Write all offsets
  ir_ring.write_offsets(offsets);

  // Write single offset
  //ir_ring.write_offset(5, 0);

  // --- THRESHOLDS & MASKS ---
  // Visibility Threshold
  uint16_t vis_th = ir_ring.read_ball_visibility_threshold();
  Serial.print(F("Vis. Threshold: ")); Serial.println(vis_th);
  ir_ring.write_ball_visibility_threshold(vis_th); // Write back same value

  // Sensor Mask
  uint16_t mask = ir_ring.read_ir_sensor_mask();
  Serial.print(F("Sensor Mask: ")); Serial.println(mask, BIN);
  ir_ring.write_ir_sensor_mask(mask); // Write back same value

  // --- POWER CONTROL ---
  // Example: Enable IR A, IR B, and TSSP
  ir_ring.write_sensor_power_supply(true, true, true);
  Serial.println(F("Power supply configured."));

  // --- CALIBRATION MEMORY ---
  // WARNING: These commands write to EEPROM.
  // They are commented out to prevent accidental wear during the demo.

  //ir_ring.save_calibration(true, true, true);  // Save Gains, Offsets, Threshold
  //ir_ring.load_calibration(true, true, true);  // Load from EEPROM
  //ir_ring.reset_calibration(true, true, true); // Reset to factory defaults

  Serial.println(F("--------------------------"));
}

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(10); // Wait for USB

  // I2C setup
  I2C_BUS.begin();
  I2C_BUS.setClock(I2C_SPEED);

  // Run one-off diagnostics and config checks
  print_diagnostics();
  demo_configuration();
}

void loop()
{
  // --- BALL DATA ---
  bool is_visible = ir_ring.read_ball_visible();

  if (is_visible)
  {
    float angle = ir_ring.read_ball_angle();
    uint16_t dist = ir_ring.read_ball_distance();

    Serial.print(F("BALL FOUND > Angle: "));
    Serial.print(angle);
    Serial.print(F(" deg | Dist: "));
    Serial.println(dist);
  }
  else
    Serial.println(F("Ball not visible"));

  // --- RAW VALUES ---
  // Reading the array (fastest way)
  uint16_t raw_data[16];
  ir_ring.read_raw_values(raw_data);

  // You can also read a specific sensor individually:
  // uint16_t raw_0 = ir_ring.read_raw_value(0);

  print_array("Raw", raw_data, 16);

  // --- CALIBRATED VALUES ---
  // Reading the array
  uint16_t calib_data[16];
  ir_ring.read_calibrated_values(calib_data);

  // You can also read a specific sensor individually:
  // uint16_t calib_0 = ir_ring.read_calibrated_value(0);

  print_array("Calib", calib_data, 16);

  // --- TSSP DATA ---
  // Method A: Read as boolean array
  bool tssp_bools[8];
  ir_ring.read_tssp(tssp_bools);

  Serial.print(F("TSSP (Arr): [ "));
  for (int i = 0; i < 8; i++)
    Serial.print(tssp_bools[i]);
  Serial.println(F(" ]"));

  // Method B: Read as byte bitmask
  uint8_t tssp_byte = ir_ring.read_tssp();
  Serial.print(F("TSSP (Byte): 0x")); Serial.println(tssp_byte, HEX);

  Serial.println();
  delay(200); // Slow down loop for readability
}