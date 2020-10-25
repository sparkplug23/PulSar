#include "mHeating.h"


#ifdef USE_MODULE_CUSTOM_HEATING

void mHeating::pre_init(void){

  init_success = true; // Begins true, anything bad sets to false

}

void mHeating::init(void){

  for(uint8_t device_id=0;device_id<4;device_id++){ SetHeatingRelay(device_id,RELAY_STATE_OFF); }

  tSavedHeatingTimer = millis();
  tSavedHeatingTemps = millis();

  for(int device_id=0;device_id<HEATING_DEVICE_TIMERS_MAX;device_id++){
    program_timers[device_id].time_minutes_on = -1;
    program_timers[device_id].time_minutes_on_start = program_timers[device_id].time_minutes_on;
    program_timers[device_id].ischanged=false;
  }

  for(int device_id=0;device_id<HEATING_DEVICE_TIMERS_MAX;device_id++){
    program_temps[device_id].time_running.on=-1;
    program_temps[device_id].time_maintaining.limit = -1;
  }

  // test fill
  memset(&program_temps[DEVICE_US_ID].status.data.ctr,0,sizeof(program_temps[DEVICE_US_ID].status.data.ctr));
  program_temps[DEVICE_US_ID].status.data.len = 0;
  memset(&program_temps[DEVICE_DS_ID].status.data.ctr,0,sizeof(program_temps[DEVICE_DS_ID].status.data.ctr));
  program_temps[DEVICE_DS_ID].status.data.len = 0;
  memset(&program_temps[DEVICE_IH_ID].status.data.ctr,0,sizeof(program_temps[DEVICE_IH_ID].status.data.ctr));
  program_temps[DEVICE_IH_ID].status.data.len = 0;
  memset(&program_temps[DEVICE_WB_ID].status.data.ctr,0,sizeof(program_temps[DEVICE_WB_ID].status.data.ctr));
  program_temps[DEVICE_WB_ID].status.data.len = 0;

  program_temps[DEVICE_US_ID].temp.desired = 20;
  program_temps[DEVICE_DS_ID].temp.desired = 21;
  program_temps[DEVICE_IH_ID].temp.desired = 40;
  program_temps[DEVICE_WB_ID].temp.desired = 50;


  program_temps[DEVICE_US_ID].temp.mapped_defaults[0] = 0;
  program_temps[DEVICE_US_ID].temp.mapped_defaults[1] = 10;
  program_temps[DEVICE_US_ID].temp.mapped_defaults[2] = 23;
  program_temps[DEVICE_US_ID].temp.mapped_defaults[3] = 25;
  
  program_temps[DEVICE_DS_ID].temp.mapped_defaults[0] = 0;
  program_temps[DEVICE_DS_ID].temp.mapped_defaults[1] = 20;
  program_temps[DEVICE_DS_ID].temp.mapped_defaults[2] = 23;
  program_temps[DEVICE_DS_ID].temp.mapped_defaults[3] = 25;
  
  program_temps[DEVICE_IH_ID].temp.mapped_defaults[0] = 0;
  program_temps[DEVICE_IH_ID].temp.mapped_defaults[1] = 30;
  program_temps[DEVICE_IH_ID].temp.mapped_defaults[2] = 40;
  program_temps[DEVICE_IH_ID].temp.mapped_defaults[3] = 50;
  
  program_temps[DEVICE_WB_ID].temp.mapped_defaults[0] = 0;
  program_temps[DEVICE_WB_ID].temp.mapped_defaults[1] = 30;
  program_temps[DEVICE_WB_ID].temp.mapped_defaults[2] = 40;
  program_temps[DEVICE_WB_ID].temp.mapped_defaults[3] = 50;


  program_timers[DEVICE_US_ID].mapped_defaults[0] = 0;
  program_timers[DEVICE_US_ID].mapped_defaults[1] = 60;
  program_timers[DEVICE_US_ID].mapped_defaults[2] = 90;
  program_timers[DEVICE_US_ID].mapped_defaults[3] = 120;
  
  program_timers[DEVICE_DS_ID].mapped_defaults[0] = 0;
  program_timers[DEVICE_DS_ID].mapped_defaults[1] = 60;
  program_timers[DEVICE_DS_ID].mapped_defaults[2] = 90;
  program_timers[DEVICE_DS_ID].mapped_defaults[3] = 120;
  
  program_timers[DEVICE_IH_ID].mapped_defaults[0] = 0;
  program_timers[DEVICE_IH_ID].mapped_defaults[1] = 30;
  program_timers[DEVICE_IH_ID].mapped_defaults[2] = 60;
  program_timers[DEVICE_IH_ID].mapped_defaults[3] = 120;
  
  program_timers[DEVICE_WB_ID].mapped_defaults[0] = 0;
  program_timers[DEVICE_WB_ID].mapped_defaults[1] = 30;
  program_timers[DEVICE_WB_ID].mapped_defaults[2] = 60;
  program_timers[DEVICE_WB_ID].mapped_defaults[3] = 120;

  pCONT_set->ClearAllDeviceName();

  init_relay_driver_parameters();
  init_dht22_sensor_parameters();
  init_db18_sensor_parameters();


  datetime_t dummy_t;

  dummy_t.Mday = 6; // set to today with __DATE__ later
  dummy_t.month = mTime::MONTH_AUGUST;
  dummy_t.year = 2019;
  dummy_t.hour = 23;
  dummy_t.minute = 30;
  dummy_t.second = 0;

#ifdef USE_SCHEDULED_HEATING

  for(int device_id=0;device_id<4;device_id++){
    // program_temps[device_id].schedule.ontime = &dummy_t;
    // program_temps[device_id].schedule.offtime = &dummy_t;
    // program_temps[device_id].schedule.untilontime = &dummy_t;
  }
  #endif

  failsafes.tSaved = millis();

  #ifdef USE_HEATING_TEMPS
  #ifdef USE_HEATING_PROFILE_ESTIMATION
    init_HeatingProfiles();
  #endif
  #endif

  // pCONT_set->AddDeviceName(D_HEATING_SENSOR_NAME_LONG_US,       D_MODULE_SENSORS_DB18S20_ID, DEVICELIST_WATER_US);
  // pCONT_set->AddDeviceName(D_HEATING_SENSOR_NAME_LONG_DS,       D_MODULE_SENSORS_DB18S20_ID, DEVICELIST_WATER_DS);
  // pCONT_set->AddDeviceName(D_HEATING_SENSOR_NAME_LONG_IH,       D_MODULE_SENSORS_DB18S20_ID, DEVICELIST_WATER_IH);
  // pCONT_set->AddDeviceName(D_HEATING_SENSOR_NAME_LONG_WB,       D_MODULE_SENSORS_DB18S20_ID, DEVICELIST_WATER_WB);
  // pCONT_set->AddDeviceName(D_HEATING_SENSOR_NAME_LONG_TT,       D_MODULE_SENSORS_DB18S20_ID, DEVICELIST_WATER_TT);
  // pCONT_set->AddDeviceName(D_HEATING_SENSOR_NAME_LONG_TM,       D_MODULE_SENSORS_DB18S20_ID, DEVICELIST_WATER_TM);
  // pCONT_set->AddDeviceName(D_HEATING_SENSOR_NAME_LONG_TB,       D_MODULE_SENSORS_DB18S20_ID, DEVICELIST_WATER_TB);
  // pCONT_set->AddDeviceName(D_HEATING_SENSOR_NAME_LONG_TO,       D_MODULE_SENSORS_DB18S20_ID, DEVICELIST_WATER_TO);


}//end function



void mHeating::init_relay_driver_parameters(){
  
  #ifdef TEST_DEVICE_NAME
  pCONT_set->AddDeviceName("One", D_MODULE_DRIVERS_RELAY_ID,   DEVICELIST_RELAY_US);
  pCONT_set->AddDeviceName("Two", D_MODULE_DRIVERS_RELAY_ID,   DEVICELIST_RELAY_DS);
  pCONT_set->AddDeviceName("Three", D_MODULE_DRIVERS_RELAY_ID,   DEVICELIST_RELAY_IH);
  pCONT_set->AddDeviceName("Four", D_MODULE_DRIVERS_RELAY_ID,   DEVICELIST_RELAY_WB);

  #else
  pCONT_set->AddDeviceName(D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG, D_MODULE_DRIVERS_RELAY_ID,   DEVICELIST_RELAY_US);
  pCONT_set->AddDeviceName(D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG, D_MODULE_DRIVERS_RELAY_ID,   DEVICELIST_RELAY_DS);
  pCONT_set->AddDeviceName(D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG, D_MODULE_DRIVERS_RELAY_ID,   DEVICELIST_RELAY_IH);
  pCONT_set->AddDeviceName(D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG, D_MODULE_DRIVERS_RELAY_ID,   DEVICELIST_RELAY_WB);
  #endif

}

void mHeating::init_db18_sensor_parameters(){
  
  //init db18
  uint8_t buffer_length = 0;
  // memset(pCONT_msdb18->name_buffer,0,sizeof(pCONT_msdb18->name_buffer));

  DeviceAddress* address_memory = nullptr;

  uint8_t sensor_found_id = 0;
  // search for sensor by address and assign its name in memory
  for(int address_id=0;address_id<8;address_id++){

    // Find the sensor name for found sensor eg WHAT IS IN ADDRESS 0
    for(int address_search_id=0;address_search_id<8;address_search_id++){
      // Load sensor to check
      switch(address_search_id){
        case ID_DB18_DS: address_memory = &thermo_addr_dsp; break;
        case ID_DB18_US: address_memory = &thermo_addr_usp; break;
        case ID_DB18_WB: address_memory = &thermo_addr_wbp; break;
        case ID_DB18_IH: address_memory = &thermo_addr_ih; break;
        case ID_DB18_TB: address_memory = &thermo_addr_tank_bottom; break;
        case ID_DB18_TO: address_memory = &thermo_addr_tank_out; break;
        case ID_DB18_TT: address_memory = &thermo_addr_tank_top; break;
        case ID_DB18_TM: address_memory = &thermo_addr_tank_middle; break;
      }
      if(memcmp(pCONT_msdb18->sensor[address_id].address,address_memory,sizeof(DeviceAddress))==0){
        sensor_found_id = address_search_id;
        pCONT_msdb18->sensor[address_id].id = address_search_id; // save id against address
        break;
      }
    }// end search for sensor

  #ifdef TEST_DEVICE_NAME
  pCONT_set->AddDeviceName("One", D_MODULE_SENSORS_DB18S20_ID,   0);
  pCONT_set->AddDeviceName("Two", D_MODULE_SENSORS_DB18S20_ID,   1);
  pCONT_set->AddDeviceName("Three", D_MODULE_SENSORS_DB18S20_ID,  2);
  pCONT_set->AddDeviceName("Four", D_MODULE_SENSORS_DB18S20_ID,   3);
  pCONT_set->AddDeviceName("Five", D_MODULE_SENSORS_DB18S20_ID,   4);
  pCONT_set->AddDeviceName("Six", D_MODULE_SENSORS_DB18S20_ID,   5);
  pCONT_set->AddDeviceName("Seven", D_MODULE_SENSORS_DB18S20_ID,   6);
  pCONT_set->AddDeviceName("Eight", D_MODULE_SENSORS_DB18S20_ID,   7);

  #else
    switch(sensor_found_id){
      case ID_DB18_DS: pCONT_set->AddDeviceName(D_DB18_NAME_DOWNSTAIRS_PIPE,D_MODULE_SENSORS_DB18S20_ID,address_id); break;
      case ID_DB18_US: pCONT_set->AddDeviceName(D_DB18_NAME_UPSTAIRS_PIPE,D_MODULE_SENSORS_DB18S20_ID,address_id); break;
      case ID_DB18_WB: pCONT_set->AddDeviceName(D_DB18_NAME_BOILER_PIPE,D_MODULE_SENSORS_DB18S20_ID,address_id); break;
      case ID_DB18_IH: pCONT_set->AddDeviceName(D_DB18_NAME_IMMERSION_HEATER,D_MODULE_SENSORS_DB18S20_ID,address_id); break;
      case ID_DB18_TB: pCONT_set->AddDeviceName(D_DB18_NAME_TANK_BOTTOM,D_MODULE_SENSORS_DB18S20_ID,address_id); break;
      case ID_DB18_TO: pCONT_set->AddDeviceName(D_DB18_NAME_TANK_OUT,D_MODULE_SENSORS_DB18S20_ID,address_id); break;
      case ID_DB18_TT: pCONT_set->AddDeviceName(D_DB18_NAME_TANK_TOP,D_MODULE_SENSORS_DB18S20_ID,address_id); break;
      case ID_DB18_TM: pCONT_set->AddDeviceName(D_DB18_NAME_TANK_MIDDLE,D_MODULE_SENSORS_DB18S20_ID,address_id); break;
    }
    #endif
  }

  // Measure and report every second
  pCONT_msdb18->settings.rate_measure_ms = 1000;
  
  //Reduce sensor lib from reporting to minimum
  pCONT_msdb18->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
  pCONT_msdb18->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
  pCONT_msdb18->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

}



