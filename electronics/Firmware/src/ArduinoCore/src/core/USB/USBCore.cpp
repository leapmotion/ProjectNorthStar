/*
  Copyright (c) 2016 Arduino LLC.  All right reserved.

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

#if defined(USBCON)

#include <Arduino.h>

#include "SAMD21_USBDevice.h"
#include "PluggableUSB.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

USBDevice_SAMD21G18x usbd;

/** Pulse generation counters to keep track of the number of milliseconds remaining for each pulse type */
#define TX_RX_LED_PULSE_MS 100
#ifdef PIN_LED_TXL
static volatile uint8_t txLEDPulse; /**< Milliseconds remaining for data Tx LED pulse */
#endif
#ifdef PIN_LED_RXL
static volatile uint8_t rxLEDPulse; /**< Milliseconds remaining for data Rx LED pulse */
#endif
static char isRemoteWakeUpEnabled = 0;
static char isEndpointHalt = 0;

extern void (*gpf_isr)(void);

// USB_Handler ISR
extern "C" void UDD_Handler(void) {
	USBDevice.ISRHandler();
}

const uint16_t STRING_LANGUAGE[2] = {
	(3<<8) | (2+2),
	0x0409	// English
};

#ifndef USB_PRODUCT
// If no product is provided, use USB IO Board
#define USB_PRODUCT     "USB IO Board"
#endif

const uint8_t STRING_PRODUCT[] = USB_PRODUCT;

#if USB_VID == 0x2341
#  if defined(USB_MANUFACTURER)
#    undef USB_MANUFACTURER
#  endif
#  define USB_MANUFACTURER "Arduino LLC"
#elif !defined(USB_MANUFACTURER)
// Fall through to unknown if no manufacturer name was provided in a macro
#  define USB_MANUFACTURER "Unknown"
#endif

const uint8_t STRING_MANUFACTURER[] = USB_MANUFACTURER;


//	DEVICE DESCRIPTOR
const DeviceDescriptor USB_DeviceDescriptorB = D_DEVICE(0xEF, 0x02, 0x01, 64, USB_VID, USB_PID, 0x100, IMANUFACTURER, IPRODUCT, ISERIAL, 1);
const DeviceDescriptor USB_DeviceDescriptor = D_DEVICE(0x00, 0x00, 0x00, 64, USB_VID, USB_PID, 0x100, IMANUFACTURER, IPRODUCT, ISERIAL, 1);

//==================================================================

volatile uint32_t _usbConfiguration = 0;
volatile uint32_t _usbSetInterface = 0;

static __attribute__((__aligned__(4))) //__attribute__((__section__(".bss_hram0")))
uint8_t udd_ep_out_cache_buffer[7][64];

static __attribute__((__aligned__(4))) //__attribute__((__section__(".bss_hram0")))
uint8_t udd_ep_in_cache_buffer[7][64];

// Some EP are handled using EPHanlders.
// Possibly all the sparse EP handling subroutines will be
// converted into reusable EPHandlers in the future.
static EPHandler *epHandlers[7];

//==================================================================

// Send a USB descriptor string. The string is stored as a
// plain ASCII string but is sent out as UTF-16 with the
// correct 2-byte prefix
bool USBDeviceClass::sendStringDescriptor(const uint8_t *string, uint8_t maxlen)
{
	if (maxlen < 2)
		return false;

	uint8_t buffer[maxlen];
	buffer[0] = strlen((const char*)string) * 2 + 2;
	buffer[1] = 0x03;

	uint8_t i;
	for (i = 2; i < maxlen && *string; i++) {
		buffer[i++] = *string++;
		if (i == maxlen) break;
		buffer[i] = 0;
	}

	return USBDevice.sendControl(buffer, i);
}

bool _dry_run = false;
bool _pack_message = false;
uint16_t _pack_size = 0;
uint8_t _pack_buffer[256];

void USBDeviceClass::packMessages(bool val)
{
	if (val) {
		_pack_message = true;
		_pack_size = 0;
	} else {
		_pack_message = false;
		sendControl(_pack_buffer, _pack_size);
	}
}

uint8_t USBDeviceClass::SendInterfaces(uint32_t* total)
{
	uint8_t interfaces = 0;

#if defined(CDC_ENABLED)
	total[0] += CDC_GetInterface(&interfaces);
#endif

#ifdef PLUGGABLE_USB_ENABLED
	total[0] += PluggableUSB().getInterface(&interfaces);
#endif

	return interfaces;
}

