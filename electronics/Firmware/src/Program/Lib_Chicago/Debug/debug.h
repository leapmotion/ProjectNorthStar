/**
* @file debug.h
*
* @brief Chicago UART debugger _H
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

/**
* @defgroup Chicago_debug [Functions] Chicago debugger
* @details
*	These functions print debugging info to the serial console. For them to work,
*	"debug.h" must be included somewhere in the header file so TRACE macros are
*	defined.
*/

/**
* @defgroup Chicago_debug_internal [Functions] Chicago debugger (internal fns)
* @details
*	These functions are internal functions for debug functionality. They don't
*	really need to be called by things outside of the debug environment.
*/


#ifndef __DEBUG_H__
	#define __DEBUG_H__
	
	//#############################################################################
	// Includes
	//-----------------------------------------------------------------------------
	#include <Arduino.h>
	#include <variant.h>
	#include <stdio.h>


	//#############################################################################
	// Pre-compiler Definitions
	//-----------------------------------------------------------------------------
	//#define DEBUG_LEVEL_3
	#define DEBUG_LEVEL_2
	//#define DEBUG_INTERRUPTS

	///	@brief Alias for sprintf with 0 args
	///	@ingroup Chicago_debug_internal
	#define TRACE(format)   \
		Serial.printf(format)

	/// @brief Alias for sprintf with 0 args
	///	@ingroup Chicago_debug_internal
	#define TRACE0(format)  \
		Serial.printf(format)
	
	///	@brief Alias for sprintf with 1 arg
	///	@ingroup Chicago_debug_internal
	#define TRACE1(format, arg1)    \
		Serial.printf(format, arg1)
	
	///	@brief Alias for sprintf with 2 args
	///	@ingroup Chicago_debug_internal
	#define TRACE2(format, arg1, arg2)  \
		Serial.printf(format, arg1, arg2)
	
	///	@brief Alias for sprintf with 3 args
	///	@ingroup Chicago_debug_internal
	#define TRACE3(format, arg1, arg2, arg3)    \
		Serial.printf(format, arg1, arg2, arg3)
	
	///	@brief Alias for sprintf with 4 args
	/// @ingroup Chicago_debug_internal
	#define TRACE4(format, arg1, arg2, arg3, arg4)  \
		Serial.printf(format, arg1, arg2, arg3, arg4)
	
	///	@brief Alias for sprintf with 5 args
	///	@ingroup Chicago_debug_internal
	#define TRACE5(format, arg1, arg2, arg3, arg4, arg5)    \
		Serial.printf(format, arg1, arg2, arg3, arg4, arg5)
	
	///	@brief Alias for sprintf with 6 args
	///	@ingroup Chicago_debug_internal
	#define TRACE6(format, arg1, arg2, arg3, arg4, arg5, arg6)  \
		Serial.printf(format, arg1, arg2, arg3, arg4, arg5, arg6)
	
	///	@brief Alias for sprintf with 7 args
	///	@ingroup Chicago_debug_internal
	#define TRACE7(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7)    \
		Serial.printf(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
	
	///	@brief Alias for sprintf with 8 args
	///	@ingroup Chicago_debug_internal
	#define TRACE8(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)  \
		Serial.printf(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
	
	///	@brief Alias for sprintf with 9 args
	///	@ingroup Chicago_debug_internal
	#define TRACE9(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)    \
		Serial.printf(format, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)

	#define ASSERT(expr)  \
		if(expr){}  \
		else{  \
			TRACE2("Assert failed: " #expr " (file %s line %d)\n", __FILE__, (int) __LINE__ );  \
			while (1);  \
		}
	
	///	@brief Print out an array of characters to serial buffer, big endian
	///	@ingroup Chicago_debug_internal	
	#define TRACE_ARRAY(array, len) \
		TraceArray(array, len)

	///	@brief Print out an array of characters to serial buffer, little endian
	///	@ingroup Chicago_debug_internal	
	#define TRACE_ARRAYI(array, len) \
		TraceArrayLittleEndian(array, len)
					
					
	//#############################################################################
	// Type Definitions
	//-----------------------------------------------------------------------------


	//#############################################################################
	// Function Prototypes
	//-----------------------------------------------------------------------------
	/**
	* @brief 
	*		Trace an array output for debugging, big endian
	* @details
	*		The array size must be no more than 256. If the size is 256 set len to 0.
	* @ingroup Chicago_debug_internal
	* @param array - The array name or address to trace output
	* @param len - The number of bytes to trace output
	* @return void
	*/		
	void TraceArray(uint8_t array[], uint8_t len);
	
	/**
	* @brief 
	*		Trace an array output for debugging, little endian
	* @details
	*		The array size must be no more than 256. If the size is 256 set len to 0.
	* @ingroup Chicago_debug_internal
	* @param array - The array name or address to trace output
	* @param len - The number of bytes to trace output
	* @return void
	*/			
	void TraceArrayLittleEndian(uint8_t array[], uint8_t len);

#endif  /* __DEBUG_H__*/

