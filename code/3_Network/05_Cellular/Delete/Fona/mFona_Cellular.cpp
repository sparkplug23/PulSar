/*
  mFona_Cellular - infra red support

  Copyright (C) 2022  Heiko Krupp, Lazar Obradovic, Theo Arends and Michael Doone

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
*/

/**
 * @file mFona_Cellular.cpp
 * 
 * Step 1 : Always getting to a configured state of being connected
 * Step 2 : An incoming text message, from my phone number (must check its my phone with "GPS") will reply with dummy gps positions
 * 
 * 
 * 
 * 
 */

#include "mFona_Cellular.h"

#ifdef USE_MODULE_DRIVERS_FONA_CELLULAR


int8_t mFona_Cellular::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;
    case TASK_EVERY_SECOND: 

    break;
    case TASK_EVERY_FIVE_SECOND:{

      ALOG_INF( PSTR("PS:%d"), digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_STATUS__ID)) );
      ALOG_INF( PSTR("RI:%d"), digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_RING_INDICATOR__ID)) );

      connection.rssi_dBm = CommandGet_RSSI_dBm();

      ALOG_INF( PSTR("rssi_dbm = %d"), connection.rssi_dBm );

      Command('n');
      Command('R');

    }
    break;
    case TASK_EVERY_MINUTE:{
       SubTask_CheckConnection();
    }
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
  }





//   bool result = false;

//   if (PinUsed(GPIO_IRSEND) || PinUsed(GPIO_IRRECV)) {
//     switch (function) {
//       case TASK_PRE_INIT:
//         break;
//       case TASK_EVERY_50_MSECOND:
// #ifdef USE_IR_RECEIVE
//         if (PinUsed(GPIO_IRRECV)) {
//           IrReceiveCheck();  // check if there's anything on IR side
//         }
// #endif  // USE_IR_RECEIVE
//         break;
//       case TASK_COMMAND:
//         if (PinUsed(GPIO_IRSEND)) {
//           result = DecodeCommand(kIrRemoteCommands, IrRemoteCommand);
//         }
//         break;
//     }
//   }
//   return result;

} // END function


void mFona_Cellular::Pre_Init(void)
{
  

  settings.fEnableSensor = true;


  // Use this for FONA 800 and 808s
  fona = new Adafruit_FONA(FONA_RST);
  // Use this one for FONA 3G
  //Adafruit_FONA_3G fona = Adafruit_FONA_3G(FONA_RST);

  if(pCONT_pins->PinUsed(GPIO_FUNCTION__FONA_RING_INDICATOR__ID))
  {
    pinMode(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_RING_INDICATOR__ID), INPUT);
  }

  if(pCONT_pins->PinUsed(GPIO_FUNCTION__FONA_POWER_KEY__ID))
  {
    pinMode(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_KEY__ID), OUTPUT);
  }

  connection.standy_mode_id = 0;


}

/**
 * @brief Must to atomic
 **/
void IRAM_ATTR ISR_Ring_Indicator_Triggered_2()
{
  pCONT_fona->connection.flag_ring_indicator = true;
}


void mFona_Cellular::Init(void)
{
  
  Serial.println(F("FONA basic test"));
  Serial.println(F("Initializing....(May take 3 seconds)"));

  fonaSerial->begin(4800);
  if (! fona->begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    // while (1);
    settings.fEnableSensor = false;
  }
  
  settings.fEnableSensor = true;

  type = fona->type();
  Serial.println(F("FONA is OK"));
  Serial.print(F("Found "));
  switch (type) {
    case FONA800L:
      Serial.println(F("FONA 800L")); break;
    case FONA800H:
      Serial.println(F("FONA 800H")); break;
    case FONA808_V1:
      Serial.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      Serial.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
      Serial.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
      Serial.println(F("FONA 3G (European)")); break;
    default: 
      Serial.println(F("???")); break;
  }
  
  // Print module IMEI number.
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona->getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }

  // Optionally configure a GPRS APN, username, and password.
  // You might need to do this to access your network's GPRS/data
  // network.  Contact your provider for the exact APN, username,
  // and password values.  Username and password are optional and
  // can be removed, but APN is required.
  fona->setGPRSNetworkSettings(F("giffgaff.com"), F("gg"), F("p"));

  // Optionally configure HTTP gets to follow redirects over SSL.
  // Default is not to follow SSL redirects, however if you uncomment
  // the following line then redirects over SSL will be followed.
  //fona->setHTTPSRedirect(true);

  printMenu();



  if(pCONT_pins->PinUsed(GPIO_FUNCTION__FONA_RING_INDICATOR__ID))
  {
    // pinMode(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_RING_INDICATOR__ID), INPUT);
    attachInterrupt(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_RING_INDICATOR__ID), ISR_Ring_Indicator_Triggered_2, CHANGE);

  }


}



