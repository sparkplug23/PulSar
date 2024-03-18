#include "mINA219.h"

#ifdef USE_MODULE_ENERGY_INA219
#ifdef USE_I2C

const char* mEnergyINA219::PM_MODULE_ENERGY_INA219_CTR = D_MODULE_ENERGY_INA219_CTR;
const char* mEnergyINA219::PM_MODULE_ENERGY_INA219_FRIENDLY_CTR = D_MODULE_ENERGY_INA219_FRIENDLY_CTR;


uint8_t ina219_type[4] = {0,0,0,0};
uint8_t ina219_addresses[] = { INA219_ADDRESS1, INA219_ADDRESS2, INA219_ADDRESS3, INA219_ADDRESS4 };

char ina219_types[] = "INA219";
uint8_t ina219_count = 0;

int8_t mEnergyINA219::Tasker(uint8_t function, JsonParserObject obj){

  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
  }

  // Only continue to remaining functions if sensor has been detected and enabled
  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_INIT:
      init();
    break;
    /************
     * SETTINGS SECTION * 
    *******************/
    case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
      // Settings_Load();
    break;
    case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
      // Settings_Save();
    break;
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
      // Settings_Default();
      // pCONT_set->SettingsSave(2);
    break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;  
    case FUNC_EVERY_SECOND:
      ALOG_DBG( PSTR("Read"));   

      
      // DIGITAL_INVERT_PIN(13);
      // DIGITAL_INVERT_PIN(14);
      // DIGITAL_INVERT_PIN(27);
      // DIGITAL_INVERT_PIN(26);


    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      // parse_JSONCommand(obj);
    break; 
    /************
     * WEBPAGE SECTION * 
    *******************/
    // #ifdef USE_MODULE_NETWORK_WEBSERVER
    // case FUNC_WEB_ADD_ROOT_TABLE_ROWS:
    //   WebAppend_Root_Status_Table_Draw();
    //   break;
    // case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED:
    //   WebAppend_Root_Status_Table_Data();
    //   break;
    // #endif //USE_MODULE_NETWORK_WEBSERVER
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
      break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
      break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
      break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return function_result;

}




void mEnergyINA219::Pre_Init(){

  settings.fEnableSensor = false;
  settings.fSensorCount = 0;

  // in futre use array to store bme type found (BME_280_ID, BME_180_ID) etc
 
 
// void Detect(void)
// {
//   // because there is 4 addresses?
//   for (uint32_t i = 0; i < sizeof(ina219_type); i++) {
//     uint16_t addr = ina219_addresses[i];
//     if (I2cActive(addr)) { continue; }
//     if (SetCalibration(Settings.ina219_mode, addr)) {
//       I2cSetActiveFound(addr, ina219_types);
//       ina219_type[i] = 1;
//       ina219_count++;
//     }
//   }
// }



 //0x40, 0x41 0x44 or 0x45
  if(pCONT_sup->I2cDevice(0x40) || pCONT_sup->I2cDevice(0x41) || pCONT_sup->I2cDevice(0x44) || pCONT_sup->I2cDevice(0x45)){

    // Wire = new TwoWire();//pCONT_pins->GetPin(GPIO_I2C_SCL_ID),pCONT_pins->GetPin(GPIO_I2C_SDA_ID));
  
//tmp
pCONT_set->Settings.ina219_mode = 0;


      ina219_count++;
      settings.fSensorCount++;


  // for (
    uint32_t i = 0;
    //  i < sizeof(ina219_type); i++) {
    uint16_t addr = ina219_addresses[i];
//     if (I2cActive(addr)) { continue; }
    if (SetCalibration(pCONT_set->Settings.ina219_mode, addr)) {
      // I2cSetActiveFound(addr, ina219_types);
      ina219_type[i] = 1;
      ina219_count++;
    }
  // }
//}



    // sensor[settings.fSensorCount].bme = new Adafruit_BME280();
    // if (sensor[settings.fSensorCount].bme->begin(pCONT_sup->wire)) {
    //   settings.fSensorCount++;
    // }else{
    //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_BME "BME280 sensor not detected"));
    // }
  }

  
  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "BME Sensor Enabled"));
  }

}

