/**
  mRCSwitch.ino - RF transceiver using RcSwitch library

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

#include "mRCSwitch.h"

#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH

const char* mRCSwitch::PM_MODULE_DRIVERS_RF433_RCSWITCH_CTR = D_MODULE_DRIVERS_RF433_RCSWITCH_CTR;
const char* mRCSwitch::PM_MODULE_DRIVERS_RF433_RCSWITCH_FRIENDLY_CTR = D_MODULE_DRIVERS_RF433_RCSWITCH_FRIENDLY_CTR;


int8_t mRCSwitch::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function)
  {
    
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_50_MSECOND:
      ReceiveCheck();
    break;
    case FUNC_EVERY_SECOND:{
      
      // AddLog(LOG_LEVEL_INFO,PSTR("pCONT_set->Settings.rf_protocol_mask=%d"), pCONT_set->Settings.rf_protocol_mask);

    }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORKS_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    break; 
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif  
  }

}//end


void mRCSwitch::Pre_Init(void)
{

}


void mRCSwitch::Init(void)
{

  if (pCONT_pins->PinUsed(GPIO_RF_433MHZ_TX_ID)) {
    mySwitch->enableTransmit(pCONT_pins->GetPin(GPIO_RF_433MHZ_TX_ID));
    settings.fEnableSensor = true;
  }
  if (pCONT_pins->PinUsed(GPIO_RF_433MHZ_RX_ID)) 
  {
    if (pCONT_set->Settings.rf_duplicate_time < 10) {
      pCONT_set->Settings.rf_duplicate_time = RF_TIME_AVOID_DUPLICATE;
    }
    pinMode( pCONT_pins->GetPin(GPIO_RF_433MHZ_RX_ID), INPUT);

    mySwitch = new RCSwitch();

    mySwitch->enableReceive(pCONT_pins->GetPin(GPIO_RF_433MHZ_RX_ID));
    // if (!pCONT_set->Settings.rf_protocol_mask) {
      // pCONT_set->Settings.rf_protocol_mask = (1ULL << mySwitch->getNumProtos()) -1;
// Correctly only permits protocol 1 through
      pCONT_set->Settings.rf_protocol_mask = (1ULL << 1) -1; //only want number 2?
    // }
    mySwitch->setReceiveProtocolMask(pCONT_set->Settings.rf_protocol_mask);
    settings.fEnableSensor = true;
  }

}


void mRCSwitch::ReceiveCheck(void) {
  if (mySwitch->available()) {

    unsigned long data = mySwitch->getReceivedValue();
    unsigned int bits = mySwitch->getReceivedBitlength();
    int protocol = mySwitch->getReceivedProtocol();
    int delay = mySwitch->getReceivedDelay();

    AddLog(LOG_LEVEL_DEBUG, PSTR("RFR: Data 0x%lX (%u), Bits %d, Protocol %d, Delay %d"), data, data, bits, protocol, delay);

    uint32_t now = millis();
    if ((now - rx_pkt.received_time_millis > pCONT_set->Settings.rf_duplicate_time) && (data > 0))
    {
      rx_pkt.received_time_millis = now;

      /**
       * @brief If backoff period has happened, then save to struct
       **/
      rx_pkt.data = data;
      rx_pkt.bit_length = bits;
      rx_pkt.protocol = protocol;
      rx_pkt.delay = delay;
      rx_pkt.received_utc_time = pCONT_time->GetTimeShortNowU32();

      /**
       * @brief Share with mqtt
       **/
			mqtthandler_state_ifchanged.flags.SendNow = true;

      /**
       * @brief TODO: Trigger Event "RF433_RECEIVED"
       **/

      //char stemp[16];
      // if (pCONT_set->Settings.flag.rf_receive_decimal) {      // SetOption28 - RF receive data format (0 = hexadecimal, 1 = decimal)
      //   snprintf_P(stemp, sizeof(stemp), PSTR("%u"), (uint32_t)data);
      // } else {
      //  snprintf_P(stemp, sizeof(stemp), PSTR("\"0x%lX\""), (uint32_t)data);
      // }

      // ResponseTime_P(PSTR(",\"" D_JSON_RFRECEIVED "\":{\"" D_JSON_RF_DATA "\":%s,\"" D_JSON_RF_BITS "\":%d,\"" D_JSON_RF_PROTOCOL "\":%d,\"" D_JSON_RF_PULSE "\":%d}}"),
      //   stemp, bits, protocol, delay);
      // MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_TELE, PSTR(D_JSON_RFRECEIVED));

        AddLog(LOG_LEVEL_TEST, PSTR("RFR: Data 0x%lX (%u), Bits %d, Protocol %d, Delay %d"), data, data, bits, protocol, delay);
      
    }
    mySwitch->resetAvailable();
  }
}


// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/

// void CmndRfProtocol(void) {
//   if (!PinUsed(GPIO_RFRECV)) { return; }

// //  AddLog(LOG_LEVEL_INFO, PSTR("RFR:CmndRfRxProtocol:: index:%d usridx:%d data_len:%d data:\"%s\""),XdrvMailbox.index, XdrvMailbox.usridx, XdrvMailbox.data_len,XdrvMailbox.data);

//   uint64_t thisdat;
//   if (1 == XdrvMailbox.usridx) {
//     if (XdrvMailbox.payload >= 0) {
//       thisdat = (1ULL << (XdrvMailbox.index -1));
//       if (XdrvMailbox.payload &1) {
//         pCONT_set->Settings.rf_protocol_mask |= thisdat;
//       } else {
//         pCONT_set->Settings.rf_protocol_mask &= ~thisdat;
//       }
//     }
//     else if (XdrvMailbox.data_len > 0) {
//       return;  // Not a number
//     }
//   } else {
//     if (XdrvMailbox.data_len > 0) {
//       if ('A' == toupper(XdrvMailbox.data[0])) {
//         pCONT_set->Settings.rf_protocol_mask = (1ULL << mySwitch->getNumProtos()) -1;
//       } else {
//         thisdat = strtoull(XdrvMailbox.data, nullptr, 0);
//         if ((thisdat > 0) || ('0' == XdrvMailbox.data[0])) {
//           pCONT_set->Settings.rf_protocol_mask = thisdat;
//         } else {
//           return;  // Not a number
//         }
//       }
//     }
//   }
//   mySwitch->setReceiveProtocolMask(pCONT_set->Settings.rf_protocol_mask);
// //  AddLog(LOG_LEVEL_INFO, PSTR("RFR: CmndRfProtocol:: Start responce"));
//   Response_P(PSTR("{\"" D_CMND_RFPROTOCOL "\":\""));
//   bool gotone = false;
//   thisdat = 1;
//   for (uint32_t i = 0; i < mySwitch->getNumProtos(); i++) {
//     if (pCONT_set->Settings.rf_protocol_mask & thisdat) {
//       ResponseAppend_P(PSTR("%s%d"), (gotone) ? "," : "", i+1);
//       gotone = true;
//     }
//     thisdat <<=1;
//   }
//   if (!gotone) { ResponseAppend_P(PSTR(D_JSON_NONE_ENABLED)); }
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
//       data = root.getULong(PSTR(D_JSON_RF_DATA), data);	// read payload data even >32bit
//       bits = root.getUInt(PSTR(D_JSON_RF_BITS), bits);
//       protocol = root.getInt(PSTR(D_JSON_RF_PROTOCOL), protocol);
//       repeat = root.getInt(PSTR(D_JSON_RF_REPEAT), repeat);
//       pulse = root.getInt(PSTR(D_JSON_RF_PULSE), pulse);
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
//     Response_P(PSTR("{\"" D_CMND_RFSEND "\":\"" D_JSON_NO " " D_JSON_RF_DATA ", " D_JSON_RF_BITS ", " D_JSON_RF_PROTOCOL ", " D_JSON_RF_REPEAT " " D_JSON_OR " " D_JSON_RF_PULSE "\"}"));
//   }
// }

// void CmndRfTimeOut(void) {
//   if (XdrvMailbox.payload >= 10) {
//     pCONT_set->Settings.rf_duplicate_time = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(pCONT_set->Settings.rf_duplicate_time);
// }


#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH