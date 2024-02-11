/*
  mPZEM004T.cpp - PZEM004T v3 MODBUS
  I will perhaps later add v1/2 into this as another type, since only the read function and address will vary

  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  https://github.com/mandulaj/PZEM-004T-v30/blob/master/src/PZEM004Tv30.cpp



*/
#include "mPZEM004T.h"

#ifdef USE_MODULE_ENERGY_PZEM004T_V3

const char* mEnergyPZEM004T::PM_MODULE_ENERGY_PZEM004T_CTR = D_MODULE_ENERGY_PZEM004T_CTR;
const char* mEnergyPZEM004T::PM_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR = D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR;

int8_t mEnergyPZEM004T::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
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
    // case FUNC_RESCAN_FOR_DEVICES:

    // break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
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

} // END function


void mEnergyPZEM004T::Pre_Init(void)
{

  if (pCONT_pins->PinUsed(GPIO_PZEM0XX_RX_MODBUS_ID) && pCONT_pins->PinUsed(GPIO_PZEM0XX_TX_ID))
  {
    pCONT_set->runtime.energy_driver = 1; // phase out // D_GROUP_MODULE_ENERGY_PZEM004T_ID; // use bit logic also
    // set bit for drivers
    settings.fEnableSensor = true;
  }

  SetIDWithAddress(0, 1);
  SetIDWithAddress(1, 2);
  SetIDWithAddress(2, 3);
  SetIDWithAddress(3, 4);
  
}


void mEnergyPZEM004T::Init(void)
{

  modbus = new TasmotaModbus(pCONT_pins->GetPin(GPIO_PZEM0XX_RX_MODBUS_ID), pCONT_pins->GetPin(GPIO_PZEM0XX_TX_ID));

  uint8_t result = modbus->Begin(9600);

  ALOG_INF(PSTR("modbus result = %d"),result);

  if (result) {
    // Change this to another function, that doesnt check pin, it just calls claimserial but internally checks if its being used
    pCONT_sup->ClaimSerial();
    /**
     * Unknown sensor count until a search is performed, them memory for readings can be allocated.
     * This needs to be done with a delayed start so PZEM MCU will always be ready when power cut happens
     * */

  } else {
    // pCONT_set->runtime.energy_driver = pCONT_iEnergy->ENERGY_MODULE_NONE_ID;
    settings.fEnableSensor = false;
    return;
  }

  settings.devices_present = 4;

  AllocateDynamicMemory();

}

/**
 * This will likely be merged into an energy type
 * */
void mEnergyPZEM004T::AllocateDynamicMemory()
{

  
  AddLog(LOG_LEVEL_TEST, PSTR( DEBUG_INSERT_PAGE_BREAK "ALLOCATE for %d sensors"), settings.devices_present);

  if(settings.devices_present<=MAX_ENERGY_SENSORS)
  {
    data_modbus = new DATA_MODBUS[settings.devices_present];

    AddLog(LOG_LEVEL_TEST, PSTR( DEBUG_INSERT_PAGE_BREAK "ALLOCATE data_modbus for %d sensors"), settings.devices_present);
  }else{

    AddLog(LOG_LEVEL_TEST, PSTR( DEBUG_INSERT_PAGE_BREAK "ERROR ALLOCATE data_modbus for %d sensors"), settings.devices_present);
    settings.devices_present = 0;
  }

  // Optional poll for all sensors?
  // // Else its default

  
}

void mEnergyPZEM004T::EveryLoop(){


  // Every second, should just be moved into every second instead of loop???
  if(mTime::TimeReachedNonReset(&measure_time,settings.rate_measure_ms))
  {
    
    // AddLog(LOG_LEVEL_TEST, PSTR( "data_modbus for %d sensors"), settings.devices_present);

    SplitTask_UpdateSensor(settings.active_sensor);
    
    if(labs(millis()-timeout)>ENERGY_PZEM004T_MEASURE_RATE_MS_TIMEOUT)
    {
      AddLog(LOG_LEVEL_DEBUG, PSTR("TRANSCEIVE_RESPONSE_TIMEOUT_ID 1"));
      transceive_mode = TRANSCEIVE_RESPONSE_TIMEOUT_ID;
      timeout = millis();
      stats.timeout_reads++;
    }

    if((transceive_mode == TRANSCEIVE_RESPONSE_SUCCESS_ID) && (settings.active_sensor == settings.devices_present-1)) // if(success) AND (last to read)
    {
      mqtthandler_sensor_ifchanged.flags.SendNow = true;
    }

    if(
      (transceive_mode == TRANSCEIVE_RESPONSE_SUCCESS_ID) ||
      (transceive_mode == TRANSCEIVE_RESPONSE_TIMEOUT_ID)
    )
    { 
      // If needed, advance to next sensor
      if(settings.active_sensor < settings.devices_present-1)
      {
        settings.active_sensor++;
      }
      else
      {
        settings.active_sensor = 0; // reset to start or remain on single sensor 
        measure_time.millis = millis();
      }

      ALOG_INF( PSTR("settings.active_sensor = %d"),settings.active_sensor);
    }

  }

} // END EveryLoop


