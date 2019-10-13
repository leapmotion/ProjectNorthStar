/**
* @file tps61194.cpp
*
* @brief TPS61194 driver
*
* @copyright
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* @copyright
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* @author Adam Munich
* @author Jim Newman
*
* Please note that this driver is a heavily redacted version of Lib_LP8545.
*
*/

// ##################################################################################
// Includes
// ----------------------------------------------------------------------------------
#include "./tps61194.h"
#include "../pin_settings.h"

#include <wiring_private.h>
#include <Wire.h>


// ##################################################################################
// Macros used in this CPP file
// ----------------------------------------------------------------------------------
#ifndef VARIANT_MCK
#error Must define CPU frequency in VARIANT_MCK
#endif


// ##################################################################################
// Global variables
// ----------------------------------------------------------------------------------

#ifdef _VARIANT_ARDUINO_ZERO_

// PWM generation timers
EPioType g_TPS_pinPwm_EPioType;
bool g_TPS_pinPwm_oneShotModeEnabled;
uint8_t g_TPS_pinPwm_tcChannel;
TPS_pinPwm_timerType_t g_TPS_pinPwm_timerType;
Tc * g_TPS_pinPwm_TCx;
Tcc * g_TPS_pinPwm_TCCx;

// Choose vsync timer (TCC2 is not used by important pins)
Tcc * g_TPS_vsyncTCC = TCC2;
IRQn g_TPS_vsyncTCC_handler = TCC2_IRQn;
bool g_TPS_vsyncTCC_isRunning = false;
#endif

// Pins
typedef struct
{
	uint32_t pwmPin;
	uint32_t vsyncIrqPin;
	uint32_t backlightEnablePin;
} pinId_t;

pinId_t g_TPS_pinId;

// Brightness state
typedef struct 
{
	int16_t currentValue;
	int16_t lastBrightnessSetting;
} brightnessState_t;

brightnessState_t g_TPS_brightnessState;


// ##################################################################################
// Function definitions
// ----------------------------------------------------------------------------------
#ifdef _VARIANT_ARDUINO_ZERO_

static void setTCXx_CCx(uint16_t brightnessValue)
{
	switch(g_TPS_pinPwm_timerType)
	{
		case pinPwm_timerType::timerTypeTCC:
		{
			g_TPS_pinPwm_TCCx->CC[g_TPS_pinPwm_tcChannel].reg = g_TPS_cie_12bit[brightnessValue];
			while (g_TPS_pinPwm_TCCx->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);
			break;
		}
		case pinPwm_timerType::timerTypeTC:
		{
			g_TPS_pinPwm_TCx->COUNT8.CC[g_TPS_pinPwm_tcChannel].reg = g_TPS_cie_8bit[brightnessValue];
			while (g_TPS_pinPwm_TCx->COUNT8.STATUS.bit.SYNCBUSY);
			break;
		}
	}
}

void TCC2_Handler(void)
{
	// If this interrupt is due to the compare register matching the timer count
	// we toggle the LED.
	
	// Capture compare 0
	if (g_TPS_vsyncTCC->INTFLAG.bit.MC0 == 1)
	{
		g_TPS_vsyncTCC->INTFLAG.bit.MC0 = 1;
		pinPeripheral(g_TPS_pinId.pwmPin, g_TPS_pinPwm_EPioType);
	}

	// Capture compare 1
	if (g_TPS_vsyncTCC->INTFLAG.bit.MC1 == 1)
	{
		g_TPS_vsyncTCC->INTFLAG.bit.MC1 = 1;
		
		if(g_TPS_pinPwm_oneShotModeEnabled)
		{
			g_TPS_vsyncTCC->CTRLBSET.reg = TCC_CTRLBSET_CMD_STOP;
		}
		pinMode(g_TPS_pinId.pwmPin, OUTPUT);
		digitalWrite(g_TPS_pinId.pwmPin, LOW);
	}
}

void VSYNC_IRQ(void)
{
	if(g_TPS_vsyncTCC_isRunning)
	{
		g_TPS_vsyncTCC->CTRLBSET.reg = TCC_CTRLBSET_CMD_STOP;
		g_TPS_vsyncTCC->CTRLBSET.reg = TCC_CTRLBSET_CMD_RETRIGGER;
	}
	else
	{
		g_TPS_vsyncTCC->CTRLA.bit.ENABLE = 1;
		g_TPS_vsyncTCC_isRunning = true;
	}
}
#endif