// Construct a dynamic configuration descriptor
// This really needs dynamic endpoint allocation etc
uint32_t USBDeviceClass::sendConfiguration(uint32_t maxlen)
{
	uint32_t total = 0;
	// Count and measure interfaces
	_dry_run = true;
	uint8_t interfaces = SendInterfaces(&total);

	_Pragma("pack(1)")
	ConfigDescriptor config = D_CONFIG((uint16_t)(total + sizeof(ConfigDescriptor)), interfaces);
	_Pragma("pack()")

	//	Now send them
	_dry_run = false;

	if (maxlen == sizeof(ConfigDescriptor)) {
		sendControl(&config, sizeof(ConfigDescriptor));
		return true;
	}

	total = 0;

	packMessages(true);
	sendControl(&config, sizeof(ConfigDescriptor));
	SendInterfaces(&total);
	packMessages(false);

	return true;
}

static void utox8(uint32_t val, char* s) {
	for (int i = 0; i < 8; i++) {
		int d = val & 0XF;
		val = (val >> 4);

		s[7 - i] = d > 9 ? 'A' + d - 10 : '0' + d;
	}
}

bool USBDeviceClass::sendDescriptor(USBSetup &setup)
{
	uint8_t t = setup.wValueH;
	uint8_t desc_length = 0;
	bool _cdcComposite;
	int ret;
	const uint8_t *desc_addr = 0;

	if (t == USB_CONFIGURATION_DESCRIPTOR_TYPE)
	{
		return USBDevice.sendConfiguration(setup.wLength);
	}

#ifdef PLUGGABLE_USB_ENABLED
	ret = PluggableUSB().getDescriptor(setup);
	if (ret != 0) {
		return (ret > 0 ? true : false);
	}
#endif

	if (t == USB_DEVICE_DESCRIPTOR_TYPE)
	{
		if (setup.wLength == 8)
			_cdcComposite = 1;

		desc_addr = _cdcComposite ?  (const uint8_t*)&USB_DeviceDescriptorB : (const uint8_t*)&USB_DeviceDescriptor;

		if (*desc_addr > setup.wLength) {
			desc_length = setup.wLength;
		}
	}
	else if (USB_STRING_DESCRIPTOR_TYPE == t)
	{
		if (setup.wValueL == 0) {
			desc_addr = (const uint8_t*)&STRING_LANGUAGE;
		}
		else if (setup.wValueL == IPRODUCT) {
			return sendStringDescriptor(STRING_PRODUCT, setup.wLength);
			// MODIFIED BY ADAM
			//return sendStringDescriptor(STRING_PRODUCT, 2*sizeof(STRING_PRODUCT));
		}
		else if (setup.wValueL == IMANUFACTURER) {
			return sendStringDescriptor(STRING_MANUFACTURER, setup.wLength);
			// MODIFIED BY ADAM
			//return sendStringDescriptor(STRING_MANUFACTURER, 2*sizeof(STRING_MANUFACTURER));
		}
		else if (setup.wValueL == ISERIAL) {
#ifdef PLUGGABLE_USB_ENABLED

			
			// from section 9.3.3 of the datasheet
			#define SERIAL_NUMBER_WORD_0	*(volatile uint32_t*)(0x0080A00C)
			#define SERIAL_NUMBER_WORD_1	*(volatile uint32_t*)(0x0080A040)
			#define SERIAL_NUMBER_WORD_2	*(volatile uint32_t*)(0x0080A044)
			#define SERIAL_NUMBER_WORD_3	*(volatile uint32_t*)(0x0080A048)

			char name[ISERIAL_MAX_LEN];
			utox8(SERIAL_NUMBER_WORD_0, &name[0]);
			utox8(SERIAL_NUMBER_WORD_1, &name[8]);
			utox8(SERIAL_NUMBER_WORD_2, &name[16]);
			utox8(SERIAL_NUMBER_WORD_3, &name[24]);

			PluggableUSB().getShortName(&name[32]);
			return sendStringDescriptor((uint8_t*)name, setup.wLength);
		
			
			// MODIFIED_BY_ADAM
			//char name[] = "HDK-20-2.00"; 
			//return sendStringDescriptor((uint8_t*)name, setup.wLength);
#endif
		}
		else {
			return false;
		}
		if (*desc_addr > setup.wLength) {
			desc_length = setup.wLength;
		}
	}
	else
	{
	}

	if (desc_addr == 0) {
		return false;
	}

	if (desc_length == 0) {
		desc_length = *desc_addr;
	}

	sendControl(desc_addr, desc_length);

	return true;
}

