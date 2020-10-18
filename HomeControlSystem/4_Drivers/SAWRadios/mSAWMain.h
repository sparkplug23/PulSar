#ifndef _MSAWMAIN_H
#define _MSAWMAIN_H
//3.0 first okay version
//#define _MSAWRADIOS_VERSION 3.1 //version that divides basic function and my 8 bytes packet forma


#include "0_ConfigUser/mUserConfig.h"
#ifdef USE_SAW_TRANSCEIVER




#include "mSAWTransceive.h"
class mSAWTransceive;

#include "mSAWProtocol.h"
class mSAWProtocol;
// #include "mSAWCodes.h"
// class mSAWCodes;
//#include "SAWRadios/mSAWCodes.h"
// Task as controller -- main SAW controller -- other Files should be considered private

#include "1_TaskerManager/mTaskerManager.h"

#include "2_CoreSystem/Logging/mLogging.h"
class mLogging;

#include "MQTT/mMQTT.h"
class mMQTT;

class mSAWMain{
public:
  mSAWMain(){};

  void parse_JSONCommand(void);

  void parsesub_Transmit(void);

  int8_t Tasker(uint8_t function);
  uint32_t tSavedDisabledTime;

  uint8_t fForceSendAllMQQTData = false;
  uint8_t fDecodedPacketToShare = false;
  void SubTasker_MQTTSender();
  void MQTTSendDecoded();
  void MQQTDataBuilder_Decoded();



};


#endif

#endif