// ##################################################################################
// TPS61194_ Class
// ----------------------------------------------------------------------------------

TPS61194_::TPS61194_(void)
{
}

TPS61194_::TPS61194_(uint32_t pwmPin, uint32_t vsyncInterruptPin, uint32_t backlightEnablePin)
{
	// Set defaults
	g_TPS_brightnessState.currentValue = 0;
	g_TPS_brightnessState.lastBrightnessSetting = 0;
	
	// Set pins
	g_TPS_pinId.pwmPin = pwmPin;
	g_TPS_pinId.vsyncIrqPin = vsyncInterruptPin;
	g_TPS_pinId.backlightEnablePin = backlightEnablePin;
	
	// Pin modes
	pinMode(g_TPS_pinId.pwmPin, OUTPUT);
	pinMode(g_TPS_pinId.vsyncIrqPin, INPUT_PULLUP);
	pinMode(g_TPS_pinId.backlightEnablePin, OUTPUT);

	// Pin default states
	digitalWrite(g_TPS_pinId.pwmPin, LOW);
	digitalWrite(g_TPS_pinId.backlightEnablePin, LOW);

	// Interrupt for one-shot		
	#ifdef _VARIANT_ARDUINO_ZERO_	
	attachInterrupt(g_TPS_pinId.vsyncIrqPin, VSYNC_IRQ, RISING);
	#endif

	// Disable output
	powerOff();
}

TPS61194_ :: ~TPS61194_(void)
{
	powerOff();
	// Detach NVIC
	detachInterrupt(PIN_backlight_vsync_in);
}

void TPS61194_ :: setup(TPS_config_vsyncType_t vsyncType, uint8_t vsyncFrequency, uint16_t vsyncStartDelay, uint16_t vsyncPulseLength)
{
	bool startNow;
	
	// Vsync Type
	switch(vsyncType)
	{
		case vsyncModeSetup::vsyncModeOneshot:
		{
			g_TPS_pinPwm_oneShotModeEnabled = true;
			g_TPS_vsyncTCC_isRunning = true;
			startNow = true;
			break;			
		}
		case vsyncModeSetup::vsyncModeAstableTriggered:
		{
			g_TPS_pinPwm_oneShotModeEnabled = false;
			g_TPS_vsyncTCC_isRunning = false;
			startNow = false;
			break;			
		}
		case vsyncModeSetup::vsyncModeAstableAutostart:
		{
			g_TPS_pinPwm_oneShotModeEnabled = false;
			g_TPS_vsyncTCC_isRunning = true;
			startNow = true;			
			break;
		}
	}
	
	// Start vsync timer
	#ifdef _VARIANT_ARDUINO_ZERO_
		startVsyncTimer(g_TPS_pinPwm_oneShotModeEnabled, vsyncFrequency, vsyncStartDelay, vsyncPulseLength, startNow);
		pinMode(g_TPS_pinId.pwmPin, OUTPUT);
		setPwmPinTimer(g_TPS_pinId.pwmPin, g_TPS_cie_12bit[255]);
	#else // NOT SAMD21	
		pinMode(g_TPS_pinId.pwmPin, OUTPUT);
		analogWrite(g_TPS_pinId.pwmPin, 0);
	#endif
	
	// Start PWM
	#ifdef _VARIANT_ARDUINO_ZERO_
		pinMode(g_TPS_pinId.pwmPin, OUTPUT);
		setPwmPinTimer(g_TPS_pinId.pwmPin, g_TPS_cie_12bit[255]);
	#else // NOT SAMD21
		pinMode(g_TPS_pinId.pwmPin, OUTPUT);
		analogWrite(g_TPS_pinId.pwmPin, 0);
	#endif
}

void TPS61194_ :: brightness(uint16_t pwmVal)
{
	pwmVal = constrain(pwmVal, 0, 512);
	
	g_TPS_brightnessState.currentValue = pwmVal;
	g_TPS_brightnessState.lastBrightnessSetting = pwmVal;
	
	#ifdef _VARIANT_ARDUINO_ZERO_
	setTCXx_CCx(g_TPS_brightnessState.currentValue);
	#else // NOT SAMD21		
	analogWrite(g_TPS_pinId.pwmPin, g_TPS_cie_8bit[g_TPS_brightnessState.currentValue]);
	#endif
}

