/**
* @file chicago.cpp
*
* @brief Chicago control functions
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
// Pre-compiler Definitions
//-----------------------------------------------------------------------------
#include <Arduino.h>
#include <variant.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "./chicago.h"
#include "./chicago_config.h"

#include "../I2C/i2c.h"
#include "../Debug/debug.h"

#include "../../display_settings.h"


//#############################################################################
// Pre-compiler Definitions
//-----------------------------------------------------------------------------
#define EDID_DB_MAX						4

#define DP_CABLE_IN						1
#define DP_CABLE_OUT					0

#define MIPI_PORT_LP					0
#define MIPI_PORT_HS					1

#define EDID_LENGTH						128
#define EDID_EXTERNAL_LENGTH			128

// 700ms for intel 520 DP flash issue // this is graphic card issue
// 1200ms for GTX970 draw issue // this is graphic card issue
#define GRAPHIC_CARD_DRAW_DELAY			1200 


//#############################################################################
// Variable Declarations
//-----------------------------------------------------------------------------

static uint8_t edid_hdmi[EDID_LENGTH] = {
 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x05, 0xD8, 0x30, 0x75, 0x01, 0x00, 0x00, 0x00,
 0x01, 0x1B, 0x01, 0x03, 0x81, 0x06, 0x05, 0x78, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x7B, 0x9A, 0x40, 0x8C, 0xB0, 0xA0, 0x10, 0x50, 0x30, 0x08,
 0x81, 0x00, 0x3C, 0x32, 0x00, 0x00, 0x00, 0x10, 0x0F, 0x67, 0x40, 0x8C, 0xB0, 0xA0, 0x10, 0x50,
 0x30, 0x08, 0x81, 0x00, 0x3C, 0x32, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
 0x00, 0x41, 0x4E, 0x58, 0x37, 0x35, 0x33, 0x30, 0x20, 0x48, 0x44, 0x4D, 0x49, 0x0A, 0x01, 0x59};

 static uint8_t edid_external_hdmi[EDID_EXTERNAL_LENGTH] = {
 0x02, 0x03, 0x4D, 0xC2, 0x5B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x29, 0x09, 0x07, 0x07, 0x11, 0x17, 0x50, 0x51, 0x07, 0x00, 0x83, 0x01, 0x00, 0x00, 0x76, 0x03,
 0x0C, 0x00, 0x10, 0x00, 0x00, 0x44, 0x20, 0xC0, 0x84, 0x01, 0x02, 0x03, 0x04, 0x01, 0x41, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0xD8, 0x5D, 0xC4, 0x01, 0x78, 0x80, 0x00, 0x56, 0x98, 0x40,
 0x60, 0xB0, 0xA0, 0x10, 0x50, 0x30, 0x08, 0x81, 0x00, 0x3C, 0x32, 0x00, 0x00, 0x00, 0x10, 0x8E,
 0x65, 0x40, 0x60, 0xB0, 0xA0, 0x10, 0x50, 0x30, 0x08, 0x81, 0x00, 0x3C, 0x32, 0x00, 0x00, 0x00,
 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2E};

static uint8_t edid_dp[EDID_LENGTH] = {
 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x05, 0xD8, 0x30, 0x75, 0x01, 0x00, 0x00, 0x00,
 0x01, 0x1B, 0x01, 0x04, 0xA5, 0x06, 0x05, 0x78, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x56, 0x98, 0x40, 0x60, 0xB0, 0xA0, 0x10, 0x50, 0x30, 0x08,
 0x81, 0x00, 0x3C, 0x32, 0x00, 0x00, 0x00, 0x10, 0x8E, 0x65, 0x40, 0x60, 0xB0, 0xA0, 0x10, 0x50,
 0x30, 0x08, 0x81, 0x00, 0x3C, 0x32, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
 0x00, 0x41, 0x4E, 0x58, 0x37, 0x35, 0x33, 0x30, 0x20, 0x44, 0x50, 0x0A, 0x20, 0x20, 0x01, 0x8C};

 static uint8_t edid_external_dp[EDID_EXTERNAL_LENGTH] = {
 0x02, 0x03, 0x2E, 0xC0, 0x5B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x29, 0x09, 0x07, 0x07, 0x11, 0x17, 0x50, 0x51, 0x07, 0x00, 0x83, 0x01, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E};
 

static uint8_t *edid_buffer;
static uint8_t *edid_external_buffer;
 
static ChicagoState	current_state		= STATE_NONE;
static ChicagoState last_state			= STATE_NONE;
static uint8_t dp_cable					= DP_CABLE_OUT;

static uint8_t video_stable				= 0;
static uint8_t dcs_port_seq[4]			= {0,2,1,3};

//uint32_t  tmp_vtotal;
static uint32_t tmp_vbp, tmp_vfp, tmp_vsync, tmp_v_active;
static uint32_t tmp_hbp, tmp_hfp, tmp_hsync, tmp_h_active;
static uint32_t temp_value;

//For HPD signal
static uint8_t set_HPD;

//External interrupt flag
uint8_t intr_flag;

// for Debug Switch in EVB only
uint8_t debug_switch					= 0xFF;

static uint32_t edid_resolution[EDID_DB_MAX][2];
static uint8_t  edid_db_count;


//#############################################################################
// Function Definitions
//-----------------------------------------------------------------------------
/// @copydoc chicago_customized_setting
static void chicago_customized_setting(void){

	#ifdef DEBUG_LEVEL_2
		TRACE0("chicago_customized_setting(void)\n");
	#endif
		
	// REG7_0_RX_REG7
	i2c_write_byte(SLAVEID_SERDES, REG7_0_RX_REG7, 0x4D); // 1.62G boosting

	// REG7_1_RX_REG7
	i2c_write_byte(SLAVEID_SERDES, REG7_1_RX_REG7, 0x6D); // 2.7G boosting

	// REG7_2_RX_REG7
	i2c_write_byte(SLAVEID_SERDES, REG7_2_RX_REG7, 0x8D); 

	// REG0_0_RX_REG0
	i2c_write_byte(SLAVEID_SERDES, REG0_0_RX_REG0, 0x20);

	// REG0_1_RX_REG0
	i2c_write_byte(SLAVEID_SERDES, REG0_1_RX_REG0, 0x20);

	// REG0_3_RX_REG0
	i2c_write_byte(SLAVEID_SERDES, REG0_3_RX_REG0, 0xDF); // enable 6.75G 

	// Commented for test PATTERN mode, no need when use OCM
	// Video related block auto reset disable.
	//i2c_write_byte(SLAVEID_DP_TOP, ADDR_RST_SEL_0, 0xFB);
	// Video power is control by software.
	//i2c_write_byte(SLAVEID_DP_TOP, ADDR_PWD_SEL, 0x04);
	// Video PLL related block auto reset disable
	//i2c_write_byte(SLAVEID_DP_TOP, ADDR_RST_SEL_1, 0xFE);
	
	// {0xC038, 0x4, {0x01, 0x28, 0x00, 0x00}},
	i2c_write_byte4(SLAVEID_MIPI_PORT0, VID_MODE_CFG, 0x00002801);

	// MIPI TX reset
	i2c_write_byte4(SLAVEID_MIPI_PORT0, PWR_UP, 0x00000000);
	// MIPI TX power-up, 

	i2c_write_byte4(SLAVEID_MIPI_PORT0, PWR_UP, 0x00000001);

	// delay 5 ms
	delay_ms(5);

	i2c_write_byte4(SLAVEID_MIPI_PORT2, VID_MODE_CFG, 0x00002801);

	// MIPI TX reset
	i2c_write_byte4(SLAVEID_MIPI_PORT2, PWR_UP, 0x00000000);

	// MIPI TX power-up, 
	i2c_write_byte4(SLAVEID_MIPI_PORT2, PWR_UP, 0x00000001);
	
	// 24MHz
	i2c_write_byte(SLAVEID_DP_TOP, ADDR_MISC_CTRL, 0x03);	
}

//-----------------------------------------------------------------------------
/// @copydoc chicago_check_ocm_status
static uint8_t chicago_check_ocm_status(void){

	#ifdef DEBUG_LEVEL_2
		TRACE0("chicago_check_ocm_status(void)\n");
	#endif	
	
	uint8_t reg_temp;
	
	// Checking OCM status
	i2c_read_byte(SLAVEID_SERDES,SERDES_POWER_CONTROL,&reg_temp);

	if((OCM_LOAD_DONE & reg_temp) == OCM_LOAD_DONE){

		#ifdef DEBUG_LEVEL_2
			TRACE1("%s\n", "\tOCM loaded");
		#endif
		return(FLAG_VALUE_ON);
	}
	else{

		#ifdef DEBUG_LEVEL_2
			TRACE1("%s\n", "\tOCM NOT loaded");
		#endif		
		return(FLAG_VALUE_OFF);
	}
}

//-----------------------------------------------------------------------------
/// @copydoc chicago_audio_mclk_always_out
static void chicago_audio_mclk_always_out(void){
	
	#ifdef DEBUG_LEVEL_2
		TRACE0("chicago_audio_mclk_always_out(void)\n");
	#endif		
	
	uint8_t reg_temp;
	
	// Set Maud, (if don't set Maud in MCU, the default value in OCM is 1677)
	// MCLK = Maud*240/Naud
	// use default value 12.288MHz
	//i2c_write_byte(SLAVEID_SPI,SW_AUD_MAUD_SVAL_7_0,0x00);
	//i2c_write_byte(SLAVEID_SPI,SW_AUD_MAUD_SVAL_15_8,0x02);
	//i2c_write_byte(SLAVEID_SPI,SW_AUD_MAUD_SVAL_23_16,0x00);
	//i2c_write_byte(SLAVEID_SPI,SW_AUD_NAUD_SVAL_7_0,0x10);
	//i2c_write_byte(SLAVEID_SPI,SW_AUD_NAUD_SVAL_15_8,0x27);
	//i2c_write_byte(SLAVEID_SPI,SW_AUD_NAUD_SVAL_23_16,0x00);
	
	// Set audio MCLK always on
	i2c_read_byte(SLAVEID_SPI,MISC_NOTIFY_OCM0,&reg_temp);

	reg_temp |= AUD_MCLK_ALWAYS_ON;

	i2c_write_byte(SLAVEID_SPI,MISC_NOTIFY_OCM0,reg_temp);
}

//-----------------------------------------------------------------------------
/// @copydoc chicago_set_panel_parameters
static void chicago_set_panel_parameters(void){

	#ifdef DEBUG_LEVEL_2
		TRACE0("chicago_set_panel_parameters(void)\n");
	#endif			
	
	uint8_t reg_temp;
	
	// write H active
	i2c_write_byte(SLAVEID_SPI, SW_H_ACTIVE_L, (uint8_t)((uint32_t)PANEL_H_ACTIVE&0x00FF));
	i2c_read_byte(SLAVEID_SPI, SW_H_ACTIVE_H, &reg_temp);

	reg_temp &= ~SW_H_ACTIVE_H_BITS;
	reg_temp |= (uint8_t)(((uint32_t)PANEL_H_ACTIVE>>8)&SW_H_ACTIVE_H_BITS);

	i2c_write_byte(SLAVEID_SPI, SW_H_ACTIVE_H, reg_temp);
	
	// write HFP
	i2c_write_byte(SLAVEID_SPI, SW_HFP_L, (uint8_t)((uint32_t)PANEL_HFP&0x00FF));
	i2c_read_byte(SLAVEID_SPI, SW_HFP_H, &reg_temp);

	reg_temp &= ~SW_HFP_H_BITS;
	reg_temp |= (uint8_t)(((uint32_t)PANEL_HFP>>8)&SW_HFP_H_BITS);

	i2c_write_byte(SLAVEID_SPI, SW_HFP_H, reg_temp);

	// write HSYNC
	i2c_write_byte(SLAVEID_SPI, SW_HSYNC_L, (uint8_t)((uint32_t)PANEL_HSYNC&0x00FF));
	i2c_read_byte(SLAVEID_SPI, SW_HSYNC_H, &reg_temp);

	reg_temp &= ~SW_HSYNC_H_BITS;
	reg_temp |= (uint8_t)(((uint32_t)PANEL_HSYNC>>8)&SW_HSYNC_H_BITS);

	i2c_write_byte(SLAVEID_SPI, SW_HSYNC_H, reg_temp);

	// write HBP
	i2c_write_byte(SLAVEID_SPI, SW_HBP_L, (uint8_t)((uint32_t)PANEL_HBP&0x00FF));
	i2c_read_byte(SLAVEID_SPI, (uint16_t)SW_HBP_H, &reg_temp);

	reg_temp &= ~SW_HBP_H_BITS;
	reg_temp |= (uint8_t)(((uint32_t)PANEL_HBP>>8)&SW_HBP_H_BITS);

	i2c_write_byte(SLAVEID_SPI, SW_HBP_H, reg_temp);

	// write V active
	i2c_write_byte(SLAVEID_SPI, SW_V_ACTIVE_L, (uint8_t)((uint32_t)PANEL_V_ACTIVE&0x00FF));
	i2c_read_byte(SLAVEID_SPI, SW_V_ACTIVE_H, &reg_temp);

	reg_temp &= ~SW_V_ACTIVE_H_BITS;
	reg_temp |= (uint8_t)(((uint32_t)PANEL_V_ACTIVE>>8)&SW_V_ACTIVE_H_BITS);

	i2c_write_byte(SLAVEID_SPI, SW_V_ACTIVE_H, reg_temp);
	
	// write VFP
	i2c_write_byte(SLAVEID_SPI, SW_VFP_L, (uint8_t)((uint32_t)PANEL_VFP&0x00FF));
	i2c_read_byte(SLAVEID_SPI, SW_VFP_H, &reg_temp);

	reg_temp &= ~SW_VFP_H_BITS;
	reg_temp |= (uint8_t)(((uint32_t)PANEL_VFP>>8)&SW_VFP_H_BITS);

	i2c_write_byte(SLAVEID_SPI, SW_VFP_H, reg_temp);

	// write VSYNC
	i2c_write_byte(SLAVEID_SPI, SW_VSYNC_L, (uint8_t)((uint32_t)PANEL_VSYNC&0x00FF));
	i2c_read_byte(SLAVEID_SPI, SW_VSYNC_H, &reg_temp);

	reg_temp &= ~SW_VSYNC_H_BITS;
	reg_temp |= (uint8_t)(((uint32_t)PANEL_VSYNC>>8)&SW_VSYNC_H_BITS);

	i2c_write_byte(SLAVEID_SPI, SW_VSYNC_H, reg_temp);

	// write VBP
	i2c_write_byte(SLAVEID_SPI, SW_VBP_L, (uint8_t)((uint32_t)PANEL_VBP&0x00FF));
	i2c_read_byte(SLAVEID_SPI, SW_VBP_H, &reg_temp);

	reg_temp &= ~SW_VBP_H_BITS;
	reg_temp |= (uint8_t)(((uint32_t)PANEL_VBP>>8)&SW_VBP_H_BITS);

	i2c_write_byte(SLAVEID_SPI, SW_VBP_H, reg_temp);

	// write Frame Rate
	i2c_write_byte(SLAVEID_SPI, SW_PANEL_FRAME_RATE, (uint8_t)PANEL_FRAME_RATE);

	// write SW_PANEL_INFO_0
	reg_temp = 0;

	// set video mode
	switch(MIPI_VIDEO_MODE){
		case VIDEOMODE_SIDE:
			reg_temp |= (((uint8_t)(0x01)<<REG_PANEL_VIDEO_MODE_SHIFT)&REG_PANEL_VIDEO_MODE);
		break;
		case VIDEOMODE_STACKED:
			reg_temp |= (((uint8_t)(0x02)<<REG_PANEL_VIDEO_MODE_SHIFT)&REG_PANEL_VIDEO_MODE);
		break;
		default:
			// do not thing;
		break;
	}

	// set MIPI lane number
	switch(MIPI_LANE_NUMBER){
		case 4:
		case 3:
		case 2:
		case 1:
			reg_temp |= (((uint8_t)(MIPI_LANE_NUMBER-1)<<REG_MIPI_LANE_COUNT_SHIFT)&REG_MIPI_LANE_COUNT);
		break;
		default:
			// do not thing;
		break;
	}

	// set MIPI total port
	switch(MIPI_TOTAL_PORT){
		case 4:
		case 3:
		case 2:
		case 1:
			reg_temp |= (((uint8_t)(MIPI_TOTAL_PORT-1)<<REG_MIPI_TOTAL_PORT_SHIFT)&REG_MIPI_TOTAL_PORT);
		break;
		default:
			// do not thing;
		break;
	}

	// set panel count
	switch(PANEL_COUNT){
		case 4:
		case 3:
		case 2:
		case 1:
			reg_temp |= (((uint8_t)(PANEL_COUNT-1)<<REG_PANEL_COUNT_SHIFT)&REG_PANEL_COUNT);
		break;
		default:
			// do not thing;
		break;
	}

	i2c_write_byte(SLAVEID_SPI, SW_PANEL_INFO_0, reg_temp);
		
	// write SW_PANEL_INFO_1
	i2c_read_byte(SLAVEID_SPI, SW_PANEL_INFO_1, &reg_temp);

	reg_temp &= ~REG_PANEL_ORDER;

	switch(MIPI_DISPLAY_EYE){
		case DISPLAY_RIGHT:
			reg_temp |= REG_PANEL_ORDER;
		break;
		default:
			// do not thing;
		break;
	}

	reg_temp &= ~REG_PANEL_DSC_MODE;

	switch(MIPI_DSC_STATUS){
		case DSC_ONE_TO_THREE:
			reg_temp |= REG_PANEL_DSC_MODE;
		break;
		default:
			// do not thing;
		break;
	}

	reg_temp &= ~REG_PANEL_TRANS_MODE;

	switch(MIPI_TRANSMIT_MODE){
		case MOD_NON_BURST_PULSES:
			reg_temp |= (((uint8_t)(0x00)<<REG_PANEL_TRANS_MODE_SHIFT)&REG_PANEL_TRANS_MODE);
		break;
		case MOD_NON_BURST_EVENTS:
			reg_temp |= (((uint8_t)(0x01)<<REG_PANEL_TRANS_MODE_SHIFT)&REG_PANEL_TRANS_MODE);
		break;
		case MOD_BURST:
		default:
			reg_temp |= (((uint8_t)(0x02)<<REG_PANEL_TRANS_MODE_SHIFT)&REG_PANEL_TRANS_MODE);
		break;
	}

	// write DIP switch
	reg_temp &= ~VIDEO_BIST_MODE;
	
	//if(DIP_SWITCH_ON==(debug_switch&DS_BIST)){
	#ifdef DEBUG_ENABLE_COLOR_BARS
		reg_temp |= VIDEO_BIST_MODE;
	#endif
	//}
	
	// Set DPHY timing that be calculated by OCM
	switch(MIPI_DSC_STATUS){
		case DSC_NO_DSC:
			reg_temp |= SET_DPHY_TIMING;
		break;
		case DSC_ONE_TO_THREE:
			reg_temp |= SET_DPHY_TIMING;
		break;
		default:
			// do not thing;
		break;
	}
	
	// Set all SW_PANEL_INFO_1 done
	i2c_write_byte(SLAVEID_SPI, SW_PANEL_INFO_1, reg_temp);
	
	// This bit should be set after parameters setting down !!!
	i2c_read_byte(SLAVEID_SPI, MISC_NOTIFY_OCM0, &reg_temp);

	reg_temp |= PANEL_INFO_SET_DONE;

	i2c_write_byte(SLAVEID_SPI, MISC_NOTIFY_OCM0, reg_temp);
}

//-----------------------------------------------------------------------------
/// @copydoc mipi_set_LP_HS_mode
static char mipi_set_LP_HS_mode(uint8_t mipi_port, uint8_t lp_hs){		
	
	#ifdef DEBUG_LEVEL_2
		TRACE2("mipi_set_LP_HS_mode(uint8_t mipi_port=%d, uint8_t lp_hs=%d)\n", mipi_port, lp_hs);
	#endif				
	
	uint8_t slave_id;
	uint32_t reg_long;

	switch(mipi_port) {
		case 0:
			slave_id = SLAVEID_MIPI_PORT0;
			break;
		case 1:
			slave_id = SLAVEID_MIPI_PORT1;
			break;
		case 2:
			slave_id = SLAVEID_MIPI_PORT2;
			break;
		case 3:
			slave_id = SLAVEID_MIPI_PORT3;
			break;
		default:
			TRACE1("%s\n", "\tMIPI port selected error!");
			return RETURN_FAILURE_VALUE;
	}

	// Select port first
	i2c_write_byte(SLAVEID_MIPI_CTRL, R_MIP_TX_SELECT,((0x10)<<(mipi_port)));

	if(MIPI_PORT_LP==lp_hs){
		reg_long = 0x00000001;
	}else if(MIPI_PORT_HS==lp_hs){
		reg_long = 0x00000000;
	}else{
		TRACE1("%s\n", "\tMIPI mode set error!");
		return RETURN_FAILURE_VALUE;
	}
	
	i2c_write_byte4(slave_id, MODE_CFG, reg_long);

	// MIPI TX reset
	reg_long = 0x00000000;
	i2c_write_byte4(slave_id, PWR_UP, reg_long);
	
	// MIPI TX power-up
	reg_long = 0x00000001;
	i2c_write_byte4(slave_id, PWR_UP, reg_long);
	
	return RETURN_NORMAL_VALUE;
}
	
//-----------------------------------------------------------------------------
/// @copydoc mipi_packet_send_short
static char mipi_packet_send_short(PacketShort_t *short_packet){		
	
	#ifdef DEBUG_LEVEL_3
		TRACE1("mipi_packet_send_short(PacketShort_t *short_packet=%x)\n", short_packet);
	#endif				
	
	uint8_t slave_id;
	uint32_t reg_long;

	switch(short_packet->mipi_port) {
	case 0:
		slave_id = SLAVEID_MIPI_PORT0;
		break;
	case 1:
		slave_id = SLAVEID_MIPI_PORT1;
		break;
	case 2:
		slave_id = SLAVEID_MIPI_PORT2;
		break;
	case 3:
		slave_id = SLAVEID_MIPI_PORT3;
		break;
	default:
		TRACE1("%s\n", "\tMIPI port selected error!");
		return RETURN_FAILURE_VALUE;
	}

	// Select port first
	i2c_write_byte(SLAVEID_MIPI_CTRL, R_MIP_TX_SELECT,((0x10)<<(short_packet->mipi_port)));

	// Write Data Type
	reg_long = (((uint32_t)short_packet->data_type)&0x000000FF);

	// Send other parameters
	switch(short_packet->data_type) {
	case DATASHORT_EoTp:
		reg_long = (reg_long | (((uint32_t)0x0F)<<8));
		reg_long = (reg_long | (((uint32_t)0x0F)<<16));
		break;
	default:
		reg_long = (reg_long | (((uint32_t)short_packet->param1)<<8));
		reg_long = (reg_long | (((uint32_t)short_packet->param2)<<16));
		break;
	}

	i2c_write_byte4(slave_id, GEN_HDR, reg_long);

	// Get return data for Read command
	switch (short_packet->data_type) {
	case DATASHORT_GEN_READ_0:
	case DATASHORT_GEN_READ_1:
	case DATASHORT_GEN_READ_2:
	case DATASHORT_DCS_READ_0:	
		// Wait > 500us
		delay_ms(1);
		i2c_read_block(slave_id, GEN_PLD_DATA, &short_packet->pData[0], 4);	
		break;
	default:
		// Do not thing
		break;
	}

	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
char mipi_packet_send_long(PacketLong_t *long_packet){
	
	#ifdef DEBUG_LEVEL_3
		TRACE1("mipi_packet_send_long(PacketLong_t *long_packet=%x)\n", long_packet);
	#endif		
		
	uint8_t			slave_id;
	uint32_t		div,mod,i;
	uint32_t		reg_long;

	switch(long_packet->mipi_port) {
		case 0:
			slave_id = SLAVEID_MIPI_PORT0;
			break;
		case 1:
			slave_id = SLAVEID_MIPI_PORT1;
			break;
		case 2:
			slave_id = SLAVEID_MIPI_PORT2;
			break;
		case 3:
			slave_id = SLAVEID_MIPI_PORT3;
			break;
		default:
			TRACE1("\t%s\n", "MIPI port selected error!");
			return RETURN_FAILURE_VALUE;
	}

	div = long_packet->word_count/4;
	mod = long_packet->word_count%4;

	// Select port first
	i2c_write_byte(SLAVEID_MIPI_CTRL, R_MIP_TX_SELECT,((0x10)<<(long_packet->mipi_port)));

	// Write Payload first
	for(i=0; i<div; i++) {
		reg_long = (((uint32_t)(long_packet->pData[i*4])) | (((uint32_t)(long_packet->pData[i*4+1]))<<8) |
			(((uint32_t)(long_packet->pData[i*4+2]))<<16) | (((uint32_t)(long_packet->pData[i*4+3]))<<24) );

		i2c_write_byte4(slave_id, GEN_PLD_DATA, reg_long);
	}

	switch(mod) {
		case 3:
			reg_long = (((uint32_t)(long_packet->pData[i*4])) | (((uint32_t)(long_packet->pData[i*4+1]))<<8) |
				(((uint32_t)(long_packet->pData[i*4+2]))<<16) );
			i2c_write_byte4(slave_id, GEN_PLD_DATA, reg_long);
			break;
		case 2:
			reg_long = (((uint32_t)(long_packet->pData[i*4])) | (((uint32_t)(long_packet->pData[i*4+1]))<<8) );
			i2c_write_byte4(slave_id, GEN_PLD_DATA, reg_long);
			break;
		case 1:
			reg_long = ((uint32_t)(long_packet->pData[i*4]));
			i2c_write_byte4(slave_id, GEN_PLD_DATA, reg_long);
			break;
		case 0:
			// Do not thing
			break;
		default:
			assert(0);
	}

	// Write long packet data type
	// Write long packet word count
	
	reg_long = (((uint32_t)(long_packet->data_type)) | (((uint32_t)(long_packet->word_count&0x00FF))<<8)
		| (((uint32_t)((long_packet->word_count>>8)&0x00FF))<<16));
	
	i2c_write_byte4(slave_id, GEN_HDR, reg_long);

	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
/// @copydoc DPCD_setting
static void DPCD_setting(void)
{
}

//-----------------------------------------------------------------------------
/// @copydoc chicago_hpd_set
static char chicago_hpd_set(uint8_t force, uint8_t high_low){
	
	#ifdef DEBUG_LEVEL_2
	TRACE2("chicago_hpd_set(uint8_t force=%d, uint8_t high_low=%d)\n", force, high_low);
	#endif
	
	uint8_t reg_temp;

	i2c_read_byte(SLAVEID_DP_IP, ADDR_SYSTEM_CTRL_0, &reg_temp);

	if(HDP_DATA_HIGH==high_low){ // HPD set to High
		reg_temp |= FORCE_HPD_VALUE;
		}else{ // HPD set to Low
		reg_temp &= ~FORCE_HPD_VALUE;
	}

	if(HDP_FORCE==force){ // HPD force
		reg_temp |= FORCE_HPD_EN;
		}else{
		reg_temp &= ~FORCE_HPD_EN;
	}

	i2c_write_byte(SLAVEID_DP_IP, ADDR_SYSTEM_CTRL_0, reg_temp);
	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
char mipi_panel_power_supply(uint8_t onoff){

	#ifdef DEBUG_LEVEL_2
	TRACE1("mipi_panel_power_supply(uint8_t onoff=%d)\n", onoff);
	#endif
	
		if(onoff == PANEL_POWER_SUPPLY_OFF){

			// Turn OFF backlight
			//CHICAGO_PANEL_V33_DOWN();
			//HDK_LCD_BL_OFF();

			// Power Supply OFF
			// RESET pin control for Sharp LS029B3SX02
			CHICAGO_PANEL_V18_DOWN();
			CHICAGO_PANEL_V33_DOWN();
			CHICAGO_PANEL_V55_DOWN();
			CHICAGO_PANEL_V55N_DOWN();
			#ifdef DEBUG_LEVEL_2
				TRACE1("\t%s\n", "Panel power supplies OFF");
			#endif
			
			// Initial condition
			CHICAGO_PANEL0_RESET_DOWN();
			CHICAGO_PANEL1_RESET_DOWN();		
			#ifdef DEBUG_LEVEL_2
				TRACE1("\t%s\n", "Panel n_reset LOW");
			#endif			
			
			CHICAGO_PANEL0_ENPORT_OFF();
			CHICAGO_PANEL1_ENPORT_OFF();				
			#ifdef DEBUG_LEVEL_2
				TRACE1("\t%s\n", "Panel enport LOW");
			#endif					
			delay_ms(2);
		}
	
		// ON --------------
		else{
			// Power Supply ON
			// RESET pin control for Sharp LS029B3SX02
			CHICAGO_PANEL_V18_DOWN();
			CHICAGO_PANEL_V33_DOWN();
			CHICAGO_PANEL_V55_DOWN();
			CHICAGO_PANEL_V55N_DOWN();
			#ifdef DEBUG_LEVEL_2
				TRACE1("\t%s\n", "Panel power supplies OFF");
			#endif
			
			// Reset Low
			CHICAGO_PANEL0_RESET_DOWN();
			CHICAGO_PANEL1_RESET_DOWN();
			
			#ifdef DEBUG_LEVEL_2
				TRACE1("\t%s\n", "Panel n_reset LOW");
			#endif			

			delay_ms(2);
			
			// Power Supply VDDI (Typ. 1.8V)
			CHICAGO_PANEL_V18_ON();

			// delay more then 10ms, for HW request
			delay_ms(10);

			// Power Supply AVDD+ (Typ. +5.5V)
			CHICAGO_PANEL_V55N_ON();

			// Wait > 1ms
			delay_ms(2);

			// Power Supply AVDD- (Typ. -5.5V)
			CHICAGO_PANEL_V55_ON();

			#ifdef DEBUG_LEVEL_2
				TRACE1("\t%s\n", "Panel power supplies ON");
			#endif
			
			// Wait, >= 10ms
			delay_ms(20);

			/*
			// Reset High
			CHICAGO_PANEL0_RESET_UP();
			CHICAGO_PANEL1_RESET_UP();
			#ifdef DEBUG_LEVEL_2
				TRACE1("\t%s\n", "Panel n_reset HIGH");
			#endif
			*/
		
			// delay more then 2ms, for HW request
			delay_ms(15);

			// Reset High
			CHICAGO_PANEL0_RESET_UP();
			CHICAGO_PANEL1_RESET_UP();

			#ifdef DEBUG_LEVEL_2
				TRACE1("\t%s\n", "Panel n_reset HIGH");
			#endif
			
			// Wait, >= 10ms
			delay_ms(15);
		}

	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
/// @copydoc mipi_power_on_sequence
static char DCS_sleep_control(uint8_t onoff){
	
	#ifdef DEBUG_LEVEL_2
		TRACE1("mipi_power_on_sequence_dcs_sleep_control(uint8_T onoff=%d)\n", onoff);
	#endif	

	PacketShort_t	dcs;

	if(onoff == PANEL_SLEEP_IN){
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "Sleep In");
		#endif
		
		// Panel power OFF sequence
		// Sleep In
		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			dcs.mipi_port = dcs_port_seq[i];
			dcs.data_type = DATASHORT_DCS_WRITE_0;
			dcs.param1 = DCS_enter_sleep_mode;
			dcs.param2 = 0;
			dcs.pData = NULL;
			mipi_packet_send_short(&dcs);
			delay_ms(5);
		}
		
		// Wait, > 4 frame (60Hz = 66.666ms)
		delay_ms(PANEL_SLEEP_IN_DELAY);
	}
	
	else{
		#ifdef DEBUG_LEVEL_2
		TRACE1("\t%s\n", "Sleep Out");
		#endif
				
		// Sleep Out
		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			dcs.mipi_port = dcs_port_seq[i];
			dcs.data_type = DATASHORT_DCS_WRITE_0;
			dcs.param1 = DCS_exit_sleep_mode;
			dcs.param2 = 0;
			dcs.pData = NULL;
			mipi_packet_send_short(&dcs);
			delay_ms(5);
		}
		
		// Wait, >= 100ms
		delay_ms(PANEL_SLEEP_OUT_DELAY);
	}
	
	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
