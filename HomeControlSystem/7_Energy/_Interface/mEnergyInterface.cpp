
#include "mEnergyInterface.h"

#ifdef USE_MODULE_ENERGY_INTERFACE

const char* mEnergyInterface::PM_MODULE_ENERGY_INTERFACE_CTR = D_MODULE_ENERGY_INTERFACE_CTR;
const char* mEnergyInterface::PM_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR = D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR;


int8_t mEnergyInterface::Tasker(uint8_t function, JsonParserObject obj)
{

  int8_t function_result = 0;
  
  switch(function){
    case FUNC_PRE_INIT:
      // Pre_Init();
    break;
    case FUNC_SETTINGS_DEFAULT:
      Settings_Default();
    break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand_BootSafe(obj);
    break; 
  }

  // if(!pCONT_set->runtime_var.energy_driver){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_INIT:
      Init();
    break;
    /************
     * SETTINGS SECTION * 
    *******************/
    // case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
    //   Settings_Load();
    // break;
    // case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
    //   Settings_Save();
    // break;
    // case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
    //   Settings_Default();
    // break;
    // case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
    //   Settings_Default();
    //   // pCONT_set->SettingsSave(2);
    // break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      // EveryLoop();
    break;  
    // case FUNC_EVERY_200_MSECOND:
    //   //Energy200ms();
    // break;
    case FUNC_EVERY_SECOND:
      EnergyEverySecond();
      UpdateThresholdLimits();
      //  mqtthandler_sensor_ifchanged.flags.SendNow = true;

    break;
    case FUNC_EVERY_MINUTE:
      //UpdateEnergyUsagePerMinute();
    break;
    case FUNC_EVERY_MIDNIGHT:
    
    break;


    case FUNC_SENSOR_UPDATED:

//recalculate energy

    break;


    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break; 
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Draw_Table();
    break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break;
    #endif //USE_MODULE_NETWORK_WEBSERVER
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
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

}








void mEnergyInterface::Init(void)
{


// This should really be completely dynamic, but right now size is forced
for(int i=0;i<MAX_ENERGY_SENSORS;i++){
  Energy.voltage[i] = 0;
  Energy.current[i] = 0;
  Energy.active_power[i] = 0;
  Energy.apparent_power[i] = 0;
  Energy.reactive_power[i] = 0;
  Energy.power_factor[i] = 0;
  Energy.frequency[i] = 0;
  Energy.energy_read[i] = 0;
}




  if (pCONT_set->runtime_var.energy_driver) {
    // // if (RtcSettingsValid()) {
    //   Energy.kWhtoday_offset = pCONT_set->RtcSettings.energy_kWhtoday;
    // // }
    // else if (RtcTime.day_of_year == Settings.energy_kWhdoy) {
    //   Energy.kWhtoday_offset = Settings.energy_kWhtoday;
    // }
    // else {
      Energy.kWhtoday_offset = 0;
    // }
    Energy.kWhtoday = 0;
    Energy.kWhtoday_delta = 0;
    Energy.period = Energy.kWhtoday_offset;
    EnergyUpdateToday();
    //ticker_energy.attach_ms(200, Energy200ms);
  }

  for(uint8_t ii=0;ii<MAX_ENERGY_SENSORS;ii++){

    // Disable all values
    parameter_thresholds[ii].flags.data = (uint16_t)0; //disable all
    // Enable select flags
    parameter_thresholds[ii].flags.mqtt_report_threshold_transition = 1; //disable all

    // Voltage
    parameter_thresholds[ii].voltage.upper_limit = 260;
    parameter_thresholds[ii].voltage.lower_limit = 210;
    parameter_thresholds[ii].voltage.over_limit_seconds_counter = 0;
    parameter_thresholds[ii].voltage.over_limit_seconds_trigger_value = 120;
    // Current
    parameter_thresholds[ii].current.upper_limit = 4.0f;
    parameter_thresholds[ii].current.lower_limit = 1.0f;
    parameter_thresholds[ii].current.over_limit_seconds_counter = 0;
    parameter_thresholds[ii].current.over_limit_seconds_trigger_value = 5;
    // active_power
    parameter_thresholds[ii].active_power.upper_limit = 260;
    parameter_thresholds[ii].active_power.lower_limit = 210;
    parameter_thresholds[ii].active_power.over_limit_seconds_counter = 0;
    parameter_thresholds[ii].active_power.over_limit_seconds_trigger_value = 120;
    // Current
    // parameter_thresholds[ii].current.upper_limit = 260;
    // parameter_thresholds[ii].current.lower_limit = 210;
    // parameter_thresholds[ii].current.over_limit_seconds_counter = 0;
    // parameter_thresholds[ii].current.over_limit_seconds_trigger_value = 120;
    // frequency
    parameter_thresholds[ii].frequency.upper_limit = 260;
    parameter_thresholds[ii].frequency.lower_limit = 210;
    parameter_thresholds[ii].frequency.over_limit_seconds_counter = 0;
    parameter_thresholds[ii].frequency.over_limit_seconds_trigger_value = 120;
    // power_factor
    parameter_thresholds[ii].power_factor.upper_limit = 260;
    parameter_thresholds[ii].power_factor.lower_limit = 210;
    parameter_thresholds[ii].power_factor.over_limit_seconds_counter = 0;
    parameter_thresholds[ii].power_factor.over_limit_seconds_trigger_value = 120;
    // energy
    parameter_thresholds[ii].energy.upper_limit = 260;
    parameter_thresholds[ii].energy.lower_limit = 210;
    parameter_thresholds[ii].energy.over_limit_seconds_counter = 0;
    parameter_thresholds[ii].energy.over_limit_seconds_trigger_value = 120;

  } // END for

  //single changes
  
    parameter_thresholds[0].current.lower_limit = 0;


  
}