void mFona_Cellular::ISR_Ring_Indicator_Triggered()
{


}



void mFona_Cellular::EveryLoop()
{

  if(pCONT_fona->connection.flag_ring_indicator)
  {
    // ALOG_INF(PSTR("pCONT_fona->connection.flag_ring_indicator SET"));
  }

  /**
   * @brief Setting desired device state, ie power_on, then this code will attempt to configure it and check relevant pins
   * 
   */


}




void mFona_Cellular::printMenu(void) {
  Serial.println(F("-------------------------------------"));
  Serial.println(F("[?] Print this menu"));
  Serial.println(F("[a] read the ADC 2.8V max (FONA800 & 808)"));
  Serial.println(F("[b] read the Battery V and % charged"));
  Serial.println(F("[C] read the SIM CCID"));
  Serial.println(F("[U] Unlock SIM with PIN code"));
  Serial.println(F("[i] read RSSI"));
  Serial.println(F("[n] get Network status"));
  Serial.println(F("[v] set audio Volume"));
  Serial.println(F("[V] get Volume"));
  Serial.println(F("[H] set Headphone audio (FONA800 & 808)"));
  Serial.println(F("[e] set External audio (FONA800 & 808)"));
  Serial.println(F("[T] play audio Tone"));
  Serial.println(F("[P] PWM/Buzzer out (FONA800 & 808)"));

  // FM (SIM800 only!)
  Serial.println(F("[f] tune FM radio (FONA800)"));
  Serial.println(F("[F] turn off FM (FONA800)"));
  Serial.println(F("[m] set FM volume (FONA800)"));
  Serial.println(F("[M] get FM volume (FONA800)"));
  Serial.println(F("[q] get FM station signal level (FONA800)"));

  // Phone
  Serial.println(F("[c] make phone Call"));
  Serial.println(F("[A] get call status"));
  Serial.println(F("[h] Hang up phone"));
  Serial.println(F("[p] Pick up phone"));

  // SMS
  Serial.println(F("[N] Number of SMSs"));
  Serial.println(F("[r] Read SMS #"));
  Serial.println(F("[R] Read All SMS"));
  Serial.println(F("[d] Delete SMS #"));
  Serial.println(F("[s] Send SMS"));
  Serial.println(F("[u] Send USSD"));
  
  // Time
  Serial.println(F("[y] Enable network time sync (FONA 800 & 808)"));
  Serial.println(F("[Y] Enable NTP time sync (GPRS FONA 800 & 808)"));
  Serial.println(F("[t] Get network time"));

  // GPRS
  Serial.println(F("[G] Enable GPRS"));
  Serial.println(F("[g] Disable GPRS"));
  Serial.println(F("[l] Query GSMLOC (GPRS)"));
  Serial.println(F("[w] Read webpage (GPRS)"));
  Serial.println(F("[W] Post to website (GPRS)"));

  // GPS
  // if ((type == FONA3G_A) || (type == FONA3G_E) || (type == FONA808_V1) || (type == FONA808_V2)) {
  //   Serial.println(F("[O] Turn GPS on (FONA 808 & 3G)"));
  //   Serial.println(F("[o] Turn GPS off (FONA 808 & 3G)"));
  //   Serial.println(F("[L] Query GPS location (FONA 808 & 3G)"));
  //   if (type == FONA808_V1) {
  //     Serial.println(F("[x] GPS fix status (FONA808 v1 only)"));
  //   }
  //   Serial.println(F("[E] Raw NMEA out (FONA808)"));
  // }
  
  Serial.println(F("[S] create Serial passthru tunnel"));
  Serial.println(F("-------------------------------------"));
  Serial.println(F(""));

}



