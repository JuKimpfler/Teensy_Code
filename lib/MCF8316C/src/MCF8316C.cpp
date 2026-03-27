/**
 * MCF8316C.cpp
 *
 * Implementation of the MCF8316C library.
 * See MCF8316C.h for full documentation.
 *
 * I2C register transaction format (MCF8316C-Q1, datasheet SLLSFV2 + SLLA662):
 *
 *   Write:
 *     START | [ADDR+W] | CW[23:16] | CW[15:8] | CW[7:0]
 *           | DB0(LSB) | DB1 | DB2 | DB3(MSB) | STOP
 *
 *   Read:
 *     START | [ADDR+W] | CW[23:16] | CW[15:8] | CW[7:0] (OP_R/W=1)
 *           | REPEATED-START | [ADDR+R]
 *           | DB0(LSB) | DB1 | DB2 | DB3(MSB) | STOP
 *
 * Control Word (24-bit, sent MSB first):
 *   Bit [23]     OP_R/W  : 0 = Write, 1 = Read
 *   Bit [22]     CRC_EN  : 0 = disabled
 *   Bits [21:20] DLEN    : 01b = 32-bit data
 *   Bits [19:16] MEM_SEC : 0x0
 *   Bits [15:12] MEM_PAGE: 0x0
 *   Bits [11:0]  MEM_ADDR: 12-bit register address
 *
 * Data bytes are always LSB-first (DB0 = bits [7:0], DB3 = bits [31:24]).
 */

#include "MCF8316C.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
MCF8316C::MCF8316C()
    : _wire(nullptr), _addr(MCF8316C_DEFAULT_ADDR)
{}

// ---------------------------------------------------------------------------
// buildControlWord  (private static helper)
// ---------------------------------------------------------------------------
uint32_t MCF8316C::buildControlWord(bool isRead, uint16_t addr)
{
    uint32_t cw = 0;
    cw |= (isRead ? 1UL : 0UL) << 23;  // OP_R/W
    // Bit 22: CRC_EN = 0 (CRC disabled)
    cw |= 0x01UL << 20;                 // DLEN = 01b (32-bit data)
    // Bits [19:16]: MEM_SEC = 0x0
    // Bits [15:12]: MEM_PAGE = 0x0
    cw |= static_cast<uint32_t>(addr & 0x0FFFu);  // MEM_ADDR (12-bit)
    return cw;
}

// ---------------------------------------------------------------------------
// begin
// ---------------------------------------------------------------------------
bool MCF8316C::begin(TwoWire& wire, uint8_t i2cAddr)
{
    _wire = &wire;
    _addr = i2cAddr;

    // Connectivity check: read Gate Driver Fault Status register (0x00E0).
    // This register is always present and readable on a powered device.
    uint32_t dummy = 0;
    return readReg(MCF8316C_REG_GD_FAULT_STATUS, dummy);
}