void mHeating::init_dht22_sensor_parameters(){

  pCONT_set->AddDeviceName(D_DHT_NAME_DOWNSTAIRS, D_MODULE_SENSORS_DHT_ID,     ID_DHT_DS);
  pCONT_set->AddDeviceName(D_DHT_NAME_UPSTAIRS, D_MODULE_SENSORS_DHT_ID,     ID_DHT_US);

  // Measure and report every second
  pCONT_msdb18->settings.rate_measure_ms = 1000;

  //Reduce sensor lib from reporting to minimum
  pCONT->msdht->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
  pCONT->msdht->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
  pCONT->msdht->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

}

/**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
***********ANYTHING ABOVE, MUST BE FROM 2020***************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
**************************************************************   Huge rewrite
*/



/*******************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
********* RELAYS AND FAILSAFES ***************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************/


void mHeating::SetHeatingRelay(uint8_t device_id, uint8_t state){
  pCONT_mry->SetRelay(device_id,state); //index offset starts at 1
}

uint8_t mHeating::GetHeatingRelay(uint8_t device_id){
  DEBUG_LINE;
  return pCONT_mry->GetRelay(device_id);//index offset starts at 1
}

uint8_t mHeating::GetAnyHeatingRelay(){
  uint8_t oncount = 0;
  for(uint8_t i = 0;i < 4;i++){
    oncount += GetHeatingRelay(i);
  }
  return oncount;
}


//temps at max/above threshold for extended time (count++ if temp>threshold, reset when lower)
// check once a minute, if relay on OR temp above threshold, add time to counter
void mHeating::FunctionHandler_FailSafe(void){

  // if(mSupport::TimeReached(&failsafes.tSaved,60000)){

    // uint8_t fMessageToSend = false;
    // char tmpctr[100];

    // for(int device_id=0;device_id<4;device_id++){
    //   if((heating_device_relays[device_id].time_minutes_on>FAILSAFE_MINUTES_WARNING)&&(heating_device_relays[device_id].time_minutes_on<FAILSAFE_MINUTES_ERROR)){
    //     memset(tmpctr,0,sizeof(tmpctr));
    //     sprintf(tmpctr, "Warning, %s heating has been on for a long time of %d minutes",GetDeviceNameLongbyIDCtr(device_id),heating_device_relays[device_id].time_minutes_on);
    //     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_FAILSAFE D_WARNING "\"%s\""),tmpctr);
    //     fMessageToSend = true;
    //   }else if(heating_device_relays[device_id].time_minutes_on>FAILSAFE_MINUTES_ERROR){
    //     memset(tmpctr,0,sizeof(tmpctr));
    //     sprintf(tmpctr, "Error, %s heating has been on for too long. Turning off now.",GetDeviceNameLongbyIDCtr(device_id));
    //     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_FAILSAFE D_ERROR "\"%s\""),tmpctr);
    //     fMessageToSend = true;
    //   }

    //   // if(fMessageToSend){
    //   //   StaticJsonDocument<300> doc;
    //   //   JsonObject obj = doc.to<JsonObject>();
    //   //   obj[D_JSON_TYPE] = "voice+notification"; // broadcast (public/non bedroom speakers), notification(on phones) //SPECIAL, can contain both
    //   //   obj[D_JSON_MESSAGE] = tmpctr;
    //   //   data_buffer2.payload.len = measureJson(obj)+1;
    //   //   serializeJson(doc,data_buffer2.payload.ctr);
    //   //   pCONT->mqt->ppublish("status/alert",data_buffer2.payload.ctr,false);
    //   // }
    // }
  // } // end timer check

} //end fucntion


void mHeating::FunctionHandler_Relay_Status(){ DEBUG_LINE;

  uint8_t relay_state = 0;
  for(int device_id=0;device_id<DEVICE_ID_TOTAL;device_id++){

    switch(device_id){
      case DEVICE_DS_ID: relay_state = GetHeatingRelay(DEVICE_DS_ID); break;
      case DEVICE_US_ID: relay_state = GetHeatingRelay(DEVICE_US_ID); break;
      case DEVICE_WB_ID: relay_state = GetHeatingRelay(DEVICE_WB_ID); break;
      case DEVICE_IH_ID: relay_state = GetHeatingRelay(DEVICE_IH_ID); break;
    }

    switch(relay_state){
      case 1: heating_device_relays[device_id].time_minutes_on++;  break;
      case 0: heating_device_relays[device_id].time_minutes_on=-1; break;// RESET
    }

  }

} // END function


// This function detects if the traditional heating system has been activated.
// Methods I can use:
//  - furnace active sensor without my own relays on, which means I need to listen for data from oilfurnace
//  - if temp is above a limit (ie radiators when the pulse on and off) and if +ve ROC exceeds a value
int8_t mHeating::Tasker_PredictManualHeating(){

//if ROC > X with programs off = on

// /if(pCONT->mhs->watertemps.tank_top.ischanged){
//
//   // test for rising tank temps
//   if(watertemps.tank_top.roc1m.val>0.2){
//     pCONT->mso->MessagePrintln("watertemps.tank_top.roc1m.val>0.2");
//   }
//
// }

}


/*******************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
********* HEATING TIMERS ***************************************************************************************************************************************
*************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
***********************************************************************************************************************************************
************************************************************************************************************************************************/

void mHeating::FunctionHandler_Programs_Timers(void){

  char buffer[50];
  DEBUG_LINE;

  uint8_t log_level = LOG_LEVEL_INFO;//LOG_LEVEL_DEBUG;
  for(int device_id=0;device_id<DEVICE_ID_TOTAL;device_id++){
    if(program_timers[device_id].time_minutes_on){log_level = LOG_LEVEL_INFO;}
    AddLog_P(log_level, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "\"%s\" %d min"),GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),program_timers[device_id].time_minutes_on);
  }
  
  uint8_t state_tmp = 0;

  for(int device_id=0;device_id<DEVICE_ID_TOTAL;device_id++){

      if(program_timers[device_id].time_minutes_on == 0){
        program_timers[device_id].time_minutes_on = -1; //to disable it
        program_timers[device_id].time_minutes_on_start = program_timers[device_id].time_minutes_on;
        SetHeater(device_id, RELAY_STATE_OFF); //turn off
        program_timers[device_id].ischanged=true;
        state_tmp = ACTIVEP_OFF; // #1 error needs to be done here
        activeprograms[device_id].timers.state = 0;  
      }else 
      if(program_timers[device_id].time_minutes_on > 0){
        program_timers[device_id].time_minutes_on--;
        SetHeater(device_id, RELAY_STATE_ON); // only needs called once, that or add ifnoton inside this function turn on
        program_timers[device_id].ischanged=true;
        state_tmp = ACTIVEP_ON;
        activeprograms[device_id].timers.state = 1;        
      }else{ // <=1 not active
        activeprograms[device_id].timers.state = 0;  
      }

      // Update state change and run special functions if needed // makes no sense
      // if(activeprograms[device_id].timers.state != state_tmp){ // CAUSES ERROR #1 - duplicate on state
      //   activeprograms[device_id].timers.state = state_tmp;
      //   SendMQTTAlertOnChangeofState(device_id,state_tmp);//device,new state
      // }

    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING "activeprograms[%d].timers.state = %d"),device_id,
      activeprograms[device_id].timers.state);
    
  } // end for

  isanychanged_timers = 0;
  for(uint8_t device_id=0;device_id<4;device_id++){
    isanychanged_timers += program_timers[device_id].ischanged;
  }

} // end function



/*******************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
********* Heating TEMPS and Schedules ***************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************/

#ifdef USE_HEATING_TEMPS


void mHeating::FunctionHandler_Programs_Temps(void){

// DISABLE UNTIL REWRITE
  // Update times running 1/minute
  if(mSupport::TimeReached(&tSavedHeatingTemps2,60000)){
    SubTask_HeatingTemps_Ticker();
    fHeatingTempsChanged = true;
  }

  // Update temp variables 1/sec
  if(mSupport::TimeReached(&tSavedHeatingTemps,60000)||fForceHeatingTempsUpdate){
    SubTask_HeatingTemps();
  }

  // Update mqtt status info (5 secs)
  if(mSupport::TimeReached(&tSavedHeatingTemps3,60000)||fForceHeatingTempsUpdate){
    SubTask_HeatingTemps_StatusMessage();
  }


  // Use 1 ROC to deactive rapid heating decreases indicating using water
  /*if(CheckIfROCExceedsNegativeLimit(device_id)){

    // REPEAT OF BELOW CODE
    pCONT->mso->MessagePrint("CheckIfROCExceedsNegativeLimit");
    program_temps[device_id].time_running.on = -2; // -1 is just not active, -2 is a freeze out that lasts one hour unless overriden
    program_temps[device_id].time_maintaining.limit = -1;
    SetHeater(device_id,0);
    if(program_temps[device_id].schedule.mode_sch == SCHEDULED_ON){
      program_temps[device_id].schedule.mode_sch = SCHEDULED_OFF;
      program_temps[device_id].status.mode = TEMP_MODE_OFF;
      activeprograms[device_id].temps.state = ACTIVEP_OFF;
    }else{ //manual
      program_temps[device_id].schedule.mode_sch = SCHEDULED_OFF;
      program_temps[device_id].status.mode = TEMP_MODE_OFF;
      activeprograms[device_id].temps.state = ACTIVEP_OFF;
    }
    fForceHeatingTempsUpdate = true;

    char tmpctr[100];  memset(tmpctr,0,sizeof(tmpctr));
    sprintf(tmpctr, "Detected water usage, %s heating turned off",GetDeviceNameLongbyIDCtr(device_id));
    pCONT->mso->MessagePrintln(tmpctr);
    pCONT->mqt->pubsub->publish("heating/alert",tmpctr);
    pCONT->mqt->pubsub->publish("heating/voice/alert/direct",tmpctr);

  }*/

  fForceHeatingTempsUpdate = false; // RESET FLAG 

}


void mHeating::SubTask_HeatingTemps_Ticker(void){

  for(int device_id=0;device_id<4;device_id++){
    if(program_temps[device_id].time_running.on>=0){
      program_temps[device_id].time_running.on++;
    }
  }

}

uint8_t mHeating::CheckIfROCExceedsNegativeLimit(uint8_t id){

  // switch(id){
  //   case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
  //   case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
  //   case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
  //   case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
  //   case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
  //   case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
  //   case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
  //   case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
  // }

  // if(pCONT->mhs->watertemps.ptr->roc1m.val < -10){ // large change = water emptying therefore turn off
  //   return 1;
  // }
  return 0;

}// END function

