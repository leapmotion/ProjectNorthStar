/**
* @file serial.cpp
*
* @brief Chicago UART communication helper functions
*
* @copyright
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* @copyright
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* @author Analogix, Inc
* @author Arduino, LLC
* @author Adam Munich
*/

//#############################################################################
// Includes
//-----------------------------------------------------------------------------
#include <Arduino.h>
#include <variant.h>

#include "./serial.h"
#include "./debug.h"

#include "../Chicago/chicago_config.h"


//#############################################################################
// Pre-compiler Definitions
//-----------------------------------------------------------------------------
#define IS_RECV_EMPTY()			!Serial.available()
#define IS_RECV_NOT_EMPTY()		Serial.available()
#define GETC()					Serial.read()
#define PUTC(t)					Serial.write(t)


//#############################################################################
// Variable Declarations
//-----------------------------------------------------------------------------
uint8_t g_SerialRecvBuf[SERIAL_RECV_BUF_SIZE];
uint8_t *g_pSerialRecvHead;
uint8_t *g_pSerialRecvTail;
uint8_t g_SerialSendBuf[SERIAL_SEND_BUF_SIZE];
uint8_t *g_pSerialSendHead;
uint8_t *g_pSerialSendTail;
uint8_t g_bPutcharEnable;
uint8_t g_CmdLineBuf[CMD_LINE_SIZE];


//#############################################################################
// Function Definitions
//-----------------------------------------------------------------------------
uint8_t deque(void){
    uint8_t c;
	
	c = Serial.read();
	
    return c;
}

//-----------------------------------------------------------------------------
uint8_t SerialRecv(void){
    static uint8_t i;
    uint8_t c;
    uint8_t RetVal;
	
    while(IS_RECV_NOT_EMPTY()){
        g_bPutcharEnable	= 1; // enable output
        c					= deque();
		
        // control character
        //if(c == '\b'){// backspace
        //    PUTC(c); // echo
        //    PUTC(' '); // space
        //    PUTC(c);
        //    if(i != 0){
        //        i--;
        //    }
        //    g_bPutcharEnable = 0; // disable output
        //}
				
		// Backspace: do nothing
		if(c == '\b'){
			while(IS_RECV_NOT_EMPTY()){
				GETC();	
			}			
		} 
		
		// Arrow keys: do nothing
		else if(c == 27){
			while(IS_RECV_NOT_EMPTY()){
				GETC();	
			}
		} 
		
		else if(c == '\r'){// CR

            if(IS_RECV_EMPTY()){
				
                //DELAY_US(US_PER_CHARACTER);
                if(IS_RECV_NOT_EMPTY()){
                    if(GETC() == '\n'){
                        deque(); // discard \n
                    }
                }
            }
            else{
                if(GETC() == '\n'){
                    deque(); // discard \n
                }
            }
			
            PUTC('\n'); // echo
            g_CmdLineBuf[i]		= '\0'; // end of string
            RetVal				= i;
            i					= 0;
            return RetVal;    // return the string length
        }
		
        else if(c == '\n'){// LF

            if(IS_RECV_EMPTY()){
				
                //DELAY_US(US_PER_CHARACTER);
                if(IS_RECV_NOT_EMPTY()){
                    if(GETC() == '\r'){
                        deque(); // discard \r
                    }
                }
            }
			
            else{
                if(GETC() == '\r'){
                    deque(); // discard \r
                }
            }
			
            PUTC('\n'); // echo
            g_CmdLineBuf[i]		= '\0'; // end of string
            RetVal				= i;
            i					= 0;
            return RetVal;    // return the string length
        }
		
        else{// receive command data
            if(i < CMD_LINE_SIZE - 1){// command line buffer not full
                if(c == '\\'){// command header
                    // wait for all data sent
                    //while(g_bSerialBusy) ;
                }
                PUTC(c); // echo
                g_CmdLineBuf[i]		= c;
                i++;
            }
            else{// command line buffer full
                // discard the character
            }
            g_bPutcharEnable	= 0; // disable output
        }
    }
    return RETURN_NORMAL_VALUE;
}

