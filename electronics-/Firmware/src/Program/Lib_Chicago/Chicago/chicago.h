/**
* @file chicago.h
*
* @brief Chicago control functions _H
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
* @author Arduino LLC
* @author Adam Munich
*/

/**
* @defgroup Chicago_ocm [Functions] Chicago OCM
* @details
*	These functions control the Chicago On-Chip-Microcontroller behavior. They
*	include operations such as flashing, checking status, and other similar
*	functionality. See ANX753X_Programming_Guide.pdf, page 33 for detailed 
*	information.
*/		

/**
* @defgroup Chicago_statemachine [Functions] Chicago state machine
* @details
*	These functions control the Chicago state machine program flow.
*	Some functions refer to the Chicago INTP I.S.R. and its associated registers.
*	Functions exist to act upon certain characteristics of INTP, such as cable detect
*	and OCM status flags. See ANX753X_Programming_Guide.pdf, page 19 for detailed 
*	information about the state machine behavior. 
* @note 
*	chicago_main() is the entry point for the state machine and must be
*	called in a larger loop quite often.
*/

/**
* @defgroup Chicago_i2s [Functions] Chicago I2S audio 
* @details
*	These functions configure the Chicago I2S master interface.
*/

/**
* @defgroup Chicago_mipi [Functions] Chicago MIPI PHY and panel 
* @details
*	These functions configure the MIPI PHY and panel functions such as backlight, 
*	power enable, etc. See ANX753X_Programming_Guide.pdf, page 25 for detailed 
*	information.
*/

/**
* @defgroup Chicago_displayport [Functions] Chicago Displayport PHY 
* @details
*	These functions configure the Displayport PHY as well as poll it for particular
*	things (eg, pixel clock rate, video stable). See ANX753X_Programming_Guide.pdf 
*	for detailed information.
*/

/**
* @defgroup Chicago_edid [Functions] Chicago EDID
* @details
*	These functions modify and write the Chicago EDID data. See 
*	ANX753X_Programming_Guide.pdf, page 63 for detailed information.
*/


