/*
  Copyright (c) 2014 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../Arduino.h"
#include "variant.h"
#include "USB_host.h"
#include "samd21_host.h"
#include "sam.h"
#include "wiring_digital.h"
#include "wiring_private.h"

#define HOST_DEFINED
#ifdef HOST_DEFINED

//#define TRACE_UOTGHS_HOST(x)	x
#define TRACE_UOTGHS_HOST(x)

// Handle UOTGHS Host driver state
static uhd_vbus_state_t uhd_state = UHD_STATE_NO_VBUS;

__attribute__((__aligned__(4))) volatile UsbHostDescriptor usb_pipe_table[USB_EPT_NUM];

extern void (*gpf_isr)(void);


// NVM Software Calibration Area Mapping
// USB TRANSN calibration value. Should be written to the USB PADCAL register.
#define NVM_USB_PAD_TRANSN_POS     45
#define NVM_USB_PAD_TRANSN_SIZE    5
// USB TRANSP calibration value. Should be written to the USB PADCAL register.
#define NVM_USB_PAD_TRANSP_POS     50
#define NVM_USB_PAD_TRANSP_SIZE    5
// USB TRIM calibration value. Should be written to the USB PADCAL register.
#define NVM_USB_PAD_TRIM_POS       55
#define NVM_USB_PAD_TRIM_SIZE      3

/**
 * \brief Initialize the SAMD21 host driver.
 */