// Change to switch cases later for easier reading
void mHeating::SubTask_HeatingTemps(void){

  // NEED TO HAVE CUTOFF WHEN ITS USED PRIOR TO HITTING TEMP

  float measured_temp=0;

  for(int device_id=0;device_id<HEATING_DEVICE_TIMERS_MAX;device_id++){ //needs converted to only run this seciton of code IF timer>-1 so on, but less than max

    //run on every loop
    switch(device_id){
      case DEVICE_DS_ID: measured_temp = pCONT->msdht->sensor[DHT_DOWNSTAIRS_ID].instant.temperature; break;
      case DEVICE_US_ID: measured_temp = pCONT->msdht->sensor[DHT_UPSTAIRS_ID].instant.temperature; break;
      case DEVICE_WB_ID: measured_temp = pCONT_msdb18->sensor[ID_DB18_WB].reading.val; break;
      case DEVICE_IH_ID: measured_temp = pCONT_msdb18->sensor[ID_DB18_IH].reading.val; break;
    }

    // Recaulate parameters
    program_temps[device_id].temp.current = measured_temp;
    program_temps[device_id].temp.error = measured_temp - program_temps[device_id].temp.desired;

    // Schedule temp settings
    switch(program_temps[device_id].schedule.mode_sch){
      case SCHEDULED_MANUAL_ON_ID:
        program_temps[device_id].status.mode = TEMP_MODE_HEATING_ID;
        activeprograms[device_id].temps.state = ACTIVEP_ON;
        fRunTemp = true;
        break;
      case SCHEDULED_ON_ID: //IE RUNNING
        program_temps[device_id].status.mode = TEMP_MODE_HEATING_ID;
        activeprograms[device_id].temps.state = ACTIVEP_ON;
        fRunTemp = true;
        //if on, check time limits
      break;
      case SCHEDULED_SET_ID:{

          //program_temps[device_id].status.mode = TEMP_MODE_SCHEDULED;

          activeprograms[device_id].temps.state = ACTIVEP_SCHEDULED;

          //uint8_t fTimeReached = pCONT->mt->CheckBetween_Week_DateTimes(&program_temps[device_id].schedule.ontime,&program_temps[device_id].schedule.offtime);

          uint8_t fTimeReached = pCONT->mt->CheckDateTimeWeekIsNow(&program_temps[device_id].schedule.ontime);

          program_temps[device_id].schedule.untilontime = pCONT->mt->GetDifferenceInDateTimes(&pCONT->mt->mtime,&program_temps[device_id].schedule.ontime);

          AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "CheckBetween_Week_DateTimes [%d]"),fTimeReached);

          // uint8_t hours, minutes, seconds;
          // pCONT->mt->DateTimeWeek2HHMMSS(&program_temps[device_id].schedule.untilontime,&hours,&minutes,&seconds);
          //
          // pCONT->mso->MessagePrint("hours> "); pCONT->mso->MessagePrintln(hours);
          // pCONT->mso->MessagePrint("minutes> "); pCONT->mso->MessagePrintln(minutes);
          // pCONT->mso->MessagePrint("seconds> "); pCONT->mso->MessagePrintln(seconds);

          // if is reached AND NOT run yet
          if(fTimeReached&&(!program_temps[device_id].schedule.fHasRun)){
            program_temps[device_id].schedule.fHasRun = 1;
            program_temps[device_id].schedule.mode_sch = SCHEDULED_ON_ID;
            program_temps[device_id].time_running.on = 0;
          }

        // if(program_temps[device_id].schedule.fRunning){}

        // check times and update time until it will run (use this as seconds count check?)
        fRunTemp = false;
      }
      break;
      case SCHEDULED_OFF_ID: // off, no temp heating
        activeprograms[device_id].temps.state = ACTIVEP_OFF;
        fRunTemp = false;
      break;
    }


    // Running section of code
    if(fRunTemp){
 
     // program_temps[device_id].mqtt.fForceUpdate = 1;  //CAUSES ERROR
      //pCONT->mso->MessagePrint(program_temps[device_id].mqtt.fForceUpdate);

      // if timer is active
      if(program_temps[device_id].time_running.on >= 0){

          // Time checks - if we have hit max time on (INCLUDES maintain)
          if(program_temps[device_id].time_running.on > program_temps[device_id].time_running.limit){   
            AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "minutes_on [%d] > minutes_max [%d]"),program_temps[device_id].time_running.on,program_temps[device_id].time_running.limit);
            program_temps[device_id].time_running.on = -1; //deactviate
            program_temps[device_id].time_maintaining.limit = -1;
          }else{ //stay on
            SetHeater(device_id,1);
            program_temps[device_id].status.mode = TEMP_MODE_HEATING_ID;
            activeprograms[device_id].temps.state = ACTIVEP_ON;
          }

          // Temp checks
          if(measured_temp >= program_temps[device_id].temp.desired){   
            AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "Measured Temp [%f] > Set Temp [%f]"),measured_temp,program_temps[device_id].temp.desired);
            AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "Exceeded Set Point"));
      
            // Check should we maintain temp (> minus 1, less than max)
            if((program_temps[device_id].time_maintaining.on>=0)&&(program_temps[device_id].time_maintaining.on < program_temps[device_id].time_maintaining.limit)){
              program_temps[device_id].status.mode = TEMP_MODE_MAINTAINING_ID;
              activeprograms[device_id].temps.state = ACTIVEP_ON;
            }
            // Turn off
            else{
              
              AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "desired_count > desired_max"));
      
              program_temps[device_id].time_running.on = -2; // -1 is just not active, -2 is a freeze out that lasts one hour unless overriden
              program_temps[device_id].time_maintaining.limit = -1;

              //record time it turned off ie record temp difference and how long we took to get there, I can use this later to estimate


              //add mode to repeat schedule

              //will make a basic estimate table ie time per degree

              SetHeater(device_id,0);

              if(program_temps[device_id].schedule.mode_sch == SCHEDULED_ON_ID){
                program_temps[device_id].schedule.mode_sch = SCHEDULED_OFF_ID;
                program_temps[device_id].status.mode = TEMP_MODE_OFF_ID;
                activeprograms[device_id].temps.state = ACTIVEP_OFF;
              }else{ //manual
                program_temps[device_id].schedule.mode_sch = SCHEDULED_OFF_ID;
                program_temps[device_id].status.mode = TEMP_MODE_OFF_ID;
                activeprograms[device_id].temps.state = ACTIVEP_OFF;
              }

              fForceHeatingTempsUpdate = true;

            }

      }//if temp check
      else{ 
        AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "Measured Temp [%f] < Set Temp [%f]"),measured_temp,program_temps[device_id].temp.desired);
      }
      // else{
      //   program_temps[device_id].status.mode = TEMP_MODE_OFF;
      //   activeprograms[device_id].temps.state = ACTIVEP_OFF;
      // }// end if timer active

    } //if time running on

  }//if(fRunTemp){

  }//end for loop

}//end temps fucntion




// MESSAGES
// Heating for 0/120 minutes
// Maintaining for 0/120 minutes
// Off : Unset
// Waiting: Set for 07:00 (T-03:45)
void mHeating::SubTask_HeatingTemps_StatusMessage(){

int16_t timeon;
char tmp[10];
char namectr[2];
char time_ctr[50];
uint8_t ischanged = 0;
float valf,val;
int vali;
int i=0;

  char tmpctr[50];  memset(tmpctr,0,sizeof(tmpctr));

  for(int device_id=0;device_id<4;device_id++){

    // clear message
    memset(&program_temps[device_id].status.data,0,sizeof(program_temps[device_id].status.data));

    switch(program_temps[device_id].status.mode){
      default:
        memcpy(&program_temps[device_id].status.data.ctr[0],"Error:Unknown",sizeof("Error:Unknown")-1);
      break;
      case TEMP_MODE_OFF_ID:
        memcpy(&program_temps[device_id].status.data.ctr[0],"Off",sizeof("Off")-1); program_temps[device_id].status.data.len += sizeof("Off")-1;
      break;
      case TEMP_MODE_HEATING_ID: // Heating for 0/120 minutes
        memset(time_ctr,'\0',sizeof(time_ctr));
        sprintf(time_ctr, "Heating for %d/%d minutes",(program_temps[device_id].time_running.on<1?0:program_temps[device_id].time_running.on),(program_temps[device_id].time_running.limit));
        memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
        program_temps[device_id].status.data.len += strlen(time_ctr);
      break;
      case TEMP_MODE_MAINTAINING_ID:
        memset(time_ctr,'\0',sizeof(time_ctr));
        sprintf(time_ctr, "Maintaining for %d/%d minutes",(program_temps[device_id].time_maintaining.on<1?0:program_temps[device_id].time_maintaining.on),(program_temps[device_id].time_maintaining.limit));
        memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
        program_temps[device_id].status.data.len += strlen(time_ctr);
      break;
      case TEMP_MODE_SCHEDULED_ID: // Waiting: Set for 07:00 (T-03:45)
        memset(time_ctr,'\0',sizeof(time_ctr));
        sprintf(time_ctr, "Set for %02d:%02d (in %02d:%02d)",
        program_temps[device_id].schedule.ontime.hour,program_temps[device_id].schedule.ontime.minute,
        program_temps[device_id].schedule.untilontime.hour,program_temps[device_id].schedule.untilontime.minute,program_temps[device_id].schedule.untilontime.second);
        memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
        program_temps[device_id].status.data.len += strlen(time_ctr);
      break;
      case TEMP_MODE_SPLASH_RUN_TIME_ID:
        // Scheduled to run in xx hours and yy minutes (xx:xx:xx/yy:yy:yy)
        if(abs(millis()-program_temps[device_id].status.tSplashTime)<10000){//program_temps[device_id].status.msSplashtime){
          memset(time_ctr,'\0',sizeof(time_ctr));
          sprintf(time_ctr, "Running in %d hours and %d minutes (%02d:%02d:%02d/%02d:%02d:%02d)",//abs(millis()-program_temps[device_id].status.tSplashTime),
            program_temps[device_id].schedule.untilontime.hour,program_temps[device_id].schedule.untilontime.minute,
            program_temps[device_id].schedule.ontime.hour,program_temps[device_id].schedule.ontime.minute,program_temps[device_id].schedule.ontime.second,
            program_temps[device_id].schedule.offtime.hour,program_temps[device_id].schedule.offtime.minute,program_temps[device_id].schedule.offtime.second);
          memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
          program_temps[device_id].status.data.len += strlen(time_ctr);
         }else{
           program_temps[device_id].status.mode = program_temps[device_id].status.mode_next;
         }
      break;
    }//END switch
  }//END for
}// END function

#ifdef USE_HEATING_PROFILE_ESTIMATION
    
// 6.25-25
// 6.55 -49.3
// 24.3 in 30 minutes
// 24.4/30 = deg/min = 0.8133
// 30/24.4 = minutes per degree
//float temp_step = 1.0/100.0; //degree range 0-1
void mHeating::init_HeatingProfiles(){

  for(int device_id=0;device_id<4;device_id++){

    switch(device_id){
      default:
      case DEVICE_DS_ID:
      case DEVICE_US_ID:{
        heating_profiles[device_id].temperature_max = 25;
        heating_profiles[device_id].temperature_min = 15;
        heating_profiles[device_id].temperature_step = (heating_profiles[device_id].temperature_max-heating_profiles[device_id].temperature_min)/100.0; //degree range 0-1
        for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){ //index = percentage
          heating_profiles[device_id].temperature[index] = mSupport::mapfloat(index,0,100,heating_profiles[device_id].temperature_min,heating_profiles[device_id].temperature_max);
          heating_profiles[device_id].duration_secs[index] = (60*30)/10; // seconds
        }
      }
      break;
      case DEVICE_IH_ID:{
        heating_profiles[device_id].temperature_max = 60;
        heating_profiles[device_id].temperature_min = 15;
        heating_profiles[device_id].temperature_step = (heating_profiles[device_id].temperature_max-heating_profiles[device_id].temperature_min)/100.0; //degree range 0-1
        for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){ //index = percentage
          heating_profiles[device_id].temperature[index] = mSupport::mapfloat(index,0,100,heating_profiles[device_id].temperature_min,heating_profiles[device_id].temperature_max);
          heating_profiles[device_id].duration_secs[index] = 214/2; // seconds
        }
      }
      break;
      case DEVICE_WB_ID:{
        heating_profiles[device_id].temperature_max = 60;
        heating_profiles[device_id].temperature_min = 15;
        heating_profiles[device_id].temperature_step = (heating_profiles[device_id].temperature_max-heating_profiles[device_id].temperature_min)/100.0; //degree range 0-1
        for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){ //index = percentage
          heating_profiles[device_id].temperature[index] = mSupport::mapfloat(index,0,100,heating_profiles[device_id].temperature_min,heating_profiles[device_id].temperature_max);
          heating_profiles[device_id].duration_secs[index] = 70;//140; // seconds by 1 degree then convert to new range
        }
      }
      break;
    }
  }
}

void mHeating::FunctionHandler_Heating_Profiles(){

 // if(abs(millis()-tSavedHeatingProfiles)>60000){tSavedHeatingProfiles = millis();

    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROFILES));
            
    //MQQTSendHeatingProfile_Raw_IfChanged();

    //GetHeatingProfilesTimeSeconds(DEVICE_IH,random(20,30)*1.12,40);

  //}

}

uint32_t mHeating::GetHeatingProfilesTimeMinutes(uint8_t device_id, float temp_now, float temp_target){
  uint32_t seconds = GetHeatingProfilesTimeSeconds(device_id, temp_now, temp_target);
  uint32_t minutes = seconds/60;
  return minutes;
}

