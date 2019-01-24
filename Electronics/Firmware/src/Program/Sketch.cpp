/**
* @file start.cpp
*
* @brief Main program entry point
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

//#############################################################################
// Includes
//-----------------------------------------------------------------------------
#include <Arduino.h>
#include <Wire.h>
#include <variant.h>

#include "./Sketch.h"
#include "./pin_settings.h"
#include "./display_settings.h"

#include "./Lib_Chicago/Chicago/chicago.h"
#include "./Lib_Chicago/Chicago/chicago_config.h"
#include "./Lib_Chicago/I2C/i2c.h"
#include "./Lib_Chicago/Debug/cmd.h"

#include "./Lib_TPS61194/tps61194.h"


//#############################################################################
// Pre-compiler Definitions
//-----------------------------------------------------------------------------


//#############################################################################
// Variable Declarations
//-----------------------------------------------------------------------------
bool g_bDebug = 0;
TPS61194_ TPS61194(PIN_backlight_pwm, PIN_backlight_vsync_in, PIN_lcd_bl_en);


//#############################################################################
// Function Definitions
//-----------------------------------------------------------------------------
void setup(){
	
	// Pins
	setup_GPIO();
	setup_USART(true, 921600);
	setup_I2C();
	
	// VRPN serial debug
	SerialUSB.begin(921600);
	
	Serial.println("");
	Serial.println("************        Leap Motion        ***********");
	Serial.println("*                Project North Star              *");
	Serial.println("**************************************************");
	Serial.printf("Firmware release: %x.%x.%x\n", ((LEAP_DISPLAY_BOARD_REV & 0xff00) >> 8),
												  ((LEAP_DISPLAY_BOARD_REV & 0x00f0) >> 4),
												   (LEAP_DISPLAY_BOARD_REV & 0x000f));
	Serial.println(buildDate);
	Serial.println(buildTime);
	Serial.printf("Resolution: %dx%d@%dfps\n", PANEL_H_ACTIVE, PANEL_V_ACTIVE, PANEL_FRAME_RATE);
	Serial.println("");
	
	// Backlight driver
	TPS61194.setup(
		TPS61194.vsyncModeSetup::vsyncModeOneshot,	// vsyncMode
		PANEL_FRAME_RATE,							// frameRate
		3333,										// oneshot delay (us)
		1667										// oneshot pulse width (us)
	);
	
	TPS61194.brightness(512);
	TPS61194.powerOn();
	
	// Chicago bridge
	EXT_INTR_ENABLE();
	chicago_state_change(STATE_NONE);
	chicago_last_state_change(STATE_NONE);
	
	// indicate that the device is initialized
	LED_YELLOW_ON();
}

void loop(){		
	cmd();

	if(!g_bDebug)
	{
		chicago_main();
		if(IS_INTR_EVENT())
		{
			chicago_interrupt_handle();
		}
	}
}