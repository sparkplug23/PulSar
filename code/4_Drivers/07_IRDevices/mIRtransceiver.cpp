#include "mIRtransceiver.h"

#ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER

void mIRtransceiver::Pre_Init(void){

  // AddLog(LOG_LEVEL_INFO,PSTR("Pre_Init pCONT_pins->GetPin(GPIO_IRSEND]=%d\n\r\n\n\n\n\n\n"),pCONT_pins->GetPin(GPIO_IRSEND]);

  if (pCONT_pins->GetPin(GPIO_IRSEND_ID] < 99) {  // not set when 255
    pin = pCONT_pins->GetPin(GPIO_IRSEND_ID];
    AddLog(LOG_LEVEL_INFO,PSTR("pin[GPIO_IRSEND] %d"),pin);
  }
  #ifdef CLIMATE_DHT1_PIN
    pin = CLIMATE_DHT1_PIN;
  #endif
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_IRR "IRSend init, Pin = %d"),pin);

  pinMode(pin,OUTPUT);

}

void mIRtransceiver::TransmitCode(uint32_t code, uint8_t code_bits, uint8_t repeat_count, uint8_t _pin){

  uint8_t pin_local = pin;

  if(pin_local<0){
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_IRR "IRSend NOT init, Pin = %d"),pin);
    return; // pin not set
  }

  if(_pin == 255){ //use module pin
    pin_local = pin;
  }else{ //use passed in pin
    pin_local = _pin;
  }

  pinMode(pin_local,OUTPUT); //temp fix
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_IRR "IRSend init, pin_local = %d"),pin_local);

  uint16_t timetmp;
  #ifdef ESP32
    IRsend irsend;
  #endif
  #ifdef ESP8266
    IRsend irsend(pin_local);  // Set the GPIO to be used to sending the message.
  #endif

  while(repeat_count--){
    irsend.enableIROut(38);//6);
    for(int ii=0;ii<code_bits;ii++){
      timetmp = ((code>>(code_bits-1-ii))&(0x01))>0 ? ONE_BIT_TIME : ZERO_BIT_TIME;
      if(ii & 1){ irsend.space(timetmp); }else{ irsend.mark(timetmp); }
    }
    irsend.space(2000); // Last bit should be space to turn IR off, also needs to be longer for EOF
  }

}




int8_t mIRtransceiver::Tasker(uint8_t func){

  switch(func){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      //init();
    break;
  }
  
  return 0;

}//end function


void mIRtransceiver::parse_JSONCommand(char* topic, char* payload,unsigned int length){}


#endif

// #ifdef ESP8266

// #ifdef USE_IR_REMOTE
// /*********************************************************************************************\
//  * IR Remote send and receive using IRremoteESP8266 library
// \*********************************************************************************************/

// #define XDRV_05             5

// #include <IRremoteESP8266.h>

// enum IrErrors { IE_NO_ERROR, IE_INVALID_RAWDATA, IE_INVALID_JSON, IE_SYNTAX_IRSEND, IE_SYNTAX_IRHVAC };

// enum IrRemoteCommands { CMND_IRSEND, CMND_IRHVAC };
// const char kIrRemoteCommands[] PROGMEM = D_IRSEND "|" D_IRHVAC ;

// // Based on IRremoteESP8266.h enum decode_type_t
// const char kIrRemoteProtocols[] PROGMEM =
//   "UNKNOWN|RC5|RC6|NEC|SONY|PANASONIC|JVC|SAMSUNG|WHYNTER|AIWA_RC_T501|LG|SANYO|MITSUBISHI|DISH|SHARP";

// #ifdef USE_IR_HVAC

// #include <ir_Mitsubishi.h>
// #include <ir_Fujitsu.h>

// enum IrHvacVendors { VNDR_TOSHIBA, VNDR_MITSUBISHI, VNDR_LG, VNDR_FUJITSU };
// const char kIrHvacVendors[] PROGMEM = "Toshiba|Mitsubishi|LG|Fujitsu" ;

// // HVAC TOSHIBA_
// const uint16_t HVAC_TOSHIBA_HDR_MARK = 4400;
// const uint16_t HVAC_TOSHIBA_HDR_SPACE = 4300;
// const uint16_t HVAC_TOSHIBA_BIT_MARK = 543;
// const uint16_t HVAC_TOSHIBA_ONE_SPACE = 1623;
// const uint16_t HVAC_MISTUBISHI_ZERO_SPACE = 472;
// const uint16_t HVAC_TOSHIBA_RPT_MARK = 440;
// const uint16_t HVAC_TOSHIBA_RPT_SPACE = 7048; // Above original iremote limit
// const uint8_t HVAC_TOSHIBA_DATALEN = 9;

