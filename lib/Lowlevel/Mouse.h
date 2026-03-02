#pragma once
#include "Elementar.h"
#include "pmw3389.h"

class MouseC{
    private:
    public:
        double deltaX; // X Geschwindigkeit
        double deltaY; // Y Geschwindigkeit
        double xPos = 0; // X Position
        double yPos = 0; // Y Position
        double delta_dist = 0; // Diagonale Geschwindigkeit
        double delta_angle = 0;
        double delta_sure = 0;
        bool lift; // Angehoben ?
        bool movement; // Bewegung ?

        int pmwId; // Sensor Id

        void init();
        void read();
};

extern PMW3389 pmw; // SPI CS vom Moussensor kommt auf PixyTx

extern MouseC Mouse;