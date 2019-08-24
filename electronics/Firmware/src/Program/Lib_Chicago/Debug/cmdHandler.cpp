/**
* @file cmdHandler.cpp
*
* @brief Chicago UART command line interpreter functions
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
#include <stdint.h>
#include <string.h>
#include <stdint.h>

#include "./cmdHandler.h"

#include "./debug.h"

#include "../Chicago/chicago_config.h"
#include "../Chicago/chicago.h"
#include "../I2C/i2c.h"


//#############################################################################
// Pre-compiler Definitions
//-----------------------------------------------------------------------------
//bit g_bBurnHex;
//#define AUX_ERR  1
//#define AUX_OK   0
//#define DPCD_LANE0_1_STATUS  0x02
//#define DPCD_LANE_ALIGN_UD  0x04
//#define MAX_BUF_CNT 16
//uint8_t  bytebuf[16];
//#define write_dpcd_addr(addrh, addrm, addrl) do{ }while(0)
//#define sp_tx_addronly_set(enable) reg_bit_ctl(TX_P0, AUX_CTRL2, ADDR_ONLY_BIT, enable)
//#define reg_bit_ctl(addr, offset, data, enable) do{ }while(0)


//#############################################################################
// Variable Declarations
//-----------------------------------------------------------------------------
extern uint8_t g_bDebug;
extern uint8_t g_CmdLineBuf[CMD_LINE_SIZE];
static uint8_t NameTemp[CMD_NAME_SIZE];

// for flash OCM
uint8_t g_bFlashWrite = 0;

#ifdef FALSH_READ_BACK
	uint8_t g_bFlashResult = 0;
#endif


//#############################################################################
// Function Definitions
//-----------------------------------------------------------------------------
void CmdHandler(void){
	uint8_t  CommandName[CMD_NAME_SIZE];
	uint8_t  current_version[3];
	uint8_t  hex_version[3];
	uint8_t reg_temp;
	
	uint32_t  size_to_be_read;	
	
	{
		if (sscanf((const char *)g_CmdLineBuf, "\\%15s", CommandName) == 1)
		{
			MakeLower(CommandName);
			if (strcmp((const char *)CommandName, "help") == 0)
			{
				help();
			}
			else if (strcmp((const char *)CommandName, "man") == 0)
			{
				man();
			}
			else if (strcmp((const char *)CommandName, "clrint") == 0)
			{
				clrint();
			}
			else if (strcmp((const char *)CommandName, "rdint") == 0)
			{
				rdint();
			}
			else if (strcmp((const char *)CommandName, "rd") == 0)
			{
				rd();
			}
			else if (strcmp((const char *)CommandName, "rd4") == 0)
			{
				rd4();
			}
			else if (strcmp((const char *)CommandName, "wr") == 0)
			{
				wr();
			}
			else if (strcmp((const char *)CommandName, "wr4") == 0)
			{
				wr4();
			}
			else if (strcmp((const char *)CommandName, "delay") == 0)
			{
				delay();
			}
			else if (strcmp((const char *)CommandName, "dump") == 0)
			{
				dump();
			}
			else if (strcmp((const char *)CommandName, "poweron") == 0)
			{
				poweron();
			}
			else if (strcmp((const char *)CommandName, "poweroff") == 0)
			{
				poweroff();
			}
			else if (strcmp((const char *)CommandName, "chippowerup") == 0)
			{
				chippowerup();
			}
			else if (strcmp((const char *)CommandName, "chippowerdown") == 0)
			{
				chippowerdown();
			}
			else if (strcmp((const char *)CommandName, "resetup") == 0)
			{
				resetup();
			}
			else if (strcmp((const char *)CommandName, "resetdown") == 0)
			{
				resetdown();
			}
			else if (strcmp((const char *)CommandName, "debugon") == 0)
			{
				TRACE("\tturn OFF state machine\n");
				g_bDebug=1;
				chicago_stop_ocm();
			}
			else if (strcmp((const char *)CommandName, "debugoff") == 0)
			{
				start_ocm();
				TRACE("\tturn ON state machine\n");
				g_bDebug=0;
			}
			else if (strcmp((const char *)CommandName, "stopocm") == 0)
			{
				chicago_stop_ocm();
			}
			else if (strcmp((const char *)CommandName, "startocm") == 0)
			{
				start_ocm();
			}
			else if (strcmp((const char *)CommandName, "showmipi") == 0)
			{
				show_mipi();
			}
			else if (strcmp((const char *)CommandName, "showmipitx") == 0)
			{
				show_mipi_tx();
			}
			else if (strcmp((const char *)CommandName, "showdprx") == 0)
			{
				show_dprx();
			}
			else if (strcmp((const char *)CommandName, "panelon") == 0)
			{
				panelon();
			}
			else if (strcmp((const char *)CommandName, "paneloff") == 0)
			{
				paneloff();
			}
			else if (strcmp((const char *)CommandName, "erase") == 0)
			{
				power_restart();
				//delay_ms(10);
			}
			else if (strcmp((const char *)CommandName, "readintr") == 0)
			{
				chicago_read_intr_state();
			}

/*
#if 0
			else if (strcmp((const char *)CommandName, "dpcdw") == 0)
			{
				if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x %x %x %x", &addrh, &addrm, &addrl, &c)==4)
				{
					//sp_tx_aux_dpcdwrite_bytes(addrh, addrm, addrl, 1, &c);
				}
			}
			else if (strcmp((const char *)CommandName, "dpcdr") == 0)
			{
				// if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x %x %x", &addrh, &addrm, &addrl, &c)==3)
				//{
				//	sp_tx_aux_dpcdread_bytes(addrh, addrm, addrl, 1, &c);
				//}
				if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x %x %x %x", &addrh, &addrm, &addrl, &c)==4)
				{
					uint8_t buff[16];
					if (c >16)
						c =1;

					sp_tx_aux_dpcdread_bytes(addrh, addrm, addrl, c, buff);
				}
			}
			else if (strcmp((const char *)CommandName, "errchk") == 0)
			{
				errchk();
			}
			else if (strcmp((const char *)CommandName, "fl_be_32k") == 0)
			{
				//command_flash_BE_32K();
			}
			else if (strcmp((const char *)CommandName, "fl_be_64k") == 0)
			{
				//command_flash_BE_64K();
			}
			else if (strcmp((const char *)CommandName, "fl_pd") == 0)
			{
				//command_flash_PD();
			}
			else if (strcmp((const char *)CommandName, "fl_rddid") == 0)
			{
				//command_flash_read_device_ID();
			}
			else if (strcmp((const char *)CommandName, "fl_rdid") == 0)
			{
				//command_flash_RDID();
			}
			else if (strcmp((const char *)CommandName, "dpi") == 0)
			{
				uint8_t table_id;
				 
				if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x", &table_id) == 1)
				{
					TRACE1("DPI input configure table id %d\n", (uint)table_id);
				}
				else 
				{
					TRACE("\tBad parameter! Usage:\n");
				}
			}
			else if (strcmp((const char *)CommandName, "dsi") == 0)
			{
				uint8_t table_id;
				 
				if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x", &table_id) == 1)
				{
					TRACE1("\tDSI input configure table id %d\n", (uint)table_id);
				}
				else 
				{
					TRACE("\tBad parameter! Usage:\n");
				}
			}
			else if (strcmp((const char *)CommandName, "dsc") == 0)
			{
				uint8_t table_id;
				 
				if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x", &table_id) == 1)
				{
					TRACE1("\tDPI +DSC input configure table id %d\n", (uint)table_id);
				}
				else 
				{
					TRACE("\tBad parameter! Usage:\n");
				}
			}
			else if (strcmp((const char *)CommandName, "audio") == 0)
			{
				uint8_t table_id;
				 
				if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x", &table_id) == 1)
				{
					TRACE1("\tAudio input configure table id %d\n", (uint)table_id);
				}
				else 
				{
					TRACE("\tBad parameter! Usage:\n");
				}
			}
			else if (strcmp((const char *)CommandName, "load0") == 0)
			{
				uint8_t c;
				TRACE("\tload secure ocm flash\n");
				//flash_section_loader(0x52, HDCP22TXFW_START_ADDR, HDCP22TXFW_SIZE);
				delay_ms(100);
				//WriteReg(TX_P0, 0x80, 0x10); //enable secure ocm
				//ReadReg(RX_P0, R_RAM_CTRL, &c);
				TRACE1("\tload status %x\n", (uint)c);

			}
			else if (strcmp((const char *)CommandName, "load1") == 0)
			{
				uint8_t c;
				TRACE("\tload HDCP key flash\n");
				//flash_section_loader(0x12, HDCP14KEY_START_ADDR, HDCP14KEY_SIZE);
				delay_ms(100);
				//ReadReg(RX_P0, R_RAM_CTRL, &c);
				TRACE1("\tload status %x\n", (uint)c);
			}
			else if (strcmp((const char *)CommandName, "fusew") == 0)
			{
				if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x %x", &addrh, &addrl)==2)
				{
					//Write fuse data to buffer
					//WriteReg(0x7e, 0x0a, addrh);
					//WriteReg(0x7e, 0x0c, addrl);
					//Send flash write enable and write fuse_write
					//WriteReg(0x7e, 0x36, 0x06);
					//WriteReg(0x7e, 0x33, 0x40);
					//WriteReg(0x7e, 0x00, 0x0a);
					//ReadReg(0x7e, 0x00, &addrm);
					while((addrm&0x04)==0x04)
					{
						//ReadReg(0x7e, 0x00, &addrm);
					}
					TRACE("\tFuse data write done\n");
				}
			}
#endif
*/
			else
			{
				TRACE("\tUnsupported command!\n\n");
				help();
			}
		}
	}
}