/**
 * Init the default values, these will be configured before reading templates
 * */
void mEnergyInterface::Settings_Default()
{
  pCONT_iEnergy->Energy.phase_count = 0;


}



void mEnergyInterface::SetIDWithAddress(uint8_t address_id, uint8_t address_to_save)//, uint8_t address_length)
{
  
  // for(int ii=0;ii<address_length;ii++){
    
  //   address[address_id][ii] = address_to_save[ii];

    // address.insert(address_id, 2);//*address_to_save);
    address.push_back(address_to_save);

  // }

}

uint8_t mEnergyInterface::GetAddressWithID(uint8_t address_id)//, uint8_t* address_to_get, uint8_t address_length)
{
  // if(address_id > address.size()-1)
  // {
  // return 0;
  //   }
  // AddLog(LOG_LEVEL_TEST, PSTR("address.size() = %d"),address.size());

  if(address.size())
  {
    
  // for(int ii=0;ii<address_length;ii++){
    return address[address_id];
  //   address[address_id][ii] = address_to_save[ii];
  }
    // address.insert(address_id, 2);//*address_to_save);
    // address.push_back(*address_to_save);
return 0;
  // }

}


void mEnergyInterface::SetEnergyDeviceCount(uint8_t address_length)
{

  pCONT_iEnergy->Energy.phase_count = address_length;

  AddLog(LOG_LEVEL_TEST, PSTR("Energy.phase_count=%d"),pCONT_iEnergy->Energy.phase_count);

}





// /********************************************************************************************/

