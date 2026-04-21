#include "PFU.h"

bool readLineSensor(int index) { return (Line.line[index] == 1) ; } // true = Weiß

Kalmanfilter PFU; // PositionFilteringUnit

void Kalmanfilter::begin(){
    unsigned long now = micros();
    last500Hz = now; last20Hz = now; last10Hz = now;
    Mouse.init();
    //Line.init();    
    //US.init();
}

void Kalmanfilter::updateOdometry(float dt) {
  thetaDeg = BNO055.giveDeg();
  thetaRad = BNO055.giveRad();
  
  float vX_local = Mouse.giveVx() * 100.0; 
  float vY_local = Mouse.giveVy() * 100.0; 
  
  float deltaX = (vX_local * cos(thetaRad) + vY_local * sin(thetaRad)) * dt;
  float deltaY = (-vX_local * sin(thetaRad) + vY_local * cos(thetaRad)) * dt;
  
  posX += deltaX;
  posY += deltaY;
}

void Kalmanfilter::checkLineSensors() {
  for (int i = 0; i < NUM_LINE_SENSORS; i++) {
    if (readLineSensor(i)) {
      float sensorX = posX + R_SENSOR * sin(thetaRad + sensorAngles[i]); 
      float sensorY = posY + R_SENSOR * cos(thetaRad + sensorAngles[i]);
      
      float minDistX = 9999.0; float matchedLineX = 0.0;
      for (int k = 0; k < NUM_X_LINES; k++) {
        if (fabs(sensorX - LINES_X[k]) < minDistX) { minDistX = fabs(sensorX - LINES_X[k]); matchedLineX = LINES_X[k]; }
      }
      
      float minDistY = 9999.0; float matchedLineY = 0.0;
      for (int k = 0; k < NUM_Y_LINES; k++) {
        if (fabs(sensorY - LINES_Y[k]) < minDistY) { minDistY = fabs(sensorY - LINES_Y[k]); matchedLineY = LINES_Y[k]; }
      }
      
      if (minDistX < minDistY && minDistX < MAX_DRIFT_TOLERANCE) {
        posX = matchedLineX - (R_SENSOR * sin(thetaRad + sensorAngles[i]));
      } else if (minDistY < minDistX && minDistY < MAX_DRIFT_TOLERANCE) {
        posY = matchedLineY - (R_SENSOR * cos(thetaRad + sensorAngles[i]));
      }
    }
  }
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
  Mouse.read();
  //Cam.readCameraAbsolute(camAbsX, camAbsY);
  US.read();

  unsigned long currentMicros = micros();

  // --- 500 Hz TASK (Prädiktion & Linien-Notfall) ---
  if (currentMicros - last500Hz >= 2000) {
    float dt = (currentMicros - last500Hz) / 1000000.0f; 
    last500Hz = currentMicros;
    
    //updateOdometry(dt); 
    //checkLineSensors(); 
  }

  /*// --- 20 Hz TASK (Kamera) ---
  if (currentMicros - last20Hz >= 50000) {
    last20Hz = currentMicros;
    
    if (Cam.readCameraAbsolute(camAbsX, camAbsY)) {
      // Plausibilität (nicht außerhalb des Feldes interpolieren)
      if(camAbsX > -10.0 && camAbsX < 192.0 && camAbsY > -10.0 && camAbsY < 253.0) {
        posX = posX * (1.0 - K_CAM) + (camAbsX * K_CAM);
        posY = posY * (1.0 - K_CAM) + (camAbsY * K_CAM);
      }
    }
  }*/

  // --- 10 Hz TASK (Ultraschall) ---
  if (currentMicros - last10Hz >= 10000) {
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