// // HVAC LG
// const uint8_t HVAC_LG_DATALEN = 7;

// IRMitsubishiAC *mitsubir = nullptr;

// const char kFanSpeedOptions[] = "A12345S";
// const char kHvacModeOptions[] = "HDCA";
// #endif  // USE_IR_HVAC

// /*********************************************************************************************\
//  * IR Send
// \*********************************************************************************************/

// #include <IRsend.h>

// IRsend *irsend = nullptr;
// bool irsend_active = false;

// void IrSendInit(void)
// {
//   irsend = new IRsend(pin[GPIO_IRSEND]); // an IR led is at GPIO_IRSEND
//   irsend->begin();

// #ifdef USE_IR_HVAC
//   mitsubir = new IRMitsubishiAC(pin[GPIO_IRSEND]);
// #endif //USE_IR_HVAC
// }

// char* IrUint64toHex(uint64_t value, char *str, uint16_t bits)
// {
//   ulltoa(value, str, 16);  // Get 64bit value

//   int fill = 8;
//   if ((bits > 3) && (bits < 65)) {
//     fill = bits / 4;  // Max 16
//     if (bits % 4) { fill++; }
//   }
//   int len = strlen(str);
//   fill -= len;
//   if (fill > 0) {
//     memmove(str + fill, str, len +1);
//     memset(str, '0', fill);
//   }
//   memmove(str + 2, str, strlen(str) +1);
//   str[0] = '0';
//   str[1] = 'x';
//   return str;
// }

// #ifdef USE_IR_RECEIVE
// /*********************************************************************************************\
//  * IR Receive
// \*********************************************************************************************/

// const bool IR_RCV_SAVE_BUFFER = false;         // false = do not use buffer, true = use buffer for decoding
// const uint32_t IR_TIME_AVOID_DUPLICATE = 500;  // Milliseconds

// #include <IRrecv.h>

// IRrecv *irrecv = nullptr;

// unsigned long ir_lasttime = 0;

// void IrReceiveInit(void)
// {
//   // an IR led is at GPIO_IRRECV
//   irrecv = new IRrecv(pin[GPIO_IRRECV], IR_RCV_BUFFER_SIZE, IR_RCV_TIMEOUT, IR_RCV_SAVE_BUFFER);
//   irrecv->setUnknownThreshold(IR_RCV_MIN_UNKNOWN_SIZE);
//   irrecv->enableIRIn();                  // Start the receiver

//   //  ALOG_DBG(PSTR("IrReceive initialized"));
// }

// void IrReceiveCheck(void)
// {
//   char sirtype[14];  // Max is AIWA_RC_T501
//   int8_t iridx = 0;

//   decode_results results;

//   if (irrecv->decode(&results)) {
//     char hvalue[64];
//     IrUint64toHex(results.value, hvalue, results.bits);  // Get 64bit value as hex 0x00123456

//     AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_IRR "Echo %d, RawLen %d, Overflow %d, Bits %d, Value %s, Decode %d"),
//               irsend_active, results.rawlen, results.overflow, results.bits, hvalue, results.decode_type);

//     unsigned long now = millis();
// //    if ((now - ir_lasttime > IR_TIME_AVOID_DUPLICATE) && (UNKNOWN != results.decode_type) && (results.bits > 0)) {
//     if (!irsend_active && (now - ir_lasttime > IR_TIME_AVOID_DUPLICATE)) {
//       ir_lasttime = now;

//       iridx = results.decode_type;
//       if ((iridx < 0) || (iridx > 14)) { iridx = 0; }  // UNKNOWN
//       char svalue[64];
//       if (Settings.flag_system.ir_receive_decimal) {
//         ulltoa(results.value, svalue, 10);
//       } else {
//         snprintf_P(svalue, sizeof(svalue), PSTR("\"%s\""), hvalue);
//       }
//       Response_P(PSTR("{\"" D_IRRECEIVED "\":{\"" D_IR_PROTOCOL "\":\"%s\",\"" D_IR_BITS "\":%d,\"" D_IR_DATA "\":%s"),
//         GetTextIndexed_P(sirtype, sizeof(sirtype), iridx, kIrRemoteProtocols), results.bits, svalue);

