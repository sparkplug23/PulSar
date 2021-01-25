#ifndef _MSAWPROTOCOL_H
#define _MSAWPROTOCOL_H 0.1


#include "2_CoreSystem/mBaseConfig.h"
#ifdef USE_SAW_TRANSCEIVER


#include "Arduino.h"
#include <stdint.h>

#include "mSAWTransceive.h"
class mSAWTransceive;
// #include "mSAWCodes.h"
// class mSAWCodes;


// BASIC code method, recording and checking against knowns

#define KEY_ZAP_1_ON    4478259
#define KEY_ZAP_1_OFF   4478268
//#define KEY_ZAP_2_ON    4478403 //BROKE
//#define KEY_ZAP_2_OFF   4478412 //BROKE
#define KEY_ZAP_3_ON  4478723
#define KEY_ZAP_3_OFF 4478732
#define KEY_ZAP_4_ON   4480259
#define KEY_ZAP_4_OFF  4480268
#define KEY_ZAP_5_ON   4486403
#define KEY_ZAP_5_OFF  4486412
#define KEY_750_1_ON    333107
#define KEY_750_1_OFF   333116
#define KEY_750_2_ON    333251
#define KEY_750_2_OFF   333260
#define KEY_750_3_ON  333571
#define KEY_750_3_OFF 333580
#define KEY_750_4_ON   335107
#define KEY_750_4_OFF  335116
#define KEY_750_5_ON   341251
#define KEY_750_5_OFF  341260

// ON
#define SAW_SOCKET_ID01_NUM_ON KEY_ZAP_1_ON
#define SAW_SOCKET_ID02_NUM_ON KEY_ZAP_3_ON
#define SAW_SOCKET_ID03_NUM_ON KEY_ZAP_4_ON
#define SAW_SOCKET_ID04_NUM_ON KEY_ZAP_5_ON
#define SAW_SOCKET_ID05_NUM_ON KEY_750_1_ON
#define SAW_SOCKET_ID06_NUM_ON KEY_750_2_ON
#define SAW_SOCKET_ID07_NUM_ON KEY_750_3_ON
#define SAW_SOCKET_ID08_NUM_ON KEY_750_4_ON
#define SAW_SOCKET_ID09_NUM_ON KEY_750_5_ON
// OFF
#define SAW_SOCKET_ID01_NUM_OFF KEY_ZAP_1_OFF
#define SAW_SOCKET_ID02_NUM_OFF KEY_ZAP_3_OFF
#define SAW_SOCKET_ID03_NUM_OFF KEY_ZAP_4_OFF
#define SAW_SOCKET_ID04_NUM_OFF KEY_ZAP_5_OFF
#define SAW_SOCKET_ID05_NUM_OFF KEY_750_1_OFF
#define SAW_SOCKET_ID06_NUM_OFF KEY_750_2_OFF
#define SAW_SOCKET_ID07_NUM_OFF KEY_750_3_OFF
#define SAW_SOCKET_ID08_NUM_OFF KEY_750_4_OFF
#define SAW_SOCKET_ID09_NUM_OFF KEY_750_5_OFF

#define KEY_144_ONE_ON    5330227
#define KEY_144_ONE_OFF   5330236
#define KEY_144_TWO_ON    5330371
#define KEY_144_TWO_OFF   5330380
#define KEY_144_THR_ON  5330691
#define KEY_144_THR_OFF 5330700
#define SAWCODE_ALARMSENSOR_1  13548650
#define SAWCODE_ALARMSENSOR_2  102591
#define SAWCODE_ALARMSENSOR_3  75871
#define SAWCODE_BACKDOORBELL_1 1380693

//Decimal: 5353736 (24Bit) Binary: 010100011011000100001000 Tri-State: not applicable PulseLength: 208 microseconds Protocol: 1
//Raw data: 6467,197,636,615,218,192,637,611,224,197,634,200,634,199,633,616,218,616,217,199,635,614,220,613,220,196,638,195,639,194,638,612,222,196,636,197,636,196,639,194,638,614,219,197,636,161,637,195,638,
#define SAWCODE_BACKDOORBELL_2 5353736
#define SAWPULSELENGTH_BACKDOORBELL_2 208 //ms


class mSAWProtocol{

  public:
    mSAWProtocol(){};

    void DecodePayload(uint64_t decimal, uint16_t bitlength, uint16_t protocol);

    struct DECODEDATA{
      uint64_t decimel;
      uint16_t bitlength;
      uint16_t protocol;
      char namectr[30];
      char statectr[2];
      uint8_t fWaiting;
    }decode;

    uint32_t GetCodeFromName(const char* name);
    const char* GetNameFromCodeCtr(uint32_t code);

};

#endif

#endif