void mipi_panel_brightness(uint8_t bl){

	#ifdef DEBUG_LEVEL_2
		TRACE1("mipi_panel_brightness(uint8_T bl=%d)", bl);
	#endif	
		
	uint8_t	i;
	PacketLong_t	long_dcs;
	uint8_t	long_data[8];

	#ifdef DEBUG_LEVEL_2
		TRACE1("\tmipi_panel_brightness = %d\n", bl);
	#endif	

	long_data[0] = 0xB0;
	long_data[1] = 0x04;
	long_data[2] = 0x00;
	long_data[3] = 0x00;
	
	for(i = 0; i < MIPI_TOTAL_PORT; i++){
		long_dcs.mipi_port = dcs_port_seq[i];
		long_dcs.data_type = DATALONG_GEN_WRITE;
		long_dcs.word_count = 2;
		long_dcs.pData = &long_data[0];
		mipi_packet_send_long(&long_dcs);
	}

	long_data[0] = 0xD6;
	long_data[1] = 0x01;
	long_data[2] = 0x00;
	long_data[3] = 0x00;
	
	for(i = 0; i < MIPI_TOTAL_PORT; i++){
		long_dcs.mipi_port = dcs_port_seq[i];
		long_dcs.data_type = DATALONG_GEN_WRITE;
		long_dcs.word_count = 2;
		long_dcs.pData = &long_data[0];
		mipi_packet_send_long(&long_dcs);
	}

	delay_ms(40);

	// PWM
	long_data[0] = 0xEE;
	long_data[1] = 0x01;
	long_data[2] = 0x32;
	long_data[3] = 0x00;
	long_data[4] = bl;
	long_data[5] = 0x00;
	long_data[6] = 0x00;
	long_data[7] = 0x00;
	
	for(i = 0; i < MIPI_TOTAL_PORT; i++){
		long_dcs.mipi_port = dcs_port_seq[i];
		long_dcs.data_type = DATALONG_GEN_WRITE;
		long_dcs.word_count = 5;
		long_dcs.pData = &long_data[0];
		mipi_packet_send_long(&long_dcs);
	}
	
	delay_ms(100);

	long_data[0] = 0xEF;
	long_data[1] = 0x90;
	long_data[2] = 0xB0;
	long_data[3] = 0x00;
	for(i = 0; i < MIPI_TOTAL_PORT; i++){
		long_dcs.mipi_port = dcs_port_seq[i];
		long_dcs.data_type = DATALONG_GEN_WRITE;
		long_dcs.word_count = 3;
		long_dcs.pData = &long_data[0];
		mipi_packet_send_long(&long_dcs);
	}
	delay_ms(40);
}

//-----------------------------------------------------------------------------
void mipi_read_test(void){
	// ADAM -- DISABLE???
    //return;		
	
	#ifdef DEBUG_LEVEL_2
		TRACE0("mipi_read_test(void)\n");
	#endif		
	
	uint8_t	i;
	
	PacketShort_t	dcs;
	uint8_t	cmd, param1, param2;
	uint8_t	read_buff[4];
	
	for(i=0;i<PANEL_COUNT;i++){
		dcs.mipi_port	= dcs_port_seq[i];
		dcs.data_type	= cmd;
		dcs.param1		= param1;
		dcs.param2		= param2;
		dcs.pData		= &read_buff[0];
		mipi_packet_send_short(&dcs);
	}
}