void UHD_Init(void)
{
	uint32_t pad_transn;
	uint32_t pad_transp;
	uint32_t pad_trim;
	uint32_t i;

	USB_SetHandler(&UHD_Handler);

	/* Enable USB clock */
	PM->APBBMASK.reg |= PM_APBBMASK_USB;

	/* Set up the USB DP/DM pins */
	pinPeripheral( PIN_USB_DM, PIO_COM );
	pinPeripheral( PIN_USB_DP, PIO_COM );
// 	PORT->Group[0].PINCFG[PIN_PA24G_USB_DM].bit.PMUXEN = 1;
// 	PORT->Group[0].PMUX[PIN_PA24G_USB_DM/2].reg &= ~(0xF << (4 * (PIN_PA24G_USB_DM & 0x01u)));
// 	PORT->Group[0].PMUX[PIN_PA24G_USB_DM/2].reg |= MUX_PA24G_USB_DM << (4 * (PIN_PA24G_USB_DM & 0x01u));
// 	PORT->Group[0].PINCFG[PIN_PA25G_USB_DP].bit.PMUXEN = 1;
// 	PORT->Group[0].PMUX[PIN_PA25G_USB_DP/2].reg &= ~(0xF << (4 * (PIN_PA25G_USB_DP & 0x01u)));
// 	PORT->Group[0].PMUX[PIN_PA25G_USB_DP/2].reg |= MUX_PA25G_USB_DP << (4 * (PIN_PA25G_USB_DP & 0x01u));

	/* ----------------------------------------------------------------------------------------------
	* Put Generic Clock Generator 0 as source for Generic Clock Multiplexer 6 (USB reference)
	*/
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(6) |        // Generic Clock Multiplexer 6
						GCLK_CLKCTRL_GEN_GCLK0 |    // Generic Clock Generator 0 is source
						GCLK_CLKCTRL_CLKEN;

	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY)
	{
		/* Wait for synchronization */
	}

	/* Reset */
	USB->HOST.CTRLA.bit.SWRST = 1;
	while (USB->HOST.SYNCBUSY.bit.SWRST)
	{
		/* Sync wait */
	}

	//  uhd_enable();
	USB->DEVICE.CTRLA.reg |= USB_CTRLA_ENABLE | USB_CTRLA_MODE;
	uhd_force_host_mode();
	while (USB->HOST.SYNCBUSY.reg == USB_SYNCBUSY_ENABLE);

	/* Load Pad Calibration */
	pad_transn = (*((uint32_t *)(NVMCTRL_OTP4)       // Non-Volatile Memory Controller
					+ (NVM_USB_PAD_TRANSN_POS / 32))
					>> (NVM_USB_PAD_TRANSN_POS % 32))
				& ((1 << NVM_USB_PAD_TRANSN_SIZE) - 1);

	if (pad_transn == 0x1F)         // maximum value (31)
	{
		pad_transn = 5;
	}

	USB->HOST.PADCAL.bit.TRANSN = pad_transn;

	pad_transp = (*((uint32_t *)(NVMCTRL_OTP4)
					+ (NVM_USB_PAD_TRANSP_POS / 32))
					>> (NVM_USB_PAD_TRANSP_POS % 32))
				& ((1 << NVM_USB_PAD_TRANSP_SIZE) - 1);

	if (pad_transp == 0x1F)         // maximum value (31)
	{
		pad_transp = 29;
	}

	USB->HOST.PADCAL.bit.TRANSP = pad_transp;

	pad_trim = (*((uint32_t *)(NVMCTRL_OTP4)
					+ (NVM_USB_PAD_TRIM_POS / 32))
				>> (NVM_USB_PAD_TRIM_POS % 32))
				& ((1 << NVM_USB_PAD_TRIM_SIZE) - 1);

	if (pad_trim == 0x7)         // maximum value (7)
	{
		pad_trim = 3;
	}

	USB->HOST.PADCAL.bit.TRIM = pad_trim;


	/* Set the configuration */
	uhd_run_in_standby();
	// Set address of USB SRAM
	USB->HOST.DESCADD.reg = (uint32_t)(&usb_pipe_table[0]);
	// For USB_SPEED_FULL
	uhd_force_full_speed();
	for (i = 0; i < sizeof(usb_pipe_table); i++)
	{
		(*(uint32_t *)(&usb_pipe_table[0] + i)) = 0;
	}

	uhd_state = UHD_STATE_NO_VBUS;

	// Put VBUS on USB port
	pinMode( PIN_USB_HOST_ENABLE, OUTPUT );
	digitalWrite( PIN_USB_HOST_ENABLE, HIGH );

	uhd_enable_connection_int();

	USB->HOST.INTENSET.reg = USB_HOST_INTENSET_DCONN;
	USB->HOST.INTENSET.reg = USB_HOST_INTENSET_WAKEUP;
	USB->HOST.INTENSET.reg = USB_HOST_INTENSET_DDISC;

	USB->HOST.CTRLB.bit.VBUSOK = 1;

	// Configure interrupts
	NVIC_SetPriority((IRQn_Type)USB_IRQn, 0UL);
	NVIC_EnableIRQ((IRQn_Type)USB_IRQn);
}


/**
 * \brief Interrupt sub routine for USB Host state machine management.
 */
void UHD_Handler(void)
{
   uint16_t flags;

	if (USB->HOST.CTRLA.bit.MODE) {
		/*host mode ISR */

		/* get interrupt flags */
		flags = USB->HOST.INTFLAG.reg;

		/* host SOF interrupt */
		if (flags & USB_HOST_INTFLAG_HSOF)
		{
			/* clear the flag */
			USB->HOST.INTFLAG.reg = USB_HOST_INTFLAG_HSOF;
			uhd_state             = UHD_STATE_CONNECTED;
			return;
		}

		/* host reset interrupt */
		if (flags & USB_HOST_INTFLAG_RST)
		{
			/* clear the flag */
			USB->HOST.INTFLAG.reg = USB_HOST_INTFLAG_RST;
			uhd_state             = UHD_STATE_DISCONNECTED;    //UHD_STATE_ERROR;
			return;
		}

		/* host upstream resume interrupts */
		if (flags & USB_HOST_INTFLAG_UPRSM)
		{
			/* clear the flags */
			USB->HOST.INTFLAG.reg = USB_HOST_INTFLAG_UPRSM;
			uhd_state             = UHD_STATE_DISCONNECTED;    //UHD_STATE_ERROR;
			return;
		}

		/* host downstream resume interrupts */
		if (flags & USB_HOST_INTFLAG_DNRSM)
		{
			/* clear the flags */
			USB->HOST.INTFLAG.reg = USB_HOST_INTFLAG_DNRSM;
			uhd_state             = UHD_STATE_DISCONNECTED;    //UHD_STATE_ERROR;
			return;
		}

		/* host wakeup interrupts */
		if (flags & USB_HOST_INTFLAG_WAKEUP)
		{
			/* clear the flags */
			USB->HOST.INTFLAG.reg = USB_HOST_INTFLAG_WAKEUP;
			uhd_state             = UHD_STATE_CONNECTED;    //UHD_STATE_ERROR;
			return;
		}

		/* host ram access interrupt  */
		if (flags & USB_HOST_INTFLAG_RAMACER)
		{
			/* clear the flag */
			USB->HOST.INTFLAG.reg = USB_HOST_INTFLAG_RAMACER;
			uhd_state             = UHD_STATE_DISCONNECTED;    //UHD_STATE_ERROR;
			return;
		}

		/* host connect interrupt */
		if (flags & USB_HOST_INTFLAG_DCONN)
		{
			TRACE_UOTGHS_HOST(printf(">>> UHD_ISR : Connection INT\r\n");
							)
			/* clear the flag */
			uhd_ack_connection();
			uhd_disable_connection_int();
			uhd_ack_disconnection();
			uhd_enable_disconnection_int();
			//uhd_enable_sof();
			uhd_state = UHD_STATE_CONNECTED;
			return;
		}

		/* host disconnect interrupt  */
		if (flags & USB_HOST_INTFLAG_DDISC)
		{
			TRACE_UOTGHS_HOST(printf(">>> UHD_ISR : Disconnection INT\r\n");
							)
			/* clear the flag */
			uhd_ack_disconnection();
			uhd_disable_disconnection_int();
			// Stop reset signal, in case of disconnection during reset
			uhd_stop_reset();
			// Disable wakeup/resumes interrupts,
			// in case of disconnection during suspend mode
			uhd_ack_connection();
			uhd_enable_connection_int();
			uhd_state = UHD_STATE_DISCONNECTED;
			return;
		}
	}
	else {
		while(1);
	}
}



/**
 * \brief Get VBUS state.
 *
 * \return VBUS status.
 */
uhd_vbus_state_t UHD_GetVBUSState(void)
{
   return uhd_state;
}


/**
 * \brief Allocate FIFO for pipe 0.
 *
 * \param ul_add Address of remote device for pipe 0.
 * \param ul_ep_size Actual size of the FIFO in bytes.
 *
 * \retval 0 success.
 * \retval 1 error.
 */
uint32_t UHD_Pipe0_Alloc(uint32_t ul_add , uint32_t ul_ep_size)
{
	(void)(ul_add); // Unused argument

	if( USB->HOST.STATUS.reg & USB_HOST_STATUS_SPEED(1) )
		ul_ep_size = USB_PCKSIZE_SIZE_8_BYTES;  // Low Speed
	else
		ul_ep_size = USB_PCKSIZE_SIZE_64_BYTES; // Full Speed

	USB->HOST.HostPipe[0].PCFG.bit.PTYPE = 1; //USB_HOST_PCFG_PTYPE_CTRL;
	usb_pipe_table[0].HostDescBank[0].CTRL_PIPE.bit.PEPNUM = 0;
	usb_pipe_table[0].HostDescBank[0].PCKSIZE.bit.SIZE = ul_ep_size;

	return 0;
}


/**
 * \brief Allocate a new pipe.
 *
 * \note UOTGHS maximum pipe number is limited to 10, meaning that only a limited
 * amount of devices can be connected. Unfortunately, using only one pipe shared accross
 * various endpoints and devices is not possible because the UOTGHS IP does not allow to
 * change the data toggle value through register interface.
 *
 * \param ul_dev_addr Address of remote device.
 * \param ul_dev_ep Targeted endpoint of remote device.
 * \param ul_type Pipe type.
 * \param ul_dir Pipe direction.
 * \param ul_maxsize Pipe size.
 * \param ul_interval Polling interval (if applicable to pipe type).
 * \param ul_nb_bank Number of banks associated with this pipe.
 *
 * \return 1.
 */
