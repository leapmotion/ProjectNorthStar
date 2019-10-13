/**
* @file cmd.cpp
*
* @brief Chicago UART command line interpreter
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
* @author Adam Munich
*/

//#############################################################################
// Includes
//-----------------------------------------------------------------------------
#include <string.h>
#include <stdint.h>

#include "./cmd.h"

#include "./debug.h"
#include "./serial.h"
#include "./cmdHandler.h"

#include "../Chicago/chicago_config.h"


//#############################################################################
// Variable Declarations
//-----------------------------------------------------------------------------
extern uint8_t g_CmdLineBuf[CMD_LINE_SIZE];
extern const uint8_t g_bPutcharEnable;


//#############################################################################
// Function Definitions
//-----------------------------------------------------------------------------
void cmd(void){
	volatile uint8_t ser = SerialRecv();
    if(ser != 0){
        CmdHandler();
    }
}

