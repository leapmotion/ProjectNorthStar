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

#include "HardwareSerial.h"
#include "SERCOM.h"
#include "RingBuffer.h"

#include <cstddef>

class Uart : public HardwareSerial
{
  public:
    Uart(SERCOM *_s, uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX);
    Uart(SERCOM *_s, uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX, uint8_t _pinRTS, uint8_t _pinCTS);
    void begin(unsigned long baudRate);
    void begin(unsigned long baudrate, uint16_t config);
    void end();
    int available();
    int availableForWrite();
    int peek();
    int read();
    void flush();
    size_t write(const uint8_t data);
    using Print::write; // pull in write(str) and write(buf, size) from Print

    void IrqHandler();

    operator bool() { return true; }

  private:
    SERCOM *sercom;
    RingBuffer rxBuffer;
    RingBuffer txBuffer;

    uint8_t uc_pinRX;
    uint8_t uc_pinTX;
    SercomRXPad uc_padRX;
    SercomUartTXPad uc_padTX;
    uint8_t uc_pinRTS;
    volatile uint32_t* pul_outsetRTS;
    volatile uint32_t* pul_outclrRTS;
    uint32_t ul_pinMaskRTS;
    uint8_t uc_pinCTS;

    SercomNumberStopBit extractNbStopBit(uint16_t config);
    SercomUartCharSize extractCharSize(uint16_t config);
    SercomParityMode extractParity(uint16_t config);
};
