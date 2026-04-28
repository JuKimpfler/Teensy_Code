#include "PFU.h"

bool readLineSensor(int index) { return (Line.line[index] == 1) ; } // true = Weiß

Kalmanfilter PFU; // PositionFilteringUnit

void Kalmanfilter::begin(){
    unsigned long now = micros();
    last500Hz = now; last20Hz = now; last10Hz = now;
}

void Kalmanfilter::updateUltrasonic() {
  float angle360 = fmod(thetaDeg + 360.0, 360.0);
  float cardinalDir = round(angle360 / 90.0) * 90.0;
  if (cardinalDir == 360.0) cardinalDir = 0.0;
  
  float deviationDeg = fabs(angle360 - cardinalDir);
  if (deviationDeg > 45.0) deviationDeg = fabs(deviationDeg - 90.0);
  
  if (deviationDeg <= 15.0) {
    float deviationRad = deviationDeg * (M_PI / 180.0);
    
    for (int sensorDir = 0; sensorDir < 3; sensorDir++) {
      float dist = US.giveNR(sensorDir);
      
      if (dist > 5.0 && dist < 200.0) { 
        float trueDist = dist * cos(deviationRad);
        int absoluteLookingDir = ((int)cardinalDir + (sensorDir * 90)) % 360;
        
        if (absoluteLookingDir == 0) {
          posY = posY * (1.0 - K_US) + ((WALL_Y_TOP - trueDist) * K_US);
        } else if (absoluteLookingDir == 90) {
          posX = posX * (1.0 - K_US) + ((WALL_X_RIGHT - trueDist) * K_US);
        } else if (absoluteLookingDir == 180) {
          posY = posY * (1.0 - K_US) + ((WALL_Y_BOTTOM + trueDist) * K_US);
        } else if (absoluteLookingDir == 270) {
          posX = posX * (1.0 - K_US) + ((WALL_X_LEFT + trueDist) * K_US);
        }
      }
    }
  }
}

void Kalmanfilter::Update() {
  float camAbsX, camAbsY;
  unsigned long currentMicros = micros();

  // --- 20 Hz TASK (Kamera) ---
  if (currentMicros - last20Hz >= 50000) {
    last20Hz = currentMicros;
    
    if (Cam.isValid()) {
      camAbsY = Cam.giveYPos();
      camAbsX = Cam.giveXPos();
      // Plausibilität (nicht außerhalb des Feldes interpolieren)
      if(camAbsX > -10.0 && camAbsX < 192.0 && camAbsY > -10.0 && camAbsY < 253.0) {
        posX = posX * (1.0 - K_CAM) + (camAbsX * K_CAM);
        posY = posY * (1.0 - K_CAM) + (camAbsY * K_CAM);
      }
    }
  }

  if (currentMicros - last10Hz >= 5000) {
    last10Hz = currentMicros;
    updateUltrasonic();
  }

  X=posX;
  Y=posY;
}

int Kalmanfilter::giveX(){return X;}
int Kalmanfilter::giveY(){return Y;}
int Kalmanfilter::giveGAngle(){return GAngle;}
int Kalmanfilter::giveHomeAngle(){return HomeAngle;}
int Kalmanfilter::giveBallX(){return BallX;}
int Kalmanfilter::giveBallY(){return BallY;}