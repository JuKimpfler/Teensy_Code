# MCF8316C — Arduino/Teensy Library

A minimal, beginner-friendly library to configure and run a
**Texas Instruments MCF8316C-Q1** sensorless BLDC motor driver over I²C,
targeting the **Teensy 4.0** (Arduino/PlatformIO framework).

---

## ⚠️ Important Disclaimer

The register addresses and bit-field definitions in this library are based on
the MCF8316C-Q1 datasheet (**SLVSEO5, rev. A**).  
**Always verify every constant marked `// VERIFY` against your exact silicon
revision and the TI EVM GUI register export before deploying to production.**  
Using incorrect motor parameters can prevent the motor from starting or cause
hardware damage.

---

## Wiring Assumptions

| MCF8316C-Q1 pin | Teensy 4.0 pin | Notes |
|-----------------|---------------|-------|
| SDA             | 18 (Wire)     | I²C data; add 4.7 kΩ pull-up to 3.3 V |
| SCL             | 19 (Wire)     | I²C clock; add 4.7 kΩ pull-up to 3.3 V |
| SPEED           | 3 (PWM)       | PWM signal 0–100 %; frequency ≥ 1 kHz |
| DIR             | 4             | HIGH = CW, LOW = CCW |
| BRAKE           | 5             | HIGH = active brake, LOW = release |
| DRVOFF          | 6             | LOW = driver on, HIGH = driver off |
| FAULT           | 7 (INPUT_PULLUP) | Active-LOW fault indicator from IC |
| FG              | optional      | Motor speed feedback (frequency output) |

> Pin numbers can be changed at the top of `BasicSpin.ino`.

---

## I²C Address

The 7-bit I²C base address is **0x60**.  The two LSBs are set by the `ADDR`
pin on the MCF8316C-Q1:

| ADDR pin  | I²C address |
|-----------|-------------|
| GND       | 0x60        |
| VCC       | 0x61        |

Change `DRIVER_ADDR` in the example sketch and pass the correct value to
`driver.begin(Wire, addr)`.

---

## Quick Start

```cpp
#include <Wire.h>
#include "MCF8316C.h"

MCF8316C driver;

void setup() {
    Wire.begin();
    Wire.setClock(400000);

    if (!driver.begin(Wire, 0x60)) {
        // handle error
    }
    driver.basicConfigure();
    driver.setDirection(true);   // CW
    driver.enableDriver();

    // Ramp to 50 %
    for (float pct = 0; pct <= 50; pct += 5) {
        driver.setSpeedPercent(pct);
        delay(500);
    }
}

void loop() {
    if (driver.hasFault()) {
        driver.clearFaults();
    }
}
```

---

## API Reference

| Method | Description |
|--------|-------------|
| `begin(wire, addr)` | Initialise I²C, verify connectivity |
| `basicConfigure()` | Apply safe startup defaults via registers |
| `enableDriver()` | Clear DRVOFF bit (driver on) |
| `disableDriver()` | Set DRVOFF bit (driver off) |
| `setDirection(cw)` | true = CW, false = CCW |
| `setSpeedPercent(pct)` | 0.0 … 100.0 % target speed |
| `setBrake(on)` | Enable / disable active braking |
| `readFault(bits)` | Read raw 32-bit fault status register |
| `hasFault()` | true if any fault bit is set |
| `clearFaults()` | Write CLR_FLT bit to clear latched faults |
| `readReg(addr, val)` | Low-level 32-bit register read |
| `writeReg(addr, val)` | Low-level 32-bit register write |
| `modifyReg(addr, mask, val)` | Read-modify-write a register |
| `printDeviceId()` | Print device-ID register over Serial |

---

## Adjusting Motor Parameters

Open `src/MCF8316C.cpp` → `basicConfigure()` and update the following values
to match your motor's datasheet:

- **Align current** (startup torque) — too high causes OCP; too low causes startup failure
- **Align time** — time for rotor alignment before open-loop spin-up
- **Acceleration rate** — ramp speed from open-loop to closed-loop
- **Hand-off speed** — minimum RPM at which BEMF sensing becomes reliable
- **Motor parameters** (ALGO_CTRL6–8) — Kv, pole pairs, winding R/L

Use the **TI EVM GUI** to configure and export register settings, then copy the
resulting values into the `basicConfigure()` function.

---

## File Structure

```
lib/MCF8316C/
├── src/
│   ├── MCF8316C.h        ← Register map + class declaration
│   └── MCF8316C.cpp      ← Implementation
├── examples/
│   └── BasicSpin/
│       └── BasicSpin.ino ← First bring-up example
├── keywords.txt
├── library.properties
└── README.md             ← This file
```