void mEnergyINA219::init(void){

  for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){    
    sensor[sensor_id].tSavedMeasure = millis();
    sensor[sensor_id].sReadSensor = SPLIT_TASK_SEC1_ID;    
  }
  settings.measure_rate_ms = 1000;
  
}

void mEnergyINA219::EveryLoop(){
    
  for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){
    if(mTime::TimeReachedNonReset(&sensor[sensor_id].tSavedMeasure,settings.measure_rate_ms)){
      SplitTask_ReadSensor(sensor_id, DONTREQUIRE_COMPLETE);
      if(sensor[sensor_id].sReadSensor==SPLIT_TASK_DONE_ID){
        sensor[sensor_id].sReadSensor=SPLIT_TASK_SUCCESS_ID;
        sensor[sensor_id].tSavedMeasure = millis();
      }
    }
  }
  
}

//     #ifdef USE_MODULE_NETWORK_WEBSERVER
// void mEnergyINA219::WebAppend_Root_Status_Table_Draw(){

//   char buffer[30];
   
//   BufferWriterI->Append_P(PSTR("{t}"));  
//   BufferWriterI->Append_P(PSTR("<tr><td><b>INA219 Current Sensor</b></td></tr>"));//GetPaletteFriendlyName(),GetNumberOfColoursInPalette(mPaletteI->static_palettes.ptr));

//   //headers
//   BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//   for(int col=0;col<settings.fSensorCount+1;col++){ //sensors + title colomn
//     if(col==0){ //first column blank
//       BufferWriterI->Append_P(PSTR("<th></th>"));
//     }else{
//       BufferWriterI->Append_P(PSTR("<td>%s</td>"), DLI->GetDeviceNameWithEnumNumber(D_MODULE_SENSORS_INA219_ID,col-1,buffer,sizeof(buffer)));
//     }
//   }    
//   BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_END_0V);
//   //rows
//   for(int row=0;row<TABLE_ROW_ITEMS_COUNT+1;row++){
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//     for(int col=0;col<settings.fSensorCount+1;col++){
//       if(col==0){ //row name
//         BufferWriterI->Append_P(PSTR("<th>%s</th>"), pCONT_sup->GetTextIndexed_P(buffer, sizeof(buffer), row, PM_DLIM_LIST_TABLE_HEADERS_INA219));
//       }else{
//         BufferWriterI->Append_P(PSTR("<td>{dc}%s'>" D_DEFAULT_HTML_VALUE "</div></td>"),"ina219_tab");  
//       }
//     }
//     BufferWriterI->Append_P(PM_WEBAPPEND_TABLE_ROW_START_0V);
//   }
//   BufferWriterI->Append_P(PSTR("{t2}")); 

// }


// //append to internal buffer if any root messages table
// void mEnergyINA219::WebAppend_Root_Status_Table_Data(){
  
//   uint8_t count = 0;

//   JsonBuilderI->Array_Start("ina219_tab");// Class name
//   for(int row=0;row<6;row++){
//     for(int sensor_id=0;sensor_id<settings.fSensorCount;sensor_id++){
//       JsonBuilderI->Object_Start();
//         JsonBuilderI->Add("id",count++);
//         if(sensor[sensor_id].power_mw>30){ JsonBuilderI->Add("fc","#ff0000");
//         }else{ JsonBuilderI->Add("fc","#ffffff"); }
//         switch(row){
//           case 0: JsonBuilderI->Add("ih",sensor[sensor_id].bus_voltage_mv); break;
//           case 1: JsonBuilderI->Add("ih",sensor[sensor_id].shunt_voltage_mv); break;
//           case 2: JsonBuilderI->Add("ih",sensor[sensor_id].load_voltage_mv); break;
//           case 3: JsonBuilderI->Add("ih",sensor[sensor_id].current_ma); break;
//           case 4: JsonBuilderI->Add("ih",sensor[sensor_id].power_mw); break;
//         } //switch      
//       JsonBuilderI->Object_End();
//     }
//   }//end for
//   JsonBuilderI->Array_End();

// }

//     #endif // USE_MODULE_NETWORK_WEBSERVER

