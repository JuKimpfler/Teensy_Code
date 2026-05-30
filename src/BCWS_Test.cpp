#include "System.h"
#include "Defender.h"
#include "INA.h"
#include "Defender_Tim.h"

elapsedMillis debugTimer;
static constexpr uint32_t DEBUG_INTERVAL_MS = 40; // Serielle Ausgabe alle 100ms

void setup() {
    Wire1.begin();
    Wire1.setClock(I2C_SPEED);
    
    Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);
    delay(500);
    Expander.I2C.read(I2C_ITF_Main);
    Color_ID = Expander.I2C.give(I2C_ITF_Main,ITF_Main_CID);
    ESC.Enable = Expander.I2C.give(I2C_ITF_Main,ITF_Main_SW0);

    SPI.begin();
    Serial.begin(115200);
    UART_2.begin(115200);
    UART_1.begin(115200);

    UART_Pixy.begin(115200);

    INA.init();

    pinMode(Start_Port,INPUT);
    pinMode(Kicker_Port, OUTPUT);
    pinMode(Drib_Port,OUTPUT);
    //pinMode(RCJ_Port,INPUT);
    System.begin(Color_ID);

    RGB.write(0,"Off");
    RGB.write(1,"Off");
    RGB.write(2,"Off");

    RGB.write(1,"R");  

    if(ESC.Enable){
        while(INA.Voltage_DR()>4){delay(30);}
        RGB.write(1,"R");  
        //Serial.println("push button 3");
        RGB.Apply();
        ESC.init(Drib_Port);
        delay(2000);
        while(INA.Voltage_DR()<4){delay(30);Serial.println("waiting on power up");}
        delay(100);
        ESC.init_Power();
        ESC.set(30);
        RGB.write(1,"G");  
        Serial.println("ON!");
        RGB.Apply();
        delay(1000);
        ESC.stop();
    }
    else{
        RGB.write(1,"G");  
        Serial.println("ON!");
        RGB.Apply();
    }
    
    //              Cam1 / Cam2
    Cam.init(115200,true,true,UART_2,UART_1);

    Cam.setSign1(false);
    Cam.setSign2(false);

    pinMode(RCJ_Port, INPUT);

    Defender.set_State(false);
}

