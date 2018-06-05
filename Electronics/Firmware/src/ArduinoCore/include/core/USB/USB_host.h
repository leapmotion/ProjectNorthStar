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

#ifndef USB_HOST_H_INCLUDED
#define USB_HOST_H_INCLUDED

#include <stdint.h>
#include "samd21_host.h"
#ifdef __cplusplus
extern "C" {
#endif

#define tokSETUP		USB_HOST_PCFG_PTOKEN_SETUP
#define tokIN			USB_HOST_PCFG_PTOKEN_IN
#define tokOUT			USB_HOST_PCFG_PTOKEN_OUT
#define tokINHS			USB_HOST_PCFG_PTOKEN_IN
#define tokOUTHS		USB_HOST_PCFG_PTOKEN_OUT

//! \brief Device speed
/*typedef enum {
	UHD_SPEED_LOW  = 0,
	UHD_SPEED_FULL = 1,
	UHD_SPEED_HIGH = 2,
} uhd_speed_t;*/

//! States of USBB interface
typedef enum {
	UHD_STATE_NO_VBUS = 0,
	UHD_STATE_DISCONNECTED = 1,
	UHD_STATE_CONNECTED = 2,
	UHD_STATE_ERROR = 3,
} uhd_vbus_state_t;

extern void UHD_Init(void);
extern void UHD_Handler(void);
extern void USB_SetHandler(void (*pf_isr)(void));
extern uhd_vbus_state_t UHD_GetVBUSState(void);
extern uint32_t UHD_Pipe0_Alloc(uint32_t ul_add, uint32_t ul_ep_size);
extern uint32_t UHD_Pipe_Alloc(uint32_t ul_dev_addr, uint32_t ul_dev_ep, uint32_t ul_type, uint32_t ul_dir, uint32_t ul_maxsize, uint32_t ul_interval, uint32_t ul_nb_bank);
extern void UHD_Pipe_CountZero(uint32_t ul_pipe);
extern void UHD_Pipe_Free(uint32_t ul_pipe);
extern uint32_t UHD_Pipe_Read(uint32_t ul_pipe, uint32_t ul_size, uint8_t* data);
extern void UHD_Pipe_Write(uint32_t ul_pipe, uint32_t ul_size, uint8_t* data);
extern void UHD_Pipe_Send(uint32_t ul_pipe, uint32_t ul_token_type);
extern uint32_t UHD_Pipe_Is_Transfer_Complete(uint32_t ul_pipe, uint32_t ul_token_type);

#ifdef __cplusplus
}
#endif

#endif /* USB_HOST_H_INCLUDED */