//       if (Settings.flag_network.receive_raw) {
//         ResponseAppend_P(PSTR(",\"" D_IR_RAWDATA "\":["));
//         uint16_t i;
//         for (i = 1; i < results.rawlen; i++) {
//           if (i > 1) { ResponseAppend_P(PSTR(",")); }
//           uint32_t usecs;
//           for (usecs = results.rawbuf[i] * kRawTick; usecs > UINT16_MAX; usecs -= UINT16_MAX) {
//             ResponseAppend_P(PSTR("%d,0,"), UINT16_MAX);
//           }
//           ResponseAppend_P(PSTR("%d"), usecs);
//           if (strlen(data_buffer.payload.ctr) > sizeof(data_buffer.payload.ctr) - 40) { break; }  // Quit if char string becomes too long
//         }
//         uint16_t extended_length = results.rawlen - 1;
//         for (uint16_t j = 0; j < results.rawlen - 1; j++) {
//           uint32_t usecs = results.rawbuf[j] * kRawTick;
//           // Add two extra entries for multiple larger than UINT16_MAX it is.
//           extended_length += (usecs / (UINT16_MAX + 1)) * 2;
//         }
//         ResponseAppend_P(PSTR("],\"" D_IR_RAWDATA "Info\":[%d,%d,%d]"), extended_length, i -1, results.overflow);
//       }

//       ResponseAppend_P(PSTR("}}"));
//       MqttPublishPrefixTopic_P(RESULT_OR_TELE, PSTR(D_IRRECEIVED));

//       if (iridx) {
//         XdrvRulesProcess();
// #ifdef USE_DOMOTICZ
//         unsigned long value = results.value | (iridx << 28);  // [Protocol:4, Data:28]
//       //  DomoticzSensor(DZ_COUNT, value);                      // Send data as Domoticz Counter value
// #endif  // USE_DOMOTICZ
//       }
//     }

//     irrecv->resume();
//   }
// }
// #endif // USE_IR_RECEIVE


// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/

// /*
//  * ArduinoJSON entry used to calculate jsonBuf: JSON_OBJECT_SIZE(3) + 40 = 96
//  IRsend:
//  { "protocol": "RC5", "bits": 12, "data":"0xC86" }
//  { "protocol": "SAMSUNG", "bits": 32, "data": 551502015 }
//  IRhvac:
//  { "Vendor": "<Toshiba|Mitsubishi>", "Power": <0|1>, "Mode": "<Hot|Cold|Dry|Auto>", "FanSpeed": "<1|2|3|4|5|Auto|Silence>", "Temp": <17..30> }
// */

// bool IrSendCommand(void)
// {
//   char command [CMDSZ];
//   bool serviced = true;
//   uint8_t error = IE_NO_ERROR;

//   int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kIrRemoteCommands);
//   if (-1 == command_code) {
//     serviced = false;  // Unknown command
//   }
//   else if (CMND_IRSEND == command_code) {
//     if (XdrvMailbox.data_len) {
//       Response_P(S_JSON_COMMAND_SVALUE, command, D_DONE);

