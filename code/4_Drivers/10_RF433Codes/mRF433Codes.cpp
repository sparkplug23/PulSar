/**
  mRF433Codes.ino - RF transceiver using RcSwitch library

  Copyright (C) 2022    Michael Doone

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "mRF433Codes.h"

#ifdef USE_MODULE_DRIVERS_RF433_CODES

int8_t mRF433Codes::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function)
  {
    
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_50_MSECOND:
      ReceiveCheck();
    break;
    case TASK_UPDATE_OTA_BEFORE_ON_START:
      ALOG_INF(PSTR("disableReceive"));
      mySwitch->disableReceive();
    break;
    case TASK_EVERY_FIVE_SECOND:{
      
      // AddLog(LOG_LEVEL_INFO,PSTR("tkr_set->Settings.rf_protocol_mask=%d"), tkr_set->Settings.rf_protocol_mask);

      
    // mySwitch->disableReceive();

    //   SubTask_SendCommand_Up();
    //   SubTask_SendCommand_Up();
    //   SubTask_SendCommand_Up();

      
// // low, 8 high then low, held high 5000ms... code


// uint16_t pre_command_raw[] = {

//   // 2000, //low
//   400, 600,
//   400, 600,
//   400, 600,
//   400, 600,
//   400, 600,
//   400, 600,
//   400, 600,
//   400, 600,




// };


// digitalWrite(22, LOW); // set low first, header will toggle high again
// delayMicroseconds(2000);

// digitalWrite(22, HIGH); // set low first, header will toggle high again
// delayMicroseconds(2500);

// digitalWrite(22, LOW); // set low first, header will toggle high again
// delayMicroseconds(2200);



// for(uint16_t i=0;i<ARRAY_SIZE(pre_command_raw); i++)
// {
//   // if(i%2)
  
//   digitalWrite(22, ((i%2)==1)?LOW:HIGH);
//   delayMicroseconds(pre_command_raw[i]);
// }
// digitalWrite(22, LOW);




// /**
//  * @brief State starts high, toggles ie [0] = high, [1] = low..... finish with assert low
//  * 
//  */
// uint16_t up_command_raw[] = {
  
// 4958,655,
// 550,333,
// 227,652,
// 549,331,
// 228,653, //10

// 547,338,
// 543,336,
// 224,656,
// 225,661,
// 542,337, //20

// 222,658,
// 544,339,
// 222,655,
// 225,658,
// 227,653, //30

// 228,651,
// 225,657,
// 545,336,
// 546,334,
// 547,337, //40

// 544,335,
// 225,655,
// 226,655,
// 548,333,
// 227,655, //50

// 547,336,
// 546,336,
// 543,341,
// 541,336,
// 224,658, //60

// 544,337,
// 545,337,
// 544,340,
// 221,660,
// 220,658, //70

// 544,338,
// 543,339,
// 543,335,
// 546,339,
// 541,338, //80

// 542,338,      //6of6 fat tops
// //start of 12 narrow tops
// 223,655,   // 1/12 up as thin
// 225,657,
// 225,656,
// 225,655, //90

// 226,660,
// 220,658,
// 223,656,
// 226,655,
// 226,657, //100
// 224,661, //wrong after this   10/12
// 224, 661, //short here manually added   11/12
// 224, 661, //completely added by me

// //
// // after 12 short top pulses

// 551,334,
// 223,664,
// 548,336,
// 546,337,
// 554,335,

// 546,335,
// 226,658,544,335,226,655,226,653,547,338,544,334,545




// // 550,335,
// // 226,661,
// // 543,334,
// // 547,334,
// // 547,339,
// // 541,338,
// // 228,653,
// // 553,328,
// // 232,648,
// // 231,654,
// // 548,332,
// // 550,330,

// // 548

// // 653,223,
// // 657,547, //110

// // 333, //110

// // 227,655,
// // 226,653,
// // 548,339,
// // 543,338,
// // 542,338, //120

// // 223,655,
// // 545,338,
// // 544,334,


// // 227,656,224

// };

// uint8_t logic_level = LOW;

// // digitalWrite(22, HIGH); // set low first, header will toggle high again
// // delayMicroseconds(8000);


// // digitalWrite(22, LOW); // set low first, header will toggle high again
// // delayMicroseconds(10000);