bool mEnergyInterface::EnergyTariff1Active()  // Off-Peak hours
{

  // uint8_t dst = 0;
  // if (pCONT_time->IsDst() && (pCONT_set->Settings.energy_usage.tariff[0][1] != pCONT_set->Settings.energy_usage.tariff[1][1])) {
  //   dst = 1;
  // }
  // if (pCONT_set->Settings.energy_usage.tariff[0][dst] != pCONT_set->Settings.energy_usage.tariff[1][dst]) {
  //   if (pCONT_set->Settings.flag_network.energy_weekend && ((
  //     pCONT_time->RtcTime.day_of_week == 1) ||   // CMND_TARIFF
  //                                         (pCONT_time->RtcTime.day_of_week == 7))) {
  //     return true;
  //   }
  //   uint32_t minutes = pCONT_time->MinutesPastMidnight();
  //   if (pCONT_set->Settings.energy_usage.tariff[0][dst] > pCONT_set->Settings.energy_usage.tariff[1][dst]) {
  //     // {"Tariff":{"Off-Peak":{"STD":"22:00","DST":"23:00"},"Standard":{"STD":"06:00","DST":"07:00"},"Weekend":"OFF"}}
  //     return ((minutes >= pCONT_set->Settings.energy_usage.tariff[0][dst]) || (minutes < pCONT_set->Settings.energy_usage.tariff[1][dst]));
  //   } else {
  //     // {"Tariff":{"Off-Peak":{"STD":"00:29","DST":"01:29"},"Standard":{"STD":"07:29","DST":"08:29"},"Weekend":"OFF"}}
  //     return ((minutes >= pCONT_set->Settings.energy_usage.tariff[0][dst]) && (minutes < pCONT_set->Settings.energy_usage.tariff[1][dst]));
  //   }
  // } else {
  //   return false;
  // }
}

void mEnergyInterface::EnergyUpdateToday(void)
{
 
  //  if (Energy.kWhtoday_delta > 1000) {
  //   unsigned long delta = Energy.kWhtoday_delta / 1000;
  //   Energy.kWhtoday_delta -= (delta * 1000);
  //   Energy.kWhtoday += delta;
  // }

  // pCONT_set->RtcSettings.energy_kWhtoday = Energy.kWhtoday_offset + Energy.kWhtoday;
  // Energy.daily = (float)(pCONT_set->RtcSettings.energy_kWhtoday) / 100000;
  // Energy.total = (float)(pCONT_set->RtcSettings.energy_kWhtotal + pCONT_set->RtcSettings.energy_kWhtoday) / 100000;

  // //if (pCONT_set->RtcTime.valid){ // We calc the difference only if we have a valid RTC time.

  //   uint32_t energy_diff = (uint32_t)(Energy.total * 100000) - pCONT_set->RtcSettings.energy_usage.last_usage_kWhtotal;
  //   pCONT_set->RtcSettings.energy_usage.last_usage_kWhtotal = (uint32_t)(Energy.total * 100000);

  //   uint32_t return_diff = 0;
  //   if (!isnan(Energy.export_active)) {
  //     return_diff = (uint32_t)(Energy.export_active * 100000) - pCONT_set->RtcSettings.energy_usage.last_return_kWhtotal;
  //     pCONT_set->RtcSettings.energy_usage.last_return_kWhtotal = (uint32_t)(Energy.export_active * 100000);
  //   }

  //   if (EnergyTariff1Active()) {  // Tarrif1 = Off-Peak
  //     pCONT_set->RtcSettings.energy_usage.usage1_kWhtotal += energy_diff;
  //     pCONT_set->RtcSettings.energy_usage.return1_kWhtotal += return_diff;
  //   } else {
  //     pCONT_set->RtcSettings.energy_usage.usage2_kWhtotal += energy_diff;
  //     pCONT_set->RtcSettings.energy_usage.return2_kWhtotal += return_diff;
  //   }
  // //}
  
}