/**
 * I need to split this to read a single sensor, thus I can have different rates
 * */
void mEnergyPZEM004T::SplitTask_UpdateSensor(uint8_t device_id)
{
  // AddLog(LOG_LEVEL_TEST, PSTR("SplitTask_UpdateSensor %d %d"), device_id, transceive_mode);

  switch(transceive_mode)
  {
    default: // Restart new reading
    case TRANSCEIVE_REQUEST_READING_ID:
    case TRANSCEIVE_RESPONSE_TIMEOUT_ID:
    case TRANSCEIVE_RESPONSE_SUCCESS_ID:
    {

      timeout = millis();

      uint16_t backoff_millis_to_wait = 10;

      #ifdef ENABLE_DEBUGFEATURE_PZEM_BACKOFF_TIME_MS
        backoff_millis_to_wait = ENABLE_DEBUGFEATURE_PZEM_BACKOFF_TIME_MS;
      #endif

      if(mTime::TimeReached(&tSaved_backoff, backoff_millis_to_wait))
      {
        stats.start_time = millis();

        uint8_t address = GetAddressWithID(device_id);

        modbus->Send(address, 0x04, 0, 10);      
        
        AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_PZEM "modbus->Send(address=%d)"), address);

        transceive_mode = TRANSCEIVE_AWAITING_RESPONSE_ID;
      }
    }
    break; // Allow follow into next mode
    case TRANSCEIVE_AWAITING_RESPONSE_ID:

      // AddLog(LOG_LEVEL_TEST, PSTR("SplitTask_UpdateSensor %d %d %d"), device_id, pCONT_iEnergy->GetAddressWithID(device_id), transceive_mode);
      // ALOG_INF( "ReceiveCount() %d", modbus->ReceiveCount());
      if(modbus->ReceiveReady())
      {

        uint8_t modbus_buffer[30] = {0};  // At least 5 + (2 * 10) = 25
        uint8_t registers = 10;
        uint8_t error = modbus->ReceiveBuffer(modbus_buffer, registers);

        ALOG_INF( "ReceiveCount()================================= %d", modbus->ReceiveCount());
        AddLog_Array(LOG_LEVEL_DEBUG, PSTR("bufferA"), modbus_buffer, (uint8_t)30);

        if(!error)
        {
          // AddLog_Array(LOG_LEVEL_DEBUG, PSTR("buffer"), modbus_buffer, (uint8_t)30);
          ALOG_DBM( "ReceiveCount() %d", modbus->ReceiveCount());
          // Check if response matches expected device
          ALOG_INF( PSTR("Read SUCCESS id=%d \tvolt=%d"), device_id, (int)data_modbus[device_id].voltage);
          ParseModbusBuffer(&data_modbus[device_id], modbus_buffer);
          stats.success_reads++;
          stats.end_time = millis();
          stats.sample_time = stats.end_time - stats.start_time;

          transceive_mode = TRANSCEIVE_RESPONSE_SUCCESS_ID;
        }
        else
        {
          AddLog(LOG_LEVEL_DEBUG, "error %d %d", modbus->ReceiveReady(), error);
          transceive_mode = TRANSCEIVE_RESPONSE_ERROR_ID;
        }
      }

    break;
  }
  
}