// void loop() {
//   Serial.print(F("FONA> "));
//   while (! Serial.available() ) {
//     if (fona->available()) {
//       Serial.write(fona->read());
//     }
//   }

//   char command = Serial.read();
//   Serial.println(command);

void mFona_Cellular::Command(char command)
{

  switch (command) {
    default:
    case '?': {
        printMenu();
        break;
      }

    case 'a': {
        // read the ADC
        uint16_t adc;
        if (! fona->getADCVoltage(&adc)) {
          Serial.println(F("Failed to read ADC"));
        } else {
          Serial.print(F("ADC = ")); Serial.print(adc); Serial.println(F(" mV"));
        }
        break;
      }

    case 'b': {
        // read the battery voltage and percentage
        uint16_t vbat;
        if (! fona->getBattVoltage(&vbat)) {
          Serial.println(F("Failed to read Batt"));
        } else {
          Serial.print(F("VBat = ")); Serial.print(vbat); Serial.println(F(" mV"));
        }


        if (! fona->getBattPercent(&vbat)) {
          Serial.println(F("Failed to read Batt"));
        } else {
          Serial.print(F("VPct = ")); Serial.print(vbat); Serial.println(F("%"));
        }

        break;
      }

//     case 'U': {
//         // Unlock the SIM with a PIN code
//         char PIN[5];
//         flushSerial();
//         Serial.println(F("Enter 4-digit PIN"));
//         readline(PIN, 3);
//         Serial.println(PIN);
//         Serial.print(F("Unlocking SIM card: "));
//         if (! fona->unlockSIM(PIN)) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("OK!"));
//         }
//         break;
//       }

//     case 'C': {
//         // read the CCID
//         fona->getSIMCCID(replybuffer);  // make sure replybuffer is at least 21 bytes!
//         Serial.print(F("SIM CCID = ")); Serial.println(replybuffer);
//         break;
//       }

//     case 'i': {
//         CommandGet_RSSI_dBm();
//         break;
//       }

    case 'n': {
        // read the network/cellular status
        uint8_t n = fona->getNetworkStatus();
        Serial.print(F("Network status "));
        Serial.print(n);
        Serial.print(F(": "));
        if (n == 0) Serial.println(F("Not registered"));
        if (n == 1) Serial.println(F("Registered (home)"));
        if (n == 2) Serial.println(F("Not registered (searching)"));
        if (n == 3) Serial.println(F("Denied"));
        if (n == 4) Serial.println(F("Unknown"));
        if (n == 5) Serial.println(F("Registered roaming"));
        break;
      }

//     /*** Audio ***/
//     case 'v': {
//         // set volume
//         flushSerial();
//         if ( (type == FONA3G_A) || (type == FONA3G_E) ) {
//           Serial.print(F("Set Vol [0-8] "));
//         } else {
//           Serial.print(F("Set Vol % [0-100] "));
//         }
//         uint8_t vol = readnumber();
//         Serial.println();
//         if (! fona->setVolume(vol)) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("OK!"));
//         }
//         break;
//       }

//     case 'V': {
//         uint8_t v = fona->getVolume();
//         Serial.print(v);
//         if ( (type == FONA3G_A) || (type == FONA3G_E) ) {
//           Serial.println(" / 8");
//         } else {
//           Serial.println("%");
//         }
//         break;
//       }

//     case 'H': {
//         // Set Headphone output
//         if (! fona->setAudio(FONA_HEADSETAUDIO)) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("OK!"));
//         }
//         fona->setMicVolume(FONA_HEADSETAUDIO, 15);
//         break;
//       }
//     case 'e': {
//         // Set External output
//         if (! fona->setAudio(FONA_EXTAUDIO)) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("OK!"));
//         }

//         fona->setMicVolume(FONA_EXTAUDIO, 10);
//         break;
//       }

//     case 'T': {
//         // play tone
//         flushSerial();
//         Serial.print(F("Play tone #"));
//         uint8_t kittone = readnumber();
//         Serial.println();
//         // play for 1 second (1000 ms)
//         if (! fona->playToolkitTone(kittone, 1000)) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("OK!"));
//         }
//         break;
//       }

//     /*** FM Radio ***/

//     case 'f': {
//         // get freq
//         flushSerial();
//         Serial.print(F("FM Freq (eg 1011 == 101.1 MHz): "));
//         uint16_t station = readnumber();
//         Serial.println();
//         // FM radio ON using headset
//         if (fona->FMradio(true, FONA_HEADSETAUDIO)) {
//           Serial.println(F("Opened"));
//         }
//         if (! fona->tuneFMradio(station)) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("Tuned"));
//         }
//         break;
//       }
//     case 'F': {
//         // FM radio off
//         if (! fona->FMradio(false)) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("OK!"));
//         }
//         break;
//       }
//     case 'm': {
//         // Set FM volume.
//         flushSerial();
//         Serial.print(F("Set FM Vol [0-6]:"));
//         uint8_t vol = readnumber();
//         Serial.println();
//         if (!fona->setFMVolume(vol)) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("OK!"));
//         }
//         break;
//       }
//     case 'M': {
//         // Get FM volume.
//         uint8_t fmvol = fona->getFMVolume();
//         if (fmvol < 0) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.print(F("FM volume: "));
//           Serial.println(fmvol, DEC);
//         }
//         break;
//       }
//     case 'q': {
//         // Get FM station signal level (in decibels).
//         flushSerial();
//         Serial.print(F("FM Freq (eg 1011 == 101.1 MHz): "));
//         uint16_t station = readnumber();
//         Serial.println();
//         int8_t level = fona->getFMSignalLevel(station);
//         if (level < 0) {
//           Serial.println(F("Failed! Make sure FM radio is on (tuned to station)."));
//         } else {
//           Serial.print(F("Signal level (dB): "));
//           Serial.println(level, DEC);
//         }
//         break;
//       }

//     /*** PWM ***/

//     case 'P': {
//         // PWM Buzzer output @ 2KHz max
//         flushSerial();
//         Serial.print(F("PWM Freq, 0 = Off, (1-2000): "));
//         uint16_t freq = readnumber();
//         Serial.println();
//         if (! fona->setPWM(freq)) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("OK!"));
//         }
//         break;
//       }

//     /*** Call ***/
//     case 'c': {
//         // call a phone!
//         char number[30];
//         flushSerial();
//         Serial.print(F("Call #"));
//         readline(number, 30);
//         Serial.println();
//         Serial.print(F("Calling ")); Serial.println(number);
//         if (!fona->callPhone(number)) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("Sent!"));
//         }

//         break;
//       }
//     case 'A': {
//         // get call status
//         int8_t callstat = fona->getCallStatus();
//         switch (callstat) {
//           case 0: Serial.println(F("Ready")); break;
//           case 1: Serial.println(F("Could not get status")); break;
//           case 3: Serial.println(F("Ringing (incoming)")); break;
//           case 4: Serial.println(F("Ringing/in progress (outgoing)")); break;
//           default: Serial.println(F("Unknown")); break;
//         }
//         break;
//       }
      
//     case 'h': {
//         // hang up!
//         if (! fona->hangUp()) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("OK!"));
//         }
//         break;
//       }

//     case 'p': {
//         // pick up!
//         if (! fona->pickUp()) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("OK!"));
//         }
//         break;
//       }

//     /*** SMS ***/

//     case 'N': {
//         // read the number of SMS's!
//         int8_t smsnum = fona->getNumSMS();
//         if (smsnum < 0) {
//           Serial.println(F("Could not read # SMS"));
//         } else {
//           Serial.print(smsnum);
//           Serial.println(F(" SMS's on SIM card!"));
//         }
//         break;
//       }
//     case 'r': {
//         // read an SMS
//         flushSerial();
//         Serial.print(F("Read #"));
//         uint8_t smsn = readnumber();
//         Serial.print(F("\n\rReading SMS #")); Serial.println(smsn);

//         // Retrieve SMS sender address/phone number.
//         if (! fona->getSMSSender(smsn, replybuffer, 250)) {
//           Serial.println("Failed!");
//           break;
//         }
//         Serial.print(F("FROM: ")); Serial.println(replybuffer);

//         // Retrieve SMS value.
//         uint16_t smslen;
//         if (! fona->readSMS(smsn, replybuffer, 250, &smslen)) { // pass in buffer and max len!
//           Serial.println("Failed!");
//           break;
//         }
//         Serial.print(F("***** SMS #")); Serial.print(smsn);
//         Serial.print(" ("); Serial.print(smslen); Serial.println(F(") bytes *****"));
//         Serial.println(replybuffer);
//         Serial.println(F("*****"));

//         break;
//       }
    case 'R': {
        // read all SMS
        int8_t smsnum = fona->getNumSMS();
        uint16_t smslen;
        int8_t smsn;

        if ( (type == FONA3G_A) || (type == FONA3G_E) ) {
          smsn = 0; // zero indexed
          smsnum--;
        } else {
          smsn = 1;  // 1 indexed
        }

        for ( ; smsn <= smsnum; smsn++) {
          Serial.print(F("\n\rReading SMS #")); Serial.println(smsn);
          if (!fona->readSMS(smsn, replybuffer, 250, &smslen)) {  // pass in buffer and max len!
            Serial.println(F("Failed!"));
            break;
          }
          // if the length is zero, its a special case where the index number is higher
          // so increase the max we'll look at!
          if (smslen == 0) {
            Serial.println(F("[empty slot]"));
            smsnum++;
            continue;
          }

          Serial.print(F("***** SMS #")); Serial.print(smsn);
          Serial.print(" ("); Serial.print(smslen); Serial.println(F(") bytes *****"));
          Serial.println(replybuffer);
          Serial.println(F("*****"));
        }
        break;
      }

//     case 'd': {
//         // delete an SMS
//         flushSerial();
//         Serial.print(F("Delete #"));
//         uint8_t smsn = readnumber();

//         Serial.print(F("\n\rDeleting SMS #")); Serial.println(smsn);
//         if (fona->deleteSMS(smsn)) {
//           Serial.println(F("OK!"));
//         } else {
//           Serial.println(F("Couldn't delete"));
//         }
//         break;
//       }

//     case 's': {
//         // send an SMS!
//         char sendto[21], message[141];
//         flushSerial();
//         Serial.print(F("Send to #"));
//         readline(sendto, 20);
//         Serial.println(sendto);
//         Serial.print(F("Type out one-line message (140 char): "));
//         readline(message, 140);
//         Serial.println(message);
//         if (!fona->sendSMS(sendto, message)) {
//           Serial.println(F("Failed"));
//         } else {
//           Serial.println(F("Sent!"));
//         }

//         break;
//       }

//     case 'u': {
//       // send a USSD!
//       char message[141];
//       flushSerial();
//       Serial.print(F("Type out one-line message (140 char): "));
//       readline(message, 140);
//       Serial.println(message);

//       uint16_t ussdlen;
//       if (!fona->sendUSSD(message, replybuffer, 250, &ussdlen)) { // pass in buffer and max len!
//         Serial.println(F("Failed"));
//       } else {
//         Serial.println(F("Sent!"));
//         Serial.print(F("***** USSD Reply"));
//         Serial.print(" ("); Serial.print(ussdlen); Serial.println(F(") bytes *****"));
//         Serial.println(replybuffer);
//         Serial.println(F("*****"));
//       }
//     }

//     /*** Time ***/

//     case 'y': {
//         // enable network time sync
//         if (!fona->enableNetworkTimeSync(true))
//           Serial.println(F("Failed to enable"));
//         break;
//       }

//     case 'Y': {
//         // enable NTP time sync
//         if (!fona->enableNTPTimeSync(true, F("pool.ntp.org")))
//           Serial.println(F("Failed to enable"));
//         break;
//       }

//     case 't': {
//         // read the time
//         char buffer[23];

//         fona->getTime(buffer, 23);  // make sure replybuffer is at least 23 bytes!
//         Serial.print(F("Time = ")); Serial.println(buffer);
//         break;
//       }


//     /*********************************** GPS (SIM808 only) */

//     case 'o': {
//         // turn GPS off
//         if (!fona->enableGPS(false))
//           Serial.println(F("Failed to turn off"));
//         break;
//       }
//     case 'O': {
//         // turn GPS on
//         if (!fona->enableGPS(true))
//           Serial.println(F("Failed to turn on"));
//         break;
//       }
//     case 'x': {
//         int8_t stat;
//         // check GPS fix
//         stat = fona->GPSstatus();
//         if (stat < 0)
//           Serial.println(F("Failed to query"));
//         if (stat == 0) Serial.println(F("GPS off"));
//         if (stat == 1) Serial.println(F("No fix"));
//         if (stat == 2) Serial.println(F("2D fix"));
//         if (stat == 3) Serial.println(F("3D fix"));
//         break;
//       }

//     case 'L': {
//         // check for GPS location
//         char gpsdata[120];
//         fona->getGPS(0, gpsdata, 120);
//         if (type == FONA808_V1)
//           Serial.println(F("Reply in format: mode,longitude,latitude,altitude,utctime(yyyymmddHHMMSS),ttff,satellites,speed,course"));
//         else 
//           Serial.println(F("Reply in format: mode,fixstatus,utctime(yyyymmddHHMMSS),latitude,longitude,altitude,speed,course,fixmode,reserved1,HDOP,PDOP,VDOP,reserved2,view_satellites,used_satellites,reserved3,C/N0max,HPA,VPA"));
//         Serial.println(gpsdata);

//         break;
//       }

//     case 'E': {
//         flushSerial();
//         if (type == FONA808_V1) {
//           Serial.print(F("GPS NMEA output sentences (0 = off, 34 = RMC+GGA, 255 = all)"));
//         } else {
//           Serial.print(F("On (1) or Off (0)? "));
//         }
//         uint8_t nmeaout = readnumber();

//         // turn on NMEA output
//         fona->enableGPSNMEA(nmeaout);

//         break;
//       }

//     /*********************************** GPRS */

//     case 'g': {
//         // turn GPRS off
//         if (!fona->enableGPRS(false))
//           Serial.println(F("Failed to turn off"));
//         break;
//       }
//     case 'G': {
//         // turn GPRS on
//         if (!fona->enableGPRS(true))
//           Serial.println(F("Failed to turn on"));
//         break;
//       }
//     case 'l': {
//         // check for GSMLOC (requires GPRS)
//         uint16_t returncode;

//         if (!fona->getGSMLoc(&returncode, replybuffer, 250))
//           Serial.println(F("Failed!"));
//         if (returncode == 0) {
//           Serial.println(replybuffer);
//         } else {
//           Serial.print(F("Fail code #")); Serial.println(returncode);
//         }

//         break;
//       }
//     case 'w': {
//         // read website URL
//         uint16_t statuscode;
//         int16_t length;
//         char url[80];

//         flushSerial();
//         Serial.println(F("NOTE: in beta! Use small webpages to read!"));
//         Serial.println(F("URL to read (e.g. wifitest.adafruit.com/testwifi/index.html):"));
//         Serial.print(F("http://")); readline(url, 79);
//         Serial.println(url);

//         Serial.println(F("****"));
//         if (!fona->HTTP_GET_start(url, &statuscode, (uint16_t *)&length)) {
//           Serial.println("Failed!");
//           break;
//         }
//         while (length > 0) {
//           while (fona->available()) {
//             char c = fona->read();

//             // Serial.write is too slow, we'll write directly to Serial register!
// #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//             loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
//             UDR0 = c;
// #else
//             Serial.write(c);
// #endif
//             length--;
//             if (! length) break;
//           }
//         }
//         Serial.println(F("\n****"));
//         fona->HTTP_GET_end();
//         break;
//       }

//     case 'W': {
//         // Post data to website
//         uint16_t statuscode;
//         int16_t length;
//         char url[80];
//         char data[80];

//         flushSerial();
//         Serial.println(F("NOTE: in beta! Use simple websites to post!"));
//         Serial.println(F("URL to post (e.g. httpbin.org/post):"));
//         Serial.print(F("http://")); readline(url, 79);
//         Serial.println(url);
//         Serial.println(F("Data to post (e.g. \"foo\" or \"{\"simple\":\"json\"}\"):"));
//         readline(data, 79);
//         Serial.println(data);

//         Serial.println(F("****"));
//         if (!fona->HTTP_POST_start(url, F("text/plain"), (uint8_t *) data, strlen(data), &statuscode, (uint16_t *)&length)) {
//           Serial.println("Failed!");
//           break;
//         }
//         while (length > 0) {
//           while (fona->available()) {
//             char c = fona->read();

// #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
//             loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
//             UDR0 = c;
// #else
//             Serial.write(c);
// #endif

//             length--;
//             if (! length) break;
//           }
//         }
//         Serial.println(F("\n****"));
//         fona->HTTP_POST_end();
//         break;
//       }
//     /*****************************************/

//     case 'S': {
//         Serial.println(F("Creating SERIAL TUBE"));
//         while (1) {
//           while (Serial.available()) {
//             delay(1);
//             fona->write(Serial.read());
//           }
//           if (fona->available()) {
//             Serial.write(fona->read());
//           }
//         }
//         break;
//       }

//     default: {
//         Serial.println(F("Unknown command"));
//         printMenu();
//         break;
//       }
//   }
//   // flush input
//   flushSerial();
//   while (fona->available()) {
//     Serial.write(fona->read());
//   }

  } //end switcj

} // Command