//-----------------------------------------------------------------------------
/// @copydoc mipi_panel_onoff
static char DCS_display_control(uint8_t backlight){

	#ifdef DEBUG_LEVEL_2
		TRACE1("mipi_panel_onoff(uint8_T backlight=%d)\n", backlight);
	#endif		

	PacketShort_t	short_dcs;

	if(PANEL_DISPLAY_OFF == backlight){
		
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "Panel 3v3 OFF");
		#endif
		
		// Turn OFF backlight
		// CHICAGO_PANEL_V33_DOWN();
		// HDK_LCD_BL_OFF();
		
		// Display data transfer
		// Display Off
		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port		= dcs_port_seq[i];
			short_dcs.data_type		= DATASHORT_DCS_WRITE_0;
			short_dcs.param1		= DCS_set_display_off;
			short_dcs.param2		= 0;
			short_dcs.pData			= NULL;
			mipi_packet_send_short(&short_dcs);
		}
		
		// Wait, >= 1 frame (60Hz = 16.666ms)
		delay_ms(PANEL_DISPLAY_OFF_DELAY);
	}
	
	else{
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "Panel ON");
		#endif		
		
		// Display data transfer
		// Display On
		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port		= dcs_port_seq[i];
			short_dcs.data_type		= DATASHORT_DCS_WRITE_1;
			short_dcs.param1		= 0xFF;
			short_dcs.param2		= 0x10;
			short_dcs.pData			= NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port		= dcs_port_seq[i];
			short_dcs.data_type		= DATASHORT_DCS_WRITE_0;
			short_dcs.param1		= DCS_set_display_on;
			short_dcs.param2		= 0;
			short_dcs.pData			= NULL;
			mipi_packet_send_short(&short_dcs);
		}
		
		// Wait, >= 40ms
		delay_ms(PANEL_DISPLAY_ON_DELAY);
		
		// turn right for BOE panel
		#ifdef BOE_VS055QHM
			for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
				short_dcs.mipi_port		= dcs_port_seq[i];
				short_dcs.data_type		= DATASHORT_DCS_WRITE_1;
				short_dcs.param1		= DCS_set_address_mode;
				short_dcs.param2		= 0x03;
				short_dcs.pData			= NULL;
				mipi_packet_send_short(&short_dcs);
			}
		#endif
		
		// Wait, >= 40ms
		delay_ms(PANEL_DISPLAY_ON_DELAY);
		
		// Turn ON backlight
		// CHICAGO_PANEL_V33_ON();
		// HDK_LCD_BL_ON();
	}
	
	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
/// @copydoc mipi_power_off_sequence
static char mipi_power_off_sequence(void){
	// PIN control for Sharp LS029B3SX02
	// DCS for Sharp LS029B3SX02
	
	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
/// @copydoc mipi_mcu_write_done
static void mipi_mcu_write_done(void){
	
	#ifdef DEBUG_LEVEL_2
		TRACE0("mipi_mcu_write_done(void)\n");
	#endif
	
	uint8_t reg_temp;
	i2c_read_byte(SLAVEID_SPI, MISC_NOTIFY_OCM0, &reg_temp);
	
	reg_temp |= MCU_LOAD_DONE;
	i2c_write_byte(SLAVEID_SPI, MISC_NOTIFY_OCM0, reg_temp);

	//power is controlled by software
	i2c_write_byte(SLAVEID_DP_TOP, ADDR_PWD_SEL, 0xff);  

	//i2c_read_byte(SLAVEID_SPI, R_SPI_REG_95, &reg_temp);
	//reg_temp |= 0x02;
	//i2c_write_byte(SLAVEID_SPI, R_SPI_REG_95, reg_temp);
}

//-----------------------------------------------------------------------------
/// @copydoc get_pixel_clock
static uint32_t get_pixel_clock(void){
	
	#ifdef DEBUG_LEVEL_2
		TRACE0("get_pixel_clock(void)\n");
	#endif	
	
	uint32_t pixel_clk_full;
	uint32_t h_active, hfp, hsync, hbp;
	uint32_t v_active, vfp, vsync, vbp;
	uint8_t temp_char;
	
	temp_char = MIPI_VIDEO_MODE;

	if(VIDEOMODE_STACKED!=temp_char){
		h_active	= (PANEL_H_ACTIVE*PANEL_COUNT);
		hfp			= (PANEL_HFP*PANEL_COUNT);
		hsync		= (PANEL_HSYNC*PANEL_COUNT);
		hbp			= (PANEL_HBP*PANEL_COUNT);;
		v_active	= PANEL_V_ACTIVE;
		vfp			= PANEL_VFP;
		vsync		= PANEL_VSYNC;
		vbp			= PANEL_VBP;
	}
	
	else{
		h_active	= PANEL_H_ACTIVE;
		hfp			= PANEL_HFP;
		hsync		= PANEL_HSYNC;
		hbp			= PANEL_HBP;;
		v_active	= (PANEL_V_ACTIVE*PANEL_COUNT);
		vfp			= (PANEL_VFP*PANEL_COUNT);
		vsync		= (PANEL_VSYNC*PANEL_COUNT);
		vbp			= (PANEL_VBP*PANEL_COUNT);
	}

	temp_char = MIPI_DSC_STATUS;
	
	if(DSC_ONE_TO_THREE == temp_char){
		TRACE1("\t%s\n", "DSC pixel clock");
		h_active	= h_active*3;
		hfp			= hfp*3;
		hsync		= hsync*3;
		hbp			= hbp*3;;
	}		
	
	pixel_clk_full = ((uint32_t)(h_active+hfp+hsync+hbp))*((uint32_t)(v_active+vfp+vsync+vbp))*((uint32_t)PANEL_FRAME_RATE);
	return((uint32_t)(pixel_clk_full/(uint32_t)10000));
}

//-----------------------------------------------------------------------------
/// @copydoc mipi_edid_write_buffer
static char mipi_edid_write_buffer(uint32_t edid_serial_number){
	
	#ifdef DEBUG_LEVEL_2
		TRACE0("mipi_edid_write_buffer(void)\n");
	#endif
		
	uint32_t checksum;
	uint32_t count;

	checksum = 0;
	count = 0;
	
	#ifdef EDID_AUTO_CALCULATE
		uint32_t temp_int;
		uint32_t hactive,hbp,hfp,hsync;
	#endif

	#ifdef EDID_AUTO_CALCULATE
		temp_int = MIPI_DSC_STATUS;
	
		if(DSC_ONE_TO_THREE == temp_int){
			TRACE1("\t%s\n", "DSC EDID");
			hactive = PANEL_H_ACTIVE*3;
			hbp = PANEL_HBP*3;
			hsync = PANEL_HSYNC*3;
			hfp = PANEL_HFP*3;
		}else{
			hactive = PANEL_H_ACTIVE;
			hbp = PANEL_HBP;
			hsync = PANEL_HSYNC;
			hfp = PANEL_HFP;
		}		
	
		i2c_write_byte(SLAVEID_EDIT_BUF, count, edid_buffer[0]);
	
		checksum = edid_buffer[0];
	
		// Calculate the Vendor Plug-N-Play ID bytes
		//(7, 6, 5, 4, 3, 2, 1, 0)
		//(0, A, A, A, A, a, B, B)
		//(B, B, b, C, C, C, C, c)
	
		// 65 - 64 = 1, is A
		// 90 - 64 = 26, is Z
		uint8_t pip_id_byte_a = ((EDID_PNP_NAME[0] - 64) << 2) | (((EDID_PNP_NAME[1] - 64) & 0b00011000) >> 3);
		uint8_t pip_id_byte_b = (((EDID_PNP_NAME[1] - 64) & 0b00000111) << 5) | (EDID_PNP_NAME[2] & 0b00011111);
	
		edid_buffer[EDID_MANUFACTURER_ID_L] = pip_id_byte_a;
		edid_buffer[EDID_MANUFACTURER_ID_H] = pip_id_byte_b;
		
		// Calculate product ID
		edid_buffer[EDID_PRODUCT_ID_L]			= (EDID_PRODUCT_ID  & 0x000000FF);
		edid_buffer[EDID_PRODUCT_ID_H]			= ((EDID_PRODUCT_ID & 0x0000FF00) >> 8);		
	
		// Calculate product serial number for EDID
		edid_buffer[EDID_SERIAL_NUM_L]			= (edid_serial_number & 0x000000FF);
		edid_buffer[EDID_SERIAL_NUM_L + 1]		= ((edid_serial_number & 0x0000FF00) >> 8);
		edid_buffer[EDID_SERIAL_NUM_H]			= ((edid_serial_number & 0x00FF0000) >> 16);
		edid_buffer[EDID_SERIAL_NUM_H + 1]		= ((edid_serial_number & 0xFF000000) >> 24);	
	
		// Calculate manufacturing week, year
		const char build_year[4] = {
			(__DATE__[7]), (__DATE__[8]), (__DATE__[9]), (__DATE__[10])
		};

		const char build_day_str[2] = {
			(__DATE__[ 4]), (__DATE__[ 5])
		};
	
		const char build_month_str[3] = {
			(__DATE__[ 0]), (__DATE__[1]), (__DATE__[2])
		};
	
		uint16_t build_day;
		uint8_t build_month;
	
		if(strstr((const char *)build_month_str, "Jan") != NULL){
			build_month = 1;
			build_day = atoi(build_day_str);
		}
		else if(strstr((const char *)build_month_str, "Feb") != NULL){
			build_month = 2;
			build_day = 31 + atoi(build_day_str);
		}
		else if(strstr((const char *)build_month_str, "Mar") != NULL){
			build_month = 3;
			build_day = 31 + 28 + atoi(build_day_str);
		}
		else if(strstr((const char *)build_month_str, "Apr") != NULL){
			build_month = 4;
			build_day = 31 + 28 + 31 + atoi(build_day_str);		
		}
		else if(strstr((const char *)build_month_str, "May") != NULL){
			build_month = 5;
			build_day = 31 + 28 + 31 + 30 + atoi(build_day_str);		
		}
		else if(strstr((const char *)build_month_str, "Jun") != NULL){
			build_month = 6;
			build_day = 31 + 28 + 31 + 30 + 31 + atoi(build_day_str);		
		}
		else if(strstr((const char *)build_month_str, "Jul") != NULL){
			build_month = 7;
			build_day = 31 + 28 + 31 + 30 + 31 + 30 + atoi(build_day_str);		
		}
		else if(strstr((const char *)build_month_str, "Aug") != NULL){
			build_month = 8;
			build_day = 31 + 28 + 31 + 30 + 31 + 30 + 31 + atoi(build_day_str);		
		}
		else if(strstr((const char *)build_month_str, "Sep") != NULL){
			build_month = 9;
			build_day = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + atoi(build_day_str);		
		}	
		else if(strstr((const char *)build_month_str, "Oct") != NULL){
			build_month = 10;
			build_day = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + atoi(build_day_str);		
		}
		else if(strstr((const char *)build_month_str, "Nov") != NULL){
			build_month = 11;
			build_day = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + atoi(build_day_str);		
		}	
		else if(strstr((const char *)build_month_str, "Dec") != NULL){
			build_month = 12;
			build_day = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + atoi(build_day_str);		
		}
	
		uint8_t approx_build_week	= build_day / 7;
		uint16_t mfg_year			= atoi(build_year);
	
		edid_buffer[EDID_WEEK] = approx_build_week;
		edid_buffer[EDID_YEAR] = mfg_year - 1990;
	
		// Set the screen size
		edid_buffer[21] = (EDID_SCREEN_SIZE_HORIZONTAL_CM);
		edid_buffer[22] = (EDID_SCREEN_SIZE_VERTICAL_CM);
		
		// Set the screen transfer function (gamma)
		edid_buffer[23] = (EDID_SCREEN_GAMMA * 100) - 100;
	
		// Write the first EDID data block
		for(count=1; count<EDID_DB1_BASE; count++){
			i2c_write_byte_keep(count, edid_buffer[count]);
			checksum = (checksum + edid_buffer[count]);
		}
	
		temp_int = get_pixel_clock();
		#ifdef DEBUG_LEVEL_2
			TRACE1("\tPixel_clk = %d\n", (temp_int/100));
		#endif

		// Pixel clock
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_PIXEL_CLK_L, (uint8_t)(temp_int&0x00FF));
		checksum = (checksum + (uint8_t)(temp_int&0x00FF));
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_PIXEL_CLK_H, (uint8_t)((temp_int>>8)&0x00FF));
		checksum = (checksum + (uint8_t)((temp_int>>8)&0x00FF)); 

		// H active low bit
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_HACTIVE_L, (uint8_t)((hactive*PANEL_COUNT)&0x00FF));
		checksum = (checksum + (uint8_t)((hactive*PANEL_COUNT)&0x00FF));

		// H blank (HBP+Hsync+HFP) low bit
		temp_int = (hbp+hfp+hsync)*PANEL_COUNT;
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_HBP_L, (uint8_t)(temp_int&0x00FF));
		checksum = (checksum + (uint8_t)(temp_int&0x00FF));

		// H active and HBP high bit
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_HACT_HBP_H, (uint8_t)((((hactive*PANEL_COUNT)>>4)&0x00F0)|((temp_int>>8)&0x000F)));
		checksum = (checksum + (uint8_t)((((hactive*PANEL_COUNT)>>4)&0x00F0)|((temp_int>>8)&0x000F)));

		// V active low bit
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_VACTIVE_L, (uint8_t)(PANEL_V_ACTIVE&0x00FF));
		checksum = (checksum + (uint8_t)(PANEL_V_ACTIVE&0x00FF)); 

		// V blank (VBP+Vsync+VFP) low bit
		temp_int = PANEL_VBP+PANEL_VFP+PANEL_VSYNC;
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_VBP_L, (uint8_t)(temp_int&0x00FF));
		checksum = (checksum + (temp_int&0x00FF)); 

		// V active and VBP high bit
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_VACT_VBP_H, (uint8_t)(((PANEL_V_ACTIVE>>4)&0x00F0)|((temp_int>>8)&0x000F)));
		checksum = (checksum + (uint8_t)(((PANEL_V_ACTIVE>>4)&0x00F0)|((temp_int>>8)&0x000F)));

		// HFP low bit
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_HFP_L, (uint8_t)((hfp*PANEL_COUNT)&0x00FF));
		checksum = (checksum + (uint8_t)((hfp*PANEL_COUNT)&0x00FF));

		// HSYNC low bit
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_HSYNC_L, (uint8_t)((hsync*PANEL_COUNT)&0x00FF));
		checksum = (checksum + (uint8_t)((hsync*PANEL_COUNT)&0x00FF));

		// VFP and VSYNC low bit
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_VFP_VSYNC_L, (uint8_t)(((PANEL_VFP<<4)&0x00F0)|(PANEL_VSYNC&0x000F)));
		checksum = (checksum + (uint8_t)(((PANEL_VFP<<4)&0x00F0)|(PANEL_VSYNC&0x000F)));

		// HFP, HSYNC, VFP, VSYNC high bit
		i2c_write_byte_keep(EDID_DB1_BASE+EDID_HFP_HSYNC_VFP_VSYNC_H, \
			(uint8_t)((((hfp*PANEL_COUNT)>>2)&0x00C0)|(((hsync*PANEL_COUNT)>>4)&0x0030)|((PANEL_VFP>>2)&0x00C0)|((PANEL_VSYNC>>4)&0x0003)));
		checksum = (checksum + (uint8_t)((((hfp*PANEL_COUNT)>>2)&0x00C0)|(((hsync*PANEL_COUNT)>>4)&0x0030)|((PANEL_VFP>>2)&0x00C0)|((PANEL_VSYNC>>4)&0x0003))); 

		count = EDID_DB1_BASE+EDID_H_DISPLAY_SIZE;
	
		// Finish writing DB1
		for(; count < EDID_DB2_BASE; count++){
			i2c_write_byte_keep(count, edid_buffer[count]);
			checksum = (checksum + edid_buffer[count]);
		}	
	
		// Fill DB2 with zeros
		for(uint8_t i = 72; i < 90; i++){
			edid_buffer[i] = 0;			
		}
		
	
		// Calculate display name
		if(strlen(EDID_DISPLAY_NAME) > 12){
			assert(false);
		}
				
		uint8_t i = 0;
				
		for(; i < strlen(EDID_DISPLAY_NAME); i++){
			edid_buffer[EDID_DB4_BASE + 0x05 + i] = EDID_DISPLAY_NAME[i];
		}
				
		edid_buffer[EDID_DB4_BASE + 0x05 + i] = 0x0A;
				
		for (; i < 13 ; i++){
			edid_buffer[EDID_DB4_BASE + 0x05 + i] = 0x20;
		}
		
		
		// Write DB2
		count = EDID_DB2_BASE;

		for(; count < (EDID_DB3_BASE); count++){
			i2c_write_byte_keep(count, edid_buffer[count]);
			checksum = (checksum + edid_buffer[count]);
		}

		// Write DB3
		count = EDID_DB3_BASE;

		for(; count < (EDID_DB4_BASE); count++){
			i2c_write_byte_keep(count, edid_buffer[count]);
			checksum = (checksum + edid_buffer[count]);
		}		
	
		// Write DB4
		count = EDID_DB4_BASE;
	
		for(; count<(EDID_DB4_BASE + EDID_DB_SIZE); count++){
			i2c_write_byte_keep(count, edid_buffer[count]);
			checksum = (checksum + edid_buffer[count]);
		}
	
		// Set last byte before checksum
		i2c_write_byte_keep(count, edid_buffer[count]);
		checksum = (checksum + edid_buffer[count]);
		count++;
	
	#else
		i2c_write_byte(SLAVEID_EDIT_BUF, count, edid_buffer[0]);
		checksum = edid_buffer[0];
		
		for(count=1; count<(EDID_LENGTH-1); count++){
			i2c_write_byte_keep(count, edid_buffer[count]);
			checksum = (checksum + edid_buffer[count]);
		}
	#endif
	
	checksum = ((0xff - (uint8_t)(checksum & 0x00FF)) + 1);

	i2c_write_byte_keep(count, (uint8_t)(checksum & 0x00FF));
	
	#ifdef DEBUG_LEVEL_2
		TRACE1("\tDone, checksum = 0x%02X\n", (uint8_t)(checksum & 0x00FF));
	 #endif
	
	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