void mEnergyInterface::EnergyUpdateTotal(float value, bool kwh)
{
  
  char energy_total_chr[FLOATSZ];
  pCONT_sup->dtostrfd(value, 4, energy_total_chr);
  AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Energy Total %s %sWh"), energy_total_chr, (kwh) ? "k" : "");

  uint32_t multiplier = (kwh) ? 100000 : 100;  // kWh or Wh to deca milli Wh

  if (0 == Energy.start_energy || (value < Energy.start_energy)) {
    Energy.start_energy = value;  // Init after restart and handle roll-over if any
  }
  else if (value != Energy.start_energy) {
    Energy.kWhtoday = (unsigned long)((value - Energy.start_energy) * multiplier);
  }

  if((Energy.total < (value - 0.01)) &&       // We subtract a little offset to avoid continuous updates
      pCONT_set->Settings.flag_network.hardware_energy_total) {  // SetOption72 - Enable hardware energy total counter as reference (#6561)
    pCONT_set->RtcSettings.energy_kWhtotal = (unsigned long)((value * multiplier) - Energy.kWhtoday_offset - Energy.kWhtoday);
    pCONT_set->Settings.energy_usage.energy_kWhtotal = pCONT_set->RtcSettings.energy_kWhtotal;
    Energy.total = (float)(pCONT_set->RtcSettings.energy_kWhtotal + Energy.kWhtoday_offset + Energy.kWhtoday) / 100000;
    pCONT_set->Settings.energy_usage.energy_kWhtotal_time = (!Energy.kWhtoday_offset) ? pCONT_time->LocalTime() : pCONT_time->Midnight();
    AddLog(LOG_LEVEL_DEBUG, PSTR("NRG: Energy Total updated with hardware value"));
  }
  EnergyUpdateToday();
  
}

/*********************************************************************************************/

void mEnergyInterface::Energy200ms(void)
{
  
  // Energy.power_on = (pCONT_set->power != 0) | pCONT_set->Settings.flag_system.no_power_on_check;  // SetOption21 - Show voltage even if powered off

  // Energy.fifth_second++;
  // if (5 == Energy.fifth_second) {
  //   Energy.fifth_second = 0;

  //   //XnrgCall(FUNC_ENERGY_EVERY_SECOND);

  //   if (pCONT_set->RtcTime.valid) {
  //     if (pCONT_time->LocalTime() == pCONT_time->Midnight()) {
  //       pCONT_set->Settings.energy_usage.energy_kWhyesterday = pCONT_set->RtcSettings.energy_kWhtoday;

  //       pCONT_set->RtcSettings.energy_kWhtotal += pCONT_set->RtcSettings.energy_kWhtoday;
  //       pCONT_set->Settings.energy_usage.energy_kWhtotal = pCONT_set->RtcSettings.energy_kWhtotal;
  //       Energy.kWhtoday = 0;
  //       Energy.kWhtoday_offset = 0;
  //       pCONT_set->RtcSettings.energy_kWhtoday = 0;
  //       Energy.start_energy = 0;

  //       Energy.kWhtoday_delta = 0;
  //       Energy.period = Energy.kWhtoday;
  //       EnergyUpdateToday();
  //       #if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
  //         Energy.max_energy_state  = 3;
  //       #endif  // USE_ENERGY_POWER_LIMIT
  //     }
  //     #if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
  //       if ((RtcTime.hour == Settings.energy_max_energy_start) && (3 == Energy.max_energy_state )) {
  //         Energy.max_energy_state  = 0;
  //       }
  //     #endif  // USE_ENERGY_POWER_LIMIT
  //   }
  // }

  //XnrgCall(FUNC_EVERY_200_MSECOND);
}

void mEnergyInterface::EnergySaveState(void)
{
  // Settings.energy_kWhdoy = (RtcTime.valid) ? RtcTime.day_of_year : 0;

  // Settings.energy_kWhtoday = pCONT_set->RtcSettings.energy_kWhtoday;
  // Settings.energy_kWhtotal = pCONT_set->RtcSettings.energy_kWhtotal;

  // Settings.energy_usage = pCONT_set->RtcSettings.energy_usage;
}

// #ifdef USE_ENERGY_MARGIN_DETECTION
// bool mEnergyInterface::EnergyMargin(bool type, uint16_t margin, uint16_t value, bool &flag){//, bool &save_flag)
// {
//   // bool change;

