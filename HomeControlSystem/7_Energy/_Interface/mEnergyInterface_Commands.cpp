#include "mEnergyInterface.h"

#ifdef USE_MODULE_ENERGY_INTERFACE

// /*********************************************************************************************\
//  * Energy
// \*********************************************************************************************/

// // #define XDRV_03                3
// // #define XSNS_03                3

// //#define USE_ENERGY_MARGIN_DETECTION
//  #define USE_ENERGY_POWER_LIMIT


// enum EnergyCommands {
//   CMND_POWERCAL, CMND_VOLTAGECAL, CMND_CURRENTCAL,
//   CMND_POWERSET, CMND_VOLTAGESET, CMND_CURRENTSET, CMND_FREQUENCYSET, CMND_MODULEADDRESS };


// Integrate these with my code ie
// ./set/energy/<kEnergyCommands> list, then use ID to call function (eventually make it function handler.. maybe try this now?)

// const char kEnergyCommands[] PROGMEM = "|"  // No prefix
//   D_JSON_POWERCAL "|" D_JSON_VOLTAGECAL "|" D_JSON_CURRENTCAL "|"
//   D_JSON_POWERSET "|" D_JSON_VOLTAGESET "|" D_JSON_CURRENTSET "|" D_JSON_FREQUENCYSET "|" D_JSON_MODULEADDRESS "|"
// // #ifdef USE_ENERGY_MARGIN_DETECTION
//   D_JSON_POWERDELTA "|" D_JSON_POWERLOW "|" D_JSON_POWERHIGH "|" D_JSON_VOLTAGELOW "|" D_JSON_VOLTAGEHIGH "|" D_JSON_CURRENTLOW "|" D_JSON_CURRENTHIGH "|"
// // #ifdef USE_ENERGY_POWER_LIMIT
//   D_JSON_MAXENERGY "|" D_JSON_MAXENERGYSTART "|"
//   D_JSON_MAXPOWER "|" D_JSON_MAXPOWERHOLD "|" D_JSON_MAXPOWERWINDOW "|"
//   D_JSON_SAFEPOWER "|" D_JSON_SAFEPOWERHOLD "|"  D_JSON_SAFEPOWERWINDOW "|"
// // #endif  // USE_ENERGY_POWER_LIMIT
// // #endif  // USE_ENERGY_MARGIN_DETECTION
//   D_JSON_ENERGYRESET "|" D_JSON_TARIFF ;

// // void (* const EnergyCommand[])(void) PROGMEM = {
// //   &CmndPowerCal, &CmndVoltageCal, &CmndCurrentCal,
// //   &CmndPowerSet, &CmndVoltageSet, &CmndCurrentSet, &CmndFrequencySet, &CmndModuleAddress,
// // #ifdef USE_ENERGY_MARGIN_DETECTION
// //   &CmndPowerDelta, &CmndPowerLow, &CmndPowerHigh, &CmndVoltageLow, &CmndVoltageHigh, &CmndCurrentLow, &CmndCurrentHigh,
// // #ifdef USE_ENERGY_POWER_LIMIT
// //   &CmndMaxEnergy, &CmndMaxEnergyStart,
// //   &CmndMaxPower, &CmndMaxPowerHold, &CmndMaxPowerWindow,
// //   &CmndSafePower, &CmndSafePowerHold, &CmndSafePowerWindow,
// // #endif  // USE_ENERGY_POWER_LIMIT
// // #endif  // USE_ENERGY_MARGIN_DETECTION
// //   &CmndEnergyReset, &CmndTariff };


// void mEnergyInterface::parse_JSONCommand(JsonParserObject obj)
// {

//   JsonParserToken jtok = 0; 
//   int8_t tmp_id = 0;

// 	// if(jtok = obj["MQTT"].getObject()["ScanSensors"])
// 	// {
// 	// 	JBI->Start();

// 	// 	pCONT->Tasker_Interface(FUNC_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID);

// 	// 	bool ready_to_send = JBI->End();

// 	// 	if(!ready_to_send)
// 	// 	{
// 	// 		// Nothing was found, create new message
// 	// 		JBI->Start();
// 	// 			JBI->Add("SensorScan", "No Devices Found");
// 	// 		ready_to_send = JBI->End();
// 	// 	}


