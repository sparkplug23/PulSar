/**
 * @file mCellular.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-29
 * 
 * @note Cellular should only be handling interface level. 
 *       In relation to GPS, it should be read here if available then inserted into sensor interface
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mCellular.h"


/**
 * @brief 
 * 
 * If modem connection is handled by another task pinned to the other core, then any slow downs will not hold the main processing back
 * Leave this until I better understand pinning tasks and can make them work well
 * 
 */

#ifdef USE_MODULE_NETWORK_CELLULAR





#endif // USE_MODULE_NETWORK_CELLULAR





// void mCellular::Handler_ModemResponses_Fast_PDU()
// {

//   // Response
//   // String incoming = String();
//   // SerialAT.setTimeout(200);


// /**
//  * @brief 
//  * 
//   >>> Example: Test from my phone "0000012345432100000" <<<
 
//  +CMT: ,36
//   0791448720003023040C914457515358790000325060717104401330180C068BC966B41A6D268BC16030180C
 
//   // Decoded with https://www.diafaan.com/sms-tutorials/gsm-modem-tutorial/online-sms-pdu-decoder/
//   Text message
//   From:	+447515358597
//   Message:	0000012345432100000

//   Additional information
//   PDU type:	SMS-DELIVER
//   Time stamp:	06/05/2023 17:17:40
//   SMSC:	+447802000332
//   Data coding:	SMS Default Alphabet

//   Original Encoded PDU fields
//   SMSC:	0791448720003023
//   PDU header:	04
//   TP-MTI:	00
//   TP-MMS:	04
//   TP-SRI:	00
//   TP-RP:	00
//   TP-UDHI:00
//   TP-OA:	0C91445751535879
//   TP-PID: 00
//   TP-DCS: 00
//   TP-SCTS:32506071710440
//   TP-UDL:	13
//   TP-UD:	30180C068BC966B41A6D268BC16030180C
    
//   07 - Length of the SMSC information (in this case 7 octets)
//   91 - Type-of-address of the SMSC. (91 means international format of the phone number)
//   44 87 20 00 30 23 - Service center number(in decimal semi-octets). The length of the phone  number is odd (11), so a trailing F has been added to form proper octets. 
//                       The  phone number of this service center is "+27831000015". See below.
//   04 - First octet of this SMS-DELIVER message.
//   0C - Address-Length. Length of the sender number (0B hex = 11 dec)
//   91 - Type-of-address of the sender number
//   44 57 51 53 58 79 - Sender number (decimal semi-octets), with a trailing F
//   00
//   00
//   32 50 60 71 71 04 40
//   13
//   30180C068BC966B41A6D268BC16030180C

// ***/




//   if (SerialAT.available()) 
//   {


//     uint32_t receive_millis = millis();

//     Serial.write(Serial1.read());

//     // ALOG_INF(PSTR("millisA = %d"), millis());

//     // char buffer[300];

//     // // Add timeout
//     // int c = SerialAT.read();
//     // while(c >= 0) {
//     //   incoming += (char) c;
//     //   c = SerialAT.read();
//     // }
    
//     // // ALOG_HGL(PSTR("while\nIncoming \n\r===(%s)==="), incoming.c_str());

//     // // ALOG_HGL(PSTR("Incoming \n\r===(%s)==="), incoming.c_str());

//     // // ALOG_INF(PSTR("millisB = %d\tR\t%d"), millis(), millis()-receive_millis);
//     // char buffer2[300];
//     // // ALOG_INF(PSTR("millisC = %d"), millis());

//     // sprintf(buffer, "%s", incoming.c_str());

//     // ALOG_INF(PSTR("buffer = %s"), buffer);
//     // for(int i = 0;i < incoming.length(); i++)
//     // {
//     //   Serial.printf("%02d>    %c       \n\r",i,buffer[i]);
//     //   if(buffer[i]=='\n')
//     //   {
//     //     Serial.printf("=====================%02d>\"%c\"\n\r",i,buffer[i]);
//     //   }
//     // }

//     // char *search = "\r\n+CMT";
//     // char *result = strstr(buffer, search);
//     // if(result)
//     // {
//     //   ALOG_INF(PSTR("FOUND CMT MESSAGE result >>>%s<<<"), result);
//     //   ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));
//     // }

//     // ALOG_INF(PSTR("buffer2 >>>%s<<<"), buffer2);
    
//     // // ALOG_INF(PSTR("millisD = %d"), millis());

//     // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//     // // Check for request at GPS
//     // if (strncasecmp(buffer2, "GPS", 3) == 0)
//     // {
//     //   ALOG_INF(PSTR("Request for GPS"));
//     //   SMS_GPSLocation();
//     // }
//     // else{
//     //   ALOG_INF(PSTR("Unknown Message"));
//     //   // SMS_GPSLocation();
//     // }
//     // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

//   }

// }