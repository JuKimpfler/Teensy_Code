#include "US.h"
#include "RGB.h"
#include "SRF08.h"
#include "BNO055.h"

elapsedMillis US_Timer ;
USC US;

SRF08Sensor sensorH(SRF08_ADDR(0xE4), SRF08_RANGE_3M, SRF08_GAIN_MID);
SRF08Sensor sensorV(SRF08_ADDR(0xE6), SRF08_RANGE_2M, SRF08_GAIN_MID);
SRF08Sensor sensorL(SRF08_ADDR(0xE0), SRF08_RANGE_3M, SRF08_GAIN_MID);
SRF08Sensor sensorR(SRF08_ADDR(0xE2), SRF08_RANGE_3M, SRF08_GAIN_MID);

SRF08Manager sonar;

void USC::init(){
    //Wire1.setClock(I2C_SPEED_US);
    sensorV.setEMAAlpha(0.25f);
    sensorH.setEMAAlpha(0.25f);
    sensorR.setEMAAlpha(0.25f);
    sensorL.setEMAAlpha(0.25f);

    // Sprungfilter: Max. 50cm Änderung pro Zyklus
    // (Roboter bewegt sich max. ~1.5 m/s × 0.03s ≈ 4.5cm/Zyklus → 50cm sehr konservativ)
    sensorV.setJumpThreshold(50);
    sensorH.setJumpThreshold(50);
    sensorR.setJumpThreshold(50);
    sensorL.setJumpThreshold(50);

    // Sensoren zum Manager hinzufügen
    sonar.addSensor(&sensorR);
    sonar.addSensor(&sensorH);
    sonar.addSensor(&sensorL);
    sonar.addSensor(&sensorL);

    // Initialisieren — begin() testet Erreichbarkeit
    sonar.begin(Wire1);
    //Wire1.setClock(I2C_SPEED);
}

void USC::read(){
    //Wire1.setClock(I2C_SPEED_US);
    if(abs(BNO055.giveDeg())<7){
        sonar.update();
        Distance[1]=sonar.getDistance(0);
        Distance[2]=sonar.getDistance(1);
        Distance[3]=sonar.getDistance(2);
        Distance[4]=sonar.getDistance(3);
    }
    //Distance[3]=sonar.getDistance(3);
    //Wire1.setClock(I2C_SPEED);
}

int USC::giveNR(int NR){
    return Distance[NR];
}