// New function that breaks things up into switch statements
// Extra argument -- "require_completion" ie loop until status SPLIT_TASK_DONE_ID
void mEnergyINA219::SplitTask_ReadSensor(uint8_t sensor_id, uint8_t require_completion){

  uint16_t addr = ina219_addresses[sensor_id];

  // Add sensor class operator overload? that += actually averages?

  sensor[sensor_id].bus_voltage_mv = GetBusVoltage_mV(addr);

  //Average storing
  // I should save all these into a temporary struct, then choose whether to save (copy) or merge (average)
  // but for now, lazy way
  sensor_averages[0].bus_voltage_mv += sensor[sensor_id].bus_voltage_mv;
  sensor_averages[0].bus_voltage_mv /= 2; // running average of 2


  sensor[sensor_id].shunt_voltage_mv = GetShuntVoltage_mV(addr);   
  sensor[sensor_id].direct_current_ma = GetCurrent_mA(addr);
  sensor[sensor_id].direct_power_mw = GetPower_mW(addr);
  sensor[sensor_id].load_voltage_mv = sensor[sensor_id].bus_voltage_mv + sensor[sensor_id].shunt_voltage_mv; 
  // we return the power-supply-side voltage (as bus_voltage register provides the load-side voltage)

  // The following multiplier is used to convert shunt voltage (in mV) to current (in A)
  sensor[sensor_id].current_ma = sensor[sensor_id].shunt_voltage_mv * ina219_current_multiplier * 1000;
  sensor[sensor_id].power_mw  = (sensor[sensor_id].current_ma/1000) * sensor[sensor_id].load_voltage_mv;
  
  ALOG_DBG( PSTR("sensor[%d].load_voltage_mv=%d"), sensor_id, (int)sensor[sensor_id].load_voltage_mv);
  ALOG_DBG( PSTR("sensor[%d].current_ma=%d"),      sensor_id, (int)sensor[sensor_id].current_ma);   
  ALOG_DBG( PSTR("sensor[%d].power_mw=%d"),        sensor_id, (int)sensor[sensor_id].power_mw);   

  sensor[sensor_id].isvalid = true;
  sensor[sensor_id].ischanged = true;
  sensor[sensor_id].sReadSensor=SPLIT_TASK_DONE_ID;

  // unsigned long timeout = millis();
  // do{

  //   switch(sensor[sensor_id].sReadSensor){
  //     case SPLIT_TASK_SUCCESS_ID: // allow it to run into task1
  //     case SPLIT_TASK_TIMEOUT_ID:
  //     case SPLIT_TASK_SEC1_ID:

  //       sensor[sensor_id].bme->takeForcedMeasurement(); // has no effect in normal mode

  //       sensor[sensor_id].isvalid = true;

  //       if(
  //         (sensor[sensor_id].temperature != sensor[sensor_id].bme->readTemperature())||
  //         (sensor[sensor_id].humidity != sensor[sensor_id].bme->readHumidity())         
  //         ){
  //         sensor[sensor_id].ischanged = true; // check if updated
  //       }else{
  //         sensor[sensor_id].ischanged = false;
  //       }
        
  //       if(
  //         (fabsf(sensor[sensor_id].temperature-sensor[sensor_id].bme->readTemperature())>0.1)||
  //         (sensor[sensor_id].temperature != sensor[sensor_id].bme->readTemperature())&&(abs(millis()-sensor[sensor_id].ischangedtLast)>60000)  
  //       ){
  //         sensor[sensor_id].ischanged_over_threshold = true;
  //         mqtthandler_sensor_ifchanged.flags.SendNow = true;
  //         sensor[sensor_id].ischangedtLast = millis();
  //       }else{
  //         sensor[sensor_id].ischanged_over_threshold = false;
  //       }

  //       sensor[sensor_id].temperature = sensor[sensor_id].bme->readTemperature();
  //       sensor[sensor_id].humidity =    sensor[sensor_id].bme->readHumidity();
  //       sensor[sensor_id].pressure =    sensor[sensor_id].bme->readPressure() / 100.0f;
  //       sensor[sensor_id].altitude =    sensor[sensor_id].bme->readAltitude(sealevel_pressure);

  //       AddLog(LOG_LEVEL_DEBUG,      PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_TEMPERATURE,  (int)sensor[sensor_id].temperature);
  //       ALOG_DBM( PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_HUMIDITY,    (int)sensor[sensor_id].humidity);
  //       ALOG_DBM( PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_PRESSURE,    (int)sensor[sensor_id].pressure);
  //       ALOG_DBM( PSTR(D_LOG_BME D_MEASURE D_JSON_COMMAND_NVALUE), D_ALTITUDE,    (int)sensor[sensor_id].altitude);

  //       sensor[sensor_id].sReadSensor = SPLIT_TASK_DONE_ID;

  //     break;
  //     case SPLIT_TASK_DONE_ID: //exiting
  //       fWithinLimit = 1;
  //     break;
  //     default:
  //     break;
  //   } // end switch

  //   if(require_completion){ //delay required if we are going to do multiple calls
  //   //  delay(100);
  //   }

  //   if(abs(millis()-timeout)>=2000){
  //     sensor[sensor_id].sReadSensor = SPLIT_TASK_TIMEOUT_ID;
  //     break;
  //   }

  // }while(require_completion); // loops once even if false

}//end function


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mEnergyINA219::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.fSensorCount);
  return JsonBuilderI->End();

}

