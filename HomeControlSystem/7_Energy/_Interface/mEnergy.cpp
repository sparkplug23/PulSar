#include "mEnergy.h"

#ifdef USE_MODULE_DRIVERS_ENERGY

const char HTTP_ENERGY_SNS1[] PROGMEM =
  "{s}" D_POWERUSAGE_APPARENT "{m}%s " D_UNIT_VA "{e}"
  "{s}" D_POWERUSAGE_REACTIVE "{m}%s " D_UNIT_VAR "{e}"
  "{s}" D_POWER_FACTOR "{m}%s{e}";

const char HTTP_ENERGY_SNS2[] PROGMEM =
  "{s}" D_ENERGY_TODAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_YESTERDAY "{m}%s " D_UNIT_KILOWATTHOUR "{e}"
  "{s}" D_ENERGY_TOTAL "{m}%s " D_UNIT_KILOWATTHOUR "{e}";      // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>

const char HTTP_ENERGY_SNS3[] PROGMEM =
  "{s}" D_EXPORT_ACTIVE "{m}%s " D_UNIT_KILOWATTHOUR "{e}";

void mEnergy::Init(void){

  if (pCONT_set->energy_flg) {
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
  
}

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

const char kEnergyPhases[] PROGMEM = "|%s / %s|%s / %s / %s||[%s,%s]|[%s,%s,%s]";


// /********************************************************************************************/

bool mEnergy::EnergyTariff1Active()  // Off-Peak hours
{

  uint8_t dst = 0;
  if (pCONT->mt->IsDst() && (pCONT_set->Settings.energy_usage.tariff[0][1] != pCONT_set->Settings.energy_usage.tariff[1][1])) {
    dst = 1;
  }
  if (pCONT_set->Settings.energy_usage.tariff[0][dst] != pCONT_set->Settings.energy_usage.tariff[1][dst]) {
    if (pCONT_set->Settings.flag_network.energy_weekend && ((pCONT_set->RtcTime.day_of_week == 1) ||   // CMND_TARIFF
                                          (pCONT_set->RtcTime.day_of_week == 7))) {
      return true;
    }
    uint32_t minutes = pCONT->mt->MinutesPastMidnight();
    if (pCONT_set->Settings.energy_usage.tariff[0][dst] > pCONT_set->Settings.energy_usage.tariff[1][dst]) {
      // {"Tariff":{"Off-Peak":{"STD":"22:00","DST":"23:00"},"Standard":{"STD":"06:00","DST":"07:00"},"Weekend":"OFF"}}
      return ((minutes >= pCONT_set->Settings.energy_usage.tariff[0][dst]) || (minutes < pCONT_set->Settings.energy_usage.tariff[1][dst]));
    } else {
      // {"Tariff":{"Off-Peak":{"STD":"00:29","DST":"01:29"},"Standard":{"STD":"07:29","DST":"08:29"},"Weekend":"OFF"}}
      return ((minutes >= pCONT_set->Settings.energy_usage.tariff[0][dst]) && (minutes < pCONT_set->Settings.energy_usage.tariff[1][dst]));
    }
  } else {
    return false;
  }
}

void mEnergy::EnergyUpdateToday(void)
{
 
   if (Energy.kWhtoday_delta > 1000) {
    unsigned long delta = Energy.kWhtoday_delta / 1000;
    Energy.kWhtoday_delta -= (delta * 1000);
    Energy.kWhtoday += delta;
  }

  pCONT_set->RtcSettings.energy_kWhtoday = Energy.kWhtoday_offset + Energy.kWhtoday;
  Energy.daily = (float)(pCONT_set->RtcSettings.energy_kWhtoday) / 100000;
  Energy.total = (float)(pCONT_set->RtcSettings.energy_kWhtotal + pCONT_set->RtcSettings.energy_kWhtoday) / 100000;

  //if (pCONT_set->RtcTime.valid){ // We calc the difference only if we have a valid RTC time.

    uint32_t energy_diff = (uint32_t)(Energy.total * 100000) - pCONT_set->RtcSettings.energy_usage.last_usage_kWhtotal;
    pCONT_set->RtcSettings.energy_usage.last_usage_kWhtotal = (uint32_t)(Energy.total * 100000);

    uint32_t return_diff = 0;
    if (!isnan(Energy.export_active)) {
      return_diff = (uint32_t)(Energy.export_active * 100000) - pCONT_set->RtcSettings.energy_usage.last_return_kWhtotal;
      pCONT_set->RtcSettings.energy_usage.last_return_kWhtotal = (uint32_t)(Energy.export_active * 100000);
    }

    if (EnergyTariff1Active()) {  // Tarrif1 = Off-Peak
      pCONT_set->RtcSettings.energy_usage.usage1_kWhtotal += energy_diff;
      pCONT_set->RtcSettings.energy_usage.return1_kWhtotal += return_diff;
    } else {
      pCONT_set->RtcSettings.energy_usage.usage2_kWhtotal += energy_diff;
      pCONT_set->RtcSettings.energy_usage.return2_kWhtotal += return_diff;
    }
  //}
  
}

void mEnergy::EnergyUpdateTotal(float value, bool kwh)
{
  
  char energy_total_chr[FLOATSZ];
  pCONT_sup->dtostrfd(value, 4, energy_total_chr);
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("NRG: Energy Total %s %sWh"), energy_total_chr, (kwh) ? "k" : "");

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
    pCONT_set->Settings.energy_usage.energy_kWhtotal_time = (!Energy.kWhtoday_offset) ? pCONT->mt->LocalTime() : pCONT->mt->Midnight();
    AddLog_P(LOG_LEVEL_DEBUG, PSTR("NRG: Energy Total updated with hardware value"));
  }
  EnergyUpdateToday();
  
}

/*********************************************************************************************/