void USBDeviceClass::standby() {
	usbd.noRunInStandby();
}


void USBDeviceClass::handleEndpoint(uint8_t ep)
{
#if defined(CDC_ENABLED)
	if (ep == CDC_ENDPOINT_IN)
	{
		// NAK on endpoint IN, the bank is not yet filled in.
		usbd.epBank1ResetReady(CDC_ENDPOINT_IN);
		usbd.epBank1AckTransferComplete(CDC_ENDPOINT_IN);
	}
	if (ep == CDC_ENDPOINT_ACM)
	{
		// NAK on endpoint IN, the bank is not yet filled in.
		usbd.epBank1ResetReady(CDC_ENDPOINT_ACM);
		usbd.epBank1AckTransferComplete(CDC_ENDPOINT_ACM);
	}
#endif

#if defined(PLUGGABLE_USB_ENABLED)
	// Empty
#endif
}

void USBDeviceClass::init()
{
#ifdef PIN_LED_TXL
	txLEDPulse = 0;
	pinMode(PIN_LED_TXL, OUTPUT);
	digitalWrite(PIN_LED_TXL, HIGH);
#endif

#ifdef PIN_LED_RXL
	rxLEDPulse = 0;
	pinMode(PIN_LED_RXL, OUTPUT);
	digitalWrite(PIN_LED_RXL, HIGH);
#endif

	// Enable USB clock
	PM->APBBMASK.reg |= PM_APBBMASK_USB;

	// Set up the USB DP/DN pins
	PORT->Group[0].PINCFG[PIN_PA24G_USB_DM].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[PIN_PA24G_USB_DM/2].reg &= ~(0xF << (4 * (PIN_PA24G_USB_DM & 0x01u)));
	PORT->Group[0].PMUX[PIN_PA24G_USB_DM/2].reg |= MUX_PA24G_USB_DM << (4 * (PIN_PA24G_USB_DM & 0x01u));
	PORT->Group[0].PINCFG[PIN_PA25G_USB_DP].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[PIN_PA25G_USB_DP/2].reg &= ~(0xF << (4 * (PIN_PA25G_USB_DP & 0x01u)));
	PORT->Group[0].PMUX[PIN_PA25G_USB_DP/2].reg |= MUX_PA25G_USB_DP << (4 * (PIN_PA25G_USB_DP & 0x01u));

	// Put Generic Clock Generator 0 as source for Generic Clock Multiplexer 6 (USB reference)
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(6)     | // Generic Clock Multiplexer 6
	                    GCLK_CLKCTRL_GEN_GCLK0 | // Generic Clock Generator 0 is source
	                    GCLK_CLKCTRL_CLKEN;
	while (GCLK->STATUS.bit.SYNCBUSY)
		;

	USB_SetHandler(&UDD_Handler);

	// Reset USB Device
	usbd.reset();

	usbd.calibrate();
	usbd.setUSBDeviceMode();
	usbd.runInStandby();
	usbd.setFullSpeed();

	// Configure interrupts
	NVIC_SetPriority((IRQn_Type) USB_IRQn, 0UL);
	NVIC_EnableIRQ((IRQn_Type) USB_IRQn);

	usbd.enable();

	initialized = true;
}

bool USBDeviceClass::attach()
{
	if (!initialized)
		return false;

	usbd.attach();
	usbd.enableEndOfResetInterrupt();
	usbd.enableStartOfFrameInterrupt();

	_usbConfiguration = 0;
	return true;
}

void USBDeviceClass::setAddress(uint32_t addr)
{
	usbd.epBank1SetByteCount(0, 0);
	usbd.epBank1AckTransferComplete(0);

	// RAM buffer is full, we can send data (IN)
	usbd.epBank1SetReady(0);

	// Wait for transfer to complete
	while (!usbd.epBank1IsTransferComplete(0)) {}

	// Set USB address to addr
	USB->DEVICE.DADD.bit.DADD = addr; // Address
	USB->DEVICE.DADD.bit.ADDEN = 1; // Enable
}