//-----------------------------------------------------------------------------
/// @copydoc help
static void help(void)
{
    TRACE("\tSupport Command List:\n");
    TRACE("\t\\help \\man \\rdint \\clrint \\rd \\rd4 \\wr \\wr4 \\delay \\dump \n");
    TRACE("\t\\poweron \\poweroff \\debugon \\debugoff \\chippowerup \\chippowerdown \n");
	TRACE("\t\\resetup \\resetdown \\showmipi \\showmipitx \\showdprx \\panelon\n");
    TRACE("\t\\paneloff \\stopocm \\startocm \\readintr \n\n");	
}

//-----------------------------------------------------------------------------
/// @copydoc man
static void man(void)
{
	uint8_t CommandName[CMD_NAME_SIZE];

	if (sscanf((const char *)g_CmdLineBuf, (const char *)"\\%*s %s", CommandName) == 1)
	{
		TRACE("\n");
		MakeLower(CommandName);
		if (strcmp((const char *)CommandName, (const char *)"help") == 0)
		{
			TRACE("\tCommand: help\n");
			TRACE("\tFunction: print top-level help information\n");
			TRACE("\tUsage: \\help\n");
		}
		else if (strcmp((const char *)CommandName, (const char *)"man") == 0)
		{
			TRACE("\tCommand: man\n");
			TRACE("\tFunction: print detailed help information\n");
			TRACE("\tUsage: \\man <command>\n");
			TRACE("\tExample: \\man rd\n");
		}
		else if (strcmp((const char *)CommandName, (const char *)"clrint") == 0)
		{
			TRACE("\tCommand: clrint\n");
			TRACE("\tFunction: clear interrupt states\n");
			TRACE("\tUsage: \\clrint\n");
		}
		else if (strcmp((const char *)CommandName, (const char *)"rdint") == 0)
		{
			TRACE("\tCommand: rdint\n");
			TRACE("\tFunction: read interrupt states\n");
			TRACE("\tUsage: \\rdint\n");
		}
		else if (strcmp((const char *)CommandName, (const char *)"rd") == 0)
		{
			TRACE("\tCommand: rd\n");
			TRACE("\tFunction: read a register\n");
			TRACE("\tUsage: \\rd <Slave ID> <Offset>\n");
			TRACE("\tExample: \\rd C0 2CE\n\n");

			TRACE("\tSalve ID: in HEX format, but without 0x prefix; 8-bit\n");
			TRACE("\tOffset: in HEX format, but without 0x prefix; 12-bit\n\n");

			TRACE("\t note: Silicon Labs EFM8UB20F64G MCU has 2 I2C master modules.\n");
			TRACE("\t       rd command is to read an Chicago register, i.e. rd command always operates I2C master 0.\n");
			Chicago_info();
		}
		else if (strcmp((const char *)CommandName, "rd4") == 0)
		{
			TRACE("\tCommand: rd4\n");
			TRACE("\tFunction: read 4 bytes from register\n");
			TRACE("\tUsage: \\rd4 <Slave ID> <Offset>\n");
			TRACE("\tExample: \\rd4 C0 2CE\n\n");

			TRACE("\tSalve ID: in HEX format, but without 0x prefix; 8-bit\n");
			TRACE("\tOffset: in HEX format, but without 0x prefix; 12-bit\n\n");
			Chicago_info();
		}
		else if (strcmp((const char *)CommandName, "wr") == 0)
		{
			TRACE("\tCommand: wr\n");
			TRACE("\tFunction: write a register\n");
			TRACE("\tUsage: \\wr <Slave ID> <Offset> <value_to_be_written>\n");
			TRACE("\tExample: \\wr E0 2AC 5A\n\n");

			TRACE("\tSalve ID: in HEX format, but without 0x prefix; 8-bit\n");
			TRACE("\tOffset: in HEX format, but without 0x prefix; 12-bit\n");
			TRACE("\tvalue_to_be_written: value to be written into the register; in HEX format, but without 0x prefix; 8-bit\n\n");

			TRACE("\t note: Silicon Labs EFM8UB20F64G MCU has 2 I2C master modules.\n");
			TRACE("\t       wr command is to write an Chicago register, i.e. wr command always operates I2C master 0.\n");
			Chicago_info();
		}
		else if (strcmp((const char *)CommandName, "wr4") == 0)
		{
			TRACE("\tCommand: wr4\n");
			TRACE("\tFunction: write a 4 bytes data to register\n");
			TRACE("\tUsage: \\wr4 <Slave ID> <Offset> <value_to_be_written>\n");
			TRACE("\tExample: \\wr4 E0 2AC 5A5A00CE\n\n");

			TRACE("\tSalve ID: in HEX format, but without 0x prefix; 8-bit\n");
			TRACE("\tOffset: in HEX format, but without 0x prefix; 12-bit\n");
			TRACE("\tvalue_to_be_written: value to be written into the register; in HEX format, but without 0x prefix; 32-bit\n\n");
			Chicago_info();
		}
		else if (strcmp((const char *)CommandName, "delay") == 0)
		{
			TRACE("\tCommand: delay\n");
			TRACE("\tFunction: delay ms\n");
			TRACE("\tUsage: \\delay <ms>\n");
			TRACE("\tExample: \\delay 1000\n\n");

			TRACE("\tms: in decimal format, max 65535\n");
			Chicago_info();
		}
		else if (strcmp((const char *)CommandName, "dump") == 0)
		{
			TRACE("\tCommand: dump\n");
			TRACE("\tFunction: dump all registers in a specified I2C SlaveID|Offset[11:8]\n");
			TRACE("\tUsage: \\dump <SlaveID|Offset[11:8]>\n");
			TRACE("\tExample: \\dump A0\n\n");
			TRACE("\t=========>dump SlaveID=0xA0 Offset from 0x000 to 0x0FF\n\n");
			TRACE("\tExample: \\dump A2\n\n");			
			TRACE("\t=========>dump SlaveID=0xA0 Offset from 0x200 to 0x2FF\n\n");

			TRACE("\tSlaveID|Offset[11:8]: in HEX format, but without 0x prefix; 8-bit\n\n");
		}
		else if (strcmp((const char *)CommandName, "poweron") == 0)
		{
			TRACE("\tCommand: poweron\n");
			TRACE("\tFunction: force powering on Chicago\n");
			TRACE("\tUsage: \\poweron\n");
		}
		else if (strcmp((const char *)CommandName, "poweroff") == 0)
		{
			TRACE("\tCommand: poweroff\n");
			TRACE("\tFunction: force powering off Chicago\n");
			TRACE("\tUsage: \\poweroff\n");
		}
		else if (strcmp((const char *)CommandName, "debugon") == 0)
		{
			TRACE("\tCommand: debugon\n");
			TRACE("\tFunction: turn OFF Ext MCU state machine and stop OCM\n");
			TRACE("\tUsage: \\debugon\n");
		}
		else if (strcmp((const char *)CommandName, "debugoff") == 0)
		{
			TRACE("\tCommand: debugoff\n");
			TRACE("\tFunction: turn ON Ext MCU state machine and restart OCM\n");
			TRACE("\tUsage: \\debugoff\n");
		}
		else if (strcmp((const char *)CommandName, "stopocm") == 0)
		{
			TRACE("\tCommand: stopocm\n");
			TRACE("\tFunction: stop ocm\n");
			TRACE("\tUsage: \\stopocm\n");
		}
		else if (strcmp((const char *)CommandName, "startocm") == 0)
		{
			TRACE("\tCommand: startocm\n");
			TRACE("\tFunction: re-run ocm\n");
			TRACE("\tUsage: \\startocm\n");
		}
		else if (strcmp((const char *)CommandName, "chippowerup") == 0)
		{
			TRACE("\tCommand: chippowerup\n");
			TRACE("\tFunction: turn on CHIP_POWER_UP to high\n");
			TRACE("\tUsage: \\chippowerup\n");
		}
		else if (strcmp((const char *)CommandName, "chippowerdown") == 0)
		{
			TRACE("\tCommand: chippowerdown\n");
			TRACE("\tFunction: force CHIP_POWER_UP to low\n");
			TRACE("\tUsage: \\chippowerdown\n");
		}
		else if (strcmp((const char *)CommandName, "resetup") == 0)
		{
			TRACE("\tCommand: resetup\n");
			TRACE("\tFunction: turn on RESET to high\n");
			TRACE("\tUsage: \\resetup\n");
		}
		else if (strcmp((const char *)CommandName, "resetdown") == 0)
		{
			TRACE("\tCommand: resetdown\n");
			TRACE("\tFunction: froce RESET to low\n");
			TRACE("\tUsage: \\resetdown\n");
		}
		else if (strcmp((const char *)CommandName, "showmipi") == 0)
		{
			TRACE("\tCommand: showmipi\n");
			TRACE("\tFunction: dump MIPI TX related register\n");
			TRACE("\tUsage: \\showmipi\n");
		}
		else if (strcmp((const char *)CommandName, "showmipitx") == 0)
		{
			TRACE("\tCommand: showmipitx\n");
			TRACE("\tFunction: dump MIPI TX related register\n");
			TRACE("\tUsage: \\showmipitx <port number>\n");
			TRACE("\tExample: \\showmipitx 2\n\n");	
		}
		else if (strcmp((const char *)CommandName, "showdprx") == 0)
		{
			TRACE("\tCommand: showdprx\n");
			TRACE("\tFunction: dump DPRX related register\n");
			TRACE("\tUsage: \\showdprx\n");
		}
		else if (strcmp((const char *)CommandName, "panelon") == 0)
		{
			TRACE("\tCommand: panelon\n");
			TRACE("\tFunction: turn on panel power supply and RESET control (not include DCS)\n");
			TRACE("\tUsage: \\panelon\n");
		}
		else if (strcmp((const char *)CommandName, "paneloff") == 0)
		{
			TRACE("\tCommand: paneloff\n");
			TRACE("\tFunction: turn off panel power supply and RESET control to low\n");
			TRACE("\tUsage: \\paneloff\n");
		}
		else if (strcmp((const char *)CommandName, "readintr") == 0)
		{
			TRACE("\tCommand: readintr\n");
			TRACE("\tFunction: read interrupt status\n");
			TRACE("\tUsage: \\readintr\n");
		}
#if 0		
		else if (strcmp((const char *)CommandName, "delay_ms") == 0)
		{
			TRACE("\tCommand: delay_ms\n");
			TRACE("\tFunction: delay <time_in_ms> miliseconds\n");
			TRACE("\tUsage: \\delay_ms <time_in_ms>\n");
			TRACE("\tExample: \\delay_ms 10\n\n");
			TRACE("\ttime_in_ms: time in miliseconds, written in decimal format, integer in [1, 65535]\n");
		}
		else if (strcmp((const char *)CommandName, "delay_us") == 0)
		{
			TRACE("\tCommand: delay_us\n");
			TRACE("\tFunction: delay <time_in_us> microseconds\n");
			TRACE("\tUsage: \\delay_us <time_in_us>\n");
			TRACE("\tExample: \\delay_us 100\n\n");

			TRACE("\ttime_in_us: time in microseconds, written in decimal format, integer in [1, 65535]\n");
		}
#endif
		else
		{
			TRACE("\tNo such command or hasn't documented!\n");
			help();
		}
	}
	else
	{
		TRACE("\tUsage: \\man <command>\n");
	}
}


