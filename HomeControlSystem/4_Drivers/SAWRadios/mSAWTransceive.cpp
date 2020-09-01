#include "mSAWTransceive.h"


#include "1_ConfigUser/mUserConfig.h"
#ifdef USE_SAW_TRANSCEIVER


mSAWTransceive::mSAWTransceive() {
  this->nTransmitterPin = -1;
  this->setRepeatTransmit(10);
  this->setProtocol(1);
  this->nReceiverInterrupt = -1;
  this->setReceiveTolerance(100);
  mSAWTransceive::nReceivedValue = 0;
}

#if defined(ESP8266) || defined(ESP32)
    // interrupt handler and related code must be in RAM on ESP8266,
    // according to issue #46.
    #define RECEIVE_ATTR ICACHE_RAM_ATTR
#else
    #define RECEIVE_ATTR
#endif


/* Format for protocol definitions: {pulselength, Sync bit, "0" bit, "1" bit}
 * pulselength: pulse length in microseconds, e.g. 350
 * Sync bit: {1, 31} means 1 high pulse and 31 low pulses
 *     (perceived as a 31*pulselength long pulse, total length of sync bit is
 *     32*pulselength microseconds), i.e:
 *      _
 *     | |_______________________________ (don't count the vertical bars)
 * "0" bit: waveform for a data bit of value "0", {1, 3} means 1 high pulse
 *     and 3 low pulses, total length (1+3)*pulselength, i.e:
 *      _
 *     | |___
 * "1" bit: waveform for a data bit of value "1", e.g. {3,1}:
 *      ___
 *     |   |_
 * These are combined to form Tri-State bits when sending or receiving codes.*/
 #if defined(ESP8266) || defined(ESP32)
 static const mSAWTransceive::Protocol proto[] = {
 #else
 static const mSAWTransceive::Protocol PROGMEM proto[] = {
 #endif
  { 102, {  10, 58 }, {  3,  11 }, {  10, 4  }, true },   // Protocol 1 - Used for ??
  { 139, {  1, 41 }, {  1,  4 }, {  4,  1 }, false },     // Protocol 2 - Used for plugs and my protocol
  { 101, {  58, 10 }, {  11,  3 }, {  4, 10  }, true },
};

enum { numProto = sizeof(proto) / sizeof(proto[0]) };
uint64_t mSAWTransceive::nReceivedValue = 0;
uint16_t mSAWTransceive::nReceivedBitlength = 0;
uint16_t mSAWTransceive::nReceivedDelay = 0;
uint16_t mSAWTransceive::nReceivedProtocol = 0;
int16_t mSAWTransceive::nReceiveTolerance = 60;
const uint16_t mSAWTransceive::nSeparationLimit = 4300;
// separationLimit: minimum microseconds between received codes, closer codes are ignored.
// according to discussion on issue #14 it might be more suitable to set the separation
// limit to the same time as the 'low' part of the sync signal for the current protocol.
uint16_t mSAWTransceive::timings[SAWRADIO_MAX_CHANGES];

// Sets the protocol to send.
void mSAWTransceive::setProtocol(Protocol protocol) {
  this->protocol = protocol;
}

// Sets the protocol to send, from a list of predefined protocols

void mSAWTransceive::setProtocol(int16_t nProtocol) {
if (nProtocol < 1 || nProtocol > numProto) {
  nProtocol = 1;  // TODO: trigger an error, e.g. "bad protocol" ???
}
#if defined(ESP8266) || defined(ESP32)
  this->protocol = proto[nProtocol-1];
#else
  memcpy_P(&this->protocol, &proto[nProtocol-1], sizeof(Protocol));
#endif
}

// Sets the protocol to send with pulse length in microseconds.
void mSAWTransceive::setProtocol(int16_t nProtocol, int16_t nPulseLength) {
  setProtocol(nProtocol);
  this->setPulseLength(nPulseLength);
}

// Sets pulse length in microseconds
void mSAWTransceive::setPulseLength(int16_t nPulseLength) {
  this->protocol.pulseLength = nPulseLength;
}

// Sets Repeat Transmits
void mSAWTransceive::setRepeatTransmit(int16_t nRepeatTransmit) {
  this->nRepeatTransmit = nRepeatTransmit;
}

// getReceiveEnableState
char mSAWTransceive::getReceiveEnableState() {
  return ReceiveEnableState;
}

// Set Receiving Tolerance
void mSAWTransceive::setReceiveTolerance(int16_t nPercent) {
  mSAWTransceive::nReceiveTolerance = nPercent;
}

// Enable transmissions @param nTransmitterPin    Arduino Pin to which the sender is connected to
void mSAWTransceive::enableTransmit(int16_t nTransmitterPin) {
  this->nTransmitterPin = nTransmitterPin;
  pinMode(this->nTransmitterPin, OUTPUT);
}

// Disable transmissions
void mSAWTransceive::disableTransmit() {
  this->nTransmitterPin = -1;
}

/* Transmit the first 'length' bits of the integer 'code'. The
 * bits are sent from MSB to LSB, i.e., first the bit at position length-1,
 * then the bit at position length-2, and so on, till finally the bit at position 0. */
void mSAWTransceive::send(uint64_t code, uint16_t length) {

  if (this->nTransmitterPin == -1){
    return;
  }

  // make sure the receiver is disabled while we transmit
  int nReceiverInterrupt_backup = nReceiverInterrupt;
  if (nReceiverInterrupt_backup != -1) {
    this->disableReceive();
  }

  for (int nRepeat = 0; nRepeat < nRepeatTransmit; nRepeat++) {
    for (int i = length-1; i >= 0; i--) {
      if (code & (1LL << i))
        this->transmit(protocol.one);
      else
        this->transmit(protocol.zero);
    }
    this->transmit(protocol.syncFactor);
  }

  // enable receiver again if we just disabled it
  if (nReceiverInterrupt_backup != -1) {
    this->enableReceive(nReceiverInterrupt_backup);
  }

}

// Transmit a single high-low pulse.
void mSAWTransceive::transmit(HighLow pulses) {
  uint8_t firstLogicLevel = (this->protocol.invertedSignal) ? LOW : HIGH;
  uint8_t secondLogicLevel = (this->protocol.invertedSignal) ? HIGH : LOW;

  digitalWrite(this->nTransmitterPin, firstLogicLevel);
  delayMicroseconds(this->protocol.pulseLength * pulses.high);
  digitalWrite(this->nTransmitterPin, secondLogicLevel);
  delayMicroseconds(this->protocol.pulseLength * pulses.low);
}

// Enable receiving data
void mSAWTransceive::enableReceive(int16_t interrupt) {
  this->nReceiverInterrupt = interrupt;
  this->enableReceive();
  ReceiveEnableState = true;
}

void mSAWTransceive::enableReceive() { //Serial.println("mSAWTransceive::enableReceive()");
  if (this->nReceiverInterrupt != -1) {
    mSAWTransceive::nReceivedValue = 0;
    mSAWTransceive::nReceivedBitlength = 0;
    //Serial.print("pinrx>> "); Serial.println((uint32_t)this->nReceiverInterrupt);
    attachInterrupt(this->nReceiverInterrupt, handleInterrupt, CHANGE);
  }
}

// Disable receiving data
void mSAWTransceive::disableReceive() {
  detachInterrupt(this->nReceiverInterrupt);
  this->nReceiverInterrupt = -1;
  ReceiveEnableState = false;
}

bool mSAWTransceive::available() {
  return mSAWTransceive::nReceivedValue != 0;
}

void mSAWTransceive::resetAvailable() {
  mSAWTransceive::nReceivedValue = 0;
}

uint64_t mSAWTransceive::getReceivedValue() {
  return mSAWTransceive::nReceivedValue;
}

uint16_t mSAWTransceive::getReceivedBitlength() {
  return mSAWTransceive::nReceivedBitlength;
}

uint16_t mSAWTransceive::getReceivedDelay() {
  return mSAWTransceive::nReceivedDelay;
}

uint16_t mSAWTransceive::getReceivedProtocol() {
  return mSAWTransceive::nReceivedProtocol;
}

uint16_t* mSAWTransceive::getReceivedRawdata() {
  return mSAWTransceive::timings;
}


/* helper function for the receiveProtocol method */
static inline unsigned int diff(int16_t A, int16_t B) {
  return abs(A - B);
}

//
bool RECEIVE_ATTR mSAWTransceive::receiveProtocol(const int16_t p, uint16_t changeCount) { //
  //Serial.println("mSAWTransceive::receiveProtocol");

  #if defined(ESP8266) || defined(ESP32)
      const Protocol &pro = proto[p-1];
  #else
      Protocol pro;
      memcpy_P(&pro, &proto[p-1], sizeof(Protocol));
  #endif

  uint64_t code = 0;
  //Assuming the longer pulse length is the pulse captured in timings[0]
  const unsigned int syncLengthInPulses =  ((pro.syncFactor.low) > (pro.syncFactor.high)) ? (pro.syncFactor.low) : (pro.syncFactor.high);
  const unsigned int delay = mSAWTransceive::timings[0] / syncLengthInPulses;
  const unsigned int delayTolerance = delay * mSAWTransceive::nReceiveTolerance / 100;

  /* For protocols that start low, the sync period looks like
   *               _________
   * _____________|         |XXXXXXXXXXXX|
   * |--1st dur--|-2nd dur-|-Start data-|
   * The 3rd saved duration starts the data.
   * For protocols that start high, the sync period looks like
   *  ______________
   * |              |____________|XXXXXXXXXXXXX|
   * |-filtered out-|--1st dur--|--Start data--|
   * The 2nd saved duration starts the data */

  const unsigned int firstDataTiming = (pro.invertedSignal) ? (2) : (1);

  for (unsigned int i = firstDataTiming; i < changeCount - 1; i += 2) {
      code <<= 1;

      if (diff(mSAWTransceive::timings[i], delay * pro.zero.high) < delayTolerance &&
          diff(mSAWTransceive::timings[i + 1], delay * pro.zero.low) < delayTolerance) {
           //Serial.print('0');
      } else
      if (diff(mSAWTransceive::timings[i], delay * pro.one.high) < delayTolerance &&
          diff(mSAWTransceive::timings[i + 1], delay * pro.one.low) < delayTolerance) {
          // one BIT
          code |= 1;
          //Serial.print('1');
      } else { //error, drop the packet
  			return false;
      }
  }//end for

  if (changeCount > 7) {    // ignore very short transmissions: no device sends them, so this must be noise
      mSAWTransceive::nReceivedValue = code;
      mSAWTransceive::nReceivedBitlength = (changeCount - 1) / 2;
      mSAWTransceive::nReceivedDelay = delay;
      mSAWTransceive::nReceivedProtocol = p;
      return true;
  }

  return false;
}

void RECEIVE_ATTR mSAWTransceive::handleInterrupt() { //Serial.println("mSAWTransceive::handleInterrupt");

  static unsigned int changeCount = 0;
  static unsigned long lastTime = 0;
  static unsigned int repeatCount = 0;

  const long time = micros();
  const unsigned int duration = time - lastTime;

  if (duration > mSAWTransceive::nSeparationLimit) {
    // A long stretch without signal level change occurred. This could be the gap between two transmission.
    if (diff(duration, mSAWTransceive::timings[0]) < 6000) {
      // This long signal is close in length to the long signal which started the previously recorded timings; this suggests that
      // it may indeed by a a gap between two transmissions (we assume here that a sender will send the signal multiple times, with roughly the same gap between them).
      repeatCount++;
      if (repeatCount == 2) {
        for(unsigned int i = 1; i <= numProto; i++) {
          if (receiveProtocol(i, changeCount)) {
            // receive succeeded for protocol i
            Serial.println("[SAW] New data received");
            break;
          }
        }
        repeatCount = 0;
      }
    }
    changeCount = 0;
  }

  if (changeCount >= SAWRADIO_MAX_CHANGES) {
    changeCount = 0;
    repeatCount = 0;
  }

  mSAWTransceive::timings[changeCount++] = duration;
  lastTime = time;
}


#endif
