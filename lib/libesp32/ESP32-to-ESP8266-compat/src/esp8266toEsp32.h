/*
 This library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
#pragma once

#ifndef mInterfaceController_H_compat2
#define mInterfaceController_H_compat2 1.1


#ifdef ESP32
// my debug Stuff
#define Serial_Debug1(p) Serial.printf p
#define Serial_DebugX(p)

//
// basics
//
// dummy defines
#define SPIFFS_END (SPI_FLASH_SEC_SIZE * 200)

// #ifndef SETTINGS_LOCATION
// #define SETTINGS_LOCATION SPIFFS_END
// #error "nere"
// #endif
#define SETTINGS_LOCATION 0x300000 //SPIFFS_END

#ifndef SPI_FLASH_SEC_SIZE
#define SPI_FLASH_SEC_SIZE 4096
#endif

#define STR(x) #x
#define XSTR(x) STR(x)

#warning "SETTINGS_LOCATION is " XSTR(SETTINGS_LOCATION)


#include <Esp.h>

/*********************************************************************************************\
 * ESP32 analogWrite emulation support
\*********************************************************************************************/

// #define ENABLE_DEVFEATURE_ANALOG_WRITE_EMULATION_VERSION2


#ifdef ENABLE_DEVFEATURE_ANALOG_WRITE_EMULATION_VERSION2

#define PWM_SUPPORTED_CHANNELS 8
#define PWM_CHANNEL_OFFSET     2   // Webcam uses channel 0, so we offset standard PWM

extern uint8_t _pwm_channel[PWM_SUPPORTED_CHANNELS];// = { 99, 99, 99, 99, 99, 99, 99, 99 };
extern uint32_t _pwm_frequency;// = 977;     // Default 977Hz
// uint8_t _pwm_bit_num;// = 10;         // Default 1023

extern uint8_t _pwm_bit_num;


//
// basics
//
#include <Esp.h>

/*******************************************************************************************\
 * ESP32/S2/S3/C3... PWM analog support
 *
 * The following supersedes Arduino framework and provides more granular control:
 * - fine grained phase control (in addition to duty cycle)
 * - fine control of PWM frequency and resolution per GPIO
 *
 * By default, all PWM are using the same timer called Timer 0.
 * Changes in frequency of resolution apply to all PWM using Timer 0.
 *
 * You can specify a different a different resolution/frequency for
 * specific GPIOs, this will internally assign a new timer to the GPIO.
 * The limit is 3 specific values in addition to the global value.
 *
 * Note: on ESP32-only, there are 2 groups of PWM and 2 groups of timers.
 * Although there are internally 8 timers, to simplifiy management,
 * Timer 4..7 are mirrored from Timer 0..3.
 * So it all happens like if there were only 4 timers and a single group of PWM channels.
\*******************************************************************************************/

extern "C" uint32_t ledcReadFreq2(uint8_t chan);
uint8_t ledcReadResolution(uint8_t chan);

//
// analogAttach - attach a GPIO to a hardware PWM
//
// Calling explcitly analogAttach() allows to specify the `output_invert` flag
// However it is called implicitly if `analogWrite()` is called and the GPIO
// was not yet attached.
//
// Returns: hardware channel number, or -1 if it failed
int32_t analogAttach(uint32_t pin, bool output_invert = false);   // returns the ledc channel, or -1 if failed. This is implicitly called by analogWrite if the channel was not already allocated

//
// analogDetach - detach attached GPIO from a hardware PWM
//
void analogDetach(uint32_t pin);

//
// analogDetachAll - detach all attached GPIOs from a hardware PWM
//
// This solves ghost PWM activity on reconfigured GPIOs after a restart
void analogDetachAll(void);

// change both freq and range
// `0`: set to global value
// `-1`: keep unchanged
// if pin < 0 then change global value for timer 0

//
// analogWriteFreqRange - change the range and/or frequency of a GPIO
//
// `void analogWriteFreqRange(int32_t freq, int32_t range, int32_t pin)`
//
// The range is converted to a number of bits, so range must be a power of 2 minus 1.
// By default, the resolution is 10 bits, i.e. a range of 1023.
//
// Special cases:
// - if `pin < 0`, changes the global value for Timer 0 and all PWM using default
// - if `range == 0` or `freq == 0`, revert to using Timer 0 (i.e. reassign to global values)
// - if `range < 0` or `freq < 0`, keep the previous value unchanged
// - if `pin` is unassigned, silently ignore
void analogWriteFreqRange(int32_t freq, int32_t range, int32_t pin = -1);

//
// analogWriteRange - change the range of PWM
//
// short-cut for:
// `analogWriteFreqRange(-1, range, pin)`
void analogWriteRange(uint32_t range, int32_t pin = -1);

//
// analogWriteFreq - change the frequency of PWM in Hz
//
// short-cut for:
// `analogWriteFreqRange(-1, range, pin)`
void analogWriteFreq(uint32_t freq, int32_t pin = -1);

//
// analogWrite - change the value of PWM
//
// val must be in range.
void analogWrite(uint8_t pin, int val);

// Extended version that also allows to change phase
extern void analogWritePhase(uint8_t pin, uint32_t duty, uint32_t phase = 0);

//
// ledcReadDutyResolution - read the resolution
//
// return -1 if pin is not assigned to ledc
int32_t ledcReadDutyResolution(uint8_t pin);

//
// ledcRead2 - read the value of PWM
//
// return -1 if pin is not assigned to ledc
int32_t ledcRead2(uint8_t pin);

