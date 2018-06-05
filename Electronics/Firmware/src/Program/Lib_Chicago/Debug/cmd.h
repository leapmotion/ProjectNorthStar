/**
* @file cmd.h
*
* @brief Chicago UART command line interpreter _H
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

#ifndef __CMD_H__
	#define __CMD_H__

	//#############################################################################
	// Includes
	//-----------------------------------------------------------------------------
	
	
	//#############################################################################
	// Type Definitions
	//-----------------------------------------------------------------------------

	
	//#############################################################################
	// Variable Declarations
	//-----------------------------------------------------------------------------
	
	
	//#############################################################################
	// Function Prototypes
	//-----------------------------------------------------------------------------
	/**
	  * @brief 
	  *		Command line handler
	  * @details
	  *		Including this function in the main loop enables command line console
	  *		functionality over the primary UART port.
	  * @ingroup Chicago_Debug
	  * @return void
	  */
	void cmd(void);
	
#endif  /* __CMD_H__ */

