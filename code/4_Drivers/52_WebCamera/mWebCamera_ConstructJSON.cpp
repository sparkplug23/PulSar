#include "mWebCamera.h"

#ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM_V4


uint8_t mWebCamera::ConstructJSON_Settings(uint8_t json_method){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.fEnableSensor);
  return JBI->End();

}

uint8_t mWebCamera::ConstructJSON_State(uint8_t json_method){

  char buffer[40];

  JBI->Start();

    JBI->Level_Start(D_JSON_RFRECEIVED);
  
    
    JBI->Level_End();
  
  

  return JBI->End();

}



#endif // USE_MODULE_ENERGY_PZEM004T_V3