void mEnergy::Energy200ms(void)
{
  
  Energy.power_on = (pCONT_set->power != 0) | pCONT_set->Settings.flag_system.no_power_on_check;  // SetOption21 - Show voltage even if powered off

  Energy.fifth_second++;
  if (5 == Energy.fifth_second) {
    Energy.fifth_second = 0;

    //XnrgCall(FUNC_ENERGY_EVERY_SECOND);

    if (pCONT_set->RtcTime.valid) {
      if (pCONT->mt->LocalTime() == pCONT->mt->Midnight()) {
        pCONT_set->Settings.energy_usage.energy_kWhyesterday = pCONT_set->RtcSettings.energy_kWhtoday;

        pCONT_set->RtcSettings.energy_kWhtotal += pCONT_set->RtcSettings.energy_kWhtoday;
        pCONT_set->Settings.energy_usage.energy_kWhtotal = pCONT_set->RtcSettings.energy_kWhtotal;
        Energy.kWhtoday = 0;
        Energy.kWhtoday_offset = 0;
        pCONT_set->RtcSettings.energy_kWhtoday = 0;
        Energy.start_energy = 0;

        Energy.kWhtoday_delta = 0;
        Energy.period = Energy.kWhtoday;
        EnergyUpdateToday();
        #if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
          Energy.max_energy_state  = 3;
        #endif  // USE_ENERGY_POWER_LIMIT
      }
      #if defined(USE_ENERGY_MARGIN_DETECTION) && defined(USE_ENERGY_POWER_LIMIT)
        if ((RtcTime.hour == Settings.energy_max_energy_start) && (3 == Energy.max_energy_state )) {
          Energy.max_energy_state  = 0;
        }
      #endif  // USE_ENERGY_POWER_LIMIT
    }
  }

  //XnrgCall(FUNC_EVERY_200_MSECOND);
}

void mEnergy::EnergySaveState(void)
{
  // Settings.energy_kWhdoy = (RtcTime.valid) ? RtcTime.day_of_year : 0;

  // Settings.energy_kWhtoday = pCONT_set->RtcSettings.energy_kWhtoday;
  // Settings.energy_kWhtotal = pCONT_set->RtcSettings.energy_kWhtotal;

  // Settings.energy_usage = pCONT_set->RtcSettings.energy_usage;
}

// #ifdef USE_ENERGY_MARGIN_DETECTION
// bool mEnergy::EnergyMargin(bool type, uint16_t margin, uint16_t value, bool &flag){//, bool &save_flag)
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

void mEnergy::EnergyMarginCheck(void)
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


void mEnergy::EnergyEverySecond(void)
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




void mEnergy::UpdateEnergyUsagePerMinute(){

  // needs to add guard to wait on valid previous data

  // Get index values checking for loop around
  uint8_t index = Energy.stats.kwh_per_minute_index;
  uint8_t last_index = (index==0)?59:index-1;

  AddLog_P(LOG_LEVEL_TEST,PSTR("index=%d,last_index=%d"),index,last_index);

  // Get new power last minute
  Energy.stats.last_minutes_energy = Energy.stats.kwh_per_minute[last_index];
  // Get new power
  Energy.stats.current_energy = pCONT->mspm->PzemAc.last_energy;
  //this minutes usage
  Energy.stats.this_minutes_energy = Energy.stats.current_energy - Energy.stats.last_minutes_energy;
  //store into array
  Energy.stats.kwh_each_minute[index] = pCONT->mspm->PzemAc.last_energy;
  Energy.stats.kwh_per_minute[index] = Energy.stats.this_minutes_energy;
  
  AddLog_P(LOG_LEVEL_TEST,PSTR(D_JSON_COMMAND_NVALUE),"last_minutes_energy",(int)Energy.stats.last_minutes_energy);
  AddLog_P(LOG_LEVEL_TEST,PSTR(D_JSON_COMMAND_NVALUE),"current_energy",(int)Energy.stats.current_energy);
  AddLog_P(LOG_LEVEL_TEST,PSTR(D_JSON_COMMAND_NVALUE),"this_minutes_energy",(int)Energy.stats.this_minutes_energy);

  //increment index and save
  Energy.stats.kwh_per_minute_index = (index+1);

  if(Energy.stats.kwh_per_minute_index>59){
    Energy.stats.kwh_per_minute_index = 0;
  }

  mqtthandler_sensor_ifchanged.flags.SendNow = true;

}







// // #ifdef USE_MODULE_CORE_WEBSERVER

const char* mEnergy::EnergyFormatIndex(char* result, char* input, bool json, uint32_t index, bool single)
{
  char layout[16];
  pCONT_sup->GetTextIndexed_P(layout, sizeof(layout), (index -1) + (3 * json), kEnergyPhases);
  
  switch (index) {
    case 2:
      snprintf_P(result, FLOATSZ *3, layout, input, input + FLOATSZ);  // Dirty
    break;
    case 3:
      snprintf_P(result, FLOATSZ *3, layout, input, input + FLOATSZ, input + FLOATSZ + FLOATSZ);  // Even dirtier
    break;
    default:
      snprintf_P(result, FLOATSZ *3, input);
    break;
  }

  // AddLog_P(LOG_LEVEL_TEST,PSTR("layout=%s"),layout);
  // AddLog_P(LOG_LEVEL_TEST,PSTR("result=%s"),result);
  // AddLog_P(LOG_LEVEL_TEST,PSTR("input=%s"),input);
  // AddLog_P(LOG_LEVEL_TEST,PSTR("json=%d"),json);
  // AddLog_P(LOG_LEVEL_TEST,PSTR("index=%d"),index);

  return result;
}

const char* mEnergy::EnergyFormat(char* result, char* input, bool json, bool single)
{
  // AddLog_P(LOG_LEVEL_TEST,PSTR("mEnergy::EnergyFormat"));
  // AddLog_P(LOG_LEVEL_TEST,PSTR("input=%s"),input);
  // AddLog_P(LOG_LEVEL_TEST,PSTR("json=%d"),json);
  uint8_t index = (single) ? 1 : pCONT_iEnergy->Energy.phase_count;  // 1,2,3
  const char* ch = EnergyFormatIndex(result, input, json, index, single);
  // AddLog_P(LOG_LEVEL_TEST,PSTR("mEnergy::EnergyFormat=%s"),ch);
  //TRACE();
  return ch;
}

