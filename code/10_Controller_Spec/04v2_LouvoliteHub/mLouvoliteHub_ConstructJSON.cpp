#include "mLouvoliteHub.h"

#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2


uint8_t mLouvoliteHub::ConstructJSON_Settings(uint8_t json_method){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.fEnableSensor);
    JBI->Add("RfMask", transceive->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mLouvoliteHub::ConstructJSON_State(uint8_t json_method){

  char buffer[40];

  JBI->Start();  
    JBI->Add("LastCommand", last_position_command);
    JBI->Add("LastCommandName", CommandMode_GetName_by_ID(last_position_command, buffer, sizeof(buffer)));
  return JBI->End();

}



#endif // USE_MODULE_ENERGY_PZEM004T_V3