// void flushSerial() {
//   while (Serial.available())
//     Serial.read();
// }

// char readBlocking() {
//   while (!Serial.available());
//   return Serial.read();
// }
// uint16_t readnumber() {
//   uint16_t x = 0;
//   char c;
//   while (! isdigit(c = readBlocking())) {
//     //Serial.print(c);
//   }
//   Serial.print(c);
//   x = c - '0';
//   while (isdigit(c = readBlocking())) {
//     Serial.print(c);
//     x *= 10;
//     x += c - '0';
//   }
//   return x;
// }

// uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout) {
//   uint16_t buffidx = 0;
//   boolean timeoutvalid = true;
//   if (timeout == 0) timeoutvalid = false;

//   while (true) {
//     if (buffidx > maxbuff) {
//       //Serial.println(F("SPACE"));
//       break;
//     }

//     while (Serial.available()) {
//       char c =  Serial.read();

//       //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);

//       if (c == '\r') continue;
//       if (c == 0xA) {
//         if (buffidx == 0)   // the first 0x0A is ignored
//           continue;

//         timeout = 0;         // the second 0x0A is the end of the line
//         timeoutvalid = true;
//         break;
//       }
//       buff[buffidx] = c;
//       buffidx++;
//     }

//     if (timeoutvalid && timeout == 0) {
//       //Serial.println(F("TIMEOUT"));
//       break;
//     }
//     delay(1);
//   }
//   buff[buffidx] = 0;  // null term
//   return buffidx;
// }