uint32_t mHeating::GetHeatingProfilesTimeSeconds(uint8_t device_id, float temp_now, float temp_target){

uint32_t seconds=0;
uint8_t index_now=20, index_target=30;

/*

//find nerest temp to now (get index)
float difference[HEATINGPROFILE_RESOLUTION];

// temp_now = 50;
// for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
//   heating_profiles[device_id].temperature[index]=index;
// }

// Get difference
for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
  difference[index] = fabs(temp_now - heating_profiles[device_id].temperature[index]);
  //Serial.print(fabs(temp_now - heating_profiles[device_id].temperature[index])); Serial.print("_");
  //Serial.print(difference[index]); Serial.println("_");
}

//Get smallest difference ie closest
float lowest_difference_val,lowest_difference_temp_val;// = min()
uint8_t lowest_difference_index;

lowest_difference_val = difference[0];// record first value
for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
  if(difference[index]<lowest_difference_val){
    lowest_difference_val = difference[index];
    lowest_difference_temp_val = heating_profiles[device_id].temperature[index];
    lowest_difference_index = index;
    // pCONT->mso->MessagePrint("lowest_difference_val<difference[index]> ");
    // pCONT->mso->MessagePrint(lowest_difference_val);
    // pCONT->mso->MessagePrint("lowest_difference_temp_val> ");
    // pCONT->mso->MessagePrint(lowest_difference_temp_val);
    // pCONT->mso->MessagePrint(" lowest_difference_index> ");
    // pCONT->mso->MessagePrintln(lowest_difference_index);
  }
}



// pCONT->mso->MessagePrint("[HEAT] Searching for [");
// pCONT->mso->MessagePrint(temp_now);
// pCONT->mso->MessagePrint("]");
//
// pCONT->mso->MessagePrint(" found [");
// pCONT->mso->MessagePrint(heating_profiles[device_id].temperature[lowest_difference_index]);
// pCONT->mso->MessagePrint("] at index [");
// pCONT->mso->MessagePrint(lowest_difference_index);
// pCONT->mso->MessagePrintln("]");

index_now = lowest_difference_index;

//find nereast temp to target (get index)

// Get difference
for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
  difference[index] = fabs(temp_target - heating_profiles[device_id].temperature[index]);
  //Serial.print(fabs(temp_now - heating_profiles[device_id].temperature[index])); Serial.print("_");
  //Serial.print(difference[index]); Serial.println("_");
}

//Get smallest difference ie closest
// float lowest_difference_val,lowest_difference_temp_val;// = min()
// uint8_t lowest_difference_index;

lowest_difference_val = difference[0];// record first value
for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
  if(difference[index]<lowest_difference_val){
    lowest_difference_val = difference[index];
    lowest_difference_temp_val = heating_profiles[device_id].temperature[index];
    lowest_difference_index = index;
    // pCONT->mso->MessagePrint("lowest_difference_val<difference[index]> ");
    // pCONT->mso->MessagePrint(lowest_difference_val);
    // pCONT->mso->MessagePrint("lowest_difference_temp_val> ");
    // pCONT->mso->MessagePrint(lowest_difference_temp_val);
    // pCONT->mso->MessagePrint(" lowest_difference_index> ");
    // pCONT->mso->MessagePrintln(lowest_difference_index);
  }
}



// pCONT->mso->MessagePrint("[HEAT] Searching for [");
// pCONT->mso->MessagePrint(temp_target);
// pCONT->mso->MessagePrint("]");
//
// pCONT->mso->MessagePrint(" found [");
// pCONT->mso->MessagePrint(heating_profiles[device_id].temperature[lowest_difference_index]);
// pCONT->mso->MessagePrint("] at index [");
// pCONT->mso->MessagePrint(lowest_difference_index);
// pCONT->mso->MessagePrintln("]");

index_target = lowest_difference_index;

//sum seconds
for(int ii=index_now;index_now<index_target;index_now++){
  seconds += heating_profiles[device_id].duration_secs[ii];
  //Serial.print("secs> ");
  //Serial.println(seconds);
}

  // Possibly adding heating start time too? so curves are assuming heating is on

  datetime_t untiltime = pCONT->mt->GetDifferenceInDateTimes(&pCONT->mt->mtime,&heating_device_relays[device_id].ontime);
  uint32_t secondson = pCONT->mt->GetSecondsOfDayFromDateTime(&untiltime);

  int cold_start_seconds = 0;
  float sensor_temp = 0;
  // Add time on if system is only starting
  if(secondson<(60*10)){




    switch(device_id){   /// WRONG INDEXING -- NEED TO USE FUNCTION THAT RETURNS TEMP CORRECTLY FOR ID, NOT BASED ON INDEX, OR RETURNS POINTER TO THE TEMP INFO CONTAINED WITHIN IT
      case ID_DB18_DS: sensor_temp = pCONT_msdb18->sensor[ID_DB18_DS].reading.val; break;
      case ID_DB18_US: sensor_temp = pCONT_msdb18->sensor[ID_DB18_US].reading.val; break;
      case ID_DB18_WB: sensor_temp = pCONT_msdb18->sensor[ID_DB18_WB].reading.val; break;
      case ID_DB18_IH: sensor_temp = pCONT_msdb18->sensor[ID_DB18_IH].reading.val; break;
    }

    // Colder = more time added
    cold_start_seconds = map(sensor_temp,15,50,(60*10),1);
    cold_start_seconds = constrain(cold_start_seconds,1,(60*10));

    seconds += cold_start_seconds;// 10 minutes needs to go by relay on timer (needs added!)
  }
*/
return seconds;
}








void mHeating::ConstructJSON_HeatingProfile_Raw(uint8_t device_id){

  // StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
  // JsonObject obj = doc.to<JsonObject>();

  // obj[D_JSON_SIZE] = HEATINGPROFILE_RESOLUTION;
  
  // JsonObject temp = obj.createNestedObject(D_JSON_TEMP);
  //   temp[D_JSON_MAX] = mSupport::roundfloat(heating_profiles[device_id].temperature_max,1);
  //   temp[D_JSON_MIN] = mSupport::roundfloat(heating_profiles[device_id].temperature_min,1);;
  //   temp[D_JSON_STEP] = mSupport::roundfloat(heating_profiles[device_id].temperature_step,1);;
  //   JsonArray temparr = temp.createNestedArray(D_JSON_VALUES);
  //   for(int i=0;i<HEATINGPROFILE_RESOLUTION;i++){
  //     temparr.add(mSupport::roundfloat(heating_profiles[device_id].temperature[i],1));
  //   }

  // JsonArray dursarr = obj.createNestedArray(D_JSON_DURATION);
  // for(int i=0;i<HEATINGPROFILE_RESOLUTION;i++){
  //   dursarr.add(mSupport::roundfloat(heating_profiles[device_id].duration_secs[i],1));
  // }

  // data_buffer2.payload.len = measureJson(obj)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);

  // return 0;

}
#endif

#endif //#ifdef USE_HEATING_PROFILE_ESTIMATION
    









/*******************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
********* HEating TEMPS and Schedules ***************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************/










#ifdef USE_MQTT

// schedule.run_time:hh:mm
// schedule.run_days:M-W-FSS
// schedule.mode
// temp.set
// temp.max_time
// new time values that make sense

/*********************************************************************** JSON Payload Constructors *********************************************************************************************/

uint8_t mHeating::ConstructJSON_ProgramTimers(uint8_t json_level){

  memset(&data_buffer2,0,sizeof(data_buffer2));
  uint8_t ischanged = 0;
  uint8_t fSendAll = false;
  char buffer[50];
  
  if(json_level == JSON_LEVEL_IFCHANGED){
    fSendAll = false;
  }else
  if(json_level > JSON_LEVEL_IFCHANGED){
    fSendAll = true;
  }
  
  for(int ii=0;ii<4;ii++){

    if(ischanged = program_timers[ii].ischanged){
      program_timers[ii].ischanged = false;
    }

    if(ischanged||fSendAll){
      if(!data_buffer2.payload.len){
        pCONT_sup->WriteBuffer_P(data_buffer2.payload.ctr,&data_buffer2.payload.len,PSTR("{"));
      }else{
        pCONT_sup->WriteBuffer_P(data_buffer2.payload.ctr,&data_buffer2.payload.len,PSTR(","));
      }
      
      pCONT_sup->WriteBuffer_P(data_buffer2.payload.ctr,&data_buffer2.payload.len,
        PSTR(
          "\"%s\":{"
            "\"" D_JSON_TIME_ON    "\":%d"
          "}"
        ), 
        GetDeviceNamebyIDCtr(ii,buffer, sizeof(buffer)),
        program_timers[ii].time_minutes_on==-1?0:program_timers[ii].time_minutes_on
      );
    } //END if

  } //END for

  if(data_buffer2.payload.len){
    pCONT_sup->WriteBuffer_P(data_buffer2.payload.ctr,&data_buffer2.payload.len,PSTR("}"));
  }

  return data_buffer2.payload.len>3?1:0;
  
}

//Update: when times changed (set forced), or 60s elapsed
//new method, diving up Constructs if they are too big, return if all data was sent
uint8_t mHeating::ConstructJSON_ProgramTemps(uint8_t json_level){ //MQQTSendTimers
// return false;
  char buffer[50];
  JsonBuilderI->Start();
  for(int device=0;device<4;device++){

    // if((program_temps[device].mqtt.fForceUpdate)||(json_level==JSON_LEVEL_IFCHANGED)){program_temps[device].mqtt.fForceUpdate = false;

      JsonBuilderI->Level_Start_P(GetDeviceNamebyIDCtr(device, buffer, sizeof(buffer))); 
        JsonBuilderI->Level_Start_P(D_JSON_TEMPERATURE);   
          JsonBuilderI->Add(D_JSON_CURRENT, program_temps[device].temp.current);
          JsonBuilderI->Add(D_JSON_DESIRED, program_temps[device].temp.desired);
          JsonBuilderI->Add(D_JSON_ERROR, program_temps[device].temp.error);
        JsonBuilderI->Level_End();
        JsonBuilderI->Level_Start_P(D_JSON_STATUS);   
          JsonBuilderI->Add(D_JSON_MODE, GetTempModeByDeviceIDCtr(device, buffer, sizeof(buffer)));
          JsonBuilderI->Add(D_JSON_DATA, program_temps[device].status.data.ctr);
        JsonBuilderI->Level_End();

    //       JsonObject maintainingobj = deviceobj.createNestedObject(D_JSON_TIME_MAINTAINING);
    //         maintainingobj[D_JSON_ONTIME] = program_temps[device].time_maintaining.on;
    //         maintainingobj[D_JSON_LIMIT] = program_temps[device].time_maintaining.limit;

    //       JsonObject timerunningobj = deviceobj.createNestedObject(D_JSON_TIME_RUNNING);
    //         timerunningobj[D_JSON_ONTIME] = program_temps[device].time_maintaining.on;
    //         timerunningobj[D_JSON_LIMIT] = program_temps[device].time_maintaining.limit;

    //       #ifdef USE_HEATING_TEMPS
    //       // JsonObject time_to_heatobj = deviceobj.createNestedObject(D_JSON_TIME_TO_HEAT);
    //       //   time_to_heatobj[D_JSON_SECONDS] = GetHeatingProfilesTimeSeconds(device,program_temps[device].temp.current,program_temps[device].temp.desired);
    //       //   time_to_heatobj[D_JSON_MINUTES] = GetHeatingProfilesTimeMinutes(device,program_temps[device].temp.current,program_temps[device].temp.desired);
    //     #endif

    //     JsonObject scheduleobj = deviceobj.createNestedObject(D_JSON_SCHEDULE);
    //       scheduleobj[D_JSON_MODE] = GetScheduleNameCtrbyID(program_temps[device].schedule.mode_sch);
    //       scheduleobj[D_JSON_ISRUNNING] = program_temps[device].schedule.fRunning;

    //       // char time_ctr[20]; memset(time_ctr,'\0',sizeof(time_ctr));
    //       // sprintf(time_ctr, "%02d:%02d:%02d",(int)program_temps[device].schedule.ontime.hour,(int)program_temps[device].schedule.ontime.minute,(int)program_temps[device].schedule.ontime.second);
    //       // scheduleobj[D_JSON_ONTIME] = time_ctr;//pCONT->mt->getFormattedTime();
          
    //       // char time_ctr2[20];
    //       // memset(time_ctr2,'\0',sizeof(time_ctr2));
    //       // sprintf(time_ctr2, "%02d:%02d:%02d",(int)program_temps[device].schedule.offtime.hour,(int)program_temps[device].schedule.offtime.minute,(int)program_temps[device].schedule.offtime.second);
    //       // scheduleobj[D_JSON_OFFTIME] = time_ctr2;//pCONT->mt->getFormattedTime();

    //     //data_buffer2.payload.json_pairs++;
    JsonBuilderI->Level_End();
    // }
  }
  return JsonBuilderI->End();

}



// relays{ds,us},timers{ds,us},temps{ds,us} in simple 1 or 0
uint8_t mHeating::ConstructJSON_ProgramActive(uint8_t json_level){
  JsonBuilderI->Start();
    JsonBuilderI->Level_Start_P(D_JSON_RELAY);   
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_US, GetHeatingRelay(DEVICE_US_ID));
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_DS, GetHeatingRelay(DEVICE_DS_ID));
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_IH, GetHeatingRelay(DEVICE_IH_ID));
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_WB, GetHeatingRelay(DEVICE_WB_ID));
      JsonBuilderI->Add(D_JSON_ANY,                     GetAnyHeatingRelay());      
    JsonBuilderI->Level_End();
    JsonBuilderI->Level_Start_P(D_JSON_TIMER);  
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_US, activeprograms[DEVICE_US_ID].timers.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_DS, activeprograms[DEVICE_DS_ID].timers.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_IH, activeprograms[DEVICE_IH_ID].timers.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_WB, activeprograms[DEVICE_WB_ID].timers.state); 
    JsonBuilderI->Level_End();
    JsonBuilderI->Level_Start_P(D_JSON_TEMP);  
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_US, activeprograms[DEVICE_US_ID].temps.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_DS, activeprograms[DEVICE_DS_ID].temps.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_IH, activeprograms[DEVICE_IH_ID].temps.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_WB, activeprograms[DEVICE_WB_ID].temps.state); 
    JsonBuilderI->Level_End();
    JsonBuilderI->Level_Start_P(D_JSON_STATUS);  
      JsonBuilderI->Add(D_JSON_MESSAGE, pCONT_tel->hardwarestatus.ctr); 
      JsonBuilderI->Add(D_JSON_LENGTH, pCONT_tel->hardwarestatus.len); 
      JsonBuilderI->Add(D_JSON_IMPORTANCE, pCONT_tel->hardwarestatus.importance); 
    JsonBuilderI->Level_End();
  return JsonBuilderI->End();
}


uint8_t mHeating::ConstructJSON_HeatingRelays(uint8_t json_level){
  char buffer[50];
  JsonBuilderI->Start();
  for(int device_id=0;device_id<4;device_id++){
    JsonBuilderI->Level_Start_P(
      pCONT_set->GetDeviceName(D_MODULE_DRIVERS_RELAY_ID, device_id, buffer, sizeof(buffer)));
      JsonBuilderI->Add_FP(D_JSON_ONTIME, PSTR("\"%02d:%02d:%02d\""),  heating_device_relays[device_id].ontime.hour,  heating_device_relays[device_id].ontime.minute,  heating_device_relays[device_id].ontime.second);
      JsonBuilderI->Add_FP(D_JSON_OFFTIME, PSTR("\"%02d:%02d:%02d\""), heating_device_relays[device_id].offtime.hour, heating_device_relays[device_id].offtime.minute, heating_device_relays[device_id].offtime.second);
      JsonBuilderI->Add(D_JSON_TIME_ON,   heating_device_relays[device_id].time_minutes_on);
      JsonBuilderI->Add(D_JSON_ISCHANGED, heating_device_relays[device_id].ischanged);
    JsonBuilderI->Level_End();
  }
  return JsonBuilderI->End();
}



// Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
uint8_t mHeating::ConstructJSON_PipeTemps(uint8_t json_level){

  char buffer[50];
  
  JsonBuilderI->Start();
    for(int i=0;i<pCONT_msdb18->db18_sensors_active;i++){ 
      if(pCONT_sup->JsonLevelFlagCheck(JSON_LEVEL_IFCHANGED, json_level, pCONT_msdb18->sensor[i].reading.ischanged)){
        JsonBuilderI->Level_Start_P(pCONT_set->GetDeviceName(D_MODULE_SENSORS_DB18S20_ID, i, buffer, sizeof(buffer)));
          JsonBuilderI->Add(D_JSON_TEMPERATURE, pCONT_msdb18->sensor[i].reading.val);
          JsonBuilderI->Add(D_JSON_ISVALID, pCONT_msdb18->sensor[i].reading.isvalid);
          JsonBuilderI->Add(D_JSON_SECS, (int)abs(pCONT->mt->uptime.seconds_nonreset-pCONT_msdb18->sensor[i].reading.captureupsecs));
        JsonBuilderI->Level_End();
      } 
    }
  return JsonBuilderI->End();

}


// Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
uint8_t mHeating::ConstructJSON_PipeTempsROC10s(uint8_t json_level){

return 0;
  // StaticJsonDocument<700> doc;
  // JsonObject obj = doc.to<JsonObject>();
  // memset(&data_buffer2,0,sizeof(data_buffer2));

  // for(uint8_t id=0;id<8;id++){

  //   switch(id){
  //     case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
  //     case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
  //     case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
  //     case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
  //     case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
  //     case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
  //     case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
  //     case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
  //   }

  //   //if(pCONT->mhs->watertemps.ptr->roc10s.ischanged||fSendAllData||fSendSingleFunctionData){ //data_buffer2.payload.json_pairs++;
  //     JsonObject json1 = obj.createNestedObject(GetSensorNameByID(id));
  //       json1[D_JSON_TEMP] = mSupport::roundfloat(pCONT->mhs->watertemps.ptr->roc10s.val,1);
  //       json1[D_JSON_ISVALID] = (int)pCONT->mhs->watertemps.ptr->roc10s.isvalid;
  //       json1[D_JSON_SECS] = (int)abs(pCONT->mt->uptime.second-pCONT->mhs->watertemps.ptr->roc10s.captureupsecs);
  //   //}

  // }

  

  // data_buffer2.payload.len = measureJson(obj)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);
  // return data_buffer2.payload.len>3?1:0;

}



// Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
uint8_t mHeating::ConstructJSON_PipeTempsROC1m(uint8_t json_level){

return 0;
//   StaticJsonDocument<700> doc;
//   JsonObject obj = doc.to<JsonObject>();
//   memset(&data_buffer2,0,sizeof(data_buffer2));

//   for(uint8_t id=0;id<8;id++){

//     switch(id){
//       case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
//       case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
//       case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
//       case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
//       case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
//       case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
//       case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
//       case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
//     }

// //    pCONT->mhs->watertemps.ptr->roc1m.ischanged=1;
//     //if(pCONT->mhs->watertemps.ptr->roc1m.ischanged||fSendAllData||fSendSingleFunctionData){ //data_buffer2.payload.json_pairs++;
//       JsonObject json1 = obj.createNestedObject(GetSensorNameByID(id));
//       json1[D_JSON_TEMP] = mSupport::roundfloat(pCONT->mhs->watertemps.ptr->roc1m.val,1);
//       json1[D_JSON_ISVALID] = (int)pCONT->mhs->watertemps.ptr->roc1m.isvalid;
//       json1[D_JSON_SECS] = (int)abs(pCONT->mt->uptime.second-pCONT->mhs->watertemps.ptr->roc1m.captureupsecs);
//     //}

//   }

  

//   data_buffer2.payload.len = measureJson(obj)+1;
//   serializeJson(doc,data_buffer2.payload.ctr);
//   return data_buffer2.payload.len>3?1:0;

}



// Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
uint8_t mHeating::ConstructJSON_PipeTempsROC10m(uint8_t json_level){

  return 0;

  // StaticJsonDocument<700> doc;
  // JsonObject obj = doc.to<JsonObject>();
  // memset(&data_buffer2,0,sizeof(data_buffer2));

  // for(uint8_t id=0;id<8;id++){

  //   switch(id){
  //     case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
  //     case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
  //     case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
  //     case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
  //     case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
  //     case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
  //     case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
  //     case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
  //   }
    
  //   //if(pCONT->mhs->watertemps.ptr->roc10m.ischanged||fSendAllData||fSendSingleFunctionData){ //data_buffer2.payload.json_pairs++;
  //     JsonObject json1 = obj.createNestedObject(GetSensorNameByID(id));
  //     json1[D_JSON_TEMP] = mSupport::roundfloat(pCONT->mhs->watertemps.ptr->roc10m.val,1);
  //     json1[D_JSON_ISVALID] = (int)pCONT->mhs->watertemps.ptr->roc10m.isvalid;
  //     json1[D_JSON_SECS] = (int)abs(pCONT->mt->uptime.second-pCONT->mhs->watertemps.ptr->roc10m.captureupsecs);
  //  // }

  // }

  

  // data_buffer2.payload.len = measureJson(obj)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);
  // return data_buffer2.payload.len>3?1:0;

}



uint8_t mHeating::ConstructJSON_ClimateTemps(uint8_t json_level){

  memset(&data_buffer2,0,sizeof(data_buffer2));
  char namectr[3]; memset(&namectr,0,sizeof(namectr));
  uint8_t ischanged=false, isvalid = false;

  StaticJsonDocument<400> doc;
  JsonObject obj = doc.to<JsonObject>();

  for(int dht_id=0;dht_id<2;dht_id++){

    // switch(ii){
    //   case 0:
    //     ischanged = pCONT->msdht->sensor[dht_id].instant.ischanged;
    //     isvalid = pCONT->msdht->sensor[dht_id].instant.isvalid;
    //   break;
    //   case 1:
        ischanged = pCONT->msdht->sensor[dht_id].instant.ischanged;
        isvalid = pCONT->msdht->sensor[dht_id].instant.isvalid;
  //     break;
  //  }

  
   
   if(!isvalid){
     return 0; // send nothing
   }

    //  data_buffer2.payload.json_pairs++;

    //   switch(ii){
    //     case 0: memcpy(namectr,D_HEATING_SENSOR_NAME_SHORT_DS,2*sizeof(char)); pCONT->mhs->climate.ptr = &pCONT->mhs->climate.downstairs; break;
    //     case 1: memcpy(namectr,D_HEATING_SENSOR_NAME_SHORT_US,2*sizeof(char)); pCONT->mhs->climate.ptr = &pCONT->mhs->climate.upstairs; break;
    //   }
    char name_buffer_tmp[25];
    // pCONT_sup->GetTextIndexed_P(name_buffer_tmp, sizeof(name_buffer_tmp), dht_id, pCONT->msdht->name_buffer);

      pCONT_set->GetDeviceName(D_MODULE_SENSORS_DHT_ID, dht_id, name_buffer_tmp, sizeof(name_buffer_tmp));


      JsonObject json1 = obj.createNestedObject(name_buffer_tmp);
        // pCONT->msdht->sensor[dht_id].name_ptr);
      json1[D_JSON_TEMP] = pCONT->msdht->sensor[dht_id].instant.temperature;
      json1[D_JSON_HUM] = pCONT->msdht->sensor[dht_id].instant.humidity;


      json1[D_JSON_ISVALID] = (int)isvalid;
      json1[D_JSON_ISCHANGED] = (int)ischanged;
      // json1["iserrored"] = pCONT->mhs->climate.ptr->iserrored;
      // json1[D_JSON_SECS] = (int)abs(pCONT->mt->uptime.seconds_nonreset-pCONT->mhs->climate.ptr->raw.captureupsecs);
      // json1["heatindex"] = pCONT->mhs->climate.ptr->raw.heatIndex; // DONT KNOW WHAT THEY ARE
      // json1["dewpoint"] = pCONT->mhs->climate.ptr->raw.dewPoint;

    //}

  }

  data_buffer2.payload.len = measureJson(obj)+1;
  serializeJson(doc,data_buffer2.payload.ctr);
  return data_buffer2.payload.len>3?1:0;

}


uint8_t mHeating::ConstructJSON_ClimateTempsROC1m(uint8_t json_level){

// clear entire mqtt packet
memset(&data_buffer2,0,sizeof(data_buffer2));

  // uint8_t ischanged=false;
  // StaticJsonDocument<400> doc;
  // JsonObject obj = doc.to<JsonObject>();

  // for(int ii=0;ii<2;ii++){

  //   switch(ii){
  //     case 0: ischanged = pCONT->mhs->climate.downstairs.roc1m.ischanged; break;
  //     case 1: ischanged = pCONT->mhs->climate.upstairs.roc1m.ischanged; break;
  //   }

  //   //if(ischanged||fSendAllData||fSendSingleFunctionData){
  //     // data_buffer2.payload.json_pairs++;
  //     switch(ii){
  //       case 0: pCONT->mhs->climate.ptr = &pCONT->mhs->climate.downstairs; break;
  //       case 1: pCONT->mhs->climate.ptr = &pCONT->mhs->climate.upstairs; break;
  //     }
  //     JsonObject json1 = obj.createNestedObject(GetDeviceNamebyIDCtr(ii));
  //       json1[D_JSON_TEMP] = pCONT->mhs->climate.ptr->roc1m.temperature;
  //       json1[D_JSON_HUM] = pCONT->mhs->climate.ptr->roc1m.humidity;
  //   //}

  // }


  // data_buffer2.payload.len = measureJson(obj)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);
  // return data_buffer2.payload.len>3?1:0;
  return 0;

}

uint8_t mHeating::ConstructJSON_ClimateTempsROC10m(uint8_t json_level){

  // clear entire mqtt packet
  memset(&data_buffer2,0,sizeof(data_buffer2));

  // char namectr[3]; memset(&namectr,0,sizeof(namectr));
  // uint8_t ischanged=false;

  // StaticJsonDocument<400> doc;
  //   JsonObject obj = doc.to<JsonObject>();

  //  for(int ii=0;ii<2;ii++){

  //    switch(ii){
  //      case 0: ischanged = pCONT->mhs->climate.downstairs.roc10m.ischanged; break;
  //      case 1: ischanged = pCONT->mhs->climate.upstairs.roc10m.ischanged; break;
  //    }

  //   // ischanged=1;
  //  //  if(ischanged||fSendAllData||fSendSingleFunctionData){

  //     //  data_buffer2.payload.json_pairs++;

  //       switch(ii){
  //         case 0: memcpy(namectr,D_HEATING_SENSOR_NAME_SHORT_DS,2*sizeof(char)); pCONT->mhs->climate.ptr = &pCONT->mhs->climate.downstairs; break;
  //         case 1: memcpy(namectr,D_HEATING_SENSOR_NAME_SHORT_US,2*sizeof(char)); pCONT->mhs->climate.ptr = &pCONT->mhs->climate.upstairs; break;
  //       }

  //       JsonObject json1 = obj.createNestedObject(namectr);
  //       json1[D_JSON_TEMP] = pCONT->mhs->climate.ptr->roc10m.temperature;
  //       json1[D_JSON_HUM] = pCONT->mhs->climate.ptr->roc10m.humidity;

  //     }

  //  // }

     

  // data_buffer2.payload.len = measureJson(obj)+1;
  // serializeJson(doc,data_buffer2.payload.ctr);
  // return data_buffer2.payload.len>3?1:0;
  return 0;

}



uint8_t mHeating::ConstructJSON_Settings(uint8_t json_method){

    // memset(&data_buffer2,0,sizeof(data_buffer2));
    // StaticJsonDocument<400> doc;
    // JsonObject root = doc.to<JsonObject>();

    // root["json_teleperiod_level"] = pCONT_set->GetTelePeriodJsonLevelCtr();


    // data_buffer2.payload.len = measureJson(root)+1;
    // serializeJson(doc,data_buffer2.payload.ctr);

    return 0;//data_buffer2.payload.len>3?true:false;

}



#endif








// Keeps the highest importance flag
void mHeating::SetHighestImportance(uint8_t* importanceset, int8_t thisvalue){
  if(thisvalue > *importanceset){
    *importanceset = thisvalue;
  }
  //moved into telemetrym
}