//   // if (!margin) return false;
//   // change = save_flag;
//   // if (type) {
//   //   flag = (value > margin);
//   // } else {
//   //   flag = (value < margin);
//   // }
//   // save_flag = flag;
//   // return (change != save_flag);
// }

void mEnergyInterface::EnergyMarginCheck(void)
{
//   if (Energy.power_steady_counter) {
//     Energy.power_steady_counter--;
//     return;
//   }

//   uint16_t energy_power_u = (uint16_t)(Energy.active_power[0]);

//   if (Settings.energy_power_delta) {
//     uint16_t delta = abs(Energy.power_history[0] - energy_power_u);
//     uint16_t min_power = (Energy.power_history[0] > energy_power_u) ? energy_power_u : Energy.power_history[0];

//     DEBUG_DRIVER_LOG(PSTR("NRG: Delta %d, Power %d"), delta, min_power);

//     if ((delta > 0) && (min_power > 0)) {  // Fix divide by 0 exception (#6741)
//       if (((Settings.energy_power_delta < 101) && (((delta * 100) / min_power) > Settings.energy_power_delta)) ||  // 1..100 = Percentage
//           ((Settings.energy_power_delta > 100) && (delta > (Settings.energy_power_delta -100)))) {                 // 101..32000 = Absolute
//         Energy.power_delta = true;
//         Energy.power_history[1] = Energy.active_power[0];  // We only want one report so reset history
//         Energy.power_history[2] = Energy.active_power[0];
//       }
//     }
//   }
//   Energy.power_history[0] = Energy.power_history[1];  // Shift in history every second allowing power changes to settle for up to three seconds
//   Energy.power_history[1] = Energy.power_history[2];
//   Energy.power_history[2] = energy_power_u;

//   if (Energy.power_on && (Settings.energy_min_power || Settings.energy_max_power || Settings.energy_min_voltage || Settings.energy_max_voltage || Settings.energy_min_current || Settings.energy_max_current)) {
//     uint16_t energy_voltage_u = (uint16_t)(Energy.voltage[0]);
//     uint16_t energy_current_u = (uint16_t)(Energy.current[0] * 1000);

//     DEBUG_DRIVER_LOG(PSTR("NRG: W %d, U %d, I %d"), energy_power_u, energy_voltage_u, energy_current_u);

//     Response_P(PSTR("{"));
//     bool flag;
//     bool jsonflg = false;
//     if (EnergyMargin(false, Settings.energy_min_power, energy_power_u, flag, Energy.min_power_flag)) {
//       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_JSON_POWERLOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
//       jsonflg = true;
//     }
//     if (EnergyMargin(true, Settings.energy_max_power, energy_power_u, flag, Energy.max_power_flag)) {
//       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_JSON_POWERHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
//       jsonflg = true;
//     }
//     if (EnergyMargin(false, Settings.energy_min_voltage, energy_voltage_u, flag, Energy.min_voltage_flag)) {
//       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_JSON_VOLTAGELOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
//       jsonflg = true;
//     }
//     if (EnergyMargin(true, Settings.energy_max_voltage, energy_voltage_u, flag, Energy.max_voltage_flag)) {
//       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_JSON_VOLTAGEHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
//       jsonflg = true;
//     }
//     if (EnergyMargin(false, Settings.energy_min_current, energy_current_u, flag, Energy.min_current_flag)) {
//       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_JSON_CURRENTLOW "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
//       jsonflg = true;
//     }
//     if (EnergyMargin(true, Settings.energy_max_current, energy_current_u, flag, Energy.max_current_flag)) {
//       pCONT_sup->ResponseAppend_P(PSTR("%s\"" D_JSON_CURRENTHIGH "\":\"%s\""), (jsonflg)?",":"", GetStateText(flag));
//       jsonflg = true;
//     }
//     if (jsonflg) {
//       ResponseJsonEnd();
//       MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_MARGINS), MQTT_TELE_RETAIN);
//       EnergyMqttShow();
//     }
//   }

// #ifdef USE_ENERGY_POWER_LIMIT
//   // Max Power
//   if (Settings.energy_max_power_limit) {
//     if (Energy.active_power[0] > Settings.energy_max_power_limit) {
//       if (!Energy.mplh_counter) {
//         Energy.mplh_counter = Settings.energy_max_power_limit_hold;
//       } else {
//         Energy.mplh_counter--;
//         if (!Energy.mplh_counter) {
//           ResponseTime_P(PSTR(",\"" D_JSON_MAXPOWERREACHED "\":%d}"), energy_power_u);
//           MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
//           EnergyMqttShow();
//           SetAllPower(POWER_ALL_OFF, SRC_MAXPOWER);
//           if (!Energy.mplr_counter) {
//             Energy.mplr_counter = Settings.param[P_MAX_POWER_RETRY] +1;
//           }
//           Energy.mplw_counter = Settings.energy_max_power_limit_window;
//         }
//       }
//     }
//     else if (power && (energy_power_u <= Settings.energy_max_power_limit)) {
//       Energy.mplh_counter = 0;
//       Energy.mplr_counter = 0;
//       Energy.mplw_counter = 0;
//     }
//     if (!power) {
//       if (Energy.mplw_counter) {
//         Energy.mplw_counter--;
//       } else {
//         if (Energy.mplr_counter) {
//           Energy.mplr_counter--;
//           if (Energy.mplr_counter) {
//             ResponseTime_P(PSTR(",\"" D_JSON_POWERMONITOR "\":\"%s\"}"), GetStateText(1));
//             MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_JSON_POWERMONITOR));
//             RestorePower(true, SRC_MAXPOWER);
//           } else {
//             ResponseTime_P(PSTR(",\"" D_JSON_MAXPOWERREACHEDRETRY "\":\"%s\"}"), GetStateText(0));
//             MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
//             EnergyMqttShow();
//           }
//         }
//       }
//     }
//   }

//   // Max Energy
//   if (Settings.energy_max_energy) {
//     uint16_t energy_daily_u = (uint16_t)(Energy.daily * 1000);
//     if (!Energy.max_energy_state  && (RtcTime.hour == Settings.energy_max_energy_start)) {
//       Energy.max_energy_state  = 1;
//       ResponseTime_P(PSTR(",\"" D_JSON_ENERGYMONITOR "\":\"%s\"}"), GetStateText(1));
//       MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_JSON_ENERGYMONITOR));
//       RestorePower(true, SRC_MAXENERGY);
//     }
//     else if ((1 == Energy.max_energy_state ) && (energy_daily_u >= Settings.energy_max_energy)) {
//       Energy.max_energy_state  = 2;
//       char stemp[FLOATSZ];
//       pCONT_sup->dtostrfd(Energy.daily, 3, stemp);
//       ResponseTime_P(PSTR(",\"" D_JSON_MAXENERGYREACHED "\":%s}"), stemp);
//       MqttPublishPrefixTopic_P(STAT, S_RSLT_WARNING);
//       EnergyMqttShow();
//       SetAllPower(POWER_ALL_OFF, SRC_MAXENERGY);
//     }
//   }
// #endif  // USE_ENERGY_POWER_LIMIT

//   if (Energy.power_delta) { EnergyMqttShow(); }
}


