/**
 * BasicSpin.ino
 *
 * MCF8316C-Q1 — first bring-up example for Teensy 4.0
 *
 * Wiring (adjust pin numbers to your board):
 *   I2C SDA   -> Teensy pin 18  (Wire default)
 *   I2C SCL   -> Teensy pin 19  (Wire default)
 *   SPEED     -> Teensy pin  3  (PWM capable)
 *   DIR       -> Teensy pin  4
 *   BRAKE     -> Teensy pin  5  (optional; tie to GND if not used)
 *   DRVOFF    -> Teensy pin  6  (optional; tie to GND to always enable)
 *   FAULT     -> Teensy pin  7  (optional; INPUT_PULLUP)
 *
 * This sketch:
 *   1. Initialises I2C and checks device communication
 *   2. Applies basic safe startup configuration via I2C
 *   3. Sets direction and gradually ramps speed 0 -> 80 %
 *   4. Polls the fault register every second and prints status to Serial
 *
 * NOTE: Adjust MCF8316C register values in basicConfigure() to match
 *       your specific motor (Kv, pole pairs, winding resistance, etc.).
 *       Using wrong parameters may prevent the motor from starting or
 *       cause over-current trips.
 */

#include <Wire.h>
#include "MCF8316C.h"

// ---------------------------------------------------------------------------
// Pin definitions — change these to match your wiring
// ---------------------------------------------------------------------------
static constexpr uint8_t PIN_SPEED  = 3;   // PWM output  -> SPEED
static constexpr uint8_t PIN_DIR    = 4;   // digital     -> DIR
static constexpr uint8_t PIN_BRAKE  = 5;   // digital     -> BRAKE  (active-HIGH on this board)
static constexpr uint8_t PIN_DRVOFF = 6;   // digital     -> DRVOFF (active-HIGH = off)
static constexpr uint8_t PIN_FAULT  = 7;   // digital input <- FAULT (active-LOW)

// I2C address — match jumper/ADDR pin on your board
static constexpr uint8_t DRIVER_ADDR = MCF8316C_DEFAULT_ADDR;

// ---------------------------------------------------------------------------
// Globals
// ---------------------------------------------------------------------------
MCF8316C driver;

