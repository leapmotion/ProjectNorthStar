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

#ifndef SAMD21_HOST_H_INCLUDED
#define SAMD21_HOST_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

extern __attribute__((__aligned__(4))) volatile UsbHostDescriptor usb_pipe_table[USB_EPT_NUM];

#define  USB_EP_DIR_IN        0x80  // USB_SETUP_DEVICE_TO_HOST
#define  USB_EP_DIR_OUT       0x00  // USB_SETUP_HOST_TO_DEVICE

#define USB_HOST_PTYPE_DIS     USB_HOST_PCFG_PTYPE(0x0) // Pipe is disabled
#define USB_HOST_PTYPE_CTRL    USB_HOST_PCFG_PTYPE(0x1) // Pipe is enabled and configured as CONTROL
#define USB_HOST_PTYPE_ISO     USB_HOST_PCFG_PTYPE(0x2) // Pipe is enabled and configured as ISO
#define USB_HOST_PTYPE_BULK    USB_HOST_PCFG_PTYPE(0x3) // Pipe is enabled and configured as BULK
#define USB_HOST_PTYPE_INT     USB_HOST_PCFG_PTYPE(0x4) // Pipe is enabled and configured as INTERRUPT
#define USB_HOST_PTYPE_EXT     USB_HOST_PCFG_PTYPE(0x5) // Pipe is enabled and configured as EXTENDED

#define USB_HOST_NB_BK_1		1

#define USB_HOST_PCFG_PTOKEN_SETUP  USB_HOST_PCFG_PTOKEN(0x0)
#define USB_HOST_PCFG_PTOKEN_IN     USB_HOST_PCFG_PTOKEN(0x1)
#define USB_HOST_PCFG_PTOKEN_OUT    USB_HOST_PCFG_PTOKEN(0x2)

#define USB_ERRORFLOW	      USB_HOST_STATUS_BK_ERRORFLOW
#define USB_ERRORTIMEOUT      USB_HOST_STATUS_PIPE_TOUTER
#define USB_ERROR_DATATOGGLE  USB_HOST_STATUS_PIPE_DTGLER

#define USB_PCKSIZE_SIZE_8_BYTES        0
#define USB_PCKSIZE_SIZE_16_BYTES       1
#define USB_PCKSIZE_SIZE_32_BYTES       2
#define USB_PCKSIZE_SIZE_64_BYTES       3 
#define USB_PCKSIZE_SIZE_128_BYTES      4
#define USB_PCKSIZE_SIZE_256_BYTES      5
#define USB_PCKSIZE_SIZE_512_BYTES      6
#define USB_PCKSIZE_SIZE_1023_BYTES_FS  7   
#define USB_PCKSIZE_SIZE_1024_BYTES_HS  7  

#define USB_HOST_DTGL(p)               (USB->HOST.HostPipe[p].PSTATUS.reg & USB_HOST_PSTATUS_DTGL)>>USB_HOST_PSTATUS_DTGL_Pos

// USB host connection/disconnection monitoring
#define uhd_enable_connection_int()           USB->HOST.INTENSET.reg = USB_HOST_INTENSET_DCONN
#define uhd_disable_connection_int()          USB->HOST.INTENCLR.reg = USB_HOST_INTENCLR_DCONN
#define uhd_ack_connection()                  USB->HOST.INTFLAG.reg = USB_HOST_INTFLAG_DCONN

#define uhd_enable_disconnection_int()        USB->HOST.INTENSET.reg = USB_HOST_INTENSET_DDISC
#define uhd_disable_disconnection_int()       USB->HOST.INTENCLR.reg = USB_HOST_INTENCLR_DDISC
#define uhd_ack_disconnection()               USB->HOST.INTFLAG.reg = USB_HOST_INTFLAG_DDISC

// Initiates a USB register reset
#define uhd_start_USB_reg_reset()             USB->HOST.CTRLA.bit.SWRST = 1;

// Bus Reset
#define Is_uhd_starting_reset()             (USB->HOST.CTRLB.bit.BUSRESET == 1)
#define UHD_BusReset()                      USB->HOST.CTRLB.bit.BUSRESET = 1
#define uhd_stop_reset()                    // nothing to do

#define uhd_ack_reset_sent()                         USB->HOST.INTFLAG.reg = USB_HOST_INTFLAG_RST
#define Is_uhd_reset_sent()                          (USB->HOST.INTFLAG.reg & USB_HOST_INTFLAG_RST)

