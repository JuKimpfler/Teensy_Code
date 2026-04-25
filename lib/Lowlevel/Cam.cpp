#include "Cam.h"
#include "BNO055.h"
CamC Cam;
GoalC Goal;

void CamC::init(HardwareSerial& ser, uint32_t baud) {
  port = &ser;
  lineLen = 0;

  rawValid = false;
  lastCamUpdateMs = 0;
}

void CamC::setField(float fieldW_cm, float fieldH_cm) {
  fieldW = fieldW_cm;
  fieldH = fieldH_cm;
}

void CamC::setGoalCenters(float blueX, float blueY, float yellowX, float yellowY) {
  blueGoalX = blueX; blueGoalY = blueY;
  yellowGoalX = yellowX; yellowGoalY = yellowY;
}

bool CamC::isValid(uint32_t maxAgeMs) const {
  if (!rawValid) return false;
  return (millis() - lastCamUpdateMs) <= maxAgeMs;
}

void CamC::readOpenMV() {
  if (!port) return;

  while (port->available()) {
    char c = (char)port->read();

    if (c == '\n') {
      lineBuf[lineLen] = '\0';
      // strip CR
      if (lineLen > 0 && lineBuf[lineLen - 1] == '\r') {
        lineBuf[lineLen - 1] = '\0';
      }

      handleLine(lineBuf);
      lineLen = 0;
      continue;
    }

    if (lineLen < (LINE_BUF_SIZE - 1)) {
      lineBuf[lineLen++] = c;
    } else {
      // overflow -> drop line
      lineLen = 0;
    }
  }
}

void CamC::handleLine(const char* line) {
  if (!line || !line[0]) return;

  if (line[0] == 'N') {
    rawValid = false;
    return;
  }

  if (line[0] == 'P') {
    float x, y, relx, reld;
    char g;
    int q;

    if (parsePLine(line, x, y, g, q, relx, reld)) {
      cam_x_cm = x;   // falls du sie debuggen willst
      cam_y_cm = y;
      goal = g;
      qual = q;
      rel_x_cm = relx;
      rel_dist_cm = reld;

      rawValid = true;
      lastCamUpdateMs = millis();
    }
  }
}

bool CamC::parsePLine(const char* line,
                      float& out_x, float& out_y,
                      char& out_goal, int& out_qual,
                      float& out_relx, float& out_reldist) {

  char tmp[LINE_BUF_SIZE];
  strncpy(tmp, line, LINE_BUF_SIZE - 1);
  tmp[LINE_BUF_SIZE - 1] = '\0';

  char* save = nullptr;
  char* tok = strtok_r(tmp, ",", &save);
  int idx = 0;

  while (tok) {
    if (idx == 1) out_x = strtof(tok, nullptr);
    else if (idx == 2) out_y = strtof(tok, nullptr);
    else if (idx == 3) out_goal = tok[0];
    else if (idx == 4) out_qual = atoi(tok);
    else if (idx == 5) out_relx = strtof(tok, nullptr);
    else if (idx == 6) out_reldist = strtof(tok, nullptr);

    tok = strtok_r(nullptr, ",", &save);
    idx++;
  }

  return idx >= 7;
}

void CamC::Update() {
  // 1) UART pollen (damit Update() alleine reicht)
  readOpenMV();

  // 2) Ohne frische Daten keine neuen Ausgaben berechnen
  if (!isValid(250)) {
    return;
  }

  const float gx_r = -rel_x_cm;      // Goal->Cam in Roboter-X (rechts+)
  const float gy_r = -rel_dist_cm;   // Goal->Cam in Roboter-Y (vorne+)

  X_pos = gx_r+91;
  Y_pos = gy_r+121.5;

}