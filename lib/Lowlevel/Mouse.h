#pragma once
#include "Elementar.h"
#include "pmw3389.h"

class MouseC{
    private:
    public:
        float deltaX; // X Geschwindigkeit
        float deltaY; // Y Geschwindigkeit
        float xPos = 0; // X Position
        float yPos = 0; // Y Position
        float delta_dist = 0; // Diagonale Geschwindigkeit
        bool lift; // Angehoben ?
        bool movement; // Bewegung ?

        int pmwId; // Sensor Id

        void init();
        void read();
};

extern PMW3389 pmw; // SPI CS vom Moussensor kommt auf PixyTx

extern MouseC Mouse;