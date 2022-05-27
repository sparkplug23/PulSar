
#include "mButtons.h"

#ifdef USE_MODULE_SENSORS_BUTTONS


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mButtons::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.buttons_found);
  return JsonBuilderI->End();

}

uint8_t mButtons::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();
    // JsonBuilderI->Array_AddArray("lastbutton", lastbutton, sizeof(lastbutton));

    JBI->Level_Start("ButtonPressed");
      JBI->Add("IsButtonActiveCtr", buttons[0].isactive);
    JBI->Level_End();


    /**
     * @brief New method to show type of press (short/long/multi)
     **/
    JBI->Level_Start("Event"); // asumes only one button at a time, will need nicer formatting later (arrays?)
      JBI->Add("ID", pCONT_rules->event_triggered.device_id);
      // JBI->Add("func", pCONT_rules->event_triggered.function_id);
      // JBI->Array_AddArray("data1", pCONT_rules->event_triggered.value.data, pCONT_rules->event_triggered.value.length);

      // [state][type][opt. count]
      if(pCONT_rules->event_triggered.value.data[1] == INPUT_TYPE_SINGLE_PRESS_ID)
      {
        JBI->Add("type", "Single");
        JBI->Add("count", 1);
      }
      if(pCONT_rules->event_triggered.value.data[1] == INPUT_TYPE_SINGLE_HOLD_ID)
      {
        JBI->Add("type", "Hold");
        JBI->Add("count", pCONT_rules->event_triggered.value.data[2]);
      }
      if(pCONT_rules->event_triggered.value.data[1] == INPUT_TYPE_MULTIPLE_PRESS_ID)
      {
        JBI->Add("type", "Multiple");
        JBI->Add("count", pCONT_rules->event_triggered.value.data[2]);
      }

    JBI->End();



    JBI->Add("pin",    buttons[0].pin);
    JBI->Add("dpin",   digitalRead(buttons[0].pin));
    JBI->Array_Start("bit_set_invert");
      for(int i=0;i<MAX_KEYS;i++){ JBI->Add(bitRead(key_inverted, i)); }
    JBI->Array_End();
    JBI->Array_Start("state");
      for(int i=0;i<MAX_KEYS;i++){ JBI->Add(buttons[i].state); }
    JBI->Array_End();

  return JsonBuilderI->End();

}

#endif