//-----------------------------------------------------------------------------
/// @copydoc rdint
static void rdint(void){
	chicago_read_intr_state();
}

//-----------------------------------------------------------------------------
/// @copydoc clrint
static void clrint(void){
	HDK_chicago_clear_intr_state(true);
}


//-----------------------------------------------------------------------------
/// @copydoc rd
static void rd(void){
	uint32_t SlaveID;
	uint32_t OffsetAddr;
	uint8_t RegData;

	int8_t RetVal;

	if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x %x", &SlaveID, &OffsetAddr) == 2){
		RetVal = i2c_read_byte(SlaveID, (uint16_t)OffsetAddr, &RegData);
		if (RetVal == 0)
		{
			TRACE3("\tSlaveID = 0x%02X, Offset = 0x%03X, Data = 0x%02X\n", SlaveID, OffsetAddr, RegData);
		}
		else
		{
			TRACE2("\t%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
	}
	else
	{
		TRACE("\tBad parameter! Usage:\n");
		TRACE("\t\\rd <Slave ID> <Offset>\n");
	}
}

//-----------------------------------------------------------------------------
/// @copydoc rd4
static void rd4(void){
	uint32_t SlaveID;
	uint32_t OffsetAddr;
	uint8_t buf[4];
	int8_t RetVal;

	if (sscanf((const char *)g_CmdLineBuf, "\t\\%*s %x %x", &SlaveID, &OffsetAddr) == 2)
	{
		TRACE2("\tSlaveID = 0x%02X, Offset = 0x%03X, Data = 0x", SlaveID, OffsetAddr);
		RetVal = i2c_read_block(SlaveID, (uint16_t)OffsetAddr, buf, 4);
		if (RetVal == 0)
		{
			TRACE_ARRAYI(buf, 4);
			TRACE("\n");
		}
	}
	else
	{
		TRACE("\tBad parameter! Usage:\n");
		TRACE("\t\\dump <SlaveID|Offset[11:8]>\n");
	}
}

//-----------------------------------------------------------------------------
/// @copydoc wr
static void wr(void){
	uint32_t SlaveID;
	uint32_t OffsetAddr;
	uint32_t RegData;
	uint8_t RetRegData;
	int8_t RetVal;

	if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x %x %x", &SlaveID, &OffsetAddr, &RegData) == 3)
	{
		RetVal = i2c_write_byte((uint8_t)SlaveID, (uint16_t)OffsetAddr, (uint8_t)RegData);
		
		TRACE3("\tWrote SlaveID = 0x%02X, Offset = 0x%03X, Data = 0x%02X\n", SlaveID, OffsetAddr, (uint8_t)RegData);
		
		if (RetVal == 0)
		{
			RetVal = i2c_read_byte((uint8_t)SlaveID, (uint16_t)OffsetAddr, &RetRegData);
			if (RetVal == 0)
			{
				TRACE3("\tRead back SlaveID = 0x%02X, Offset = 0x%03X, Data = 0x%02X\n", SlaveID, OffsetAddr, RetRegData);
			}
			else
			{
				TRACE2("\t%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
			}
		}
		else
		{
			TRACE2("\t%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
	}
	else
	{
		TRACE("\tBad parameter! Usage:\n");
		TRACE("\t\\wr <Slave ID> <Offset> <value_to_be_written>\n");
	}
}

//-----------------------------------------------------------------------------
/// @copydoc wr4
static void wr4(void){
	uint32_t SlaveID;
	uint32_t OffsetAddr;
	uint32_t RegData;
	int8_t RetVal;

	if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x %x %lx", &SlaveID, &OffsetAddr, &RegData) == 3)
	{
		TRACE3("\tWrote SlaveID = 0x%02X, Offset = 0x%03X, Data = 0x%08X\n", SlaveID, OffsetAddr, RegData);
		
		RetVal = i2c_write_byte4((uint8_t)SlaveID, (uint16_t)OffsetAddr, RegData);
		if (RetVal == 0)
		{
			rd4();
		}
		else
		{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
	}
	else
	{
		TRACE("\tBad parameter! Usage:\n");
		TRACE("\t\\wr4 <Slave ID> <Offset> <value_to_be_written>\n");
	}
}

//-----------------------------------------------------------------------------
/// @copydoc delay
static void delay(void){
	uint32_t delay_time;

	if (sscanf((const char *)g_CmdLineBuf, "\\%*s %d", &delay_time) == 1)
	{
		TRACE1("Delay = %d ms", delay_time);
		delay_ms(delay_time);
		TRACE("\n");
	}
	else
	{
		TRACE("\tBad parameter! Usage:\n");
		TRACE("\t\\delay <decimal ms>\n");
	}
}

//-----------------------------------------------------------------------------
/// @copydoc dump
static void dump(void){
	uint32_t SlaveID;
	uint32_t OffsetAddr;
	uint8_t i;
	uint8_t buf[16];
	int8_t RetVal;

	if (sscanf((const char *)g_CmdLineBuf, "\\%*s %x", &SlaveID) == 1)
	{
		if(0!=(SlaveID>>4)){
			OffsetAddr = (((uint32_t)SlaveID&0x000F)<<8);
			SlaveID = (SlaveID&0xF0);
			}else{
			OffsetAddr = 0;
		}
		TRACE2("\tSlaveID = 0x%02X, Offset from 0x%03X\n", SlaveID, OffsetAddr);
		TRACE("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
		i = 0;
		do
		{
			RetVal = i2c_read_block(SlaveID, (uint16_t)(OffsetAddr+i), buf, 16);
			if (RetVal == 0)
			{
				TRACE1("[%02X]:", i);
				TRACE_ARRAY(buf, 16);
				TRACE("\n");
				i += 16;
			}
			else
			{
				TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
				break;
			}
		}while(i != 0);
		TRACE("\n");
	}
	else
	{
		TRACE("\tBad parameter! Usage:\n");
		TRACE("\t\\dump <SlaveID|Offset[11:8]>\n");
	}
}

//-----------------------------------------------------------------------------
/// @copydoc power_restart
static void power_restart(void){
	TRACE("Chip power restart\n");
	
	//debug on
	g_bDebug=1;
	chicago_power_supply(0);
	chicago_power_onoff(0);
	delay_ms(50);
	chicago_power_supply(1);
	chicago_power_onoff(1);
	delay_ms(50);
}

//-----------------------------------------------------------------------------
/// @copydoc poweron
static void poweron(void){
	TRACE("Chip power supply ON\n");
	chicago_power_supply(1);
	chicago_state_change(STATE_NONE);
}

//-----------------------------------------------------------------------------
/// @copydoc poweroff
static void poweroff(void){
	TRACE("\tChip power supply OFF\n");
	chicago_power_supply(0);
	chicago_state_change(STATE_POWEROFF);
}

//-----------------------------------------------------------------------------
/// @copydoc chippowerup
static void chippowerup(void){
	TRACE("\tCHIP_POWER_UP to high\n");
	CHICAGO_CHIP_POWER_UP();

	chicago_last_state_change(STATE_WAITCABLE);
	chicago_state_change(STATE_WAITCABLE);
}

//-----------------------------------------------------------------------------
/// @copydoc chippowerdown
static void chippowerdown(void){
	TRACE("CHIP_POWER_UP to low\n");
	CHICAGO_CHIP_POWER_DOWN();

	chicago_last_state_change(STATE_WAITCABLE);
	chicago_state_change(STATE_WAITCABLE);	
}

//-----------------------------------------------------------------------------
/// @copydoc resetup
static void resetup(void){
	TRACE("RESET to high\n");
	CHICAGO_RESET_UP();

	chicago_last_state_change(STATE_WAITCABLE);
	chicago_state_change(STATE_WAITCABLE);
}

//-----------------------------------------------------------------------------
/// @copydoc resetdown
static void resetdown(void){
	TRACE("RESET to low\n");
	CHICAGO_RESET_DOWN();

	chicago_last_state_change(STATE_WAITCABLE);
	chicago_state_change(STATE_WAITCABLE);	
}

//-----------------------------------------------------------------------------
/// @copydoc show_mipi
static void show_mipi(void){
	uint8_t reg_temp, i;
	uint32_t  reg_int;
	int8_t RetVal;
	uint32_t reg_long;

	// show panel timing
	if(0==RetVal) {
		TRACE("\n=======================================\n");
		TRACE("H-act   HBP     H-sync  HFP     H-total\n");
		// H Active
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_ACTIVE_PIXEL_CFG_L, &reg_temp);
		reg_int = (uint32_t)reg_temp;
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_ACTIVE_PIXEL_CFG_H, &reg_temp);
		reg_int |= (0xFF00&(((uint32_t)reg_temp)<<8));
		TRACE1("%04d    ", reg_int);
		// HBP
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_H_B_PORCH_CFG_L, &reg_temp);
		reg_int = (uint32_t)reg_temp;
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_H_B_PORCH_CFG_H, &reg_temp);
		reg_int |= (0xFF00&(((uint32_t)reg_temp)<<8));
		TRACE1("%04d    ", reg_int);
		// HSYNC
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_H_SYNC_CFG_L, &reg_temp);
		reg_int = (uint32_t)reg_temp;
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_H_SYNC_CFG_H, &reg_temp);
		reg_int |= (0xFF00&(((uint32_t)reg_temp)<<8));
		TRACE1("%04d    ", reg_int);
		// HFP
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_H_F_PORCH_CFG_L, &reg_temp);
		reg_int = (uint32_t)reg_temp;
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_H_F_PORCH_CFG_H, &reg_temp);
		reg_int |= (0xFF00&(((uint32_t)reg_temp)<<8));
		TRACE1("%04d    ", reg_int);
		// Total pixel, H total
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_TOTAL_PIXEL_CFG_L, &reg_temp);
		reg_int = (uint32_t)reg_temp;
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_TOTAL_PIXEL_CFG_H, &reg_temp);
		reg_int |= (0xFF00&(((uint32_t)reg_temp)<<8));
		TRACE1("%04d\n\n", reg_int);

		TRACE("V-act   VBP     V-sync  VFP\n");
		// V Active
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_ACTIVE_LINE_CFG_L, &reg_temp);
		reg_int = (uint32_t)reg_temp;
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_ACTIVE_LINE_CFG_H, &reg_temp);
		reg_int |= (0x3F00&(((uint32_t)reg_temp)<<8));
		TRACE1("%04d    ", reg_int);
		
		// VBP
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_V_B_PORCH_CFG_L, &reg_temp);
		reg_int = (uint32_t)reg_temp;
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_V_B_PORCH_CFG_H, &reg_temp);
		reg_int |= (0x0F00&(((uint32_t)reg_temp)<<8));
		TRACE1("%04d    ", reg_int);
		
		// VSYNC
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_V_SYNC_CFG, &reg_temp);
		TRACE1("%02d      ", reg_temp);
		// VFP
		
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_V_F_PORCH_CFG_L, &reg_temp);
		reg_int = (uint32_t)reg_temp;
		i2c_read_byte(SLAVEID_PPS, REG_ADDR_V_F_PORCH_CFG_H, &reg_temp);
		reg_int |= (0x0F00&(((uint32_t)reg_temp)<<8));
		TRACE1("%04d\n", reg_int);
		TRACE("=======================================\n");
	}else{
		TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
	}
	
	// show other data
	RetVal = i2c_read_byte(SLAVEID_SPI, GPIO_CTRL_2, &reg_temp);
	if(0==RetVal) {
		TRACE("port    scan mode    video mode      DSC\n");
		if(0==(reg_temp&0x03)){
			i = 1;
		}else{
			i = (reg_temp&0x03)*2;
		}
		TRACE1("%02d      ", i);
		
		if(0==(reg_temp&0x10)){
			TRACE1("%s", "left first   ");
		}else{
			TRACE1("%s", "right first  ");
		}
		
		switch((reg_temp>>2)&0x03){
			case 0:
				TRACE1("%s", "one in one out  ");
			break;
			case 1:
				TRACE1("%s", "side by side    ");
			break;
			case 2:
				TRACE1("%s", "stacked frame   ");
			break;
			default:
				TRACE("ERROR!!         ");
		}
		
		i2c_read_byte(SLAVEID_SPI, R_DSC_CTRL_0, &reg_temp);
		if(0x01==(0x01&reg_temp)){
			// DSC enable
			TRACE1("%s\n", "YES");
		}else{
			TRACE1("%s\n", "NO");
		}
		TRACE("=======================================\n");		
		// show value
		TRACE("     M-vid-force          N-vid-force\n");
		RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_M_FORCE_VALUE_3, &reg_temp);
		if(0==RetVal) {
			reg_long = ((((uint32_t)reg_temp)<<16)&0x00FF0000);
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_M_FORCE_VALUE_2, &reg_temp);
		if(0==RetVal) {
			reg_long = (reg_long|((((uint32_t)reg_temp)<<8)&0x0000FF00));
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_M_FORCE_VALUE_1, &reg_temp);
		if(0==RetVal) {
			reg_long = (reg_long|(((uint32_t)reg_temp)&0x000000FF));
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		TRACE1("%16ld     ", reg_long);
		
		RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_N_FORCE_VALUE_3, &reg_temp);
		if(0==RetVal) {
			reg_long = ((((uint32_t)reg_temp)<<16)&0x00FF0000);
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_N_FORCE_VALUE_2, &reg_temp);
		if(0==RetVal) {
			reg_long = (reg_long|((((uint32_t)reg_temp)<<8)&0x0000FF00));
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_N_FORCE_VALUE_1, &reg_temp);
		if(0==RetVal) {
			reg_long = (reg_long|(((uint32_t)reg_temp)&0x000000FF));
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		TRACE1("%16ld\n", reg_long);
		TRACE("=======================================\n\n");		
				
	}else{
		TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
	}
}

