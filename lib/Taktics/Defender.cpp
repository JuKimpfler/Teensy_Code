#include "Defender.h"
#include "Elementar.h"

DefenderC Defender;

void DefenderC::Bump_Ball(){
    if(Ball.Stilltime > 1500){
        Robot.Drive(Ball.Angle,0,40);
    }
}

static inline float angleDiff(float a, float b) {
  return fabsf(U.Circel(a - b));
}

// Gibt 999.0f zurück => stehen bleiben
float berechneLinienFahrWinkel(float ballAngle,
                               const uint8_t* lineArray,
                               int arraySize = 32,
                               float toleranzGrenzWert = 10.0f)
{
  if (!lineArray) return 0.0f;
  if (arraySize > 32) arraySize = 32;

  float aktiveSensoren[32];
  int anzahlAktiv = 0;

  // 1) aktive Sensoren sammeln (Winkelpositionen)
  for (int i = 0; i < arraySize; i++) {
    if (lineArray[i] != 0u) { // uint16_t: 0/1
      aktiveSensoren[anzahlAktiv++] = U.Circel((i * 11.25f) - 90.0f);
    }
  }

  if (anzahlAktiv == 0) return 0.0f;
  if (anzahlAktiv == 1) return aktiveSensoren[0];

  // 2) zwei Sensoren mit maximaler Winkel-Distanz finden (kreisförmig korrekt)
  float maxDistanz = -1.0f;
  float sensorA = 0.0f, sensorB = 0.0f;

  for (int i = 0; i < anzahlAktiv; i++) {
    for (int j = i + 1; j < anzahlAktiv; j++) {
      float dist = angleDiff(aktiveSensoren[i], aktiveSensoren[j]);
      if (dist > maxDistanz) {
        maxDistanz = dist;
        sensorA = aktiveSensoren[i];
        sensorB = aktiveSensoren[j];
      }
    }
  }

  // 3) Mittelwinkel zwischen A und B bestimmen (2 Möglichkeiten)
  float delta  = U.Circel(sensorB - sensorA);
  float mitte1 = U.Circel(sensorA + (delta * 0.5f));
  float mitte2 = U.Circel(mitte1 + 180.0f);

  // 4) Mitte wählen, die näher am Ballwinkel liegt
  float zielWinkel = (angleDiff(mitte1, ballAngle) < angleDiff(mitte2, ballAngle)) ? mitte1 : mitte2;

  // 5) Wenn Zielwinkel schon nahe am Ballwinkel ist: stehen bleiben
  if (angleDiff(zielWinkel, ballAngle) <= toleranzGrenzWert) {
    return 999.0f;
  }

  // 6) Fahrwinkel muss ein aktiver Sensorwinkel sein: nächstgelegenen wählen
  float besterFahrWinkel = aktiveSensoren[0];
  float minFehler = 1e9f;

  for (int i = 0; i < anzahlAktiv; i++) {
    float fehler = angleDiff(aktiveSensoren[i], zielWinkel);
    if (fehler < minFehler) {
      minFehler = fehler;
      besterFahrWinkel = aktiveSensoren[i];
    }
  }
  Debug.Start();
  Debug.Plot("Anzahl",anzahlAktiv);
  Debug.Plot_List("Aktive",aktiveSensoren,anzahlAktiv);
  Debug.Plot("ziel",zielWinkel);
  Debug.Plot("ball",Ball.Angle);
  Debug.Plot("fahr",besterFahrWinkel);
  Debug.Send();
  delay(20);
  return besterFahrWinkel;
}

float DefenderC::Follow_Line(){
    if(Line.Summe > 0){
        float fahrWinkel = berechneLinienFahrWinkel(Ball.Angle, Line.line , 32 ,10 );
        if (fahrWinkel == 999.0f) {
            Serial.println("Stop!");
            Robot.Stop();
        } else {
            Robot.Drive(fahrWinkel,0,20);
        }
    }
    else {
        Serial.println("No_line!");
        Robot.Drive(180,0,20);
    }
    return 0;
}

void DefenderC::set_State(bool State){
    State_Def = State;
}

void DefenderC::init(){
    
}

void DefenderC::Update(){
    if(State_Def == true){
        Follow_Line();
        Bump_Ball();
    }
}