uint8_t mEnergyINA219::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JsonBuilderI->Start();

  char buffer[50];

  for(uint8_t sensor_id = 0;sensor_id<MAX_SENSORS;sensor_id++){
    // if(sensor[sensor_id].ischanged_over_threshold || (json_level>JSON_LEVEL_IFCHANGED)){
      JsonBuilderI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),sensor_id,buffer,sizeof(buffer))); 


      JsonBuilderI->Add("ina219_current_multiplier",ina219_current_multiplier);


        JsonBuilderI->Add("Bus" D_JSON_VOLTAGE,   sensor[sensor_id].bus_voltage_mv);
        JsonBuilderI->Add("Shunt" D_JSON_VOLTAGE, sensor[sensor_id].shunt_voltage_mv);
        JsonBuilderI->Add("Load" D_JSON_VOLTAGE,  sensor[sensor_id].load_voltage_mv);
        JsonBuilderI->Add(D_JSON_CURRENT,         sensor[sensor_id].current_ma);
        JsonBuilderI->Add("Direct" D_JSON_CURRENT,sensor[sensor_id].direct_current_ma);
        JsonBuilderI->Add(D_JSON_POWER,           sensor[sensor_id].power_mw);
        JsonBuilderI->Add("Direct" D_JSON_POWER,  sensor[sensor_id].direct_power_mw);


        // JsonBuilderI->Add("Bus" D_JSON_VOLTAGE "_v",   sensor[sensor_id].bus_voltage_mv/1000);
        // JsonBuilderI->Add("Shunt" D_JSON_VOLTAGE"_v", sensor[sensor_id].shunt_voltage_mv/1000);
        // JsonBuilderI->Add("Load" D_JSON_VOLTAGE"_v",  sensor[sensor_id].load_voltage_mv/1000);
        // JsonBuilderI->Add(D_JSON_CURRENT "_a",         sensor[sensor_id].current_ma/1000);
        // JsonBuilderI->Add(D_JSON_POWER "_w",           sensor[sensor_id].power_mw/1000);


        // JsonBuilderI->Add(D_JSON_HUMIDITY, sensor[sensor_id].humidity);
        // JsonBuilderI->Add(D_JSON_PRESSURE, sensor[sensor_id].pressure);
        // JsonBuilderI->Add(D_JSON_ALTITUDE, sensor[sensor_id].altitude);
        // JsonBuilderI->Object_Start(D_JSON_ISCHANGEDMETHOD);
        //   JsonBuilderI->Add(D_JSON_TYPE, D_JSON_SIGNIFICANTLY);
        //   JsonBuilderI->Add(D_JSON_AGE, (uint16_t)round(abs(millis()-sensor[sensor_id].ischangedtLast)/1000));
        // JsonBuilderI->Object_End(); 



      JsonBuilderI->Object_End();
    // }
  }
  
  return JsonBuilderI->End();

}



void mEnergyINA219::MQTTHandler_Init(){

  struct handler<mEnergyINA219>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mEnergyINA219::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyINA219::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = 1;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyINA219::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);
  
} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mEnergyINA219::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mEnergyINA219::MQTTHandler_Set_DefaultPeriodRate()
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
void mEnergyINA219::MQTTHandler_Sender()
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_ENERGY_INA219_ID, handle);
  }
}








