#include "LineCalculations.h"
#include "RGB.h"
#include "Robot.h"

LineCalcC LineCalc;

void LineCalcC::Calc(){
    if (Line.Summe > 0 || Line.Summe_VW != 0){//|| Line.Summe_VW != 0
        if(Line.Summe > 0){ // Line
            summex = 0;
            summey = 0;
            Distance = 0;
            for(int i = 0 ; i<32 ; i++){ // Positionen Durchgehen
                Distance = Distance + Line.Raw[i]; // Nummern Zusammenählen
                summex = summex + (Line.Raw[i]*sinf(((i*11.25)*DEG_TO_RAD))); // Linie Abstand in X Richtung
                summey = summey + (Line.Raw[i]*cosf(((i*11.25)*DEG_TO_RAD))); // Linie Abstand in Y Richtung
            }
            RawAngle = (atan2f(summex,summey)*RAD_TO_DEG - 90); // Winkel berechnung und drehung um 90 Grad 
            if (RawAngle<0){RawAngle = RawAngle+360;}
            else if (RawAngle>359){RawAngle = RawAngle-360;}
            if(RawAngle<=180){RawAngle=RawAngle;}
            else if (RawAngle>180){RawAngle=RawAngle-360;}
            if (RawAngle<-180){RawAngle = RawAngle+360;}
            else if (RawAngle>359){RawAngle = RawAngle-360;}
            
            DriveAngle = RawAngle-180;
            
            if (DriveAngle<0){DriveAngle = DriveAngle+360;}
            else if (DriveAngle>359){DriveAngle = DriveAngle-360;}
            if(DriveAngle<=180){DriveAngle=DriveAngle;}
            else if (DriveAngle>180){DriveAngle=DriveAngle-360;}
            if (DriveAngle<-180){DriveAngle = DriveAngle+360;}
            else if (DriveAngle>359){DriveAngle = DriveAngle-360;}
        }
        else if (Line.Summe_VW > 0 || Line.Summe == 0){ // Line Basic
            for (int i = 0 ; i<8 ; i++){
                if(Line.Raw_VW[i]==1){ // Positionen durchgehen
                    DriveAngle = (((i*45) - 135)-90); // Position * 45°(abstand zwischen Sensoren) -135(offset zum ersten Sensor) - 180(Wegfahren von Linie)
                }
            }
        }
    }
}