//       if (strstr(XdrvMailbox.data, "{") == nullptr) {  // If no JSON it must be rawdata
//         // IRsend <freq>,<rawdata>,<rawdata> ...
//         // or
//         // IRsend raw,<freq>,<zero space>,<bit stream> (one space = zero space *2)
//         // IRsend raw,<freq>,<zero space>,<zero space multiplier becoming one space>,<bit stream>
//         // IRsend raw,<freq>,<zero space>,<one space>,<bit stream>
//         // IRsend raw,<freq>,<header mark>,<header space>,<bit mark>,<zero space>,<one space>,<bit stream>
//         char *p;
//         char *str = strtok_r(XdrvMailbox.data, ", ", &p);
//         if (p == nullptr) {
//           error = IE_INVALID_RAWDATA;
//         } else {
//           uint16_t freq = atoi(str);
//           if (!freq && (*str != '0')) {                // First parameter is any string
//             uint16_t count = 0;
//             char *q = p;
//             for (; *q; count += (*q++ == ','));
//             if (count < 2) {                           // Parameters must be at least 3
//               error = IE_INVALID_RAWDATA;
//             } else {
//               uint16_t parm[count];
//               for (uint8_t i = 0; i < count; i++) {
//                 parm[i] = strtol(strtok_r(nullptr, ", ", &p), nullptr, 0);
//                 if (!parm[i]) {
//                   if (!i) {
//                     parm[0] = 38000;                   // Frequency default to 38kHz
//                   } else {
//                     error = IE_INVALID_RAWDATA;        // Other parameters may not be 0
//                     break;
//                   }
//                 }
//               }
//               if (IE_NO_ERROR == error) {
//                 uint16_t i = 0;
//                 if (count < 4) {
//                   // IRsend raw,0,889,000000100110000001001
//                   uint16_t mark = parm[1] *2;          // Protocol where 0 = t, 1 = 2t (RC5)
//                   if (3 == count) {
//                     if (parm[2] < parm[1]) {
//                       // IRsend raw,0,889,2,000000100110000001001
//                       mark = parm[1] * parm[2];        // Protocol where 0 = t1, 1 = t1*t2 (Could be RC5)
//                     } else {
//                       // IRsend raw,0,889,1778,000000100110000001001
//                       mark = parm[2];                  // Protocol where 0 = t1, 1 = t2 (Could be RC5)
//                     }
//                   }
//                   uint16_t raw_array[strlen(p)];       // Bits
//                   for (; *p; *p++) {
//                     if (*p == '0') {
//                       raw_array[i++] = parm[1];        // Space
//                     }
//                     else if (*p == '1') {
//                       raw_array[i++] = mark;           // Mark
//                     }
//                   }
//                   irsend_active = true;
//                   irsend->sendRaw(raw_array, i, parm[0]);
//                 }
//                 else if (6 == count) {                 // NEC Protocol
//                   // IRsend raw,0,8620,4260,544,411,1496,010101101000111011001110000000001100110000000001100000000000000010001100
//                   uint16_t raw_array[strlen(p)*2+3];   // Header + bits + end
//                   raw_array[i++] = parm[1];            // Header mark
//                   raw_array[i++] = parm[2];            // Header space
//                   for (; *p; *p++) {
//                     if (*p == '0') {
//                       raw_array[i++] = parm[3];        // Bit mark
//                       raw_array[i++] = parm[4];        // Zero space
//                     }
//                     else if (*p == '1') {
//                       raw_array[i++] = parm[3];        // Bit mark
//                       raw_array[i++] = parm[5];        // One space
//                     }
//                   }
//                   raw_array[i++] = parm[3];            // Trailing mark
//                   irsend_active = true;
//                   irsend->sendRaw(raw_array, i, parm[0]);
//                 }
//                 else {
//                   error = IE_INVALID_RAWDATA;          // Invalid number of parameters
//                 }
//               }
//             }
//           } else {
//             if (!freq) { freq = 38000; }  // Default to 38kHz
//             uint16_t count = 0;
//             char *q = p;
//             for (; *q; count += (*q++ == ','));
//             if (0 == count) {
//               error = IE_INVALID_RAWDATA;
//             } else {  // At least two raw data values
//               // IRsend 0,896,876,900,888,894,876,1790,874,872,1810,1736,948,872,880,872,936,872,1792,900,888,1734
//               count++;
//               uint16_t raw_array[count];  // It's safe to use stack for up to 240 packets (limited by data_buffer.payload.ctr length)
//               for (uint16_t i = 0; i < count; i++) {
//                 raw_array[i] = strtol(strtok_r(nullptr, ", ", &p), nullptr, 0);  // Allow decimal (20496) and hexadecimal (0x5010) input
//               }
//               irsend_active = true;
//               irsend->sendRaw(raw_array, count, freq);
//             }
//           }
//         }
//       } else {
//         char dataBufUc[XdrvMailbox.data_len];
//         UpperCase(dataBufUc, XdrvMailbox.data);
//         RemoveSpace(dataBufUc);
//         if (strlen(dataBufUc) < 8) {
//           error = IE_INVALID_JSON;
//         } else {
//           StaticJsonBuffer<128> jsonBuf;
//           JsonObject &root = jsonBuf.parseObject(dataBufUc);
//           if (!root.success()) {
//             error = IE_INVALID_JSON;
//           } else {
//             // IRsend { "protocol": "SAMSUNG", "bits": 32, "data": 551502015 }
//             char parm_uc[10];
//             const char *protocol = root[UpperCase_P(parm_uc, PSTR(D_IR_PROTOCOL))];
//             uint16_t bits = root[UpperCase_P(parm_uc, PSTR(D_IR_BITS))];
//             uint64_t data = strtoull(root[UpperCase_P(parm_uc, PSTR(D_IR_DATA))], nullptr, 0);
//             if (protocol && bits) {
//               char protocol_text[20];
//               int protocol_code = GetCommandCode(protocol_text, sizeof(protocol_text), protocol, kIrRemoteProtocols);

