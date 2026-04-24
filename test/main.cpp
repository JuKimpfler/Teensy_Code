#include "System.h"
#include "ESC.h"

void setup() {
    Wire1.begin();
    Wire1.setClock(I2C_SPEED);

    Expander.I2C.init(I2C_ITF_Main,Input_Mode,All_Off);
    delay(1000);
    Expander.I2C.read(I2C_ITF_Main);
    Color_ID = Expander.I2C.give(I2C_ITF_Main,ITF_Main_CID);

    SPI.begin();
    Serial.begin(115200);
    UART_2.begin(115200);
    UART_Pixy.begin(115200);

    pinMode(Start_Port,INPUT);
    pinMode(Kicker_Port, OUTPUT);
    pinMode(Mouse_Port,INPUT);
    pinMode(Kicker_Port, OUTPUT);
    pinMode(RCJ_Port,INPUT);
    System.begin(Color_ID);

    RGB.write(0,"Off");
    RGB.write(1,"Off");
    RGB.write(2,"Off");

    RGB.write(1,"R");  
    Serial.println("push button 3");
    RGB.Apply();
    ESC.init(33);
    delay(1000);
    while(!System.Button[2]){System.Update.Interface();Serial.println("waiting on power up");}
    delay(100);
    ESC.init_Power();
    ESC.set(15);
    RGB.write(1,"G");  
    Serial.println("ON!");
    RGB.Apply();
}

void loop() { 
    Cycle_Timer = 0;
    MainSpeed = 40;

    if(System.Start){
        //if(!Game.LineInterrupt()){
        //    int drive = U.Circel(((LUT.get_DriveAngle(U.Circel(Ball.Angle),Ball.Distance))));
        //    Robot.Drive(drive,0,20);
        //}
        //Debug.Start();
        //Debug.Plot("mx",Mouse.xPos);
        //Debug.Plot("my",Mouse.yPos);
        //Debug.Plot("px",PFU.giveX());
        //Debug.Plot("py",PFU.giveY());
        Serial.print(PFU.giveX());
        Serial.print(",");
        Serial.println(PFU.giveY());
        //Debug.Plot("l",US.Distance[0]);
        //Debug.Plot("h",US.Distance[1]);
        //Debug.Plot("r",US.Distance[2]);
        //Debug.Send();
        delay(20);
        
        Game.Stop();
    } 
    else{
        Game.Stop();
        //BC.sendTelemetryFloat("test",0.321);
    

        if(System.Button[0]){BNO055.Calibrate();} // BNO055 set to 0

        if (System.Button[1]){Robot.Kicker.On();} // Kicker test

        if (System.Button[2]){IR.Calib_Dist();} 
        else{}

    }

    System.Update.Calculations();
    System.Update.Sensors();
    System.Update.Interface();

    Robot.Kicker.Update_End();
    Cycletime=Cycle_Timer;
    //Serial.println("Cycle: "+String(Cycletime));
}