float mEnergyINA219::GetShuntVoltage_mV(uint16_t addr)
{
  // raw shunt voltage (16-bit signed integer, so +-32767)
  int16_t value = pCONT_sup->I2cReadS16(addr, INA219_REG_SHUNTVOLTAGE);
  // //DEBUG_SENSOR_LOG("GetShuntVoltage_mV: ShReg = 0x%04X",value);
  // // convert to shunt voltage in mV (so +-327mV) (LSB=10µV=0.01mV)
  return value * 0.01;
}

float mEnergyINA219::GetBusVoltage_mV(uint16_t addr){
  return GetBusVoltage_V(addr) * 1000;
}
float mEnergyINA219::GetBusVoltage_V(uint16_t addr)
{
  // Shift 3 to the right to drop CNVR and OVF as unsigned
  uint16_t value = pCONT_sup->I2cRead16(addr, INA219_REG_BUSVOLTAGE) >> 3;
  //ALOG_DBG( PSTR("GetBusVoltage_V: BusReg = 0x%04X"),value);
  // and multiply by LSB raw bus voltage to return bus voltage in volts (LSB=4mV=0.004V)
  return value * 0.004;
}



// Not used any more
float mEnergyINA219::GetCurrent_mA(uint16_t addr)
{
  // Sometimes a sharp load will reset the INA219, which will reset the cal register,
  // meaning CURRENT and POWER will not be available ... avoid this by always setting
  // a cal value even if it's an unfortunate extra step

  ina219_calValue = 4096; //default full range
  ina219_currentDivider_mA = 10; // Current LSB = 100uA per bit (1000/100 = 10)
  ina219_powerMultiplier_mW = 2; // Power LSB = 1mW per bit (2/1)
 
 pCONT_sup->I2cWrite16(addr, INA219_REG_CALIBRATION, ina219_calValue);
  // Now we can safely read the CURRENT register!
  // raw current value (16-bit signed integer, so +-32767)
  float value = pCONT_sup->I2cReadS16(addr, INA219_REG_CURRENT);
  value /= ina219_currentDivider_mA;
  // current value in mA, taking into account the config settings and current LSB
  return value;
}

// Not used any more
float mEnergyINA219::GetPower_mW(uint16_t addr)
{
  // Sometimes a sharp load will reset the INA219, which will reset the cal register,
  // meaning CURRENT and POWER will not be available ... avoid this by always setting
  // a cal value even if it's an unfortunate extra step

  ina219_calValue = 4096; //default full range
  ina219_currentDivider_mA = 10; // Current LSB = 100uA per bit (1000/100 = 10)
  ina219_powerMultiplier_mW = 2; // Power LSB = 1mW per bit (2/1)
 
 pCONT_sup->I2cWrite16(addr, INA219_REG_CALIBRATION, ina219_calValue);
  // Now we can safely read the CURRENT register!
  // raw current value (16-bit signed integer, so +-32767)
  float value = pCONT_sup->I2cReadS16(addr, INA219_REG_POWER);
  value *= ina219_powerMultiplier_mW;
  // current value in mA, taking into account the config settings and current LSB
  return value;
}


