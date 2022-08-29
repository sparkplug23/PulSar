#include "mSideDoorLight.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS


uint8_t mSideDoorLight::ConstructJSON_Settings(uint8_t json_method){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.fEnableSensor);
    
  return JBI->End();

}

uint8_t mSideDoorLight::ConstructJSON_State(uint8_t json_method){

  char buffer[40];

  JBI->Start();  
  
  return JBI->End();

}



#endif // USE_MODULE_ENERGY_PZEM004T_V3
