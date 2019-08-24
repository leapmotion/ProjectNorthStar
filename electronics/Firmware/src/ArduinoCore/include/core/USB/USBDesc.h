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

#ifndef __USBDESC_H__
#define __USBDESC_H__

// CDC or HID can be enabled together.
#define CDC_ENABLED
#define PLUGGABLE_USB_ENABLED

#ifdef CDC_ENABLED
#define CDC_INTERFACE_COUNT 2
#define CDC_ENPOINT_COUNT 3
#else
#define CDC_INTERFACE_COUNT 0
#define CDC_ENPOINT_COUNT 0
#endif

// CDC
#define CDC_ACM_INTERFACE	0	// CDC ACM
#define CDC_DATA_INTERFACE	1	// CDC Data
#define CDC_FIRST_ENDPOINT  1
#define CDC_ENDPOINT_ACM	1
#define CDC_ENDPOINT_OUT	2
#define CDC_ENDPOINT_IN		3

#ifdef CDC_ENABLED
#define CDC_RX CDC_ENDPOINT_OUT
#define CDC_TX CDC_ENDPOINT_IN
#endif

#define ISERIAL_MAX_LEN        65

// Defined string description
#define IMANUFACTURER	1
#define IPRODUCT    2
#define ISERIAL    3

#endif /* __USBDESC_H__ */