// return the channel assigned to a GPIO, or -1 if none
extern int32_t analogGetChannel2(uint32_t pin);

/*******************************************************************************************\
 * Low-level Timer management
\*******************************************************************************************/
// get the timer number for a GPIO, -1 if not found
int32_t analogGetTimer(uint8_t pin);
int32_t analogGetTimerForChannel(uint8_t chan);

// Get timer resolution (in bits) - default 10
uint8_t analogGetTimerResolution(uint8_t timer);

// Get timer frequency (in Hz) - default 977
uint32_t analogGetTimerFrequency(uint8_t timer);

#endif


#ifndef ENABLE_DEVFEATURE_ANALOG_WRITE_EMULATION_VERSION2

#define PWM_SUPPORTED_CHANNELS 8
#define PWM_CHANNEL_OFFSET     2   // Webcam uses channel 0, so we offset standard PWM

extern uint8_t _pwm_channel[PWM_SUPPORTED_CHANNELS];// = { 99, 99, 99, 99, 99, 99, 99, 99 };
extern uint32_t _pwm_frequency;// = 977;     // Default 977Hz
// uint8_t _pwm_bit_num;// = 10;         // Default 1023

extern uint8_t _pwm_bit_num;

inline uint32_t _analog_pin2chan(uint32_t pin) {
  for (uint32_t channel = 0; channel < PWM_SUPPORTED_CHANNELS; channel++) {
    if ((_pwm_channel[channel] < 99) && (_pwm_channel[channel] == pin)) {
      return channel;
    }
  }
  return 0;
}

inline void _analogWriteFreqRange(void) {
  for (uint32_t channel = 0; channel < PWM_SUPPORTED_CHANNELS; channel++) {
    if (_pwm_channel[channel] < 99) {
//      uint32_t duty = ledcRead(channel + PWM_CHANNEL_OFFSET);
      ledcSetup(channel + PWM_CHANNEL_OFFSET, _pwm_frequency, _pwm_bit_num);
//      ledcWrite(channel + PWM_CHANNEL_OFFSET, duty);
    }
  }
//  Serial.printf("freq - range %d - %d\n",freq,range);
}

// input range is in full range, ledc needs bits
inline uint32_t _analogGetResolution(uint32_t x) {
  uint32_t bits = 0;
  while (x) {
    bits++;
    x >>= 1;
  }
  return bits;
}

inline void analogWriteRange(uint32_t range) {
  _pwm_bit_num = _analogGetResolution(range);
  _analogWriteFreqRange();
}

inline void analogWriteFreq(uint32_t freq) {
  _pwm_frequency = freq;
  _analogWriteFreqRange();
}

inline void analogAttach(uint32_t pin, uint32_t channel) {
  _pwm_channel[channel &7] = pin;
  ledcAttachPin(pin, channel + PWM_CHANNEL_OFFSET);
  ledcSetup(channel + PWM_CHANNEL_OFFSET, _pwm_frequency, _pwm_bit_num);
  Serial.printf("attach %d - %d\n", channel, pin);
}

inline void analogWrite(uint8_t pin, int val)
{
  uint32_t channel = _analog_pin2chan(pin);
  ledcWrite(channel + PWM_CHANNEL_OFFSET, val);
//  Serial.printf("write %d - %d\n",channel,val);
}
#endif

/*********************************************************************************************/

#define INPUT_PULLDOWN_16 INPUT_PULLUP

typedef double real64_t;

//
// Time and Timer
//
#define ETS_UART_INTR_DISABLE()
#define ETS_UART_INTR_ENABLE()

#define ESPhttpUpdate httpUpdate
#define getFlashChipRealSize() getFlashChipSize()

#define os_delay_us ets_delay_us
// Serial minimal type to hold the config

#if !defined(SerialConfig)

// typedef int SerialConfig; 
#endif

typedef int SerConfu8;
//#define analogWrite(a, b)

//
// WS2812
//
#define NeoEsp8266BitBang800KbpsMethod NeoEsp32BitBang800KbpsMethod
//
// UDP
//
//#define PortUdp_writestr(log_data) PortUdp.write((const uint8_t *)(log_data), strlen(log_data))
#define PortUdp_write(log_data, n) PortUdp.write((const uint8_t *)(log_data), n)

//
#define wifi_forceSleepBegin()

#undef LWIP_IPV6

#define REASON_DEFAULT_RST      0  // "Power on"                normal startup by power on
#define REASON_WDT_RST          1  // "Hardware Watchdog"       hardware watch dog reset
#define REASON_EXCEPTION_RST    2  // "Exception"               exception reset, GPIO status won’t change
#define REASON_SOFT_WDT_RST     3  // "Software Watchdog"       software watch dog reset, GPIO status won’t change
#define REASON_SOFT_RESTART     4  // "Software/System restart" software restart ,system_restart , GPIO status won’t change
#define REASON_DEEP_SLEEP_AWAKE 5  // "Deep-Sleep Wake"         wake up from deep-sleep
#define REASON_EXT_SYS_RST      6  // "External System"         external system reset

// memmove ...
#define memcpy_P memcpy
#define memmove_P memmove
#define strncpy_P strncpy
#define strcmp_P strcmp
#define memccpy_P memccpy
#define snprintf_P snprintf
#define sprintf_P sprintf
#define strncmp_P strncmp

// LWIP STuff

#define STATION_IF 0

#endif

#endif  // mInterfaceController_H_compat