void mEnergy::EnergyShow(bool json)
{

  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("mEnergy::EnergyShow(%d)"),(int)pCONT_iEnergy->Energy.phase_count);

  for (uint32_t i = 0; i < pCONT_iEnergy->Energy.phase_count; i++) {
    if (pCONT_iEnergy->Energy.voltage_common) {
      pCONT_iEnergy->Energy.voltage[i] = pCONT_iEnergy->Energy.voltage[0];
      AddLog_P(LOG_LEVEL_TEST,PSTR("Energy.voltage[i]"),(int)pCONT_iEnergy->Energy.voltage[i]);
    }
  }

  AddLog_P(LOG_LEVEL_TEST,PSTR("Energy.phase_count=%d"),pCONT_iEnergy->Energy.phase_count);
  float power_factor_knx = pCONT_iEnergy->Energy.power_factor[0];

  if(pCONT_iEnergy->Energy.phase_count>3){
    AddLog_P(LOG_LEVEL_ERROR,PSTR("MEMORY ISSUE"));
    return;
  }

  char apparent_power_chr[pCONT_iEnergy->Energy.phase_count][FLOATSZ];
  char reactive_power_chr[pCONT_iEnergy->Energy.phase_count][FLOATSZ];
  char power_factor_chr[pCONT_iEnergy->Energy.phase_count][FLOATSZ];
  char frequency_chr[pCONT_iEnergy->Energy.phase_count][FLOATSZ];
  
  memset(apparent_power_chr,0,sizeof(apparent_power_chr));
  memset(reactive_power_chr,0,sizeof(reactive_power_chr));
  memset(power_factor_chr,0,sizeof(power_factor_chr));
  memset(frequency_chr,0,sizeof(frequency_chr));

  if (!pCONT_iEnergy->Energy.type_dc) {
    AddLog_P(LOG_LEVEL_TEST,PSTR("!pCONT_iEnergy->Energy.type_dc"));
    if (pCONT_iEnergy->Energy.current_available && pCONT_iEnergy->Energy.voltage_available) {
      AddLog_P(LOG_LEVEL_TEST,PSTR("current_available"));
      for (uint32_t i = 0; i < pCONT_iEnergy->Energy.phase_count; i++) {
        float apparent_power = pCONT_iEnergy->Energy.apparent_power[i];
        if (isnan(apparent_power)) {
          apparent_power = pCONT_iEnergy->Energy.voltage[i] * pCONT_iEnergy->Energy.current[i];
        }
        if (apparent_power < pCONT_iEnergy->Energy.active_power[i]) {  // Should be impossible
          pCONT_iEnergy->Energy.active_power[i] = apparent_power;
        }

        float power_factor = pCONT_iEnergy->Energy.power_factor[i];
        if (isnan(power_factor)) {
          power_factor = (pCONT_iEnergy->Energy.active_power[i] && apparent_power) ? pCONT_iEnergy->Energy.active_power[i] / apparent_power : 0;
          if (power_factor > 1) {
            power_factor = 1;
          }
        }
        if (0 == i) { power_factor_knx = power_factor; }

        float reactive_power = pCONT_iEnergy->Energy.reactive_power[i];
        if (isnan(reactive_power)) {
          reactive_power = 0;
          uint32_t difference = ((uint32_t)(apparent_power * 100) - (uint32_t)(pCONT_iEnergy->Energy.active_power[i] * 100)) / 10;
          if ((pCONT_iEnergy->Energy.current[i] > 0.005) && ((difference > 15) || (difference > (uint32_t)(apparent_power * 100 / 1000)))) {
            // calculating reactive power only if current is greater than 0.005A and
            // difference between active and apparent power is greater than 1.5W or 1%
            AddLog_P(LOG_LEVEL_INFO,PSTR("reactive_power_A=%d"),reactive_power);
            reactive_power = (float)(pCONT_sup->RoundSqrtInt((uint32_t)(apparent_power * apparent_power * 100) - (uint32_t)(pCONT_iEnergy->Energy.active_power[i] * pCONT_iEnergy->Energy.active_power[i] * 100))) / 10;
            AddLog_P(LOG_LEVEL_INFO,PSTR("reactive_power_B=%d"),reactive_power);
            }
        }

        pCONT_sup->dtostrfd(apparent_power, pCONT_set->Settings.flag_power.wattage_resolution, apparent_power_chr[i]);
        pCONT_sup->dtostrfd(reactive_power, pCONT_set->Settings.flag_power.wattage_resolution, reactive_power_chr[i]);
        pCONT_sup->dtostrfd(power_factor, 2, power_factor_chr[i]);
        
        AddLog_P(LOG_LEVEL_TEST,PSTR("apparent_power=%d,apparent_power_chr[%d]=%s"),(int)apparent_power,i,apparent_power_chr[i]);
        AddLog_P(LOG_LEVEL_TEST,PSTR("reactive_power_chr[%d]=%s"),i,reactive_power_chr[i]);
        AddLog_P(LOG_LEVEL_TEST,PSTR("power_factor_chr[%d]=%s"),i,power_factor_chr[i]);

      }
    }
    for (uint32_t i = 0; i < pCONT_iEnergy->Energy.phase_count; i++) {
      float frequency = pCONT_iEnergy->Energy.frequency[i];
      if (isnan(pCONT_iEnergy->Energy.frequency[i])) {
        frequency = 0;
      }
      pCONT_sup->dtostrfd(frequency, pCONT_set->Settings.flag_power.frequency_resolution, frequency_chr[i]);
    }
  }

  char voltage_chr[pCONT_iEnergy->Energy.phase_count][FLOATSZ];
  char current_chr[pCONT_iEnergy->Energy.phase_count][FLOATSZ];
  char active_power_chr[pCONT_iEnergy->Energy.phase_count][FLOATSZ];

  memset(voltage_chr,0,sizeof(voltage_chr));
  memset(current_chr,0,sizeof(current_chr));
  memset(active_power_chr,0,sizeof(active_power_chr));

  for (uint32_t i = 0; i < pCONT_iEnergy->Energy.phase_count; i++) {
    AddLog_P(LOG_LEVEL_TEST,PSTR("pCONT_iEnergy->Energy.phase_count=%d %d"),pCONT_iEnergy->Energy.phase_count,i);
    pCONT_sup->dtostrfd(pCONT_iEnergy->Energy.voltage[i], pCONT_set->Settings.flag_power.voltage_resolution, voltage_chr[i]);
    pCONT_sup->dtostrfd(pCONT_iEnergy->Energy.current[i], pCONT_set->Settings.flag_power.current_resolution, current_chr[i]);
    pCONT_sup->dtostrfd(pCONT_iEnergy->Energy.active_power[i], pCONT_set->Settings.flag_power.wattage_resolution, active_power_chr[i]);
  }

  // AddLog_P(LOG_LEVEL_TEST,PSTR("pCONT_iEnergy->Energy.phase_count=%d"),pCONT_iEnergy->Energy.phase_count);
  // AddLog_P(LOG_LEVEL_TEST,PSTR("pCONT_iEnergy->Energy.voltage[i]=%d"),(int)pCONT_iEnergy->Energy.voltage[0]);
  // AddLog_P(LOG_LEVEL_TEST,PSTR("pCONT_iEnergy->Energy.voltage_chr[i]=%s"),voltage_chr[0]);

  char energy_daily_chr[FLOATSZ];
  memset(energy_daily_chr,0,sizeof(energy_daily_chr));
  pCONT_sup->dtostrfd(pCONT_iEnergy->Energy.daily, pCONT_set->Settings.flag_power.energy_resolution, energy_daily_chr);
 
  char energy_yesterday_chr[FLOATSZ];
  memset(energy_yesterday_chr,0,sizeof(energy_yesterday_chr)); 
  pCONT_sup->dtostrfd((float)pCONT_set->Settings.energy_usage.energy_kWhyesterday / 100000, pCONT_set->Settings.flag_power.energy_resolution, energy_yesterday_chr);

  char energy_total_chr[3][FLOATSZ];
  memset(energy_total_chr,0,sizeof(energy_total_chr));
  pCONT_sup->dtostrfd(pCONT_iEnergy->Energy.total, pCONT_set->Settings.flag_power.energy_resolution, energy_total_chr[0]);

  char export_active_chr[3][FLOATSZ];
  memset(export_active_chr,0,sizeof(export_active_chr));
  pCONT_sup->dtostrfd(pCONT_iEnergy->Energy.export_active, pCONT_set->Settings.flag_power.energy_resolution, export_active_chr[0]);

  uint8_t energy_total_fields = 1;

  if (pCONT_set->Settings.energy_usage.tariff[0][0] != pCONT_set->Settings.energy_usage.tariff[1][0]) {
    pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.usage1_kWhtotal / 100000, pCONT_set->Settings.flag_power.energy_resolution, energy_total_chr[1]);  // Tariff1
    pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.usage2_kWhtotal / 100000, pCONT_set->Settings.flag_power.energy_resolution, energy_total_chr[2]);  // Tariff2
    pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.return1_kWhtotal / 100000, pCONT_set->Settings.flag_power.energy_resolution, export_active_chr[1]);  // Tariff1
    pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.return2_kWhtotal / 100000, pCONT_set->Settings.flag_power.energy_resolution, export_active_chr[2]);  // Tariff2
    energy_total_fields = 3;
  }

  char value_chr[FLOATSZ *3];   // Used by EnergyFormatIndex
  memset(value_chr,0,sizeof(value_chr)); 
  char value2_chr[FLOATSZ *3];
  memset(value2_chr,0,sizeof(value2_chr)); 
  char value3_chr[FLOATSZ *3];
  memset(value3_chr,0,sizeof(value3_chr)); 

  // if (json) {
    // bool show_energy_period = (0 == pCONT_set->tele_period);

    // pCONT_sup->ResponseAppend_P(PSTR(",\"" D_RSLT_ENERGY "\":{\"" D_JSON_TOTAL_START_TIME "\":\"%s\",\"" D_JSON_TOTAL "\":%s,\"" D_JSON_YESTERDAY "\":%s,\"" D_JSON_TODAY "\":%s"),
    //   pCONT->mt->GetDateAndTime(DT_ENERGY),
    //   EnergyFormatIndex(value_chr, energy_total_chr[0], json, energy_total_fields),
    //   energy_yesterday_chr,
    //   energy_daily_chr);

    // if (!isnan(pCONT_iEnergy->Energy.export_active)) {
    //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_EXPORT_ACTIVE "\":%s"),
    //     EnergyFormatIndex(value_chr, export_active_chr[0], json, energy_total_fields));
    // }

    // if (show_energy_period) {
    //   float energy = 0;
    //   if (pCONT_iEnergy->Energy.period) {
    //     energy = (float)(pCONT_set->RtcSettings.energy_kWhtoday - pCONT_iEnergy->Energy.period) / 100;
    //   }
    //   pCONT_iEnergy->Energy.period = pCONT_set->RtcSettings.energy_kWhtoday;
    //   char energy_period_chr[FLOATSZ];
    //   pCONT_sup->dtostrfd(energy, pCONT_set->Settings.flag_power.wattage_resolution, energy_period_chr);
    //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_PERIOD "\":%s"), energy_period_chr);
    // }
    // pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_POWERUSAGE "\":%s"),
    //   EnergyFormat(value_chr, active_power_chr[0], json));
    // if (!pCONT_iEnergy->Energy.type_dc) {
    //   if (pCONT_iEnergy->Energy.current_available && pCONT_iEnergy->Energy.voltage_available) {
    //     pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_APPARENT_POWERUSAGE "\":%s,\"" D_JSON_REACTIVE_POWERUSAGE "\":%s,\"" D_JSON_POWERFACTOR "\":%s"),
    //       EnergyFormat(value_chr, apparent_power_chr[0], json),
    //       EnergyFormat(value2_chr, reactive_power_chr[0], json),
    //       EnergyFormat(value3_chr, power_factor_chr[0], json));
    //   }
    //   if (!isnan(pCONT_iEnergy->Energy.frequency[0])) {
    //     pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_FREQUENCY "\":%s"),
    //       EnergyFormat(value_chr, frequency_chr[0], json, pCONT_iEnergy->Energy.voltage_common));
    //   }
    // }
    // if (pCONT_iEnergy->Energy.voltage_available) {
    //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_VOLTAGE "\":%s"),
    //     EnergyFormat(value_chr, voltage_chr[0], json, pCONT_iEnergy->Energy.voltage_common));
    // }
    // if (pCONT_iEnergy->Energy.current_available) {
    //   pCONT_sup->ResponseAppend_P(PSTR(",\"" D_JSON_CURRENT "\":%s"),
    //     EnergyFormat(value_chr, current_chr[0], json));
    // }

    // AddLog_P(LOG_LEVEL_TEST,PSTR(" mEnergy::FUNC_JSON_APPEND "));

    // Tasker(FUNC_JSON_APPEND);
    // pCONT_sup->ResponseJsonEnd();