// // digitalWrite(22, HIGH); // set low first, header will toggle high again
// // delayMicroseconds(10000);

// digitalWrite(22, HIGH); // set low first, header will toggle high again
// delayMicroseconds(5000);

// for(uint8_t j=0;j<11;j++)
// {



// for(uint16_t i=0;i<ARRAY_SIZE(up_command_raw); i++)
// {

// digitalWrite(22, !digitalRead(22));
// delayMicroseconds(up_command_raw[i]);


// }
// digitalWrite(22, LOW);

// delayMicroseconds(5000);


// }


    }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
     /************
     * TELEMETRY SECTION * 
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_tele->Telemetry_Rate(telemetry_list);
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY__SENDER_MQTT:
      tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_SERIAL
    case TASK_SERIAL_TELEMETRY:
      tkr_serial->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_TELEMETRY:
      tkr_web->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
  }

  return TASKER_RESULT__UNKNOWN_ID;

}//end

#ifdef ENABLE_FEATURE_DRIVERS__RF433_TRANSMIT_BITBANG_TEST
void mRF433Codes::TransmitTest()
{
  static constexpr uint8_t TX_PIN = 22;

  /**
   * ComfyShade / ZN-115T
   *
   * One captured DOWN frame, normalized to T = 333 us.
   *
   * Array starts HIGH and alternates:
   *
   *   [0] HIGH
   *   [1] LOW
   *   [2] HIGH
   *   [3] LOW
   *   ...
   *
   * Entire frame:
   *   150 timing cells
   *   ~49.95 ms
   *
   * Send same frame 5 times continuously.
   */
  static const uint16_t down_command_raw[] = {

    4995,  // HIGH 15T
    2331,  // LOW   7T
    1665,  // HIGH  5T
     666,  // LOW   2T
     333,  // HIGH  1T
    1332,  // LOW   4T
     666,  // HIGH  2T
    2331,  // LOW   7T
     666,  // HIGH  2T
     333,  // LOW   1T

     333,  // HIGH  1T
    1332,  // LOW   4T
     333,  // HIGH  1T
     333,  // LOW   1T
     666,  // HIGH  2T
     666,  // LOW   2T
     999,  // HIGH  3T
    1332,  // LOW   4T
     666,  // HIGH  2T
     333,  // LOW   1T

     333,  // HIGH  1T
    2331,  // LOW   7T
     333,  // HIGH  1T
    3663,  // LOW  11T
     333,  // HIGH  1T
     999,  // LOW   3T
     333,  // HIGH  1T
     999,  // LOW   3T
     333,  // HIGH  1T
    1998,  // LOW   6T

     666,  // HIGH  2T
    1998,  // LOW   6T
     666,  // HIGH  2T
     333,  // LOW   1T
     666,  // HIGH  2T
    1665,  // LOW   5T
     666,  // HIGH  2T
    1998,  // LOW   6T
     666,  // HIGH  2T
     333,  // LOW   1T

     333,  // HIGH  1T
    1332,  // LOW   4T
     999,  // HIGH  3T
     666,  // LOW   2T
     333,  // HIGH  1T
     666,  // LOW   2T
     666,  // HIGH  2T
     333,  // LOW   1T
     666,  // HIGH  2T
     333   // LOW   1T
  };

  pinMode(TX_PIN, OUTPUT);

  // Known idle state.
  digitalWrite(TX_PIN, LOW);
  delayMicroseconds(5000);

  for (uint8_t repeat = 0; repeat < 5; repeat++)
  {
    for (uint16_t i = 0; i < ARRAY_SIZE(down_command_raw); i++)
    {
      // Frame begins HIGH and alternates thereafter.
      digitalWrite(TX_PIN, (i & 1) ? LOW : HIGH);
      delayMicroseconds(down_command_raw[i]);
    }

    // Last array entry is already LOW.
    digitalWrite(TX_PIN, LOW);
  }

  digitalWrite(TX_PIN, LOW);
}
#endif



void mRF433Codes::SubTask_SendCommand_Up()
{
  SubTask_SendCommand_Up_PreTrain();
  SubTask_SendCommand_Up_Block2(10);
  SubTask_SendCommand_Up_Block2(10);
  SubTask_SendCommand_Up_Block2(11);


}

#define PULSE_SHORT 280
#define PULSE_LONG 600 

void mRF433Codes::SubTask_SendCommand_Up_PreTrain()
{

      /**
       * @brief 
       * Start frame to notidy device of incoming message
       * 
       */
// low, 8 high then low, held high 5000ms... code

uint8_t  pre_command_raw_start_state = HIGH;
uint16_t pre_command_raw[] = {

  PULSE_SHORT, PULSE_LONG,  // HIGH, LOW
  PULSE_SHORT, PULSE_LONG,
  PULSE_SHORT, PULSE_LONG,
  PULSE_SHORT, PULSE_LONG,
  PULSE_SHORT, PULSE_LONG,
  PULSE_SHORT, PULSE_LONG,
  PULSE_SHORT, PULSE_LONG,
  PULSE_SHORT, PULSE_LONG,

  // 5000, //5 ms up

  //final 5000ms down should be part of repeating code gaurd period and sent as first bit below




};


digitalWrite(22, LOW); // Starting with down state
delay(27);

for(uint16_t i=0;i<ARRAY_SIZE(pre_command_raw); i++)
{
  // if(i%2)
  
  // digitalWrite(22, ((i%2)==0)?LOW:HIGH);
      digitalWrite(22, !digitalRead(22));
  delayMicroseconds(pre_command_raw[i]);
}

// digitalWrite(22, HIGH);
// delayMicroseconds(5000);

// NO DELAY, START PULSE AS HEADER 




}


void mRF433Codes::SubTask_SendCommand_Up_Block2(int repeats)
{



/**
 * @brief State starts high, toggles ie [0] = high, [1] = low..... finish with assert low
 * 
 */
uint16_t up_command_raw[] = {
  
5000, //header high
PULSE_LONG, //low, still part of header in 3state


PULSE_LONG,PULSE_SHORT, //h,l
PULSE_SHORT,PULSE_LONG,
PULSE_LONG,PULSE_SHORT,
PULSE_SHORT,PULSE_LONG, //10

PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,
PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG,
PULSE_LONG,PULSE_SHORT, //20

PULSE_SHORT,PULSE_LONG,
PULSE_LONG,PULSE_SHORT,
PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG, //30

PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG,
PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT, //40

PULSE_LONG,PULSE_SHORT,
PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG,
PULSE_LONG,PULSE_SHORT,
PULSE_SHORT,PULSE_LONG, //50

PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,
PULSE_SHORT,PULSE_LONG, //60

PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,
PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG, //70

PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT, //80

PULSE_LONG,PULSE_SHORT,      //6of6 fat tops
//start of 12 narrow tops
PULSE_SHORT,PULSE_LONG,   // 1/12 up as thin
PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG, //90

PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG, //100
PULSE_SHORT,PULSE_LONG, //wrong after this   10/12
PULSE_SHORT, PULSE_LONG, //short here manually added   11/12
PULSE_SHORT, PULSE_LONG, //completely added by me

//
// after 12 short top pulses

PULSE_LONG,PULSE_SHORT,
PULSE_SHORT,PULSE_LONG,
PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,

PULSE_LONG,PULSE_SHORT,
PULSE_SHORT,PULSE_LONG,
PULSE_LONG,PULSE_SHORT,
PULSE_SHORT,PULSE_LONG,
PULSE_SHORT,PULSE_LONG,
PULSE_LONG,PULSE_SHORT,
PULSE_LONG,PULSE_SHORT,

PULSE_LONG,

5000//tailer guard of LOW signal


// 550,335,
// 226,661,
// 543,334,
// 547,334,
// 547,339,
// 541,338,
// 228,653,
// 553,328,
// 232,648,
// 231,654,
// 548,332,
// 550,330,

// 548

// 653,223,
// 657,547, //110

// 333, //110

// 227,655,
// 226,653,
// 548,339,
// 543,338,
// 542,338, //120

// 223,655,
// 545,338,
// 544,334,


// 227,656,224

};

uint8_t logic_level = LOW;

// digitalWrite(22, HIGH); // set low first, header will toggle high again
// delayMicroseconds(8000);


// digitalWrite(22, LOW); // set low first, header will toggle high again
// delayMicroseconds(10000);

// digitalWrite(22, HIGH); // set low first, header will toggle high again
// delayMicroseconds(10000);

// digitalWrite(22, HIGH); // set low first, header will toggle high again
// delayMicroseconds(5000);

  for(uint8_t j=0;j<repeats;j++)
  {

    digitalWrite(22, LOW);
    for(uint16_t i=0;i<ARRAY_SIZE(up_command_raw); i++)
    {
      digitalWrite(22, !digitalRead(22));
      // digitalWrite(22, ((i%2)==0)?LOW:HIGH);
      // ALOG_INF("i=%d,state=%d", i, ((i%2)==0)?LOW:HIGH );
      delayMicroseconds(up_command_raw[i]);
    }
    digitalWrite(22, LOW);

  // delayMicroseconds(5000);
  }


}