bool USBDeviceClass::detach()
{
	if (!initialized)
		return false;
	usbd.detach();
	return true;
}

bool USBDeviceClass::configured()
{
	return _usbConfiguration != 0;
}

bool USBDeviceClass::handleClassInterfaceSetup(USBSetup& setup)
{
	uint8_t i = setup.wIndex;

	#if defined(CDC_ENABLED)
	if (CDC_ACM_INTERFACE == i)
	{
		if (CDC_Setup(setup) == false) {
			sendZlp(0);
		}
		return true;
	}
	#endif

	#if defined(PLUGGABLE_USB_ENABLED)
	bool ret = PluggableUSB().setup(setup);
	if ( ret == false) {
		sendZlp(0);
	}
	return ret;
	#endif

	return false;
}

uint32_t EndPoints[] =
{
	USB_ENDPOINT_TYPE_CONTROL,

#ifdef CDC_ENABLED
	USB_ENDPOINT_TYPE_INTERRUPT | USB_ENDPOINT_IN(0),           // CDC_ENDPOINT_ACM
	USB_ENDPOINT_TYPE_BULK      | USB_ENDPOINT_OUT(0),               // CDC_ENDPOINT_OUT
	USB_ENDPOINT_TYPE_BULK | USB_ENDPOINT_IN(0),                // CDC_ENDPOINT_IN
#endif

#ifdef PLUGGABLE_USB_ENABLED
	//allocate 6 endpoints and remove const so they can be changed by the user
	0,
	0,
	0,
	0,
	0,
	0,
#endif
};

void USBDeviceClass::initEndpoints() {
	for (uint8_t i = 1; i < sizeof(EndPoints) && EndPoints[i] != 0; i++) {
		initEP(i, EndPoints[i]);
	}
}

void USBDeviceClass::initEP(uint32_t ep, uint32_t config)
{
	if (config == (USB_ENDPOINT_TYPE_INTERRUPT | USB_ENDPOINT_IN(0)))
	{
		usbd.epBank1SetSize(ep, 64);
		usbd.epBank1SetAddress(ep, &udd_ep_in_cache_buffer[ep]);
		usbd.epBank1SetType(ep, 4); // INTERRUPT IN
	}
	else if (config == (USB_ENDPOINT_TYPE_BULK | USB_ENDPOINT_OUT(0)))
	{
		if (epHandlers[ep] == NULL) {
			epHandlers[ep] = new DoubleBufferedEPOutHandler(usbd, ep, 256);
		}
	}
	else if (config == (USB_ENDPOINT_TYPE_BULK | USB_ENDPOINT_IN(0)))
	{
		usbd.epBank1SetSize(ep, 64);
		usbd.epBank1SetAddress(ep, &udd_ep_in_cache_buffer[ep]);

		// NAK on endpoint IN, the bank is not yet filled in.
		usbd.epBank1ResetReady(ep);

		usbd.epBank1SetType(ep, 3); // BULK IN
	}
	else if (config == USB_ENDPOINT_TYPE_CONTROL)
	{
		// Setup Control OUT
		usbd.epBank0SetSize(ep, 64);
		usbd.epBank0SetAddress(ep, &udd_ep_out_cache_buffer[ep]);
		usbd.epBank0SetType(ep, 1); // CONTROL OUT / SETUP

		// Setup Control IN
		usbd.epBank1SetSize(ep, 64);
		usbd.epBank1SetAddress(ep, &udd_ep_in_cache_buffer[0]);
		usbd.epBank1SetType(ep, 1); // CONTROL IN

		// Release OUT EP
		usbd.epBank0SetMultiPacketSize(ep, 64);
		usbd.epBank0SetByteCount(ep, 0);

		// NAK on endpoint OUT, the bank is full.
		usbd.epBank0SetReady(ep);
	}
}

void USBDeviceClass::flush(uint32_t ep)
{
	if (available(ep)) {
		// RAM buffer is full, we can send data (IN)
		usbd.epBank1SetReady(ep);

	 	// Clear the transfer complete flag
		usbd.epBank1AckTransferComplete(ep);
	}
}

void USBDeviceClass::stall(uint32_t ep)
{
	// TODO: test
	// TODO: use .bit. notation

	// Stall endpoint
	USB->DEVICE.DeviceEndpoint[ep].EPSTATUSSET.reg = USB_DEVICE_EPSTATUSSET_STALLRQ(2);
}

