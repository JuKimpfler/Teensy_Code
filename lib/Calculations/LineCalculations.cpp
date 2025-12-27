#include "LineCalculations.h"

LineCalcC LineCalc;

void LineCalcC::Calc(){
    if(Line.Summe_VW > 0){ // Line VorWarnung
        for (int i = 0 ; i<8 ; i++){
            if(Line.Raw_VW[i]==1){ // Positionen durchgehen
                DriveAngle = ((i*45) - 135)-180; // Position * 45°(abstand zwischen Sensoren) -135(offset zum ersten Sensor) - 180(Wegfahren von Linie)
            }
        }
        DriveAngle = (DriveAngle - 90)*-1;
    }
    else{ // Line Basic
        summex = 0;
        summey = 0;
        Distance = 0;
        for(int i = 0 ; i<32 ; i++){ // Positionen Durchgehen
            Distance = Distance + Line.Raw[i]; // Nummern Zusammenählen
            summex = summex + (Line.Raw[i]*sinf(((i*11.25)*DEG_TO_RAD))); // Linie Abstand in X Richtung
            summey = summey + (Line.Raw[i]*cosf(((i*11.25)*DEG_TO_RAD))); // Linie Abstand in Y Richtung
        }
        RawAngle = atan2f(summex,summey)*RAD_TO_DEG - 90; // Winkel berechnung und drehung um 90 Grad 

        DriveAngle = -1*(RawAngle-180); // Links Rechtsvertauschen (wegen Spiegelverkehrt) und in gegenrichtung von Linie Wegbewegen

        if (DriveAngle<0){DriveAngle = DriveAngle+360;}
        else if (DriveAngle>359){DriveAngle = DriveAngle-360;}
        if(DriveAngle<=180){DriveAngle=DriveAngle;}
        else if (DriveAngle>180){DriveAngle=DriveAngle-360;}
        if (DriveAngle<-180){DriveAngle = DriveAngle+360;}
        else if (DriveAngle>359){DriveAngle = DriveAngle-360;}

        DriveAngle = DriveAngle*-1;
    }
}