#include "Line.h"
#include "SPI.h"
#include "RGB.h"

LineC Line;

/**
 * @brief Kalibrierungsfunktion
 * 
 * Liest die Sensorwerte von allen 5 geraten per SPI aus und berechnet
 * je nach dem Modus (Modes) einen Wert, welcher in den entsprechenden
 * Variablen gespeichert wird.
 * 
 * Wenn Modes == 1, wird der Durchschnitt der analogen Werte
 * in min_schwelle gespeichert.
 * Wenn Modes == 2, wird der maximalwert in max_schwelle gespeichert.
 * Wenn Modes == 3, wird ein Wert berechnet, welcher zwischen min und max
 * liegt, jedoch mit 60% Abstand zu min_schwelle und 40% Abstand
 * zu max_schwelle. Dieser Wert wird in Line_Schwelle gespeichert.
 */
void LineC::Calibrate(bool Modes){
    messzyklenDurchschnitt(Modes);

    for (int i = 0 ; i<40 ; i++){
        Line_conf[i] = Line_calib_min[i]+((Line_calib_max[i] - Line_calib_min[i])/2);
        Line_Schwelle[Color_ID][i] = Line_conf[i];
    }
}

void LineC::messzyklenDurchschnitt(bool modus) {
    RGB.write(0,"R");
    RGB.Apply();

    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE3));

    digitalWriteFast(CS_LineA, LOW);
    for (int i = 0; i < 8; i++) { eingangDaten[i] = SPI.transfer16(ADC_Befehle[i]); }
    digitalWriteFast(CS_LineA, HIGH);

    digitalWriteFast(CS_LineB, LOW);
    for (int i = 0; i < 8; i++) { eingangDaten[i+8] = SPI.transfer16(ADC_Befehle[i]); }
    digitalWriteFast(CS_LineB, HIGH);

    digitalWriteFast(CS_LineC, LOW);
    for (int i = 0; i < 8; i++) { eingangDaten[i+16] = SPI.transfer16(ADC_Befehle[i]); }
    digitalWriteFast(CS_LineC, HIGH);

    digitalWriteFast(CS_LineD, LOW);
    for (int i = 0; i < 8; i++) { eingangDaten[i+24] = SPI.transfer16(ADC_Befehle[i]); }
    digitalWriteFast(CS_LineD, HIGH);

    digitalWriteFast(CS_LineVW, LOW);
    for (int i = 0; i < 8; i++) { eingangDaten[i+32] = SPI.transfer16(ADC_Befehle[i]); }
    digitalWriteFast(CS_LineVW, HIGH);

    SPI.endTransaction();

    // Buffer auf Null setzen
    for (int i = 0; i < N; i++) mittelwertBuffer[i] = 0;
    
    // Zyklusweise Durchschnitt berechnen
    for (int z = 0; z < ZYKLEN; z++) {
        SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE3));

        digitalWriteFast(CS_LineA, LOW);
        for (int i = 0; i < 8; i++) { eingangDaten[i] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineA, HIGH);

        digitalWriteFast(CS_LineB, LOW);
        for (int i = 0; i < 8; i++) { eingangDaten[i+8] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineB, HIGH);

        digitalWriteFast(CS_LineC, LOW);
        for (int i = 0; i < 8; i++) { eingangDaten[i+16] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineC, HIGH);

        digitalWriteFast(CS_LineD, LOW);
        for (int i = 0; i < 8; i++) { eingangDaten[i+24] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineD, HIGH);

        digitalWriteFast(CS_LineVW, LOW);
        for (int i = 0; i < 8; i++) { eingangDaten[i+32] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineVW, HIGH);

        SPI.endTransaction();

        for (int i = 0; i < N; i++) {
        // Rolling Average: y[n] = y[n-1] + (x[n] - y[n-1]) / (n+1)
        mittelwertBuffer[i] += (eingangDaten[i] - mittelwertBuffer[i]) / (z + 1);
        }
        delay(6);
    }

    for (int i = 0; i < N; i++) Line_Schwelle_calib[modus][i] = mittelwertBuffer[i];

    RGB.write(0,"OFF");
    RGB.Apply();
}

void LineC::read_Fast(){
    Summe = 0;
    VW_Summe = 0;
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE3)); //Start SPI und Einstellern der Übertragungsparameter
    digitalWriteFast(CS_LineA, LOW);  /* prepare default state of ADC chip select */
    for (int i = 0 ; i<8 ; i++){Values_raw[i]= SPI.transfer16(ADC_Befehle[i]);if(Values_raw[i]<Line_Schwelle[Color_ID][i]){line[i]=0;}else{line[i]=1;Summe++;}}
    digitalWriteFast(CS_LineA, HIGH);  // prepare default state of ADC chip select 

    digitalWriteFast(CS_LineB, LOW);  /* prepare default state of ADC chip select */
    for (int i = 0 ; i<8 ; i++){Values_raw[i+8]= SPI.transfer16(ADC_Befehle[i]);if(Values_raw[i+8]<Line_Schwelle[Color_ID][i+8]){line[i+8]=0;}else{line[i+8]=1;Summe++;}}
    digitalWriteFast(CS_LineB, HIGH);  // prepare default state of ADC chip select 

    digitalWriteFast(CS_LineC, LOW);  /* prepare default state of ADC chip select */
    for (int i = 0 ; i<8 ; i++){Values_raw[i+16]= SPI.transfer16(ADC_Befehle[i]);if(Values_raw[i+16]<Line_Schwelle[Color_ID][i+16]){line[i+16]=0;}else{line[i+16]=1;Summe++;}}
    digitalWriteFast(CS_LineC, HIGH);  // prepare default state of ADC chip select 

    digitalWriteFast(CS_LineD, LOW);  /* prepare default state of ADC chip select */
    for (int i = 0 ; i<8 ; i++){Values_raw[i+24]= SPI.transfer16(ADC_Befehle[i]);if(Values_raw[i+24]<Line_Schwelle[Color_ID][i+24]){line[i+24]=0;}else{line[i+24]=1;Summe++;}}
    digitalWriteFast(CS_LineD, HIGH);  // prepare default state of ADC chip select 

    digitalWriteFast(CS_LineVW, LOW);  /* prepare default state of ADC chip select */
    for (int i = 0 ; i<8 ; i++){Values_raw_VW[i]= SPI.transfer16(ADC_Befehle[i]);if(Values_raw_VW[i]<Line_Schwelle[Color_ID][i+32]){lineVW[i]=0;}else{lineVW[i]=1;VW_Summe++;}}
    digitalWriteFast(CS_LineVW, HIGH);  // prepare default state of ADC chip select 
    SPI.endTransaction();
}

void LineC::init(){
    pinMode(CS_LineA, OUTPUT);  /* ADC1 chip select */ 
    digitalWrite(CS_LineA, HIGH);  /* prepare default state of ADC1 chip select */
    pinMode(CS_LineB, OUTPUT);  /* ADC2 chip select */
    digitalWrite(CS_LineB, HIGH);  /* prepare default state of ADC2 chip select */
    pinMode(CS_LineC, OUTPUT);  /* ADC3 chip select */
    digitalWrite(CS_LineC, HIGH);  /* prepare default state of ADC3 chip select */
    pinMode(CS_LineD, OUTPUT);  /* ADC4 chip select */
    digitalWrite(CS_LineD, HIGH);  /* prepare default state of ADC4 chip select */
    pinMode(CS_LineVW, OUTPUT);  /* ADC5 chip select */
    digitalWrite(CS_LineVW, HIGH);  /* prepare default state of ADC5 chip select */
}

