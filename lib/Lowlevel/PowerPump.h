#pragma once
#include "Elementar.h"

class PowerPumpC {
    public:
        void init();
        bool kickerReady() const;
        bool shoot();

    private:
        static constexpr unsigned long CHARGE_TIME_MS = 300;
        static constexpr unsigned int SHOOT_PULSE_US = 800;
        unsigned long lastShootTime = 0;
};

extern PowerPumpC PowerPump;