bool USBDeviceClass::connected()
{
	// Count frame numbers
	uint8_t f = USB->DEVICE.FNUM.bit.FNUM;
	//delay(3);
	return f != USB->DEVICE.FNUM.bit.FNUM;
}


uint32_t USBDeviceClass::recvControl(void *_data, uint32_t len)
{
	uint8_t *data = reinterpret_cast<uint8_t *>(_data);

	// The RAM Buffer is empty: we can receive data
	usbd.epBank0ResetReady(0);

	//usbd.epBank0AckSetupReceived(0);
	uint32_t read = armRecvCtrlOUT(0);
	if (read > len)
		read = len;
	//while (!usbd.epBank0AckTransferComplete(0)) {}
	uint8_t *buffer = udd_ep_out_cache_buffer[0];
	for (uint32_t i=0; i<len; i++) {
		data[i] = buffer[i];
	}

	return read;
}

// Number of bytes, assumes a rx endpoint
uint32_t USBDeviceClass::available(uint32_t ep)
{
	if (epHandlers[ep]) {
		return epHandlers[ep]->available();
	} else {
		return usbd.epBank0ByteCount(ep);
	}
}

// Non Blocking receive
// Return number of bytes read
uint32_t USBDeviceClass::recv(uint32_t ep, void *_data, uint32_t len)
{
	if (!_usbConfiguration)
		return -1;

#ifdef PIN_LED_RXL
	if (rxLEDPulse == 0)
		digitalWrite(PIN_LED_RXL, LOW);

	rxLEDPulse = TX_RX_LED_PULSE_MS;
#endif

	if (epHandlers[ep]) {
		return epHandlers[ep]->recv(_data, len);
	}

	if (available(ep) < len)
		len = available(ep);

	armRecv(ep);

	usbd.epBank0DisableTransferComplete(ep);

	memcpy(_data, udd_ep_out_cache_buffer[ep], len);

	// release empty buffer
	if (len && !available(ep)) {
		// The RAM Buffer is empty: we can receive data
		usbd.epBank0ResetReady(ep);

		// Clear Transfer complete 0 flag
		usbd.epBank0AckTransferComplete(ep);

		// Enable Transfer complete 0 interrupt
		usbd.epBank0EnableTransferComplete(ep);
	}

	return len;
}

// Recv 1 byte if ready
int USBDeviceClass::recv(uint32_t ep)
{
	uint8_t c;
	if (recv(ep, &c, 1) != 1) {
		return -1;
	} else {
		return c;
	}
}

uint8_t USBDeviceClass::armRecvCtrlOUT(uint32_t ep)
{
	// Get endpoint configuration from setting register
	usbd.epBank0SetAddress(ep, &udd_ep_out_cache_buffer[ep]);
	usbd.epBank0SetMultiPacketSize(ep, 8);
	usbd.epBank0SetByteCount(ep, 0);

	usbd.epBank0ResetReady(ep);

	// Wait OUT
	while (!usbd.epBank0IsReady(ep)) {}
	while (!usbd.epBank0IsTransferComplete(ep)) {}
	return usbd.epBank0ByteCount(ep);
}

uint8_t USBDeviceClass::armRecv(uint32_t ep)
{
	uint16_t count = usbd.epBank0ByteCount(ep);
	if (count >= 64) {
		usbd.epBank0SetByteCount(ep, count - 64);
	} else {
		usbd.epBank0SetByteCount(ep, 0);
	}
	return usbd.epBank0ByteCount(ep);
}

// Timeout for sends
#define TX_TIMEOUT_MS 70

static char LastTransmitTimedOut[7] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