#ifndef __CHICAGO_H__
	#define __CHICAGO_H__

	#include "chicago_registers.h"

	//#############################################################################
	// Pre-compiler Definitions
	//-----------------------------------------------------------------------------
	// Register ADDR definition
	#define I2C_ADDR_SEL	0
	
	#if I2C_ADDR_SEL == 1
		#define CHICAGO_SLAVEID_ADDR		0x8E
		#define CHICAGO_OFFSET_ADDR			0x86
	#elif I2C_ADDR_SEL == 0
		#define CHICAGO_SLAVEID_ADDR		0x54
		#define CHICAGO_OFFSET_ADDR			0x52
	#endif	
	
	#define PANEL_POWER_SUPPLY_ON			1
	#define PANEL_POWER_SUPPLY_OFF			0
	#define PANEL_SLEEP_OUT					1
	#define PANEL_SLEEP_IN					0
	#define PANEL_DISPLAY_ON				1
	#define PANEL_DISPLAY_OFF				0

	#define PANEL_BRIGHTNESS_LEVEL0			0x0A
	#define PANEL_BRIGHTNESS_LEVEL1			0x40
	#define PANEL_BRIGHTNESS_LEVEL3			0x60

	#define CHICAGO_POWER_SUPPLY_ON			1
	#define CHICAGO_POWER_SUPPLY_OFF		0
	#define CHICAGO_TURN_ON					1
	#define CHICAGO_TURN_OFF				0

	#define CHICAGO_POWER_V10_DELAY			3		// Wait >= 2ms
	#define CHICAGO_POWER_V18_DELAY			2		// Wait >= 1ms
	#define CHICAGO_RESET_UP_DELAY			11		// delay >= 10ms, ocm start to work
	#define CHICAGO_RESET_DOWN_DELAY		1		
	#define CHICAGO_CHIPPOWER_UP_DELAY		21		// delay >= 20ms, for clock stable
	#define CHICAGO_CHIPPOWER_DOWN_DELAY	1		

	#define HDP_FORCE						1
	#define	HDP_UNFORCE						0
	#define HDP_DATA_HIGH					1
	#define HDP_DATA_LOW					0

	//-----------------------------------------------------------------------------
	// Description: Panel definition
	
	//#define SHARP_LS029B3SX02_FOUR_90Hz

	
	//-----------------------------------------------------------------------------
	// Description: EDID data offset definition
	// EDID position
	#define EDID_MANUFACTURER_ID_L			0x08
	#define EDID_MANUFACTURER_ID_H			0x09
	#define EDID_PRODUCT_ID_L				0x0A
	#define EDID_PRODUCT_ID_H				0x0B
	#define EDID_SERIAL_NUM_L				0x0C
	#define EDID_SERIAL_NUM_H				0x0E
	#define EDID_WEEK						0x10
	#define EDID_YEAR						0x11
	
	#define EDID_DISPLAY_NAME_START			0x05

	#define EDID_DB1_BASE					0x36
	#define EDID_DB2_BASE					0x48
	#define EDID_DB3_BASE					0x5A
	#define EDID_DB4_BASE					0x6C	

	#define EDID_DB_MAX						4
	#define EDID_DB_SIZE					18
	#define EDID_DB_DUMMY					0x10

	#define EDID_PIXEL_CLK_L				0
	#define EDID_PIXEL_CLK_H				1
	#define EDID_HACTIVE_L					2
	#define EDID_HBP_L						3
	#define EDID_HACT_HBP_H					4
	#define EDID_VACTIVE_L					5
	#define EDID_VBP_L						6
	#define EDID_VACT_VBP_H					7
	#define	EDID_HFP_L						8
	#define	EDID_HSYNC_L					9
	#define	EDID_VFP_VSYNC_L				10
	#define EDID_HFP_HSYNC_VFP_VSYNC_H		11
	#define EDID_H_DISPLAY_SIZE				12
	#define EDID_FEATURES_BITMAP			17

	#define EDID_EXTERN_PANEL_DATA			0x49

	//-----------------------------------------------------------------------------
	// Description: Display Command Set List
	#define DATASHORT_VSYNC_START			0x01
	#define DATASHORT_VSYNC_END				0x11
	#define DATASHORT_HSYNC_START			0x21
	#define DATASHORT_HSYNC_END				0x31
	#define DATASHORT_COMPRESSION_MODE		0x07
	#define DATASHORT_EoTp					0x08
	#define DATASHORT_CM_OFF				0x02
	#define DATASHORT_CM_ON					0x12
	#define DATASHORT_SHUTDOWN_PERIPH		0x22
	#define DATASHORT_TURNON_PERIPH			0x32
	#define DATASHORT_GEN_WRITE_0			0x03
	#define DATASHORT_GEN_WRITE_1			0x13
	#define DATASHORT_GEN_WRITE_2			0x23
	#define DATASHORT_GEN_READ_0			0x04
	#define DATASHORT_GEN_READ_1			0x14
	#define DATASHORT_GEN_READ_2			0x24
	#define DATASHORT_DCS_WRITE_0			0x05
	#define DATASHORT_DCS_WRITE_1			0x15
	#define DATASHORT_DCS_READ_0			0x06
	#define DATASHORT_EXE_QUEUE				0x16
	#define DATASHORT_SET_RET_SIZE			0x37
	#define DATALONG_NULL_PACKET			0x09
	#define DATALONG_BLANK_PACKET			0x19
	#define DATALONG_GEN_WRITE				0x29
	#define DATALONG_DCS_WRITE				0x39
	#define DATALONG_PIC_PARAMETER			0x0A
	#define DATALONG_COMPRESSED_STREAM		0x0B
	#define DATALONG_30RGB_STREAM			0x0D
	#define DATALONG_24RGB_STREAM			0x3E

	//-----------------------------------------------------------------------------
	// Description: DCS Command definition
	// DCS Command definition
	#define	DCS_nop							0x00
	#define	DCS_soft_reset					0x01
	#define	DCS_get_compression_mode		0x03
	#define	DCS_get_error_count_on_DSI		0x05
	#define	DCS_get_red_channel				0x06
	#define	DCS_get_green_channel			0x07
	#define	DCS_get_blue_channel			0x08
	#define	DCS_get_power_mode				0x0A
	#define	DCS_get_address_mode			0x0B
	#define	DCS_get_pixel_format			0x0C
	#define	DCS_get_display_mode			0x0D
	#define	DCS_get_signal_mode				0x0E
	#define	DCS_get_diagnostic_result		0x0F
	#define	DCS_enter_sleep_mode			0x10
	#define	DCS_exit_sleep_mode				0x11
	#define	DCS_enter_partial_mode			0x12
	#define	DCS_enter_normal_mode			0x13
	#define	DCS_get_image_checksum_rgb		0x14
	#define	DCS_get_image_checksum_ct		0x15
	#define	DCS_exit_invert_mode			0x20
	#define	DCS_enter_invert_mode			0x21
	#define	DCS_set_gamma_curve				0x26
	#define	DCS_set_display_off				0x28
	#define	DCS_set_display_on				0x29
	#define	DCS_set_column_address			0x2A
	#define	DCS_set_page_address			0x2B
	#define	DCS_write_memory_start			0x2C
	#define	DCS_write_LUT					0x2D
	#define	DCS_read_memory_start			0x2E
	#define	DCS_set_partial_rows			0x30
	#define	DCS_set_partial_columns			0x31
	#define	DCS_set_scroll_area				0x33
	#define	DCS_set_tear_off				0x34
	#define	DCS_set_tear_on					0x35
	#define	DCS_set_address_mode			0x36
	#define	DCS_set_scroll_start			0x37
	#define	DCS_exit_idle_mode				0x38
	#define	DCS_enter_idle_mode				0x39
	#define	DCS_set_pixel_format			0x3A
	#define	DCS_write_memory_continue		0x3C
	#define	DCS_set_3D_control				0x3D
	#define	DCS_read_memory_continue		0x3E
	#define	DCS_get_3D_control				0x3F
	#define	DCS_set_vsync_timing			0x40
	#define	DCS_set_tear_scanline			0x44
	#define	DCS_get_scanline				0x45
	#define	DCS_set_display_brightness		0x51
	#define	DCS_get_display_brightness		0x52
	#define	DCS_write_control_display		0x53
	#define	DCS_get_control_display			0x54
	#define	DCS_write_power_save			0x55
	#define	DCS_get_power_save				0x56
	#define	DCS_set_CABC_min_brightness		0x5E
	#define	DCS_get_CABC_min_brightness		0x5F
	#define	DCS_read_DDB_start				0xA1
	#define	DCS_read_PPS_start				0xA2
	#define	DCS_read_DDB_continue			0xA8
	#define	DCS_read_PPS_continue			0xA9

	//-----------------------------------------------------------------------------
	// Description: Video Stable Mask
	#define VIDEOSTABLE_VIDEO_STREAM			_BIT0
	#define VIDEOSTABLE_DP_SOURCE_INPUT			_BIT1
	#define VIDEOSTABLE_DP_RESOLUTION_MATCH		_BIT2

	#ifdef DEBUG_CHECK_DP_RESOLUTION
		#define VIDEOSTABLE_ALL_CONDITION	(VIDEOSTABLE_VIDEO_STREAM|VIDEOSTABLE_DP_SOURCE_INPUT|VIDEOSTABLE_DP_RESOLUTION_MATCH)
	#else
		#define VIDEOSTABLE_ALL_CONDITION	(VIDEOSTABLE_VIDEO_STREAM|VIDEOSTABLE_DP_SOURCE_INPUT)
	#endif

	//-----------------------------------------------------------------------------
	// Description: Short packet structure
	// Short packet
	typedef struct{
		unsigned char	mipi_port;	// Selected MIPI port
		unsigned char 	data_type;	// Packet Data Type
		unsigned char 	param1;		// Parameter 1
		unsigned char 	param2;		// Parameter 2
		unsigned char	*pData;		// Read data for Read Command, NULL for Write Command
	} PacketShort_t;

	//-----------------------------------------------------------------------------
	// Description: Long packet structure
	// Long packet
	typedef struct{
		unsigned char	mipi_port;	// Selected MIPI port
		unsigned char 	data_type;	// Packet Data Type
		unsigned int 	word_count;	// how many bytes are in packet payload
		unsigned char	*pData;		// PACKET DATA (Payload)
	} PacketLong_t;

	//-----------------------------------------------------------------------------
	// Description: Panel type structure
	// Panel type
	typedef struct{
		unsigned int	h_active;	// Horizon resolution
		unsigned int	v_active;	// Vertical resolution
		unsigned int	vfp;		// Vertical front proch
		unsigned int	vsync;		// Vertical sync width
		unsigned int	vbp;		// Vertical back proch
		unsigned int	hfp;		// Horizon front proch
		unsigned int	hsync;		// Horizon sync width
		unsigned int	hbp;		// Horizon back proch
	} PanelParam_t;

	//-----------------------------------------------------------------------------
	// Description: Panel transmit mode
	// Panel transmit mode
	typedef enum{
		MOD_NON_BURST_PULSES,
		MOD_NON_BURST_EVENTS,
		MOD_BURST,
	}PanelTransMod_t;

	//-----------------------------------------------------------------------------
	// Description: DSC enum
	typedef enum{
		DSC_NONE,
		DSC_NO_DSC,
		DSC_ONE_TO_THREE,
	}DSCStatus;

	//-----------------------------------------------------------------------------
	// Description: MIPI Video Mode emun
	typedef enum{
		VIDEOMODE_NONE,
		VIDEOMODE_ONE,
		VIDEOMODE_SIDE,
		VIDEOMODE_STACKED,
	}VideoMode;

	//-----------------------------------------------------------------------------
	// Description: State mechanism emun
	typedef enum{
		STATE_NONE,
		STATE_POWEROFF,	// for debug only
		STATE_WAITCABLE,
		STATE_CONNECTING,
		STATE_NORMAL,
	}ChicagoState;

	//-----------------------------------------------------------------------------
	// Description: Display eye sequence emun
	typedef enum{
		DISPLAY_NONE,
		DISPLAY_LEFT,
		DISPLAY_RIGHT
	}DisplayEye;

	//-----------------------------------------------------------------------------
	// Description: Display eye sequence emun
	typedef enum{
		DSI_VIDEO_MODE,
		DSI_CMD_MODE,
	}DSIOptMode_t;


	//#############################################################################
	// Function Prototypes
	//-----------------------------------------------------------------------------
	// external for debug used
	
	/**
	 * @brief 
	 *		Check the display port plug-in interrupt status
	 * @ingroup Chicago_displayport
	 * @return DP_CABLE_IN if cable is in
	 * @return DP_CABLE_OUT if cable is out
	 */	
	unsigned char dp_check_cable_plug_int(void);
	
	/**
	 * @brief 
	 *		Update the last_state to state
	 * @ingroup Chicago_statemachine
	 * @param state - See ChicagoState ENUM
	 * @return void
	 */	
	void chicago_last_state_change(ChicagoState state);

	/**
	 * @brief 
	 *		Update the current_state to state
	 * @ingroup Chicago_statemachine
	 * @param state - See ChicagoState ENUM
	 * @return void
	 */	
	void chicago_state_change(ChicagoState state);
		
	/**
	 * @brief
	 *		Get the Chicago state machine's current_state
	 * @ingroup Chicago_statemachine
	 * @return ChicagoState current_state
	 */	
	ChicagoState chicago_get_current_state(void);
	
	/**
	 * @brief
	 *		Get the Chicago state machine's last_state
	 * @ingroup Chicago_statemachine
	 * @return ChicagoState last_state
	 */		
	ChicagoState chicago_get_last_state(void);

	/**
	 * @brief
	 *		Stop the Chicago On Chip Microcontroller
	 * @ingroup Chicago_ocm
	 * @return void
	 */			
	void chicago_stop_ocm(void);
	
	/**
	 * @brief
	 *		Send customized settings to Chicago
	 * @ingroup Chicago_ocm
	 * @return void
	 */	
	static void chicago_customized_setting(void);
	
	
	/**
	 * @brief
	 *		Check OCM status flags on the Chicago
	 * @ingroup Chicago_ocm
	 * @return FLAG_VALUE_ON if OCM has finished loading
	 * @return FLAG_VALUE_OFF if OCM has not finished loading
	 */		
	static uint8_t chicago_check_ocm_status(void);
	
	/**
	 * @brief
	 *		Turn on I2S master audio clock
	 * @ingroup Chicago_i2s
	 * @return void
	 */	
	static void chicago_audio_mclk_always_out(void);
	
	/**
	 * @brief
	 *		Send panel parameters to the Chicago OCM
	 * @ingroup Chicago_ocm
	 * @return void
	 */		
	static void chicago_set_panel_parameters(void);
	
	/**
	 * @brief
	 *		Set the... idunno
	 * @ingroup Chicago_mipi
	 * @param mipi_port
	 * @param lp_hs state
	 * @return RETURN_NORMAL_VALUE if success
	 * @return RETURN_FAILURE_VALUE if fail
	 */			
	static char mipi_set_LP_HS_mode(uint8_t mipi_port, uint8_t lp_hs);
	
	/**
	 * @brief
	 *		Send a short packet over the MIPI PHY
	 * @ingroup Chicago_mipi
	 * @param short_packet - Mipi packet data structure
	 * @return RETURN_NORMAL_VALUE upon completion
	 */				
	static char mipi_packet_send_short(PacketShort_t *short_packet);

	static char mipi_packet_send_short_2(PacketShort_t *short_packet);

	/**
	 * @brief
	 *		Send a short packet over the MIPI PHY
	 * @ingroup Chicago_mipi
	 * @param long_packet - Mipi packet data structure
	 * @return RETURN_NORMAL_VALUE upon completion
	 */				
	char mipi_packet_send_long(PacketLong_t *long_packet);
	char mipi_packet_send_long_2(PacketLong_t *long_packet);

	/**
	 * @brief
	 *		Set the DCPDkmfckimrdidm see datahet
	 * @ingroup Chicago_mipi
	 * @return void
	 */	
	static void DPCD_setting(void);

	/**
	 * @brief
	 *		Set the Displayport Hot Plug Detect pin state
	 * @ingroup Chicago_displayport
	 * @param force - Force HPD state regardless of anything
	 * @param high_low - State of the hot plug detect pin
	 * @return RETURN_NORMAL_VALUE upon completion
	 */		
	static char chicago_hpd_set(uint8_t force, uint8_t high_low);
	
	/**
	 * @brief
	 *		Change the sleep state of the panel, and send init sequences, 
	 *		if any need to be sent.
	 * @ingroup Chicago_mipi
	 * @param onoff - PANEL_SLEEP_IN, or PANEL_SLEEP_OUT
	 * @return RETURN_NORMAL_VALUE upon completion
	 */			
	static char DCS_sleep_control(uint8_t onoff);

	/**
	 * @brief
	 *		Does absolutely nothing
	 * @ingroup Chicago_mipi
	 * @return RETURN_NORMAL_VALUE upon completiton
	 */			
	static char mipi_power_off_sequence(uint8_t onoff);

	/**
	 * @brief
	 *		Preform an cyclic-redundancy-check on flashed OCM HEX file
	 * @ingroup Chicago_ocm
	 * @return RETURN_NORMAL_VALUE if check passes
	 * @return RETURN_FAILURE_VALUE if check fails
	 * @return RETURN_FAILURE_VALUE2 if crc-check serious fault
	 */		
	static char Chicago_ocm_crc_checking(void);
	
	/**
	 * @brief
	 *		Write the GRAPHICS_CARD_DRAW_DELAY to Chicago
	 * @ingroup Chicago_displayport
	 * @return void
	 */		
	static void chicago_set_display_delay_ms(void);
	
	/**
	 * @brief
	 *		Write the DSC settings to Chicago
	 * @ingroup Chicago_mipi
	 * @return void
	 */			
	static void DSC_setting(void); 

	/**
	 * @brief
	 *		Enable packed-pixel-stream on Chicago
	 * @ingroup Chicago_mipi
	 * @return void
	 */				
	static void DSC_PPS_enable_and_DCS_init(void);
	
	/**
	 * @brief
	 *		Turns power on or off to the MIPI panel
	 * @ingroup Chicago_mipi
	 * @param onoff - Power state of the panel
	 * @return RETURN_NORMAL_VALUE upon completion
	 */		
	char mipi_panel_power_supply(unsigned char onoff);
	
	/**
	 * @brief
	 *		Sets the MIPI panel brightness value
	 * @ingroup Chicago_mipi
	 * @param bl - Brightness from 0-255
	 * @return void
	 */		
	void mipi_panel_brightness(unsigned char bl);

	/**
	 * @brief
	 *		Tests the MIPI bus with a packet request
	 * @ingroup Chicago_mipi
	 * @return void
	 */	
	void mipi_read_test(void);
	
	/**
	 * @brief
	 *		Turns the panel backlight power supply on or off
	 * @ingroup Chicago_mipi
	 * @param backlight - PANEL_DISPLAY_OFF, PANEL_DISPLAY_ON
	 * @return RETURN_NORMAL_VALUE upon completion
	 */		
	static char DCS_display_control(uint8_t backlight);
	
	/**
	 * @brief
	 *		Notify the OCM that writes to its init registers are complete
	 * @ingroup Chicago_ocm
	 * @return void
	 */			
	static void mipi_mcu_write_done(void);
	
	/**
	 * @brief
	 *		Get the measured pixel clock from the Displayport RX
	 * @ingroup Chicago_displayport
	 * @return uint32_t - Pixel clock in MHz
	 */				
	static uint32_t get_pixel_clock(void);
	
	/**
	 * @brief
	 *		Write the primary EDID buffer, and calculate the EDID before 
	 *		doing so
	 * @ingroup Chicago_edid
	 * @return RETURN_NORMAL_VALUE upon completion
	 */		
	static char mipi_edid_write_buffer(uint32_t edid_serial_number);
	
	/**
	 * @brief
	 *		Write the secondary EDID buffer
	 * @ingroup Chicago_edid
	 * @return RETURN_NORMAL_VALUE upon completion
	 */			
	static char mipi_edid_write_external_buffer(void);
	
	/**
	 * @brief
	 *		Get the calculated EDID resolution and print to Debug console 
	 * @ingroup Chicago_edid
	 * @return void
	 */				
	static void chicago_get_edid_resolution(void);	
	
	/**
	 * @brief
	 *		Check the EDID resolution against the measured DP RX resolution, 
	 *		print to Debug console
	 * @ingroup Chicago_edid
	 * @return RETURN_NORMAL_VALUE upon completion
	 */					
	static char chicago_check_dp_edid_resolution(void);
	
	/**
	 * @brief
	 *		Check if the Displayport video stream is stable or not by checking internal flags.
	 *		Not guaranteed to report correct results...
	 * @ingroup Chicago_displayport
	 * @return RETURN_FLAG_ON if video stable
	 * @return RETURN_FLAG_OFF if video is not stable
	 */						
	static uint8_t chicago_check_video_stable(void);

	/**
	 * @brief
	 *		ACTUALLY Check if the Displayport video stream is stable or not by ACTUALLY 
	 *		asking the OCM for a correct answer.
	 * @ingroup Chicago_displayport
	 * @return true if video stable
	 * @return false if video is not stable
	 */						
	static uint8_t HDK_chicago_actually_check_video_stable(void);


	/**
	 * @brief
	 *		Set M_SDA to output vsync signal.
	 * @ingroup Chicago_displayport
	 * @return void
	 */		
	void HDK_chicago_set_vsync_out(void);

	/**
	 * @brief
	 *		Clear the video_stable flag
	 * @ingroup Chicago_displayport
	 * @return void
	 */						
	static void chicago_clear_video_stable(void);

	/**
	 * @brief
	 *		Interrupt service routine for Chicago INTP
	 * @note
	 *		Bind to INTP pin on FALLING edge only.
	 * @ingroup Chicago_statemachine
	 * @return void
	 */			
	void external_int_isr(void);
	
	/**
	 * @brief
	 *		Clear Chicago INTP interrupt flag register
	 * @ingroup Chicago_statemachine
	 * @return void
	 */					
	static void clear_software_int(void);		

	/**
	 * @brief
	 *		Handle any software interrupt flags and act upon them
	 * @ingroup Chicago_statemachine
	 * @return void
	 */
	void chicago_interrupt_handle(void);

	/**
	 * @brief
	 *		Does absolutely nothing and serves no purpose
	 * @ingroup Chicago_statemachine
	 * @return RETURN_NORMAL_VALUE
	 */	
	static char chicago_interrupt_set(void);
	
	/**
	 * @brief
	 *		Reads the Chicago chip_id register and prints to serial
	 *		debug console
	 * @ingroup Chicago_debug
	 * @return void
	 */		
	static void chicago_read_chip_id(void);
	
	/**
	 * @brief
	 *		Turns power supply to Chicago on or off
	 * @ingroup Chicago_ocm
	 * @param onoff - On[1], Off[0]
	 * @return RETURN_NORMAL_VALUE upon completion
	 */		
	char chicago_power_supply(unsigned char onoff);

	/**
	 * @brief
	 *		Disables/Enables both Chicago PWEN and NRESET pins
	 * @ingroup Chicago_ocm
	 * @param onoff - On[1], Off[0]
	 * @return RETURN_NORMAL_VALUE upon completion
	 */	
	char chicago_power_onoff(unsigned char onoff);
	
	/**
	 * @brief
	 *		Clears state of the interrupt status registers and optionally
	 *		prints result to debug console
	 * @ingroup Chicago_debug
	 * @return void
	 */		
	void HDK_chicago_clear_intr_state(uint8_t readOut);
	
	/**
	 * @brief
	 *		Reads state of the interrupt status registers and 
	 *		prints result to debug console
	 * @ingroup Chicago_debug
	 * @return void
	 */	
	void chicago_read_intr_state(void);

	/**
	 * @brief
	 *		Main state machine where all the magic happens
	 * @note 
	 *		Call this function from main program at least every 50ms
	 * @ingroup Chicago_statemachine
	 * @return void
	 */	
	void chicago_main(void);

#endif  /* __CHICAGO_H__ */

