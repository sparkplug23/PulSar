#include "mOLED_SH1106.h"

#ifdef USE_MODULE_DISPLAYS_OLED_SH1106


uint8_t mOLED_SH1106::ConstructJSON_Settings(uint8_t json_method){

  JBI->Start();
    JBI->Add("Enabled", settings.fEnableSensor);
  return JBI->End();

}

#endif // USE_MODULE_DISPLAYS_OLED_SH1106