//-----------------------------------------------------------------------------
/// @copydoc show_mipi_tx
static void show_mipi_tx(void){
	uint32_t PortID;
	uint8_t buf[4];
	uint8_t i;
	int8_t RetVal;
	uint32_t reg_long;
	uint32_t reg_int;

	//mipi burst mode/non-burst, MIPI tx data rate
	if (sscanf((const char *)g_CmdLineBuf, "\\%*s %d", &PortID) == 1){
		if(PortID>3){
			// Error Port ID
			TRACE("\tERROR PortID, PortID should be less 4\n");
			return;
		}
	
		TRACE1("\nShow MIPI TX port = %d\n", PortID);
		TRACE("=========================================\n");
		// show VID mode
		TRACE("VID Mode\n0x");
		RetVal = i2c_read_block(SLAVEID_MIPI_PORT0+PortID, VID_MODE_CFG, buf, 4);
		if (RetVal == 0)
		{
			TRACE_ARRAYI(buf, 4);
			TRACE("\n");
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		TRACE("=========================================\n");
		// Show V value
		TRACE("VSA     VBP     VFP     VACTIVE\n");
		for(i=0;i<16;(i=i+4)){
			// VSA // VBP //VFP //Vactive
			RetVal = i2c_read_block(SLAVEID_MIPI_PORT0+PortID, VID_VSA_LINES+i, buf, 4);
			if(0==RetVal){
				reg_long = (((uint32_t)buf[0])&0x000000FF)+((((uint32_t)buf[1])<<8)&0x0000FF00)+\
					((((uint32_t)buf[2])<<16)&0x00FF0000)+((((uint32_t)buf[3])<<24)&0xFF000000);
				TRACE1("%04d    ", (uint32_t)reg_long);
			}else{
				TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
			}
		}
		TRACE("\n\n");

		// Show H value
		TRACE("HSA     HBP     HLINE   PKT\n");
		for(i=0;i<12;(i=i+4)){
			// HSA // HBP //H-total
			RetVal = i2c_read_block(SLAVEID_MIPI_PORT0+PortID, VID_HSA_TIME+i, buf, 4);
			if(0==RetVal){
				reg_long = (((uint32_t)buf[0])&0x000000FF)+((((uint32_t)buf[1])<<8)&0x0000FF00)+\
					((((uint32_t)buf[2])<<16)&0x00FF0000)+((((uint32_t)buf[3])<<24)&0xFF000000);
				TRACE1("%04d    ", (uint32_t)reg_long);
			}else{
				TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
			}
		}

		//H-active
		RetVal = i2c_read_block(SLAVEID_MIPI_PORT0+PortID, VID_PKT_SIZE, buf, 4);
		if(0==RetVal){
			reg_long = (((uint32_t)buf[0])&0x000000FF)+((((uint32_t)buf[1])<<8)&0x0000FF00)+\
				((((uint32_t)buf[2])<<16)&0x00FF0000)+((((uint32_t)buf[3])<<24)&0xFF000000);
			TRACE1("%04d\n", (uint32_t)reg_long);
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		TRACE("=========================================\n");

		// Other Data
		TRACE("CLK-LP2HS  CLK-HS2LP  HS-LP2HS   HS-HS2LP\n");
		for(i=0;i<8;(i=i+4)){
			RetVal = i2c_read_block(SLAVEID_MIPI_PORT0+PortID, PHY_TMR_LPCLK_CFG+i, buf, 4);
			if(0==RetVal) {
				reg_int = (((uint32_t)buf[0])&0x00FF)+((((uint32_t)buf[1])<<8)&0xFF00);
				TRACE1("0x%04X     ", reg_int);
				reg_int = (((uint32_t)buf[2])&0x00FF)+((((uint32_t)buf[3])<<8)&0xFF00);
				TRACE1("0x%04X     ", reg_int);
			}else{
				TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
			}
		}
	
		TRACE("\n\nLane-Total\n");
		RetVal = i2c_read_block(SLAVEID_MIPI_PORT0+PortID, PHY_TMR_LPCLK_CFG+i, buf, 4);
		if(0==RetVal) {
			TRACE1("%02d\n", ((buf[0]&0x03)+1));
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		TRACE("=========================================\n\n");
	}
	else
	{
		TRACE("\tBad parameter! Usage:\n");
		TRACE("\t\\showmipitx <port number>\n");
	}
}

//-----------------------------------------------------------------------------
/// @copydoc show_dprx
static void show_dprx(void){
	uint8_t reg_temp;
	uint8_t i;
	char RetVal;
	uint32_t reg_int;

	// show DP H and V DBG data
	TRACE("\nHSTART  HSW     HTOTAL  HWIDTH  VHEIGHT VSTART  VSW     VTOTAL  MISC\n");
	TRACE("--------------------------------------------------------------------\n");
	// HSTART
	RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_HSTART_DBG, &reg_temp);
	if(0==RetVal){
		reg_int = ((((uint32_t)reg_temp)<<8)&0xFF00);
	}else{
		TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
	}
	RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_HSTART_DBG+4, &reg_temp);
	if(0==RetVal){
		reg_int = (reg_int|(((uint32_t)reg_temp)&0x00FF));
		TRACE1("%04d    ", reg_int);
	}else{
		TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
	}
	
	// HSW
	RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_HSW_DBG+4, &reg_temp);
	if(0==RetVal){
		TRACE1("%02d      ", reg_temp);
	}else{
		TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
	}
	
	// HTOTAL  HWIDTH  VHEIGHT VSTART
	for(i=0;i<32;(i=i+8)){
		RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_HTOTAL_DBG+i, &reg_temp);
		if(0==RetVal){
			reg_int = ((((uint32_t)reg_temp)<<8)&0xFF00);
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_HTOTAL_DBG+i+4, &reg_temp);
		if(0==RetVal){
			reg_int = (reg_int|(((uint32_t)reg_temp)&0x00FF));
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		TRACE1("%04d    ", reg_int);
	}
	
	// VSW
	RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_VSW_DBG+4, &reg_temp);
	if(0==RetVal){
		TRACE1("%02d      ", reg_temp);
	}else{
		TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
	}

	// VTOTAL  MISC
	for(i=0;i<16;(i=i+8)){
		RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_VTOTAL_DBG+i, &reg_temp);
		if(0==RetVal){
			reg_int = ((((uint32_t)reg_temp)<<8)&0xFF00);
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_VTOTAL_DBG+i+4, &reg_temp);
		if(0==RetVal){
			reg_int = (reg_int|(((uint32_t)reg_temp)&0x00FF));
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
		TRACE1("%04d    ", reg_int);
	}
	TRACE("\n\n");
	
	// show AVI InfoFrame 
	TRACE("AVI InfoFrame \n");
	TRACE("000 008 016 024 032 040 048 056 064 072 080 088 096 104 112 120\n");
	TRACE("---------------------------------------------------------------\n");
	for(i=0;i<64;(i=i+4)){
		RetVal = i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_AVI_INFOR+i, &reg_temp);
		if(0==RetVal){
			TRACE1("%02X  ", reg_temp);
		}else{
			TRACE2("%s(%u): NAK!\n", __FILE__, (uint32_t)__LINE__);
		}
	}
	TRACE("\n\n");
}