/*********************************************************************************************************************************************
**********************************************************************************************************************************************
**********************************************************************************************************************************************
**********************************************************************************************************************************************
**********************************************************************************************************************************************
**********************************************************************************************************************************************
******** CALIBRATION SECTION **************************************************************************************************************************************
**********************************************************************************************************************************************
**********************************************************************************************************************************************
**********************************************************************************************************************************************
**********************************************************************************************************************************************
**********************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/



/*********************************************************************************************\
 * Calculate current multiplier depending on the selected mode
 * For mode = 0, 1, 2 : legacy modes simplified as Vmax: 32V, Imax: 3.2A range
 * For mode = 10..255 : specify Rshunt encoded as RRM where resistor value is RR * 10^M milliOhms
 *                      Vmax: 32V, Imax: 0.320 / Rshunt
 * Exemple:
 *     10: Rshunt = 1 * 10^0 =  1 millOhms   => Max current = 320A !
 *     11: Rshunt = 1 * 10^1 = 10 milliOhms  => Max current = 32A
 *     21: Rshunt = 2 * 10^1 = 20 milliOhms  => Max current = 16A
 *     12: Rshunt = 1 * 10^2 = 100 milliOhms => Max current = 3.2A == mode 0,1,2
 *     13: Rshunt = 1 * 10^3 =   1 Ohms      => Max current = 320mA
 * Note that some shunt values can be represented by 2 different encoded values such as
 *     11 or 100 both present 10 milliOhms
 * Because it is difficult to make a range check on such encoded value, none is performed
\*********************************************************************************************/
bool mEnergyINA219::SetCalibration(uint8_t mode, uint16_t addr)
{
  uint16_t config = 0;

  ALOG_DBG( "SetCalibration: mode=%d",mode);
  if (mode < 5)
  {
    // All legacy modes 0..2 are handled the same and consider default 0.1 shunt resistor
    ina219_current_multiplier = 1.0 / INA219_DEFAULT_SHUNT_RESISTOR_MILLIOHMS;
    // #ifdef DEBUG_TASMOTA_SENSOR
    // dtostrfd(ina219_current_multiplier,5,__ina219_dbg1);
    // //DEBUG_SENSOR_LOG("SetCalibration: cur_mul=%s",__ina219_dbg1);
    // #endif
  }
  else if (mode >= 10)
  {
    int mult = mode % 10;
    int shunt_milliOhms = mode / 10;
    for ( ; mult > 0 ; mult-- )
      shunt_milliOhms *= 10;
    ina219_current_multiplier = 1.0 / shunt_milliOhms;
    // #ifdef DEBUG_TASMOTA_SENSOR
    // dtostrfd(ina219_current_multiplier,5,__ina219_dbg1);
    // //DEBUG_SENSOR_LOG("SetCalibration: shunt=%dmO => cur_mul=%s",shunt_milliOhms,__ina219_dbg1);
    // #endif
  }
  config = INA219_CONFIG_BVOLTAGERANGE_32V
         | INA219_CONFIG_GAIN_8_320MV               // Use max scale
         | INA219_CONFIG_BADCRES_12BIT_16S_8510US   // use averaging to improve accuracy
         | INA219_CONFIG_SADCRES_12BIT_16S_8510US   // use averaging to improve accuracy
         | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  // Set Config register to take into account the settings above
  return pCONT_sup->I2cWrite16(addr, INA219_REG_CONFIG, config);
}
/*!
 *  @brief  Configures to INA219 to be able to measure up to 32V and 2A
 *          of current.  Each unit of current corresponds to 100uA, and
 *          each unit of power corresponds to 2mW. Counter overflow
 *          occurs at 3.2A.
 *  @note   These calculations assume a 0.1 ohm resistor is present
 */
void mEnergyINA219::setCalibration_32V_2A() {
  // By default we use a pretty huge range for the input voltage,
  // which probably isn't the most appropriate choice for system
  // that don't use a lot of power.  But all of the calculations
  // are shown below if you want to change the settings.  You will
  // also need to change any relevant register settings, such as
  // setting the VBUS_MAX to 16V instead of 32V, etc.

  // VBUS_MAX = 32V             (Assumes 32V, can also be set to 16V)
  // VSHUNT_MAX = 0.32          (Assumes Gain 8, 320mV, can also be 0.16, 0.08, 0.04) 
  // RSHUNT = 0.1               (Resistor value in ohms)

  // 1. Determine max possible current
  // MaxPossible_I = VSHUNT_MAX / RSHUNT
  // MaxPossible_I = 3.2A

  // 2. Determine max expected current
  // MaxExpected_I = 2.0A

  // 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
  // MinimumLSB = MaxExpected_I/32767
  // MinimumLSB = 0.000061              (61uA per bit)
  // MaximumLSB = MaxExpected_I/4096
  // MaximumLSB = 0,000488              (488uA per bit)

  // 4. Choose an LSB between the min and max values
  //    (Preferrably a roundish number close to MinLSB)
  // CurrentLSB = 0.0001 (100uA per bit)

  // 5. Compute the calibration register
  // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
  // Cal = 4096 (0x1000)

  ina219_calValue = 4096;

  // 6. Calculate the power LSB
  // PowerLSB = 20 * CurrentLSB
  // PowerLSB = 0.002 (2mW per bit)

  // 7. Compute the maximum current and shunt voltage values before overflow
  //
  // Max_Current = Current_LSB * 32767
  // Max_Current = 3.2767A before overflow
  //
  // If Max_Current > Max_Possible_I then
  //    Max_Current_Before_Overflow = MaxPossible_I
  // Else
  //    Max_Current_Before_Overflow = Max_Current
  // End If
  //
  // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
  // Max_ShuntVoltage = 0.32V
  //
  // If Max_ShuntVoltage >= VSHUNT_MAX
  //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
  // Else
  //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
  // End If

  // 8. Compute the Maximum Power
  // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
  // MaximumPower = 3.2 * 32V
  // MaximumPower = 102.4W

  // Set multipliers to convert raw current/power values
  ina219_currentDivider_mA = 10; // Current LSB = 100uA per bit (1000/100 = 10)
  ina219_powerMultiplier_mW = 2; // Power LSB = 1mW per bit (2/1)

  // Set Calibration register to 'Cal' calculated above
  // Adafruit_BusIO_Register calibration_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
  // calibration_reg.write(ina219_calValue, 2);

  // // Set Config register to take into account the settings above
  uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V 
                  | INA219_CONFIG_GAIN_8_320MV 
                  | INA219_CONFIG_BADCRES_12BIT 
                  | INA219_CONFIG_SADCRES_12BIT_1S_532US
                  | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  // Adafruit_BusIO_Register config_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);
  // config_reg.write(config, 2);





}

