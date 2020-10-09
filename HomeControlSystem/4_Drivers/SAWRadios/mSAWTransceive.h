#ifndef _MSAWTRANSCEIVE_H
#define _MSAWTRANSCEIVE_H
#define _MSAWTRANSCEIVE_VERSION 4.0
// STABLE
// Basic low level transceive functions, protocol handled elsewhere
// Works with Arduino and ESP32


#include "0_ConfigUser/mUserConfig.h"
#ifdef USE_SAW_TRANSCEIVER


#include "Arduino.h"
#include <stdint.h>


//#define USE_SAWRADIO_LITE // for basic function of transmitting

// #ifdef USE_SAWRADIO_LITE
//   #define SAWSWITCH_MAX_PACKET_LENGTH_BYTES 2 // smallest version
// #else
//  #define SAW_DETECT_KNOWN
#define SAWSWITCH_MAX_PACKET_LENGTH_BYTES 4 // since code is uint64_t

#define PROTOCOL_PLUG 2
#define SAWRADIO_PROTOCOL 2

#define SAWReceiveRadioPin digitalPinToInterrupt(SAW_RADIO_RX_PIN)
#define SAWTransmitRadioPin SAW_RADIO_TX_PIN

#define SAWSWITCH_MAX_PACKET_LENGTH_BITS (8*SAWSWITCH_MAX_PACKET_LENGTH_BYTES) //64
#define SAWRADIO_MAX_CHANGES (SAWSWITCH_MAX_PACKET_LENGTH_BITS*2)+2 // //130 //Number of maximum high/Low changes per packet. We can handle up to (unsigned long) => 32 bit * 2 H/L changes per bit + 2 for sync

typedef unsigned long long codetype; //uint64_t

class mSAWTransceive { // The new transmit or receive functions, super basic low level e.g. cc1101

  public:
    mSAWTransceive();

    void enableReceive(int16_t interrupt);
    void enableReceive();
    void disableReceive();
    bool available();
    void resetAvailable();

    uint64_t getReceivedValue();
    uint16_t getReceivedBitlength();
    uint16_t getReceivedDelay();
    uint16_t getReceivedProtocol();
    uint16_t* getReceivedRawdata();

    void enableTransmit(int16_t nTransmitterPin);
    void disableTransmit();
    void setPulseLength(int16_t nPulseLength);
    void setRepeatTransmit(int16_t nRepeatTransmit);
    void setReceiveTolerance(int16_t nPercent);

    // Description of a single pule, which consists of a high signal
     //* whose duration is "high" times the base pulse length, followed
     // by a low signal lasting "low" times the base pulse length.
     //* Thus, the pulse overall lasts (high+low)*pulseLength */
    struct HighLow {
        uint8_t high;
        uint8_t low;
    };

    /* A "protocol" describes how zero and one bits are encoded into high/low
     * pulses. */
    struct Protocol {
        /** base pulse length in microseconds, e.g. 350 */
        uint16_t pulseLength;

        HighLow syncFactor;
        HighLow zero;
        HighLow one;

        /* If true, interchange high and low logic levels in all transmissions.
         * By default, RCSwitch assumes that any signals it sends or receives
         * can be broken down into pulses which start with a high signal level,
         * followed by a a low signal level. This is e.g. the case for the
         * popular PT 2260 encoder chip, and thus many switches out there. */
        bool invertedSignal;
    };

    void setProtocol(Protocol protocol);
    void setProtocol(int16_t nProtocol);
    void setProtocol(int16_t nProtocol, int16_t nPulseLength);

    char getReceiveEnableState();
    char ReceiveEnableState;

    // Sending Functions
    void send(uint64_t code, uint16_t length);

  private:

    void transmit(HighLow pulses);

    static void handleInterrupt();
    static bool receiveProtocol(const int16_t p, uint16_t changeCount);
    int16_t nReceiverInterrupt;
    int16_t nTransmitterPin;
    int16_t nRepeatTransmit;

    Protocol protocol;

    static int16_t nReceiveTolerance;
    static uint64_t nReceivedValue;
    static uint16_t nReceivedBitlength;
    static uint16_t nReceivedDelay;
    static uint16_t nReceivedProtocol;
    const static uint16_t nSeparationLimit;

    // Large arrays
    static uint16_t timings[SAWRADIO_MAX_CHANGES]; // timings[0] contains sync timing, followed by a number of bits

};

#endif


#endif