// ---------------------------------------------------------------------------
// Low-level I2C helpers — MCF8316C-Q1 proprietary protocol
// ---------------------------------------------------------------------------
bool MCF8316C::readReg(uint16_t regAddr, uint32_t& value)
{
    if (!_wire) return false;

    uint32_t cw = buildControlWord(true, regAddr);  // OP_R/W = 1

    // Phase 1: Send control word via write transaction, NO STOP (repeated start)
    _wire->beginTransmission(_addr);
    _wire->write(static_cast<uint8_t>((cw >> 16) & 0xFF));  // CW[23:16]
    _wire->write(static_cast<uint8_t>((cw >>  8) & 0xFF));  // CW[15:8]
    _wire->write(static_cast<uint8_t>( cw        & 0xFF));  // CW[7:0]
    if (_wire->endTransmission(false) != 0) {  // false = repeated start, no STOP
        Serial.println("fail to connect");
        return false;
    }

    // Phase 2: Read 4 data bytes (LSB first per MCF8316C protocol)
    if (_wire->requestFrom(_addr, static_cast<uint8_t>(4u)) != 4) {
        Serial.println("fail to receive");
        return false;
    }

    // Reconstruct 32-bit value: DB0 = LSB, DB3 = MSB
    uint32_t b0 = _wire->read();  // DB0 (bits  [7:0])
    uint32_t b1 = _wire->read();  // DB1 (bits [15:8])
    uint32_t b2 = _wire->read();  // DB2 (bits [23:16])
    uint32_t b3 = _wire->read();  // DB3 (bits [31:24])
    value = (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
    return true;
}

bool MCF8316C::writeReg(uint16_t regAddr, uint32_t value)
{
    if (!_wire) return false;

    uint32_t cw = buildControlWord(false, regAddr);  // OP_R/W = 0

    _wire->beginTransmission(_addr);
    // Control word — 3 bytes, MSB first
    _wire->write(static_cast<uint8_t>((cw >> 16) & 0xFF));  // CW[23:16]
    _wire->write(static_cast<uint8_t>((cw >>  8) & 0xFF));  // CW[15:8]
    _wire->write(static_cast<uint8_t>( cw        & 0xFF));  // CW[7:0]
    // Data — 4 bytes, LSB first (DB0…DB3)
    _wire->write(static_cast<uint8_t>( value        & 0xFF));  // DB0 (LSB)
    _wire->write(static_cast<uint8_t>((value >>  8) & 0xFF));  // DB1
    _wire->write(static_cast<uint8_t>((value >> 16) & 0xFF));  // DB2
    _wire->write(static_cast<uint8_t>((value >> 24) & 0xFF));  // DB3 (MSB)

    return (_wire->endTransmission(true) == 0);
}

bool MCF8316C::modifyReg(uint16_t regAddr, uint32_t mask, uint32_t values)
{
    uint32_t current = 0;
    if (!readReg(regAddr, current)) return false;
    current = (current & ~mask) | (values & mask);
    return writeReg(regAddr, current);
}

// ---------------------------------------------------------------------------
// basicConfigure
//
// Applies safe startup defaults to the MCF8316C-Q1 shadow registers.
// Writes to shadow registers take effect immediately in RAM; the EEPROM is
// NOT modified (no EEPROM_WRT trigger), so settings are valid only for this
// power-cycle.
//
// Key design choice — Torque (current) mode instead of speed mode:
//   The EEPROM default for MAX_SPEED (CLOSED_LOOP4) is 0.
//   In speed mode (SPEED_LOOP_DIS = 0): SPEED_REF = Duty × MAX_SPEED = 0,
//   so the motor never starts even with PWM applied.
//   In torque mode (SPEED_LOOP_DIS = 1): CURRENT_REF = Duty × ILIMIT,
//   which works without motor-specific parameters.
// ---------------------------------------------------------------------------
bool MCF8316C::basicConfigure()
{
    bool ok = true;

    // --- MOTOR_STARTUP1 (0x0084): align ramp rate, time and current ----------
    // Default (0,0,0) = 0.1 A/s ramp × 10 ms = ~0.001 A → not enough to align.
    // Set: 100 A/s ramp, 400 ms align time, 1.0 A align current.
    //   Bits [28:25] ALIGN_SLOW_RAMP_RATE = 7 → 100 A/s
    //   Bits [24:21] ALIGN_TIME           = 5 → 400 ms
    //   Bits [20:17] ALIGN_ILIMIT         = 3 → 1.0 A
    ok &= writeReg(MCF8316C_REG_MOTOR_STARTUP1,
                   (7UL << 25) | (5UL << 21) | (3UL << 17));  // 0x0EA60000

    // --- MOTOR_STARTUP2 (0x0086): open-loop parameters -----------------------
    // 2 A open-loop current limit, 10 Hz/s linear accel, auto BEMF hand-off.
    //   Bits [30:27] OL_ILIMIT         = 5 → 2.0 A
    //   Bits [26:23] OL_ACC_A1         = 5 → 10 Hz/s
    //   Bits [22:19] OL_ACC_A2         = 4 → 5 Hz/s²
    //   Bit  [18]    AUTO_HANDOFF_EN   = 1 → automatic OL→CL transition
    //   Bits [2:0]   THETA_ERROR_RAMP_RATE = 5 → 0.5 °/ms
    ok &= writeReg(MCF8316C_REG_MOTOR_STARTUP2,
                   (5UL << 27) | (5UL << 23) | (4UL << 19) | (1UL << 18) | 5UL);  // 0x2AA40005

    // --- CLOSED_LOOP1 (0x0088): torque (current) mode -----------------------
    // SPEED_LOOP_DIS = 1: disable speed loop → CURRENT_REF = Duty × ILIMIT.
    // This avoids the MAX_SPEED = 0 default problem.
    ok &= writeReg(MCF8316C_REG_CLOSED_LOOP1, MCF8316C_CL1_SPEED_LOOP_DIS);  // 0x00000002

    // --- FAULT_CONFIG1 (0x0090): raise current limits ------------------------
    // Default HW_LOCK_ILIMIT = 0.125 A would immediately trip the over-current
    // fault in any real motor.  Set ILIMIT and all lock thresholds to 3.0 A
    // and disable the lock-detection modes.
    //   Bits [30:27] ILIMIT           = 7 → 3.0 A
    //   Bits [26:23] HW_LOCK_ILIMIT   = 7 → 3.0 A
    //   Bits [22:19] LOCK_ILIMIT      = 7 → 3.0 A
    //   Bits [18:15] LOCK_ILIMIT_MODE = 9 → lock detection disabled
    ok &= writeReg(MCF8316C_REG_FAULT_CONFIG1,
                   (7UL << 27) | (7UL << 23) | (7UL << 19) | (9UL << 15));  // 0x3BBC8000

    // --- FAULT_CONFIG2 (0x0092): disable HW-lock comparator -----------------
    // Default HW_LOCK_ILIMIT_MODE = 0 (latched fault, no deglitch) would trip
    // immediately at > 0.125 A.
    //   Bits [18:15] HW_LOCK_ILIMIT_MODE = 9 → disabled
    ok &= writeReg(MCF8316C_REG_FAULT_CONFIG2, (9UL << 15));  // 0x00048000

    // --- PIN_CONFIG1 (0x00A4): confirm PWM speed mode -----------------------
    // SPEED_MODE = 00b: duty cycle on SPEED pin controls current reference.
    // 00b is the EEPROM default; writing it explicitly guards against any
    // prior misconfiguration.
    ok &= writeReg(MCF8316C_REG_PIN_CONFIG1, MCF8316C_SPEED_MODE_PWM);  // 0x00000000

    // --- Clear any latched faults from previous run -------------------------
    clearFaults();

    return ok;
}

// ---------------------------------------------------------------------------
// enableDriver
// ---------------------------------------------------------------------------
bool MCF8316C::enableDriver()
{
    // DRVOFF is hard-wired on this hardware, so there is no I2C enable bit.
    // Send a preventive CLR_FLT to ensure all latched faults are cleared
    // before the motor is commanded to spin.
    return clearFaults();
}

bool MCF8316C::disableDriver()
{
    // DRVOFF is hard-wired; motor is stopped externally by setting PWM to 0.
    return true;
}

// ---------------------------------------------------------------------------
// setDirection
// ---------------------------------------------------------------------------
bool MCF8316C::setDirection(bool cw)
{
    // DIR is hard-wired on this hardware.  No I2C action required.
    (void)cw;
    return true;
}

// ---------------------------------------------------------------------------
// setSpeedPercent
// ---------------------------------------------------------------------------
bool MCF8316C::setSpeedPercent(float pct)
{
    // In PWM speed mode (SPEED_MODE = 00b, the default) the MCF8316C-Q1
    // derives its speed/current reference directly from the duty cycle on the
    // hardware SPEED pin.  Use analogWrite() on the SPEED pin to set speed;
    // no I2C register write is needed or effective in this mode.
    (void)pct;
    return true;
}

// ---------------------------------------------------------------------------
// setBrake
// ---------------------------------------------------------------------------
bool MCF8316C::setBrake(bool on)
{
    // BRAKE is hard-wired on this hardware.  No I2C action required.
    (void)on;
    return true;
}

// ---------------------------------------------------------------------------
// readFault / hasFault / clearFaults
// ---------------------------------------------------------------------------
bool MCF8316C::readFault(uint32_t& faultBits)
{
    return readReg(MCF8316C_REG_GD_FAULT_STATUS, faultBits);
}

bool MCF8316C::hasFault(bool* commError)
{
    uint32_t faults = 0;
    if (!readFault(faults)) {
        if (commError) *commError = true;
        return true;  // conservative: treat I2C failure as fault condition
    }
    if (commError) *commError = false;
    return (faults & MCF8316C_FAULT_ANY) != 0u;
}

bool MCF8316C::clearFaults()
{
    // Write CLR_FLT (bit 29) to ALGO_CTRL1 (0x00EA).
    // The datasheet recommends waiting up to 200 ms after writing CLR_FLT.
    bool ok = writeReg(MCF8316C_REG_ALGO_CTRL1, MCF8316C_ALGO1_CLR_FLT);
    delay(200);
    return ok;
}

// ---------------------------------------------------------------------------
// printDeviceId
// ---------------------------------------------------------------------------
void MCF8316C::printDeviceId()
{
    // The MCF8316C-Q1 does not expose a dedicated "Device ID" register in the
    // accessible address range.  Read the Gate Driver Fault Status register
    // instead to confirm I2C communication is working.
    uint32_t status = 0;
    if (readReg(MCF8316C_REG_GD_FAULT_STATUS, status)) {
        Serial.print(F("[MCF8316C] GD Fault Status (0x0E0): 0x"));
        Serial.println(static_cast<unsigned long>(status), HEX);
    } else {
        Serial.println(F("[MCF8316C] ERROR: could not read GD Fault Status register"));
    }
}