void mRF433Codes::SubTask_SendCommand_Up_Block(int repeats)
{



/**
 * @brief State starts high, toggles ie [0] = high, [1] = low..... finish with assert low
 * 
 */
uint16_t up_command_raw[] = {
  
4958,655, // header
550,333,
227,652,
549,331,
228,653, //10

547,338,
543,336,
224,656,
225,661,
542,337, //20

222,658,
544,339,
222,655,
225,658,
227,653, //30

228,651,
225,657,
545,336,
546,334,
547,337, //40

544,335,
225,655,
226,655,
548,333,
227,655, //50

547,336,
546,336,
543,341,
541,336,
224,658, //60

544,337,
545,337,
544,340,
221,660,
220,658, //70

544,338,
543,339,
543,335,
546,339,
541,338, //80

542,338,      //6of6 fat tops
//start of 12 narrow tops
223,655,   // 1/12 up as thin
225,657,
225,656,
225,655, //90

226,660,
220,658,
223,656,
226,655,
226,657, //100
224,661, //wrong after this   10/12
224, 661, //short here manually added   11/12
224, 661, //completely added by me

//
// after 12 short top pulses

551,334,
223,664,
548,336,
546,337,
554,335,

546,335,
226,658,544,335,226,655,226,653,547,338,544,334,545,

5000//tailer guard of LOW signal


// 550,335,
// 226,661,
// 543,334,
// 547,334,
// 547,339,
// 541,338,
// 228,653,
// 553,328,
// 232,648,
// 231,654,
// 548,332,
// 550,330,

// 548

// 653,223,
// 657,547, //110

// 333, //110

// 227,655,
// 226,653,
// 548,339,
// 543,338,
// 542,338, //120

// 223,655,
// 545,338,
// 544,334,


// 227,656,224

};

uint8_t logic_level = LOW;

// digitalWrite(22, HIGH); // set low first, header will toggle high again
// delayMicroseconds(8000);


// digitalWrite(22, LOW); // set low first, header will toggle high again
// delayMicroseconds(10000);

// digitalWrite(22, HIGH); // set low first, header will toggle high again
// delayMicroseconds(10000);

// digitalWrite(22, HIGH); // set low first, header will toggle high again
// delayMicroseconds(5000);

  for(uint8_t j=0;j<repeats;j++)
  {

    digitalWrite(22, LOW);
    for(uint16_t i=0;i<ARRAY_SIZE(up_command_raw); i++)
    {
      digitalWrite(22, !digitalRead(22));
      // digitalWrite(22, ((i%2)==0)?LOW:HIGH);
      // ALOG_INF("i=%d,state=%d", i, ((i%2)==0)?LOW:HIGH );
      delayMicroseconds(up_command_raw[i]);
    }
    digitalWrite(22, LOW);

  // delayMicroseconds(5000);
  }


}

void mRF433Codes::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
}


void mRF433Codes::Init(void)
{

  if (tkr_pins->PinUsed(GPIO_RF_433MHZ_RX)) 
  {

    if (tkr_set->Settings.rf_duplicate_time < 10) {
      tkr_set->Settings.rf_duplicate_time = RF_TIME_AVOID_DUPLICATE;
    }

    pinMode( tkr_pins->GetPin(GPIO_RF_433MHZ_RX), INPUT);

    if(mySwitch==nullptr)
    {
      mySwitch = new RCSwitch();
    }

    ALOG_INF(PSTR("mRF433Codes RX: %d"), tkr_pins->GetPin(GPIO_RF_433MHZ_RX));

    mySwitch->enableReceive(tkr_pins->GetPin(GPIO_RF_433MHZ_RX));
    // if (!tkr_set->Settings.rf_protocol_mask) {
      // tkr_set->Settings.rf_protocol_mask = (1ULL << mySwitch->getNumProtos()) -1;
      // Correctly only permits protocol 1 through
      tkr_set->Settings.rf_protocol_mask = (1ULL << 1) -1; //only want number 2?
    // }

    #ifndef ENABLE_DEVFETURE_DISABLE_EXTENDED_FEATURES_START
    mySwitch->setReceiveProtocolMask(tkr_set->Settings.rf_protocol_mask);
    #endif // ENABLE_DEVFETURE_DISABLE_EXTENDED_FEATURES_START

    module_state.mode = ModuleStatus::Running;
  }

  if (tkr_pins->PinUsed(GPIO_RF_433MHZ_TX)) 
  {
    
    if(mySwitch==nullptr)
    {
      mySwitch = new RCSwitch();
    }

    mySwitch->enableTransmit(tkr_pins->GetPin(GPIO_RF_433MHZ_TX));
    module_state.mode = ModuleStatus::Running;
  }

}