/// @copydoc mipi_edid_write_external_buffer
static char mipi_edid_write_external_buffer(void){
	
	#ifdef DEBUG_LEVEL_2
		TRACE0("mipi_edid_write_external_buffer(void)\n");
	#endif
		
	uint32_t checksum;
	uint32_t count;

	checksum	= 0;
	count		= 0;
	
	// Enable or disable audio sink support
	if(EDID_HAS_AUDIO_SINK != 1){
		edid_external_buffer[131 - 128] = 0x80;
	
		for(uint8_t i = (160 - 128); i < (173 - 128); i++){
			edid_external_buffer[i] = 0;
		}
	}
	
	// Write first extended EDID block
	i2c_write_byte(SLAVEID_EDIT_BUF, EDID_EXTERNAL_BUF, edid_external_buffer[0]);
	
	// Calculate checksum
	checksum = edid_external_buffer[0];
	
	for(count=1; count<(EDID_EXTERNAL_LENGTH-1); count++){
		i2c_write_byte_keep(EDID_EXTERNAL_BUF+count, edid_external_buffer[count]);
		checksum = (checksum + edid_external_buffer[count]);
	}

	checksum = ((0xff-(uint8_t)(checksum&0x00FF))+1);

	i2c_write_byte_keep(EDID_EXTERNAL_BUF+count, (uint8_t)(checksum&0x00FF));

	#ifdef DEBUG_LEVEL_2
		TRACE1("\tDone, checksum = 0x%02X \n", ((uint8_t)(checksum&0x00FF)));
	#endif	

	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
/// @copydoc chicago_get_edid_resolution
static void chicago_get_edid_resolution(void){

	#ifdef DEBUG_LEVEL_2
		TRACE0("chicago_get_edid_resolution(void)\n");
	#endif		
	
	uint8_t	i, reg_temp;
	uint32_t	reg_int;

	edid_db_count = 0;

	for(i = 0; i < EDID_DB_MAX; i++){
		// read pixel clock
		i2c_read_byte(SLAVEID_EDIT_BUF, (EDID_DB1_BASE+(EDID_DB_SIZE*i)), &reg_temp);
		reg_int = (uint32_t)reg_temp;
		
		i2c_read_byte(SLAVEID_EDIT_BUF, (EDID_DB1_BASE+(EDID_DB_SIZE*i))+1, &reg_temp);
		reg_int |= (0xFF00 & ((uint32_t) reg_temp << 8));
		
		if(reg_int != 0){
			
			// read H active
			i2c_read_byte(SLAVEID_EDIT_BUF, (EDID_DB1_BASE+(EDID_DB_SIZE*i))+EDID_HACTIVE_L, &reg_temp);
			reg_int = (uint32_t) reg_temp;
			i2c_read_byte(SLAVEID_EDIT_BUF, (EDID_DB1_BASE+(EDID_DB_SIZE*i))+EDID_HACT_HBP_H, &reg_temp);
			reg_int |= (0x0F00 & ((uint32_t) reg_temp << 4));
			edid_resolution[edid_db_count][0] = reg_int;
			
			// read V active
			i2c_read_byte(SLAVEID_EDIT_BUF, (EDID_DB1_BASE+(EDID_DB_SIZE*i))+EDID_VACTIVE_L, &reg_temp);
			reg_int = (uint32_t) reg_temp;
			i2c_read_byte(SLAVEID_EDIT_BUF, (EDID_DB1_BASE+(EDID_DB_SIZE*i))+EDID_VACT_VBP_H, &reg_temp);
			reg_int |= (0x0F00 & ((uint32_t) reg_temp << 4));
 			edid_resolution[edid_db_count][1] = reg_int;

			edid_db_count++;
		}
	}

	#ifdef DEBUG_LEVEL_2
		TRACE1("\tEDID DB = %d\n", edid_db_count);
	
		for(i = 0; i < edid_db_count; i++){
			TRACE2("\tH = %d, V = %d\n", edid_resolution[i][0], edid_resolution[i][1]);
		}
	#endif
}

//-----------------------------------------------------------------------------
/// @copydoc chicago_check_dp_edid_resolution
static char chicago_check_dp_edid_resolution(void){
	
	#ifdef DEBUG_LEVEL_2
		TRACE0("chicago_check_dp_edid_resolution(void)\n");
	#endif
	
	uint8_t	reg_temp;
	uint32_t	dp_h_active, dp_v_active;
	uint8_t	i;

	// Read H active from DP
	i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_HWIDTH7_0_DBG, &reg_temp);
	dp_h_active = (uint32_t)reg_temp;
	
	i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_HWIDTH15_8_DBG, &reg_temp);
	dp_h_active |= (0x0F00 &((uint32_t)reg_temp << 8));

	// read V active from DP
	i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_VHEIGHT7_0_DBG, &reg_temp);
	dp_v_active = (uint32_t)reg_temp;
	
	i2c_read_byte(SLAVEID_MAIN_LINK, ADDR_VHEIGHT15_8_DBG, &reg_temp);
	dp_v_active |= (0x0F00 &((uint32_t)reg_temp << 8));

	#ifdef DEBUG_LEVEL_2
		TRACE2("\tDisplayPort reports H:%d, V:%d\n", dp_h_active, dp_v_active);
	#endif

	// compare resolution
	for(i=0; i < edid_db_count; i++){
		if((dp_h_active == edid_resolution[i][0]) && (dp_v_active == edid_resolution[i][1])){
			return 1;
		}
	}

	#ifdef DEBUG_LEVEL_2
		TRACE2("\tResolution DOES NOT MATCH H:%s, V:%s !\n", edid_resolution[i][0], edid_resolution[i][1]);
	#endif 
	
	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