// Initiates a SOF events
#define uhd_enable_sof()                             USB->HOST.CTRLB.bit.SOFE = 1
#define uhd_disable_sof()                            USB->HOST.CTRLB.bit.SOFE = 0
#define Is_uhd_sof_enabled()                         (USB->HOST.CTRLB & USB_HOST_CTRLB_SOFE)
#define Is_uhd_sof()                                 (USB->HOST.INTFLAG.reg & USB_HOST_INTFLAG_HSOF)

// USB address of pipes
#define uhd_configure_address(pipe_num, addr) usb_pipe_table[pipe_num].HostDescBank[0].CTRL_PIPE.bit.PDADDR = addr
#define uhd_get_configured_address(pipe_num)  usb_pipe_table[pipe_num].HostDescBank[0].CTRL_PIPE.bit.PDADDR

// Pipes
#define uhd_freeze_pipe(p)                       USB->HOST.HostPipe[p].PSTATUSSET.reg = USB_HOST_PSTATUSSET_PFREEZE
#define uhd_unfreeze_pipe(p)                     USB->HOST.HostPipe[p].PSTATUSCLR.reg = USB_HOST_PSTATUSCLR_PFREEZE
#define Is_uhd_pipe_frozen(p)                    ((USB->HOST.HostPipe[p].PSTATUS.reg&USB_HOST_PSTATUS_PFREEZE)==USB_HOST_PSTATUS_PFREEZE)

// Pipe configuration
#define uhd_configure_pipe_token(p, token)       USB->HOST.HostPipe[p].PCFG.bit.PTOKEN = token

// Pipe data management
#define uhd_byte_count(p)                        usb_pipe_table[p].HostDescBank[0].PCKSIZE.bit.BYTE_COUNT
#define uhd_ack_setup_ready(p)                   USB->HOST.HostPipe[p].PINTFLAG.reg = USB_HOST_PINTFLAG_TXSTP
#define Is_uhd_setup_ready(p)                    ((USB->HOST.HostPipe[p].PINTFLAG.reg&USB_HOST_PINTFLAG_TXSTP) == USB_HOST_PINTFLAG_TXSTP)
#define uhd_ack_in_received(p)                   USB->HOST.HostPipe[p].PINTFLAG.reg = USB_HOST_PINTFLAG_TRCPT(1)
#define Is_uhd_in_received(p)                    ((USB->HOST.HostPipe[p].PINTFLAG.reg&USB_HOST_PINTFLAG_TRCPT(1)) == USB_HOST_PINTFLAG_TRCPT(1))
#define uhd_ack_out_ready(p)                     USB->HOST.HostPipe[p].PINTFLAG.reg = USB_HOST_PINTFLAG_TRCPT(1)
#define Is_uhd_out_ready(p)                      ((USB->HOST.HostPipe[p].PINTFLAG.reg&USB_HOST_PINTFLAG_TRCPT(1)) == USB_HOST_PINTFLAG_TRCPT(1))
#define uhd_ack_nak_received(p)                  usb_pipe_table[p].HostDescBank[1].STATUS_BK.reg &= ~USB_HOST_STATUS_BK_ERRORFLOW
#define Is_uhd_nak_received(p)                   (usb_pipe_table[p].HostDescBank[1].STATUS_BK.reg & USB_HOST_STATUS_BK_ERRORFLOW)

// Endpoint Interrupt Summary
#define uhd_endpoint_interrupt()            USB->HOST.PINTSMRY.reg

// Run in Standby
#define uhd_run_in_standby()                USB->HOST.CTRLA.reg |= USB_CTRLA_RUNSTDBY
// Force host mode
#define uhd_force_host_mode()               USB->HOST.CTRLA.reg |= USB_CTRLA_MODE

// Enable USB macro
#define uhd_enable()                        USB->HOST.CTRLA.reg |= USB_CTRLA_ENABLE
// Disable USB macro
#define uhd_disable()                       USB->HOST.CTRLA.reg &= ~USB_CTRLA_ENABLE

// Force full speed mode
#define uhd_force_full_speed()              USB->HOST.CTRLB.reg &= ~USB_HOST_CTRLB_SPDCONF_Msk

#ifdef __cplusplus
}
#endif

#endif /* SAMD21_HOST_H_INCLUDED */