void mRF433Codes::ReceiveCheck(void) 
{

  if(tkr_time->uptime_seconds_nonreset < 10)
  {
    return;
  }

  // ALOG_INF(PSTR("RFR: ReceiveCheck() %d"), mySwitch->available());

  if (mySwitch->available())
  {

    unsigned long data = mySwitch->getReceivedValue();
    unsigned int bits = mySwitch->getReceivedBitlength();
    int protocol = mySwitch->getReceivedProtocol();
    int delay = mySwitch->getReceivedDelay();

    ALOG_INF(PSTR("RFR: Data 0x%lX (%u), Bits %d, Protocol %d, Delay %d"), data, data, bits, protocol, delay);

    uint32_t now = millis();
    if ((now - rx_pkt.received_time_millis > tkr_set->Settings.rf_duplicate_time) && (data > 0))
    {
      rx_pkt.received_time_millis = now;

      /**
       * @brief If backoff period has happened, then save to struct
       **/
      rx_pkt.data = data;
      rx_pkt.bit_length = bits;
      rx_pkt.protocol = protocol;
      rx_pkt.delay = delay;
      rx_pkt.received_utc_time = tkr_time->UtcTime();

      /**
       * @brief Share with mqtt
       **/
			telemetry_state_ifchanged.flags.SendNow = true;

      /**
       * @brief TODO: Trigger Event "RF433_RECEIVED"
       **/

      //char stemp[16];
      // if (tkr_set->Settings.flag.rf_receive_decimal) {      // SetOption28 - RF receive data format (0 = hexadecimal, 1 = decimal)
      //   snprintf_P(stemp, sizeof(stemp), PSTR("%u"), (uint32_t)data);
      // } else {
      //  snprintf_P(stemp, sizeof(stemp), PSTR("\"0x%lX\""), (uint32_t)data);
      // }

      // ResponseTime_P(PSTR(",\"" D_RFRECEIVED "\":{\"" D_RF_DATA "\":%s,\"" D_RF_BITS "\":%d,\"" D_RF_PROTOCOL "\":%d,\"" D_RF_PULSE "\":%d}}"),
      //   stemp, bits, protocol, delay);
      // MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_RFRECEIVED));

        ALOG_TST(PSTR("RFR: Data 0x%lX (%u), Bits %d, Protocol %d, Delay %d"), data, data, bits, protocol, delay);
      
    }
    mySwitch->resetAvailable();
  }
}


// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/

// void CmndRfProtocol(void) {
//   if (!PinUsed(GPIO_RFRECV)) { return; }

// //  ALOG_INF(PSTR("RFR:CmndRfRxProtocol:: index:%d usridx:%d data_len:%d data:\"%s\""),XdrvMailbox.index, XdrvMailbox.usridx, XdrvMailbox.data_len,XdrvMailbox.data);

