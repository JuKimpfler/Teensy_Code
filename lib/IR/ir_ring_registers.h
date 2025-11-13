//
// Created by Rafael Günther on 23.10.25.
//

#pragma once

enum class Ir_ring_registers : uint8_t
{
  sensor_id           = 0x00,

  error_msb           = 0x0A,
  error_lsb           = 0x0B,
  user_input          = 0x0C,
  tssp_input          = 0x0D,

  ball_angle_msb      = 0x11,
  ball_angle_lsb      = 0x12,
  ball_distance_msb   = 0x13,
  ball_distance_lsb   = 0x14,

  sensor_enable       = 0x19,
  led_enable          = 0x1D,

  ir1_data_raw_msb    = 0x28,

  ir1_data_calib_msb  = 0x48,

  ir1_offset_msb      = 0x68,
  ir1_offset_lsb      = 0x69,

  ir1_gain_msb        = 0x88,
  ir1_gain_lsb        = 0x89,

};