void TPS61194_ :: powerOn(void)
{
	digitalWrite(g_TPS_pinId.backlightEnablePin, HIGH);
}

void TPS61194_ :: powerOff(void)
{
	digitalWrite(g_TPS_pinId.backlightEnablePin, LOW);
}

void TPS61194_ :: lightOn(void)
{		
	#ifdef _VARIANT_ARDUINO_ZERO_
	
	// Set target and stuff.
	g_TPS_brightnessState.currentValue = g_TPS_brightnessState.lastBrightnessSetting;
		
	// Set target values
	setTCXx_CCx(g_TPS_brightnessState.currentValue);
	#else // NOT SAMD21	
	analogWrite(g_TPS_pinId.pwmPin, g_TPS_cie_8bit[g_TPS_brightnessState.currentValue]);
	#endif
}

void TPS61194_ :: lightOff(void)
{
	g_TPS_brightnessState.currentValue = 0;
	#ifdef _VARIANT_ARDUINO_ZERO_
	setTCXx_CCx(g_TPS_brightnessState.currentValue);
	#else // NOT SAMD21	
	analogWrite(g_TPS_pinId.pwmPin, g_TPS_brightnessState.currentValue);
	#endif
}


// ----------------------------------------------------------------------------------
// Private
// ----------------------------------------------------------------------------------
// Hardware abstraction layer
void TPS61194_ :: setPwmPinTimer(uint32_t pin, uint32_t value)
{
	#ifdef _VARIANT_ARDUINO_ZERO_
	uint32_t attr = g_APinDescription[pin].ulPinAttribute;

	if ((attr & PIN_ATTR_PWM) == PIN_ATTR_PWM)
	{
		uint32_t tcNum = GetTCNumber(g_APinDescription[pin].ulPWMChannel);
		g_TPS_pinPwm_tcChannel = GetTCChannelNumber(g_APinDescription[pin].ulPWMChannel);
		static bool tcEnabled[TCC_INST_NUM+TC_INST_NUM];

		if (attr & PIN_ATTR_TIMER)
		{
			#if !(ARDUINO_SAMD_VARIANT_COMPLIANCE >= 10603)
			// Compatibility for cores based on SAMD core <=1.6.2
			if (g_APinDescription[pin].ulPinType == PIO_TIMER_ALT)
			{
				pinPeripheral(pin, PIO_TIMER_ALT);
				g_TPS_pinPwm_EPioType = PIO_TIMER_ALT;
			}
			else
			#endif
			{
				pinPeripheral(pin, PIO_TIMER);
				g_TPS_pinPwm_EPioType = PIO_TIMER;
			}
		}
		else
		{
			// We suppose that attr has PIN_ATTR_TIMER_ALT bit set...
			pinPeripheral(pin, PIO_TIMER_ALT);
			g_TPS_pinPwm_EPioType = PIO_TIMER_ALT;
		}

		if (!tcEnabled[tcNum])
		{
			tcEnabled[tcNum] = true;

			uint16_t GCLK_CLKCTRL_IDs[] =
			{
				GCLK_CLKCTRL_ID(GCM_TCC0_TCC1), // TCC0
				GCLK_CLKCTRL_ID(GCM_TCC0_TCC1), // TCC1
				GCLK_CLKCTRL_ID(GCM_TCC2_TC3),  // TCC2
				GCLK_CLKCTRL_ID(GCM_TCC2_TC3),  // TC3
				GCLK_CLKCTRL_ID(GCM_TC4_TC5),   // TC4
				GCLK_CLKCTRL_ID(GCM_TC4_TC5),   // TC5
				GCLK_CLKCTRL_ID(GCM_TC6_TC7),   // TC6
				GCLK_CLKCTRL_ID(GCM_TC6_TC7)    // TC7
			};
			
			GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_IDs[tcNum]);
			while (GCLK->STATUS.bit.SYNCBUSY == 1);

			// Set PORT
			if (tcNum >= TCC_INST_NUM)
			{
				// This timer only supports 8 bit periods, sad panda, reduce PWM resolution...
				value = mapResolution(value, 12, 8);
				
				// -- Configure TC
				Tc * TCx = (Tc*) GetTC(g_APinDescription[pin].ulPWMChannel);
				
				// Set global for later reference
				g_TPS_pinPwm_timerType = pinPwm_timerType::timerTypeTC;
				g_TPS_pinPwm_TCx = TCx;
				
				// Disable timer-counter[x]
				TCx->COUNT8.CTRLA.bit.ENABLE = 0;
				while (TCx->COUNT8.STATUS.bit.SYNCBUSY);
				
				// Set timer-counter[x] mode to 8 bits, normal PWM, pre-scaler 1/4
				TCx->COUNT8.CTRLA.reg |= TC_CTRLA_MODE_COUNT8;
				while (TCx->COUNT8.STATUS.bit.SYNCBUSY);
				
				// Set timer-counter[x] as normal PWM
				TCx->COUNT8.CTRLA.reg |= TC_CTRLA_WAVEGEN_NPWM;
				while (TCx->COUNT8.STATUS.bit.SYNCBUSY);

				// Set timer-counter[x] pre-scaler to 1/4
				TCx->COUNT8.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV4;
				while (TCx->COUNT8.STATUS.bit.SYNCBUSY);

				// Set the initial value
				TCx->COUNT8.CC[g_TPS_pinPwm_tcChannel].reg = (uint8_t) value;
				while (TCx->COUNT8.STATUS.bit.SYNCBUSY);
				
				// Set PER to maximum counter value (resolution : 0xFF, 255)
				TCx->COUNT8.PER.reg = 0xFF;
				while (TCx->COUNT8.STATUS.bit.SYNCBUSY);
				
				// Enable timer-counter[x]
				TCx->COUNT8.CTRLA.bit.ENABLE = 1;
				while (TCx->COUNT8.STATUS.bit.SYNCBUSY);
			}
			else
			{
				// This timer supports > 8 bit periods!
				value = mapResolution(value, 12, 12);

				// -- Configure timer-control-counter[x]
				Tcc * TCCx = (Tcc*) GetTC(g_APinDescription[pin].ulPWMChannel);

				// Set timer-control-counter global for later reference
				g_TPS_pinPwm_timerType = pinPwm_timerType::timerTypeTCC;
				g_TPS_pinPwm_TCCx = TCCx;

				// Disable timer-control-counter[x]
				TCCx->CTRLA.bit.ENABLE = 0;
				while (TCCx->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);

				// Set pre-scaler to 1/1
				TCCx->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV1;
				while (TCCx->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);

				// Set timer-control-counter as normal PWM
				TCCx->WAVE.reg |= TCC_WAVE_WAVEGEN_NPWM;
				while (TCCx->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);

				// Set the initial value
				TCCx->CC[g_TPS_pinPwm_tcChannel].reg = (uint32_t) value;
				while (TCCx->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);
				
				// Set PER to maximum counter value (resolution : 12 Bits, 4096)
				TCCx->PER.reg = 0xFFF;
				while (TCCx->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);
				
				// Enable timer-control-counter[x]
				TCCx->CTRLA.bit.ENABLE = 1;
				while (TCCx->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);
			}
		}
	}
	
	#else // NOT SAMD21	
	#error "TPS61194 timer functions only support SAMD21G/J, uncomment to proceed..."
	
	analogWrite(pin, g_TPS_cie_8bit[value]);
	
	#endif
}