/*!
 *  @brief  Set power save mode according to parameters
 *  @param  on
 *          boolean value
 */
// void mEnergyINA219::powerSave(bool on) {
//   Adafruit_BusIO_Register config_reg =
//       Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);

//   Adafruit_BusIO_RegisterBits mode_bits =
//       Adafruit_BusIO_RegisterBits(&config_reg, 3, 0);
//   if (on) {
//     mode_bits.write(INA219_CONFIG_MODE_POWERDOWN);
//   } else {
//     mode_bits.write(INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS);
//   }
// }

/*!
 *  @brief  Configures to INA219 to be able to measure up to 32V and 1A
 *          of current.  Each unit of current corresponds to 40uA, and each
 *          unit of power corresponds to 800uW. Counter overflow occurs at
 *          1.3A.
 *  @note   These calculations assume a 0.1 ohm resistor is present
 */
void mEnergyINA219::setCalibration_32V_1A() {
  // By default we use a pretty huge range for the input voltage,
  // which probably isn't the most appropriate choice for system
  // that don't use a lot of power.  But all of the calculations
  // are shown below if you want to change the settings.  You will
  // also need to change any relevant register settings, such as
  // setting the VBUS_MAX to 16V instead of 32V, etc.

  // VBUS_MAX = 32V		(Assumes 32V, can also be set to 16V)
  // VSHUNT_MAX = 0.32	(Assumes Gain 8, 320mV, can also be 0.16, 0.08, 0.04)
  // RSHUNT = 0.1			(Resistor value in ohms)

  // 1. Determine max possible current
  // MaxPossible_I = VSHUNT_MAX / RSHUNT
  // MaxPossible_I = 3.2A

  // 2. Determine max expected current
  // MaxExpected_I = 1.0A

  // 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
  // MinimumLSB = MaxExpected_I/32767
  // MinimumLSB = 0.0000305             (30.5uA per bit)
  // MaximumLSB = MaxExpected_I/4096
  // MaximumLSB = 0.000244              (244uA per bit)

  // 4. Choose an LSB between the min and max values
  //    (Preferrably a roundish number close to MinLSB)
  // CurrentLSB = 0.0000400 (40uA per bit)

  // 5. Compute the calibration register
  // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
  // Cal = 10240 (0x2800)

  ina219_calValue = 10240;

  // 6. Calculate the power LSB
  // PowerLSB = 20 * CurrentLSB
  // PowerLSB = 0.0008 (800uW per bit)

  // 7. Compute the maximum current and shunt voltage values before overflow
  //
  // Max_Current = Current_LSB * 32767
  // Max_Current = 1.31068A before overflow
  //
  // If Max_Current > Max_Possible_I then
  //    Max_Current_Before_Overflow = MaxPossible_I
  // Else
  //    Max_Current_Before_Overflow = Max_Current
  // End If
  //
  // ... In this case, we're good though since Max_Current is less than
  // MaxPossible_I
  //
  // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
  // Max_ShuntVoltage = 0.131068V
  //
  // If Max_ShuntVoltage >= VSHUNT_MAX
  //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
  // Else
  //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
  // End If

  // 8. Compute the Maximum Power
  // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
  // MaximumPower = 1.31068 * 32V
  // MaximumPower = 41.94176W

  // Set multipliers to convert raw current/power values
  ina219_currentDivider_mA = 25;    // Current LSB = 40uA per bit (1000/40 = 25)
  ina219_powerMultiplier_mW = 0.8f; // Power LSB = 800uW per bit

  // Set Calibration register to 'Cal' calculated above
  // Adafruit_BusIO_Register calibration_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
  // calibration_reg.write(ina219_calValue, 2);

  // // Set Config register to take into account the settings above
  // uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
  //                   INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
  //                   INA219_CONFIG_SADCRES_12BIT_1S_532US |
  //                   INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
  // Adafruit_BusIO_Register config_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);
  // config_reg.write(config, 2);
}

