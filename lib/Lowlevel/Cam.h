#pragma once
#include <Arduino.h>

// Data provided by a goal-facing camera.
// In the original Defender design this is filled by a BACK camera that
// looks toward the own goal.  The DefenderAdapter synthesises it from the
// single FORWARD camera that looks toward the opponent goal.
struct CamData {
    float GoalAgnle          = 0.0f;   // camera-relative angle to goal (degrees, -180..+180)
    bool  BlueGoal_visible   = false;  // blue goal detected
    bool  YellowGoal_visible = false;  // yellow goal detected
    int   GoalHeight         = 0;      // goal-blob height in pixels (distance indicator)
};

class CamC {
public:
  void init(HardwareSerial& ser = Serial1, uint32_t baud = 115200);
  void readOpenMV();
  void Update();

  // Ergebnis: Tor-Bearing im Feld-KS (Grad, -180..180)
  float give_Angle() const { return angle_field_deg; }

  // Debug/Zusatz
  bool isValid(uint32_t maxAgeMs = 200) const;
  float give_Angle_Cam() const { return angle_cam_deg; }  // relativ zur Kamera
  int   give_BlobH() const { return blob_h_px; }

  // falls Vorzeichen nicht passt, hier umstellen:
  void setSign(bool usePlus) { usePlusSign = usePlus; } // true: field = cam + imu, false: field = imu - cam

private:
  HardwareSerial* port = nullptr;

  static constexpr uint16_t LINE_BUF_SIZE = 96;
  char lineBuf[LINE_BUF_SIZE];
  uint16_t lineLen = 0;

  bool rawValid = false;
  uint32_t lastCamUpdateMs = 0;

  char goal = 'N';
  float angle_cam_deg = 0.0f;
  int blob_h_px = 0;

  float angle_field_deg = 0.0f;

  bool usePlusSign = true; // Standard: field = cam + imu

  void handleLine(const char* line);
  static float wrap180(float a);
};

extern CamC Cam;