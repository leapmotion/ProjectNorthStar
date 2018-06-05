/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

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

#include <Arduino.h>
#include <Reset.h> // Needed for auto-reset with 1200bps port touch

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifdef CDC_ENABLED

#define CDC_SERIAL_BUFFER_SIZE	256

/* For information purpose only since RTS is not always handled by the terminal application */
#define CDC_LINESTATE_DTR		0x01 // Data Terminal Ready
#define CDC_LINESTATE_RTS		0x02 // Ready to Send

#define CDC_LINESTATE_READY		(CDC_LINESTATE_RTS | CDC_LINESTATE_DTR)

typedef struct {
	uint32_t dwDTERate;
	uint8_t bCharFormat;
	uint8_t bParityType;
	uint8_t bDataBits;
	uint8_t lineState;
} LineInfo;

_Pragma("pack(1)")
static volatile LineInfo _usbLineInfo = {
	115200, // dWDTERate
	0x00,   // bCharFormat
	0x00,   // bParityType
	0x08,   // bDataBits
	0x00    // lineState
};

static volatile int32_t breakValue = -1;

static CDCDescriptor _cdcInterface = {
	D_IAD(0, 2, CDC_COMMUNICATION_INTERFACE_CLASS, CDC_ABSTRACT_CONTROL_MODEL, 0),

	// CDC communication interface
	D_INTERFACE(CDC_ACM_INTERFACE, 1, CDC_COMMUNICATION_INTERFACE_CLASS, CDC_ABSTRACT_CONTROL_MODEL, 0),
	D_CDCCS(CDC_HEADER, CDC_V1_10 & 0xFF, (CDC_V1_10>>8) & 0x0FF), // Header (1.10 bcd)

	D_CDCCS4(CDC_ABSTRACT_CONTROL_MANAGEMENT, 6), // SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported
	D_CDCCS(CDC_UNION, CDC_ACM_INTERFACE, CDC_DATA_INTERFACE), // Communication interface is master, data interface is slave 0
	D_CDCCS(CDC_CALL_MANAGEMENT, 1, 1), // Device handles call management (not)
	D_ENDPOINT(USB_ENDPOINT_IN(CDC_ENDPOINT_ACM), USB_ENDPOINT_TYPE_INTERRUPT, 0x10, 0x10),

	// CDC data interface
	D_INTERFACE(CDC_DATA_INTERFACE, 2, CDC_DATA_INTERFACE_CLASS, 0, 0),
	D_ENDPOINT(USB_ENDPOINT_OUT(CDC_ENDPOINT_OUT), USB_ENDPOINT_TYPE_BULK, EPX_SIZE, 0),
	D_ENDPOINT(USB_ENDPOINT_IN (CDC_ENDPOINT_IN ), USB_ENDPOINT_TYPE_BULK, EPX_SIZE, 0)
};
_Pragma("pack()")

const void* _CDC_GetInterface(void)
{
	return &_cdcInterface;
}

uint32_t _CDC_GetInterfaceLength(void)
{
	return sizeof(_cdcInterface);
}

int CDC_GetInterface(uint8_t* interfaceNum)
{
	interfaceNum[0] += 2;	// uses 2
	return USBDevice.sendControl(&_cdcInterface,sizeof(_cdcInterface));
}

bool CDC_Setup(USBSetup& setup)
{
	uint8_t requestType = setup.bmRequestType;
	uint8_t r = setup.bRequest;

	if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE)
	{
		if (r == CDC_GET_LINE_CODING)
		{
			USBDevice.sendControl((void*)&_usbLineInfo, 7);
			return true;
		}
	}

	if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE)
	{
		if (r == CDC_SET_LINE_CODING)
		{
			USBDevice.recvControl((void*)&_usbLineInfo, 7);
		}

		if (r == CDC_SET_CONTROL_LINE_STATE)
		{
			_usbLineInfo.lineState = setup.wValueL;
		}

		if (r == CDC_SET_LINE_CODING || r == CDC_SET_CONTROL_LINE_STATE)
		{
			// auto-reset into the bootloader is triggered when the port, already
			// open at 1200 bps, is closed. We check DTR state to determine if host 
			// port is open (bit 0 of lineState).
			if (_usbLineInfo.dwDTERate == 1200 && (_usbLineInfo.lineState & 0x01) == 0)
			{
				initiateReset(250);
			}
			else
			{
				cancelReset();
			}
			return false;
		}

		if (CDC_SEND_BREAK == r)
		{
			breakValue = ((uint16_t)setup.wValueH << 8) | setup.wValueL;
			return false;
		}
	}
	return false;
}

