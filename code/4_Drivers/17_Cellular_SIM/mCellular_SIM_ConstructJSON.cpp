#include "mCellular_SIM.h"

#ifdef USE_MODULE_DRIVERS__CELLULAR_SIM7000


uint8_t mCellular_SIM7000::ConstructJSON_Settings(uint8_t json_method){

  JBI->Start();
    // JBI->Add(D_JSON_COUNT, settings.fEnableSensor);
    // JBI->Add("RfMask", mySwitch->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mCellular_SIM7000::ConstructJSON_State(uint8_t json_method){

  char buffer[40];

  JBI->Start();

    // JBI->Level_Start(D_JSON_RFRECEIVED);
  
    //   JBI->Add("Pin1", pCONT_pins->GetPin(GPIO_LED1_ID));
    //   // JBI->Add(D_JSON_RF_BITS, rx_pkt.bit_length);
    //   // JBI->Add(D_JSON_RF_PROTOCOL, rx_pkt.protocol);
    //   // JBI->Add(D_JSON_RF_PULSE, rx_pkt.delay);   
    //   // JBI->Add(D_JSON_MILLIS, rx_pkt.received_time_millis);   
    //   // JBI->Add(D_JSON_TIME, mTime::ConvertU32TimetoCtr(&rx_pkt.received_utc_time, buffer, sizeof(buffer)));
      
    
    // JBI->Level_End();

    JBI->Add("rssi", connection.rssi_dBm);


    JBI->Level_Start("GPIO_State");
      JBI->Add("Key", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_KEY__ID)));
      JBI->Add("PS", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_STATUS__ID)));
      JBI->Add("NS", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_NETWORK_STATUS__ID)));
      JBI->Add("RST", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_RESET__ID)));
      // JBI->Add("RI", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_UART_TX__ID)));
      // JBI->Add("RI", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_UART_RX__ID)));
      JBI->Add("RI", digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_RING_INDICATOR__ID)));
    JBI->Level_End();


    JBI->Level_Start("Debug");

      uint16_t adc = 0;
      fona->getADCVoltage(&adc);
      JBI->Add("getADCVoltage", adc);








    JBI->Level_End();




  
  

  return JBI->End();

}



#ifdef ENABLE_DEBUG_FEATURE_MQTT__CELLULAR_SIM__DEBUG_POLL_LATEST
uint8_t mCellular_SIM7000::ConstructJSON_Debug_RequestLatest(uint8_t json_method){

  char buffer[100];

  JBI->Start();

    JBI->Level_Start("Debug");

      uint16_t adc = 0;
      fona->getADCVoltage(&adc);
      JBI->Add("getADCVoltage", adc);

      JBI->Add("RSSI_dBm", CommandGet_RSSI_dBm());

      fona->getSIMCCID(buffer);
      JBI->Add("getSIMCCID", buffer);
      
      uint8_t n = fona->getNetworkStatus();
      JBI->Add("getNetworkStatus_Num", n);
      JBI->Add("getNetworkStatus_Name", GetNetworkStatusNameByID(n, buffer, sizeof(buffer)));

      int8_t smsnum = fona->getNumSMS();
      uint16_t smslen;
      int8_t smsn;
      if(smsnum)
      {
        JBI->Array_Start("SMS_Array");
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
        JBI->Add(replybuffer);
        Serial.println(F("*****"));
      }
      if(smsnum)
      {
        JBI->Array_End();
      }
      

    JBI->Level_End();




  
  

  return JBI->End();

}
#endif // ENABLE_DEBUG_FEATURE_MQTT__CELLULAR_SIM__DEBUG_POLL_LATEST


#endif // USE_MODULE_ENERGY_PZEM004T_V3
