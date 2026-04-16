#include "PowerPump.h"

PowerPumpC PowerPump;
/*
void PowerPumpC::init() {
    pinMode(Schuss_Port, OUTPUT);
    digitalWrite(Schuss_Port, LOW);
}

bool PowerPumpC::kickerReady() const {
    return (millis() - lastShootTime) >= CHARGE_TIME_MS;
}

bool PowerPumpC::shoot() {
    if (!kickerReady()) {
        return false;
    }

    digitalWrite(Schuss_Port, HIGH);
    delayMicroseconds(SHOOT_PULSE_US);
    digitalWrite(Schuss_Port, LOW);
    lastShootTime = millis();
    return true;
}*/
