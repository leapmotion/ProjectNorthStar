/**
* @file chicago_config.h
*
* @brief Chicago config _H
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

#ifndef __CONFIG_H__
	#define __CONFIG_H__
	
	#include "../../pin_settings.h"
	
	#define _BIT0								0x01
	#define _BIT1								0x02
	#define _BIT2								0x04
	#define _BIT3								0x08
	#define _BIT4								0x10
	#define _BIT5								0x20
	#define _BIT6								0x40
	#define _BIT7								0x80

	#define MAX(x, y)							((x) > (y) ? (x) : (y))
	#define MIN(x, y)							((x) < (y) ? (x) : (y))
	#define ABS(x, y)							((x) > (y) ? ((x) - (y)) : ((y) - (x)))

	////////////////////////////////////////////////////////////////////////////////
	// byte extraction macros
	#define LOBYTE(x)							((uint8_t)(x))
	#define HIBYTE(x)							((uint8_t)((x) >> 8))

	// byte access macros
	// generic pointers
	#define BYTE0(x)							((uint8_t *) & x)[0]
	#define BYTE1(x)							((uint8_t *) & x)[1]
	#define BYTE2(x)							((uint8_t *) & x)[2]
	#define BYTE3(x)							((uint8_t *) & x)[3]

	////////////////////////////////////////////////////////////////////////////////
	// bitwise operation macros
	#define BIT_IS_0(x, bitindex)				(((x) & 1 << bitindex) == 0)
	#define BIT_IS_1(x, bitindex)				(((x) & 1 << bitindex) != 0)
	#define BIT(x, bitindex)					((bit)((x) & 1<<bitindex))
	#define BITSET(x, bitindex)					((x) |= 1<<bitindex)
	#define BITCLEAR(x, bitindex)				((x) &= ~(1<<bitindex))

	// Extracts bits: from x, starting at bit bitindex, extracts bitlen bits; bit bitindex is the extracted LSB.
	#define BITS_EXTRACT(x, bitindex, bitlen)	((x) >> (bitindex) & ((1 << bitlen) - 1))
	#define BITMASK(bitindex)					(1 << bitindex)
	#define BITSMASK(bitindex, bitlen)			(((1 << bitlen) - 1) << bitindex)

	// in x, set bits [bitindex+bitlen-1 : bitindex] to value y; y <= ((1<<bitlen) - 1), otherwise data corruption!!
	#define BITS_SET(x, bitindex, bitlen, y)	((x) = (x) & ~BITSMASK(bitindex, bitlen) | (y) << bitindex)

	#define UNUSED_VAR(x)						((x) = (x))

	/* ================= board dependent macros ============================== */
	// 1. General control logic
	#define IS_CHICAGO_INT()					digitalRead(PIN_chicago_intp)
	
	//    b) serial communication buffer
	#define SERIAL_RECV_BUF_SIZE				64
	#define SERIAL_SEND_BUF_SIZE				64
	#define CMD_LINE_SIZE						128
	#define CMD_NAME_SIZE						16
	
	
	#define POWERCYCLE_DELAY					250  // in miliseconds
	#define RESET_RELEASE_DELAY					50  // in miliseconds

	/* note: On POWER_EN and RESET_N pins */
	#define CHICAGO_CHIP_POWER_UP()				digitalWrite(PIN_chicago_pwen, HIGH);
	#define CHICAGO_CHIP_POWER_DOWN()			digitalWrite(PIN_chicago_pwen, LOW);
	#define CHICAGO_RESET_DOWN()				digitalWrite(PIN_chicago_rst, LOW);
	#define CHICAGO_RESET_UP()					digitalWrite(PIN_chicago_rst, HIGH);

	//#define CS4350_RESET()					CS4350_RESET_N = 1
	//#define CS4350_RESET_RELEASE()			CS4350_RESET_N = 0

	/* note: On AVDD and DVDD pins */
	#define CHICAGO_V10D_ON()					{}//digitalWrite(PIN_1V0_en, HIGH);
	#define CHICAGO_V10D_DOWN()					{}//digitalWrite(PIN_1V0_en, LOW);
	#define CHICAGO_V10A_ON()					{}//digitalWrite(PIN_1V0_en, HIGH);
	#define CHICAGO_V10A_DOWN()					{}//digitalWrite(PIN_1V0_en, LOW);
	#define CHICAGO_V18D_ON()					{}
	#define CHICAGO_V18D_DOWN()					{}
	#define CHICAGO_V18A_ON()					{}
	#define CHICAGO_V18A_DOWN()					{}

	/* note: Panel power pins */

	#define CHICAGO_PANEL_V18_ON()				; //digitalWrite(PIN_lcd_bias_en, HIGH);
	#define CHICAGO_PANEL_V18_DOWN()			; //digitalWrite(PIN_lcd_bias_en, LOW);

	#define CHICAGO_PANEL_V33_ON()				; //digitalWrite(PIN_lcd_bias_en, HIGH);
	#define CHICAGO_PANEL_V33_DOWN()			; //digitalWrite(PIN_lcd_bias_en, LOW);

	#define CHICAGO_PANEL_V55_ON()				digitalWrite(PIN_lcd_bias_en, HIGH);
	#define CHICAGO_PANEL_V55_DOWN()			digitalWrite(PIN_lcd_bias_en, LOW);

	#define CHICAGO_PANEL_V55N_ON()				; //digitalWrite(PIN_lcd_bias_en, HIGH);
	#define CHICAGO_PANEL_V55N_DOWN()			; //digitalWrite(PIN_lcd_bias_en, LOW);

	/* note: Panel RESET pins */
	#define CHICAGO_PANEL0_RESET_UP()			digitalWrite(PIN_panel_reset, HIGH);
	#define CHICAGO_PANEL0_RESET_DOWN()			digitalWrite(PIN_panel_reset, LOW);	
	#define CHICAGO_PANEL1_RESET_UP()			digitalWrite(PIN_panel_reset, HIGH);
	#define CHICAGO_PANEL1_RESET_DOWN()			digitalWrite(PIN_panel_reset, LOW);

	/* note: Panel ENPORT pins */
	#define CHICAGO_PANEL0_ENPORT_ON()			{} //digitalWrite(PIN_panel_enport, HIGH);
	#define CHICAGO_PANEL0_ENPORT_OFF()			{} //digitalWrite(PIN_panel_enport, LOW);
	#define CHICAGO_PANEL1_ENPORT_ON()			{} //digitalWrite(PIN_panel_enport, HIGH);
	#define CHICAGO_PANEL1_ENPORT_OFF()			{} //digitalWrite(PIN_panel_enport, LOW);

	// misc macros which are bringup program dependent 
	#define  MAX_BYTE_COUNT_PER_RECORD_EEPROM   16
	#define  MAX_BYTE_COUNT_PER_RECORD_FLASH    16

	// For Debug Switch
	#define	IS_DIP1_1							((P1 & _BIT3) >> 3)
	#define	IS_DIP1_2							((P1 & _BIT2) >> 2)
	#define	IS_DIP1_3							((P1 & _BIT1) >> 1)
	#define	IS_DIP1_4							((P1 & _BIT0))
	#define	IS_DIP2_1							((P4 & _BIT0))
	#define	IS_DIP2_2							((P4 & _BIT1) >> 1)
	#define	IS_DIP2_3							((P4 & _BIT2) >> 2)
	#define	IS_DIP2_4							((P4 & _BIT3) >> 3)

	// for Debug Switch in EVB only
	#define DS_BIST								_BIT0
	#define DS_SCRITP_ONLY						_BIT1
	#define DS_PORT_SELECT						_BIT2
	#define DS_EDID_SELECT						_BIT3

	#define DELAY_US(t)							delayMicroseconds(t);
	#define delay_ms(t)							delay(t);

	#define OFFSET(s, m)						(uint8_t) & (((s *) 0 ) -> m)

	////////////////////////////////////////////////////////////////////////////////
	extern uint8_t								intr_flag;

	#define EXT_INTR_SET						1
	#define EXT_INTR_CLEAR						0

	#define ALL_INTR_ENABLE()					attachInterrupt(PIN_chicago_intp, external_int_isr, FALLING);
	#define EXT_INTR_ENABLE()					attachInterrupt(PIN_chicago_intp, external_int_isr, FALLING);
	#define EXT_INTR_DISABLE()					detachInterrupt(PIN_chicago_intp);

	#define EXT_INTR_EVENT_SET()				(intr_flag = EXT_INTR_SET)
	#define EXT_INTR_EVENT_CLEAR()				(intr_flag = EXT_INTR_CLEAR)
	#define IS_INTR_EVENT()						(intr_flag)


	////////////////////////////////////////////////////////////////////////////////
	#define FLAG_VALUE_ON						1
	#define FLAG_VALUE_OFF						0

	#define RETURN_NORMAL_VALUE					0
	#define RETURN_FAILURE_VALUE				-1
	#define RETURN_FAILURE_VALUE2				-2
	#define RETURN_FAILURE_VALUE3				-3
	#define RETURN_FAILURE_VALUE4				-4
	#define RETURN_FAILURE_VALUE5				-5

	#define DIP_SWITCH_ON						0

#endif  /* __CONFIG_H__ */