// #ifdef USE_DOMOTICZ
//     if (show_energy_period) {  // Only send if telemetry
//       pCONT_sup->dtostrfd(pCONT_iEnergy->Energy.total * 1000, 1, energy_total_chr[0]);
//       DomoticzSensorPowerEnergy((int)pCONT_iEnergy->Energy.active_power[0], energy_total_chr[0]);  // PowerUsage, EnergyToday

//       pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.usage1_kWhtotal / 100, 1, energy_total_chr[1]);  // Tariff1
//       pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.usage2_kWhtotal / 100, 1, energy_total_chr[2]);  // Tariff2
//       pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.return1_kWhtotal / 100, 1, export_active_chr[1]);
//       pCONT_sup->dtostrfd((float)pCONT_set->RtcSettings.energy_usage.return2_kWhtotal / 100, 1, export_active_chr[2]);
//       DomoticzSensorP1SmartMeter(energy_total_chr[1], energy_total_chr[2], export_active_chr[1], export_active_chr[2], (int)pCONT_iEnergy->Energy.active_power[0]);

//       if (pCONT_iEnergy->Energy.voltage_available) {
//         DomoticzSensor(DZ_VOLTAGE, voltage_chr[0]);  // Voltage
//       }
//       if (pCONT_iEnergy->Energy.current_available) {
//         DomoticzSensor(DZ_CURRENT, current_chr[0]);  // Current
//       }
//     }
// #endif  // USE_DOMOTICZ
// #ifdef USE_KNX
//     if (show_energy_period) {
//       if (pCONT_iEnergy->Energy.voltage_available) {
//         KnxSensor(KNX_ENERGY_VOLTAGE, pCONT_iEnergy->Energy.voltage[0]);
//       }
//       if (pCONT_iEnergy->Energy.current_available) {
//         KnxSensor(KNX_ENERGY_CURRENT, pCONT_iEnergy->Energy.current[0]);
//       }
//       KnxSensor(KNX_ENERGY_POWER, pCONT_iEnergy->Energy.active_power[0]);
//       if (!pCONT_iEnergy->Energy.type_dc) {
//         KnxSensor(KNX_ENERGY_POWERFACTOR, power_factor_knx);
//       }
//       KnxSensor(KNX_ENERGY_DAILY, pCONT_iEnergy->Energy.daily);
//       KnxSensor(KNX_ENERGY_TOTAL, pCONT_iEnergy->Energy.total);
//       KnxSensor(KNX_ENERGY_START, pCONT_iEnergy->Energy.start_energy);
//     }
// #endif  // USE_KNX
// #ifdef USE_MODULE_CORE_WEBSERVER
//   // } else {

