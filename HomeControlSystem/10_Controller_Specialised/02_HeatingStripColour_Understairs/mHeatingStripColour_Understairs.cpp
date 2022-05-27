#include "mHeatingStripColour_Understairs.h"

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
    colour_array[1] = 9;
    colour_array[2] = 0;
    colour_array[3] = 0;
    colour_array[4] = 0;
    // Colour0
    colour_array[5] = 255;
    colour_array[6] = 0;
    colour_array[7] = 0;
    colour_array[8] = 0;
    colour_array[9] = 0;
    // Colour0
    colour_array[10] = 0;
    colour_array[11] = 255;
    colour_array[12] = 0;
    colour_array[13] = 0;
    colour_array[14] = 0;
    // Colour0
    colour_array[15] = 0;
    colour_array[16] = 0;
    colour_array[17] = 255;
    colour_array[18] = 0;
    colour_array[19] = 0;
    // Colour0
    colour_array[20] = 0;
    colour_array[21] = 0;
    colour_array[22] = 0;
    colour_array[23] = 255;
    colour_array[24] = 255;


   pCONT_iLight->CommandSet_PaletteColour_RGBCCT_Raw_By_ID(15, colour_array, sizeof(colour_array));

}


uint8_t mHeatingStripColour_Understairs::ConstructJSON_Settings(uint8_t json_method)
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


#endif
