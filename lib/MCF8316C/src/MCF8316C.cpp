/**
 * MCF8316C.cpp
 *
 * Implementation of the MCF8316C library.
 * See MCF8316C.h for full documentation.
 *
 * I2C register transaction format (MCF8316C-Q1):
 *   Write: START | ADDR+W | RegHigh | RegLow | Byte3 | Byte2 | Byte1 | Byte0 | STOP
 *   Read : START | ADDR+W | RegHigh | RegLow | REPEATED-START | ADDR+R |
 *          Byte3 | Byte2 | Byte1 | Byte0 | STOP
 * Bytes are MSB-first (big-endian).
 *
 * IMPORTANT: Verify register addresses and bit-field positions against your
 * exact MCF8316C-Q1 datasheet revision before using in production.
 */

#include "MCF8316C.h"

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
MCF8316C::MCF8316C()
    : _wire(nullptr), _addr(MCF8316C_DEFAULT_ADDR), _algo1Cache(0u)
{}

// ---------------------------------------------------------------------------
// begin
// ---------------------------------------------------------------------------
bool MCF8316C::begin(TwoWire& wire, uint8_t i2cAddr)
{
    _wire = &wire;
    _addr = i2cAddr;

    // Quick I2C connectivity check: try to read the device-ID register.
    uint32_t id = 0;
    if (!readReg(MCF8316C_REG_DEVICE_ID, id)) {
        return false;  // No ACK — wrong address or wiring problem
    }

    // Cache ALGO_CTRL1 so we can do read-modify-write cheaply later.
    readReg(MCF8316C_REG_ALGO_CTRL1, _algo1Cache);

    return true;
}

// ---------------------------------------------------------------------------
// Low-level I2C helpers
// ---------------------------------------------------------------------------
bool MCF8316C::readReg(uint16_t regAddr, uint32_t& value)
{
    if (!_wire) return false;

    // Send register address (2 bytes, big-endian)
    _wire->beginTransmission(_addr);
    _wire->write(static_cast<uint8_t>(regAddr >> 8));
    _wire->write(static_cast<uint8_t>(regAddr & 0xFF));
    if (_wire->endTransmission(false) != 0) {  // false = repeated start
        return false;
    }

    // Request 4 bytes (32-bit register, MSB first)
    if (_wire->requestFrom(_addr, static_cast<uint8_t>(4)) != 4) {
        return false;
    }

    uint32_t result = 0;
    result  = static_cast<uint32_t>(_wire->read()) << 24;
    result |= static_cast<uint32_t>(_wire->read()) << 16;
    result |= static_cast<uint32_t>(_wire->read()) << 8;
    result |= static_cast<uint32_t>(_wire->read());

    value = result;
    return true;
}