uint32_t UHD_Pipe_Alloc(uint32_t ul_dev_addr, uint32_t ul_dev_ep, uint32_t ul_type, uint32_t ul_dir, uint32_t ul_maxsize, uint32_t ul_interval, uint32_t ul_nb_bank)
{
	/* set pipe config */
	USB->HOST.HostPipe[ul_dev_ep].PCFG.bit.BK    = ul_nb_bank;
	// PTYPE:
	USB->HOST.HostPipe[ul_dev_ep].PCFG.reg &= ~USB_HOST_PCFG_MASK;  // USB->HOST.HostPipe[0].PCFG.bit.PTYPE = 1; //USB_HOST_PCFG_PTYPE_CTRL;
	USB->HOST.HostPipe[ul_dev_ep].PCFG.reg |= ul_type;
	USB->HOST.HostPipe[ul_dev_ep].BINTERVAL.reg  = ul_interval;

   if (ul_dir & USB_EP_DIR_IN)
   {
      USB->HOST.HostPipe[ul_dev_ep].PCFG.bit.PTOKEN = USB_HOST_PCFG_PTOKEN_IN;
      USB->HOST.HostPipe[ul_dev_ep].PSTATUSSET.reg  = USB_HOST_PSTATUSSET_BK0RDY;
   }
   else
   {
      USB->HOST.HostPipe[ul_dev_ep].PCFG.bit.PTOKEN = USB_HOST_PCFG_PTOKEN_OUT;
      USB->HOST.HostPipe[ul_dev_ep].PSTATUSCLR.reg  = USB_HOST_PSTATUSCLR_BK0RDY;
   }

	if( USB->HOST.STATUS.reg & USB_HOST_STATUS_SPEED(1) )
	   ul_maxsize = USB_PCKSIZE_SIZE_8_BYTES;  // Low Speed
	else
	   ul_maxsize = USB_PCKSIZE_SIZE_64_BYTES; // Full Speed

   memset((uint8_t *)&usb_pipe_table[ul_dev_ep], 0, sizeof(usb_pipe_table[ul_dev_ep]));

   usb_pipe_table[ul_dev_ep].HostDescBank[0].CTRL_PIPE.bit.PDADDR = ul_dev_addr;
   usb_pipe_table[ul_dev_ep].HostDescBank[0].CTRL_PIPE.bit.PEPNUM = ul_dev_ep;
   usb_pipe_table[ul_dev_ep].HostDescBank[0].PCKSIZE.bit.SIZE     = ul_maxsize;

   return 1;
}


void UHD_Pipe_CountZero(uint32_t ul_pipe)
{
	usb_pipe_table[ul_pipe].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT = 0;
}

/**
 * \brief Free a pipe.
 *
 * \param ul_pipe Pipe number to free.
 */
void UHD_Pipe_Free(uint32_t ul_pipe)
{
   // The Pipe is frozen and no additional requests will be sent to the device on this pipe address.
   USB->HOST.HostPipe[ul_pipe].PSTATUSSET.reg = USB_HOST_PSTATUSSET_PFREEZE;
}


/**
 * \brief Read from a pipe.
 *
 * \param ul_pipe Pipe number.
 * \param ul_size Maximum number of data to read.
 * \param data Buffer to store the data.
 *
 * \return number of data read.
 */
uint32_t UHD_Pipe_Read(uint32_t pipe_num, uint32_t buf_size, uint8_t *buf)
{
   if (USB->HOST.HostPipe[pipe_num].PCFG.bit.PTYPE == USB_HOST_PTYPE_DIS)
   {
      return 0;
   }

   /* get pipe config from setting register */
   usb_pipe_table[pipe_num].HostDescBank[0].ADDR.reg = (uint32_t)buf;
   usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT        = 0;
   usb_pipe_table[pipe_num].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE = buf_size;
   USB->HOST.HostPipe[pipe_num].PCFG.bit.PTOKEN = USB_HOST_PCFG_PTOKEN_IN;

   /* Start transfer */
   USB->HOST.HostPipe[pipe_num].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_BK0RDY;

   // Unfreeze pipe
   USB->HOST.HostPipe[pipe_num].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_PFREEZE;

   return buf_size;
}


