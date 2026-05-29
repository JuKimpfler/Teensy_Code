#pragma once
#include <Arduino.h>

class CamC {
public:
  void init(uint32_t baud, bool num1, bool num2, HardwareSerial& ser = Serial6, HardwareSerial& serb = Serial7);
  void readOpenMV();
  void Update();

  // Ergebnis: Tor-Bearing im Feld-KS (Grad, -180..180)
  float give_Angle1() const { return angle_field_deg1; }
  float give_Angle2() const { return angle_field_deg2; }

  // Debug/Zusatz
  bool isValid1(uint32_t maxAgeMs = 200) const;
  bool isValid2(uint32_t maxAgeMs = 200) const;
  float give_Angle_Cam1() const { return angle_cam_deg1; }  // relativ zur Kamera
  float give_Angle_Cam2() const { return angle_cam_deg2; }  // relativ zur Kamera
  int   give_BlobH1() const { return blob_h_px1; }
  int   give_BlobH2() const { return blob_h_px2; }

  // falls Vorzeichen nicht passt, hier umstellen:
  void setSign1(bool usePlus) { usePlusSign1 = usePlus; } // true: field = cam + imu, false: field = imu - cam
  void setSign2(bool usePlus) { usePlusSign2 = usePlus; } // true: field = cam + imu, false: field = imu - cam

private:
  HardwareSerial* port1 = nullptr;
  HardwareSerial* port2 = nullptr;

  static constexpr uint16_t LINE_BUF_SIZE = 96;
  char lineBuf[LINE_BUF_SIZE];
  uint16_t lineLen = 0;

  bool rawValid1 = false;
  bool rawValid2 = false;
  uint32_t lastCamUpdateMs1 = 0;
  uint32_t lastCamUpdateMs2 = 0;

  char goal1 = 'N';
  char goal2 = 'N';
  float angle_cam_deg1 = 0.0f;
  float angle_cam_deg2 = 0.0f;
  int blob_h_px1 = 0;
  int blob_h_px2 = 0;

  float angle_field_deg1 = 0.0f;
  float angle_field_deg2 = 0.0f;

  bool usePlusSign1 = true; // Standard: field = cam + imu
  bool usePlusSign2 = true; // Standard: field = cam + imu

  void handleLine(const char* line, int num);
  static float wrap180(float a);
};

extern CamC Cam;