// Blocking Send of data to an endpoint
uint32_t USBDeviceClass::send(uint32_t ep, const void *data, uint32_t len)
{
	uint32_t written = 0;
	uint32_t length = 0;

	if (!_usbConfiguration)
		return -1;
	if (len > 16384)
		return -1;

#ifdef PIN_LED_TXL
	if (txLEDPulse == 0)
		digitalWrite(PIN_LED_TXL, LOW);

	txLEDPulse = TX_RX_LED_PULSE_MS;
#endif

	// Flash area
	while (len != 0)
	{
		if (usbd.epBank1IsReady(ep)) {
			// previous transfer is still not complete

			// convert the timeout from microseconds to a number of times through
			// the wait loop; it takes (roughly) 23 clock cycles per iteration.
			uint32_t timeout = microsecondsToClockCycles(TX_TIMEOUT_MS * 1000) / 23;

			// Wait for (previous) transfer to complete
			// inspired by Paul Stoffregen's work on Teensy
			while (!usbd.epBank1IsTransferComplete(ep)) {
				if (LastTransmitTimedOut[ep] || timeout-- == 0) {
					LastTransmitTimedOut[ep] = 1;

					// set byte count to zero, so that ZLP is sent
					// instead of stale data
					usbd.epBank1SetByteCount(ep, 0);
					return -1;
				}
			}
		}

		LastTransmitTimedOut[ep] = 0;

		if (len >= EPX_SIZE) {
			length = EPX_SIZE - 1;
		} else {
			length = len;
		}

		/* memcopy could be safer in multi threaded environment */
		memcpy(&udd_ep_in_cache_buffer[ep], data, length);

		usbd.epBank1SetAddress(ep, &udd_ep_in_cache_buffer[ep]);
		usbd.epBank1SetByteCount(ep, length);

		// Clear the transfer complete flag
		usbd.epBank1AckTransferComplete(ep);

		// RAM buffer is full, we can send data (IN)
		usbd.epBank1SetReady(ep);

		written += length;
		len -= length;
		data = (char *)data + length;
	}
	return written;
}

uint32_t USBDeviceClass::armSend(uint32_t ep, const void* data, uint32_t len)
{
	memcpy(&udd_ep_in_cache_buffer[ep], data, len);

	// Get endpoint configuration from setting register
	usbd.epBank1SetAddress(ep, &udd_ep_in_cache_buffer[ep]);
	usbd.epBank1SetMultiPacketSize(ep, 0);
	usbd.epBank1SetByteCount(ep, len);

	return len;
}

uint32_t USBDeviceClass::sendControl(const void* _data, uint32_t len)
{
	const uint8_t *data = reinterpret_cast<const uint8_t *>(_data);
	uint32_t length = len;
	uint32_t sent = 0;
	uint32_t pos = 0;

	if (_dry_run == true)
		return length;

	if (_pack_message == true) {
		memcpy(&_pack_buffer[_pack_size], data, len);
		_pack_size += len;
		return length;
	}

 	while (len > 0)
 	{
		sent = armSend(EP0, data + pos, len);
		pos += sent;
		len -= sent;
 	}

	return length;
}

void USBDeviceClass::sendZlp(uint32_t ep)
{
	// Set the byte count as zero
	usbd.epBank1SetByteCount(ep, 0);
}

bool USBDeviceClass::handleStandardSetup(USBSetup &setup)
{
	switch (setup.bRequest) {
	case GET_STATUS:
		if (setup.bmRequestType == 0)  // device
		{
			// Send the device status
			// TODO: Check current configuration for power mode (if device is configured)
			// TODO: Check if remote wake-up is enabled
			uint8_t buff[] = { 0, 0 };
			armSend(0, buff, 2);
			return true;
		}
		// if( setup.bmRequestType == 2 ) // Endpoint:
		else
		{
			// Send the endpoint status
			// Check if the endpoint if currently halted
			uint8_t buff[] = { 0, 0 };
			if (isEndpointHalt == 1)
				buff[0] = 1;
			armSend(0, buff, 2);
			return true;
		}

	case CLEAR_FEATURE:
		// Check which is the selected feature
		if (setup.wValueL == 1) // DEVICEREMOTEWAKEUP
		{
			// Enable remote wake-up and send a ZLP
			uint8_t buff[] = { 0, 0 };
			if (isRemoteWakeUpEnabled == 1)
				buff[0] = 1;
			armSend(0, buff, 2);
			return true;
		}
		else // if( setup.wValueL == 0) // ENDPOINTHALT
		{
			isEndpointHalt = 0;
			sendZlp(0);
			return true;
		}

	case SET_FEATURE:
		// Check which is the selected feature
		if (setup.wValueL == 1) // DEVICEREMOTEWAKEUP
		{
			// Enable remote wake-up and send a ZLP
			isRemoteWakeUpEnabled = 1;
			uint8_t buff[] = { 0 };
			armSend(0, buff, 1);
			return true;
		}
		if (setup.wValueL == 0) // ENDPOINTHALT
		{
			// Halt endpoint
			isEndpointHalt = 1;
			sendZlp(0);
			return true;
		}

	case SET_ADDRESS:
		setAddress(setup.wValueL);
		return true;

	case GET_DESCRIPTOR:
		return sendDescriptor(setup);

	case SET_DESCRIPTOR:
		return false;

	case GET_CONFIGURATION:
		armSend(0, (void*)&_usbConfiguration, 1);
		return true;

	case SET_CONFIGURATION:
		if (REQUEST_DEVICE == (setup.bmRequestType & REQUEST_RECIPIENT)) {

			initEndpoints();
			_usbConfiguration = setup.wValueL;

			#if defined(CDC_ENABLED)
			// Enable interrupt for CDC reception from host (OUT packet)
			usbd.epBank1EnableTransferComplete(CDC_ENDPOINT_ACM);
			usbd.epBank0EnableTransferComplete(CDC_ENDPOINT_OUT);
			#endif

			sendZlp(0);
			return true;
		} else {
			return false;
		}

	case GET_INTERFACE:
		armSend(0, (void*)&_usbSetInterface, 1);
		return true;

	case SET_INTERFACE:
		_usbSetInterface = setup.wValueL;
		sendZlp(0);
		return true;

	default:
		return true;
	}
}