/******************************************************************************************************************
 * 
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mFona_Cellular::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["LED"].getObject()["SetState"])
	{

		if(jtok.isNum())
		{
			// mySwitch->setReceiveProtocolMask(jtok.getUInt());
			mqtthandler_settings.flags.SendNow = true;
		}

		// JBI->Start();

		// pCONT->Tasker_Interface(TASK_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID);

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
    	// ALOG_TST(PSTR("RfMask = %d / %d"), jtok.getUInt(), mySwitch->GetReceiveProtolMask());
		// 	pCONT_mqtt->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
		// }

	}



int8_t led_id = -1;
uint16_t state_value = 0;

 if(jtok = obj["Fona"].getObject()["CommandList"])
 {
	// char buffer[10];
	const char* buf = jtok.getStr();
	Command(buf[0]);
  }


 if(jtok = obj["Fona"].getObject()["SendTextMessage"])
 {
	// char buffer[10];
	char* message = (char*)jtok.getStr();
	// Command(buf[0]);


	
	if (!fona->sendSMS(D_FLEXI_O2_PHONE_NUMBER_CTR, message)) {
		Serial.println(F("Failed"));
	} else {
		Serial.println(F("Sent!"));
	}


  }

 if(jtok = obj["Fona"].getObject()["CallPhone"])
 {
// 	// char buffer[10];
// 	char* message = (char*)jtok.getStr();
// 	// Command(buf[0]);


	
// 	if (!fona->sendSMS(D_FLEXI_O2_PHONE_NUMBER_CTR, message)) {
// 		Serial.println(F("Failed"));
// 	} else {
// 		Serial.println(F("Sent!"));
// 	}

	// call a phone!
        // char number[30];
        // flushSerial();
        Serial.print(F("Call #"));
        // readline(number, 30);
        // Serial.println();
        Serial.print(F("Calling ")); Serial.println(D_FLEXI_O2_PHONE_NUMBER_CTR);
        if (!fona->callPhone(D_FLEXI_O2_PHONE_NUMBER_CTR)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("Sent!"));
        }


  }


 if(jtok = obj["Fona"].getObject()["PowerCycle"])
 {
	digitalWrite(GPIO_FUNCTION__FONA_POWER_KEY__ID, LOW);
	delay(1000);
	digitalWrite(GPIO_FUNCTION__FONA_POWER_KEY__ID, HIGH);

	Serial.println(F("Power Cycle"));

	// ALOG_INF
	


  }


    
}
  
/**
 * @brief 
 * 
 * @return int8_t 
 */
