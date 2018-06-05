/**
* @file debug.cpp
*
* @brief Chicago UART debugger
*
* @details 
*		Include this file to enable UART debugging of the Chicago chipset.
*		There are 3 debug verbosity levels, the default level is 2.
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
#include <stdint.h>
#include "./debug.h"


//#############################################################################
// Variable Declarations
//-----------------------------------------------------------------------------


//#############################################################################
// Function Definitions
//-----------------------------------------------------------------------------
void TraceArray(uint8_t array[], uint8_t len){
	unsigned char i;

	i = 0;
	while(1){
		TRACE1("%02X", array[i]);
		i++;
		if (i != len){
			if (i%16 == 0){
				TRACE("\n");
			}
			else{
				TRACE(" ");
			}
		}
		else{
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void TraceArrayLittleEndian(uint8_t array[], uint8_t len){
	unsigned char i;

	i = 0;
		
	while(1){
		TRACE1("%02X", array[len-i-1]);
		i++;
		if (i != len){
			if (i%16 == 0){
				TRACE("\n");
			}
		}
		else{
			break;
		}
	}
}

