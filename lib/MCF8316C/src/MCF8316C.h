/**
 * MCF8316C.h
 *
 * Arduino/Teensy library for the Texas Instruments MCF8316C-Q1
 * sensorless BLDC motor driver over I2C.
 *
 * I2C protocol (MCF8316C-Q1, datasheet SLLSFV2 + app-note SLLA662):
 *   The device does NOT use a standard 2-byte register address.
 *   Every transaction starts with a 24-bit Control Word (3 bytes, MSB first):
 *     Bit [23]    OP_R/W   : 0 = Write, 1 = Read
 *     Bit [22]    CRC_EN   : 0 = CRC disabled
 *     Bits [21:20] DLEN    : 01b = 32-bit data (always used here)
 *     Bits [19:16] MEM_SEC : 0x0
 *     Bits [15:12] MEM_PAGE: 0x0
 *     Bits [11:0]  MEM_ADDR: 12-bit register address
 *   Data bytes are transmitted/received LSB-first.
 *
 * Wiring assumptions (see README.md for full details):
 *   SDA / SCL  — I2C bus (Teensy 4.0: Wire1 pins 17/16)
 *   SPEED/WAKE — optional digital wake pulse from Teensy
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
// The MCF8316C-Q1 7-bit I2C address defaults to 0x01 (ADDR pin = GND).
// ---------------------------------------------------------------------------
#define MCF8316C_DEFAULT_ADDR  0x01u

// ---------------------------------------------------------------------------
// Register addresses  (12-bit MEM_ADDR field used inside the control word)
//
// EEPROM Shadow registers (0x080–0x0AE): writes take effect immediately in
// shadow RAM; EEPROM is NOT changed unless EEPROM_WRT is triggered.
// ---------------------------------------------------------------------------
#define MCF8316C_REG_ISD_CONFIG      0x0080u  ///< ISD Configuration
#define MCF8316C_REG_REV_DRIVE       0x0082u  ///< Reverse Drive Configuration
#define MCF8316C_REG_MOTOR_STARTUP1  0x0084u  ///< Motor Startup 1
#define MCF8316C_REG_MOTOR_STARTUP2  0x0086u  ///< Motor Startup 2
#define MCF8316C_REG_CLOSED_LOOP1    0x0088u  ///< Closed-Loop Control 1
#define MCF8316C_REG_CLOSED_LOOP2    0x008Au  ///< Closed-Loop Control 2
#define MCF8316C_REG_CLOSED_LOOP3    0x008Cu  ///< Closed-Loop Control 3
#define MCF8316C_REG_CLOSED_LOOP4    0x008Eu  ///< Closed-Loop Control 4 (MAX_SPEED)
#define MCF8316C_REG_FAULT_CONFIG1   0x0090u  ///< Fault Configuration 1
#define MCF8316C_REG_FAULT_CONFIG2   0x0092u  ///< Fault Configuration 2
#define MCF8316C_REG_PIN_CONFIG1     0x00A4u  ///< Pin Configuration 1 (SPEED_MODE)
#define MCF8316C_REG_DEVICE_CONFIG2  0x00A8u  ///< Device Configuration 2 (DEV_MODE)

// RAM registers (0x0C0–0x0EE): runtime status and algorithm control
#define MCF8316C_REG_GD_FAULT_STATUS 0x00E0u  ///< Gate Driver Fault Status (read-only)
#define MCF8316C_REG_CT_FAULT_STATUS 0x00E2u  ///< Controller Fault Status  (read-only)
#define MCF8316C_REG_ALGO_CTRL1      0x00EAu  ///< Algorithm Control 1 (CLR_FLT / EEPROM ops)

// ---------------------------------------------------------------------------
// PIN_CONFIG1 — SPEED_MODE field (bits [1:0])
// ---------------------------------------------------------------------------
#define MCF8316C_SPEED_MODE_PWM      0x0000u  ///< 0b00 = PWM input  (EEPROM default)
#define MCF8316C_SPEED_MODE_ANALOG   0x0001u  ///< 0b01 = Analog voltage input
#define MCF8316C_SPEED_MODE_I2C      0x0002u  ///< 0b10 = I2C speed register

// ---------------------------------------------------------------------------
// ALGO_CTRL1 bit-fields  (register 0x0EA)
// ---------------------------------------------------------------------------
#define MCF8316C_ALGO1_CLR_FLT       (1u << 29)  ///< Clear latched faults
#define MCF8316C_ALGO1_EEPROM_READ   (1u << 30)  ///< Load EEPROM → shadow RAM
#define MCF8316C_ALGO1_EEPROM_WRT    (1u << 31)  ///< Write shadow RAM → EEPROM

// ---------------------------------------------------------------------------
// CLOSED_LOOP1 bit-fields  (register 0x0088)
// ---------------------------------------------------------------------------
#define MCF8316C_CL1_SPEED_LOOP_DIS  (1u << 1)   ///< 1 = Torque/current mode

// ---------------------------------------------------------------------------
// Fault status bit masks
// Gate Driver Fault Status register (0x00E0)
// ---------------------------------------------------------------------------
#define MCF8316C_FAULT_OCP           (1u << 28)  ///< Over-current protection
#define MCF8316C_FAULT_OVP           (1u << 26)  ///< Over-voltage protection
#define MCF8316C_FAULT_OTW           (1u << 25)  ///< Over-temperature warning
#define MCF8316C_FAULT_UVLO          (1u << 24)  ///< Under-voltage lockout
// Controller Fault Status register (0x00E2)
#define MCF8316C_FAULT_MOTOR_LOCK    (1u << 27)  ///< Motor lock detected
#define MCF8316C_FAULT_STARTUP_FAIL  (1u << 26)  ///< Startup failure
#define MCF8316C_FAULT_ANY           (MCF8316C_FAULT_OCP | MCF8316C_FAULT_OVP | \
                                      MCF8316C_FAULT_OTW | MCF8316C_FAULT_UVLO)

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
     * @param i2cAddr  7-bit I2C address of the device (default 0x01)
     * @return true on success (device acknowledged), false otherwise
     */
    bool begin(TwoWire& wire = Wire, uint8_t i2cAddr = MCF8316C_DEFAULT_ADDR);

    // -----------------------------------------------------------------------
    // Low-level register access
    // -----------------------------------------------------------------------

    /**
     * Read a 32-bit register.
     *
     * @param regAddr  12-bit register address (MEM_ADDR field)
     * @param value    Receives the 32-bit value
     * @return true on success
     */
    bool readReg(uint16_t regAddr, uint32_t& value);

    /**
     * Write a 32-bit register.
     *
     * @param regAddr  12-bit register address (MEM_ADDR field)
     * @param value    32-bit value to write
     * @return true on success
     */
    bool writeReg(uint16_t regAddr, uint32_t value);

    /**
     * Read-modify-write a register (set bits in mask to values).
     *
     * @param regAddr  12-bit register address
     * @param mask     Bits to modify
     * @param values   New bit values (only bits in mask are applied)
     * @return true on success
     */
    bool modifyReg(uint16_t regAddr, uint32_t mask, uint32_t values);

    // -----------------------------------------------------------------------
    // Motor control
    // -----------------------------------------------------------------------

    /**
     * Apply safe startup defaults via I2C shadow registers.
     * Call once after begin() before enabling the driver.
     * Configures torque (current) mode to avoid the MAX_SPEED=0 default problem.
     *
     * @return true on success
     */
    bool basicConfigure();

    /**
     * Enable the motor driver.
     * On hardware where DRVOFF is hard-wired LOW, this is effectively a
     * preventive CLR_FLT to ensure a clean start state.
     * @return true on success
     */
    bool enableDriver();

    /**
     * Disable the motor driver (set speed reference to 0 via I2C;
     * no dedicated I2C disable register exists when DRVOFF is hard-wired).
     * @return true
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
     * Uses CLOSED_LOOP3[11:0] SPEED_REF in I2C speed mode (SPEED_MODE = 10b).
     *
     * @param pct  0.0 … 100.0  (clamped to valid range)
     * @return true on success
     */
    bool setSpeedPercent(float pct);

    /**
     * Enable or disable the brake function.
     *
     * @param on  true = brake active, false = brake released
     * @return true
     */
    bool setBrake(bool on);

    // -----------------------------------------------------------------------
    // Fault management
    // -----------------------------------------------------------------------

    /**
     * Read the Gate Driver fault status register (0x00E0).
     *
     * @param faultBits  Receives the raw 32-bit fault status word
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
     * Clear latched faults by writing CLR_FLT (bit 29) in ALGO_CTRL1.
     * Blocks for up to 200 ms (per datasheet) to allow the chip to process
     * the fault clear before the next operation.
     * @return true on success
     */
    bool clearFaults();

    // -----------------------------------------------------------------------
    // Diagnostics
    // -----------------------------------------------------------------------

    /**
     * Read the Gate Driver Fault Status register and print it over Serial.
     * Useful during first bring-up to confirm I2C communication.
     */
    void printDeviceId();

private:
    TwoWire* _wire;
    uint8_t  _addr;

    // Build the 24-bit MCF8316C control word
    static uint32_t buildControlWord(bool isRead, uint16_t addr);
};