/// @copydoc chicago_check_video_stable
static uint8_t chicago_check_video_stable(void){
	
	#ifdef DEBUG_LEVEL_3
		TRACE0("chicago_check_video_stable(void)\n");
	#endif
	
	if(VIDEOSTABLE_ALL_CONDITION == video_stable){
		return FLAG_VALUE_ON;
	}
	
	else{
		return FLAG_VALUE_OFF;
	}
}


//-----------------------------------------------------------------------------
/// @copydoc HDK_chicago_actually_check_video_stable
static uint8_t HDK_chicago_actually_check_video_stable(void){
	
	#ifdef DEBUG_LEVEL_3
		TRACE0("HDK_chicago_actually_check_video_stable(void)\n");
	#endif
	
	uint8_t reg_temp;

	// Check Video format stable status
	i2c_read_byte(SLAVEID_VIDEO, ADDR_VID_STABLE_DET, &reg_temp);

	if((reg_temp & VID_STRM_STABLE_STATUS) != 0){
		return true;
	}
	
	return false;
}



//-----------------------------------------------------------------------------
/// @copydoc HDK_chicago_actually_check_video_stable
void HDK_chicago_set_vsync_out(void){
	
	CHICAGO_CHIP_POWER_UP();
	CHICAGO_RESET_UP();
	
	delay(10);
	
	// Set R_SYS_CTRL_1 to set M_SDA and M_SCL to GPIO
	uint8_t reg_temp;
	i2c_read_byte(SLAVEID_SPI, R_SYS_CTRL_1, &reg_temp);
	i2c_write_byte(SLAVEID_SPI, R_SYS_CTRL_1, 0b11000000 | reg_temp);
	
	// Set R_PPS_REG_110 (0xEE)
	i2c_write_byte(SLAVEID_SPI, 0xEE, 0b00001001); // Check
	
	// Set GPIO_MAP_0 (0x41)
	i2c_write_byte(SLAVEID_SPI, 0x41, 120); // Check
	
	delay(10);
}



//-----------------------------------------------------------------------------
/// @copydoc chicago_clear_video_stable
static void chicago_clear_video_stable(void){
	// Clean all conditions
    video_stable = 0;
}

//-----------------------------------------------------------------------------
void external_int_isr(void) {	
	EXT_INTR_DISABLE(); // Disable /INT0 interrupts first
	EXT_INTR_EVENT_SET();

	#ifdef DEBUG_INTERRUPTS
		chicago_read_intr_state();
	#endif

	if(chicago_get_current_state() == STATE_NONE){
		// Error state
		EXT_INTR_EVENT_CLEAR();
		EXT_INTR_ENABLE(); // Enable /INT0 interrupts
		return;
	}
	
	// DP cable plug-in
	if(chicago_get_current_state() == STATE_WAITCABLE){
		dp_cable = DP_CABLE_IN;
		return;
	}
}

//-----------------------------------------------------------------------------
void chicago_read_intr_state(void){
	
	#ifdef DEBUG_LEVEL_2
		TRACE0("chicago_read_intr_state(void)\n");
	#endif
	
	uint8_t reg_temp;

	i2c_read_byte(SLAVEID_DP_TOP,ADDR_INTR,&reg_temp);
	TRACE1("\tADDR_INTR = %02X\n", reg_temp);

	i2c_read_byte(SLAVEID_MIPI_CTRL,R_MIP_TX_INT,&reg_temp);
	TRACE1("\tR_MIP_TX_INT = %02X\n", reg_temp);

	i2c_read_byte(SLAVEID_MAIN_LINK,ADDR_MAIN_LINK_INTR0,&reg_temp);
	TRACE1("\tADDR_MAIN_LINK_INTR0 = %02X\n", reg_temp);

	i2c_read_byte(SLAVEID_MAIN_LINK,ADDR_MAIN_LINK_INTR1,&reg_temp);
	TRACE1("\tADDR_MAIN_LINK_INTR1 = %02X\n", reg_temp);

	i2c_read_byte(SLAVEID_MAIN_LINK,ADDR_MAIN_LINK_INTR2,&reg_temp);
	TRACE1("\tADDR_MAIN_LINK_INTR2 = %02X\n", reg_temp);

	i2c_read_byte(SLAVEID_MAIN_LINK,ADDR_MAIN_LINK_STATUS_0,&reg_temp);
	TRACE1("\tADDR_MAIN_LINK_STATUS_0 = %02X\n", reg_temp);

	i2c_read_byte(SLAVEID_DP_IP,ADDR_SYSTEM_STATUS_1,&reg_temp);
	TRACE1("\tADDR_SYSTEM_STATUS_1 = %02X\n", reg_temp);

	i2c_read_byte(SLAVEID_DP_IP,ADDR_AUX_CH_STATUS,&reg_temp);
	TRACE1("\tADDR_AUX_CH_STATUS = %02X\n", reg_temp);

	i2c_read_byte(SLAVEID_DP_IP,ADDR_DPIP_INTR,&reg_temp);
	TRACE1("\tADDR_DPIP_INTR = %02X\n", reg_temp);

	i2c_read_byte(SLAVEID_AUDIO,ADDR_AUD_INTR,&reg_temp);
	TRACE1("\tADDR_AUD_INTR = %02X\n", reg_temp);

	i2c_read_byte(SLAVEID_VIDEO,ADDR_VID_INT,&reg_temp);
	TRACE1("\tADDR_VID_INT = %02X\n", reg_temp);

	i2c_read_byte(SLAVEID_PLL,ADDR_PLL_INTR,&reg_temp);
	TRACE1("\tADDR_PLL_INTR = %02X\n", reg_temp);
}

//-----------------------------------------------------------------------------
void HDK_chicago_clear_intr_state(uint8_t readOut){
	
	#ifdef DEBUG_LEVEL_2
		TRACE0("HDK_chicago_clear_intr_state(void)\n");
	#endif

	i2c_write_byte(SLAVEID_DP_TOP,		ADDR_INTR,					ADDR_INTR_MASK            | 0x0000);
	i2c_write_byte(SLAVEID_MIPI_CTRL,	R_MIP_TX_INT,				R_MIP_TX_INT_MASK         | 0x0000);
	i2c_write_byte(SLAVEID_MAIN_LINK,	ADDR_MAIN_LINK_INTR0,		ADDR_MAIN_LINK_INTR0_MASK | 0x0000);
	i2c_write_byte(SLAVEID_MAIN_LINK,	ADDR_MAIN_LINK_INTR1,		ADDR_MAIN_LINK_INTR1_MASK | 0x0000);
	i2c_write_byte(SLAVEID_MAIN_LINK,	ADDR_MAIN_LINK_INTR2,		ADDR_MAIN_LINK_INTR2_MASK | 0x0000);
	i2c_write_byte(SLAVEID_MAIN_LINK,	ADDR_MAIN_LINK_STATUS_0,    0x0000);
	i2c_write_byte(SLAVEID_DP_IP,		ADDR_SYSTEM_STATUS_1,		0x0000);
	i2c_write_byte(SLAVEID_DP_IP,		ADDR_AUX_CH_STATUS,			0x0000);
	i2c_write_byte(SLAVEID_DP_IP,		ADDR_DPIP_INTR,				ADDR_DPIP_INTR_MASK | 0x0000);
	i2c_write_byte(SLAVEID_AUDIO,		ADDR_AUD_INTR,				ADDR_DPIP_INTR_MASK | 0x0000);
	i2c_write_byte(SLAVEID_VIDEO,		ADDR_VID_INT,				ADDR_VID_INT_MASK   | 0x0000);
	i2c_write_byte(SLAVEID_PLL,			ADDR_PLL_INTR,				ADDR_PLL_INTR_MASK  | 0x0000);

	if(readOut) chicago_read_intr_state();
}


//-----------------------------------------------------------------------------
/// @copydoc clear_software_int
static void clear_software_int(void){	
	
	#ifdef DEBUG_LEVEL_3
		TRACE0("clear_software_int(void)\n");
	#endif	
	
	uint8_t reg_temp;

	i2c_read_byte(SLAVEID_DP_TOP, ADDR_SW_INTR_CTRL, &reg_temp);

	reg_temp &= ~SOFT_INTR;

	i2c_write_byte(SLAVEID_DP_TOP, ADDR_SW_INTR_CTRL, reg_temp);
}

//-----------------------------------------------------------------------------
void chicago_interrupt_handle(void){
	
	#ifdef DEBUG_LEVEL_3
		TRACE0("chicago_interrupt_handle(void)\n");
	#endif
	
	uint8_t reg_temp;

	#ifdef DEBUG_POWER_OFF_CHICAGO_WHEN_CABLE_OUT
		// Check AUX status

		i2c_read_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, &reg_temp);

		if(0 != (reg_temp & AUX_CABLE_OUT)){

			#ifdef DEBUG_LEVEL_2
				TRACE2("\t%s\n\t%s\n", "cable removed!", "Bridge POWERING OFF");
			#endif
		
			// bit[5] = 1, DP cable disconnect
			chicago_state_change(STATE_NONE);
		
			i2c_write_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, 0x00);

			chicago_clear_video_stable();
		}
	#else
		// Check AUX status
		i2c_read_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, &reg_temp);
	
		if(0 != (reg_temp & AUX_CABLE_OUT)){
	
			#ifdef DEBUG_LEVEL_2
				TRACE2("\t%s\n\t%s\n", "cable removed!", "Bridge STAYING ON");
			#endif
		
			// Turn off backlight
			DCS_display_control(PANEL_DISPLAY_OFF);
	
			// clear AUX_CABLE_OUT
			reg_temp &= ~AUX_CABLE_OUT;
			i2c_write_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, reg_temp);
	
			chicago_clear_video_stable();
		}
	#endif
	
	// Check cable in
	i2c_read_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, &reg_temp);
	
	if((reg_temp & AUX_CABLE_IN) != 0){
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "cable inserted!");
		#endif

		// clear AUX_CABLE_IN
		reg_temp &= ~AUX_CABLE_IN;
		i2c_write_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, reg_temp);
	}

	// Check VIDEO M/N RE-CACULATE
	i2c_read_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, &reg_temp);
	
	if((reg_temp & VIDEO_RE_CACULATE) != 0){ 
			
		#ifdef DEBUG_LEVEL_3
			TRACE1("\t%s\n","M/N");
		#endif 
			
		// clear VIDEO_UNSTABLE
		reg_temp &= ~VIDEO_RE_CACULATE;
		i2c_write_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, reg_temp);
		
		chicago_clear_video_stable();
		
		// handle panel off first.
		return;
	}

	// Check Video input missing
	i2c_read_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, &reg_temp);
	
	if((reg_temp & VIDEO_INPUT_EMPTY) != 0){
		
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n","Empty video stream");
		#endif
		
		// clear VIDEO_INPUT_EMPTY
		reg_temp &= ~VIDEO_INPUT_EMPTY;
		i2c_write_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, reg_temp);
		video_stable &= ~VIDEOSTABLE_DP_SOURCE_INPUT;
		
		// handle video missing first.
		return;
	}

	// Check Video format stable status
	i2c_read_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, &reg_temp);
	
	if((reg_temp & VIDEO_STABLE) != 0){
		
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "Video stream stable");
		#endif
			
		// clear VIDEO_STABLE
		reg_temp &= ~VIDEO_STABLE;
		
		i2c_write_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, reg_temp);
		
		video_stable |= (VIDEOSTABLE_VIDEO_STREAM|VIDEOSTABLE_DP_SOURCE_INPUT);
		
		#ifdef DEBUG_CHECK_DP_RESOLUTION
			if(chicago_check_dp_edid_resolution() == 1){
			
				#ifdef DEBUG_LEVEL_2
					TRACE1("\t%s\n", "Measured resolution matches EDID");
				#endif 
			
				video_stable |= VIDEOSTABLE_DP_RESOLUTION_MATCH;
			}
		#endif
		
		return;
	}

	// Check Audio sample rate changed
	i2c_read_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, &reg_temp);
	
	if((reg_temp & AUDIO_MN_RST) != 0){
	
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "Audio MN reset");
		#endif
		
		// clear AUDIO_MN_RST
		reg_temp &= ~AUDIO_MN_RST;
		i2c_write_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, reg_temp);
		
		// Do nothing
	}	

	// Check Audio PLL RST
	i2c_read_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, &reg_temp);
	if((reg_temp & AUDIO_PLL_RST) != 0){
	
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "Audio PLL reset");
		#endif 
			
		// clear AUDIO_PLL_RST
		reg_temp &= ~AUDIO_PLL_RST;
		i2c_write_byte(SLAVEID_SPI, INT_NOTIFY_MCU0, reg_temp);
		
		// Do nothing
	}	
	
	// Clear interrupt
	clear_software_int();
	
	//interrupts();
	EXT_INTR_EVENT_CLEAR();
	EXT_INTR_ENABLE();
}

