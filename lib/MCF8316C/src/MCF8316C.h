/**
 * MCF8316C.h
 *
 * Arduino/Teensy library for the Texas Instruments MCF8316C-Q1
 * sensorless BLDC motor driver over I2C.
 *
 * IMPORTANT: Register addresses and bit-field definitions are based on the
 * MCF8316C-Q1 datasheet (SLVSEO5, rev. A).  Always cross-check with your
 * specific silicon revision and the TI EVM GUI register export before
 * deploying to production.  Uncertain fields are marked with "// VERIFY".
 *
 * Wiring assumptions (see README.md for full details):
 *   SDA / SCL  — I2C bus (Teensy 4.0: pins 18/19 by default)
 *   SPEED      — PWM output from Teensy to SPEED pin of driver
 *   DIR        — digital output (HIGH = CW, LOW = CCW) or tie to GND/VCC
 *   BRAKE      — digital output (HIGH = brake, LOW = coast/run)
 *   DRVOFF     — digital output (LOW = driver enabled, HIGH = driver off)
 *   FAULT      — digital input (LOW = fault present)
 *   FG         — optional frequency output (motor speed feedback)
 *
 * License: MIT
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>

// ---------------------------------------------------------------------------
// Default I2C address
// The MCF8316C-Q1 base address is 0x60; the two LSBs are set by the ADDR pin.
//   ADDR = GND  -> 0x60
//   ADDR = VCC  -> 0x61   (VERIFY with your board)
// ---------------------------------------------------------------------------
#define MCF8316C_DEFAULT_ADDR  0x60u

// ---------------------------------------------------------------------------
// Register addresses  (VERIFY against datasheet Table 7-x)
// All registers are 32-bit wide; the driver auto-increments by 4 bytes.
// The I2C transaction sends: [addr_high, addr_low, byte3, byte2, byte1, byte0]
// ---------------------------------------------------------------------------
#define MCF8316C_REG_ALGO_CTRL1      0x0080u  ///< Algorithm Control 1
#define MCF8316C_REG_ALGO_CTRL2      0x0084u  ///< Algorithm Control 2
#define MCF8316C_REG_ALGO_CTRL3      0x0088u  ///< Algorithm Control 3
#define MCF8316C_REG_ALGO_CTRL4      0x008Cu  ///< Algorithm Control 4
#define MCF8316C_REG_ALGO_CTRL5      0x0090u  ///< Algorithm Control 5
#define MCF8316C_REG_ALGO_CTRL6      0x0094u  ///< Algorithm Control 6
#define MCF8316C_REG_ALGO_CTRL7      0x0098u  ///< Algorithm Control 7
#define MCF8316C_REG_ALGO_CTRL8      0x009Cu  ///< Algorithm Control 8

#define MCF8316C_REG_SYS_CTRL1       0x00E0u  ///< System Control 1
#define MCF8316C_REG_SYS_CTRL2       0x00E4u  ///< System Control 2

#define MCF8316C_REG_FAULT_STATUS    0x00E8u  ///< Fault Status (read-only)
#define MCF8316C_REG_MOTOR_STATUS    0x00ECu  ///< Motor Status  (read-only)

#define MCF8316C_REG_DEVICE_ID       0x00F8u  ///< Device ID / revision (read-only)

// ---------------------------------------------------------------------------
// ALGO_CTRL1 bit-fields  (VERIFY)
// ---------------------------------------------------------------------------
#define MCF8316C_ALGO1_DIR_BIT       (1u << 0)   ///< 0 = CCW, 1 = CW
#define MCF8316C_ALGO1_BRAKE_BIT     (1u << 1)   ///< 0 = run, 1 = brake
#define MCF8316C_ALGO1_DRVOFF_BIT    (1u << 2)   ///< 0 = driver on, 1 = off

// ---------------------------------------------------------------------------
// SYS_CTRL1 bit-fields  (VERIFY)
// ---------------------------------------------------------------------------
#define MCF8316C_SYSCTRL1_CLR_FLT    (1u << 0)   ///< Write 1 to clear faults

// ---------------------------------------------------------------------------
// FAULT_STATUS bit masks  (VERIFY against Table 8-x)
// ---------------------------------------------------------------------------
#define MCF8316C_FAULT_OCP           (1u << 0)   ///< Over-current protection
#define MCF8316C_FAULT_OVP           (1u << 1)   ///< Over-voltage protection
#define MCF8316C_FAULT_UVLO          (1u << 2)   ///< Under-voltage lockout
#define MCF8316C_FAULT_OTW           (1u << 3)   ///< Over-temperature warning
#define MCF8316C_FAULT_OTS           (1u << 4)   ///< Over-temperature shutdown
#define MCF8316C_FAULT_MOTOR_LOCK    (1u << 5)   ///< Motor lock detected
#define MCF8316C_FAULT_STARTUP_FAIL  (1u << 6)   ///< Startup failure
#define MCF8316C_FAULT_ANY           (0x7Fu)      ///< Any fault mask


// ---------------------------------------------------------------------------
// Class declaration
// ---------------------------------------------------------------------------
class MCF8316C {
public:
    /**
     * Constructor.
     * Does not communicate with the device; call begin() first.
     */
    MCF8316C();

    /**
     * Initialise the library and verify I2C communication.
     *
     * @param wire     Reference to a TwoWire bus (Wire, Wire1, …)
     * @param i2cAddr  7-bit I2C address of the device (default 0x60)
     * @return true on success (device acknowledged), false otherwise
     */
    bool begin(TwoWire& wire = Wire, uint8_t i2cAddr = MCF8316C_DEFAULT_ADDR);

    // -----------------------------------------------------------------------
    // Low-level register access
    // -----------------------------------------------------------------------

    /**
     * Read a 32-bit register.
     *
     * @param regAddr  16-bit register address
     * @param value    Receives the 32-bit value
     * @return true on success
     */
    bool readReg(uint16_t regAddr, uint32_t& value);

    /**
     * Write a 32-bit register.
     *
     * @param regAddr  16-bit register address
     * @param value    32-bit value to write
     * @return true on success
     */
    bool writeReg(uint16_t regAddr, uint32_t value);

    /**
     * Read-modify-write a register (set bits in mask to values).
     *
     * @param regAddr  16-bit register address
     * @param mask     Bits to modify
     * @param values   New bit values (only bits in mask are applied)
     * @return true on success
     */
    bool modifyReg(uint16_t regAddr, uint32_t mask, uint32_t values);

    // -----------------------------------------------------------------------
    // Motor control
    // -----------------------------------------------------------------------

    /**
     * Apply safe startup defaults via I2C registers.
     * Call once after begin() before enabling the driver.
     *
     * @return true on success
     */
    bool basicConfigure();

    /**
     * Enable the motor driver (clear DRVOFF bit in register).
     * @return true on success
     */
    bool enableDriver();

    /**
     * Disable the motor driver (set DRVOFF bit in register).
     * @return true on success
     */
    bool disableDriver();

    /**
     * Set motor rotation direction.
     *
     * @param cw  true = clockwise, false = counter-clockwise
     * @return true on success
     */
    bool setDirection(bool cw);

    /**
     * Set motor speed as a percentage of full speed.
     * The speed is mapped to the SPEED_INPUT register field.
     * Note: actual shaft speed depends on motor parameters and load.
     *
     * @param pct  0.0 … 100.0  (clamped to valid range)
     * @return true on success
     */
    bool setSpeedPercent(float pct);

    /**
     * Enable or disable the brake function.
     *
     * @param on  true = brake active, false = brake released
     * @return true on success
     */
    bool setBrake(bool on);

    // -----------------------------------------------------------------------
    // Fault management
    // -----------------------------------------------------------------------

    /**
     * Read the 32-bit fault status register.
     *
     * @param faultBits  Receives the raw fault status word
     * @return true if the read was successful
     */
    bool readFault(uint32_t& faultBits);

    /**
     * Convenience: return true if any fault bit is set.
     * Sets commError to true if the I2C read itself failed so the caller
     * can distinguish a hardware fault from a communication failure.
     *
     * @param commError  Optional pointer; set to true if I2C read failed,
     *                   false if the read succeeded (fault bits may still be set).
     */
    bool hasFault(bool* commError = nullptr);

    /**
     * Attempt to clear latched faults by writing the CLR_FLT bit.
     * @return true on success
     */
    bool clearFaults();

    // -----------------------------------------------------------------------
    // Diagnostics
    // -----------------------------------------------------------------------

    /**
     * Read the device-ID register and print it over Serial for verification.
     * Useful during first bring-up.
     */
    void printDeviceId();

private:
    TwoWire* _wire;
    uint8_t  _addr;
    uint32_t _algo1Cache;  ///< cached value of ALGO_CTRL1 to avoid extra reads

    // Speed is stored in ALGO_CTRL2[31:22] as a 10-bit value (0–1023) // VERIFY
    static constexpr uint32_t SPEED_FIELD_MASK  = 0xFFC00000u;
    static constexpr uint8_t  SPEED_FIELD_SHIFT = 22u;
    static constexpr uint32_t SPEED_MAX_RAW     = 1023u;
};