void USBDeviceClass::ISRHandler()
{

	if (_pack_message == true) {
		return;
	}
	// End-Of-Reset
	if (usbd.isEndOfResetInterrupt())
	{
		// Configure EP 0
		initEP(0, USB_ENDPOINT_TYPE_CONTROL);

		// Enable Setup-Received interrupt
		usbd.epBank0EnableSetupReceived(0);

		_usbConfiguration = 0;

		usbd.ackEndOfResetInterrupt();
	}

	// Start-Of-Frame
	if (usbd.isStartOfFrameInterrupt())
	{
		usbd.ackStartOfFrameInterrupt();

		// check whether the one-shot period has elapsed.  if so, turn off the LED
#ifdef PIN_LED_TXL
		if (txLEDPulse > 0) {
			txLEDPulse--;
			if (txLEDPulse == 0)
				digitalWrite(PIN_LED_TXL, HIGH);
		}
#endif

#ifdef PIN_LED_RXL
		if (rxLEDPulse > 0) {
			rxLEDPulse--;
			if (rxLEDPulse == 0)
				digitalWrite(PIN_LED_RXL, HIGH);
		}
#endif
	}

	// Endpoint 0 Received Setup interrupt
	if (usbd.epBank0IsSetupReceived(0))
	{
		usbd.epBank0AckSetupReceived(0);

		USBSetup *setup = reinterpret_cast<USBSetup *>(udd_ep_out_cache_buffer[0]);

		/* Clear the Bank 0 ready flag on Control OUT */
		// The RAM Buffer is empty: we can receive data
		usbd.epBank0ResetReady(0);

		bool ok;
		if (REQUEST_STANDARD == (setup->bmRequestType & REQUEST_TYPE)) {
			// Standard Requests
			ok = handleStandardSetup(*setup);
		} else {
			// Class Interface Requests
			ok = handleClassInterfaceSetup(*setup);
		}

		if (ok) {
			usbd.epBank1SetReady(0);
		} else {
			stall(0);
		}

		if (usbd.epBank1IsStalled(0))
		{
			usbd.epBank1AckStalled(0);

			// Remove stall request
			usbd.epBank1DisableStalled(0);
		}

	} // end Received Setup handler

	uint8_t i=0;
	uint8_t ept_int = usbd.epInterruptSummary() & 0xFE; // Remove endpoint number 0 (setup)
	while (ept_int != 0)
	{
		// Check if endpoint has a pending interrupt
		if ((ept_int & (1 << i)) != 0)
		{
			// Endpoint Transfer Complete (0/1) Interrupt
			if (usbd.epBank0IsTransferComplete(i) ||
			    usbd.epBank1IsTransferComplete(i))
			{
				if (epHandlers[i]) {
					epHandlers[i]->handleEndpoint();
				} else {
					handleEndpoint(i);
				}
			}
			ept_int &= ~(1 << i);
		}
		i++;
		if (i > USB_EPT_NUM)
			break;  // fire exit
	}
}

/*
 * USB Device instance
 * -------------------
 */

// USBDevice class instance
USBDeviceClass USBDevice;

#endif