void mEnergyInterface::EnergyEverySecond(void)
{
  // Overtemp check
  // if (global_update) {
  //   if (power && (global_temperature != 9999) && (global_temperature > Settings.param[P_OVER_TEMP])) {  // Device overtemp, turn off relays
  //     SetAllPower(POWER_ALL_OFF, SRC_OVERTEMP);
  //   }
  // }
    
  // Invalid data reset
  uint32_t data_valid = Energy.phase_count;
  for (uint32_t i = 0; i < Energy.phase_count; i++) {
    if (Energy.data_valid[i] <= ENERGY_WATCHDOG) {
      Energy.data_valid[i]++;
      if (Energy.data_valid[i] > ENERGY_WATCHDOG) {
        // Reset energy registers
        Energy.voltage[i] = 0;
        Energy.current[i] = 0;
        Energy.active_power[i] = 0;
        if (!isnan(Energy.apparent_power[i])) { Energy.apparent_power[i] = 0; }
        if (!isnan(Energy.reactive_power[i])) { Energy.reactive_power[i] = 0; }
        if (!isnan(Energy.frequency[i]))      { Energy.frequency[i] = 0; }
        if (!isnan(Energy.power_factor[i]))   { Energy.power_factor[i] = 0; }
        data_valid--;
      }
    }
  }
  if (!data_valid) {
    if (!isnan(Energy.export_active)) { Energy.export_active = 0; }
    Energy.start_energy = 0;
    pCONT->Tasker_Interface(FUNC_ENERGY_RESET);
  }

  // #ifdef USE_ENERGY_MARGIN_DETECTION
  //   EnergyMarginCheck();
  // #endif  // USE_ENERGY_MARGIN_DETECTION
}