void mEnergyPZEM004T::ParseModbusBuffer(DATA_MODBUS* mod, uint8_t* buffer){
  //           0     1     2     3     4     5     6     7     8     9           = ModBus register
  //  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24  = Buffer index
  // 01 04 14 08 D1 00 6C 00 00 00 F4 00 00 00 26 00 00 01 F4 00 64 00 00 51 34
  // Id Cc Sz Volt- Current---- Power------ Energy----- Frequ PFact Alarm Crc--
  mod->voltage       = (float)((buffer[3]  << 8)  + buffer[4]) / 10.0;                                                  // 6553.0 V
  mod->current       = (float)((buffer[7]  << 24) + (buffer[8] << 16) + (buffer[5] << 8) + buffer[6]) / 1000.0;        // 4294967.000 A
  mod->active_power  = (float)((buffer[11] << 24) + (buffer[12] << 16) + (buffer[9] << 8) + buffer[10]) / 10.0;  // 429496729.0 W
  mod->frequency     = (float)((buffer[17] << 8)  + buffer[18]) / 10.0;                                              // 50.0 Hz
  mod->power_factor  = (float)((buffer[19] << 8)  + buffer[20]) / 100.0;  
  mod->energy        = (float)((buffer[15] << 24) + (buffer[16] << 16) + (buffer[13] << 8) + buffer[14]); 
  mod->measured_time = millis();

}

  

void mEnergyPZEM004T::SetIDWithAddress(uint8_t address_id, uint8_t address_to_save)//, uint8_t address_length)
{
  address.insert(address.begin() + address_id, address_to_save);
}

uint8_t mEnergyPZEM004T::GetAddressWithID(uint8_t address_id)//, uint8_t* address_to_get, uint8_t address_length)
{
  if(address.size())
  {
    return address[address_id];
  }

  ALOG_ERR(PSTR("Not address size"));
  return 0;

}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

// /*!
//  * PZEM004Tv30::setAddress
//  *
//  * Set a new device address and update the device
//  * WARNING - should be used to set up devices once.
//  * Code initializtion will still have old address on next run!
//  *
//  * @param[in] addr New device address 0x01-0xF7
//  *
//  * @return success
// */
// bool PZEM004Tv30::setAddress(uint8_t addr)
// {
//     if(addr < 0x01 || addr > 0xF7) // sanity check
//         return false;

//     // Write the new address to the address register
//     if(!sendCmd8(CMD_WSR, WREG_ADDR, addr, true))
//         return false;

//     _addr = addr; // If successful, update the current slave address

//     return true;
// }
// /*!
//  * PZEM004Tv30::sendCmd8
//  *
//  * Prepares the 8 byte command buffer and sends
//  *
//  * @param[in] cmd - Command to send (position 1)
//  * @param[in] rAddr - Register address (postion 2-3)
//  * @param[in] val - Register value to write (positon 4-5)
//  * @param[in] check - perform a simple read check after write
//  *
//  * @return success
// */
// bool PZEM004Tv30::sendCmd8(uint8_t cmd, uint16_t rAddr, uint16_t val, bool check, uint16_t slave_addr){
//     uint8_t sendBuffer[8]; // Send buffer
//     uint8_t respBuffer[8]; // Response buffer (only used when check is true)

//     if((slave_addr == 0xFFFF) ||
//        (slave_addr < 0x01) ||
//        (slave_addr > 0xF7)){
//         slave_addr = _addr;
//     }

//     sendBuffer[0] = slave_addr;                   // Set slave address
//     sendBuffer[1] = cmd;                     // Set command

//     sendBuffer[2] = (rAddr >> 8) & 0xFF;     // Set high byte of register address
//     sendBuffer[3] = (rAddr) & 0xFF;          // Set low byte =//=

//     sendBuffer[4] = (val >> 8) & 0xFF;       // Set high byte of register value
//     sendBuffer[5] = (val) & 0xFF;            // Set low byte =//=

//     setCRC(sendBuffer, 8);                   // Set CRC of frame

//     _serial->write(sendBuffer, 8); // send frame

//     if(check) {
//         if(!receive(respBuffer, 8)){ // if check enabled, read the response
//             return false;
//         }

//         // Check if response is same as send
//         for(uint8_t i = 0; i < 8; i++){
//             if(sendBuffer[i] != respBuffer[i])
//                 return false;
//         }
//     }
//     return true;
// }