// returns > 0 for events added
int8_t mHeating::SubContructCtr_HardwareStatus(){

  //ORDER BY LEAST TO MOST IMPORTANT SO HIGHEST FLAG IS SET LAST
  uint8_t fNotFirstItem = 0;

  // for(int device_id=0;device_id<4;device_id++){
  //   if(activeprograms[device_id].timers.state){
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%s timers %s",
  //       GetDeviceNamebyIDCtr(device_id),
  //       GetActiveProgramNameCtrbyID(activeprograms[device_id].timers.state));
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //     SetHighestImportance(&pCONT_tel->hardwarestatus.importance,activeprograms[device_id].timers.state); //med
  //   }
  //   if(fNotFirstItem){
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%c",'c');
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //   }
  // }

  // for(int device_id=0;device_id<4;device_id++){
  //   if(activeprograms[device_id].temps.state){ pCONT_tel->hardwarestatus.importance = 2;//med
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%s temps %s",
  //       GetDeviceNamebyIDCtr(device_id),
  //       GetActiveProgramNameCtrbyID(activeprograms[device_id].temps.state));
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //   }
  //   SetHighestImportance(&pCONT_tel->hardwarestatus.importance,activeprograms[device_id].temps.state); //med
  //   if(fNotFirstItem){
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%c",'c');
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //   }
  // }

  // for(int device_id=0;device_id<4;device_id++){
  //   if(activeprograms[device_id].relays.state){ pCONT_tel->hardwarestatus.importance = 2;//high
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%s relays %s",
  //       GetDeviceNamebyIDCtr(device_id),
  //       GetActiveProgramNameCtrbyID(activeprograms[device_id].relays.state));
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //   }
  //   SetHighestImportance(&pCONT_tel->hardwarestatus.importance,activeprograms[device_id].relays.state); //med
  //   if(fNotFirstItem){
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%c",'c');
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //   }
  // }

}

// returns > 0 for events added
int8_t mHeating::SubContructCtr_HardwareStatus_Long(){

  //ORDER BY LEAST TO MOST IMPORTANT SO HIGHEST FLAG IS SET LAST
  uint8_t fNotFirstItem = 0;

  // for(int device_id=0;device_id<4;device_id++){
  //   if(activeprograms[device_id].timers.state){
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%s Timer %s",
  //       GetDeviceNameLongbyIDCtr(device_id),
  //       GetActiveProgramNameCtrbyID(activeprograms[device_id].timers.state));
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //     SetHighestImportance(&pCONT_tel->hardwarestatus.importance,activeprograms[device_id].timers.state); //med
  //   }
  //   if(fNotFirstItem){
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%c",'c');
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //   }
  // }

  // for(int device_id=0;device_id<4;device_id++){
  //   if(activeprograms[device_id].temps.state){ pCONT_tel->hardwarestatus.importance = 2;//med
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%s Temp %s",
  //       GetDeviceNameLongbyIDCtr(device_id),
  //       GetActiveProgramNameCtrbyID(activeprograms[device_id].temps.state));
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //   }
  //   SetHighestImportance(&pCONT_tel->hardwarestatus.importance,activeprograms[device_id].temps.state); //med
  //   if(fNotFirstItem){
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%c",'c');
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //   }
  // }

  // for(int device_id=0;device_id<4;device_id++){
  //   if(activeprograms[device_id].relays.state){ pCONT_tel->hardwarestatus.importance = 2;//high
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%s relays %s",
  //       GetDeviceNameLongbyIDCtr(device_id),
  //       GetActiveProgramNameCtrbyID(activeprograms[device_id].relays.state));
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //   }
  //   SetHighestImportance(&pCONT_tel->hardwarestatus.importance,activeprograms[device_id].relays.state); //med
  //   if(fNotFirstItem){
  //     sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"%c",'c');
  //     pCONT_tel->hardwarestatus.len = strlen(pCONT_tel->hardwarestatus.ctr);
  //   }
  // }

  if(!pCONT_tel->hardwarestatus.len){
    pCONT_tel->hardwarestatus.len += sprintf(&pCONT_tel->hardwarestatus.ctr[pCONT_tel->hardwarestatus.len],"Online");
  }

} //end function



int mHeating::mapHeatingTempToHueColour(int val){
   int val2 = mSupport::mapfloat(val,15.0,50.0,180,0); //float mSupport::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
   if(val2<0){
     if(val2<-60){ //limit so we stay within purple and never looping as far as blue
       val2 = 300;
     }else{
       val2 += 360;
     }
   }
  return val2;
}

int mHeating::mapHeatingTempToBrightness(int temp){

  int brightness;

  if(temp<25){
    brightness = map(temp,15,25,1,10); //brightness propotional to temp; hotter = brighter
    brightness = constrain(brightness,9,10);
  }else{
    brightness = map(temp,25,50,10,100); //brightness propotional to temp; hotter = brighter
    brightness = constrain(brightness,10,100);
  }

  return brightness;

}




// Send all water sensors by [temp,huebytemp(0-360),brightnessbytemp(0-100)]
// ds:{temp:0,hue:0,bright:0},
// us:{temp:0,hue:0,bright:0},
// FLASHER to be send seperate when programs change state
uint8_t mHeating::ConstructSON_PipeTempsByColours(uint8_t json_level){

  // StaticJsonDocument<800> doc;
  // JsonObject obj = doc.to<JsonObject>();
  // memset(&data_buffer2,0,sizeof(data_buffer2));

  // uint8_t ischanged = false;

  // for(uint8_t device_id=0;device_id<8;device_id++){

  //   switch(device_id){
  //     case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
  //     case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
  //     case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
  //     case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
  //     case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
  //     case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
  //     case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
  //     case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
  //   }

  //   if(stored_new.temp[device_id] != pCONT->mhs->watertemps.ptr->raw.val){
  //     stored_new.temp[device_id] = pCONT->mhs->watertemps.ptr->raw.val;
  //     ischanged = 1;
  //   }
  //   #ifdef DEBUG_SENDALL
  //     ischanged=1; //ALL
  //   #endif
  //   if(ischanged){ ischanged = 0;
  //     //data_buffer2.payload.json_pairs++;
  //     JsonObject json1 = obj.createNestedObject(GetSensorNameByID(device_id));
  //     json1[D_JSON_TEMP] = pCONT->mhs->watertemps.ptr->raw.val;
  //     json1[D_JSON_HUE] = mapHeatingTempToHueColour(pCONT->mhs->watertemps.ptr->raw.val);
  //     json1[D_JSON_BRT] = mapHeatingTempToBrightness(pCONT->mhs->watertemps.ptr->raw.val);

  //     char tmpctr[10];  memset(tmpctr,0,sizeof(tmpctr));
  //     RgbColor c = HsbColor(mapHeatingTempToHueColour(pCONT->mhs->watertemps.ptr->raw.val)/360.0f,100/100.0f,100/100.0f);
  //     sprintf(tmpctr,"%02X%02X%02X",c.R,c.G,c.B);
  //     json1[D_JSON_RGB] = tmpctr;

  //     switch(device_id){
  //       case ID_DB18_DS: json1[D_JSON_FLASHER] = GetHeatingRelay(DEVICE_DS_ID); break;
  //       case ID_DB18_US: json1[D_JSON_FLASHER] = GetHeatingRelay(DEVICE_US_ID); break;
  //       case ID_DB18_WB: json1[D_JSON_FLASHER] = GetHeatingRelay(DEVICE_WB_ID); break;
  //       case ID_DB18_IH: json1[D_JSON_FLASHER] = GetHeatingRelay(DEVICE_IH_ID); break;
  //       default: break; //nothing
  //     }
  //   }

  // }

  // // if(data_buffer2.payload.json_pairs>0){
  //   data_buffer2.payload.len = measureJson(obj)+1;
  //   serializeJson(doc,data_buffer2.payload.ctr);
  //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATINGPANEL D_PAYLOAD " \"%s\""),data_buffer2.payload.ctr);
  // // }

  return 0;// data_buffer2.payload.len>3?1:0;

}


/*******************************************************************************************************************************************************
************************************************************************************************************************************************
********* Controls ***************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************/

// reads relay position for devices
int8_t mHeating::GetHeater(uint8_t device){
  switch(device){
    case DEVICE_DS_ID: return GetHeatingRelay(DEVICE_DS_ID);
    case DEVICE_US_ID: return GetHeatingRelay(DEVICE_US_ID);
    case DEVICE_WB_ID: return GetHeatingRelay(DEVICE_WB_ID);
    case DEVICE_IH_ID: return GetHeatingRelay(DEVICE_IH_ID);
    default: return -1;
  }
}

void mHeating::SetHeater(uint8_t device, uint8_t state){

  //AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "minutes_on [%d] > minutes_max [%d]"),program_temps[device_id].time_running.on,program_temps[device_id].time_running.limit);

  #ifdef ENABLE_MQTT_DEBUG_MESSAGES //debug_message/function 
  pCONT_mqtt->Send_Prefixed_P(PSTR("debug_message/setheater"),PSTR("device=%d,state=%d"),device,state);
  #endif


  //add ontime like relays          

  //add state check that only runs updates below if new state has been selected, else, return
  if(GetHeater(device)==state){
    heating_device_relays[device].ischanged = false;
    return;
  }else{
    heating_device_relays[device].ischanged = true;
    fForceHeatingTempsUpdate = true;
  }

  #ifdef ENABLE_RELAY_CONTROLS

    SetHeatingRelay(device,state);

    if(state==1){heating_device_relays[device].ontime = pCONT->mt->mtime;}
    if(state==0){heating_device_relays[device].offtime = pCONT->mt->mtime;}

  #else
    //Serial.println(F("SetHeater Disabled for debugging!"));
    //pCONT->mqt->pubsub->publish("heating/alert","Heating DISABLED for testing!");}
  #endif

} // end function























const char* mHeating::GetActiveProgramNameCtrbyID(uint8_t activeprogram_id, char* buffer){
  if(buffer == nullptr){ return 0;}
    // return (activeprogram_id == ACTIVEP_OFF ? "OFF" :
    //     (activeprogram_id == ACTIVEP_ON ? "ON" :
    //     (activeprogram_id == ACTIVEP_SCHEDULED ? "SCH" :
    //     "unk\0")));
    return 0;
}


const char* mHeating::GetDeviceNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buflen){
  if(buffer == nullptr){ return 0;}
  switch(device_id){ 
    case DEVICE_DS_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_DS_CTR); break;
    case DEVICE_US_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_US_CTR); break;
    case DEVICE_WB_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_WB_CTR); break;
    case DEVICE_IH_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_IH_CTR); break;
  }
  return buffer;
}
const char* mHeating::GetDeviceNameLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buflen){
  if(buffer == nullptr){ return 0;}
  switch(device_id){ 
    case DEVICE_DS_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_DS_CTR); break;
    case DEVICE_US_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_US_CTR); break;
    case DEVICE_WB_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_WB_CTR); break;
    case DEVICE_IH_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_IH_CTR); break;
  }
  return buffer;
}
int8_t mHeating::GetDeviceIDbyName(const char* c){
  if(c=='\0'){
    return -1;
  }
  if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_DS)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_DS_LOWERCASE)){
    return DEVICE_DS_ID;
  }else 
  if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_US)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_US_LOWERCASE)){
    return DEVICE_US_ID;
  }else 
  if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_WB)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_WB_LOWERCASE)){
    return DEVICE_WB_ID;
  }else 
  if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_IH)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_IH_LOWERCASE)){
    return DEVICE_IH_ID;
  }else{
    return -1;
  }
}


#ifdef USE_SCHEDULED_HEATING
int8_t mHeating::GetScheduleModeIDByCtr(const char* c){
  if(c=='\0'){ return -1; }
  // if(strstr(c,D_JSON_SCHEDULED_OFF)){ return SCHEDULED_OFF; }
  // if(strstr(c,D_JSON_SCHEDULED_SET)){ return SCHEDULED_SET; }
  // if(strstr(c,D_JSON_SCHEDULED_ON)){ return SCHEDULED_ON; }
  // if(strstr(c,D_JSON_MANUAL_ON)){ return SCHEDULED_MANUAL_ON; }
  return -1; // none
}
const char* mHeating::GetScheduleNameCtrbyID(uint8_t mode, char* buffer){
  if(buffer == nullptr){ return 0;}
  switch(mode){
    case SCHEDULED_OFF_ID:       memcpy_P(buffer, PM_HEATING_SENSOR_NAME_LONG_DS_CTR, sizeof(PM_HEATING_SENSOR_NAME_LONG_DS_CTR)); break;
    case SCHEDULED_SET_ID:       memcpy_P(buffer, PM_HEATING_SENSOR_NAME_LONG_US_CTR, sizeof(PM_HEATING_SENSOR_NAME_LONG_US_CTR)); break;
    case SCHEDULED_ON_ID:        memcpy_P(buffer, PM_HEATING_SENSOR_NAME_LONG_WB_CTR, sizeof(PM_HEATING_SENSOR_NAME_LONG_WB_CTR)); break;
    case SCHEDULED_MANUAL_ON_ID: memcpy_P(buffer, PM_HEATING_SENSOR_NAME_LONG_IH_CTR, sizeof(PM_HEATING_SENSOR_NAME_LONG_IH_CTR)); break;
  }
  return buffer;
}
#endif //USE_SCHEDULED_HEATING

const char* mHeating::GetTempModeByDeviceIDCtr(uint8_t device, char* buffer, uint8_t buflen){
  if(buffer == nullptr){ return 0;}
  switch(program_temps[device].status.mode){
    default:               snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH); break;
    case TEMP_MODE_OFF_ID: snprintf_P(buffer, buflen, PM_TEMP_MODE_OFF_CTR); break;
  }
  return buffer;
  //   (program_temps[device].status.mode == TEMP_MODE_HEATING ? "TEMP_MODE_HEATING\0" :
  //   (program_temps[device].status.mode == TEMP_MODE_MAINTAINING ? "TEMP_MODE_MAINTAINING\0" :
  //   (program_temps[device].status.mode == TEMP_MODE_SCHEDULED ? "TEMP_MODE_SCHEDULED\0" :
  //   D_UNKNOWN))));
}

