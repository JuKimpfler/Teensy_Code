#include "Cam.h"
#include "BNO055.h"
CamC Cam;
GoalC Goal;

void CamC::readSerialFromOpenMV() {
  while (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');
    if (data.length() > 4) {
      last_goal_color = data.charAt(0);
      int commaIndex = data.indexOf(',', 2);
      if (commaIndex > 0) {
        last_blob_cx = data.substring(2, commaIndex).toFloat();
        last_blob_w = data.substring(commaIndex + 1).toFloat();
        last_cam_update = millis(); 
      }
    }
  }
}

bool CamC::readCameraAbsolute(float &camX, float &camY) {
  readSerialFromOpenMV();
  
  if (millis() - last_cam_update > 150 || last_blob_w < 5.0) {
    return false; // Keine aktuellen/gültigen Daten
  }
  
  float Z_cam = (GOAL_WIDTH_CM * FOCAL_LENGTH_PX) / last_blob_w;
  float X_cam = ((last_blob_cx - CAM_CENTER_X) * Z_cam) / FOCAL_LENGTH_PX;
  float fieldOffsetX, fieldOffsetY;
  
  if (last_goal_color == 'B') { 
    fieldOffsetX = (X_cam * cos(BNO055.giveRad())) - (Z_cam * sin(BNO055.giveRad()));
    fieldOffsetY = (X_cam * sin(BNO055.giveRad())) + (Z_cam * cos(BNO055.giveRad()));
    camX = BLUE_GOAL_X - fieldOffsetX;
    camY = BLUE_GOAL_Y - fieldOffsetY;
    return true;
  } 
  else if (last_goal_color == 'Y') { 
    float Z_cam_inv = -Z_cam;
    float X_cam_inv = -X_cam;
    fieldOffsetX = (X_cam_inv * cos(BNO055.giveRad())) - (Z_cam_inv * sin(BNO055.giveRad()));
    fieldOffsetY = (X_cam_inv * sin(BNO055.giveRad())) + (Z_cam_inv * cos(BNO055.giveRad()));
    camX = YELLOW_GOAL_X - fieldOffsetX;
    camY = YELLOW_GOAL_Y - fieldOffsetY;
    return true;
  }
  return false;
}