//-----------------------------------------------------------------------------
/// @copydoc chicago_interrupt_set
static char chicago_interrupt_set(void){
	
	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
/// @copydoc chicago_read_chip_id
static void chicago_read_chip_id(void){

	#ifdef DEBUG_LEVEL_2
		TRACE0("chicago_read_chip_id(void)\n");
	#endif
	
	uint8_t reg_temp;
	uint32_t reg_int;
	
	i2c_read_byte(SLAVEID_SPI, CHIP_ID_HIGH, &reg_temp);
	reg_int = ((((uint32_t)reg_temp)<<8)&0xFF00);
	
	i2c_read_byte(SLAVEID_SPI, CHIP_ID_LOW, &reg_temp);
	reg_int |= (((uint32_t)reg_temp)&0x00FF);
	
	#ifdef DEBUG_LEVEL_2
		TRACE1("\tChip ID = %04X\n", reg_int);
	#endif
}

//-----------------------------------------------------------------------------
char chicago_power_supply(uint8_t onoff){

	#ifdef DEBUG_LEVEL_2	
		TRACE1("chicago_power_supply(uint8_t onoff=%d)\n", onoff);
	#endif
	
	if(onoff == 0) {
		CHICAGO_RESET_DOWN();
		CHICAGO_CHIP_POWER_DOWN();
		CHICAGO_V10D_DOWN();
		CHICAGO_V10A_DOWN();
		CHICAGO_V18D_DOWN();
		CHICAGO_V18A_DOWN();
	} 
	
	else {
		// Turn on 1.8v first
		CHICAGO_V18D_ON();
		CHICAGO_V18A_ON();
		// Wait >= 1ms
		delay_ms(CHICAGO_POWER_V18_DELAY);
		//
		// Possibility?
		//
		//
		// Turn on 1.0v
		CHICAGO_V10D_ON();
		CHICAGO_V10A_ON();
		// Wait >= 2ms
		delay_ms(CHICAGO_POWER_V10_DELAY);
	}
	
	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
char chicago_power_onoff(uint8_t onoff){
	
	#ifdef DEBUG_LEVEL_2
		TRACE1("chicago_power_onoff(uint8_t onoff=%d)\n", onoff);
	#endif
	
	if(onoff == 0) {
		CHICAGO_RESET_DOWN();
		
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "Chicago n_reset LOW");
		#endif	
		
		delay_ms(CHICAGO_RESET_DOWN_DELAY);
		
		CHICAGO_CHIP_POWER_DOWN();
		
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "Chicago pwen LOW");
		#endif		
		
		delay_ms(CHICAGO_CHIPPOWER_DOWN_DELAY);
	} 
	
	else {
		CHICAGO_CHIP_POWER_UP();
			
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "Chicago pwen HIGH");
		#endif	
			
		// delay >= 20ms, for clock stable
		delay_ms(CHICAGO_CHIPPOWER_UP_DELAY);
		
		CHICAGO_RESET_UP();

		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "Chicago n_reset HIGH");
		#endif
		
		// delay >= 10ms, ocm start to work
		delay_ms(CHICAGO_RESET_UP_DELAY);
		
		//chicago_read_chip_id();
	}
	
	return RETURN_NORMAL_VALUE;
}

//-----------------------------------------------------------------------------
uint8_t dp_check_cable_plug_int(void){
	
	#ifdef DEBUG_LEVEL_2
		TRACE0("dp_check_cable_plug_int(void)\n");
	#endif
	
	if(IS_CHICAGO_INT()){
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "DP cable IN");
		#endif
		return DP_CABLE_IN;
	}
	
	else{
		#ifdef DEBUG_LEVEL_2
			TRACE1("\t%s\n", "DP cable OUT");
		#endif
		return DP_CABLE_OUT;
	}
}

//-----------------------------------------------------------------------------
void chicago_state_change(ChicagoState state){
	
	#ifdef DEBUG_LEVEL_2
		TRACE1("chicago_state_change(ChicagoState state=%d)\n", state);
		
		TRACE1("\t%s", "State change to ");
		
		switch(state){
			case STATE_NONE:
				TRACE1("%s", "STATE_NONE");
				break;
			case STATE_POWEROFF:
				TRACE1("%s", "STATE_POWEROFF");
				break;
			case STATE_WAITCABLE:
				TRACE1("%s", "STATE_WAITCABLE");
				break;
			case STATE_CONNECTING:
				TRACE1("%s", "STATE_CONNECTING");
				break;
			case STATE_NORMAL:
				TRACE1("%s", "STATE_NORMAL");
				break;
			default:
				TRACE1("%s", "ERROR_STATE");
				break;			
		}
		
		TRACE1(", %s", "last_state is ");
		
		switch(state){
			case STATE_NONE:
			TRACE1("%s\n", "STATE_NONE");
			break;
			case STATE_POWEROFF:
			TRACE1("%s\n", "STATE_POWEROFF");
			break;
			case STATE_WAITCABLE:
			TRACE1("%s\n", "STATE_WAITCABLE");
			break;
			case STATE_CONNECTING:
			TRACE1("%s\n", "STATE_CONNECTING");
			break;
			case STATE_NORMAL:
			TRACE1("%s\n", "STATE_NORMAL");
			break;
			default:
			TRACE1("%s\n", "ERROR_STATE");
			break;
		}
	#endif
	
	current_state = state;
}

//-----------------------------------------------------------------------------
void chicago_last_state_change(ChicagoState state){
	last_state = state;
}

//-----------------------------------------------------------------------------
ChicagoState chicago_get_current_state(void){
	return current_state;
}

//-----------------------------------------------------------------------------
ChicagoState chicago_get_last_state(void){
	return last_state;
}

//-----------------------------------------------------------------------------
void chicago_stop_ocm(void){

	#ifdef DEBUG_LEVEL_2
		TRACE0("chicago_stop_ocm(void)\n");
	#endif
	
	uint8_t reg_temp;
	
	// stop main OCM
	i2c_read_byte(SLAVEID_SPI, OCM_DEBUG_CTRL, &reg_temp);

	reg_temp |= OCM_RESET;

	i2c_write_byte(SLAVEID_SPI, OCM_DEBUG_CTRL, reg_temp);
}

//-----------------------------------------------------------------------------
/// @copydoc chicago_ocm_crc_checking
static char chicago_ocm_crc_checking(void){
	
	#ifdef DEBUG_LEVEL_2
		TRACE0("chicago_ocm_crc_checking(void)\n");
	#endif
	
	uint8_t reg_temp;

	if(RETURN_NORMAL_VALUE == i2c_read_byte(SLAVEID_SPI, HDCP_LOAD_STATUS, &reg_temp)){
		if(OCM_FW_CRC32==(OCM_FW_CRC32&reg_temp)){
			return RETURN_NORMAL_VALUE;
		}
		return RETURN_FAILURE_VALUE;
	}
	return RETURN_FAILURE_VALUE2;
}

