/**
* @file display_settings.h
*
* @brief Chicago display settings _H
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

#ifndef __DISPLAY_SETTINGS_H__
	#define __DISPLAY_SETTINGS_H__

	/** ###########################################################################
	* @defgroup Config_paneldebug [Config] Panel debug functions
	* @details
	*	These macros allow for debugging of the MIPI bus and other things. Edit 
	*	them inside display_settings.h
	*/
	
	// Todo, actually write this documentation block 
		
	//#define DEBUG_PWM_TEST
	#define DEBUG_REMOVE_CABLE_DET
	//#define DEBUG_CHECK_DP_RESOLUTION
	//#define DEBUG_POWER_OFF_CHICAGO_WHEN_CABLE_OUT
	#define DEBUG_ENABLE_COLOR_BARS
	
    // Panel type ......
   #define BOE_VS035ZSM
	
	/** ###########################################################################
	* @defgroup Config_panelsettings [Config] Panel settings
	* @details
	*	These macros allow Chicago to properly set up the panel timing. Edit them
	*	inside display_settings.h
	*/
		
	/** 
	*	@brief	Horizontal active pixels (per panel)
	*	@details
	*		Number of horizontal active pixels per MIPI panel. An active pixel 
	*		is a physical pixel capable of displaying light, not a logical 
	*		pixel in software.
	*	@ingroup Config_panelsettings
	*/
    #define PANEL_H_ACTIVE						1440

	/** 
	*	@brief	Vertical active pixels (per panel)
	*	@details
	*		Number of vertical active pixels per MIPI panel. An active pixel 
	*		is a physical pixel capable of displaying light, not a logical 
	*		pixel in software.
	*	@ingroup Config_panelsettings
	*/	
    #define	PANEL_V_ACTIVE						1600

    
	/** 
	*	@brief	Frame rate (whole display)
	*	@details
	*		Number of full color frames per second across all MIPI ports. If set 
	*		too low, and the pixel porches are also too low (total pixel clock 
	*		too low), then the Chicago PLL will lock up. It will also lock up if 
	*		the clock is too high. Target 350MHz.
	*	@ingroup Config_panelsettings
	*/
    #define	PANEL_FRAME_RATE				120

	/** 
	*	@brief	Vertical front porch (per panel)
	*	@details
	*		Number of blank over-scan rows at the "bottom" of the MIPI panel. 
	*	@ingroup Config_panelsettings
	*/
    #define	PANEL_VFP						10
	
	/** 
	*	@brief	Vertical back porch (per panel)
	*	@details
	*		Number of blank over-scan rows at the "top" of the MIPI panel.
	*		Used as a blank space for panel VSYNC. If this is too low, strange
	*		things may happen like the first row showing up at the wrong spot.
	*	@ingroup Config_panelsettings
	*/	
    #define	PANEL_VBP						10
	
	/** 
	*	@brief	Vertical sync pulse width
	*	@details
	*		Vertical synchronization pulse width for "ONE" MIPI panel. Must be 
	*		smaller than the vertical back porch width.
	*	@ingroup Config_panelsettings
	*/		
    #define	PANEL_VSYNC						2
	
	/** 
	*	@brief	Horizontal back porch (per panel)
	*	@details
	*		Number of blank over-scan columns at the "left" of the MIPI panel.
	*		Used as a blank space for panel HSYNC. If this is too low, panel
	*		may not operate correctly.
	*		The panel may not light up if the pixel clock is too low. You can 
	*		increase the total pixel clock by adding arbitrarily large horizontal
	*		porch values. 
	*	@ingroup Config_panelsettings
	*/	
    #define	PANEL_HBP					30

	/** 
	*	@brief	Horizontal front porch (per panel)
	*	@details
	*		Number of blank over-scan columns at the "right" of the MIPI panel.
	*		The panel may not light up if the pixel clock is too low. You can 
	*		increase the total pixel clock by adding arbitrarily large horizontal
	*		porch values. 
	*	@ingroup Config_panelsettings
	*/		
    #define	PANEL_HFP					30

	/** 
	*	@brief	Horizontal sync pulse width
	*	@details
	*		Horizontal synchronization pulse width for "ONE" MIPI panel. Must be 
	*		smaller than the horizontal back porch width.
	*	@ingroup Config_panelsettings
	*/	
    #define	PANEL_HSYNC					2
	
	/** 
	*	@brief	Number of panels (whole display)
	*	@details
	*		Number of panels on the MIPI ports. Either one or two. In the case
	*		of two, the panels can act as one panel, split down the center.
	*	@ingroup Config_panelsettings
	*/	
	#define PANEL_COUNT							2

	/** 
	*	@brief	SLEEP_IN delay
	*	@details
	*		Delay after a MIPI DCS SLEEP_IN request (milliseconds).
	*	@ingroup Config_panelsettings
	*	@note Typically > 4 frames (70ms)
	*/	
	#define PANEL_SLEEP_IN_DELAY				70

	/** 
	*	@brief	SLEEP_OUT delay
	*	@details
	*		Delay after a MIPI DCS SLEEP_OUT request (milliseconds).
	*	@ingroup Config_panelsettings
	*	@note Typically > 6 frames (100ms)
	*/		
	#define PANEL_SLEEP_OUT_DELAY				110
	
	/** 
	*	@brief	DISPLAY_ON delay
	*	@details
	*		Delay after a MIPI DCS DISPLAY_ON request (milliseconds).
	*	@note Typically > 3 frames (40ms)
	*	@ingroup Config_panelsettings
	*/			
	#define PANEL_DISPLAY_ON_DELAY				45
	
	/** 
	*	@brief	DISPLAY_OFF delay
	*	@details
	*		Delay after a MIPI DCS DISPLAY_OFF request (milliseconds).
	*	@ingroup Config_panelsettings
	*	@note Typically > 1 frames (20ms)
	*/				
	#define PANEL_DISPLAY_OFF_DELAY				20

	/** 
	*	@brief	Number of active MIPI ports (whole display)
	*	@details
	*		Number of activated MIPI ports. Either 1, 2, 3, or 4. Typically 
	*		you want to use more ports than fewer, as that keeps the data rates 
	*		lower and less error prone.
	*	@ingroup Chicago_panelsetting
	*/	
    #define MIPI_TOTAL_PORT						4
		
	/** 
	*	@brief	Number of active MIPI lanes per port
	*	@details
	*		Number of activated MIPI lanes per port. Either 1, 2, 3, or 4. 
	*		Typically you want to use more ports than fewer, as that keeps 
	*		the data rates lower and less error prone.
	*	@ingroup Chicago_panelsetting
	*/	
	#define MIPI_LANE_NUMBER					4
		
	/** 
	*	@brief	Display compression type (whole display)
	*	@details
	*		Type of display compression used on the MIPI bus. Allows for lower
	*		MIPI clock rates on panels which support the feature. Possible values
	*		are DSC_NO_DSC, DSC_NONE, DSC_ONE_TO_THREE. See ANX735X_Programming_Guide,
	*		section 5.1.2.2 for further details.
	*	@ingroup Chicago_panelsetting
	*	@note DSC_NO_DSC is the default value
	*/	
	#define MIPI_DSC_STATUS						DSC_NO_DSC

	/** 
	*	@brief	Frame display mode (whole display)
	*	@details
	*		Configures how the image should be displayed on the panels. Can be either
	*		VIDEOMODE_SIDE, VIDEOMODE_ONE, VIDEOMODE_STACKED, or VIDEOMODE_NONE. 
	*		VIDEOMODE_ONE maps one MIPI port 00 to the whole panel. VIDEOMODE_SIDE
	*		maps port 00 to the left side of the panel, and port 10 to the right.
	*		See See ANX735X_Programming_Guide, section 5.1.2.3 for further details
	*		about other video modes.
	*	@ingroup Chicago_panelsetting
	*	@note VIDEOMODE_SIDE is the default value
	*/	
	#define	MIPI_VIDEO_MODE						VIDEOMODE_SIDE
		
	/** 
	*	@brief	First MIPI port
	*	@details
	*		Configures which MIPI port in VIDEOMODE_SIDE is considered the first.
	*		Possible values are DISPLAY_LEFT, DISPLAY_RIGHT, or DISPLAY_NONE.
	*	@ingroup Chicago_panelsetting
	*	@note DISPLAY_LEFT is the default value
	*/	
	#define MIPI_DISPLAY_EYE					DISPLAY_LEFT
		
	/** 
	*	@brief	MIPI transmit mode
	*	@details
	*		Configures how data is sent over MIPI. Possible values are MOD_BURST,
	*		MOD_NON_BURST_PULSES, and MOD_NON_BURST_EVENTS. See panel specifications
	*		to know how to properly configure.
	*	@ingroup Chicago_panelsetting
	*	@note MOD_BURST is the default value
	*/	
	#define MIPI_TRANSMIT_MODE					MOD_BURST
	
	
	/** ###########################################################################
	* @defgroup Chicago_edidsetting [Config] EDID settings
	* @details
	*	These macros configure how the VESA E-EDID is configured. Edit them in
	*	display_settings.h. See VESA_E-EDID_Standard.pdf for further details 
	*	on the EDID specification.
	*/
	
	/** 
	*	@brief	Auto-calculate EDID for panel or not
	*	@details
	*		If defined, Chicago will auto-calculate the EDID. Otherwise, it will
	*		use the raw EDID supplied in chicago.cpp. Best to leave it enabled.
	*	@ingroup Chicago_edidsetting
	*/	
	#define EDID_AUTO_CALCULATE
	
	/** 
	*	@brief	Enable or disable audio sink capability over DisplayPort.
	*	@details
	*		If 0, the device won't expose audio over DisplayPort, and a USB audio 
	*		device must be used instead. If 1, audio OUT (but no IN) is provided
	*		over the DisplayPort link.
	*	@ingroup Chicago_edidsetting
	*/	
	#define EDID_HAS_AUDIO_SINK					0
	
	/** 
	*	@brief	PNP identifier
	*	@details
	*		Plug and Play identifier, a 3-character string used for enumeration.
	*		See details at http://www.uefi.org/PNP_ACPI_Registry
	*	@ingroup Chicago_edidsetting
	*/	
	#define EDID_PNP_NAME						"LPM"
	
	/** 
	*	@brief	EDID product ID
	*	@details
	*		A 2 byte descriptor which gives the product a unique ID to latch
	*		driver configurations (if any) onto.
	*	@ingroup Chicago_edidsetting
	*/	
	#define EDID_PRODUCT_ID						0x1019

	/** 
	*	@brief	EDID display name
	*	@details
	*		A 12 byte string which provides a user-friendly name for the
	*		display device. The OS user will see this name in things like the
	*		volume control panel.
	*	@ingroup Chicago_edidsetting
	*	@note This can be less than, but must NEVER exceed 12 bytes!
	*/	
	#define  EDID_DISPLAY_NAME					"NORTHSTAR"
	
	/** 
	*	@brief	EDID screen horizontal width (cm)
	*	@ingroup Chicago_edidsetting
	*/	
	#define EDID_SCREEN_SIZE_HORIZONTAL_CM		6

	/** 
	*	@brief	EDID screen vertical height (cm)
	*	@ingroup Chicago_edidsetting
	*/	
	#define EDID_SCREEN_SIZE_VERTICAL_CM		5
	
	/** 
	*	@brief	EDID screen transfer function (gamma curve)
	*	@details
	*		Must make sure this value matches panel gamma (usually 2.2). See 
	*		MIPI_DCS_Specification_v1.02.00.pdf, section 5.2 for details.
	*	@ingroup Chicago_edidsetting
	*/
	#define EDID_SCREEN_GAMMA					1.0 //2.2
	
#endif