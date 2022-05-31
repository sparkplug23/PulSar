#include "mRadiatorFan.h"

#ifdef USE_MODULE_CONTROLLER_RADIATORFAN

const char* mRadiatorFan::PM_MODULE_CONTROLLER_RADIATORFAN_CTR = D_MODULE_CONTROLLER_RADIATORFAN_CTR;
const char* mRadiatorFan::PM_MODULE_CONTROLLER_RADIATORFAN_FRIENDLY_CTR = D_MODULE_CONTROLLER_RADIATORFAN_FRIENDLY_CTR;

int8_t mRadiatorFan::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_INIT:
      Init();
      break;
  }

  if(!settings.enabled_module){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    case FUNC_EVERY_MINUTE: 
      Task_UseTemperatureToControlRelay();
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


void mRadiatorFan::Init(void)
{

}


void mRadiatorFan::Task_UseTemperatureToControlRelay()
{

  #ifdef USE_MODULE_SENSORS_DS18X

  /**
   * hard coding solution for now, search for sensor by its address
   * later, search for sensor by its name (using generic sensor_interface ie "name" return "temp")
   * */
  DeviceAddress known_address = {40,159,147,2,0,0,0,117};

  state.temperature_current = pCONT_msdb18->sensor_group[0].dallas->getTempC(known_address);

  AddLog(LOG_LEVEL_INFO, PSTR("temperature = %d"), state.temperature_current);

  state.iswithin_temperature_limit = IsWithinLimits(state.threshold_minimum_temperature, state.temperature_current, state.threshold_maximum_temperature);

  if(state.iswithin_temperature_limit)
  {
    /**
     * In case the sensor becomes unresponse, we will only command the relay on when the state&condition was correct, 
     * this will automatically turn off by the relay timer
     * Checks: 1 minute
     * Backoff: 2 minutes until it would turn off
     * */
    pCONT_mry->CommandSet_Timer_Decounter(60*5, 0 /*Relay0*/);   
  }

  #endif

}


uint8_t mRadiatorFan::ConstructJSON_Settings(uint8_t json_method)
{
  JBI->Start();
    JBI->Add("TemperatureControlEnabled", settings.enabled_temperature_control);
  return JBI->End();
}


uint8_t mRadiatorFan::ConstructJSON_State(uint8_t json_level)
{
  char buffer[100];

  JBI->Start();
    JBI->Level_Start("Temperature");
      JBI->Add("IsWithinLimit", state.iswithin_temperature_limit);
      JBI->Level_Start("Threshold");
        JBI->Add("Maximum", state.threshold_maximum_temperature);
        JBI->Add("Minimum", state.threshold_minimum_temperature);
      JBI->Level_End();
      JBI->Add("TriggerTemperature", state.temperature_current);
      JsonBuilderI->Level_Start("InternalSensors");
        for(int sensor_id=0;sensor_id<3;sensor_id++)
        { 
          // JsonBuilderI->Level_Start(DLI->GetDeviceNameWithEnumNumber(E M_MODULE_SENSORS_DB18S20_ID,pCONT_msdb18->sensor[sensor_id].address_id,buffer,sizeof(buffer)));    
          JsonBuilderI->Level_Start(DLI->GetDeviceName_WithModuleUniqueID( pCONT_msdb18->GetModuleUniqueID() ,pCONT_msdb18->sensor[sensor_id].address_id,buffer,sizeof(buffer)));         


          
            JsonBuilderI->Add(D_JSON_TEMPERATURE, pCONT_msdb18->sensor[sensor_id].reading.val);
          JsonBuilderI->Level_End();  
        }
      JsonBuilderI->Level_End();  
    JBI->Level_End();
    
    JBI->Level_Start("Fan"); // Based on relay controls
      JBI->Add("State",                  pCONT_mry->CommandGet_Relay_Power(0));
      JBI->Add("TimeOnSeconds",          pCONT_mry->CommandGet_SecondsRelayHasBeenOn(0));          // total time on
      JBI->Add("TimeOnDecounterSeconds", pCONT_mry->CommandGet_SecondsToRemainOn(0)); // time to stay on
    JBI->Level_End();
  return JBI->End();
}


#endif
