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

#include "Tone.h"
#include "variant.h"

#define WAIT_TC16_REGS_SYNC(x) while(x->COUNT16.STATUS.bit.SYNCBUSY);

uint32_t toneMaxFrequency = F_CPU / 2;
uint32_t lastOutputPin = 0xFFFFFFFF;

volatile uint32_t *portToggleRegister;
volatile uint32_t *portClearRegister;
volatile uint32_t portBitMask;
volatile int64_t toggleCount;
volatile bool toneIsActive = false;
volatile bool firstTimeRunning = false;

#define TONE_TC         TC5
#define TONE_TC_IRQn    TC5_IRQn
#define TONE_TC_TOP     0xFFFF
#define TONE_TC_CHANNEL 0

void TC5_Handler (void) __attribute__ ((weak, alias("Tone_Handler")));

static inline void resetTC (Tc* TCx)
{
  // Disable TCx
  TCx->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  WAIT_TC16_REGS_SYNC(TCx)

  // Reset TCx
  TCx->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  WAIT_TC16_REGS_SYNC(TCx)
  while (TCx->COUNT16.CTRLA.bit.SWRST);
}

void toneAccurateClock (uint32_t accurateSystemCoreClockFrequency)
{
  toneMaxFrequency = accurateSystemCoreClockFrequency / 2;
}

void tone (uint32_t outputPin, uint32_t frequency, uint32_t duration)
{
  // Configure interrupt request
  NVIC_DisableIRQ(TONE_TC_IRQn);
  NVIC_ClearPendingIRQ(TONE_TC_IRQn);
  
  if(!firstTimeRunning)
  {
    firstTimeRunning = true;
    
    NVIC_SetPriority(TONE_TC_IRQn, 0);
      
    // Enable GCLK for TC4 and TC5 (timer counter input clock)
    GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5));
    while (GCLK->STATUS.bit.SYNCBUSY);
  }
  
  if (toneIsActive && (outputPin != lastOutputPin))
    noTone(lastOutputPin);

  //
  // Calculate best prescaler divider and comparator value for a 16 bit TC peripheral
  //

  uint32_t prescalerConfigBits;
  uint32_t ccValue;

  ccValue = toneMaxFrequency / frequency - 1;
  prescalerConfigBits = TC_CTRLA_PRESCALER_DIV1;
  
  uint8_t i = 0;
  
  while(ccValue > TONE_TC_TOP)
  {
    ccValue = toneMaxFrequency / frequency / (2<<i) - 1;
    i++;
    if(i == 4 || i == 6 || i == 8) //DIV32 DIV128 and DIV512 are not available
     i++;
  }
  
  switch(i-1)
  {
    case 0: prescalerConfigBits = TC_CTRLA_PRESCALER_DIV2; break;
    
    case 1: prescalerConfigBits = TC_CTRLA_PRESCALER_DIV4; break;
    
    case 2: prescalerConfigBits = TC_CTRLA_PRESCALER_DIV8; break;
    
    case 3: prescalerConfigBits = TC_CTRLA_PRESCALER_DIV16; break;
    
    case 5: prescalerConfigBits = TC_CTRLA_PRESCALER_DIV64; break;
      
    case 7: prescalerConfigBits = TC_CTRLA_PRESCALER_DIV256; break;
    
    case 9: prescalerConfigBits = TC_CTRLA_PRESCALER_DIV1024; break;
    
    default: break;
  }

  toggleCount = (duration > 0 ? frequency * duration * 2 / 1000UL : -1);

  resetTC(TONE_TC);

  uint16_t tmpReg = 0;
  tmpReg |= TC_CTRLA_MODE_COUNT16;  // Set Timer counter Mode to 16 bits
  tmpReg |= TC_CTRLA_WAVEGEN_MFRQ;  // Set TONE_TC mode as match frequency
  tmpReg |= prescalerConfigBits;
  TONE_TC->COUNT16.CTRLA.reg |= tmpReg;
  WAIT_TC16_REGS_SYNC(TONE_TC)

  TONE_TC->COUNT16.CC[TONE_TC_CHANNEL].reg = (uint16_t) ccValue;
  WAIT_TC16_REGS_SYNC(TONE_TC)

  portToggleRegister = &(PORT->Group[g_APinDescription[outputPin].ulPort].OUTTGL.reg);
  portClearRegister = &(PORT->Group[g_APinDescription[outputPin].ulPort].OUTCLR.reg);
  portBitMask = (1ul << g_APinDescription[outputPin].ulPin);

  // Enable the TONE_TC interrupt request
  TONE_TC->COUNT16.INTENSET.bit.MC0 = 1;
  
  if (outputPin != lastOutputPin)
  {
    lastOutputPin = outputPin;
    digitalWrite(outputPin, LOW);
    pinMode(outputPin, OUTPUT);
    toneIsActive = true;
  }

  // Enable TONE_TC
  TONE_TC->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
  WAIT_TC16_REGS_SYNC(TONE_TC)
  
  NVIC_EnableIRQ(TONE_TC_IRQn);
}

void noTone (uint32_t outputPin)
{
  resetTC(TONE_TC);
  digitalWrite(outputPin, LOW);
  toneIsActive = false;
}

#ifdef __cplusplus
extern "C" {
#endif

void Tone_Handler (void)
{
  if (toggleCount != 0)
  {
    // Toggle the ouput pin
    *portToggleRegister = portBitMask;

    if (toggleCount > 0)
      --toggleCount;

    // Clear the interrupt
    TONE_TC->COUNT16.INTFLAG.bit.MC0 = 1;
  }
  else
  {
    resetTC(TONE_TC);
    *portClearRegister = portBitMask;
    toneIsActive = false;
  }
}

#ifdef __cplusplus
}
#endif