bool MCF8316C::writeReg(uint16_t regAddr, uint32_t value)
{
    if (!_wire) return false;

    _wire->beginTransmission(_addr);
    _wire->write(static_cast<uint8_t>(regAddr >> 8));
    _wire->write(static_cast<uint8_t>(regAddr & 0xFF));
    _wire->write(static_cast<uint8_t>((value >> 24) & 0xFF));
    _wire->write(static_cast<uint8_t>((value >> 16) & 0xFF));
    _wire->write(static_cast<uint8_t>((value >>  8) & 0xFF));
    _wire->write(static_cast<uint8_t>( value        & 0xFF));

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
// Apply conservative startup defaults.  Tweak the values below to match
// your motor's datasheet (pole pairs, Kv, winding resistance, inductance).
// ---------------------------------------------------------------------------
bool MCF8316C::basicConfigure()
{
    bool ok = true;

    // --- ALGO_CTRL1: direction = CW, brake = off, driver = enabled ----------
    // Start with driver OFF while we configure; enableDriver() is called by
    // the user after basicConfigure().
    _algo1Cache = MCF8316C_ALGO1_DRVOFF_BIT;  // driver off, CW, no brake
    ok &= writeReg(MCF8316C_REG_ALGO_CTRL1, _algo1Cache);

    // --- ALGO_CTRL2: speed = 0 (motor stopped), conservative ramp rate ------
    // Bits [31:22] = speed target (10-bit, 0 = 0 %, 1023 = 100 %)  // VERIFY
    // Bits [21:12] = acceleration rate  // VERIFY
    // A slow ramp helps sensorless start-up on unknown motors.
    uint32_t ctrl2 = 0;
    ctrl2 |= (0u    & 0x3FFu) << 22;  // speed  = 0
    ctrl2 |= (100u  & 0x3FFu) << 12;  // accel  = 100 (moderate)  // VERIFY
    ok &= writeReg(MCF8316C_REG_ALGO_CTRL2, ctrl2);

    // --- ALGO_CTRL3: open-loop startup current and align time ---------------
    // Bits [31:24] = align/startup current setting (0–255 mapped to 0–100 %)
    //               Lower value = less torque but fewer over-current trips.
    // Bits [23:16] = align time in ms (0–255 mapped to 0–500 ms)  // VERIFY
    uint32_t ctrl3 = 0;
    ctrl3 |= (50u  & 0xFFu) << 24;  // align current ~20 %  // VERIFY
    ctrl3 |= (100u & 0xFFu) << 16;  // align time   ~200 ms  // VERIFY
    ok &= writeReg(MCF8316C_REG_ALGO_CTRL3, ctrl3);

    // --- ALGO_CTRL4: open-loop to closed-loop hand-off ----------------------
    // Conservative hand-off speed (higher = easier for sensorless).  // VERIFY
    ok &= writeReg(MCF8316C_REG_ALGO_CTRL4, 0x00000000u);  // VERIFY all bits

    // --- ALGO_CTRL5: closed-loop speed controller gains ---------------------
    ok &= writeReg(MCF8316C_REG_ALGO_CTRL5, 0x00000000u);  // VERIFY

    // --- ALGO_CTRL6 / 7 / 8: motor electrical parameters -------------------
    // These MUST be set to match your specific motor for stable sensorless
    // operation.  The zeros below will use device defaults.  // VERIFY
    ok &= writeReg(MCF8316C_REG_ALGO_CTRL6, 0x00000000u);  // VERIFY
    ok &= writeReg(MCF8316C_REG_ALGO_CTRL7, 0x00000000u);  // VERIFY
    ok &= writeReg(MCF8316C_REG_ALGO_CTRL8, 0x00000000u);  // VERIFY

    // --- SYS_CTRL2: clear any latched faults from previous run --------------
    clearFaults();

    return ok;
}

// ---------------------------------------------------------------------------
// enableDriver / disableDriver
// ---------------------------------------------------------------------------
bool MCF8316C::enableDriver()
{
    _algo1Cache &= ~MCF8316C_ALGO1_DRVOFF_BIT;
    return writeReg(MCF8316C_REG_ALGO_CTRL1, _algo1Cache);
}

bool MCF8316C::disableDriver()
{
    _algo1Cache |= MCF8316C_ALGO1_DRVOFF_BIT;
    return writeReg(MCF8316C_REG_ALGO_CTRL1, _algo1Cache);
}

// ---------------------------------------------------------------------------
// setDirection
// ---------------------------------------------------------------------------
bool MCF8316C::setDirection(bool cw)
{
    if (cw) {
        _algo1Cache |=  MCF8316C_ALGO1_DIR_BIT;
    } else {
        _algo1Cache &= ~MCF8316C_ALGO1_DIR_BIT;
    }
    return writeReg(MCF8316C_REG_ALGO_CTRL1, _algo1Cache);
}

// ---------------------------------------------------------------------------
// setSpeedPercent
// ---------------------------------------------------------------------------
bool MCF8316C::setSpeedPercent(float pct)
{
    // Clamp to [0, 100]
    if (pct < 0.0f)   pct = 0.0f;
    if (pct > 100.0f) pct = 100.0f;

    // Map to 10-bit raw value
    uint32_t rawSpeed = static_cast<uint32_t>((pct / 100.0f) * static_cast<float>(SPEED_MAX_RAW) + 0.5f);
    if (rawSpeed > SPEED_MAX_RAW) rawSpeed = SPEED_MAX_RAW;

    // Read-modify-write the speed field in ALGO_CTRL2
    return modifyReg(MCF8316C_REG_ALGO_CTRL2,
                     SPEED_FIELD_MASK,
                     (rawSpeed << SPEED_FIELD_SHIFT) & SPEED_FIELD_MASK);
}

// ---------------------------------------------------------------------------
// setBrake
// ---------------------------------------------------------------------------
bool MCF8316C::setBrake(bool on)
{
    if (on) {
        _algo1Cache |=  MCF8316C_ALGO1_BRAKE_BIT;
    } else {
        _algo1Cache &= ~MCF8316C_ALGO1_BRAKE_BIT;
    }
    return writeReg(MCF8316C_REG_ALGO_CTRL1, _algo1Cache);
}

// ---------------------------------------------------------------------------
// readFault / hasFault / clearFaults
// ---------------------------------------------------------------------------
bool MCF8316C::readFault(uint32_t& faultBits)
{
    return readReg(MCF8316C_REG_FAULT_STATUS, faultBits);
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
    return modifyReg(MCF8316C_REG_SYS_CTRL1,
                     MCF8316C_SYSCTRL1_CLR_FLT,
                     MCF8316C_SYSCTRL1_CLR_FLT);
}

// ---------------------------------------------------------------------------
// printDeviceId
// ---------------------------------------------------------------------------
void MCF8316C::printDeviceId()
{
    uint32_t id = 0;
    if (readReg(MCF8316C_REG_DEVICE_ID, id)) {
        Serial.print(F("[MCF8316C] Device ID register: 0x"));
        Serial.println(static_cast<unsigned long>(id), HEX);
    } else {
        Serial.println(F("[MCF8316C] ERROR: could not read Device ID register"));
    }
}