void mEnergyPZEM004T::parse_JSONCommand(JsonParserObject obj)
{

  // AddLog(LOG_LEVEL_TEST, PSTR( DEBUG_INSERT_PAGE_BREAK "mEnergyInterface::parse_JSONCommand"));
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  
  if(jtok = obj[D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR].getObject()["SetAddress"])
  {
    // I could the first time it is sent that a random number is generated, and only when that number is sent in the next command will it be accepted. Otherwise a new number is generated..
    ALOG_WRN(PSTR("SetAddress = %d, only one device should be connected"), jtok.getInt());
    
    uint16_t addr = jtok.getInt();

    #define SLAVE_ADDRESS 0xF8 // I think this means of the esp32, not the pzem004t.

    uint8_t return_code = modbus->Send(SLAVE_ADDRESS, CMD_WSR, WREG_ADDR, 1, (uint16_t *) &addr);

    // PzemAcModbus->Send(0xF8, 0x06, 0x0002, 1, (uint16_t *) &addr);

    // Addres(1), Function(1), Start/Coil Address(2), Registercount or Data (2), CRC(2)
    
    ALOG_INF(PSTR("return_code = %d"), return_code);

    // delay(5000);


  }
  
  if(jtok = obj[D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR].getObject()["SearchForDevices"])
  {

    uint8_t address_check_maxrange = jtok.getInt();

    ALOG_INF(PSTR("address_check_maxrange = %d"), address_check_maxrange);

    uint8_t found_address = 0;
    uint8_t modbus_buffer[30] = {0}; 
    bool flag_timeout = false;

    for(
      uint8_t address_search=0;
              address_search<address_check_maxrange;
              address_search++
    ){

      modbus->Send(address_search, 0x04, 0, 10);      
      found_address = 0;
      flag_timeout = true;
      timeout = millis();
      while(abs(millis()-timeout)<100)
      {

        WDT_Reset();

        if(modbus->ReceiveReady())
        { 
          uint8_t error = modbus->ReceiveBuffer(modbus_buffer, 10);
          found_address = modbus_buffer[2]; // addres byte
          AddLog(LOG_LEVEL_INFO, PSTR("MODBUS Address = %d FOUND %d"), address_search, found_address);
          flag_timeout = false;
          break; // out of the while
        }
      
      }

      if(flag_timeout)
      {
        AddLog(LOG_LEVEL_INFO, PSTR("MODBUS Address %d: No Response"), address_search);
      }
    }

  }
   
}

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/



uint8_t mEnergyPZEM004T::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.devices_present);
  return JBI->End();

}

uint8_t mEnergyPZEM004T::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[40];

  // AddLog(LOG_LEVEL_TEST, PSTR( DEBUG_INSERT_PAGE_BREAK "mEnergyInterface::ConstructJSON_Sensor %d"), pCONT_iEnergy->Energy.phase_count);
  JBI->Start();
  for( int ii=0; ii < settings.devices_present; ii++)
  {
    JsonBuilderI->Object_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), ii, buffer, sizeof(buffer)));
      JBI->Add("Address", GetAddressWithID(ii) );
      JBI->Add(D_JSON_VOLTAGE,      data_modbus[ii].voltage);
      JBI->Add(D_JSON_CURRENT,      data_modbus[ii].current);
      JBI->Add(D_JSON_ACTIVE_POWER, data_modbus[ii].active_power);
      // JBI->Add(D_JSON_FREQUENCY,    data_modbus[ii].frequency);
      // JBI->Add(D_JSON_POWER_FACTOR, data_modbus[ii].power_factor);
      // JBI->Add(D_JSON_ENERGY,       data_modbus[ii].energy);
    JBI->Object_End();
  }

  // float error_rate_perc = (float)(stats.success_reads+stats.timeout_reads)/(float);
  float errors = stats.timeout_reads;
  float total = stats.success_reads;//+stats.timeout_reads;
  float error_rate_perc = (errors/total)*100;

  // JBI->Object_Start("stats");
  //   JBI->Add("success", stats.success_reads);
  //   JBI->Add("timeout", stats.timeout_reads);
  //   JBI->Add("error_rate_perc", error_rate_perc);
  //   JBI->Add("sample_time", stats.sample_time);
  //   JBI->Add("start_time", stats.start_time);
  //   JBI->Add("end_time", stats.end_time);
  // JBI->Object_End();

  return JBI->End();

}


  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mEnergyPZEM004T::MQTTHandler_Init(){

  struct handler<mEnergyPZEM004T>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->handler_id = MQTT_HANDLER_SETTINGS_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->handler_id = MQTT_HANDLER_SENSOR_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->handler_id = MQTT_HANDLER_SENSOR_IFCHANGED_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60;//pCONT_set->Settings.sensors.ifchanged_secs;  // until redunction in when interface is reporting
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mEnergyPZEM004T::ConstructJSON_Sensor;
  
} 

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mEnergyPZEM004T::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mEnergyPZEM004T::MQTTHandler_Set_DefaultPeriodRate()
{
  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  // }
}

/**
 * @brief Check all handlers if they require action
 * */
void mEnergyPZEM004T::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_ENERGY_PZEM004T_V3_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT
/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/




#endif
