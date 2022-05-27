#include "mEnergyInterface.h"

#ifdef USE_MODULE_ENERGY_INTERFACE


// void mEnergyInterface::parse_JSONCommand(){

//   return;// THIS IS WRONG!!

//   // Check if instruction is for me
//   if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/energy")>=0){
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
//     pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//     // fOpenHABDataStreamActive_last_secs = 1; // set to be positive to start
//     // fOpenHABDataStreamActive = true;
//   }else{
//     return; // not meant for here
//   }

//   // u
//   // int8_t tmp_id = 0;

//   // #ifdef JSONDOCUMENT_STATIC
//   //   StaticJsonDocument<800> doc;
//   // #else
//   //   DynamicJsonDocument doc(600);
//   // #endif
//   // DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
  
//   // if(error){
//   //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_DESERIALIZATION_ERROR));
//   //   Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_JSON_DESERIALIZATION_ERROR);
//   //   return 0;
//   // }
//   // JsonObject obj = doc.as<JsonObject>();
  
//   // if(!obj["command"].isNull()){ 
//   //   const char* command = obj["command"];
//   //   if(strstr(command,"system_send_all")){ 
//   //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"command\"=\"system_send_all\""));
//   //     MQTTHandler_Set_RefreshAll();
//   //     isserviced++;
//   //   }
//   //   else{
//   //     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
//   //   }
//   // }

//   // 

// } // END FUNCTION

uint8_t mEnergyInterface::ConstructJSON_Sensor(uint8_t json_method){

  // Serial.println("mEnergyInterface::ConstructJSON_Sensor"); Serial.flush();

  JsonBuilderI->Start();

  JsonBuilderI->Add(D_JSON_CHANNELCOUNT,         Energy.phase_count);
  JsonBuilderI->Array_AddArray_F(D_JSON_VOLTAGE,              Energy.voltage,       Energy.phase_count);
  JsonBuilderI->Array_AddArray_F(D_JSON_CURRENT,              Energy.current,       Energy.phase_count);
  JsonBuilderI->Array_AddArray_F(D_JSON_ACTIVE_POWERUSAGE,    Energy.active_power,  Energy.phase_count);

  JBI->Add("tRateSecs", mqtthandler_sensor_ifchanged.tRateSecs);


  // JsonBuilderI->Array_AddArray_F(D_JSON_APPARENT_POWERUSAGE,  Energy.apparent_power,Energy.phase_count);
  // JsonBuilderI->Array_AddArray_F(D_JSON_REACTIVE_POWERUSAGE,  Energy.reactive_power,Energy.phase_count);
  // JsonBuilderI->Array_AddArray_F(D_JSON_POWERFACTOR,          Energy.power_factor,  Energy.phase_count);
  // JsonBuilderI->Array_AddArray_F(D_JSON_FREQUENCY,            Energy.frequency,     Energy.phase_count);
  // JsonBuilderI->Add(D_JSON_ENERGY,               Energy.energy);
  // JsonBuilderI->Add(D_JSON_ENERGYLAST,           Energy.last_energy);
  
  // if(json_method >= JSON_LEVEL_DETAILED){
  //   JsonBuilderI->Level_Start(D_JSON_KWH_STATS);
  //     JsonBuilderI->Add(D_JSON_INDEX, Energy.stats.kwh_per_minute_index);
  //     JsonBuilderI->Add(D_JSON_ENERGY "LastMinute", Energy.stats.last_minutes_energy);
  //     JsonBuilderI->Add(D_JSON_ENERGY "Currently",  Energy.stats.current_energy);
  //     JsonBuilderI->Add(D_JSON_ENERGY "ThisMinute", Energy.stats.this_minutes_energy);
  //     JsonBuilderI->Array_AddArray_F("kwh_per_minute",           Energy.stats.kwh_per_minute,     sizeof(Energy.stats.kwh_per_minute)/4);
  //     JsonBuilderI->Array_AddArray_F("kwh_each_minute",          Energy.stats.kwh_each_minute,    sizeof(Energy.stats.kwh_each_minute)/4);
  //   JsonBuilderI->Level_End();
  // }

  // Serial.println(JBI->GetBufferPtr()); Serial.flush();
  // delay(2000);

  // AddLog(LOG_LEVEL_TEST, PSTR("mEnergyInterface::ConstructJSON_Sensor"));

  return JsonBuilderI->End();

}


uint8_t mEnergyInterface::ConstructJSON_ThresholdLimits(uint8_t json_method){

  JsonBuilderI->Start();

  // Create a new "Array" method that just lets me pass infinite amount of parameters using vsprintf

    JsonBuilderI->Level_Start("Current");
    
      JsonBuilderI->Array_Start("IsExceeded"); //show value right now
      for(int ii=0;ii<Energy.phase_count;ii++){
        JsonBuilderI->Add(parameter_thresholds[ii].current.over_limit_seconds_trigger_value_exceeded);
      }
      JsonBuilderI->Array_End();

      // JsonBuilderI->Array_Start("OverLimitCounter");
      // for(int ii=0;ii<Energy.phase_count;ii++){
      //   JsonBuilderI->Add_FV(PSTR("%d"),parameter_thresholds[ii].current.over_limit_seconds_counter);
      // }
      // JsonBuilderI->Array_End();

      // JsonBuilderI->Array_Start("Value"); //show value right now
      // for(int ii=0;ii<Energy.phase_count;ii++){
      //   JsonBuilderI->Add(Energy.current[ii]);
      // }
      // JsonBuilderI->Array_End();

      // JsonBuilderI->Array_Start("UpperLimit");
      // for(int ii=0;ii<Energy.phase_count;ii++){
      //   JsonBuilderI->Add(parameter_thresholds[ii].current.upper_limit);
      // }
      // JsonBuilderI->Array_End();

      // JsonBuilderI->Array_Start("LowerLimit");
      // for(int ii=0;ii<Energy.phase_count;ii++){
      //   JsonBuilderI->Add(parameter_thresholds[ii].current.lower_limit);
      // }
      // JsonBuilderI->Array_End();

      // JsonBuilderI->Array_Start("OverLimitTrigger");
      // for(int ii=0;ii<Energy.phase_count;ii++){
      //   JsonBuilderI->Add_FV(PSTR("%d"),parameter_thresholds[ii].current.over_limit_seconds_trigger_value);
      // }
      // JsonBuilderI->Array_End();

    JsonBuilderI->Level_End();


    // Voltage
    // parameter_thresholds[ii].voltage.upper_limit = 260;
    // parameter_thresholds[ii].voltage.lower_limit = 210;
    // parameter_thresholds[ii].voltage.over_limit_seconds_counter = 0;
    // parameter_thresholds[ii].voltage.over_limit_seconds_trigger_value = 120;
    // Current
    // parameter_thresholds[ii].current.upper_limit = 260;
    // parameter_thresholds[ii].current.lower_limit = 210;
    // parameter_thresholds[ii].current.over_limit_seconds_counter = 0;
    // parameter_thresholds[ii].current.over_limit_seconds_trigger_value = 120;
    // // active_power
    // parameter_thresholds[ii].active_power.upper_limit = 260;
    // parameter_thresholds[ii].active_power.lower_limit = 210;
    // parameter_thresholds[ii].active_power.over_limit_seconds_counter = 0;
    // parameter_thresholds[ii].active_power.over_limit_seconds_trigger_value = 120;
    // // Current
    // parameter_thresholds[ii].current.upper_limit = 260;
    // parameter_thresholds[ii].current.lower_limit = 210;
    // parameter_thresholds[ii].current.over_limit_seconds_counter = 0;
    // parameter_thresholds[ii].current.over_limit_seconds_trigger_value = 120;
    // // frequency
    // parameter_thresholds[ii].frequency.upper_limit = 260;
    // parameter_thresholds[ii].frequency.lower_limit = 210;
    // parameter_thresholds[ii].frequency.over_limit_seconds_counter = 0;
    // parameter_thresholds[ii].frequency.over_limit_seconds_trigger_value = 120;
    // // power_factor
    // parameter_thresholds[ii].power_factor.upper_limit = 260;
    // parameter_thresholds[ii].power_factor.lower_limit = 210;
    // parameter_thresholds[ii].power_factor.over_limit_seconds_counter = 0;
    // parameter_thresholds[ii].power_factor.over_limit_seconds_trigger_value = 120;
    // // energy
    // parameter_thresholds[ii].energy.upper_limit = 260;
    // parameter_thresholds[ii].energy.lower_limit = 210;
    // parameter_thresholds[ii].energy.over_limit_seconds_counter = 0;
    // parameter_thresholds[ii].energy.over_limit_seconds_trigger_value = 120;
  // if(json_method >= JSON_LEVEL_DETAILED){
  //   JsonBuilderI->Level_Start(D_JSON_KWH_STATS);
  //     JsonBuilderI->Add(D_JSON_INDEX, Energy.stats.kwh_per_minute_index);
  //     JsonBuilderI->Add(D_JSON_ENERGY "LastMinute", Energy.stats.last_minutes_energy);
  //     JsonBuilderI->Add(D_JSON_ENERGY "Currently",  Energy.stats.current_energy);
  //     JsonBuilderI->Add(D_JSON_ENERGY "ThisMinute", Energy.stats.this_minutes_energy);
  //     JsonBuilderI->Array_AddArray_F("kwh_per_minute",           Energy.stats.kwh_per_minute,     sizeof(Energy.stats.kwh_per_minute)/4);
  //     JsonBuilderI->Array_AddArray_F("kwh_each_minute",          Energy.stats.kwh_each_minute,    sizeof(Energy.stats.kwh_each_minute)/4);
  //   JsonBuilderI->Level_End();
  // }

  return JsonBuilderI->End();

}
/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mEnergyInterface::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();

    // JsonBuilderI->Add(D_JSON_CHANNELCOUNT"232",         0);
    
    JBI->Level_Start("Address");
    // for(int ii=0;ii<pCONT_iEnergy->Energy.phase_count;ii++)
    for(int ii=0;ii<address.size();ii++)
    {
      JBI->Array_Start_P(PSTR("device%d"), ii);
        // for(int i=0;i<4;i++)
        // {
          JBI->Add(address[ii]);
        // }
      JBI->Array_End();
    }
    JBI->Level_End();
  
  return JsonBuilderI->End();

}


uint8_t mEnergyInterface::ConstructJSON_EnergyStats(uint8_t json_method){

  JsonBuilderI->Start();

    // JsonBuilderI->Add(D_JSON_CHANNELCOUNT"232",         0);
  
  return JsonBuilderI->End();
  // D_DATA_BUFFER_CLEAR();

  // // StaticJsonDocument<1000> doc;
  // // JsonObject root = doc.to<JsonObject>();

  // // uint8_t ischanged=false;

  // // char channel_ctr[3];
  // // memset(channel_ctr,0,sizeof(channel_ctr));
  // // // sprintf(channel_ctr,"%02d",channel);
  // // //JsonObject energy_total_obj = root.createNestedObject("Energy_Total");

  // //   JsonObject kwh_per_minute_obj = root.createNestedObject("kwh_stats"); 
    
    
  // //   kwh_per_minute_obj["index"] = Energy.stats.kwh_per_minute_index;
  // //   kwh_per_minute_obj["last_minutes_energy"] = Energy.stats.last_minutes_energy;
  // //   kwh_per_minute_obj["current_energy"] = Energy.stats.current_energy;
  // //   kwh_per_minute_obj["this_minutes_energy"] = Energy.stats.this_minutes_energy;


  // // // if(json_method >= JSON_LEVEL_DETAILED){
  // // //   JsonArray kwh_per_minute_arr = kwh_per_minute_obj.createNestedArray("kwh_per_minute"); 
  // // //   JsonArray kwh_each_minute_arr = kwh_per_minute_obj.createNestedArray("kwh_each_minute");  
  // // //   for (uint8_t min = 0; min < 60; min++) {
  // // //     kwh_per_minute_arr.add(Energy.stats.kwh_per_minute[min]);
  // // //     kwh_each_minute_arr.add(Energy.stats.kwh_each_minute[min]);
  // // //   }
  // // // }

  // // data_buffer.payload.len = measureJson(root)+1;
  // // serializeJson(doc,data_buffer.payload.ctr);
  // // return data_buffer.payload.len>3?true:false;

  // return 0;
    
}

/**
 * @brief From EnergyShow, its basically my construstjson
 * 
 */
  // if (json) {
    // bool show_energy_period = (0 == pCONT_set->tele_period);

    // pCONT_sup->ResponseAppend_P(PSTR(",\"" D_RSLT_ENERGY "\":{\"" D_JSON_TOTAL_START_TIME "\":\"%s\",\"" D_JSON_TOTAL "\":%s,\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s"),
    //   pCONT_time->GetDateAndTimeCtr(DT_ENERGY),
    //   EnergyFormatIndex(value_chr, energy_total_chr[0], json, energy_total_fields),
    //   energy_yesterday_chr,
    //   energy_daily_chr);

    // if (!isnan(Energy.export_active)) {
    //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_EXPORT_ACTIVE "\":%s"),
    //     EnergyFormatIndex(value_chr, export_active_chr[0], json, energy_total_fields));
    // }

    // if (show_energy_period) {
    //   float energy = 0;
    //   if (Energy.period) {
    //     energy = (float)(pCONT_set->RtcSettings.energy_kWhtoday - Energy.period) / 100;
    //   }
    //   Energy.period = pCONT_set->RtcSettings.energy_kWhtoday;
    //   char energy_period_chr[FLOATSZ];
    //   pCONT_sup->dtostrfd(energy, pCONT_set->Settings.flag_power.wattage_resolution, energy_period_chr);
    //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_PERIOD "\":%s"), energy_period_chr);
    // }
    // pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_POWERUSAGE "\":%s"),
    //   EnergyFormat(value_chr, active_power_chr[0], json));
    // if (!Energy.type_dc) {
    //   if (Energy.current_available && Energy.voltage_available) {
    //     pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_APPARENT_POWERUSAGE "\":%s,\"" D_JSON_REACTIVE_POWERUSAGE "\":%s,\"" D_JSON_POWERFACTOR "\":%s"),
    //       EnergyFormat(value_chr, apparent_power_chr[0], json),
    //       EnergyFormat(value2_chr, reactive_power_chr[0], json),
    //       EnergyFormat(value3_chr, power_factor_chr[0], json));
    //   }
    //   if (!isnan(Energy.frequency[0])) {
    //     pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_FREQUENCY "\":%s"),
    //       EnergyFormat(value_chr, frequency_chr[0], json, Energy.voltage_common));
    //   }
    // }
    // if (Energy.voltage_available) {
    //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_VOLTAGE "\":%s"),
    //     EnergyFormat(value_chr, voltage_chr[0], json, Energy.voltage_common));
    // }
    // if (Energy.current_available) {
    //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_CURRENT "\":%s"),
    //     EnergyFormat(value_chr, current_chr[0], json));
    // }

    // AddLog(LOG_LEVEL_TEST,PSTR(" mEnergyInterface::FUNC_JSON_APPEND "));

    // Tasker(FUNC_JSON_APPEND);
    // pCONT_sup->ResponseJsonEnd();

// #ifdef USE_DOMOTICZ
//     if (show_energy_period) {  // Only send if telemetry
//       pCONT_sup->dtostrfd(Energy.total * 1000, 1, energy_total_chr[0]);
//       DomoticzSensorPowerEnergy((int)Energy.active_power[0], energy_total_chr[0]);  // PowerUsage, EnergyToday