//   uint64_t thisdat;
//   if (1 == XdrvMailbox.usridx) {
//     if (XdrvMailbox.payload >= 0) {
//       thisdat = (1ULL << (XdrvMailbox.index -1));
//       if (XdrvMailbox.payload &1) {
//         tkr_set->Settings.rf_protocol_mask |= thisdat;
//       } else {
//         tkr_set->Settings.rf_protocol_mask &= ~thisdat;
//       }
//     }
//     else if (XdrvMailbox.data_len > 0) {
//       return;  // Not a number
//     }
//   } else {
//     if (XdrvMailbox.data_len > 0) {
//       if ('A' == toupper(XdrvMailbox.data[0])) {
//         tkr_set->Settings.rf_protocol_mask = (1ULL << mySwitch->getNumProtos()) -1;
//       } else {
//         thisdat = strtoull(XdrvMailbox.data, nullptr, 0);
//         if ((thisdat > 0) || ('0' == XdrvMailbox.data[0])) {
//           tkr_set->Settings.rf_protocol_mask = thisdat;
//         } else {
//           return;  // Not a number
//         }
//       }
//     }
//   }
//   mySwitch->setReceiveProtocolMask(tkr_set->Settings.rf_protocol_mask);
// //  ALOG_INF(PSTR("RFR: CmndRfProtocol:: Start responce"));
//   Response_P(PSTR("{\"" D_CMND_RFPROTOCOL "\":\""));
//   bool gotone = false;
//   thisdat = 1;
//   for (uint32_t i = 0; i < mySwitch->getNumProtos(); i++) {
//     if (tkr_set->Settings.rf_protocol_mask & thisdat) {
//       ResponseAppend_P(PSTR("%s%d"), (gotone) ? "," : "", i+1);
//       gotone = true;
//     }
//     thisdat <<=1;
//   }
//   if (!gotone) { ResponseAppend_P(PSTR(D_NONE_ENABLED)); }
//   ResponseAppend_P(PSTR("\""));
//   ResponseJsonEnd();
// }

// void CmndRfSend(void)
// {
//   if (!PinUsed(GPIO_RFSEND)) { return; }

//   bool error = false;

//   if (XdrvMailbox.data_len) {
//     unsigned long long data = 0;	// unsigned long long  => support payload >32bit
//     unsigned int bits = 24;
//     int protocol = 1;
//     int repeat = 10;
//     int pulse = 0; // 0 leave the library use the default value depending on protocol

//     JsonParser parser(XdrvMailbox.data);
//     JsonParserObject root = parser.getRootObject();
//     if (root) {
//       // RFsend {"data":0x501014,"bits":24,"protocol":1,"repeat":10,"pulse":350}
//       char parm_uc[10];
//       data = root.getULong(PSTR(D_RF_DATA), data);	// read payload data even >32bit
//       bits = root.getUInt(PSTR(D_RF_BITS), bits);
//       protocol = root.getInt(PSTR(D_RF_PROTOCOL), protocol);
//       repeat = root.getInt(PSTR(D_RF_REPEAT), repeat);
//       pulse = root.getInt(PSTR(D_RF_PULSE), pulse);
//     } else {
//       //  RFsend data, bits, protocol, repeat, pulse
//       char *p;
//       uint8_t i = 0;
//       for (char *str = strtok_r(XdrvMailbox.data, ", ", &p); str && i < 5; str = strtok_r(nullptr, ", ", &p)) {
//         switch (i++) {
//         case 0:
//           data = strtoul(str, nullptr, 0);  // Allow decimal (5246996) and hexadecimal (0x501014) input
//           break;
//         case 1:
//           bits = atoi(str);
//           break;
//         case 2:
//           protocol = atoi(str);
//           break;
//         case 3:
//           repeat = atoi(str);
//           break;
//         case 4:
//           pulse = atoi(str);
//         }
//       }
//     }

//     if (!protocol) { protocol = 1; }
//     mySwitch->setProtocol(protocol);
//     // if pulse is specified in the command, enforce the provided value (otherwise lib takes default)
//     if (pulse) { mySwitch->setPulseLength(pulse); }
//     if (!repeat) { repeat = 10; }     // Default at init
//     mySwitch->setRepeatTransmit(repeat);
//     if (!bits) { bits = 24; }         // Default 24 bits
//     if (data) {
//       mySwitch->send(data, bits);
//       ResponseCmndDone();
//     } else {
//       error = true;
//     }
//   } else {
//     error = true;
//   }
//   if (error) {
//     Response_P(PSTR("{\"" D_CMND_RFSEND "\":\"" D_NO " " D_RF_DATA ", " D_RF_BITS ", " D_RF_PROTOCOL ", " D_RF_REPEAT " " D_OR " " D_RF_PULSE "\"}"));
//   }
// }

// void CmndRfTimeOut(void) {
//   if (XdrvMailbox.payload >= 10) {
//     tkr_set->Settings.rf_duplicate_time = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(tkr_set->Settings.rf_duplicate_time);
// }

/**
 * @brief 
 * 
 *  Raw: holding up button with 6 channels active
Decimal: 1056974060 (64Bit) Binary: 0000000000000000000000000000000000111111000000000000000011111100 Tri-State: 00000000000000000111000000001110 PulseLength: 225 microseconds Protocol: 2
Raw data: 4958,655,550,333,227,652,549,331,228,653,547,338,543,336,224,656,225,661,542,337,222,658,544,339,222,655,225,658,227,653,228,651,225,657,545,336,546,334,547,337,544,335,225,655,226,655,548,333,227,655,547,336,546,336,543,341,541,336,224,658,544,337,545,337,544,340,221,660,220,658,544,338,543,339,543,335,546,339,541,338,542,338,223,655,225,657,225,656,225,655,226,660,220,658,223,656,226,655,226,657,224,661,542,336,227,653,223,657,547,333,227,655,226,653,548,339,543,338,542,338,223,655,545,338,544,334,227,656,224,



Raw data: 

4958,655,   1,0

550,333,    
227,652,
549,331,
228,653,
547,338,
543,336,
224,656,
225,661,
542,337,
222,658,
544,339,
222,655,
225,658,
227,653,
228,651,
225,657,
545,336,
546,334,
547,337,
544,335,
225,655,
226,655,
548,333,
227,655,
547,336,
546,336,
543,341,
541,336,
224,658,
544,337,
545,337,
544,340,
221,660,
220,658,
544,338,
543,339,
543,335,
546,339,
541,338,
542,338,
223,655,
225,657,
225,656,
225,655,
226,660,
220,658,
223,656,
226,655,
226,657,
224,661,
542,336,
227,653,
223,657,
547,333,
227,655,
226,653,
548,339,
543,338,
542,338,
223,655,
545,338,
544,334,
227,656,
224,



 * 
 */




/******************************************************************************************************************
 * 
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mRF433Codes::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  
  #ifdef ENABLE_FEATURE_DRIVERS__RF433_TRANSMIT_BITBANG_TEST
	if(jtok = obj["TransmitTest"])
	{
    TransmitTest();
  }
  #endif


	if(jtok = obj["RfMask"])
	{

		if(jtok.isNum())
		{
			mySwitch->setReceiveProtocolMask(jtok.getUInt());
			telemetry_settings.flags.SendNow = true;
		}

		// JBI->Start();

		// tkr->Tasker_Interface(TASK_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID);

		// bool ready_to_send = JBI->End();

		// if(!ready_to_send)
		// {
		// 	// Nothing was found, create new message
		// 	JBI->Start();
		// 		JBI->Add("SensorScan", "No Devices Found");
		// 	ready_to_send = JBI->End();
		// }


		// if(ready_to_send)
		// {			
    	ALOG_TST(PSTR("RfMask = %d / %d"), jtok.getUInt(), mySwitch->GetReceiveProtolMask());
		// 	tkr_mqtt->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
		// }

	}
	if(jtok = obj["RfSend"])
	{

		if(jtok.isNum())
		{
			mySwitch->send(jtok.getInt(), 24);
		}

    ALOG_TST(PSTR("RfSend = %d / %d"), jtok.getInt(), 24 );

	}



    
}

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

  
uint8_t mRF433Codes::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    // JBI->Add(D_COUNT, settings.fEnableSensor);
    JBI->Add("RfMask", mySwitch->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mRF433Codes::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    JBI->Object_Start(D_RFRECEIVED);
  
      JBI->Add(D_DATA, rx_pkt.data);
      JBI->Add(D_RF_BITS, rx_pkt.bit_length);
      JBI->Add(D_RF_PROTOCOL, rx_pkt.protocol);
      JBI->Add(D_RF_PULSE, rx_pkt.delay);   
      JBI->Add(D_MILLIS, rx_pkt.received_time_millis);   
      JBI->Add(D_TIME, tkr_time->GetTime().c_str() );
      
    
    JBI->Object_End();
  
  

  return JBI->End();

}

/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mRF433Codes::Telemetry_Init()
{

  struct telemetry_handler<mRF433Codes>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mRF433Codes::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_IFCHANGED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRF433Codes::ConstructJSON_State;
  telemetry_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT



#endif // USE_MODULE_DRIVERS_RF433_CODES