//-----------------------------------------------------------------------------
/// @copydoc panelon
static void panelon(void){
	TRACE("\tPanel power supply ON\n");
	mipi_panel_power_supply(1);
}

//-----------------------------------------------------------------------------
/// @copydoc paneloff
static void paneloff(void){
	TRACE("\tPanel power supply OFF\n");
	mipi_panel_power_supply(0);
}

//-----------------------------------------------------------------------------
/// @copydoc start_ocm
static void start_ocm(void){
	uint8_t reg_temp;

	TRACE("\tre-run Main ocm\n");
	// restart main OCM
	i2c_read_byte(SLAVEID_SPI, OCM_DEBUG_CTRL, &reg_temp);
	reg_temp &= ~OCM_RESET;
	i2c_write_byte(SLAVEID_SPI, OCM_DEBUG_CTRL, reg_temp);
}

//-----------------------------------------------------------------------------
/// @copydoc MakeLower
static void MakeLower(uint8_t *p){
	for(; *p; p++)
	{
		if (*p >= 'A' && *p <= 'Z')
		{
			*p += 0x20;
		}
	}
}

//-----------------------------------------------------------------------------
/// @copydoc Chicago_info
static void Chicago_info(void){
	TRACE("\tChicago has more than 256 registers.\n");
	TRACE("\tThese registers are combined by Slave ID and Offset.\n");
}