void mEnergyInterface::UpdateEnergyUsagePerMinute(){

  // needs to add guard to wait on valid previous data

  // Get index values checking for loop around
  uint8_t index = Energy.stats.kwh_per_minute_index;
  uint8_t last_index = (index==0)?59:index-1;

  AddLog(LOG_LEVEL_TEST,PSTR("index=%d,last_index=%d"),index,last_index);

  // Get new power last minute
  Energy.stats.last_minutes_energy = Energy.stats.kwh_per_minute[last_index];
  // Get new power
  // Energy.stats.current_energy = pCONT_pzem->PzemAc.last_energy;
  //this minutes usage
  Energy.stats.this_minutes_energy = Energy.stats.current_energy - Energy.stats.last_minutes_energy;
  //store into array
  // Energy.stats.kwh_each_minute[index] = pCONT_pzem->PzemAc.last_energy;
  Energy.stats.kwh_per_minute[index] = Energy.stats.this_minutes_energy;
  
  AddLog(LOG_LEVEL_TEST,PSTR(D_JSON_COMMAND_NVALUE),"last_minutes_energy",(int)Energy.stats.last_minutes_energy);
  AddLog(LOG_LEVEL_TEST,PSTR(D_JSON_COMMAND_NVALUE),"current_energy",(int)Energy.stats.current_energy);
  AddLog(LOG_LEVEL_TEST,PSTR(D_JSON_COMMAND_NVALUE),"this_minutes_energy",(int)Energy.stats.this_minutes_energy);

  //increment index and save
  Energy.stats.kwh_per_minute_index = (index+1);

  if(Energy.stats.kwh_per_minute_index>59){
    Energy.stats.kwh_per_minute_index = 0;
  }

  // mqtthandler_sensor_ifchanged.flags.SendNow = true;

}



void mEnergyInterface::Settings_Load(){

  // name_buffer = pCONT_set->Settings.energy_usage.name_buffer;

}


void mEnergyInterface::Settings_Save(){

  // pCONT_set->Settings.energy_usage.name_buffer = name_buffer;

}


void mEnergyInterface::UpdateThresholdLimits(){



  for(uint8_t ii=0;ii<MAX_ENERGY_SENSORS;ii++){

    // Current
    if(!IsWithinLimits(parameter_thresholds[ii].current.lower_limit, Energy.current[ii], parameter_thresholds[ii].current.upper_limit)){
            
      parameter_thresholds[ii].current.over_limit_seconds_counter++; // add time over boundaries

      if(parameter_thresholds[ii].current.over_limit_seconds_counter > parameter_thresholds[ii].current.over_limit_seconds_trigger_value){
        parameter_thresholds[ii].current.over_limit_seconds_trigger_value_exceeded = true;
      }
    }else{
      parameter_thresholds[ii].current.over_limit_seconds_counter = 0; // reset counter
      parameter_thresholds[ii].current.over_limit_seconds_trigger_value_exceeded = false;
    }

  } // END for




}























#endif