void TPS61194_ :: startVsyncTimer(bool oneShotEnabled, uint8_t timerFrequency, uint16_t pulseStart, uint16_t pulseLength, bool startNow)
{
	#ifdef _VARIANT_ARDUINO_ZERO_
	uint16_t GCLK_CLKCTRL_IDx;
	
	if(g_TPS_vsyncTCC == TCC0)
	{
		GCLK_CLKCTRL_IDx = 	GCLK_CLKCTRL_ID(GCM_TCC0_TCC1);
	}
	else if(g_TPS_vsyncTCC == TCC1)
	{
		GCLK_CLKCTRL_IDx = 	GCLK_CLKCTRL_ID(GCM_TCC0_TCC1);
	}
	else if(g_TPS_vsyncTCC == TCC2)
	{
		GCLK_CLKCTRL_IDx = 	GCLK_CLKCTRL_ID(GCM_TCC2_TC3);
	}

	GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_IDx);
	while (GCLK->STATUS.bit.SYNCBUSY == 1);

	g_TPS_vsyncTCC->INTENCLR.bit.MC0 = 1;
	g_TPS_vsyncTCC->INTENCLR.bit.MC1 = 1;

	g_TPS_vsyncTCC->CTRLA.bit.ENABLE = 0;
	
	if(oneShotEnabled)
	{
		g_TPS_vsyncTCC->CTRLBSET.bit.ONESHOT = 1;
		while (g_TPS_vsyncTCC->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);
	}

	// Set pre-scaler to 16
	uint16_t TIMER_PRESCALER_DIV = 16;
	g_TPS_vsyncTCC->CTRLA.reg |= TCC_CTRLA_PRESCALER_DIV16;
	while (g_TPS_vsyncTCC->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);

	// Set config mode
	// @todo test: is this needed?
	g_TPS_vsyncTCC->WAVE.reg |= TCC_WAVE_WAVEGEN_NFRQ;
	while (g_TPS_vsyncTCC->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);

	// Go from microseconds to sysTime
	uint32_t SYSCLOCK_HZ = (uint32_t)VARIANT_MCK;
	uint32_t TIMCLK_Hz = SYSCLOCK_HZ / TIMER_PRESCALER_DIV;
	uint32_t NANOSECONDS_PER_TICK = 1000000000 / TIMCLK_Hz;
	
	// Calculate compare values
	uint16_t pulseEnd = pulseStart + pulseLength;
	uint16_t CC0_value = (uint16_t)((float)(1000)*pulseStart / (float)NANOSECONDS_PER_TICK);
	uint16_t CC1_value = (uint16_t)((float)(1000)*pulseEnd / (float)NANOSECONDS_PER_TICK);
	
	// Calculate period value
	timerFrequency = constrain(timerFrequency, 20, 255);
	uint16_t PER_value = (uint16_t)((TIMCLK_Hz / timerFrequency) - 1);

	// Constrain in case of overflow
	CC1_value = constrain(CC1_value, 1, PER_value);
	CC0_value = constrain(CC0_value, 0, CC1_value);

	// Set timer period
	g_TPS_vsyncTCC->PER.reg = PER_value;
	while (g_TPS_vsyncTCC->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);
	
	// Set capture compare 0
	g_TPS_vsyncTCC->CC[0].reg = CC0_value;
	while (g_TPS_vsyncTCC->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);

	// Set capture compare 1
	g_TPS_vsyncTCC->CC[1].reg = CC1_value;
	while (g_TPS_vsyncTCC->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);

	// Enable match / capture events for CC0, CC1
	g_TPS_vsyncTCC->EVCTRL.bit.MCEO0 = 1;
	g_TPS_vsyncTCC->EVCTRL.bit.MCEO1 = 1;
	
	// Enable the event interrupts
	g_TPS_vsyncTCC->INTENSET.bit.MC0 = 1;
	g_TPS_vsyncTCC->INTENSET.bit.MC1 = 1;

	if(startNow == true)
	{
		NVIC_ClearPendingIRQ(g_TPS_vsyncTCC_handler);
		NVIC_SetPriority(g_TPS_vsyncTCC_handler, 1);
		NVIC_EnableIRQ(g_TPS_vsyncTCC_handler);
	}
	
	g_TPS_vsyncTCC->CTRLA.bit.ENABLE = 1;
	while (g_TPS_vsyncTCC->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);

	if(startNow == false)
	{
		g_TPS_vsyncTCC->CTRLA.bit.ENABLE = 0;
		while (g_TPS_vsyncTCC->SYNCBUSY.reg & TCC_SYNCBUSY_MASK);
		NVIC_ClearPendingIRQ(g_TPS_vsyncTCC_handler);
		NVIC_SetPriority(g_TPS_vsyncTCC_handler, 1);
		NVIC_EnableIRQ(g_TPS_vsyncTCC_handler);
	}
	
	#else // NOT SAD21
	
	#warning "TPS61194 vsync functions only support Arduino Zero"
	
	#endif
}

// ----------------------------------------------------------------------------------
// Misc
uint32_t TPS61194_ :: mapResolution(uint32_t value, uint32_t from, uint32_t to)
{
	if (from == to)
	{
		return value;
	}
	if (from > to)
	{
		return value >> (from-to);
	}
	return value << (to-from);
}