//               char dvalue[64];
//               char hvalue[64];
//               AddLog_P2(LOG_LEVEL_DEBUG, PSTR("IRS: protocol_text %s, protocol %s, bits %d, data %s (%s), protocol_code %d"),
//                 protocol_text, protocol, bits, ulltoa(data, dvalue, 10), IrUint64toHex(data, hvalue, bits), protocol_code);

//               irsend_active = true;
//               switch (protocol_code) {
//                 case NEC:
//                   irsend->sendNEC(data, (bits > NEC_BITS) ? NEC_BITS : bits); break;
//                 case SONY:
//                   irsend->sendSony(data, (bits > SONY_20_BITS) ? SONY_20_BITS : bits, 2); break;
//                 case RC5:
//                   irsend->sendRC5(data, bits); break;
//                 case RC6:
//                   irsend->sendRC6(data, bits); break;
//                 case DISH:
//                   irsend->sendDISH(data, (bits > DISH_BITS) ? DISH_BITS : bits); break;
//                 case JVC:
//                   irsend->sendJVC(data, (bits > JVC_BITS) ? JVC_BITS : bits, 1); break;
//                 case SAMSUNG:
//                   irsend->sendSAMSUNG(data, (bits > SAMSUNG_BITS) ? SAMSUNG_BITS : bits); break;
//                 case PANASONIC:
// //                  irsend->sendPanasonic(bits, data); break;
//                   irsend->sendPanasonic64(data, bits); break;
//                 default:
//                   irsend_active = false;
//                   Response_P(S_JSON_COMMAND_SVALUE, command, D_PROTOCOL_NOT_SUPPORTED);
//               }
//             } else {
//               error = IE_SYNTAX_IRSEND;
//             }
//           }
//         }
//       }
//     } else {
//       error = IE_SYNTAX_IRSEND;
//     }
//   }
//   else serviced = false;  // Unknown command

//   switch (error) {
//     case IE_INVALID_RAWDATA:
//       Response_P(S_JSON_COMMAND_SVALUE, command, D_INVALID_RAWDATA);
//       break;
//     case IE_INVALID_JSON:
//       Response_P(S_JSON_COMMAND_SVALUE, command, D_INVALID_JSON);
//       break;
//     case IE_SYNTAX_IRSEND:
//       Response_P(PSTR("{\"" D_IRSEND "\":\"" D_NO " " D_IR_PROTOCOL ", " D_IR_BITS " " D_OR " " D_IR_DATA "\"}"));
//       break;
// #ifdef USE_IR_HVAC
//     case IE_SYNTAX_IRHVAC:
//       Response_P(PSTR("{\"" D_IRHVAC "\":\"" D_WRONG " " D_IRHVAC_VENDOR ", " D_IRHVAC_MODE " " D_OR " " D_IRHVAC_FANSPEED "\"}"));
//       break;
// #endif // USE_IR_HVAC
//   }

//   return serviced;
// }

// /*********************************************************************************************\
//  * Interface
// \*********************************************************************************************/

// bool Xdrv05(uint8_t function)
// {
//     DEBUGLLN("Xdrv05 - START");
//   bool result = false;

//   if ((pin[GPIO_IRSEND] < 99) || (pin[GPIO_IRRECV] < 99)) {
//     switch (function) {
//       case TASK_PRE_INIT:
//         if (pin[GPIO_IRSEND] < 99) {
//           IrSendInit();
//         }
// #ifdef USE_IR_RECEIVE
//         if (pin[GPIO_IRRECV] < 99) {
//           IrReceiveInit();
//         }
// #endif  // USE_IR_RECEIVE
//         break;
//       case TASK_EVERY_50_MSECOND:
// #ifdef USE_IR_RECEIVE
//         if (pin[GPIO_IRRECV] < 99) {
//           IrReceiveCheck();  // check if there's anything on IR side
//         }
// #endif  // USE_IR_RECEIVE
//         irsend_active = false;  // re-enable IR reception
//         break;
//       case TASK_COMMAND:
//         if (pin[GPIO_IRSEND] < 99) {
//           result = IrSendCommand();
//         }
//         break;
//     }
//   }
//     DEBUGLLN("Xdrv05 - END");
//   return result;
// }

// #endif // USE_IR_REMOTE
// #endif