// 	// 	if(ready_to_send)
// 	// 	{			
//     // 	AddLog(LOG_LEVEL_TEST, PSTR("ScanSensors=\"%s\""), JBI->GetBufferPtr());
// 	// 		pCONT_mqtt->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
// 	// 	}

// 	// }



    
// }



/**
 * @brief "BootSafe" means these are commands that may be called before init
 * Phase out, this should instead invoke a delayed pre_init/init phase. I need to work out how to safely call init/pri-init during runtime (ie need checks for when things are not preconfigured)
 * */
void mEnergyInterface::parse_JSONCommand_BootSafe(JsonParserObject obj)
{

//   AddLog(LOG_LEVEL_TEST, PSTR( DEBUG_INSERT_PAGE_BREAK "mEnergyInterface::parse_JSONCommand_BootSafe"));
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  // Using a desired address, the sensor is searched for, then index (id) is updated
  if(jtok = obj["Energy"].getObject()["DeviceCount"])
  {
    SetEnergyDeviceCount(jtok.getInt());
  }

}


void mEnergyInterface::parse_JSONCommand(JsonParserObject obj){

    // AddLog(LOG_LEVEL_TEST, PSTR( DEBUG_INSERT_PAGE_BREAK "mEnergyInterface::parse_JSONCommand"));
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  // Using a desired address, the sensor is searched for, then index (id) is updated
  if(jtok = obj[PM_JSON_SENSORADDRESS].getObject()[D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR]){
     
    JsonParserArray array_group = obj[PM_JSON_SENSORADDRESS].getObject()[D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR];
      
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 D_PARSING_MATCHED "%s count %d"), F(D_JSON_SENSORADDRESS),array_group.size()); 
    #endif // LOG_LEVEL_COMMANDS
    
    uint8_t address_temp[8];
    uint8_t address_index = 0;
    uint8_t original_device_id = 0;

    uint8_t device_count = array_group.size();
      // SetEnergyDeviceCount(device_count);
    
    for(auto group_iter : array_group) {

      // JsonParserArray array_sensor_address_iter = group_iter;
      // memset(address_temp,0,sizeof(address_temp));
      // address_index = 0;            
      // for(auto address_id : array_sensor_address_iter) {
      //   int address = address_id.getInt();
      //   // #ifdef ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL
      //   //AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 "address = %d"),address); 
      //   // #endif
      //   address_temp[address_index++] = address;
      //   // if(address_index>7){ break; } //error!
      // }
      // AddLog_Array(LOG_LEVEL_COMMANDS, "address", address_temp, (uint8_t)4);
      // AddLog_Array(LOG_LEVEL_COMMANDS, "address", address_temp, (uint8_t)4);
      AddLog(LOG_LEVEL_TEST, PSTR(" group_iter.getInt()) = %d"),  group_iter.getInt());
      SetIDWithAddress(original_device_id++, group_iter.getInt());//, address_index);

    }
     
  }

  // else{
    
  //     AddLog(LOG_LEVEL_TEST, PSTR(" NOT FOUND group_iter.getInt()) = %d"));
  // }

}


// void mEnergyInterface::parse_JSONCommand(JsonParserObject obj){

//   JsonParserToken jtok = 0; 
//   int8_t tmp_id = 0;

//   // Using a desired address, the sensor is searched for, then index (id) is updated
//   // if(jtok = obj["Set"].getObject()[""]){
     
//   //   JsonParserArray array_group = obj[PM_JSON_SENSORADDRESS].getObject()[D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR];
      
//   //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 D_PARSING_MATCHED "%s count %d"), F(D_JSON_SENSORADDRESS),array_group.size()); 
//   //   #endif // LOG_LEVEL_COMMANDS
    
//   //   uint8_t address_temp[8];
//   //   uint8_t address_index = 0;
//   //   uint8_t original_device_id = 0;
    
//   //   for(auto group_iter : array_group) {

