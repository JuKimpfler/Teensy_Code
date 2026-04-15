#include "LineCalc.h"
#include "BNO055.h"

LineCalcC LineCalc;

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
}