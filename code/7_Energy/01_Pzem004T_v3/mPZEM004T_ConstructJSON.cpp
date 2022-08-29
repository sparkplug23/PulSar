#include "mPZEM004T.h"

#ifdef USE_MODULE_ENERGY_PZEM004T_V3





uint8_t mEnergyPZEM004T::ConstructJSON_Settings(uint8_t json_method){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.found);
  return JBI->End();

}

uint8_t mEnergyPZEM004T::ConstructJSON_Sensor(uint8_t json_method){

  char buffer[40];

  // AddLog(LOG_LEVEL_TEST, PSTR( DEBUG_INSERT_PAGE_BREAK "mEnergyInterface::ConstructJSON_Sensor %d"), pCONT_iEnergy->Energy.phase_count);
  JBI->Start();
  for(
    int ii=0;
    ii<pCONT_iEnergy->Energy.phase_count;
    // ii<2;
    ii++
  ){
    JsonBuilderI->Level_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), ii, buffer, sizeof(buffer)));
      JBI->Add(D_JSON_VOLTAGE,      data_modbus[ii].voltage);
      JBI->Add(D_JSON_CURRENT,      data_modbus[ii].current);
      JBI->Add(D_JSON_ACTIVE_POWER, data_modbus[ii].active_power);
      // JBI->Add(D_JSON_FREQUENCY,    data_modbus[ii].frequency);
      // JBI->Add(D_JSON_POWER_FACTOR, data_modbus[ii].power_factor);
      // JBI->Add(D_JSON_ENERGY,       data_modbus[ii].energy);
    JBI->Level_End();
  }

  // float error_rate_perc = (float)(stats.success_reads+stats.timeout_reads)/(float);
  float errors = stats.timeout_reads;
  float total = stats.success_reads;//+stats.timeout_reads;
  float error_rate_perc = (errors/total)*100;

  // JBI->Level_Start("stats");
  //   JBI->Add("success", stats.success_reads);
  //   JBI->Add("timeout", stats.timeout_reads);
  //   JBI->Add("error_rate_perc", error_rate_perc);
  //   JBI->Add("sample_time", stats.sample_time);
  //   JBI->Add("start_time", stats.start_time);
  //   JBI->Add("end_time", stats.end_time);
  // JBI->Level_End();

  return JBI->End();

}



#endif // USE_MODULE_ENERGY_PZEM004T_V3
