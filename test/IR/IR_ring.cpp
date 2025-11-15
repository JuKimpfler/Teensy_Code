//
// Created by Rafael Günther on 23.10.25.
//

#include "IR_ring.h"

#include "ir_ring_registers.h"

uint8_t IR_ring::read_sensor_id() const
{
  uint8_t sensor_id {};
  _handle.read_register<uint8_t, uint8_t>(static_cast<uint8_t>(Ir_ring_registers::sensor_id),sensor_id);

  return sensor_id;
}

uint16_t IR_ring::read_error_code() const
{
  uint16_t error_code {};
  _handle.read_register<uint8_t, uint16_t>(static_cast<uint8_t>(Ir_ring_registers::error_msb),error_code);

  return error_code;
}

uint16_t IR_ring::read_TSSP() const
{
  uint16_t TSSP {};
  _handle.read_register<uint8_t, uint16_t>(static_cast<uint8_t>(Ir_ring_registers::tssp_input),TSSP);

  return TSSP;
}


float IR_ring::read_ball_angle() const
{
  int16_t ball_angle;
  _handle.read_register<uint8_t, int16_t>(static_cast<uint8_t>(Ir_ring_registers::ball_angle_msb),ball_angle);

  return static_cast<float>(ball_angle) / 180.F;
}

uint16_t IR_ring::read_ball_distance() const
{
  uint16_t ball_distance;
  _handle.read_register<uint8_t, uint16_t>(static_cast<uint8_t>(Ir_ring_registers::ball_distance_msb),ball_distance);

  return ball_distance;
}

uint16_t IR_ring::read_raw_value(uint8_t sensor_id) const
{
  const uint8_t addr = static_cast<uint8_t>(Ir_ring_registers::ir1_data_raw_msb) + 2 * (sensor_id-1);
  uint16_t raw {};
  _handle.read_register<uint8_t, uint16_t>(addr,raw);

  return raw;
}

void IR_ring::read_raw_values(uint16_t (&raw)[16]) const
{
  _handle.read_registers<uint8_t, uint16_t>(static_cast<uint8_t>(Ir_ring_registers::ir1_data_raw_msb),raw,16);
}

uint16_t IR_ring::read_calibrated_value(uint8_t sensor_id) const
{
  const uint8_t addr = static_cast<uint8_t>(Ir_ring_registers::ir1_data_calib_msb) + 2 * (sensor_id-1);
  uint16_t calib {};
  _handle.read_register<uint8_t, uint16_t>(addr,calib);

  return calib;
}

void IR_ring::read_calibrated_values(uint16_t (&calib)[16]) const
{
  _handle.read_registers<uint8_t, uint16_t>(static_cast<uint8_t>(Ir_ring_registers::ir1_data_calib_msb),calib,16);
}

uint16_t IR_ring::read_gain(uint8_t sensor_id) const
{
  const uint8_t addr = static_cast<uint8_t>(Ir_ring_registers::ir1_gain_msb) + 2 * (sensor_id-1);
  uint16_t gain {};
  _handle.read_register<uint8_t, uint16_t>(addr,gain);

  return gain;
}

void IR_ring::read_gains(uint16_t (&gains)[16]) const
{
  _handle.read_registers<uint8_t, uint16_t>(static_cast<uint8_t>(Ir_ring_registers::ir1_gain_msb),gains,16);
}

void IR_ring::write_gain(uint8_t sensor_id, uint16_t gain) const
{
  const uint8_t addr = static_cast<uint8_t>(Ir_ring_registers::ir1_gain_msb) + 2 * (sensor_id-1);
  _handle.write_register<uint8_t, uint16_t>(addr, gain);
}

void IR_ring::write_gains(const uint16_t (&gains)[16]) const
{
  _handle.write_registers<uint8_t, uint16_t>(static_cast<uint8_t>(Ir_ring_registers::ir1_gain_msb), gains, 16);
}

int16_t IR_ring::read_offset(uint8_t sensor_id) const
{
  const uint8_t addr = static_cast<uint8_t>(Ir_ring_registers::ir1_offset_msb) + 2 * (sensor_id-1);
  int16_t offset {};
  _handle.read_register<uint8_t, int16_t>(addr, offset);

  return offset;
}

void IR_ring::read_offsets(int16_t (&offsets)[16]) const
{
  _handle.read_registers<uint8_t, int16_t>(static_cast<uint8_t>(Ir_ring_registers::ir1_offset_msb), offsets,16);
}

void IR_ring::write_offset(uint8_t sensor_id, int16_t offset) const
{
  const uint8_t addr = static_cast<uint8_t>(Ir_ring_registers::ir1_offset_msb) + 2 * (sensor_id-1);
  _handle.write_register<uint8_t, int16_t>(addr, offset);
}

void IR_ring::write_offsets(const int16_t (&offsets)[16]) const
{
  _handle.write_registers(static_cast<uint8_t>(Ir_ring_registers::ir1_offset_msb), offsets, 16);
}