//       if (pCONT_iEnergy->Energy.voltage_available) {
//         pCONT_web->WSContentSend_PD(HTTP_SNS_VOLTAGE, 
//           EnergyFormat(value_chr, voltage_chr[0], json, pCONT_iEnergy->Energy.voltage_common)
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
// #endif  // USE_MODULE_CORE_WEBSERVER
  //} //if json
}



uint8_t mEnergy::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();

  JsonBuilderI->Add(D_JSON_CHANNELCOUNT,         pCONT_iEnergy->Energy.phase_count);
  JsonBuilderI->Array_AddArray(D_JSON_VOLTAGE,              pCONT_iEnergy->Energy.voltage,       pCONT_iEnergy->Energy.phase_count);
  JsonBuilderI->Array_AddArray(D_JSON_CURRENT,              pCONT_iEnergy->Energy.current,       pCONT_iEnergy->Energy.phase_count);
  JsonBuilderI->Array_AddArray(D_JSON_ACTIVE_POWERUSAGE,    pCONT_iEnergy->Energy.active_power,  pCONT_iEnergy->Energy.phase_count);
  JsonBuilderI->Array_AddArray(D_JSON_APPARENT_POWERUSAGE,  pCONT_iEnergy->Energy.apparent_power,pCONT_iEnergy->Energy.phase_count);
  JsonBuilderI->Array_AddArray(D_JSON_REACTIVE_POWERUSAGE,  pCONT_iEnergy->Energy.reactive_power,pCONT_iEnergy->Energy.phase_count);
  JsonBuilderI->Array_AddArray(D_JSON_POWERFACTOR,          pCONT_iEnergy->Energy.power_factor,  pCONT_iEnergy->Energy.phase_count);
  JsonBuilderI->Array_AddArray(D_JSON_FREQUENCY,            pCONT_iEnergy->Energy.frequency,     pCONT_iEnergy->Energy.phase_count);
  JsonBuilderI->Add(D_JSON_ENERGY,               pCONT_iEnergy->Energy.energy);
  JsonBuilderI->Add(D_JSON_ENERGYLAST,           pCONT_iEnergy->Energy.last_energy);
  
  if(json_method >= JSON_LEVEL_DETAILED){
    JsonBuilderI->Level_Start(D_JSON_KWH_STATS);
      JsonBuilderI->Add(D_JSON_INDEX, pCONT_iEnergy->Energy.stats.kwh_per_minute_index);
      JsonBuilderI->Add(D_JSON_ENERGY "LastMinute", pCONT_iEnergy->Energy.stats.last_minutes_energy);
      JsonBuilderI->Add(D_JSON_ENERGY "Currently",  pCONT_iEnergy->Energy.stats.current_energy);
      JsonBuilderI->Add(D_JSON_ENERGY "ThisMinute", pCONT_iEnergy->Energy.stats.this_minutes_energy);
      JsonBuilderI->Array_AddArray("kwh_per_minute",           Energy.stats.kwh_per_minute,     sizeof(Energy.stats.kwh_per_minute)/4);
      JsonBuilderI->Array_AddArray("kwh_each_minute",          Energy.stats.kwh_each_minute,    sizeof(Energy.stats.kwh_each_minute)/4);
    JsonBuilderI->Level_End();
  }

  return JsonBuilderI->End();

}



// /*


//   float start_energy = 0;                       // 12345.12345 kWh total previous
//   float daily = 0;                              // 123.123 kWh
//   float total = 0;                              // 12345.12345 kWh total energy
//   float export_active = NAN;                    // 123.123 KWh

//   unsigned long kWhtoday_delta = 0;             // 1212312345 Wh 10^-5 (deca micro Watt hours) - Overflows to Energy.kWhtoday (HLW and CSE only)
//   unsigned long kWhtoday_offset = 0;            // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily
//   unsigned long kWhtoday;                       // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily
//   unsigned long period = 0;                     // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily

//   uint8_t fifth_second = 0;
//   uint8_t command_code = 0;
//   uint8_t data_valid[3] = { 0, 0, 0 };

//   uint8_t phase_count = 1;                      // Number of phases active
//   bool voltage_common = false;                  // Use single voltage and frequency

//   bool voltage_available = true;                // Enable if voltage is measured
//   bool current_available = true;                // Enable if current is measured

//   bool type_dc = false;
//   bool power_on = true;

