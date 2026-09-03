/*
  RCSwitch - Arduino library for remote control outlet switches
  Copyright (c) 2011 Suat Özgür. All rights reserved.

  Original project:
  https://github.com/sui77/rc-switch/

  PulSar custom version:
  lib/lib_rf_extended/rc-switch-custom

  This copy retains the original rc-switch protocol support and adds
  PulSar-specific protocol extensions and raw OOK transmission helpers.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef RCSWITCH_EXTENDED_H
#define RCSWITCH_EXTENDED_H

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#elif defined(ENERGIA)
    #include "Energia.h"
#elif defined(RPI)
    #define RaspberryPi
    #include <string.h>
    #include <stdlib.h>
    #include <wiringPi.h>
#elif defined(SPARK)
    #include "application.h"
#else
    #include "WProgram.h"
#endif

#include <stdint.h>


// ATTiny X4/X5 builds disable receiving because the required division
// helpers may not be available.
#if defined(__AVR_ATtinyX5__) || defined(__AVR_ATtinyX4__)
#define RCSwitchDisableReceiving
#endif

// Maximum number of captured HIGH/LOW timing changes per packet.
//
// 160 allows the existing extended 64/66-bit and KeeLoq-style receive
// paths to reach the 156-change boundary already handled by the ISR.
#define RCSWITCH_MAX_CHANGES 160

// Minimum duration in microseconds used to identify a separation between
// received frames. Protocols with a shorter sync/separation period may
// require this value to be reduced.
#define RCSWITCH_SEPARATION_LIMIT 4100


class RCSwitch {

  public:
    RCSwitch();

    void switchOn(int nGroupNumber, int nSwitchNumber);
    void switchOff(int nGroupNumber, int nSwitchNumber);
    void switchOn(char sFamily, int nGroup, int nDevice);
    void switchOff(char sFamily, int nGroup, int nDevice);
    void switchOn(const char* sGroup, const char* sDevice);
    void switchOff(const char* sGroup, const char* sDevice);
    void switchOn(char sGroup, int nDevice);
    void switchOff(char sGroup, int nDevice);

    void sendTriState(const char* sCodeWord);
    void send(unsigned long long code, unsigned int length);
    void send(const char* sCodeWord);

    /**
     * Transmit an arbitrary raw OOK waveform.
     *
     * timings[] contains alternating durations in microseconds.
     * The first duration is transmitted using startLevel.
     * The complete waveform is repeated nRepeatTransmit times.
     */
    void sendRaw(
      const uint16_t* timings,
      unsigned int length,
      uint8_t startLevel = HIGH
    );

    /**
     * Transmit an arbitrary raw OOK waveform using integer multiples
     * of a base pulse length.
     *
     * timings[] contains alternating pulse multipliers.
     * Each duration is timings[i] * basePulseLength microseconds.
     */
    void sendRawMultiples(
      const uint16_t* timings,
      unsigned int length,
      uint16_t basePulseLength,
      uint8_t startLevel = HIGH
    );

    uint8_t getNumProtos();

    #if !defined(RCSwitchDisableReceiving)
    void enableReceive(int interrupt);
    void enableReceive();
    void disableReceive();
    bool available();
    void resetAvailable();

    unsigned long long getReceivedValue();
    unsigned int getReceivedBitlength();
    unsigned int getReceivedDelay();
    unsigned int getReceivedProtocol();
    unsigned int* getReceivedRawdata();

    void setReceiveTolerance(int nPercent);
    void setReceiveProtocolMask(unsigned long long mask);

    unsigned long long getReceiveProtocolMask() const
    {
        return nReceiveProtocolMask;
    }

    // Legacy PulSar spelling retained for existing module compatibility.
    unsigned long long GetReceiveProtolMask() const
    {
        return getReceiveProtocolMask();
    }
    #endif

    void enableTransmit(int nTransmitterPin);
    void disableTransmit();
    void setPulseLength(int nPulseLength);
    void setRepeatTransmit(int nRepeatTransmit);

    /**
     * Description of one HIGH/LOW pulse pair.
     *
     * The HIGH state lasts high * pulseLength microseconds and the LOW
     * state lasts low * pulseLength microseconds.
     */
    struct HighLow {
        uint8_t high;
        uint8_t low;
    };

    /**
     * A protocol describes how zero and one bits are encoded into
     * HIGH/LOW pulse pairs, together with optional preamble, header
     * and guard timing.
     */
    struct Protocol {
        uint16_t pulseLength;

        uint8_t PreambleFactor;
        HighLow Preamble;

        uint8_t HeaderFactor;
        HighLow Header;

        HighLow zero;
        HighLow one;

        /**
         * If true, invert the physical logic levels used for all
         * HighLow pulse definitions.
         */
        bool invertedSignal;

        uint16_t Guard;
    };

    void setProtocol(Protocol protocol);
    void setProtocol(int nProtocol);
    void setProtocol(int nProtocol, int nPulseLength);

  private:
    char* getCodeWordA(const char* sGroup, const char* sDevice, bool bStatus);
    char* getCodeWordB(int nGroupNumber, int nSwitchNumber, bool bStatus);
    char* getCodeWordC(char sFamily, int nGroup, int nDevice, bool bStatus);
    char* getCodeWordD(char group, int nDevice, bool bStatus);
    void transmit(HighLow pulses);

    #if !defined(RCSwitchDisableReceiving)
    static void handleInterrupt();
    static bool receiveProtocol(const int p, unsigned int changeCount);
    int nReceiverInterrupt;
    #endif

    int nTransmitterPin;
    int nRepeatTransmit;
    Protocol protocol;

    #if !defined(RCSwitchDisableReceiving)
    static int nReceiveTolerance;
    volatile static unsigned long long nReceivedValue;
    volatile static unsigned long long nReceiveProtocolMask;
    volatile static unsigned int nReceivedBitlength;
    volatile static unsigned int nReceivedDelay;
    volatile static unsigned int nReceivedProtocol;
    const static unsigned int nSeparationLimit;

    // timings[0] contains the sync/separation timing followed by pulse timings.
    static unsigned int timings[RCSWITCH_MAX_CHANGES];

    // Duration buffer for the four most recent captured transitions.
    // buftimings[0] is the newest duration.
    static unsigned int buftimings[4];
    #endif
};


#endif