/**
 * \brief Write into a pipe.
 *
 * \param ul_pipe Pipe number.
 * \param ul_size Maximum number of data to read.
 * \param data Buffer containing data to write.
 */
void UHD_Pipe_Write(uint32_t ul_pipe, uint32_t ul_size, uint8_t *buf)
{
   /* get pipe config from setting register */
   usb_pipe_table[ul_pipe].HostDescBank[0].ADDR.reg = (uint32_t)buf;
   usb_pipe_table[ul_pipe].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT = ul_size;
   usb_pipe_table[ul_pipe].HostDescBank[0].PCKSIZE.bit.MULTI_PACKET_SIZE = 0;
}

/**
 * \brief Send a pipe content.
 *
 * \param ul_pipe Pipe number.
 * \param ul_token_type Token type.
 */
void UHD_Pipe_Send(uint32_t ul_pipe, uint32_t ul_token_type)
{
	USB->HOST.HostPipe[ul_pipe].PCFG.bit.PTOKEN = ul_token_type;

   /* Start transfer */
	if(ul_token_type == USB_HOST_PCFG_PTOKEN_SETUP )
	{
		USB->HOST.HostPipe[ul_pipe].PINTFLAG.reg = USB_HOST_PINTFLAG_TXSTP;
		USB->HOST.HostPipe[ul_pipe].PSTATUSSET.reg = USB_HOST_PSTATUSSET_BK0RDY;
	}
	else if(ul_token_type == USB_HOST_PCFG_PTOKEN_IN )
	{
		USB->HOST.HostPipe[ul_pipe].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_BK0RDY;
	}
	else
	{
		USB->HOST.HostPipe[ul_pipe].PINTFLAG.reg = USB_HOST_PINTFLAG_TRCPT(1);  // Transfer Complete 0
		USB->HOST.HostPipe[ul_pipe].PSTATUSSET.reg = USB_HOST_PSTATUSSET_BK0RDY;
	}
   
	// Unfreeze pipe
    uhd_unfreeze_pipe(ul_pipe);
}

/**
 * \brief Check for pipe transfer completion.
 *
 * \param ul_pipe Pipe number.
 * \param ul_token_type Token type.
 *
 * \retval 0 transfer is not complete.
 * \retval 1 transfer is complete.
 */
uint32_t UHD_Pipe_Is_Transfer_Complete(uint32_t ul_pipe, uint32_t ul_token_type)
{
   // Check for transfer completion depending on token type
   switch (ul_token_type)
   {
      case USB_HOST_PCFG_PTOKEN_SETUP:
         if (Is_uhd_setup_ready(ul_pipe))
         {
            uhd_ack_setup_ready(ul_pipe);
            uhd_freeze_pipe(ul_pipe);
            return 1;
         }
		 break;

      case USB_HOST_PCFG_PTOKEN_IN:
         if (Is_uhd_in_received(ul_pipe))
         {
            // IN packet received
            uhd_ack_in_received(ul_pipe);
			// Freeze will stop after the transfer
			uhd_freeze_pipe(ul_pipe);
            return 1;
         }
		 break;
 
      case USB_HOST_PCFG_PTOKEN_OUT:
         if (Is_uhd_out_ready(ul_pipe))
         {
            // OUT packet sent
            uhd_ack_out_ready(ul_pipe);
            uhd_freeze_pipe(ul_pipe);
            return 1;
         }
		 break;
   }

   return 0;
}




// USB_Handler ISR
// void USB_Handler(void) {
// 	UHD_Handler();
// }

#endif //  HOST_DEFINED