// // #ifdef USE_ENERGY_MARGIN_DETECTION
//   uint16_t power_history[3] = { 0 };
//   uint8_t power_steady_counter = 8;  // Allow for power on stabilization
//   bool power_delta = false;
//   bool min_power_flag = false;
//   bool max_power_flag = false;
//   bool min_voltage_flag = false;
//   bool max_voltage_flag = false;
//   bool min_current_flag = false;
//   bool max_current_flag = false;

// // #ifdef USE_ENERGY_POWER_LIMIT
//   uint16_t mplh_counter = 0;
//   uint16_t mplw_counter = 0;
//   uint8_t mplr_counter = 0;
//   uint8_t max_energy_state  = 0;
// // #endif  // USE_ENERGY_POWER_LIMIT
// // #endif  // USE_ENERGY_MARGIN_DETECTION




void mEnergy::Settings_Load(){

  // name_buffer = pCONT_set->Settings.energy_usage.name_buffer;

}

void mEnergy::Settings_Default(){
  
  // name_buffer = &pCONT_set->Settings.energy_usage.name_buffer[0];

  // uint8_t buffer_length = 0;
  // // memset(name_buffer,0,sizeof(name_buffer));
  // // for(int ii=0;ii<8;ii++){
  //   buffer_length+=sprintf(name_buffer+buffer_length,"%s|","Mains"); 
  //   buffer_length+=sprintf(name_buffer+buffer_length,"%s|","Cooker"); 
  //   buffer_length+=sprintf(name_buffer+buffer_length,"%s|","Immersion"); 
  //   buffer_length+=sprintf(name_buffer+buffer_length,"%s|","TumbleDryer"); 
  //   buffer_length+=sprintf(name_buffer+buffer_length,"%s|","Garage"); 
  //   buffer_length+=sprintf(name_buffer+buffer_length,"%s|","Shower"); 
  //   buffer_length+=sprintf(name_buffer+buffer_length,"%s|","Sockets"); 
  //   buffer_length+=sprintf(name_buffer+buffer_length,"%s|","Kitchen"); 
  // // }

}

