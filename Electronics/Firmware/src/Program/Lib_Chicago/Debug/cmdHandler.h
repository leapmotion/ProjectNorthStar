/**
* @file cmdHandler.h
*
* @brief Chicago UART command line interpreter functions _H
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

/**
* @defgroup Chicago_cmdline [Functions] Chicago debugger command line
* @details
*	These functions may be called from the UART command terminal in with
*	a preceding \\backslash and trailing newline. They may be used to
*	inspect the state of the OCM and modify other parameters in the
*	Chicago state machine. Use \\man command to find out what functions
*	are supported, and \\help to discover further details about a
*	particular function.
* @note
*	Further documentation can be found in ANX753X_Programming_Guide.pdf, 
*	page 52 about these commands and examples of their use.
*/

#ifndef CMDHANDLER_H
	#define CMDHANDLER_H

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
	  *		Operate on incoming commands from serial buffer, called from cmd();
	  * @ingroup Chicago_Debug
	  * @return void
	  */			
	void CmdHandler(void);
	
	/**
	  * @brief 
	  *		Print top-level help information
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\help
	  * @return void
	  */	
	static void help(void);
	
	/**
	  * @brief 
	  *		Print detailed help information
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\man (command)
	  * @return void
	  */	
	static void man(void);

	/**
	  * @brief 
	  *		Reads interrupt registers
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\rdint
	  * @return void
	  */	
	static void rdint(void);

	/**
	  * @brief 
	  *		Clears interrupt registers
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\clrint
	  * @return void
	  */	
	static void clrint(void);

	/**
	  * @brief 
	  *		Read a register, 1 byte
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\rd (slaveID) (register offset)
	  * @return void
	  */	
	static void rd(void);
	
	/**
	  * @brief 
	  *		Read a register, 4 bytes
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\rd4 (slave id) (register offset)
	  * @return void
	  */		
	static void rd4(void);

	/**
	  * @brief 
	  *		Write to a register, 1 byte
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\wr (slave id) (register offset) (value_to_be_written)
	  * @return void
	  */		
	static void wr(void);
	
	/**
	  * @brief 
	  *		Write to a register, 4 bytes
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\wr4 (slave id) (register offset) (value_to_be_written)
	  * @return void
	  */			
	static void wr4(void);

	/**
	  * @brief 
	  *		Delay some time, milliseconds
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\delay (milliseconds)
	  * @return void
	  */	
	static void delay(void);

	/**
	  * @brief 
	  *		Dump a block of registers
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\dump (SlaveID|Offset[11:8])
	  * @return void
	  */		
	static void dump(void);

	/**
	  * @brief 
	  *		Chicago power restart
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\power_restart
	  * @return void
	  */		
	static void power_restart(void);
	
	/**
	  * @brief 
	  *		Chicago power on state
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\poweron
	  * @return void
	  */	
	static void poweron(void);
	
	/**
	  * @brief 
	  *		Chicago power off state
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\poweroff
	  * @return void
	  */		
	static void poweroff(void);
	
	/**
	  * @brief 
	  *		Force CHIP_POWER_UP to high
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\chippowerup
	  * @return void
	  */		
	static void chippowerup(void);
	
	/**
	  * @brief 
	  *		Force CHIP_POWER_UP to low
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\chippowerdown
	  * @return void
	  */			
	static void chippowerdown(void);
	
	/**
	  * @brief 
	  *		Force RESET to high (off)
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\resetup
	  * @return void
	  */			
	static void resetup(void);
	
	/**
	  * @brief 
	  *		Force RESET to low (on)
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\resetdown
	  * @return void
	  */			
	static void resetdown(void);
	
	/**
	  * @brief 
	  *		Show MIPI related registers
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\showmipi
	  * @return void
	  */		
	static void show_mipi(void);
	
	/**
	  * @brief 
	  *		Show MIPI TX port related registers
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\showmipitx (port:0-3)
	  * @return void
	  */		
	static void show_mipi_tx(void);
	
	/**
	  * @brief 
	  *		Show Displayport RX related registers
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\showdprx
	  * @return void
	  */			
	static void show_dprx(void);
	
	/**
	  * @brief 
	  *		Turn on panel power supply and RESET control sequence (doesn't include DCS)
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\panelon
	  * @return void
	  */			
	static void panelon(void);
	
	/**
	  * @brief 
	  *		Turn off panel power supply and RESET control sequence
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\paneloff
	  * @return void
	  */			
	static void paneloff(void);
	
	/**
	  * @brief 
	  *		Restarts the Chicago on chip microcontroller
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\startocm
	  * @return void
	  */			
	static void start_ocm(void);
	
	/**
	  * @brief 
	  *		Reflashes the Chicago on chip micro controller
	  * @ingroup Chicago_cmdline
	  * @note Command line usage: \\burnhex (combination value of partition IDs, value from 0x1 to 0x7)
	  * @return void
	  */		
	static void burnhex(void);
	
	/**
	  * @brief 
	  *		Makes a char array lower case
	  * @ingroup Chicago_Debug_Internal
	  * @param p - Char array to be modified
	  * @return void
	  */		
	static void MakeLower(uint8_t* p);

	/**
	  * @brief 
	  *		Prints some boring info about Chicago
	  * @ingroup Chicago_Debug_Internal
	  * @return void
	  */		
	static void Chicago_info(void);

	//uint8_t sp_tx_aux_dpcdread_bytes(uint8_t addrh, uint8_t addrm, uint8_t addrl, uint8_t cCount, uint8_t *pBuf);

#endif  // end of CMDHANDLER_H definition
