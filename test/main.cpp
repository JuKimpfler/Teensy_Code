/**
 * MinimalExample.ino
 * 
 * Minimales Beispiel für die RobotSimComm Library
 * 
 * Funktionalität:
 * - Liest IR-Sensor
 * - Fährt auf Ball zu
 * - Aktiviert Dribbler
 * - Schießt Ball wenn in Ballkuhle
 * 
 * Hardware: Teensy/Arduino mit USB-Verbindung zum PC
 * Verbindung: USB-Kabel zum PC mit RobotSimulationTool
 */
#include "Arduino.h"
#include "lib\RobotSimComm\RobotSimComm.h"

// Erstelle RobotSimComm Objekt
// Verwendet Serial (USB) mit 115200 Baud
RobotSimComm robot(Serial, 115200);

void setup() {
    // Initialisiere Kommunikation mit Simulation
    robot.begin();
    
    // Kurze Pause um Verbindung aufzubauen
    delay(1000);
}

void loop() {
    // Lese IR-Sensor (Ball-Erkennung)
    if (robot.read.ir()) {
        
        // Ball gefunden und Signal zuverlässig?
        if (robot.read.irReliable) {
            
            // Hole Ball-Richtung und Distanz
            int16_t ballDirection = robot.read.irDirection;  // -180 bis 180°
            uint8_t ballDistance = robot.read.irDistance;    // 0 bis 255
            
            // Dribbler aktivieren
            robot.write.dribbler(true);
            
            // Fahre in Richtung des Balls mit Geschwindigkeit 150
            robot.write.moveDirection(ballDirection, 150);
            
            // Prüfe ob Ball in Ballkuhle
            if (robot.read.ldrSwitches()) {
                if (robot.read.ballDetected) {
                    // Ball in Ballkuhle - SCHIESSEN!
                    robot.write.kick();
                    
                    // Kurze Pause nach Schuss
                    delay(500);
                }
            }
            
        } else {
            // Ball nicht zuverlässig erkannt - suchen (drehen)
            robot.write.rotate(80);
        }
    }
    
    // Update-Intervall: 50ms (20 Hz)
    delay(50);
}