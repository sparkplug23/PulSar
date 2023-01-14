#include "mStripColour.h"

#ifdef USE_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS

const char* mHeatingStripColour_Understairs::PM_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_CTR = D_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_CTR;
const char* mHeatingStripColour_Understairs::PM_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_FRIENDLY_CTR = D_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_FRIENDLY_CTR;

int8_t mHeatingStripColour_Understairs::Tasker(uint8_t function, JsonParserObject obj)
{


  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;  
    case FUNC_EVERY_SECOND: 
      Every_Second();
    break;  
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED: 
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

}


void mHeatingStripColour_Understairs::Pre_Init(void)
{

  settings.fEnableSensor = true;

  // settings.fSensorCount = 0;

  // if(settings.fSensorCount){
  //   settings.fEnableSensor = true;
  //   AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "ADC_INTERNAL Sensor Enabled"));
  // }

}


void mHeatingStripColour_Understairs::Init(void)
{
  
  
}


void mHeatingStripColour_Understairs::EveryLoop()
{


}


void mHeatingStripColour_Understairs::Every_Second()
{

  uint8_t colour_array[50];
  memset(colour_array,0,sizeof(colour_array));

  // Format
  colour_array[0] = 4;
  colour_array[1] = MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX__ID;
  colour_array[2] = 0;
  colour_array[3] = 0;
  colour_array[4] = 0;
  // // Colour0
  // colour_array[5] = 255;
  // colour_array[6] = 0;
  // colour_array[7] = 0;
  // colour_array[8] = 0;
  // colour_array[9] = 0;
  // // Colour0
  // colour_array[10] = 0;
  // colour_array[11] = 255;
  // colour_array[12] = 0;
  // colour_array[13] = 0;
  // colour_array[14] = 0;
  // // Colour0
  // colour_array[15] = 0;
  // colour_array[16] = 0;
  // colour_array[17] = 255;
  // colour_array[18] = 0;
  // colour_array[19] = 0;
  // // Colour0
  // colour_array[20] = 255;
  // colour_array[21] = 255;
  // colour_array[22] = 255;
  // colour_array[23] = 255;
  // colour_array[24] = 255;


  /**
   * @brief Upstairs
   * Blink if time_on > 0, or else just show colour
   * Calling this function faster, blinking at 250ms
   */

  for(uint8_t i=0;i<4;i++)
  {

    if(pixel_info[i].flag_blink_state){
      pixel_info[i].colour = pCONT_iLight->GetColourValueUsingMaps(0);//pCONT_hvac->zone[i].sensor.temperature);
    }else{
      pixel_info[i].colour = RgbcctColor(0,0,0,50,50);
    }

    if(pCONT_hvac->zone[i].program_timer_method->GetTimer_Seconds())
    {
      pixel_info[i].flag_blink_state ^= 1;
    }else{
      pixel_info[i].flag_blink_state = true; // disable
    }
    
  }

  // Colour0
  colour_array[5] = pixel_info[0].colour.R;
  colour_array[6] = pixel_info[0].colour.G;
  colour_array[7] = pixel_info[0].colour.B;
  colour_array[8] = pixel_info[0].colour.W1;
  colour_array[9] = pixel_info[0].colour.W2;
  // Colour1
  colour_array[10] = pixel_info[1].colour.R;
  colour_array[11] = pixel_info[1].colour.G;
  colour_array[12] = pixel_info[1].colour.B;
  colour_array[13] = pixel_info[1].colour.W1;
  colour_array[14] = pixel_info[1].colour.W2;
  // Colour2
  colour_array[15] = pixel_info[1].colour.R;
  colour_array[16] = pixel_info[1].colour.G;
  colour_array[17] = pixel_info[1].colour.B;
  colour_array[18] = pixel_info[1].colour.W1;
  colour_array[19] = pixel_info[1].colour.W2;
  // Colour3
  colour_array[20] = pixel_info[1].colour.R;
  colour_array[21] = pixel_info[1].colour.G;
  colour_array[22] = pixel_info[1].colour.B;
  colour_array[23] = pixel_info[1].colour.W1;
  colour_array[24] = pixel_info[1].colour.W2;


  pCONT_iLight->CommandSet_PaletteColour_RGBCCT_Raw_By_ID(15, colour_array, sizeof(colour_array));
  pCONT_lAni->CommandSet_Global_BrtRGB_255(255);
  pCONT_lAni->CommandSet_Global_BrtCCT_255(255);
  pCONT_lAni->CommandSet_PaletteID(15);


}


uint8_t mHeatingStripColour_Understairs::ConstructJSON_Settings(uint8_t json_level, bool json_object_start_end_required)
{
  JBI->Start();
  

  return JBI->End();
}


uint8_t mHeatingStripColour_Understairs::ConstructJSON_State(uint8_t json_level)
{
  char buffer[100];

  JBI->Start();
    // if(adc_values.adc1 != 0)
    // {
    JBI->Add("adc_raw", 1);

    // JBI->Array_AddArray()

    // }
    // JBI->Add("litres", adc_values.litres);
    // JBI->Add("adc_upper_boundary_index", adc_upper_boundary_index);
    // if(adc_values.litres != 0)
    // {
    // JBI->Add("litres_non_zero", adc_values.litres);
    // }
  return JBI->End();
}



/******************************************************************************************************************
 * mStripColour_Commands.cpp
*******************************************************************************************************************/


void mHeatingStripColour_Understairs::parse_JSONCommand(JsonParserObject obj){


}


/******************************************************************************************************************
 * mStripColour_MQTT.cpp
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mHeatingStripColour_Understairs::MQTTHandler_Init()
{

  struct handler<mHeatingStripColour_Understairs>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mHeatingStripColour_Understairs::ConstructJSON_Settings;

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mHeatingStripColour_Understairs::ConstructJSON_State;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mHeatingStripColour_Understairs::ConstructJSON_State;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mHeatingStripColour_Understairs::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mHeatingStripColour_Understairs::MQTTHandler_Set_TelePeriod()
{
  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  // }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mHeatingStripColour_Understairs::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT




#endif
