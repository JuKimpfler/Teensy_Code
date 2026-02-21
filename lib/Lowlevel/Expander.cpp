#include "Expander.h"
ExpanderC Expander;

/**
 * @brief Initialisiert I2C-Portexpander
 * @param Add Adresse des zu initialisierenden Portexpanders
 * @param Mode Modus des Portexpanders (0 -> Output / 1 -> Input)
 * @param on_off Ausgangszustand des Portexpanders (0 -> Alle Ausgänge aus / 1 -> Alle Ausgänge an)
 */
void ExpanderC::I2CC::init(int Add, int Mode, int on_off){

    Wire1.begin(); // Initialisiere I2C
    Wire1.beginTransmission(Add); // Starte Übertragung
    Wire1.write(Config_Reg); // Konfigurationsregister wählen
    Wire1.write(Mode); // Modus setzen
    Wire1.endTransmission(); // Übertragung beenden
    Wire1.beginTransmission(Add); // Neue Übertragung
    Wire1.write(Output_Reg); // Ausgangsregister wählen
    Wire1.write(on_off); // Ausgangszustand setzen
    Wire1.endTransmission(); // Übertragung beenden
    // Step 1: Setze Konfiguration und Ausgangszustand des Portexpanders
}

/**
 * @brief Liest den Zustand der Schalter auf einem Portexpander
 * 
 * @details
 *      - Step 1: Initialisiere I2C-Portexpander
 *      - Step 2: Lese und speichere den Zustand der Schalter
 *      - Step 3: Speichere Schalterzustände
 *  
 * @param Add Adresse des zu lesenden Portexpanders
 * 
 * @return void
 */
void ExpanderC::I2CC::read(int Add){
    
    Wire1.beginTransmission(Add); // Adresse Portexpander
    Wire1.write(Input_Reg); // Eingangsregister wählen
    Wire1.endTransmission(false); // Repeated Start
    Wire1.requestFrom(Add, 1); // Lese ein Byte
    byte last = Wire1.read(); // Registerwert lesen
    Wire1.endTransmission(); // Übertragung beenden
    // Step 2: Lese und speichere den Zustand der Schalter



    if(Add == I2C_ITF_Main){
        Switch[0] = (last & 1);
        Switch[1] = (last & (1 << 1));
        Switch[2] = (last & (1 << 2));
        Switch[3] = (last & (1 << 3));
        Switch[4] = (last & (1 << 4));
        Switch[5] = (last & (1 << 5));
        Switch[6] = (last & (1 << 6));
        Switch[7] = (last & (1 << 7));
    }
    // Step 3: Speichere Schalterzustände
}

/**
 * @brief Gibt den Zustand eines Ports zurück
 * 
 * @details
 *      - Liest den Schalterzustand von einem Portexpander
 *      - Gibt den Zustand eines Ports zurück
 *  
 * @param Add Adresse des zu lesenden Portexpanders
 * @param Port Nummer des Ports, dessen Zustand geliefert werden soll
 * 
 * @return bool Schalterzustand des Ports (true -> Schalter an, false -> Schalter aus)
 */
bool ExpanderC::I2CC::give(int Add , int Port){
    if (Add == I2C_ITF_Main){
        return Switch[Port];
    }
    return false;
}


/**
 * @brief Schreibt einen Schalterzustand auf einen einzelnen Port
 * 
 * @details
 *      - Schreibt einen Schalterzustand auf einen einzelnen Port
 * 
 * @param Add Adresse des zu schreibenden Portexpanders
 * @param Port Nummer des Ports, dessen Zustand gesetzt werden soll
 * @param Zustand Schalterzustand des Ports (true -> Schalter an, false -> Schalter aus)
 * 
 * @return void
 */
void ExpanderC::I2CC::write_Single(int Add , int Port , bool Zustand){
    if ( Zustand == true ){
        Wire1.beginTransmission(Add); //address second port expander
        Wire1.write(Output_Reg);             // select output register
        Wire1.write(0x01);            // update output ports Port_Seting
        Wire1.endTransmission();
    }
    else if ( Zustand == false ){
        Wire1.beginTransmission(Add); //address second port expander
        Wire1.write(Output_Reg);             // select output register
        Wire1.write(0x00);            // update output ports Port_Seting
        Wire1.endTransmission();
    }
}


/*************  ✨ Windsurf Command ⭐  *************/
/**
 * @brief Schreibt einen Schalterzustand auf einen Port
 * 
 * @details
 *      - Schreibt einen Schalterzustand auf einen Port
 * 
 * @param Add Adresse des zu schreibenden Portexpanders
 * @param Port Nummer des Ports, dessen Zustand gesetzt werden soll
 * @param Zustand Schalterzustand des Ports (true -> Schalter an, false -> Schalter aus)
 * 
 * @return void
 */
