#include "mRCSwitch_Extended.h"

#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED


uint8_t mRCSwitch::ConstructJSON_Settings(uint8_t json_method){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.fEnableSensor);
    JBI->Add("RfMask", mySwitch->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mRCSwitch::ConstructJSON_State(uint8_t json_method){

  char buffer[40];

  JBI->Start();

    JBI->Level_Start(D_JSON_RFRECEIVED);
  
      JBI->Add(D_JSON_DATA, rx_pkt.data);
      JBI->Add(D_JSON_RF_BITS, rx_pkt.bit_length);
      JBI->Add(D_JSON_RF_PROTOCOL, rx_pkt.protocol);
      JBI->Add(D_JSON_RF_PULSE, rx_pkt.delay);   
      JBI->Add(D_JSON_MILLIS, rx_pkt.received_time_millis);   
      JBI->Add(D_JSON_TIME, mTime::ConvertU32TimetoCtr(&rx_pkt.received_utc_time, buffer, sizeof(buffer)));
      
    
    JBI->Level_End();
  
  

  return JBI->End();

}



#endif // USE_MODULE_ENERGY_PZEM004T_V3
