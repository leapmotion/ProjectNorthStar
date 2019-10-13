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

#include <Arduino.h>
#include <variant.h>

#include "../config.h"

extern unsigned char  g_SerialSendBuf[SERIAL_SEND_BUF_SIZE];
extern unsigned char *g_pSerialSendHead;
extern const unsigned char * const g_pSerialSendTail;
extern const uint8_t g_bSerialBusy;
extern const char g_bPutcharEnable;

/*
void putc(char c){
	Serial.write(c);
	
    unsigned char *pNext; 
    
    *g_pSerialSendHead = c;
    if(g_pSerialSendHead == (g_SerialSendBuf + SERIAL_SEND_BUF_SIZE - 1))
    {
        pNext = &g_SerialSendBuf[0];
    }
	
    else
    {
        pNext = g_pSerialSendHead + 1;
    }
    while(pNext == g_pSerialSendTail)
    {
        // buffer full, wait for at least one byte space
    }
    g_pSerialSendHead = pNext;
	
    if(!g_bSerialBusy){
		// serial tx is idle
        TRIGGER_TRANSMIT(); // start transmition loop by triggering the serial interrupt
    }
}
*/

char putchar(char c){
    if(g_bPutcharEnable){
		// putchar function enabled
        
		Serial.print(c);
		
		/*
		if(c == '\n'){
            putc('\r'); // insert \r
        }
        putc(c);
		*/
    }
    
    return c;
}

