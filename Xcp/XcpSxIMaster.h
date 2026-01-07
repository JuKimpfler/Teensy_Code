#ifndef XCP_SXI_MASTER_H
#define XCP_SXI_MASTER_H

#include "XcpCfg.h"

class XcpSxIMaster
{
private:
    /* data */
    unsigned long baudrate;
    unsigned char buf[kXcpMaxCTO+2]; // + Header + Checksum
    unsigned char bufIdx;
    int Port2;

public:
    XcpSxIMaster(unsigned long baudrate, int Port);

    void Init();
    void Event(unsigned char event); 
    void BackgroudTask();
};

extern "C"
{

extern void ApplXcpSend(unsigned char len, const unsigned char * const pMsg);
extern unsigned char * ApplXcpGetPointer(unsigned char addr_ext, unsigned long addr);
extern void XcpMain_Init(int Port);
extern void XcpMain_sendMessage(uint16 len,uint8* pData);
extern void XcpMain_receiveData(uint16 len,uint8* pData );
}


#endif