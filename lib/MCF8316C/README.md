# MCF8316C — Arduino/Teensy Library

A minimal, beginner-friendly library to configure and run a
**Texas Instruments MCF8316C-Q1** sensorless BLDC motor driver over I²C,
targeting the **Teensy 4.0** (Arduino/PlatformIO framework).

---

## I²C Protocol

The MCF8316C-Q1 uses a **proprietary I²C protocol** — it is **not** a standard
2-byte register address device.  Every transaction begins with a 24-bit
**Control Word** (3 bytes, MSB first), followed by 32-bit data (LSB first):

```
Write: START | [ADDR+W] | CW[23:16] | CW[15:8] | CW[7:0] | DB0(LSB) | DB1 | DB2 | DB3(MSB) | STOP
Read : START | [ADDR+W] | CW[23:16] | CW[15:8] | CW[7:0] | RSTART | [ADDR+R] | DB0 | DB1 | DB2 | DB3 | STOP
```

Control Word bit layout:

| Bits  | Field    | Description                          |
|-------|----------|--------------------------------------|
| [23]  | OP_R/W   | 0 = Write, 1 = Read                  |
| [22]  | CRC_EN   | 0 = CRC disabled                     |
| [21:20]| DLEN    | 01b = 32-bit data (always used here) |
| [19:16]| MEM_SEC | 0x0                                  |
| [15:12]| MEM_PAGE| 0x0                                  |
| [11:0] | MEM_ADDR| 12-bit register address              |

---

## Wiring Assumptions

This hardware uses **Wire1** (Teensy 4.0 pins 17/16) with DIR/BRAKE/DRVOFF
hard-wired and only the SPEED/WAKE pin driven from PWM:

| MCF8316C-Q1 pin | Teensy 4.0 pin | Notes |
|-----------------|----------------|-------|
| SDA             | 17 (Wire1)     | I²C data; add 4.7 kΩ pull-up to 3.3 V |
| SCL             | 16 (Wire1)     | I²C clock; add 4.7 kΩ pull-up to 3.3 V |
| SPEED/WAKE      | 33 (PWM)       | PWM signal 0–100 %; 20 kHz recommended |
| DIR             | GND (hard-wired) | Fixed direction |
| BRAKE           | GND (hard-wired) | Brake released |
| DRVOFF          | GND (hard-wired) | Driver always enabled |
| FAULT           | LED (hard-wired) | Active-LOW fault indicator |

---

## I²C Address

The MCF8316C-Q1 default 7-bit I²C address is **0x01** (ADDR pin = GND).

Change `DRIVER_ADDR` in the example sketch and pass the correct value to
`driver.begin(Wire1, addr)`.

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