//       pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.usage1_kWhtotal / 100, 1, energy_total_chr[1]);  // Tariff1
//       pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.usage2_kWhtotal / 100, 1, energy_total_chr[2]);  // Tariff2
//       pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.return1_kWhtotal / 100, 1, export_active_chr[1]);
//       pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.return2_kWhtotal / 100, 1, export_active_chr[2]);
//       DomoticzSensorP1SmartMeter(energy_total_chr[1], energy_total_chr[2], export_active_chr[1], export_active_chr[2], (int)Energy.active_power[0]);

//       if (Energy.voltage_available) {
//         DomoticzSensor(DZ_VOLTAGE, voltage_chr[0]);  // Voltage
//       }
//       if (Energy.current_available) {
//         DomoticzSensor(DZ_CURRENT, current_chr[0]);  // Current
//       }
//     }
// #endif  // USE_DOMOTICZ
// #ifdef USE_KNX
//     if (show_energy_period) {
//       if (Energy.voltage_available) {
//         KnxSensor(KNX_ENERGY_VOLTAGE, Energy.voltage[0]);
//       }
//       if (Energy.current_available) {
//         KnxSensor(KNX_ENERGY_CURRENT, Energy.current[0]);
//       }
//       KnxSensor(KNX_ENERGY_POWER, Energy.active_power[0]);
//       if (!Energy.type_dc) {
//         KnxSensor(KNX_ENERGY_POWERFACTOR, power_factor_knx);
//       }
//       KnxSensor(KNX_ENERGY_DAILY, Energy.daily);
//       KnxSensor(KNX_ENERGY_TOTAL, Energy.total);
//       KnxSensor(KNX_ENERGY_START, Energy.start_energy);
//     }
// #endif  // USE_KNX
// #ifdef USE_MODULE_NETWORK_WEBSERVER
//   // } else {

// /**
//  * @brief 
//  * Part of tas that generate json when needed, I will make this its own section
//  * 
//  */

//       if (Energy.voltage_available) {
//         pCONT_web->WSContentSend_PD(HTTP_SNS_VOLTAGE, 
//           EnergyFormat(value_chr, voltage_chr[0], json, Energy.voltage_common)
//         );
//       }
//       if (Energy.current_available) {
//         pCONT_web->WSContentSend_PD(HTTP_SNS_CURRENT, 
//           EnergyFormat(value_chr, current_chr[0], json)
//         );
//       }
//       pCONT_web->WSContentSend_PD(HTTP_SNS_POWER, EnergyFormat(value_chr, active_power_chr[0], json));
//       if (!Energy.type_dc) {
//         if (Energy.current_available && Energy.voltage_available) {
//           pCONT_web->WSContentSend_PD(HTTP_ENERGY_SNS1, 
//             EnergyFormat(value_chr, apparent_power_chr[0], json),
//             EnergyFormat(value2_chr, reactive_power_chr[0], json),
//             EnergyFormat(value3_chr, power_factor_chr[0], json)
//           );
//         }
//         if (!isnan(Energy.frequency[0])) { 
//           pCONT_web->WSContentSend_PD(PSTR("{s}" D_FREQUENCY "{m}%s " D_UNIT_HERTZ "{e}"),
//             EnergyFormat(value_chr, frequency_chr[0], json, Energy.voltage_common)
//           );
//         }
//       }
//       pCONT_web->WSContentSend_PD(HTTP_ENERGY_SNS2, energy_daily_chr, energy_yesterday_chr, energy_total_chr[0]);
//       if (!isnan(Energy.export_active)) {
//         pCONT_web->WSContentSend_PD(HTTP_ENERGY_SNS3, export_active_chr[0]);
//       }
// #endif  // USE_MODULE_NETWORK_WEBSERVER
  //} //if json


#endif // USE_MODULE_SENSORS_INTERFACE
