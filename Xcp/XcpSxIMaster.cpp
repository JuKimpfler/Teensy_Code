#include "XcpSxIMaster.h"
#include "XcpBasic.h"
#include "Arduino.h"

// Xcp Test:
// Frage : "9A 02 FF 00 01" as Hex
// Antwort : "9A" , "lenght" , "lenght of lenght" , "Summe of lenght and (lenght of lenght)"

#define ESC_SYNC  1
#define ESC_ESC   0

#define XCPMAIN_BUFFERSIZE  (2*(kXcpMaxCTO+2+1))  // data + Header + Checksum + sync



XcpSxIMaster::XcpSxIMaster(unsigned long baudrate,int Port)
{
    baudrate = baudrate;
    Port2 = Port;
    if (Port2 == 0){
        Serial.begin(baudrate);
    }
    else if (Port2 == 7){
        Serial7.begin(baudrate);
    }
}

void XcpSxIMaster::Init()
{
    XcpMain_Init(Port2);
}

void XcpSxIMaster::Event(unsigned char event)
{
    XcpEvent(event);
}

void XcpSxIMaster::BackgroudTask()
{
    if (Port2 == 0){
        while (Serial.available())
        {
            uint8 b = Serial.read();
            XcpMain_receiveData(1,&b);
        }
    }
    else if (Port2 == 7){
        while (Serial3.available())
        {
            uint8 b = Serial7.read();
            XcpMain_receiveData(1,&b);
        }
    }
    
    XcpBackground();
    XcpSendCallBack();
}

extern "C"
{
    uint8 xcpm_sync = 0x9A;
    uint8 xcpm_esc = 0x9B;
    
    
    uint8 xcpm_txbuf[XCPMAIN_BUFFERSIZE]; 
    uint16 xcpm_txLen;
    uint8 xcpm_txValid;
    
    
    uint8 xcpm_rxbuf[XCPMAIN_BUFFERSIZE]; // + Header + Checksum
    static uint8 xcpm_rxEsc;
    static uint8 xcpm_rxSync;
    static uint16 xcpm_rxLen;
    int Port2;
    
    
    
    void XcpMain_Init(int Port)
    {
        XcpInit();
        xcpm_rxLen = 0;
        xcpm_rxSync = 0;
        xcpm_txValid = 0;
        xcpm_txLen = 0;
        Port2 = Port;
    }
    
    
    
    
    void XcpMain_receiveData(uint16 len,uint8* data)
    {
        uint16 i;
        uint8 b;
        
        for (i=0; i<len;i++) 
        {
            b = data[i];
            if (b == xcpm_sync) {
                xcpm_rxSync = 1;
                xcpm_rxLen = 0;
                xcpm_rxEsc = 0;
            } else if (xcpm_rxSync) {
                if (b == xcpm_esc) {
                    xcpm_rxEsc = 1;
                } else if (xcpm_rxEsc) {
                    xcpm_rxEsc = 0;
                    if (b == ESC_SYNC) {
                        xcpm_rxbuf[xcpm_rxLen++] = xcpm_sync;
                    } else if (b == ESC_ESC) {
                        xcpm_rxbuf[xcpm_rxLen++] = xcpm_esc;
                    } else {
                       xcpm_rxSync = 0;
                       xcpm_rxLen = 0;
                    }
                } else {
                    xcpm_rxbuf[xcpm_rxLen++] = b;
                }     
                if (xcpm_rxLen && (xcpm_rxLen ==  xcpm_rxbuf[0]+2)) {
                    uint8 checksum = 0;
                    uint16 j;
                    for (j = 0; j < xcpm_rxLen-1;j++)  {
                        checksum += xcpm_rxbuf[j];
                    }
                
                    if (checksum == xcpm_rxbuf[xcpm_rxLen-1]) {
                        XcpCommand((uint32 *)&xcpm_rxbuf[1]);
                    }
                    xcpm_rxSync = 0;
                    xcpm_rxLen = 0;
                    xcpm_rxEsc = 0;
                }
            }
        }
    }
    
    void XcpMain_addTxBuffer(uint8 data)
    {
        if (data==xcpm_sync) {
            xcpm_txbuf[xcpm_txLen++] = xcpm_esc;
            xcpm_txbuf[xcpm_txLen++] = ESC_SYNC;
        } else if (data==xcpm_esc) {
            xcpm_txbuf[xcpm_txLen++] = xcpm_esc;
            xcpm_txbuf[xcpm_txLen++] = ESC_ESC;
        } else {
            xcpm_txbuf[xcpm_txLen++] = data;
        }    
    }
    
    uint8 XcpMain_prepareTxBuffer(uint8 len, const uint8 * pMsg)
    {
        uint8 checksum;
        uint8 i;
        if ((len==0) || (len>kXcpMaxDTO)) return 0; // should not happen
    
        checksum = len;
        
        xcpm_txLen = 0;    
        xcpm_txbuf[xcpm_txLen++] = xcpm_sync; // Each packet begins with SYNC
        XcpMain_addTxBuffer(len);
        for (i = 0; i < len; i++)
        {
            checksum += pMsg[i];
            XcpMain_addTxBuffer(pMsg[i]);
        }
        XcpMain_addTxBuffer(checksum);
    
        return 1; 
    }
       
    // Transmit a XCP packet
    // Called by the XCP driver
    void ApplXcpSend(uint8 len, const uint8 * pMsg) 
    {
        if (XcpMain_prepareTxBuffer(len,pMsg)) {
            uint16 i;
            for (i=0;i<xcpm_txLen;i++) {
                if (Port2 == 0){
                    if (!Serial.write(xcpm_txbuf[i])) return ;
                }
                else if (Port2 == 7){
                    if (!Serial7.write(xcpm_txbuf[i])) return ;
                }
                
            }
        }
    }
        

    void ApplXcpInit(void)
    {
        ;
    }


    //------------------------------------------------------------------------------
    // Platform dependend functions

    // Convert a XCP address to a pointer
    unsigned char * ApplXcpGetPointer(unsigned char addr_ext, unsigned long addr) 
    {
        addr_ext = addr_ext;
        return (unsigned char *)addr;
    }

}
