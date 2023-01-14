#include "mDisplaysInterface.h"

#ifdef USE_MODULE_DISPLAYS_INTERFACE

uint8_t mDisplaysInterface::ConstructJSON_Settings(uint8_t json_level, bool json_object_start_end_required){

  JsonBuilderI->Start();

    JsonBuilderI->Add("model", pCONT_set->Settings.display.model);
    JsonBuilderI->Add("mode", pCONT_set->Settings.display.mode);
    JsonBuilderI->Add("refresh", pCONT_set->Settings.display.refresh);
    JsonBuilderI->Add("rows", pCONT_set->Settings.display.rows);
    JsonBuilderI->Add("cols0", pCONT_set->Settings.display.cols[0]);
    JsonBuilderI->Add("cols1", pCONT_set->Settings.display.cols[1]);
    JsonBuilderI->Add("address0", pCONT_set->Settings.display.address[0]);
    JsonBuilderI->Add("dimmer", pCONT_set->Settings.display.dimmer);
    JsonBuilderI->Add("size", pCONT_set->Settings.display.size);
    JsonBuilderI->Add("font", pCONT_set->Settings.display.font);
    JsonBuilderI->Add("rotate", pCONT_set->Settings.display.rotate);

    JsonBuilderI->Add("width", pCONT_set->Settings.display.width);
    JsonBuilderI->Add("height", pCONT_set->Settings.display.height);

  return JsonBuilderI->End();

}


uint8_t mDisplaysInterface::ConstructJSON_Sensor(uint8_t json_level, bool json_object_start_end_required){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_VOLTAGE, 0);
  return JsonBuilderI->End();
    
}

#endif // USE_MODULE_DISPLAYS_INTERFACE
