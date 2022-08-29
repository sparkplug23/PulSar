#include "mPWM.h"

#ifdef USE_MODULE_DRIVERS_PWM

uint8_t mPWM::ConstructJSON_Settings(uint8_t json_method){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.fEnableSensor);
    // JBI->Add("RfMask", mySwitch->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mPWM::ConstructJSON_State(uint8_t json_method){

  char buffer[40];

  JBI->Start();

    JBI->Array_Start("pins");
    for(uint8_t i=0;i<5;i++){ 
      JBI->Add(pwm[i].pin);
    }
    JBI->Array_End();

  
  

  return JBI->End();

}



#endif // USE_MODULE_ENERGY_PZEM004T_V3
