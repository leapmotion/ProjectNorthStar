/******************************************************************************

Copyright (c) 2016, Analogix Semiconductor, Inc.

PKG Ver  : V0.1

Filename : 

Project  : Chicago 

Created  : 11 Nov. 2016

Devices  : Chicago

Toolchain: Keil
 
Description:

Revision History:

******************************************************************************/

#include "../config.h"

extern unsigned char /*xdata*/ g_SerialRecvBuf[SERIAL_RECV_BUF_SIZE];
extern unsigned char /*xdata*/ *g_pSerialRecvHead;
extern const unsigned char /*xdata*/ * const g_pSerialRecvTail;
extern unsigned char /*xdata*/ g_SerialSendBuf[SERIAL_SEND_BUF_SIZE];
extern const unsigned char /*xdata*/ * const g_pSerialSendHead;
extern unsigned char /*xdata*/ *g_pSerialSendTail;

bit g_bSerialBusy;

void SerialISR(void) interrupt 4
{
    if(CHECK_TRANSMIT_INTERRUPT())
    {
        CLEAR_TRANSMIT_INTERRUPT();
        if(g_pSerialSendTail != g_pSerialSendHead)  // check if send buffer empty
        {// buffer not empty
            WRITE_SBUF(*g_pSerialSendTail);
            if(g_pSerialSendTail == (g_SerialSendBuf + SERIAL_SEND_BUF_SIZE - 1))
            {
                g_pSerialSendTail = &g_SerialSendBuf[0];
            }
            else
            {
                g_pSerialSendTail++;
            }
            g_bSerialBusy = 1;  // busy
        }
        else
        {
            g_bSerialBusy = 0;  // idle
        }
    }
    if(CHECK_RECEIVE_INTERRUPT())
    {
        CLEAR_RECEIVE_INTERRUPT();
        *g_pSerialRecvHead = READ_SBUF();
        if(g_pSerialRecvHead == (g_SerialRecvBuf + SERIAL_RECV_BUF_SIZE - 1))
        {
            g_pSerialRecvHead = &g_SerialRecvBuf[0];
        }
        else
        {
            g_pSerialRecvHead++;
        }
        
		while(g_pSerialRecvHead == g_pSerialRecvTail)  // check if receive buffer full
        {
			// buffer full
			// receive buffer overflow!
        }
    }
}