void mEnergy::Settings_Save(){

  // pCONT_set->Settings.energy_usage.name_buffer = name_buffer;

}


    
int8_t mEnergy::Tasker(uint8_t function, uint8_t optional_id)
{
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parse_JSONCommand(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
  // return 0;

  // testing without pzem hardware
  // pCONT_set->energy_flg = 5; //?

  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    case FUNC_PRE_INIT:
      // Pre_Init();
    break;
  }

  // Only continue to remaining functions if sensor has been detected and enabled
  if(!pCONT_set->energy_flg){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

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
    case FUNC_EVERY_200_MSECOND:
      //Energy200ms();
    break;
    case FUNC_EVERY_SECOND:
      //EnergyEverySecond();
    break;
    case FUNC_EVERY_MINUTE:
      //UpdateEnergyUsagePerMinute();
    break;
    case FUNC_EVERY_MIDNIGHT:
    
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_COMMAND:

    break;  
    case FUNC_JSON_COMMAND:
      //function_result = parse_JSONCommand();
    break;      
    case FUNC_SAVE_BEFORE_RESTART:
      //EnergySaveState();
    break;
    /************
     * WEBPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_WEBSERVER
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
      WebAppend_Root_Draw_PageTable();
    break;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
      WebAppend_Root_Status_Table();
    break;
    #endif //USE_MODULE_CORE_WEBSERVER
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); 
    break;
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MQTT
  }
  
  return function_result;

}

void mEnergy::WebAppend_Root_Draw_Table(){
  char buffer[30];
  BufferWriterI->Append_P(PSTR("{t}"));
  // BufferWriterI->Append_P(PSTR("<table style='border: 1px solid white;border-collapse:collapse;'>"));
  //headers
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
  for(int col=0;col<9;col++){
    if(col==0){ //first column blank
      // BufferWriterI->Append_P(PSTR("<th>    Parameter /\nDevice      </th>"));
      BufferWriterI->Append_P(PSTR("<th></th>"));
    }else{
      // BufferWriterI->Append_P(PSTR("<th>%s</th>"), pCONT_sup->GetTextIndexed_P(buffer, sizeof(buffer), ii, PM_DLIM_LIST_TABLE_HEADERS));
        BufferWriterI->Append_P(PSTR("<td>%s</td>"), pCONT_set->GetDeviceName(D_MODULE_DRIVERS_ENERGY_ID,col-1,buffer,sizeof(buffer)));//"Animation List Tester");      //titles are fixed, so send them here using getindex    
    }
  }    
  BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
  //rows
  for(int row=0;row<7;row++){
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
    for(int col=0;col<9;col++){
      if(col==0){ //row name
        BufferWriterI->Append_P(PSTR("<th>%s</th>"), pCONT_sup->GetTextIndexed_P(buffer, sizeof(buffer), row, PM_DLIM_LIST_TABLE_HEADERS));
        // BufferWriterI->Append_P(PSTR("<td>%s</td>"), pCONT_set->GetDeviceName(D_MODULE_DRIVERS_ENERGY_ID,row,buffer,sizeof(buffer)));//"Animation List Tester");      //titles are fixed, so send them here using getindex
      }else{
        BufferWriterI->Append_P(PSTR("<td>{dc}%s'>" D_DEFAULT_HTML_VALUE "</div></td>"),"ener_tab");  
      }
    }
    BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
  }
  BufferWriterI->Append_P(PSTR("{t2}")); 
}

//append to internal buffer if any root messages table
void mEnergy::WebAppend_Root_Status_Table(){

  char buffer[50];
  uint8_t count = 0;
  
  JsonBuilderI->Array_Start("ener_tab");// Class name
  for(int row=0;row<6;row++){
    for(int device=0;device<8;device++){//pCONT_iEnergy->Energy.phase_count;device++){
      JsonBuilderI->Level_Start();
        JsonBuilderI->Add("id",count++);
        //add for flag, to highlight row where power is "in use"/high
        if(pCONT_pzem->pzem_modbus[device].active_power>30){
          JsonBuilderI->Add("fc","#ff0000");
        }else{
          JsonBuilderI->Add("fc","#ffffff");
        }
        // JsonBuilderI->Add_FP("ih","\"c%d d%d\"", count-1, device);
        switch(row){
          case 0: JsonBuilderI->Add("ih",pCONT_pzem->pzem_modbus[device].voltage); break;
          case 1: JsonBuilderI->Add("ih",pCONT_pzem->pzem_modbus[device].current); break;
          case 2: JsonBuilderI->Add("ih",pCONT_pzem->pzem_modbus[device].active_power); break;
          case 3: JsonBuilderI->Add("ih",pCONT_pzem->pzem_modbus[device].frequency); break;
          case 4: JsonBuilderI->Add("ih",pCONT_pzem->pzem_modbus[device].power_factor); break;
          case 5: JsonBuilderI->Add("ih",pCONT_pzem->pzem_modbus[device].energy); break;
        } //switch      
      JsonBuilderI->Level_End();
    }
  }//end for
  JsonBuilderI->Array_End();
  

  // char colour_button[8];
  // char button_ids[] = {ANIMATION_MODE_FLASHER_ID, ANIMATION_MODE_SCENE_ID, 
  // #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
  // ANIMATION_MODE_NOTIFICATIONS_ID, 
  // #endif
  // ANIMATION_MODE_NONE_ID};
  
  // JsonBuilderI->Array_Start("animod");// Class name
  // for(int row=0;row<sizeof(button_ids);row++){
  //   if(button_ids[row] == animation.mode_id){
  //     sprintf(colour_button,"%s\0",COLOR_BUTTON_HIGHLIGHT); //selected
  //   }else{
  //     sprintf(colour_button,"%s\0",COLOR_BUTTON); //NOT selected
  //   }        
  //   JsonBuilderI->Level_Start();
  //     JsonBuilderI->Add("id",row);
  //     JsonBuilderI->Add("bc",colour_button);
  //   JsonBuilderI->Level_End();
  // }  
  // JsonBuilderI->Array_End();

}




void mEnergy::parse_JSONCommand(){

  return;// THIS IS WRONG!!

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/energy")>=0){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_NEXTION));
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
    // fOpenHABDataStreamActive_last_secs = 1; // set to be positive to start
    // fOpenHABDataStreamActive = true;
  }else{
    return; // not meant for here
  }

  // u
  // int8_t tmp_id = 0;

  // #ifdef JSONDOCUMENT_STATIC
  //   StaticJsonDocument<800> doc;
  // #else
  //   DynamicJsonDocument doc(600);
  // #endif
  // DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
  
  // if(error){
  //   AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_ERROR_JSON_DESERIALIZATION));
  //   Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_ERROR_JSON_DESERIALIZATION);
  //   return 0;
  // }
  // JsonObject obj = doc.as<JsonObject>();
  
  // if(!obj["command"].isNull()){ 
  //   const char* command = obj["command"];
  //   if(strstr(command,"system_send_all")){ 
  //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"command\"=\"system_send_all\""));
  //     MQTTHandler_Set_fSendNow();
  //     isserviced++;
  //   }
  //   else{
  //     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
  //   }
  // }

  // 

} // END FUNCTION


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mEnergy::ConstructJSON_Settings(uint8_t json_method){

    memset(&data_buffer,0,sizeof(data_buffer));
    // StaticJsonDocument<400> doc;
    // JsonObject root = doc.to<JsonObject>();

    // char buffer[15];

    // root["json_teleperiod_level"] = pCONT_set->GetTelePeriodJsonLevelCtr(buffer);

    // // root["name_buffer"] = name_buffer;

    // data_buffer.payload.len = measureJson(root)+1;
    // serializeJson(doc,data_buffer.payload.ctr);

    return 0;//data_buffer.payload.len>3?true:false;

}


uint8_t mEnergy::ConstructJSON_EnergyStats(uint8_t json_method){

  memset(&data_buffer,0,sizeof(data_buffer));

  // StaticJsonDocument<1000> doc;
  // JsonObject root = doc.to<JsonObject>();

  // uint8_t ischanged=false;

  // char channel_ctr[3];
  // memset(channel_ctr,0,sizeof(channel_ctr));
  // // sprintf(channel_ctr,"%02d",channel);
  // //JsonObject energy_total_obj = root.createNestedObject("Energy_Total");

  //   JsonObject kwh_per_minute_obj = root.createNestedObject("kwh_stats"); 
    
    
  //   kwh_per_minute_obj["index"] = Energy.stats.kwh_per_minute_index;
  //   kwh_per_minute_obj["last_minutes_energy"] = Energy.stats.last_minutes_energy;
  //   kwh_per_minute_obj["current_energy"] = Energy.stats.current_energy;
  //   kwh_per_minute_obj["this_minutes_energy"] = Energy.stats.this_minutes_energy;


  // // if(json_method >= JSON_LEVEL_DETAILED){
  // //   JsonArray kwh_per_minute_arr = kwh_per_minute_obj.createNestedArray("kwh_per_minute"); 
  // //   JsonArray kwh_each_minute_arr = kwh_per_minute_obj.createNestedArray("kwh_each_minute");  
  // //   for (uint8_t min = 0; min < 60; min++) {
  // //     kwh_per_minute_arr.add(pCONT_iEnergy->Energy.stats.kwh_per_minute[min]);
  // //     kwh_each_minute_arr.add(pCONT_iEnergy->Energy.stats.kwh_each_minute[min]);
  // //   }
  // // }

  // data_buffer.payload.len = measureJson(root)+1;
  // serializeJson(doc,data_buffer.payload.ctr);
  // return data_buffer.payload.len>3?true:false;

  return 0;
    
}



/*
#ifdef ENABLE_POWER_MONITORING

uint8_t mTelemetry::ConstructJSON_PowerMonitor(uint8_t json_level){ 

  // // clear entire mqtt packet
  // memset(&data_buffer,0,sizeof(data_buffer));


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


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/
////////////////////// START OF MQTT /////////////////////////

void mEnergy::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mEnergy::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mEnergy::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mEnergy::ConstructJSON_Sensor;
  
  mqtthandler_ptr = &mqtthandler_energystats_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_ENERGY_STATS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mEnergy::ConstructJSON_EnergyStats;
  
  mqtthandler_ptr = &mqtthandler_energystats_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 10; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_ENERGY_STATS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mEnergy::ConstructJSON_EnergyStats;

} //end "MQTTHandler_Init"


void mEnergy::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;
  mqtthandler_energystats_ifchanged.flags.SendNow = true;
  mqtthandler_energystats_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mEnergy::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_energystats_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"



void mEnergy::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mSensorsDHT>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_SENSORS_DHT_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );
  uint8_t flag_handle_all = false, handler_found = false
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                       handler_found=true; mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_SENSOR_IFCHANGED_ID:               handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_ifchanged; break;
      case MQTT_HANDLER_SENSOR_TELEPERIOD_ID:              handler_found=true; mqtthandler_ptr=&mqtthandler_sensor_teleperiod; break;
      case MQTT_HANDLER_MODULE_ENERGYSTATS_IFCHANGED_ID:   handler_found=true; mqtthandler_ptr=&mqtthandler_energystats_ifchanged; break;
      case MQTT_HANDLER_MODULE_ENERGYSTATS_TELEPERIOD_ID:  handler_found=true; mqtthandler_ptr=&mqtthandler_energystats_teleperiod; break;
      default: handler_found=false; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(handler_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_DRIVERS_ENERGY_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}


////////////////////// END OF MQTT /////////////////////////






















// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/

// void mEnergy::EnergyCommandCalResponse(uint32_t nvalue)
// {
//   // snprintf_P(XdrvMailbox.command, CMDSZ, PSTR("%sCal"), XdrvMailbox.command);
//   // ResponseCmndNumber(nvalue);
// }

// void mEnergy::CmndEnergyReset(void)
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

// void mEnergy::CmndTariff(void)
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

// void mEnergy::CmndPowerCal(void)
// {
//   // Energy.command_code = CMND_POWERCAL;
//   // if (XnrgCall(FUNC_COMMAND)) {  // microseconds
//   //   if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
//   //     Settings.energy_power_calibration = XdrvMailbox.payload;
//   //   }
//   //   ResponseCmndNumber(Settings.energy_power_calibration);
//   // }
// }

// void mEnergy::CmndVoltageCal(void)
// {
//   // Energy.command_code = CMND_VOLTAGECAL;
//   // if (XnrgCall(FUNC_COMMAND)) {  // microseconds
//   //   if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
//   //     Settings.energy_voltage_calibration = XdrvMailbox.payload;
//   //   }
//   //   ResponseCmndNumber(Settings.energy_voltage_calibration);
//   // }
// }

// void mEnergy::CmndCurrentCal(void)
// {
//   // Energy.command_code = CMND_CURRENTCAL;
//   // if (XnrgCall(FUNC_COMMAND)) {  // microseconds
//   //   if ((XdrvMailbox.payload > 999) && (XdrvMailbox.payload < 32001)) {
//   //     Settings.energy_current_calibration = XdrvMailbox.payload;
//   //   }
//   //   ResponseCmndNumber(Settings.energy_current_calibration);
//   // }
// }

// void mEnergy::CmndPowerSet(void)
// {
//   // Energy.command_code = CMND_POWERSET;
//   // if (XnrgCall(FUNC_COMMAND)) {  // Watt
//   //   EnergyCommandCalResponse(Settings.energy_power_calibration);
//   // }
// }

// void mEnergy::CmndVoltageSet(void)
// {
//   // Energy.command_code = CMND_VOLTAGESET;
//   // if (XnrgCall(FUNC_COMMAND)) {  // Volt
//   //   EnergyCommandCalResponse(Settings.energy_voltage_calibration);
//   // }
// }

// void mEnergy::CmndCurrentSet(void)
// {
//   // Energy.command_code = CMND_CURRENTSET;
//   // if (XnrgCall(FUNC_COMMAND)) {  // milliAmpere
//   //   EnergyCommandCalResponse(Settings.energy_current_calibration);
//   // }
// }

// void mEnergy::CmndFrequencySet(void)
// {
//   // Energy.command_code = CMND_FREQUENCYSET;
//   // if (XnrgCall(FUNC_COMMAND)) {  // Hz
//   //   EnergyCommandCalResponse(Settings.energy_frequency_calibration);
//   // }
// }

// void mEnergy::CmndModuleAddress(void)
// {
//   // if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 4) && (1 == Energy.phase_count)) {
//   //   Energy.command_code = CMND_MODULEADDRESS;
//   //   if (XnrgCall(FUNC_COMMAND)) {  // Module address
//   //     ResponseCmndDone();
//   //   }
//   // }
// }

// // #ifdef USE_ENERGY_MARGIN_DETECTION
// void mEnergy::CmndPowerDelta(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 32000)) {
//   //   Settings.energy_power_delta = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_power_delta);
// }

// void mEnergy::CmndPowerLow(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_min_power = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_min_power);
// }

// void mEnergy::CmndPowerHigh(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power);
// }

// void mEnergy::CmndVoltageLow(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
//   //   Settings.energy_min_voltage = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_min_voltage);
// }

// void mEnergy::CmndVoltageHigh(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 501)) {
//   //   Settings.energy_max_voltage = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_voltage);
// }

// void mEnergy::CmndCurrentLow(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
//   //   Settings.energy_min_current = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_min_current);
// }

// void mEnergy::CmndCurrentHigh(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 16001)) {
//   //   Settings.energy_max_current = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_current);
// }

// // #ifdef USE_ENERGY_POWER_LIMIT
// void mEnergy::CmndMaxPower(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power_limit = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_limit);
// }

// void mEnergy::CmndMaxPowerHold(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power_limit_hold = (1 == XdrvMailbox.payload) ? MAX_POWER_HOLD : XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_limit_hold);
// }

// void mEnergy::CmndMaxPowerWindow(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power_limit_window = (1 == XdrvMailbox.payload) ? MAX_POWER_WINDOW : XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_limit_window);
// }

// void mEnergy::CmndSafePower(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power_safe_limit = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_safe_limit);
// }

// void mEnergy::CmndSafePowerHold(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_power_safe_limit_hold = (1 == XdrvMailbox.payload) ? SAFE_POWER_HOLD : XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_safe_limit_hold);
// }

// void mEnergy::CmndSafePowerWindow(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 1440)) {
//   //   Settings.energy_max_power_safe_limit_window = (1 == XdrvMailbox.payload) ? SAFE_POWER_WINDOW : XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_power_safe_limit_window);
// }

// void mEnergy::CmndMaxEnergy(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//   //   Settings.energy_max_energy = XdrvMailbox.payload;
//   //   Energy.max_energy_state  = 3;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_energy);
// }

// void mEnergy::CmndMaxEnergyStart(void)
// {
//   // if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 24)) {
//   //   Settings.energy_max_energy_start = XdrvMailbox.payload;
//   // }
//   // ResponseCmndNumber(Settings.energy_max_energy_start);
// }
// // #endif  // USE_ENERGY_POWER_LIMIT
// // #endif  // USE_ENERGY_MARGIN_DETECTION







#endif