/*!
 *  @brief set device to alibration which uses the highest precision for
 *     current measurement (0.1mA), at the expense of
 *     only supporting 16V at 400mA max.
 */
void mEnergyINA219::setCalibration_16V_400mA() {

  // Calibration which uses the highest precision for
  // current measurement (0.1mA), at the expense of
  // only supporting 16V at 400mA max.

  // VBUS_MAX = 16V
  // VSHUNT_MAX = 0.04          (Assumes Gain 1, 40mV)
  // RSHUNT = 0.1               (Resistor value in ohms)

  // 1. Determine max possible current
  // MaxPossible_I = VSHUNT_MAX / RSHUNT
  // MaxPossible_I = 0.4A

  // 2. Determine max expected current
  // MaxExpected_I = 0.4A

  // 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
  // MinimumLSB = MaxExpected_I/32767
  // MinimumLSB = 0.0000122              (12uA per bit)
  // MaximumLSB = MaxExpected_I/4096
  // MaximumLSB = 0.0000977              (98uA per bit)

  // 4. Choose an LSB between the min and max values
  //    (Preferrably a roundish number close to MinLSB)
  // CurrentLSB = 0.00005 (50uA per bit)

  // 5. Compute the calibration register
  // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
  // Cal = 8192 (0x2000)

  ina219_calValue = 8192;

  // 6. Calculate the power LSB
  // PowerLSB = 20 * CurrentLSB
  // PowerLSB = 0.001 (1mW per bit)

  // 7. Compute the maximum current and shunt voltage values before overflow
  //
  // Max_Current = Current_LSB * 32767
  // Max_Current = 1.63835A before overflow
  //
  // If Max_Current > Max_Possible_I then
  //    Max_Current_Before_Overflow = MaxPossible_I
  // Else
  //    Max_Current_Before_Overflow = Max_Current
  // End If
  //
  // Max_Current_Before_Overflow = MaxPossible_I
  // Max_Current_Before_Overflow = 0.4
  //
  // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
  // Max_ShuntVoltage = 0.04V
  //
  // If Max_ShuntVoltage >= VSHUNT_MAX
  //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
  // Else
  //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
  // End If
  //
  // Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
  // Max_ShuntVoltage_Before_Overflow = 0.04V

  // 8. Compute the Maximum Power
  // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
  // MaximumPower = 0.4 * 16V
  // MaximumPower = 6.4W

  // Set multipliers to convert raw current/power values
  ina219_currentDivider_mA = 20;    // Current LSB = 50uA per bit (1000/50 = 20)
  ina219_powerMultiplier_mW = 1.0f; // Power LSB = 1mW per bit

  // Set Calibration register to 'Cal' calculated above
  // Adafruit_BusIO_Register calibration_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CALIBRATION, 2, MSBFIRST);
  // calibration_reg.write(ina219_calValue, 2);
  // // Set Config register to take into account the settings above
  // uint16_t config = INA219_CONFIG_BVOLTAGERANGE_16V |
  //                   INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_BADCRES_12BIT |
  //                   INA219_CONFIG_SADCRES_12BIT_1S_532US |
  //                   INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

  // Adafruit_BusIO_Register config_reg =
  //     Adafruit_BusIO_Register(i2c_dev, INA219_REG_CONFIG, 2, MSBFIRST);
  // config_reg.write(config, 2);
}





#endif  // USE_I2C


#endif
