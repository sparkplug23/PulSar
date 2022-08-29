
#include "mSR04.h" 

#ifdef USE_MODULE_SENSORS_SR04 

void mSR04::parse_JSONCommand(JsonParserObject obj)
{


  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_LIGHT D_TOPIC "mSR04::parse_JSONCommand %d"),obj.isNull());
  #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS

  char buffer[50];
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  /***
   * As order of importance, others that rely on previous commands must come after
   * */
  int val = 0;

  float a1 = 0;
  float a2 = 0;

  if(jtok = obj["SetAlpha1"])
  {
    a1= jtok.getFloat();
  }

  if(jtok = obj["SetAlpha2"])
  {
    a2= jtok.getFloat();
    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    readings.average_EMA.filter->SetAlpha(a1);
    #endif
    #ifdef ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    readings.average_DEMA.filter->SetAlpha(a1,a2);
    #endif
    mqtthandler_settings_teleperiod.flags.SendNow = true;
  }

  if(jtok = obj["SR04"].getObject()["ConversionMethod"])
  {
    readings.conversion_settings.flag_distance_conversion_method = jtok.getInt();
    AddLog(LOG_LEVEL_TEST, PSTR("ConversionMethod %d"),readings.conversion_settings.flag_distance_conversion_method);
    
    mqtthandler_settings_teleperiod.flags.SendNow = true;
  }




}

#endif
