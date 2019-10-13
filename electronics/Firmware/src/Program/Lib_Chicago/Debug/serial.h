/**
* @file serial.h
*
* @brief Chicago UART communication helper functions _H
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

#ifndef __SERIAL_H__
	#define __SERIAL_H__

	//#############################################################################
	// Pre-compiler Definitions
	//-----------------------------------------------------------------------------


	//#############################################################################
	// Type Definitions
	//-----------------------------------------------------------------------------


	//#############################################################################
	// Function Prototypes
	//-----------------------------------------------------------------------------
	
	/**
	  * @brief 
	  *		Read one char from serial buffer
	  * @ingroup Chicago_Debug_Internal
	  * @return uint8_t - Read character
	  */	
	uint8_t deque(void);
	
	/**
	  * @brief 
	  *		Call to receive one byte from serial buffer and act upon it.
	  * @details
	  *		Acts upon control characters, discards backspace and arrow keys, as well
	  *		as echos received commands back to the terminal.
	  * @ingroup Chicago_Debug_Internal
	  * @return RETURN_NORMAL_VALUE if success
	  */
	uint8_t SerialRecv(void);
	
#endif  /* __SERIAL_H__ */