//   //     JsonParserArray array_sensor_address_iter = group_iter;
//   //     memset(address_temp,0,sizeof(address_temp));
//   //     address_index = 0;            
//   //     for(auto address_id : array_sensor_address_iter) {
//   //       int address = address_id.getInt();
//   //       // #ifdef ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL
//   //       //AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 "address = %d"),address); 
//   //       // #endif
//   //       address_temp[address_index++] = address;
//   //       // if(address_index>7){ break; } //error!
//   //     }
//   //     AddLog_Array(LOG_LEVEL_COMMANDS, "address", address_temp, (uint8_t)4);
//   //     SetIDWithAddress(original_device_id++, address_temp, address_index);

//   //   }
     
//   // }

// }

/*
#ifdef ENABLE_POWER_MONITORING

uint8_t mTelemetry::ConstructJSON_PowerMonitor(uint8_t json_level){ 

  // // clear entire mqtt packet
  // D_DATA_BUFFER_CLEAR();


  // root[F("tbd")] = 0;

  // D_JSON_STATUS D_STATUS9_MARGIN "\":{\"" D_JSON_POWERDELTA "\":%d,\"" D_JSON_POWERLOW "\":%d,\"" D_JSON_POWERHIGH "\":%d,\""
  //                           D_JSON_VOLTAGELOW "\":%d,\"" D_JSON_VOLTAGEHIGH "\":%d,\"" D_JSON_CURRENTLOW "\":%d,\"" D_JSON_CURRENTHIGH "\":%d}}"),
  //                           Settings.energy_power_delta, Settings.energy_min_power, Settings.energy_max_power,
  //                           Settings.energy_min_voltage, Settings.energy_max_voltage, Settings.energy_min_current, Settings.energy_max_current);
 
  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  return 0;
}
#endif

leave energy in its own module, not telemetry here

  #define D_STATUS8_POWER "PWR"
  
  #define D_STATUS9_MARGIN "PTH"
#if defined(USE_ENERGY_SENSOR) && defined(USE_ENERGY_MARGIN_DETECTION)
  if (energy_flg) {
    if ((0 == payload) || (9 == payload)) {
      Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS9_MARGIN "\":{\"" D_JSON_POWERDELTA "\":%d,\"" D_JSON_POWERLOW "\":%d,\"" D_JSON_POWERHIGH "\":%d,\""
                            D_JSON_VOLTAGELOW "\":%d,\"" D_JSON_VOLTAGEHIGH "\":%d,\"" D_JSON_CURRENTLOW "\":%d,\"" D_JSON_CURRENTHIGH "\":%d}}"),
                            Settings.energy_power_delta, Settings.energy_min_power, Settings.energy_max_power,
                            Settings.energy_min_voltage, Settings.energy_max_voltage, Settings.energy_min_current, Settings.energy_max_current);
      MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "9"));
    }
  }
#endif  // USE_ENERGY_MARGIN_DETECTION

*/




// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/

// void mEnergyInterface::EnergyCommandCalResponse(uint32_t nvalue)
// {
//   // snprintf_P(XdrvMailbox.command, CMDSZ, PSTR("%sCal"), XdrvMailbox.command);
//   // ResponseCmndNumber(nvalue);
// }

// void mEnergyInterface::CmndEnergyReset(void)
// {
// //   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 3)) {
// //     char *p;
// //     unsigned long lnum = strtoul(XdrvMailbox.data, &p, 10);
// //     if (p != XdrvMailbox.data) {
// //       switch (XdrvMailbox.index) {
// //       case 1:
// //         // Reset Energy Today
// //         Energy.kWhtoday_offset = lnum *100;
// //         Energy.kWhtoday = 0;
// //         Energy.kWhtoday_delta = 0;
// //         Energy.start_energy = 0;
// //         Energy.period = Energy.kWhtoday_offset;
// //         Settings.energy_kWhtoday = Energy.kWhtoday_offset;
// //         pCONT_set->RtcSettings.energy_kWhtoday = Energy.kWhtoday_offset;
// //         Energy.daily = (float)Energy.kWhtoday_offset / 100000;
// //         if (!pCONT_set->RtcSettings.energy_kWhtotal && !Energy.kWhtoday_offset) {
// //           Settings.energy_kWhtotal_time = LocalTime();
// //         }
// //         break;
// //       case 2:
// //         // Reset Energy Yesterday
// //         Settings.energy_kWhyesterday = lnum *100;
// //         break;
// //       case 3:
// //         // Reset Energy Total
// //         pCONT_set->RtcSettings.energy_kWhtotal = lnum *100;
// //         Settings.energy_kWhtotal = pCONT_set->RtcSettings.energy_kWhtotal;
// // //        Energy.total = (float)(pCONT_set->RtcSettings.energy_kWhtotal + Energy.kWhtoday_offset + Energy.kWhtoday) / 100000;
// //         Settings.energy_kWhtotal_time = (!Energy.kWhtoday_offset) ? LocalTime() : Midnight();
// //         pCONT_set->RtcSettings.energy_usage.last_usage_kWhtotal = (uint32_t)(Energy.total * 1000);
// //         break;
// //       }
// //     }
// //   }
// //   else if ((XdrvMailbox.index > 3) && (XdrvMailbox.index <= 5)) {
// //     char *p;
// //     char *str = strtok_r(XdrvMailbox.data, ", ", &p);
// //     int32_t position = -1;
// //     uint32_t values[2];

// //     while ((str != nullptr) && (position < 1)) {
// //       uint32_t value = strtoul(str, nullptr, 10);
// //       position++;
// //       values[position] = value *100;
// //       str = strtok_r(nullptr, ", ", &p);
// //     }

// //     switch (XdrvMailbox.index)
// //     {
// //       case 4:
// //         // Reset energy_usage.usage totals
// //         if (position > -1) {
// //           pCONT_set->RtcSettings.energy_usage.usage1_kWhtotal = values[0];
// //         }
// //         if (position > 0) {
// //           pCONT_set->RtcSettings.energy_usage.usage2_kWhtotal = values[1];
// //         }
// //         Settings.energy_usage.usage1_kWhtotal = pCONT_set->RtcSettings.energy_usage.usage1_kWhtotal;
// //         Settings.energy_usage.usage2_kWhtotal = pCONT_set->RtcSettings.energy_usage.usage2_kWhtotal;
// //         break;
// //       case 5:
// //         // Reset energy_usage.return totals
// //         if (position > -1) {
// //           pCONT_set->RtcSettings.energy_usage.return1_kWhtotal = values[0];
// //         }
// //         if (position > 0) {
// //           pCONT_set->RtcSettings.energy_usage.return2_kWhtotal = values[1];
// //         }
// //         Settings.energy_usage.return1_kWhtotal = pCONT_set->RtcSettings.energy_usage.return1_kWhtotal;
// //         Settings.energy_usage.return2_kWhtotal = pCONT_set->RtcSettings.energy_usage.return2_kWhtotal;
// //         break;
// //       }
// //   }

// //   Energy.total = (float)(pCONT_set->RtcSettings.energy_kWhtotal + Energy.kWhtoday_offset + Energy.kWhtoday) / 100000;

//   char energy_total_chr[FLOATSZ];
//   pCONT_sup->dtostrfd(Energy.total, pCONT_set->Settings.flag_power.energy_resolution, energy_total_chr);
//   char energy_daily_chr[FLOATSZ];
//   pCONT_sup->dtostrfd(Energy.daily, pCONT_set->Settings.flag_power.energy_resolution, energy_daily_chr);
//   char energy_yesterday_chr[FLOATSZ];




// //   pCONT_sup->dtostrfd((float)Settings.energy_kWhyesterday / 100000, pCONT_set->Settings.flag_power.energy_resolution, energy_yesterday_chr);

// //   char energy_usage1_chr[FLOATSZ];
// //   pCONT_sup->dtostrfd((float)Settings.energy_usage.usage1_kWhtotal / 100000, pCONT_set->Settings.flag_power.energy_resolution, energy_usage1_chr);
// //   char energy_usage2_chr[FLOATSZ];
// //   pCONT_sup->dtostrfd((float)Settings.energy_usage.usage2_kWhtotal / 100000, pCONT_set->Settings.flag_power.energy_resolution, energy_usage2_chr);
// //   char energy_return1_chr[FLOATSZ];
// //   pCONT_sup->dtostrfd((float)Settings.energy_usage.return1_kWhtotal / 100000, pCONT_set->Settings.flag_power.energy_resolution, energy_return1_chr);
// //   char energy_return2_chr[FLOATSZ];
// //   pCONT_sup->dtostrfd((float)Settings.energy_usage.return2_kWhtotal / 100000, pCONT_set->Settings.flag_power.energy_resolution, energy_return2_chr);

// //   Response_P(PSTR("{\"%s\":{\"" D_JSON_TOTAL "\":%s,\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s,\"" D_JSON_USAGE "\":[%s,%s],\"" D_JSON_EXPORT "\":[%s,%s]}}"),
// //     XdrvMailbox.command, energy_total_chr, energy_yesterday_chr, energy_daily_chr, energy_usage1_chr, energy_usage2_chr, energy_return1_chr, energy_return2_chr);
// }

// void mEnergyInterface::CmndTariff(void)
// {
//   // // Tariff1 22:00,23:00 - Tariff1 start hour for Standard Time and Daylight Savings Time
//   // // Tariff2 6:00,7:00   - Tariff2 start hour for Standard Time and Daylight Savings Time
//   // // Tariffx 1320, 1380  = minutes and also 22:003:00
//   // // Tariffx 223      = hours and also 22:003:00
//   // // Tariff9 0/1

//   // if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
//   //   uint32_t tariff = XdrvMailbox.index -1;
//   //   uint32_t time_type = 0;
//   //   char *p;
//   //   char *str = strtok_r(XdrvMailbox.data, ", ", &p);  // 23:152:30
//   //   while ((str != nullptr) && (time_type < 2)) {
//   //     char *q;
//   //     uint32_t value = strtol(str, &q, 10);            // 23 or 22
//   //     Settings.tariff[tariff][time_type] = value;
//   //     if (value < 24) {                                // Below 24 is hours
//   //       Settings.tariff[tariff][time_type] *= 60;      // Multiply hours by 60 minutes
//   //       char *minute = strtok_r(nullptr, ":", &q);
//   //       if (minute) {
//   //         value = strtol(minute, nullptr, 10);         // 15 or 30
//   //         if (value > 59) {
//   //           value = 59;
//   //         }
//   //         Settings.tariff[tariff][time_type] += value;
//   //       }
//   //     }
//   //     if (Settings.tariff[tariff][time_type] > 1439) {
//   //       Settings.tariff[tariff][time_type] = 1439;     // Max is 23:59
//   //     }
//   //     str = strtok_r(nullptr, ", ", &p);
//   //     time_type++;
//   //   }
//   // }
//   // else if (XdrvMailbox.index == 9) {
//   //   Settings.flag_network.energy_weekend = XdrvMailbox.payload & 1;  // CMND_TARIFF
//   // }
//   // Response_P(PSTR("{\"%s\":{\"Off-Peak\":{\"STD\":\"%s\",\"DST\":\"%s\"},\"Standard\":{\"STD\":\"%s\",\"DST\":\"%s\"},\"Weekend\":\"%s\"}}"),
//   //   XdrvMailbox.command,
//   //   GetMinuteTime(Settings.tariff[0][0]).c_str(),GetMinuteTime(Settings.tariff[0][1]).c_str(),
//   //   GetMinuteTime(Settings.tariff[1][0]).c_str(),GetMinuteTime(Settings.tariff[1][1]).c_str(),
//   //   GetStateText(Settings.flag_network.energy_weekend));             // CMND_TARIFF
// }

// void mEnergyInterface::CmndPowerCal(void)
// {
//   // Energy.command_code = CMND_POWERCAL;
//   // if (XnrgCall(FUNC_COMMAND)) {  // microseconds
//   //   if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
//   //     Settings.energy_power_calibration = XdrvMailbox.payload;
//   //   }
//   //   ResponseCmndNumber(Settings.energy_power_calibration);
//   // }
// }

// void mEnergyInterface::CmndVoltageCal(void)
// {
//   // Energy.command_code = CMND_VOLTAGECAL;
//   // if (XnrgCall(FUNC_COMMAND)) {  // microseconds
//   //   if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
//   //     Settings.energy_voltage_calibration = XdrvMailbox.payload;
//   //   }
//   //   ResponseCmndNumber(Settings.energy_voltage_calibration);
//   // }
// }

// void mEnergyInterface::CmndCurrentCal(void)
// {
//   // Energy.command_code = CMND_CURRENTCAL;
//   // if (XnrgCall(FUNC_COMMAND)) {  // microseconds
//   //   if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
//   //     Settings.energy_current_calibration = XdrvMailbox.payload;
//   //   }
//   //   ResponseCmndNumber(Settings.energy_current_calibration);
//   // }
// }

// void mEnergyInterface::CmndPowerSet(void)
// {
//   // Energy.command_code = CMND_POWERSET;
//   // if (XnrgCall(FUNC_COMMAND)) {  // Watt
//   //   EnergyCommandCalResponse(Settings.energy_power_calibration);
//   // }
// }

// void mEnergyInterface::CmndVoltageSet(void)
// {
//   // Energy.command_code = CMND_VOLTAGESET;
//   // if (XnrgCall(FUNC_COMMAND)) {  // Volt
//   //   EnergyCommandCalResponse(Settings.energy_voltage_calibration);
//   // }
// }

// void mEnergyInterface::CmndCurrentSet(void)
// {
//   // Energy.command_code = CMND_CURRENTSET;
//   // if (XnrgCall(FUNC_COMMAND)) {  // milliAmpere
//   //   EnergyCommandCalResponse(Settings.energy_current_calibration);
//   // }
// }

// void mEnergyInterface::CmndFrequencySet(void)
// {
//   // Energy.command_code = CMND_FREQUENCYSET;
//   // if (XnrgCall(FUNC_COMMAND)) {  // Hz
//   //   EnergyCommandCalResponse(Settings.energy_frequency_calibration);
//   // }
// }

// void mEnergyInterface::CmndModuleAddress(void)
// {
//   // if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 4) && (1 == Energy.phase_count)) {
//   //   Energy.command_code = CMND_MODULEADDRESS;
//   //   if (XnrgCall(FUNC_COMMAND)) {  // Module address
//   //     ResponseCmndDone();
//   //   }
//   // }
// }

// // #ifdef USE_ENERGY_MARGIN_DETECTION
// void mEnergyInterface::CmndPowerDelta(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32000)) {
//   //   Settings.energy_power_delta = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_power_delta);
// }

// void mEnergyInterface::CmndPowerLow(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_min_power = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_min_power);
// }

// void mEnergyInterface::CmndPowerHigh(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power);
// }

// void mEnergyInterface::CmndVoltageLow(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
//   //   Settings.energy_min_voltage = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_min_voltage);
// }

// void mEnergyInterface::CmndVoltageHigh(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
//   //   Settings.energy_max_voltage = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_voltage);
// }

// void mEnergyInterface::CmndCurrentLow(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
//   //   Settings.energy_min_current = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_min_current);
// }

// void mEnergyInterface::CmndCurrentHigh(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
//   //   Settings.energy_max_current = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_current);
// }

// // #ifdef USE_ENERGY_POWER_LIMIT
// void mEnergyInterface::CmndMaxPower(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power_limit = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_limit);
// }

// void mEnergyInterface::CmndMaxPowerHold(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power_limit_hold = (1 == XdrvMailbox.payload) ? MAX_POWER_HOLD : XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_limit_hold);
// }

// void mEnergyInterface::CmndMaxPowerWindow(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power_limit_window = (1 == XdrvMailbox.payload) ? MAX_POWER_WINDOW : XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_limit_window);
// }

// void mEnergyInterface::CmndSafePower(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power_safe_limit = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_safe_limit);
// }

// void mEnergyInterface::CmndSafePowerHold(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power_safe_limit_hold = (1 == XdrvMailbox.payload) ? SAFE_POWER_HOLD : XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_safe_limit_hold);
// }

// void mEnergyInterface::CmndSafePowerWindow(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 1440)) {
//   //   Settings.energy_max_power_safe_limit_window = (1 == XdrvMailbox.payload) ? SAFE_POWER_WINDOW : XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_safe_limit_window);
// }

// void mEnergyInterface::CmndMaxEnergy(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_energy = XdrvMailbox.payload;
//   //   Energy.max_energy_state  = 3;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_energy);
// }

// void mEnergyInterface::CmndMaxEnergyStart(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 24)) {
//   //   Settings.energy_max_energy_start = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_energy_start);
// }
// // #endif  // USE_ENERGY_POWER_LIMIT
// // #endif  // USE_ENERGY_MARGIN_DETECTION




#endif // USE_MODULE_SENSORS_INTERFACE
