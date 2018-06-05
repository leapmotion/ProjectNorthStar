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

#pragma once

#define HSTPIPCFG_PTYPE_BLK 1
#define HSTPIPCFG_PTOKEN_IN 2
#define HSTPIPCFG_PTOKEN_OUT 3
#define HSTPIPCFG_PBK_1_BANK 4
#define HSTPIPCFG_PTYPE_INTRPT 5

#define EP0      0
#define EPX_SIZE 64 // 64 for Full Speed, EPT size max is 1024

#if defined __cplusplus

#include "Stream.h"
#include "RingBuffer.h"

//================================================================================
// USB

// Low level API
typedef struct {
	union {
		uint8_t bmRequestType;
		struct {
			uint8_t direction : 5;
			uint8_t type : 2;
			uint8_t transferDirection : 1;
		};
	};
	uint8_t bRequest;
	uint8_t wValueL;
	uint8_t wValueH;
	uint16_t wIndex;
	uint16_t wLength;
} USBSetup;

class USBDeviceClass {
public:
	USBDeviceClass() {};

	// USB Device API
	void init();
	bool attach();
	bool detach();
	void setAddress(uint32_t addr);

	bool configured();
	bool connected();

	void standby();

	// Setup API
	bool handleClassInterfaceSetup(USBSetup &setup);
	bool handleStandardSetup(USBSetup &setup);
	bool sendDescriptor(USBSetup &setup);

	// Control EndPoint API
	uint32_t sendControl(const void *data, uint32_t len);
	uint32_t sendControl(int /* ep */, const void *data, uint32_t len) { return sendControl(data, len); }
	uint32_t recvControl(void *data, uint32_t len);
	uint32_t sendConfiguration(uint32_t maxlen);
	bool sendStringDescriptor(const uint8_t *string, uint8_t maxlen);
	void initControl(int end);
	uint8_t SendInterfaces(uint32_t* total);
	void packMessages(bool val);

	// Generic EndPoint API
	void initEndpoints(void);
	void initEP(uint32_t ep, uint32_t type);
	void handleEndpoint(uint8_t ep);

	uint32_t send(uint32_t ep, const void *data, uint32_t len);
	void sendZlp(uint32_t ep);
	uint32_t recv(uint32_t ep, void *data, uint32_t len);
	int recv(uint32_t ep);
	uint32_t available(uint32_t ep);
	void flush(uint32_t ep);
	void stall(uint32_t ep);

	// private?
	uint32_t armSend(uint32_t ep, const void *data, uint32_t len);
	uint8_t armRecv(uint32_t ep);
	uint8_t armRecvCtrlOUT(uint32_t ep);

	void ISRHandler();

private:
	bool initialized;
};

extern USBDeviceClass USBDevice;

//================================================================================
//	Serial over CDC (Serial1 is the physical port)

class Serial_ : public Stream
{
public:
	Serial_(USBDeviceClass &_usb) : usb(_usb), stalled(false) { }
	void begin(uint32_t baud_count);
	void begin(unsigned long, uint8_t);
	void end(void);

	virtual int available(void);
	virtual int availableForWrite(void);
	virtual int peek(void);
	virtual int read(void);
	virtual void flush(void);
	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *buffer, size_t size);
	using Print::write; // pull in write(str) from Print
	operator bool();

	size_t readBytes(char *buffer, size_t length);

	// This method allows processing "SEND_BREAK" requests sent by
	// the USB host. Those requests indicate that the host wants to
	// send a BREAK signal and are accompanied by a single uint16_t
	// value, specifying the duration of the break. The value 0
	// means to end any current break, while the value 0xffff means
	// to start an indefinite break.
	// readBreak() will return the value of the most recent break
	// request, but will return it at most once, returning -1 when
	// readBreak() is called again (until another break request is
	// received, which is again returned once).
	// This also mean that if two break requests are received
	// without readBreak() being called in between, the value of the
	// first request is lost.
	// Note that the value returned is a long, so it can return
	// 0-0xffff as well as -1.
	int32_t readBreak();

	// These return the settings specified by the USB host for the
	// serial port. These aren't really used, but are offered here
	// in case a sketch wants to act on these settings.
	uint32_t baud();
	uint8_t stopbits();
	uint8_t paritytype();
	uint8_t numbits();
	bool dtr();
	bool rts();
	enum {
		ONE_STOP_BIT = 0,
		ONE_AND_HALF_STOP_BIT = 1,
		TWO_STOP_BITS = 2,
	};
	enum {
		NO_PARITY = 0,
		ODD_PARITY = 1,
		EVEN_PARITY = 2,
		MARK_PARITY = 3,
		SPACE_PARITY = 4,
	};

private:
	int availableForStore(void);

	USBDeviceClass &usb;
	RingBuffer *_cdc_rx_buffer;
	bool stalled;
};
extern Serial_ SerialUSB;

//================================================================================
//================================================================================
//	MSC 'Driver'

uint32_t		MSC_GetInterface(uint8_t* interfaceNum);
uint32_t		MSC_GetDescriptor(uint32_t i);
bool	MSC_Setup(USBSetup& setup);
bool	MSC_Data(uint8_t rx,uint8_t tx);

//================================================================================
//================================================================================
//	CDC 'Driver'

int CDC_GetInterface(uint8_t* interfaceNum);
const void* _CDC_GetInterface(void);
uint32_t _CDC_GetInterfaceLength(void);
uint32_t		CDC_GetOtherInterface(uint8_t* interfaceNum);
uint32_t		CDC_GetDescriptor(uint32_t i);
bool	CDC_Setup(USBSetup& setup);

#endif  // __cplusplus