void loop() { 
    Cycle_Timer = 0 ;

    Debug.Start();
    Debug.Plot_List("l",Line.line,32);
    Debug.Send();
    /**/Serial.print("C1 ");
    Serial.print(Cam.give_Angle1());
    Serial.print(" , C2 ");
    Serial.print(Cam.give_Angle2());
    Serial.print(" , D "); 
    Serial.print((Cam.give_Angle1()+Cam.give_Angle2())/2);
    Serial.print(" , sq "); 
    Serial.println(sqrt(pow(Cam.give_Angle1()-3,2)+pow(Cam.give_Angle2()+3,2)));

    if((ESC.Enable) && (false)){
        if(INA.Current_DR()>Drib_schwelle){
            RGB.write(0,"R");
            Ball.catched=true;
        }
        else{
            RGB.write(0,"G");
            Ball.catched=false;
        }
    }
    else{
        Ball.catched = LDR.Aktiv();
        if(Ball.catched){RGB.write(0,"O");}
        else{RGB.write(0,"B");}
    }

    if((Serial.available()>0) && (false)){
        if(Serial.readString()=="giveCalib"){
            if(Color_ID){
                Serial.print("int Line_w_calib[] = {");
                for(int i = 0;i<40;i++){
                    Serial.print(Line_conf[i]);
                    if(i<39){Serial.print(",");}
                }
                Serial.println("};");


                Serial.print("int IR_mini_w_calib[] = ");
                for(int i = 0;i<16;i++){
                    Serial.print(IR_mini_conf[i]);
                    if(i<39){Serial.print(",");}
                }
                Serial.println("};");

                Serial.print("int IR_maxi_w_calib[] = ");
                for(int i = 0;i<16;i++){
                    Serial.print(IR_maxi_conf[i]);
                    if(i<39){Serial.print(",");}
                }
                Serial.println("};");

                Serial.print("int LDR_w_Calib = ");
                Serial.print(LDR_conf);
                Serial.println(";");
            }
            else{
                Serial.print("int Line_s_calib[] = {");
                for(int i = 0;i<40;i++){
                    Serial.print(Line_conf[i]);
                    if(i<39){Serial.print(",");}
                }
                Serial.println("};");


                Serial.print("int IR_mini_s_calib[] = ");
                for(int i = 0;i<16;i++){
                    Serial.print(IR_mini_conf[i]);
                    if(i<39){Serial.print(",");}
                }
                Serial.println("};");

                Serial.print("int IR_maxi_s_calib[] = ");
                for(int i = 0;i<16;i++){
                    Serial.print(IR_maxi_conf[i]);
                    if(i<39){Serial.print(",");}
                }
                Serial.println("};");

                Serial.print("int LDR_s_Calib = ");
                Serial.print(LDR_conf);
                Serial.println(";");
            }
            delay(10000);
        }
    }
    
    if(System.Start || BC.start || digitalRead(RCJ_Port)){ //
        /*if (BC.mode1) {Game.Run(); }
        else if (BC.mode2) {if (BC.controlActive) {Robot.Drive(BC.angle,0,BC.speed);}else{Game.Stop();}}
        else if (BC.mode3) {Robot.Turn(0);}
        else if (BC.mode4) {Robot.Turn(Cam.give_Angle1());}

        else if (!BC.mode5) {*/
            Game.Run();
        //}
    }
    else{
        Game.Stop();

        if (System.Switches[2] || BC.Sw3){ESC.set(30);}
        else{ESC.set(0);}
    }

    if(System.Button[0] || BC.Bt1 ){IR.Calib_Dist();} // BNO055 set to 0

    if (System.Button[1] || BC.Bt2 ){BNO055.Calibrate();} // Kicker test  Robot.Kicker.On();

    if (System.Button[2] || BC.Bt3 ){Line.Calibrate(0);} 
    else{}

    if (System.Button[3] || BC.Bt4 ){Line.Calibrate(1);} 
    else{}

    if (System.Switches[1] || BC.Sw2){IR.Calib_Offset();} 
    else{}

    

    if(BC.mode5){
        if (BC.calIrMax)// nichts
        {}
        if (BC.calIrMax)// nichts
        {}
        if (BC.calIrMax)// nichts
        {}
        if (BC.calIrMax)// nichts
        {}
        if (BC.calIrMax) // 
        {}
    }

    if (debugTimer >= DEBUG_INTERVAL_MS ) {
        debugTimer = 0;

        System.Update.Interface();
        RGB.Apply();

        //Debug.Start();
        //Debug.Plot_List("line",LineDef,aktive2,Line.Summe);
        //Debug.Plot("Summe",Line.Summe);
        //Debug.Send();

        //Debug.Start();
        //Debug.Plot("Hinten",US.giveNR(2));
        //Debug.Plot("Links",US.giveNR(3));
        //Debug.Plot("still",Ball.Stilltime);
        //Debug.Plot("dep",Line.dep);
        //Debug.Plot("cycl",Cycletime);
        //Debug.Send();

        //BC.sendTelemetryFloat("BNO",BNO055.giveDeg());
        //BC.sendTelemetryBool("Start",digitalRead(Start_Port));  
        //BC.sendListInt("list",Line.Values_raw,28);   
        //BC.led1=Ball.catched;
        //BC.led2=Line.Summe > 0;
        //BC.LedUpdate();
    }

    System.Update.Sensors();
    System.Update.Calculations();

    Robot.Kicker.Update_End();
    Cycletime=Cycle_Timer;
    //Serial.println("Cycle: "+String(Cycletime));
}

