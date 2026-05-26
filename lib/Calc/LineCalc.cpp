#include "LineCalc.h"
#include "BNO055.h"

LineCalcC LineCalc;

bool LineCalcC::berechneLinienTiefe(){
if (Line.Summe == 0) {Line.dep=1;return false;}
  if (Line.Summe == 1) {Line.dep=1;return false;}

  // 2) zwei Sensoren mit maximaler Winkel-Distanz finden (kreisförmig korrekt)
  float maxDistanz = -1.0f;
  float sensorA = 0.0f, sensorB = 0.0f;

  int aktiveSensoren[Line.Summe];
  int count = 0;
  for(int i = 0;i<32; i++){
    if(Line.line[i]==1){
      aktiveSensoren[count] = U.Circel((i * 11.25f) + 90.0f);
      count++;
    }
  }

  for (int i = 0; i < Line.Summe; i++) {
    for (int j = i + 1; j < Line.Summe; j++) {
      float dist = fabsf(U.Circel(aktiveSensoren[i] - aktiveSensoren[j]));
      if (dist > maxDistanz) {
        maxDistanz = dist;
        sensorA = aktiveSensoren[i];
        sensorB = aktiveSensoren[j];
      }
    }
  }

  // 3) Mittelwinkel zwischen A und B bestimmen (2 Möglichkeiten)
  float delta  = U.Circel(sensorB - sensorA);
  Line.dep = 1-abs(sin((abs(delta)/2)*DEG_TO_RAD));
  return true;
}

void LineCalcC::Calc(){
    if(Line.Summe > 0){ // Line VorWarnung
        summex = 0;
        summey = 0;
        Distance = 0;
        Distance2 = 0;
        for(int i = 0 ; i<32 ; i++){ // Positionen Durchgehen
            summex = summex + (Line.line[i]*sinf(((i*11.25)*DEG_TO_RAD))); // Linie Abstand in X Richtung
            summey = summey + (Line.line[i]*cosf(((i*11.25)*DEG_TO_RAD))); // Linie Abstand in Y Richtung
        }
        DriveAngle = (((atan2f(summex,summey)*RAD_TO_DEG + 90)) - BNO055.TiltZ)*-1; // Winkel berechnung und drehung um 90 Grad 

        if (DriveAngle<-180){DriveAngle = DriveAngle+360;}
        else if(DriveAngle>180){DriveAngle=DriveAngle-360;}
    }
    else if (Line.VW_Summe > 0){ // Line Basic
        for (int i = 0 ; i<8 ; i++){
            if(Line.lineVW[i]!=0){ // Positionen durchgehen
                DriveAngle = (((i*45) - 135)-180); // Position * 45°(abstand zwischen Sensoren) -135(offset zum ersten Sensor) - 180(Wegfahren von Linie)
            }
        }
        DriveAngle = (DriveAngle - 90) - BNO055.TiltZ;
        if (DriveAngle<-180){DriveAngle = DriveAngle+360;}
        else if(DriveAngle>180){DriveAngle=DriveAngle-360;}
    }

    RawAngle = U.Circel(DriveAngle-180)*-1;
    berechneLinienTiefe();
}

