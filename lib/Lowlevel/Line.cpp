#include "Line.h"
#include "SPI.h"

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
void LineC::Calibrate(int Modes){
    uint16_t raw[40];
    int idx = 0;

    if (Modes == 1) {

        SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE3));

        digitalWriteFast(CS_LineA, LOW);
        for (int i = 0; i < 8; i++) { raw[idx++] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineA, HIGH);

        digitalWriteFast(CS_LineB, LOW);
        for (int i = 0; i < 8; i++) { raw[idx++] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineB, HIGH);

        digitalWriteFast(CS_LineC, LOW);
        for (int i = 0; i < 8; i++) { raw[idx++] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineC, HIGH);

        digitalWriteFast(CS_LineD, LOW);
        for (int i = 0; i < 8; i++) { raw[idx++] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineD, HIGH);

        SPI.endTransaction();

        uint32_t sum = 0;
        for (int i = 0; i < 40; i++) { sum += raw[i]; }
        Line_Grass = sum / 40;

        SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE3));

        digitalWriteFast(CS_LineVW, LOW);
        for (int i = 0; i < 8; i++) { raw[idx++] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineVW, HIGH);

        SPI.endTransaction();
        
        sum = 0;
        for (int i = 0; i < 40; i++) { sum += raw[i]; }
        Line_Grass_VW = sum / 40;
    }
    else if (Modes == 2) {

        SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE3));

        digitalWriteFast(CS_LineA, LOW);
        for (int i = 0; i < 8; i++) { raw[idx++] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineA, HIGH);

        digitalWriteFast(CS_LineB, LOW);
        for (int i = 0; i < 8; i++) { raw[idx++] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineB, HIGH);

        digitalWriteFast(CS_LineC, LOW);
        for (int i = 0; i < 8; i++) { raw[idx++] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineC, HIGH);

        digitalWriteFast(CS_LineD, LOW);
        for (int i = 0; i < 8; i++) { raw[idx++] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineD, HIGH);

        SPI.endTransaction();

        uint16_t maxVal = 0;
        for (int i = 0; i < 40; i++) { if (raw[i] > maxVal) maxVal = raw[i]; }
        Line_Norm = maxVal;

        SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE3));

        digitalWriteFast(CS_LineVW, LOW);
        for (int i = 0; i < 8; i++) { raw[idx++] = SPI.transfer16(ADC_Befehle[i]); }
        digitalWriteFast(CS_LineVW, HIGH);

        SPI.endTransaction();

        maxVal = 0;
        for (int i = 0; i < 40; i++) { if (raw[i] > maxVal) maxVal = raw[i]; }
        Line_Norm_VW = maxVal;
    }

    Line_Schwelle = Line_Grass + (uint16_t)(0.6f * (Line_Norm - Line_Grass));
    Line_Schwelle_VW = Line_Grass_VW + (uint16_t)(0.6f * (Line_Norm_VW - Line_Grass_VW));
}

void LineC::read_Fast(){
    Summe = 0;
    VW_Summe = 0;
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE3)); //Start SPI und Einstellern der Übertragungsparameter
    digitalWriteFast(CS_LineA, LOW);  /* prepare default state of ADC chip select */
    for (int i = 0 ; i<8 ; i++){if(SPI.transfer16(ADC_Befehle[i])<Line_Schwelle){line[i]=0;}else{line[i]=1;Summe++;}}
    digitalWriteFast(CS_LineA, HIGH);  // prepare default state of ADC chip select 

    digitalWriteFast(CS_LineB, LOW);  /* prepare default state of ADC chip select */
    for (int i = 0 ; i<8 ; i++){if(SPI.transfer16(ADC_Befehle[i])<Line_Schwelle){line[i+8]=0;}else{line[i+8]=1;Summe++;}}
    digitalWriteFast(CS_LineB, HIGH);  // prepare default state of ADC chip select 

    digitalWriteFast(CS_LineC, LOW);  /* prepare default state of ADC chip select */
    for (int i = 0 ; i<8 ; i++){if(SPI.transfer16(ADC_Befehle[i])<Line_Schwelle){line[i+16]=0;}else{line[i+16]=1;Summe++;}}
    digitalWriteFast(CS_LineC, HIGH);  // prepare default state of ADC chip select 

    digitalWriteFast(CS_LineD, LOW);  /* prepare default state of ADC chip select */
    for (int i = 0 ; i<8 ; i++){if(SPI.transfer16(ADC_Befehle[i])<Line_Schwelle){line[i+24]=0;}else{line[i+24]=1;Summe++;}}
    digitalWriteFast(CS_LineD, HIGH);  // prepare default state of ADC chip select 

    digitalWriteFast(CS_LineVW, LOW);  /* prepare default state of ADC chip select */
    for (int i = 0 ; i<8 ; i++){if(SPI.transfer16(ADC_Befehle[i])<Line_Schwelle_VW){lineVW[i]=0;}else{lineVW[i]=1;VW_Summe++;}Values_raw[i]= SPI.transfer16(ADC_Befehle[i]);}
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

    Line_Schwelle = Line_Grass + (uint16_t)(0.6f * (Line_Norm - Line_Grass));
    Line_Schwelle_VW = Line_Grass + (uint16_t)(0.6f * (Line_Norm_VW - Line_Grass_VW));
}