const char* mHeating::GetTempActiveProgramByDeviceIDCtr(uint8_t device, char* buffer){
  if(buffer == nullptr){ return 0;}
  return 0;
  // return 
  //   (program_temps[device].status.mode == TEMP_MODE_OFF ? "TEMP_MODE_OFF\0" :
  //   (program_temps[device].status.mode == TEMP_MODE_HEATING ? "TEMP_MODE_HEATING\0" :
  //   (program_temps[device].status.mode == TEMP_MODE_MAINTAINING ? "TEMP_MODE_MAINTAINING\0" :
  //   (program_temps[device].status.mode == TEMP_MODE_SCHEDULED ? "TEMP_MODE_SCHEDULED\0" :
  //   D_UNKNOWN))));
}


int8_t mHeating::Tasker(uint8_t function){

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    pre_init();
  }else
  if(function == FUNC_INIT){
    init();
  }

  // Only continue in to tasker if module was configured properly
  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * SETTINGS SECTION * 
    *******************/
    case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
      // Settings_Load();
    break;
    case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
      // Settings_Save();
    break;
    case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
      // Settings_Default();
    break;
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
      // Settings_Default();
      // pCONT_set->SettingsSave(2);
    break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_MINUTE:     

    break;
    /************
     * FUNCTION HANDLER SECTION * 
    *******************/
    case FUNC_FUNCTION_LAMBDA_INIT:
      FunctionHandler_Init();
    break;
    case FUNC_FUNCTION_LAMBDA_LOOP:       //calls from here, into support requiring no object
      FunctionHandler_Loop();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_COMMAND:

    break;  
    /************
     * MQTT SECTION * 
    *******************/
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init(); // Reset to the initial parameters
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  
  #ifdef USE_MODULE_CORE_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_CORE_WEBSERVER

} // END Tasker
int8_t mHeating::Tasker(uint8_t function, JsonObjectConst obj){
  switch(function){
    case FUNC_JSON_COMMAND_OBJECT:
      parsesub_CheckAll(obj);
    break;
    case FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC:
      return CheckAndExecute_JSONCommands(obj);
    break;
  }
}


void mHeating::parsesub_CheckAll(JsonObjectConst obj){

  parsesub_ModeManual(obj);
  parsesub_ProgramTimers(obj);
  parsesub_ProgramTemps(obj);

}

int8_t mHeating::CheckAndExecute_JSONCommands(JsonObjectConst obj){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/heating")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_HEATING));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
      parsesub_TopicCheck_JSONCommand(obj);
      return FUNCTION_RESULT_HANDLED_ID;
  }else{
    return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
  }

}

void mHeating::parsesub_TopicCheck_JSONCommand(JsonObjectConst obj){
    
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/manual",sizeof("/manual")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "manual"));    
    #endif
    parsesub_ModeManual(obj);
  }else
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/programs/timers",sizeof("/programs/timers")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "/programs/timers"));    
    #endif
    parsesub_ProgramTimers(obj);
  }else
  if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/programs/temps",sizeof("/programs/temps")-1)>=0){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED D_TOPIC "/programs/temps"));    
    #endif
    parsesub_ProgramTemps(obj);
  }

}


void mHeating::parsesub_ModeManual(JsonObjectConst obj){

  int8_t device_id,schedule_id;
  uint8_t timeon,tempset;

  if((device_id = GetDeviceIDbyName(obj[D_JSON_DEVICE]))>=0){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_DEVICE,device_id);
    //Response_mP(S_JSON_COMMAND_NVALUE, D_DEVICE,device_id);
  }else{
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_NOMATCH));
    //Response_mP(S_JSON_COMMAND_SVALUE, D_DEVICE,D_PARSING_NOMATCH);
    return; // Unknown device, can't execute
  }

  if(!obj[D_JSON_TIME_ON].isNull()){ 
    program_timers[device_id].time_minutes_on = obj[D_JSON_TIME_ON];
    program_timers[device_id].time_minutes_on_start = program_timers[device_id].time_minutes_on;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_TIME_ON,program_timers[device_id].time_minutes_on);
    //Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_ON, program_timers[device_id].time_minutes_on);
    // fForceHeatingTimerUpdate = true;
    functionhandler_programs_timers.flags.run_now = true;
    isanychanged_timers = true;
    data_buffer2.isserviced++;
  }

  if(!obj[D_JSON_TIME_RUNNING][D_JSON_LIMIT].isNull()){ 
    program_temps[device_id].time_running.limit = obj[D_JSON_TIME_RUNNING][D_JSON_LIMIT];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_LIMIT,program_temps[device_id].time_running.limit);
    //Response_mP(S_JSON_COMMAND_NVALUE, D_LIMIT,program_temps[device_id].time_running.limit);
    fForceHeatingTempUpdate = true;
    data_buffer2.isserviced++;
  }

// #ifdef USE_HEATING_TEMPS
//   // TEMP RELATED
//   if(!obj[D_JSON_TEMP][D_JSON_SET].isNull()){ 
//     program_temps[device_id].temp.desired = obj[D_JSON_TEMP][D_JSON_SET];
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_SET,program_temps[device_id].temp.desired);
//     //Response_mP(S_JSON_COMMAND_NVALUE, D_SET,program_temps[device_id].temp.desired);
//     fForceHeatingTempUpdate = true;
//     data_buffer2.isserviced++;
//   }

//   // if(!obj[D_JSON_SCHEDULE][D_JSON_MODE].isNull()){ 
//   //   program_temps[device_id].schedule.mode_sch = GetScheduleModeIDByCtr(obj[D_JSON_SCHEDULE][D_JSON_MODE]);
//   //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
//   //   //Response_mP(S_JSON_COMMAND_SVALUE, D_SET,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
//   //   fForceHeatingTempsUpdate = true;
//   //   data_buffer2.isserviced++;

//   //   if(program_temps[device_id].schedule.mode_sch == SCHEDULED_MANUAL_ON_ID){
//   //     program_temps[device_id].time_running.on = 0;
//   //   }else if(program_temps[device_id].schedule.mode_sch == SCHEDULED_OFF_ID){
//   //     program_temps[device_id].time_running.on = -1;
//   //     program_temps[device_id].time_maintaining.on = -1;
//   //     SetHeater(device_id,0);
//   //   }
//   // }
//   // if(!obj[D_JSON_SCHEDULE][D_JSON_ONTIME].isNull()){
//   //   datetime_t ontime = pCONT->mt->GetTimefromCtr(obj["schedule"]["ontime"]);
//   //   program_temps[device_id].schedule.ontime = ontime;

//   //   //pCONT->mso->MessagePrint("ontime>>\t "); 
//   //   //pCONT->mt->PrintDateTime(&program_temps[device_id].schedule.ontime);
//   //   //pCONT->mso->MessagePrintln(" ");
//   //   // pCONT->mso->MessagePrint(program_temps[device_id].schedule.ontime.hour);pCONT->mso->MessagePrint(":");
//   //   // pCONT->mso->MessagePrint(program_temps[device_id].schedule.ontime.minute);pCONT->mso->MessagePrint(":");
//   //   // pCONT->mso->MessagePrintln(program_temps[device_id].schedule.ontime.second);

//   //   // Check if new hour is tomorrow
//   //   // if(program_temps[device_id].schedule.ontime.hour<pCONT->mt->mtime.hour){
//   //   //   pCONT->mt->AddSecondsToDateTime(program_temps[device_id].schedule.ontime,SEC2DAY);
//   //   // }

//   //   // add off time based on max limit possible
//   //    program_temps[device_id].schedule.offtime = program_temps[device_id].schedule.ontime; // Copy everything
//   //    pCONT->mt->AddSecondsToDateTime(&program_temps[device_id].schedule.offtime,SEC2MIN*program_temps[device_id].time_running.limit);

//   //   // pCONT->mso->MessagePrint("offtime>>\t "); pCONT->mt->PrintDateTime(&program_temps[device_id].schedule.offtime);
//   //   //pCONT->mso->MessagePrintln(" ");
//   //   //pCONT->mso->MessagePrint("mtime>>\t "); pCONT->mt->PrintDateTime(&pCONT->mt->mtime);

//   //    // splash result (add new ctr for this message)
//   //    program_temps[device_id].schedule.mode_sch = SCHEDULED_SET_ID;
//   //    program_temps[device_id].schedule.fHasRun = 0;
//   //    program_temps[device_id].status.mode = TEMP_MODE_SCHEDULED;
//   //    program_temps[device_id].status.mode_next = program_temps[device_id].status.mode;
//   //    program_temps[device_id].status.mode = TEMP_MODE_SPLASH_RUN_TIME;

//   //    program_temps[device_id].status.tSplashTime = millis();
//   //    program_temps[device_id].status.msSplashtime = 5000; // 2 seconds

//   //   fForceHeatingTempsUpdate = true;
//   //   fForceMQTTUpdate = true;


//   //   data_buffer2.isserviced++;
//   // }
//   #endif

  mqtthandler_program_timers_ifchanged.flags.SendNow = true;
  mqtthandler_program_temps_ifchanged.flags.SendNow = true;

  //    temp mesaure, send everything
  // MQTTHandler_Set_fSendNow();

  

}



void mHeating::parsesub_ProgramTimers(JsonObjectConst obj){

  int8_t device_id,schedule_id;
  uint8_t timeon,tempset;

  if((device_id = GetDeviceIDbyName(obj[D_JSON_DEVICE]))>=0){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_DEVICE,device_id);
    //Response_mP(S_JSON_COMMAND_NVALUE, D_DEVICE,device_id);
  }else{
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_NOMATCH));
    //Response_mP(S_JSON_COMMAND_SVALUE, D_DEVICE,D_PARSING_NOMATCH);
    return; // Unknown device, can't execute
  }

  if(!obj[D_JSON_TIME_ON].isNull()){ 
    program_timers[device_id].time_minutes_on = obj[D_JSON_TIME_ON];
    program_timers[device_id].time_minutes_on_start = program_timers[device_id].time_minutes_on;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_TIME_ON,program_timers[device_id].time_minutes_on);
    //Response_mP(S_JSON_COMMAND_NVALUE, D_JSON_TIME_ON, program_timers[device_id].time_minutes_on);
    // fForceHeatingTimerUpdate = true;
    functionhandler_programs_timers.flags.run_now = true;
    isanychanged_timers = true;
    data_buffer2.isserviced++;
  }

  if(!obj[D_JSON_TIME_RUNNING][D_JSON_LIMIT].isNull()){ 
    program_temps[device_id].time_running.limit = obj[D_JSON_TIME_RUNNING][D_JSON_LIMIT];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_JSON_LIMIT,program_temps[device_id].time_running.limit);
    //Response_mP(S_JSON_COMMAND_NVALUE, D_LIMIT,program_temps[device_id].time_running.limit);
    fForceHeatingTempUpdate = true;
    data_buffer2.isserviced++;
  }

  mqtthandler_program_timers_ifchanged.flags.SendNow = true;

  //    temp mesaure, send everything
  // MQTTHandler_Set_fSendNow();

  

}


void mHeating::parsesub_ProgramTemps(JsonObjectConst obj){

  int8_t device_id,schedule_id;
  uint8_t timeon,tempset;

  if((device_id = GetDeviceIDbyName(obj[D_JSON_DEVICE]))>=0){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_DEVICE,device_id);
    //Response_mP(S_JSON_COMMAND_NVALUE, D_DEVICE,device_id);
  }else{
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_NOMATCH));
    //Response_mP(S_JSON_COMMAND_SVALUE, D_DEVICE,D_PARSING_NOMATCH);
    return; // Unknown device, can't execute
  }

