#include "Cam.h"
#include "BNO055.h"
#include <math.h>

CamC Cam;

static float wrap180f(float a) {
  while (a > 180.0f) a -= 360.0f;
  while (a < -180.0f) a += 360.0f;
  return a;
}

void CamC::init(uint32_t baud, bool num1 , bool num2, HardwareSerial& ser, HardwareSerial& serb) {
  if(num1 == true){
    port1 = &ser;
    port1->begin(baud);
  }
  if(num2 == true){
    port2 = &serb;
    port2->begin(baud);
  }
  lineLen = 0;
  rawValid1 = false;
  rawValid2 = false;
  lastCamUpdateMs1 = 0;
  lastCamUpdateMs2 = 0;
}

bool CamC::isValid1(uint32_t maxAgeMs) const {
  if (!rawValid1) return false;
  return (millis() - lastCamUpdateMs1) <= maxAgeMs;
}

bool CamC::isValid2(uint32_t maxAgeMs) const {
  if (!rawValid2) return false;
  return (millis() - lastCamUpdateMs2) <= maxAgeMs;
}

void CamC::readOpenMV() {
  if (port1){
    while (port1->available()) {
      char c = (char)port1->read();

      if (c == '\n') {
        lineBuf[lineLen] = '\0';
        if (lineLen > 0 && lineBuf[lineLen - 1] == '\r') lineBuf[lineLen - 1] = '\0';
        handleLine(lineBuf,1);
        lineLen = 0;
        continue;
      }

      if (lineLen < (LINE_BUF_SIZE - 1)) {
        lineBuf[lineLen++] = c;
      } else {
        lineLen = 0; // overflow -> drop
      }
    }
  }

  if (port2){
    while (port2->available()) {
    char c = (char)port2->read();

    if (c == '\n') {
      lineBuf[lineLen] = '\0';
      if (lineLen > 0 && lineBuf[lineLen - 1] == '\r') lineBuf[lineLen - 1] = '\0';
      handleLine(lineBuf,2);
      lineLen = 0;
      continue;
    }

    if (lineLen < (LINE_BUF_SIZE - 1)) {
      lineBuf[lineLen++] = c;
    } else {
      lineLen = 0; // overflow -> drop
    }
  }
  }
}

void CamC::handleLine(const char* line, int num) {
  if (!line || !line[0]) return;

  if(num == 1){
    if (line[0] == 'N') {
      rawValid1 = false;
      angle_field_deg1 = 0;
      return;
    }

    // Erwartet: A,<goal>,<angle_cam_deg>,<blob_h>
    // Beispiel: A,B,-12.34,37
    if (line[0] == 'A' && line[1] == ',') {
      // einfache CSV parse ohne strtok allocation
      // Wir kopieren trotzdem in tmp für strtok_r (robust und schnell genug bei 20-60Hz)
      char tmp[LINE_BUF_SIZE];
      strncpy(tmp, line, LINE_BUF_SIZE - 1);
      tmp[LINE_BUF_SIZE - 1] = '\0';

      char* save = nullptr;
      char* tok = strtok_r(tmp, ",", &save);

      int idx = 0;
      char g = 'N';
      float ang = 0.0f;
      int h = 0;

      while (tok) {
        if (idx == 1) g = tok[0];
        else if (idx == 2) ang = strtof(tok, nullptr);
        else if (idx == 3) h = atoi(tok);
        tok = strtok_r(nullptr, ",", &save);
        idx++;
      }

      if (idx >= 4) {
        goal1 = g;
        angle_cam_deg1 = ang;
        blob_h_px1 = h;
        rawValid1 = true;
        lastCamUpdateMs1 = millis();
      }
    }
  }

  if(num == 2){
    if (line[0] == 'N') {
      rawValid2 = false;
      angle_field_deg2 = 0;
      return;
    }


    // Erwartet: A,<goal>,<angle_cam_deg>,<blob_h>
    // Beispiel: A,B,-12.34,37
    if (line[0] == 'A' && line[1] == ',') {
      // einfache CSV parse ohne strtok allocation
      // Wir kopieren trotzdem in tmp für strtok_r (robust und schnell genug bei 20-60Hz)
      char tmp[LINE_BUF_SIZE];
      strncpy(tmp, line, LINE_BUF_SIZE - 1);
      tmp[LINE_BUF_SIZE - 1] = '\0';

      char* save = nullptr;
      char* tok = strtok_r(tmp, ",", &save);

      int idx = 0;
      char g = 'N';
      float ang = 0.0f;
      int h = 0;

      while (tok) {
        if (idx == 1) g = tok[0];
        else if (idx == 2) ang = strtof(tok, nullptr);
        else if (idx == 3) h = atoi(tok);
        tok = strtok_r(nullptr, ",", &save);
        idx++;
      }

      if (idx >= 4) {
        goal2 = g;
        angle_cam_deg2 = ang;
        blob_h_px2 = h;
        rawValid2 = true;
        lastCamUpdateMs2 = millis();
      }
    }
  }
}

void CamC::Update() {
  // UART pollen
  readOpenMV();

  float imu_deg = BNO055.giveDeg();

  if (isValid1(250)){
    if (usePlusSign1) angle_field_deg1 = wrap180f(imu_deg + angle_cam_deg1);
    else             angle_field_deg1 = wrap180f(imu_deg - angle_cam_deg1);
  }

  if (isValid2(250)){
    if (usePlusSign2) angle_field_deg2 = wrap180f(imu_deg + angle_cam_deg2);
    else             angle_field_deg2 = wrap180f(imu_deg - angle_cam_deg2);
  }
}