#include "US.h"
#include "RGB.h"
#include "SRF08.h"
#include "BNO055.h"

elapsedMillis US_Timer ;
USC US;

SRF08Sensor sensorH(SRF08_ADDR(0xE2), SRF08_RANGE_1M, SRF08_GAIN_MID);
SRF08Sensor sensorV(SRF08_ADDR(0xE8), SRF08_RANGE_1M, SRF08_GAIN_MID);
SRF08Sensor sensorL(SRF08_ADDR(0xE4), SRF08_RANGE_1M, SRF08_GAIN_MID);
SRF08Sensor sensorR(SRF08_ADDR(0xE6), SRF08_RANGE_1M, SRF08_GAIN_MID);

//SRF08Manager sonar;

void USC::init(){
    Wire1.setClock(I2C_SPEED_US);
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

    if (!sensorH.begin(Wire1)) {
        Serial.println("SensorH nicht gefunden!");
        while (true) {}
    }
    if (!sensorV.begin(Wire1)) {
        Serial.println("SensorV nicht gefunden!");
        while (true) {}
    }
    if (!sensorR.begin(Wire1)) {
        Serial.println("SensorR nicht gefunden!");
        while (true) {}
    }
    if (!sensorL.begin(Wire1)) {
        Serial.println("SensorL nicht gefunden!");
        while (true) {}
    }

    // Sensoren zum Manager hinzufügen
    //sonar.addSensor(&sensorV);
    //sonar.addSensor(&sensorR);
    //sonar.addSensor(&sensorH);
    //sonar.addSensor(&sensorL);

    // Initialisieren — begin() testet Erreichbarkeit
    //sonar.begin(Wire1);

    sensorH.startRanging();  // Erste Messung starten
    sensorV.startRanging();  // Erste Messung starten
    sensorR.startRanging();  // Erste Messung starten
    sensorL.startRanging();  // Erste Messung starten

    Wire1.setClock(I2C_SPEED);
}

void USC::read(){
    Wire1.setClock(I2C_SPEED_US);

    
    
    
    
    //if(abs(BNO055.giveDeg())<7){
    if(sensorH.update()){
        Distance[2] = sensorH.getDistance();
        sensorH.startRanging();
    }
    if(sensorV.update()){
        Distance[0] = sensorV.getDistance()+30;
        sensorV.startRanging();
    }
    if(sensorR.update()){  
        Distance[1] = sensorR.getDistance()+30;
        sensorR.startRanging();
    }
    if(sensorL.update()){
        Distance[3] = sensorL.getDistance();
        sensorL.startRanging();
    }
        
        /*Distance[0]=sonar.getDistance(0)+30;
        Distance[1]=sonar.getDistance(1)+30;
        Distance[2]=sonar.getDistance(2);
        Distance[3]=sonar.getDistance(3);*/
    //}
    //Distance[3]=sonar.getDistance(3);
    Wire1.setClock(I2C_SPEED);
}

int USC::giveNR(int NR){
    return Distance[NR];
}