//-----------------------------------------------------------------------------
/// @copydoc chicago_set_display_delay_ms
static void chicago_set_display_delay_ms(void){
	uint32_t reg_int;

	reg_int = GRAPHIC_CARD_DRAW_DELAY;

	i2c_write_byte(SLAVEID_SPI, VIDEO_STABLE_DELAY_L, (uint8_t)(reg_int&0x00FF));
	i2c_write_byte(SLAVEID_SPI, VIDEO_STABLE_DELAY_H, (uint8_t)((reg_int>>8)&0x00FF));
}

	//-----------------------------------------------------------------------------
	/// @copydoc DSC_setting
	static void DSC_setting(void){	
		
		#ifdef DEBUG_LEVEL_2
			TRACE0("DSC_setting(void)\n");
		#endif		
		
        #if defined(SHARP_LS029B3SX01_ONE_120Hz)
		uint8_t reg_temp;
		uint32_t i;
		uint32_t reg_int;
	
	//	volatile uint32_t data_value,reg_temp_1=0;
	//	uint32_t trunk_size,slice_height,reg_temp_2=0;
	
		//H blanking after DSC 58 *3=174=0xae
		reg_int = ((PANEL_HFP + PANEL_HSYNC + PANEL_HBP)*3)*PANEL_COUNT;
		reg_int = ((reg_int*12)/10)+1;
		 
		TRACE1("\tDSC setting, blank = %d\n", reg_int);
		
		i2c_write_byte(SLAVEID_SPI, H_BLANK_L, (uint8_t)(reg_int&0x00FF));
		i2c_write_byte(SLAVEID_SPI, H_BLANK_H, (uint8_t)((reg_int>>8)&0x00FF));
	
		// Enable DSC function
		i2c_read_byte(SLAVEID_SPI, R_DSC_CTRL_0, &reg_temp);
		
		reg_temp |= DSC_EN; // bit[0] = 1;
		i2c_write_byte(SLAVEID_SPI, R_DSC_CTRL_0, reg_temp);

		//Config PPS table
		for(i = 0; i < PPS_LENGTH; i++) {
			i2c_write_byte(SLAVEID_PPS, PPS_REG_0+i, pps_table[i]);
		}
        
        #endif
	}

	//-----------------------------------------------------------------------------
	/// @copydoc DSC_PPS_enable
	static void DSC_PPS_enable_and_DCS_init(void){

		#ifdef DEBUG_LEVEL_2
			TRACE0("DSC_PPS_enable(void)\n");
		#endif

		PacketShort_t short_dcs;
		PacketLong_t long_dcs;
		
		#define DCS_en
		
        #ifdef DCS_en	
		uint8_t dsc_long1[5] = {
			0x3B, 0x00, 0x0A, 0x00, 0x0A
		};

		uint8_t dsc_long2[5] = {
			0xBE, 0x00, 0x0A, 0x00, 0x0A
		};

		uint8_t dsc_long3[5] = {
			0x2B, 0x00, 0x00, 0x06, 0x40
		};

		TRACE1("\t%s\n", "DSC enable in.");

		/* HSSRAM parameter */
		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xFF;
			short_dcs.param2 = 0xE0;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xFB;
			short_dcs.param2 = 0x01;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0x53;
			short_dcs.param2 = 0x22;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		/* Initial Code Release: CDM2 */
		/* Command Page */
		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xFF;
			short_dcs.param2 = 0x25;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xFB;
			short_dcs.param2 = 0x01;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0x65;
			short_dcs.param2 = 0x01;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0x66;
			short_dcs.param2 = 0x50;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0x67;
			short_dcs.param2 = 0x55;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xC4;
			short_dcs.param2 = 0x90;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		/* Command Page */
		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xFF;
			short_dcs.param2 = 0x26;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xFB;
			short_dcs.param2 = 0x01;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0x02;
			short_dcs.param2 = 0xB5;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0x4D;
			short_dcs.param2 = 0x8B;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		/* Initial Code Release: CDM2 */
		/* Command Page */
		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xFF;
			short_dcs.param2 = 0x10;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xFB;
			short_dcs.param2 = 0x01;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		/* User Command Set */
		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xC0;
			short_dcs.param2 = 0x80;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			long_dcs.mipi_port = dcs_port_seq[i];
			long_dcs.data_type = DATALONG_GEN_WRITE;
			long_dcs.word_count = 0x0005;
			long_dcs.pData = &dsc_long1[0];
			mipi_packet_send_long(&long_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			long_dcs.mipi_port = dcs_port_seq[i];
			long_dcs.data_type = DATALONG_GEN_WRITE;
			long_dcs.word_count = 0x0005;
			long_dcs.pData = &dsc_long2[0];
			mipi_packet_send_long(&long_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xBB;
			short_dcs.param2 = 0x13;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0xBA;
			short_dcs.param2 = 0x03;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0x35;
			short_dcs.param2 = 0x00;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			short_dcs.mipi_port = dcs_port_seq[i];
			short_dcs.data_type = DATASHORT_DCS_WRITE_1;
			short_dcs.param1 = 0x36;
			short_dcs.param2 = 0x00;
			short_dcs.pData = NULL;
			mipi_packet_send_short(&short_dcs);
		}

		for(uint8_t i = 0; i < MIPI_TOTAL_PORT; i++){
			long_dcs.mipi_port = dcs_port_seq[i];
			long_dcs.data_type = DATALONG_GEN_WRITE;
			long_dcs.word_count = 0x0005;
			long_dcs.pData = &dsc_long3[0];
			mipi_packet_send_long(&long_dcs);
		}
		
		/* Backlight should be turned on here but looks like it still works if its enabled earlier */
		#endif
    }
    
//-----------------------------------------------------------------------------
void chicago_main(void){
		
	#ifdef DEBUG_LEVEL_3
		TRACE0("chicago_main(void)\n");
	#endif
		
	char return_value;
	
	switch(current_state){

	case STATE_NONE:
	
		#ifdef	DEBUG_LEVEL_3
			TRACE1("\t%s\n", "STATE_NONE"); 
		#endif
	
		// Change last state
		last_state = STATE_NONE;
		
		// Panel power supply off, so that also turn off backlight
		mipi_panel_power_supply(PANEL_POWER_SUPPLY_OFF);
		
		// power down Chicago power first
		chicago_power_supply(CHICAGO_POWER_SUPPLY_OFF);
		
		// power down CHIP_POWER_UP and RESET
		chicago_power_onoff(CHICAGO_TURN_OFF);
		
		// Verify I2C is working
		// chicago_read_chip_id();
		
		// Clean dp cable state
		dp_cable = DP_CABLE_OUT;
		
		// Clean HPD flag
		set_HPD = FLAG_VALUE_OFF;
		
		// Disable /INT0 interrupts first
		EXT_INTR_DISABLE(); 
		
		delay_ms(5);
		
		// power on Chicago power first
		chicago_power_supply(CHICAGO_POWER_SUPPLY_ON);
		
		HDK_chicago_set_vsync_out(); // ADAM
		
		//if((DIP_SWITCH_ON==(debug_switch&DS_BIST)) || (DIP_SWITCH_ON==(debug_switch&DS_SCRITP_ONLY))){
		#ifdef DEBUG_ENABLE_COLOR_BARS
			dp_cable = DP_CABLE_IN;
		#endif 
		//}
			
		// Enable /INT0 interrupts first
		EXT_INTR_EVENT_CLEAR();
		EXT_INTR_ENABLE();
		
		// Check current interrupt state first
		if(dp_check_cable_plug_int() == DP_CABLE_IN)
		{
			dp_cable = DP_CABLE_IN;
		}
		
		#ifdef DEBUG_REMOVE_CABLE_DET
			// don't detect cable
			dp_cable = DP_CABLE_IN;
		#endif
		
		// move to next state
		current_state = STATE_WAITCABLE;
		break;

	case STATE_WAITCABLE:
		
		#ifdef	DEBUG_LEVEL_3
			TRACE1("\t%s\n", "STATE_WAITCABLE");
		#endif
		
		// adam edit
// 		if(0 /*DIP_SWITCH_ON==(debug_switch&DS_SCRITP_ONLY)*/){
// 
// 			// For Debug script only
// 			if(STATE_NONE==last_state){
// 	
// 				// RESET and POWER UP Chicago
// 				chicago_power_onoff(CHICAGO_TURN_ON);
// 	
// 				// Stop OCM
// 				chicago_stop_ocm();
// 	
// 				// Power On panel
// 				mipi_panel_power_supply(PANEL_POWER_SUPPLY_ON);
// 	
// 				// Turn On backlight
// 				// mipi_panel_onoff(PANEL_DISPLAY_ON);
// 	
// 				// into sleep mode
// 				mipi_power_on_sequence(PANEL_SLEEP_IN);
// 
// 				// Change state
// 				last_state = STATE_WAITCABLE;
// 			}
// 		}
//		else{
	
			// DP cable plug-in
			if(dp_cable == DP_CABLE_IN)
			{
	
				if(last_state == STATE_NONE)
				{
	
					// RESET and POWER UP Chicago
					chicago_power_onoff(CHICAGO_TURN_ON);
					
					chicago_read_chip_id();
					
					// ryan 
					// Set HPD to low
					// chicago_hpd_set(HDP_FORCE, HDP_DATA_LOW);
				
					return_value = chicago_ocm_crc_checking();
	
					// checking OCM load status, if CRC32 error, re-load OCM again
					if(return_value == RETURN_FAILURE_VALUE)
					{
	
						TRACE1("\t%s\n", "OCM CRC Error, reboot ANX7530");

						// power down CHIP_POWER_UP and RESET
						chicago_power_onoff(CHICAGO_TURN_OFF);

						// power down Chicago power first
						chicago_power_supply(CHICAGO_POWER_SUPPLY_OFF);
						delay_ms(5);

						// power on Chicago power first
						chicago_power_supply(CHICAGO_POWER_SUPPLY_ON);

						// RESET and POWER UP Chicago
						chicago_power_onoff(CHICAGO_TURN_ON);
					}
					
					else if(return_value == RETURN_FAILURE_VALUE2)
					{
						TRACE1("\t%s\n", "I2C Error!!!");						
					}
					
					else
					{					
						#ifdef DEBUG_LEVEL_2
							TRACE1("\t%s\n", "OCM CRC pass.");
						#endif
						
						// Change last state
						last_state = STATE_WAITCABLE;
						
						// adam edit
						// Get MIPI TX data
						if(0 /*DIP_SWITCH_ON==(debug_switch&DS_EDID_SELECT)*/)
						{
							// EDID for HDMI
							edid_buffer = &edid_hdmi[0];
							edid_external_buffer = &edid_external_hdmi[0];
						}
						else
						{
							// EDID for DP
							edid_buffer = &edid_dp[0];
							edid_external_buffer = &edid_external_dp[0];
						}

						uint32_t edid_serial_number = 1024;

						mipi_edid_write_buffer(edid_serial_number);
						mipi_edid_write_external_buffer();

						// Audio MCLK always on, sample interface
						//chicago_audio_mclk_always_out();

						chicago_set_panel_parameters();
						
						#ifdef DEBUG_CHECK_DP_RESOLUTION
							chicago_get_edid_resolution();
						#endif

						// adam edit
						if(0 /*DIP_SWITCH_ON==(debug_switch&DS_SCRITP_ONLY)*/ )
						{
							// Debug Script stops in current state (STATE_WAITCABLE)
						}
						else
						{
							// Normal Operation goes to next state (STATE_CONNECTING)
							current_state = STATE_CONNECTING;
						}
					}
				}
			}
		break;

	case STATE_CONNECTING:

		#ifdef	DEBUG_LEVEL_3
			TRACE1("\t%s\n", "STATE_CONNECTING"); 
		#endif
	
		if(last_state == STATE_WAITCABLE)
		{
			mipi_mcu_write_done();

			mipi_panel_power_supply(PANEL_POWER_SUPPLY_ON);
			
			// Turn off backlight
			//DCS_display_control(PANEL_DISPLAY_OFF);
			
			// into sleep mode
			//DCS_sleep_control(PANEL_SLEEP_IN);
			
			// clear video stable flag
			chicago_clear_video_stable();
			chicago_interrupt_set();

			// Change last state
			last_state = STATE_CONNECTING;
		}
		
		else if(last_state == STATE_CONNECTING)
		{
		
			// Set HPD
			if(set_HPD == FLAG_VALUE_OFF)
			{
		
				// did not set HPD before
				//if(DIP_SWITCH_ON==(debug_switch&DS_BIST)){
				#ifdef DEBUG_ENABLE_COLOR_BARS 
					// no need to set HPD in BIST mode
					set_HPD = FLAG_VALUE_ON;
				#else
				//}
				//else{
					if(chicago_check_ocm_status())
					{
						// OCM load done
						
						// We can set customized setting and DPCD 
						// after "OCM load done" and before HPD to unforce high
						// chicago_customized_setting();
						DPCD_setting();
				
                        // ADAM - Is this needed?
                        delay_ms(200);
						
						// Set HPD to unForce High
						chicago_hpd_set(HDP_UNFORCE, HDP_DATA_HIGH);
				
						// set flag to ON, no need to set HPD again
						set_HPD = FLAG_VALUE_ON;
				
						#ifdef DEBUG_LEVEL_2
							TRACE1("\t%s\n", "HotPlug Detect set to HIGH");
						#endif
					}
				#endif
				//}
			}
				
//			// Check Video stable
//			// Broken!!!
// 			if(chicago_check_video_stable() == 1){
// 				current_state = STATE_NORMAL;
// 			}
	
			if(HDK_chicago_actually_check_video_stable() == true)
			{
				current_state = STATE_NORMAL;
			}
		}
		
		else if(last_state == STATE_NORMAL)
		{
			TRACE1("\t%s\n", "Panel Off");
			
			// Turn off backlight
			DCS_display_control(PANEL_DISPLAY_OFF);
			
			// Go into sleep mode
			DCS_sleep_control(PANEL_SLEEP_IN);
						
			// power supply off panel
			mipi_panel_power_supply(PANEL_POWER_SUPPLY_OFF);
			
			// power supply on panel
			mipi_panel_power_supply(PANEL_POWER_SUPPLY_ON);

			last_state = STATE_CONNECTING;
		}
		else
		{
			#ifdef	DEBUG_LEVEL_2
				TRACE1("\t%s\n", "ERROR_STATE"); 
			#endif
	
			// Error state
			TRACE2("\t%s: Error last state, last_state = %d", __LINE__, (uint32_t)(last_state));
			
			last_state = STATE_CONNECTING;
			current_state = STATE_NONE;
		}
		break;

	case STATE_NORMAL:
		#ifdef	DEBUG_LEVEL_3
			TRACE1("\t%s\n", "STATE_NORMAL"); 
		#endif

		if(last_state == STATE_CONNECTING)
		{
			//Serial.println("Panel ON");
			last_state = STATE_NORMAL;
			
			// Panel initialization
            DSC_PPS_enable_and_DCS_init();        
			
			// Sleep control             
			DCS_sleep_control(PANEL_SLEEP_OUT);
			
			//Turn on backlight
			DCS_display_control(PANEL_DISPLAY_ON);
		}
		
		if(STATE_NORMAL==last_state)
		{
			if(chicago_check_video_stable() == 0)
			{
				current_state = STATE_CONNECTING;
			}
		}
		break;

	// for debug only
	case STATE_POWEROFF:
		#ifdef	DEBUG_LEVEL_3
			TRACE1("\t%s\n", "STATE_POWEROFF"); 
		#endif
		
		// For Debug Command Used Only
		if(last_state != STATE_POWEROFF)
		{
			chicago_power_onoff(CHICAGO_TURN_OFF);
	
			// power down Chicago power first
			chicago_power_supply(CHICAGO_POWER_SUPPLY_OFF);
	
			// Clean dp cable state
			dp_cable = DP_CABLE_OUT;
	
			// Disable /INT0 interrupts first
			EXT_INTR_DISABLE();

			// Change last state
			last_state = STATE_POWEROFF;
		}
		break;

	default:
		#ifdef	DEBUG_LEVEL_3
			TRACE1("\t%s\n", "STATE_DEFAULT_ERROR"); 
		#endif
	
		TRACE1("\tError current state, current_state = %d\n", (uint32_t)(current_state));
		
		last_state = STATE_NONE;
		current_state = STATE_NONE;
		
		break;
	}
}


