//
// Created by Rafael Günther on 23.10.25.
//

#pragma once

#include <cstdint>

#include "bus-interface/i2c_interface.h"

class IR_ring
{
public:
  explicit IR_ring(const Device_handle& handle) : _handle(handle) {};

  [[nodiscard]] uint8_t read_sensor_id() const;

  [[nodiscard]] uint16_t read_error_code() const;

  [[nodiscard]] uint16_t read_TSSP() const;

  [[nodiscard]] double read_ball_angle() const;
  [[nodiscard]] uint16_t read_ball_distance() const;

  [[nodiscard]] uint16_t read_raw_value(uint8_t sensor_id) const;
  void read_raw_values(uint16_t (&raw)[16]) const;

  [[nodiscard]] uint16_t read_calibrated_value(uint8_t sensor_id) const;
  void read_calibrated_values(uint16_t (&calib)[16]) const;

  [[nodiscard]] uint16_t read_gain(uint8_t sensor_id) const;
  void read_gains(uint16_t (&gains)[16]) const;
  void write_gain(uint8_t sensor_id, uint16_t gain) const;
  void write_gains(const uint16_t (&gains)[16]) const;

  [[nodiscard]] int16_t read_offset(uint8_t sensor_id) const;
  void read_offsets(int16_t (&offsets)[16]) const;
  void write_offset(uint8_t sensor_id, int16_t offset) const;
  void write_offsets(const uint16_t (&offsets)[16]) const;
  uint16_t offset_save[16];
private:
  const Device_handle _handle;
};