// ---------------------------------------------------------------------------
// setup
// ---------------------------------------------------------------------------
void setup()
{
    Serial.begin(115200);
    while (!Serial && millis() < 3000) { /* wait for USB CDC */ }
    Serial.println(F("=== MCF8316C BasicSpin example ==="));

    // ----- Hardware pin initialisation -----
    pinMode(PIN_DIR,    OUTPUT);
    pinMode(PIN_BRAKE,  OUTPUT);
    pinMode(PIN_DRVOFF, OUTPUT);
    pinMode(PIN_FAULT,  INPUT_PULLUP);

    // Safe initial states BEFORE I2C configuration
    digitalWrite(PIN_DRVOFF, HIGH);   // keep driver OFF while configuring
    digitalWrite(PIN_BRAKE,  LOW);    // brake released
    digitalWrite(PIN_DIR,    HIGH);   // CW

    // SPEED pin: use analogWrite for PWM.  Resolution and frequency can be
    // adjusted below — check MCF8316C-Q1 datasheet for accepted SPEED range.
    analogWriteResolution(8);          // 8-bit (0-255)
    analogWriteFrequency(PIN_SPEED, 20000); // 20 kHz  // VERIFY with datasheet
    analogWrite(PIN_SPEED, 0);         // 0 % speed initially

    // ----- I2C initialisation -----
    Wire.begin();
    Wire.setClock(400000);  // 400 kHz fast mode

    Serial.print(F("Connecting to MCF8316C at I2C address 0x"));
    Serial.println(DRIVER_ADDR, HEX);

    if (!driver.begin(Wire, DRIVER_ADDR)) {
        Serial.println(F("ERROR: MCF8316C not found on I2C bus!"));
        Serial.println(F("Check wiring, I2C address and pull-up resistors."));
        while (true) { delay(1000); }
    }
    Serial.println(F("MCF8316C found."));
    driver.printDeviceId();

    // ----- Basic configuration -----
    Serial.println(F("Applying basic configuration..."));
    if (!driver.basicConfigure()) {
        Serial.println(F("ERROR: basicConfigure() failed — check I2C connection."));
        while (true) { delay(1000); }
    }

    // Check for pre-existing faults
    if (driver.hasFault()) {
        uint32_t faults = 0;
        driver.readFault(faults);
        Serial.print(F("WARNING: Fault bits set after config: 0x"));
        Serial.println(faults, HEX);
        Serial.println(F("Attempting to clear faults..."));
        driver.clearFaults();
        delay(100);
        if (driver.hasFault()) {
            Serial.println(F("Faults persist — check hardware before continuing."));
        } else {
            Serial.println(F("Faults cleared."));
        }
    }

    // ----- Set direction -----
    driver.setDirection(true);         // CW
    digitalWrite(PIN_DIR, HIGH);       // also drive hardware pin

    // ----- Enable the driver via I2C -----
    Serial.println(F("Enabling driver..."));
    driver.enableDriver();
    digitalWrite(PIN_DRVOFF, LOW);     // also drive hardware pin LOW to enable

    delay(200);  // give driver time to power up gate drive

    // ----- Gradual speed ramp from 0 to 80 % -----
    Serial.println(F("Ramping speed 0 -> 80 %..."));
    for (float pct = 0.0f; pct <= 80.0f; pct += 5.0f) {
        driver.setSpeedPercent(pct);

        // Mirror speed to PWM pin (0 % = 0, 100 % = 255 for 8-bit)
        uint8_t pwmVal = static_cast<uint8_t>((pct / 100.0f) * 255.0f + 0.5f);
        analogWrite(PIN_SPEED, pwmVal);

        Serial.print(F("Speed: "));
        Serial.print(static_cast<int>(pct));
        Serial.print(F(" %   PWM: "));
        Serial.println(pwmVal);

        bool commErr = false;
        if (driver.hasFault(&commErr)) {
            if (commErr) {
                Serial.println(F("ERROR: I2C read failed during ramp — check connection."));
            } else {
                uint32_t faults = 0;
                driver.readFault(faults);
                Serial.print(F("FAULT during ramp: 0x"));
                Serial.println(faults, HEX);
            }
            Serial.println(F("Stopping motor."));
            driver.setSpeedPercent(0.0f);
            analogWrite(PIN_SPEED, 0);
            driver.disableDriver();
            while (true) { delay(1000); }
        }
        delay(500);  // 500 ms per step -> ~8 s total ramp
    }

    Serial.println(F("Ramp complete. Motor running at 80 %."));
    Serial.println(F("Polling fault register every 1 s (press reset to restart)..."));
}

// ---------------------------------------------------------------------------
// loop — periodic fault polling
// ---------------------------------------------------------------------------
void loop()
{
    delay(1000);

    uint32_t faults = 0;
    bool readOk = driver.readFault(faults);

    if (!readOk) {
        Serial.println(F("ERROR: I2C read failed — check connection."));
        return;
    }

    if (faults & MCF8316C_FAULT_ANY) {
        Serial.print(F("[FAULT] Status register: 0x"));
        Serial.print(faults, HEX);

        if (faults & MCF8316C_FAULT_OCP)          Serial.print(F("  OCP"));
        if (faults & MCF8316C_FAULT_OVP)          Serial.print(F("  OVP"));
        if (faults & MCF8316C_FAULT_UVLO)         Serial.print(F("  UVLO"));
        if (faults & MCF8316C_FAULT_OTW)          Serial.print(F("  OTW"));
        if (faults & MCF8316C_FAULT_OTS)          Serial.print(F("  OTS"));
        if (faults & MCF8316C_FAULT_MOTOR_LOCK)   Serial.print(F("  MOTOR_LOCK"));
        if (faults & MCF8316C_FAULT_STARTUP_FAIL) Serial.print(F("  STARTUP_FAIL"));
        Serial.println();

        // Attempt automatic fault clear and restart
        driver.clearFaults();
    } else {
        Serial.println(F("[OK] No faults."));
    }

    // Also read the hardware FAULT pin for belt-and-suspenders check
    if (digitalRead(PIN_FAULT) == LOW) {
        Serial.println(F("[HW] FAULT pin is LOW — hardware fault signal active."));
    }
}