// #if 0
//
// uint8_t sp_tx_aux_dpcdread_bytes(uint8_t addrh, uint8_t addrm,
// 	uint8_t addrl, uint8_t cCount, uint8_t *pBuf)
// {
// 	uint8_t c, i;
// 	uint8_t bOK;
//
// 	//command and length
// 	c = ((cCount - 1) << 4) | 0x09;
// 	//WriteReg(RX_P0, AP_AUX_COMMAND, c);
//       //address
// 	write_dpcd_addr(addrh, addrm, addrl);
// 	//aux en
// 	//sp_write_reg_or(RX_P0, AP_AUX_CTRL_STATUS, AP_AUX_CTRL_OP_EN);
// 	mdelay(2);
//        TRACE3("auxch addr = 0x%.2bx%.2bx%.2bx\n", addrh, addrm, addrl);
// 	wait_aux_op_finish(&bOK);
// 	if (bOK == AUX_ERR) {
// 		TRACE("aux read failed\n");
// 		return AUX_ERR;
// 	}
//
// 	for (i = 0; i < cCount; i++) {
// 		//ReadReg(RX_P0, AP_AUX_BUFF_START + i, &c);
// 		*(pBuf + i) = c;
// 		TRACE2("Buf[%d] = 0x%.2bx\n", (uint)i, *(pBuf + i));
// 		if (i >= MAX_BUF_CNT)
// 			break;
// 	}
//
// 	return AUX_OK;
// }
//
// static void wait_aux_op_finish(uint8_t * err_flag)
// {
// 	uint8_t cnt;
// 	uint8_t c;
//
// 	*err_flag = 0;
// 	cnt = 150;
// 	//while (__i2c_read_byte(RX_P0, AP_AUX_CTRL_STATUS) & AP_AUX_CTRL_OP_EN) {
// 	//			mdelay(2);
// 	//			if ((cnt--) == 0) {
// 	//				TRACE("aux operate failed!\n");
// 	//				*err_flag = 1;
// 	//				break;
// 	//			}
// 	//		}
//
// 	//ReadReg(RX_P0, AP_AUX_CTRL_STATUS, &c);
// 	if (c & 0x0F) {
// 			TRACE1("wait aux operation status %.2x\n", (uint)c);
// 			*err_flag = 1;
// 	}
// }
//
//
// static void sp_tx_rst_aux(void)
// {
// 	//sp_write_reg_or(TX_P2, RST_CTRL2, AUX_RST);
// 	//sp_write_reg_and(TX_P2, RST_CTRL2, ~AUX_RST);
// }
//
// uint8_t  sp_tx_aux_wr(uint8_t offset)
// {
// 	uint8_t c;
//
// 	//WriteReg(RX_P0, AP_AUX_BUFF_START, offset);
// 	//WriteReg(RX_P0, AP_AUX_COMMAND, 0x04);
// 	//WriteReg(RX_P0, AP_AUX_CTRL_STATUS, AP_AUX_CTRL_OP_EN);
// 	wait_aux_op_finish(&c);
//
// 	return c;
// }
//
// uint8_t  sp_tx_aux_rd(uint8_t len_cmd)
// {
// 	uint8_t c;
//
// 	//WriteReg(RX_P0, AP_AUX_COMMAND, len_cmd);
// 	//WriteReg(RX_P0, AP_AUX_CTRL_STATUS, AP_AUX_CTRL_OP_EN);
// 	wait_aux_op_finish(&c);
//
// 	return c;
// }
//
// uint8_t sp_tx_get_edid_block(void)
// {
// 	uint8_t c;
//
// 	//sp_tx_aux_wr(0x7e);
// 	//sp_tx_aux_rd(0x01);
// 	//ReadReg(RX_P0, AP_AUX_BUFF_START, &c);
// 	TRACE1(" EDID Block = %d\n", (int)(c + 1));
//
// 	if (c > 3)
// 		c = 1;
//
// 	return c;
// }
//
// static void errchk(void)
// {
// 	uint errl, errh;
// 	sp_tx_aux_dpcdread_bytes(0x00, 0x02, 0x10, 2, bytebuf);//LANE_ALIGN__STATUS_UPDATED
// 	sp_tx_aux_dpcdread_bytes(0x00, 0x02, 0x10, 2, bytebuf);//LANE0_1_STATUS : Lane0 and Lane1 Status
//
// 	errh = bytebuf[1];
// 	if (errh & 0x80)
// 	{
// 		errl = bytebuf[0];
// 		errh = (errh &0x7f) << 8;
// 		errl = errh + errl;
// 		TRACE1(" Err of Lane[0] = %d\n", (uint)errl);
// 	}
// }
//
// #endif