int8_t mFona_Cellular::CommandGet_RSSI_dBm()
{

  uint8_t n = fona->getRSSI();
  int8_t r;
  
  if (n == 0) r = -115;
  if (n == 1) r = -111;
  if (n == 31) r = -52;
  if ((n >= 2) && (n <= 30))
  {
    r = map(n, 2, 30, -110, -54);
  }

  ALOG_INF( PSTR("Rssi = %d, dBm = %d"), n, r);

  return r;
}

/**
 * @brief Set power
 * 
 * @return true 
 * @return false 
 */
bool mFona_Cellular::CommandSet_Power()
{
  
  ALOG_INF( PSTR("CommandSet_Power PS:%d"), digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_STATUS__ID)) );

  if(digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_STATUS__ID)) == 1)
  {


  }





}



/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/


uint8_t mFona_Cellular::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    // JBI->Add(D_JSON_COUNT, settings.fEnableSensor);
    // JBI->Add("RfMask", mySwitch->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mFona_Cellular::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    // JBI->Object_Start(D_JSON_RFRECEIVED);
  
    //   JBI->Add("Pin1", pCONT_pins->GetPin(GPIO_LED1_ID));
    //   // JBI->Add(D_JSON_RF_BITS, rx_pkt.bit_length);
    //   // JBI->Add(D_JSON_RF_PROTOCOL, rx_pkt.protocol);
    //   // JBI->Add(D_JSON_RF_PULSE, rx_pkt.delay);   
    //   // JBI->Add(D_JSON_MILLIS, rx_pkt.received_time_millis);   
    //   // JBI->Add(D_JSON_TIME, mTime::ConvertU32TimetoCtr(&rx_pkt.received_utc_time, buffer, sizeof(buffer)));
      
    
    // JBI->Object_End();

    JBI->Add("rssi", connection.rssi_dBm);


    JBI->Object_Start("GPIO_State");
      JBI->Add("Key", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_KEY__ID)));
      JBI->Add("PS", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_STATUS__ID)));
      JBI->Add("NS", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_NETWORK_STATUS__ID)));
      JBI->Add("RST", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_RESET__ID)));
      // JBI->Add("RI", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_UART_TX__ID)));
      // JBI->Add("RI", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_UART_RX__ID)));
      JBI->Add("RI", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_RING_INDICATOR__ID)));
    JBI->Object_End();
  
  

  return JBI->End();

}


  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

  #ifdef USE_MODULE_NETWORK_MQTT
void mFona_Cellular::MQTTHandler_Init()
{

  struct handler<mFona_Cellular>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mFona_Cellular::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mFona_Cellular::ConstructJSON_State;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mFona_Cellular::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mFona_Cellular::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mFona_Cellular::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT
/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/


#endif