#ifdef USE_HEATING_TEMPS
  // TEMP RELATED
  if(!obj[D_JSON_TEMP][D_JSON_SET].isNull()){ 
    program_temps[device_id].temp.desired = obj[D_JSON_TEMP][D_JSON_SET];
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_SET,program_temps[device_id].temp.desired);
    //Response_mP(S_JSON_COMMAND_NVALUE, D_SET,program_temps[device_id].temp.desired);
    fForceHeatingTempUpdate = true;
    data_buffer2.isserviced++;
  }

  // if(!obj[D_JSON_SCHEDULE][D_JSON_MODE].isNull()){ 
  //   program_temps[device_id].schedule.mode_sch = GetScheduleModeIDByCtr(obj[D_JSON_SCHEDULE][D_JSON_MODE]);
  //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
  //   //Response_mP(S_JSON_COMMAND_SVALUE, D_SET,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
  //   fForceHeatingTempsUpdate = true;

  //   if(program_temps[device_id].schedule.mode_sch == SCHEDULED_MANUAL_ON_ID){
  //     program_temps[device_id].time_running.on = 0;
  //   }else if(program_temps[device_id].schedule.mode_sch == SCHEDULED_OFF_ID){
  //     program_temps[device_id].time_running.on = -1;
  //     program_temps[device_id].time_maintaining.on = -1;
  //     SetHeater(device_id,0);
  //   }
  // }
  // if(!obj[D_JSON_SCHEDULE][D_JSON_ONTIME].isNull()){
  //   datetime_t ontime = pCONT->mt->GetTimefromCtr(obj["schedule"]["ontime"]);
  //   program_temps[device_id].schedule.ontime = ontime;

  //   //pCONT->mso->MessagePrint("ontime>>\t "); 
  //   //pCONT->mt->PrintDateTime(&program_temps[device_id].schedule.ontime);
  //   //pCONT->mso->MessagePrintln(" ");
  //   // pCONT->mso->MessagePrint(program_temps[device_id].schedule.ontime.hour);pCONT->mso->MessagePrint(":");
  //   // pCONT->mso->MessagePrint(program_temps[device_id].schedule.ontime.minute);pCONT->mso->MessagePrint(":");
  //   // pCONT->mso->MessagePrintln(program_temps[device_id].schedule.ontime.second);

  //   // Check if new hour is tomorrow
  //   // if(program_temps[device_id].schedule.ontime.hour<pCONT->mt->mtime.hour){
  //   //   pCONT->mt->AddSecondsToDateTime(program_temps[device_id].schedule.ontime,SEC2DAY);
  //   // }

  //   // add off time based on max limit possible
  //    program_temps[device_id].schedule.offtime = program_temps[device_id].schedule.ontime; // Copy everything
  //    pCONT->mt->AddSecondsToDateTime(&program_temps[device_id].schedule.offtime,SEC2MIN*program_temps[device_id].time_running.limit);

  //   // pCONT->mso->MessagePrint("offtime>>\t "); pCONT->mt->PrintDateTime(&program_temps[device_id].schedule.offtime);
  //   //pCONT->mso->MessagePrintln(" ");
  //   //pCONT->mso->MessagePrint("mtime>>\t "); pCONT->mt->PrintDateTime(&pCONT->mt->mtime);

  //    // splash result (add new ctr for this message)
  //    program_temps[device_id].schedule.mode_sch = SCHEDULED_SET_ID;
  //    program_temps[device_id].schedule.fHasRun = 0;
  //    program_temps[device_id].status.mode = TEMP_MODE_SCHEDULED;
  //    program_temps[device_id].status.mode_next = program_temps[device_id].status.mode;
  //    program_temps[device_id].status.mode = TEMP_MODE_SPLASH_RUN_TIME;

  //    program_temps[device_id].status.tSplashTime = millis();
  //    program_temps[device_id].status.msSplashtime = 5000; // 2 seconds

  //   fForceHeatingTempsUpdate = true;
  //   fForceMQTTUpdate = true;

  // }
  #endif

  mqtthandler_program_temps_ifchanged.flags.SendNow = true;

  //    temp mesaure, send everything
  // MQTTHandler_Set_fSendNow();

  

}






void mHeating::FunctionHandler_Init(){
  
  functionhandler_ptr = &functionhandler_status_message;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false; //on loop
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating::FunctionHandler_Program_Status;
  
  functionhandler_ptr = &functionhandler_failsafe;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false; //on loop
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_MINS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating::FunctionHandler_FailSafe;

  #ifdef USE_HEATING_PROFILE_ESTIMATION  
  functionhandler_ptr = &functionhandler_heating_profiles;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false; //on loop
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating::FunctionHandler_Heating_Profiles;
  #endif //#ifdef USE_HEATING_PROFILE_ESTIMATION
      
  functionhandler_ptr = &functionhandler_relay_status;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false; //on loop
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating::FunctionHandler_Relay_Status;

  functionhandler_ptr = &functionhandler_programs_timers;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false; //on loop
  #ifdef ENABLE_HEATING_DEBUG_TIMES
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  #else
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_MINS_ID; 
  #endif
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating::FunctionHandler_Programs_Timers;
  
  functionhandler_ptr = &functionhandler_programs_temps;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = true; //on loop
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_MS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating::FunctionHandler_Programs_Temps;
    
}


void mHeating::FunctionHandler_Loop(){

  struct functionhandler<mHeating>* functionhandler_list_ptr[] = {
    &functionhandler_failsafe,
    &functionhandler_status_message,
    #ifdef USE_HEATING_PROFILE_ESTIMATION
    &functionhandler_heating_profiles,
    #endif //#ifdef USE_HEATING_PROFILE_ESTIMATION
    &functionhandler_relay_status,
    &functionhandler_programs_timers,
    &functionhandler_programs_temps
  };

  for(uint8_t list_id=0;list_id<5;list_id++){
    pCONT_sup->FunctionHandler_Call(*this,D_MODULE_CUSTOM_HEATING_ID,functionhandler_list_ptr[list_id]); 
  }   
  
}


/**
 * Generate Messages for users to glance at via web or mqtt, timers, temps, schedules set? append the messages.
 * */
void mHeating::FunctionHandler_Program_Status(){

  memset(&heating_status,0,sizeof(heating_status));

  for(uint8_t device_id=0;device_id<4;device_id++){
    
    //Timer program active
    // if(program_timers[device_id].time_minutes_on){
      heating_status[device_id].message_len += 
        sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"Timers %d mins",program_timers[device_id].time_minutes_on>=0?program_timers[device_id].time_minutes_on:0);
    // }
    // Temps
    // if(program_temps[device_id].time_running.on){
      heating_status[device_id].message_len += 
        sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"%sTemps %d mins",heating_status[device_id].message_ctr>0?", ":"",program_temps[device_id].time_running.on>=0?program_temps[device_id].time_running.on:0);
    // }

    if(!heating_status[device_id].message_len){
      heating_status[device_id].message_len += sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"%s","Nothing Set");
    }

  }

}





////////////////////// START OF MQTT /////////////////////////

void mHeating::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60*60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_Settings;
  
  mqtthandler_ptr = &mqtthandler_program_timers_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramTimers;
  
  mqtthandler_ptr = &mqtthandler_program_timers_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramTimers;

  mqtthandler_ptr = &mqtthandler_program_temps_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramTemps;
  
  mqtthandler_ptr = &mqtthandler_program_temps_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramTemps;

  mqtthandler_ptr = &mqtthandler_program_overview_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramActive;
  
  mqtthandler_ptr = &mqtthandler_program_overview_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramActive;
  
  mqtthandler_ptr = &mqtthandler_relays_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_HeatingRelays;

  mqtthandler_ptr = &mqtthandler_relays_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_HeatingRelays;
  
  mqtthandler_ptr = &mqtthandler_sensor_pipes_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;//postfix_topic_sensor_pipes;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_PipeTemps;

  mqtthandler_ptr = &mqtthandler_sensor_pipes_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_PipeTemps;

  mqtthandler_ptr = &mqtthandler_sensor_pipes_roc1m;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC1M_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_PipeTempsROC1m;
  
  mqtthandler_ptr = &mqtthandler_sensor_pipes_roc10m;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60*10; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC10M_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_PipeTempsROC10m;

  mqtthandler_ptr = &mqtthandler_sensor_climate_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ClimateTemps;
  
  mqtthandler_ptr = &mqtthandler_sensor_climate_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ClimateTemps;

  mqtthandler_ptr = &mqtthandler_sensor_climate_roc1m;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC1M_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ClimateTempsROC1m;
  
  mqtthandler_ptr = &mqtthandler_sensor_climate_roc10m;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60*10; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC10M_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ClimateTempsROC10m;

  mqtthandler_ptr = &mqtthandler_sensor_pipes_colours_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 10; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PIPES_COLOUR_CTR;//postfix_topic_sensor_pipes_colours;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructSON_PipeTempsByColours;
  
  mqtthandler_ptr = &mqtthandler_sensor_pipes_colours_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PIPES_COLOUR_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructSON_PipeTempsByColours;

  // Measure and report every second
  pCONT_msdb18->settings.rate_measure_ms = 1000;
  // pCONT_msdb18->mqtthandler_sensor_ifchanged.tRateSecs = 5;

  //Reduce sensor lib from reporting to minimum
  pCONT_msdb18->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
  pCONT_msdb18->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
  pCONT_msdb18->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

  //Reduce sensor lib from reporting to minimum
  pCONT_dht->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
  pCONT_dht->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
  pCONT_dht->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

  

} //end "MQTTHandler_Init"


void mHeating::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_program_timers_ifchanged.flags.SendNow = true;
  mqtthandler_program_timers_teleperiod.flags.SendNow = true;
  mqtthandler_program_temps_ifchanged.flags.SendNow = true;
  mqtthandler_program_temps_teleperiod.flags.SendNow = true;
  mqtthandler_program_overview_ifchanged.flags.SendNow = true;
  mqtthandler_program_overview_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_pipes_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_pipes_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_pipes_roc1m.flags.SendNow = true;
  mqtthandler_sensor_pipes_roc10m.flags.SendNow = true;
  mqtthandler_sensor_climate_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_climate_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_climate_roc1m.flags.SendNow = true;
  mqtthandler_sensor_climate_roc10m.flags.SendNow = true;
  mqtthandler_sensor_pipes_colours_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_pipes_colours_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mHeating::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_program_timers_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_program_temps_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_program_overview_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_pipes_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_climate_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_pipes_colours_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mHeating::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t flag_handle_all = false, handler_not_found = false;
  if(mqtt_handler_id == MQTT_HANDLER_ALL_ID){ flag_handle_all = true; } //else run only the one asked for

  do{

    handler_not_found = false; //

    switch(mqtt_handler_id){
      case MQTT_HANDLER_SETTINGS_ID:                                mqtthandler_ptr=&mqtthandler_settings_teleperiod; break;
      case MQTT_HANDLER_MODULE_PROGRAM_TIMERS_IFCHANGED_ID:         mqtthandler_ptr=&mqtthandler_program_timers_ifchanged; break;
      case MQTT_HANDLER_MODULE_PROGRAM_TIMERS_TELEPERIOD_ID:        mqtthandler_ptr=&mqtthandler_program_timers_teleperiod; break;
      case MQTT_HANDLER_MODULE_PROGRAM_TEMPS_IFCHANGED_ID:          mqtthandler_ptr=&mqtthandler_program_temps_ifchanged; break;
      case MQTT_HANDLER_MODULE_PROGRAM_TEMPS_TELEPERIOD_ID:         mqtthandler_ptr=&mqtthandler_program_temps_teleperiod; break;
      case MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_IFCHANGED_ID:       mqtthandler_ptr=&mqtthandler_program_overview_ifchanged; break;
      case MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_TELEPERIOD_ID:      mqtthandler_ptr=&mqtthandler_program_overview_teleperiod; break;
      case MQTT_HANDLER_MODULE_SENSOR_PIPES_IFCHANGED_ID:           mqtthandler_ptr=&mqtthandler_sensor_pipes_ifchanged; break;
      case MQTT_HANDLER_MODULE_SENSOR_PIPES_TELEPERIOD_ID:          mqtthandler_ptr=&mqtthandler_sensor_pipes_teleperiod; break;
      case MQTT_HANDLER_MODULE_SENSOR_PIPES_ROC1M_ID:               mqtthandler_ptr=&mqtthandler_sensor_pipes_roc1m; break;
      case MQTT_HANDLER_MODULE_SENSOR_PIPES_ROC10M_ID:              mqtthandler_ptr=&mqtthandler_sensor_pipes_roc10m; break;
      case MQTT_HANDLER_MODULE_SENSOR_CLIMATE_IFCHANGED_ID:         mqtthandler_ptr=&mqtthandler_sensor_climate_ifchanged; break;
      case MQTT_HANDLER_MODULE_SENSOR_CLIMATE_TELEPERIOD_ID:        mqtthandler_ptr=&mqtthandler_sensor_climate_teleperiod; break;
      case MQTT_HANDLER_MODULE_SENSOR_CLIMATE_ROC1M_ID:             mqtthandler_ptr=&mqtthandler_sensor_climate_roc1m; break;
      case MQTT_HANDLER_MODULE_SENSOR_CLIMATE_ROC10M_ID:            mqtthandler_ptr=&mqtthandler_sensor_climate_roc10m; break;
      case MQTT_HANDLER_MODULE_SENSOR_PIPES_COLOURS_IFCHANGED_ID:   mqtthandler_ptr=&mqtthandler_sensor_pipes_colours_ifchanged; break;
      case MQTT_HANDLER_MODULE_SENSOR_PIPES_COLOURS_TELEPERIOD_ID:  mqtthandler_ptr=&mqtthandler_sensor_pipes_colours_teleperiod; break;
      case MQTT_HANDLER_MODULE_DRIVERS_RELAYS_IFCHANGED_ID:         mqtthandler_ptr=&mqtthandler_relays_ifchanged; break;
      case MQTT_HANDLER_MODULE_DRIVERS_RELAYS_TELEPERIOD_ID:        mqtthandler_ptr=&mqtthandler_relays_teleperiod; break;
      default: handler_not_found=true; break; // nothing 
    } // switch

    // Pass handlers into command to test and (ifneeded) execute
    if(!handler_not_found){ pCONT->mqt->MQTTHandler_Command(*this,D_MODULE_CUSTOM_HEATING_ID,mqtthandler_ptr); }

    // stop searching
    if(mqtt_handler_id++>MQTT_HANDLER_MODULE_LENGTH_ID){flag_handle_all = false; return;}

  }while(flag_handle_all);

}


////////////////////// END OF MQTT /////////////////////////


#endif
