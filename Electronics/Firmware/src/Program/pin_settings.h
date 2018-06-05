/**
* @file pin_settings.h
*
* @brief Pin definitions and helper functions _H
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
* @author Adam Munich
*/


/**
* @defgroup Config_pins [Config] Pin settings
* @details
*	These macros define the pin names used throughout the software, and
*	take the form PIN_(pinname). Refer to the arduino documentation for the
*	particular IC this software is flashed to
*/



#ifndef __PIN_SETTINGS_H__
	#define __PIN_SETTINGS_H__

	#include <Arduino.h>
	#include <Wire.h>
	
	//#############################################################################
	// Pre-compiler Definitions
	//-----------------------------------------------------------------------------
	// Chicago 
	
	/// @brief [DIN] Pin connected (via 1.8V level shifter) to INTP on Chicago
	/// @note On ATMEGA328, must be pin 3. No other pins support interrupts.
	/// @ingroup Config_pins
	#define PIN_chicago_intp					13
	
	/// @brief [DOUT] Pin connected (via 1.8V level shifter) to RESET on Chicago
	/// @ingroup Config_pins
	#define PIN_chicago_rst						10	
	#define PIN_chicago_rst_default_state		LOW
	
	/// @brief [DOUT] Pin connected (via 1.8V level shifter) to PWEN on Chicago
	/// @ingroup Config_pins	
	#define PIN_chicago_pwen					11
	#define PIN_chicago_pwen_default_state		LOW

	//-----------------------------------------------------------------------------
	// BNO080	

	/// @brief [DIN] Pin connected to IMU interrupt
	/// @ingroup Config_pins
	#define PIN_osvr_tracker_intn				A2

	/// @brief [DOUT] Pin connected to IMU reset
	/// @ingroup Config_pins
	#define PIN_osvr_tracker_rstn				A1

	/// @brief [DOUT] Pin connected to IMU DFU pin
	/// @ingroup Config_pins	
	#define PIN_osvr_tracker_bootn				6				

	// [DEAD] @brief [DIN] Pin connected to power supply fault detector
	// [DEAD] @ingroup Config_pins
	// [DEAD] #define PIN_lcd_bl_fault					7
	
	/// @brief [DIN] Pin connected to audio jack sense switch
	/// @ingroup Config_pins
	#define PIN_aud_sw							2

	/// @brief [AIN] Pin connected to (analog) face-side light sensor
	/// @ingroup Config_pins
	#define PIN_light_sense						A0
	
	/// @brief [DIN] Pin connected (via 1.8V level shifter) to VSYNC in signal
	/// @ingroup Config_pins
	#define PIN_backlight_vsync_in				12 //12

	/// @brief [DOUT] Pin connected to OLED SLEP2 voltage select
	/// @ingroup Config_pins
	#define PIN_oled_slep2						4
	#define PIN_oled_slep2_default_state		LOW

	/// @brief [DOUT] Pin connected (via 1.8V level shifter) to panel reset lines
	/// @ingroup Config_pins	
	#define PIN_panel_reset						3
	#define PIN_panel_reset_default_state		HIGH

	/// @brief [AOUT] Pin connected to LCD backlight PWM input
	/// @ingroup Config_pins
	#define PIN_backlight_pwm					8
	#define PIN_backlight_pwm_default_state		LOW

	/// @brief [DOUT] Pin connected to LCD backlight enable line
	/// @ingroup Config_pins
	//#define PIN_lcd_bl_en						A2
	//#define PIN_lcd_bl_en_default_state		LOW

	/// @brief [DOUT] Pin connected to panel bias enable line
	/// @ingroup Config_pins
	#define PIN_lcd_bias_en						7
	#define PIN_lcd_bias_en_default_state		LOW

	/// @brief [DOUT] Pin connected to 1.0V enable line
	/// @ingroup Config_pins	
	//#define PIN_1V0_en							2
	//#define PIN_1V0_en_default_state			HIGH
	
	//#############################################################################
	// Type Definitions
	//-----------------------------------------------------------------------------

	//#############################################################################
	// Function Prototypes
	//-----------------------------------------------------------------------------	
	#define HDK_LCD_BL_ON()						delay(0); //digitalWrite(PIN_lcd_bl_en, HIGH);
	#define HDK_LCD_BL_OFF()					delay(0); //digitalWrite(PIN_lcd_bl_en, LOW);
	
	#define HDK_LCD_BL_PWM(arg1)				analogWrite(PIN_backlight_pwm, arg1);
	#define HDK_LCD_BL_PWM_ON()					digitalWrite(PIN_backlight_pwm, HIGH);
	
	
	/**
	 * @brief 
	 *		Call this to initialize the pins and set their default state
	 * @ingroup Program
	 * @return void
	 */	
	void static setup_GPIO(void){
		pinMode(PIN_chicago_intp,	INPUT);
	
		pinMode(PIN_chicago_rst,	OUTPUT);
		digitalWrite(PIN_chicago_rst, PIN_chicago_rst_default_state);
		
		pinMode(PIN_chicago_pwen,   OUTPUT);
		digitalWrite(PIN_chicago_pwen, PIN_chicago_pwen_default_state);
			
		//pinMode(PIN_1V0_en,			OUTPUT);
		//digitalWrite(PIN_1V0_en,	PIN_1V0_en_default_state);
	
		pinMode(PIN_backlight_pwm,		OUTPUT);
		digitalWrite(PIN_backlight_pwm, PIN_backlight_pwm_default_state);
	
		pinMode(PIN_lcd_bias_en,    OUTPUT);
		digitalWrite(PIN_lcd_bias_en, PIN_lcd_bias_en_default_state);
		
		//pinMode(PIN_lcd_bl_en,		OUTPUT);
		//digitalWrite(PIN_lcd_bl_en, PIN_lcd_bl_en_default_state);
	}
	
	
	/**
	 * @brief 
	 *		Call this to initialize the serial debug bus
	 * @ingroup Program
	 * @return void
	 */		
	void static setup_USART(uint8_t clear_screen, uint32_t baudRate){
		Serial.begin(baudRate, SERIAL_8N1);
		
		if(clear_screen){
			for(uint8_t i=0; i<100; i++){
				Serial.print("\n");
			}
		}
		
		Serial.print("USART initialized!\n\n");
	}
	
	/**
	* @brief 
	*		Initialize I2C bus
	* @ingroup Program
	* @return void
	*/		
	void static setup_I2C(void){
		Wire.begin();
	}

#endif