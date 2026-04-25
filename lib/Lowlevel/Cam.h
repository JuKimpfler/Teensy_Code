#pragma once
#include "Elementar.h"


class CamC {
public:
  // UART auswählen (Standard Serial1 für Teensy pins 0/1)
  void init(HardwareSerial& ser = Serial1, uint32_t baud = 115200);

  // Muss sehr oft aus loop() aufgerufen werden (non-blocking)
  void readOpenMV();

  // Rechnet aus den letzten gültigen Kameradaten + aktuellem BNO TiltZ die Feldposition
  void Update();

  bool  isValid(uint32_t maxAgeMs = 200) const;

  // optional: Debug/Infos
  char  giveGoal() const { return goal; }
  int   giveQual() const { return qual; }

  // Feldparameter setzen
  void setField(float fieldW_cm, float fieldH_cm);
  void setGoalCenters(float blueX, float blueY, float yellowX, float yellowY);

  float giveXPos() const { return X_pos; }
  float giveYPos() const { return Y_pos; }

  // unkorigierte relative Position (Cam -> Goal) im Roboter/Kamera-KS (cm)
  float giveXPos_relativ() const { return rel_x_cm; }        // rechts +
  float giveYPos_relativ() const { return rel_dist_cm; }     // vorne +
// ...
private:
  // ...
  // Ergebnis: Goal -> Cam, feldorientiert
  float X_pos = 0.0f;
  float Y_pos = 0.0f;
  HardwareSerial* port = nullptr;

  // line buffer
  static constexpr uint16_t LINE_BUF_SIZE = 128;
  char lineBuf[LINE_BUF_SIZE];
  uint16_t lineLen = 0;

  // letzte empfangene Kamera-Rohdaten
  bool  rawValid = false;
  uint32_t lastCamUpdateMs = 0;

  float cam_x_cm = 0.0f;   // (optional) absolute Werte die OpenMV evtl. sendet
  float cam_y_cm = 0.0f;   // (optional)
  char  goal = 'N';        // 'B' oder 'Y'
  int   qual = 0;

  float rel_x_cm = 0.0f;     // rechts + (Roboter->Tor)
  float rel_dist_cm = 0.0f;  // vorne + (Roboter->Tor)

  // Ergebnis nach Rotation (Feld)
  float X_field_cm = 0.0f;
  float Y_field_cm = 0.0f;

  // Feld-/Torparameter
  float fieldW = 182.0f;
  float fieldH = 243.0f;

  float blueGoalX = 91.0f;
  float blueGoalY = 243.0f;
  float yellowGoalX = 91.0f;
  float yellowGoalY = 0.0f;

  // parsing
  void handleLine(const char* line);
  static bool parsePLine(const char* line,
                         float& out_x, float& out_y,
                         char& out_goal, int& out_qual,
                         float& out_relx, float& out_reldist);
};

extern CamC Cam;

class GoalC{
    public:
        double Angle;
        double Distance;
        double X;
        double Y;
        double Area;
        bool inSight;
        bool Regeln;
        bool lastdir; // true = - / false = +
    private:
};

extern GoalC Goal;
 