void Serial_::begin(uint32_t /* baud_count */)
{
	// uart config is ignored in USB-CDC
}

void Serial_::begin(uint32_t /* baud_count */, uint8_t /* config */)
{
	// uart config is ignored in USB-CDC
}

void Serial_::end(void)
{
}

int Serial_::available(void)
{
	return usb.available(CDC_ENDPOINT_OUT);
}

int Serial_::availableForWrite(void)
{
	// return the number of bytes left in the current bank,
	// always EP size - 1, because bank is flushed on every write
	return (EPX_SIZE - 1);
}

int _serialPeek = -1;

int Serial_::peek(void)
{
	if (_serialPeek != -1)
		return _serialPeek;
	_serialPeek = read();
	return _serialPeek;
}

int Serial_::read(void)
{
	if (_serialPeek != -1) {
		int res = _serialPeek;
		_serialPeek = -1;
		return res;
	}
	return usb.recv(CDC_ENDPOINT_OUT);
}

size_t Serial_::readBytes(char *buffer, size_t length)
{
	size_t count = 0;
	_startMillis = millis();
	while (count < length)
	{
		uint32_t n = usb.recv(CDC_ENDPOINT_OUT, buffer+count, length-count);
		if (n == 0 && (millis() - _startMillis) >= _timeout)
			break;
		count += n;
	}
	return count;
}

void Serial_::flush(void)
{
	usb.flush(CDC_ENDPOINT_IN);
}

size_t Serial_::write(const uint8_t *buffer, size_t size)
{
	uint32_t r = usb.send(CDC_ENDPOINT_IN, buffer, size);

	if (r > 0) {
		return r;
	} else {
		setWriteError();
		return 0;
	}
}

size_t Serial_::write(uint8_t c) {
	return write(&c, 1);
}

// This operator is a convenient way for a sketch to check whether the
// port has actually been configured and opened by the host (as opposed
// to just being connected to the host).  It can be used, for example, in
// setup() before printing to ensure that an application on the host is
// actually ready to receive and display the data.
// We add a short delay before returning to fix a bug observed by Federico
// where the port is configured (lineState != 0) but not quite opened.
Serial_::operator bool()
{
	// this is here to avoid spurious opening after upload
	if (millis() < 500)
		return false;

	bool result = false;

	if (_usbLineInfo.lineState > 0)
	{
		result = true;
	}

	delay(10);
	return result;
}

int32_t Serial_::readBreak() {
	uint8_t enableInterrupts = ((__get_PRIMASK() & 0x1) == 0);

	// disable interrupts,
	// to avoid clearing a breakValue that might occur 
	// while processing the current break value
	__disable_irq();

	int32_t ret = breakValue;

	breakValue = -1;

	if (enableInterrupts) {
		// re-enable the interrupts
		__enable_irq();
	}

	return ret;
}

unsigned long Serial_::baud() {
	return _usbLineInfo.dwDTERate;
}

uint8_t Serial_::stopbits() {
	return _usbLineInfo.bCharFormat;
}

uint8_t Serial_::paritytype() {
	return _usbLineInfo.bParityType;
}

uint8_t Serial_::numbits() {
	return _usbLineInfo.bDataBits;
}

bool Serial_::dtr() {
	return _usbLineInfo.lineState & 0x1;
}

bool Serial_::rts() {
	return _usbLineInfo.lineState & 0x2;
}

Serial_ SerialUSB(USBDevice);

#endif
