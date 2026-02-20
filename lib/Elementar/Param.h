#pragma once
#include "Arduino.h"

// === Projektweite Parameter mit Beschreibungen ===

// Motor-/Geschwindigkeitswerte
#define HighSpeed 30 // maximale Fahrgeschwindigkeit: genutzt von Robot::Drive beim schnellen Fahren
inline double LowSpeed = 10; // reduzierte Geschwindigkeit für langsames Manövrieren

// PID-Reglerkoeffizienten (Heading-Control)
inline double Kp = 0.6; // proportionaler Anteil für PIDC::Calculate
inline double Kd = 12;  // differentieller Anteil
inline double Ki = 0;   // integraler Anteil

// Glättungsfaktor für Drive_Smoothed_Set/Update
inline double Drive_Smoothed_Faktor = 0.5; // bestimmt Schrittgröße beim Anpassen der Geschwindigkeit

// allgemeine Multiplikatoren
inline double PID_Mult = 0.2;    // zusätzlicher Multiplikator in vielen Regelkreisen
inline double PID_Konstante = 0.6; // alternative PID-Konstante (Fallback)

// Schwellwerte für Sensoren
inline int LDR_Schwelle = 500;   // Grenzwert für LDR-basierte Ball-Erkennung
inline int Line_Schwelle = 2900; // Schwellwert für Linensensoren

// Hardware‑Parameter
inline int Motor_Frequency = 400; // PWM-Frequenz für Motoren (Hz)

// Aktualisierungsraten (Hz)
#define Interface_Frequency 10 // UI-/Taster-Abfragefrequenz
#define US_Frequency 10        // Ultraschall-Sensor-Polling-Rate

// Vision/Abstandsberechnung
#define FocalLength 2040
#define ImageWidth 2952
#define ImageHeight 1944
#define ObjectSize 4.267
#define distFactor 2.3

// Feature-Flags
#define TURN_TO_GOAL false // wenn true, nutzen Drive/Turn immer Goal.Angle statt des übergebenen Winkels

// IR-Sensor Konfiguration
inline int IR_Pos[8] = {1,2,3,4,5,6,7,8}; // physikalische Positionen der IR-Sensoren
inline int Ir_Min[8] = {1,2,3,4,5,6,7,8}; // Kalibrierungs-Mindestwerte
inline int Ir_Max[8] = {1,2,3,4,5,6,7,8}; // Kalibrierungs-Maximalwerte

//#define XCP_BL 
//#define XCP_USB