/*******  0ad956b8-6659-4450-a2b5-85dc24042dc7  *******/
void ExpanderC::I2CC::write(int Add , int Port , bool Zustand){
    Wire1.beginTransmission(Add); //address second port expander
    Wire1.write(Output_Reg);             // select output register
    Wire1.endTransmission(false);  // send restart instead of stop
    Wire1.requestFrom(Add, 1);    // Start a read access and expect one byte
    byte last = Wire1.read();          // get current port state
    Wire1.endTransmission();

    byte out = 0;
    int Switch[8] = {0,0,0,0,0,0,0,0};

    if (int((last >> 7)) % 2 == 0) {Switch[0]=0;} else{Switch[0]=1;}
    if (int((last >> 6)) % 2 == 0) {Switch[1]=0;} else{Switch[1]=1;}
    if (int((last >> 5)) % 2 == 0) {Switch[2]=0;} else{Switch[2]=1;}
    if (int((last >> 4)) % 2 == 0) {Switch[3]=0;} else{Switch[3]=1;}
    if (int((last >> 3)) % 2 == 0) {Switch[4]=0;} else{Switch[4]=1;}
    if (int((last >> 2)) % 2 == 0) {Switch[5]=0;} else{Switch[5]=1;}
    if (int((last >> 1)) % 2 == 0) {Switch[6]=0;} else{Switch[6]=1;}
    if (int((last >> 0)) % 2 == 0) {Switch[7]=0;} else{Switch[7]=1;}

    Switch[Port] = int(Zustand);

    for (int i = 0; i < 8; i++) {
        if (Switch[i] == 1) {
            out |= (1 << (7 - i)); // Bit setzen, wenn der Wert 1 ist
        }
    }

    Wire1.beginTransmission(Add); //address second port expander
    Wire1.write(Output_Reg);             // select output register
    Wire1.write(out);            // update output ports Port_Seting
    Wire1.endTransmission();
}


/*************  ✨ Windsurf Command ⭐  *************/
/**
 * @brief Initialisiert ADC
 * 
 * @details
 *      - Initialisiert ADC
 *      - Setzt den Pin als Ausgang
 *      - Setzt den default Status des ADC Chip Select
 * @param Port ADC Port
 * 
 * @return void
 */
/*******  dcedd7ed-9db9-4245-8005-34105934c394  *******/
void ExpanderC::ADCC::init(int Port){
        pinMode(Port, OUTPUT);  /* ADC chip select */
        digitalWrite(Port, HIGH);  /* prepare default state of ADC chip select */
}

/*************  ✨ Windsurf Command ⭐  *************/
/**
 * @brief Reads the ADC values from the Expander
 * 
 * @details
 *      - Reads the ADC values from the Expander
 *      - Sets the SPI settings (14000000, MSBFIRST, SPI_MODE3)
 *      - Sets the chip select pin to LOW
 *      - Reads the ADC values of the Expander
 *      - Sets the chip select pin to HIGH
 *      - Ends the SPI transaction
 * @param Port ADC Port
 * 
 * @return void
 */
/*******  804d9cf1-0b40-433f-a2cb-6f94fb5d9a1a  *******/
void ExpanderC::ADCC::read(int Port){
    SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE3)); //Start SPI und Einstellern der Übertragungsparameter

    digitalWrite(Port, LOW);  /* prepare default state of ADC chip select */

    for (int i = 0 ; i<8 ; i++){
        if (Port == CS_LineA){
            lineA[i] = SPI.transfer16(Expander.ADC_Befehle[i]);
        }
        else if (Port == CS_LineB){
            lineB[i] = SPI.transfer16(Expander.ADC_Befehle[i]);
        }
        else if (Port == CS_LineC){
            lineC[i] = SPI.transfer16(Expander.ADC_Befehle[i]);
        }
        else if (Port == CS_LineD){
            lineD[i] = SPI.transfer16(Expander.ADC_Befehle[i]);
        }
        else if (Port == CS_LineVW){
            lineOut[i] = SPI.transfer16(Expander.ADC_Befehle[i]);
        }
    }

    digitalWrite(Port, HIGH);  // prepare default state of ADC chip select 
    SPI.endTransaction();
}

/*************  ✨ Windsurf Command ⭐  *************/
/**
 * @brief Returns the ADC value of a given pin on a given port
 * 
 * @details
 *      - Returns the ADC value of a given pin on a given port
 *      - If the port is not valid, returns 0
 * @param Port ADC port
 * @param Pin ADC pin
 * @return ADC value of the pin on the port, or 0 if the port is not valid
 */
/*******  0457c852-2de1-4318-88f7-bc62b2c607ed  *******/
int ExpanderC::ADCC::give(int Port , int Pin){
    if (Port == CS_LineA){
        return lineA[Pin];
    }
    else if (Port == CS_LineB){
        return lineB[Pin];
    }
    else if (Port == CS_LineC){
        return lineC[Pin];
    }
    else if (Port == CS_LineD){
        return lineD[Pin];
    }
    else if(Port == CS_LineVW){
        return lineOut[Pin];
    }
    return 0;
}


/*************  ✨ Windsurf Command ⭐  *************/
/**
 * @brief Returns the digital value of a given pin on a given port
 * 
 * @details
 *      - Returns the digital value (0 or 1) of a given pin on a given port
 *      - If the port is not valid, returns 0
 * @param Port ADC port
 * @param Pin ADC pin
 * @return Digital value of the pin on the port, or 0 if the port is not valid
 */
/*******  dcc04c4a-f7e0-4314-83a9-e9e11d36a1b5  *******/
int ExpanderC::ADCC::give_digital(int Port , int Pin){
    if (Port == CS_LineA){
        if(lineA[Pin]<Line_Schwelle){
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (Port == CS_LineB){
        if(lineB[Pin]<Line_Schwelle){
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (Port == CS_LineC){
        if(lineC[Pin]<Line_Schwelle){
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (Port == CS_LineD){
        if(lineD[Pin]<Line_Schwelle){
            return 0;
        }
        else {
            return 1;
        }
    }
    else if (Port == CS_LineVW){
        if(lineOut[Pin]<Line_Schwelle){
            return 0;
        }
        else {
            return 1;
        }
    }
    return 0;
}
