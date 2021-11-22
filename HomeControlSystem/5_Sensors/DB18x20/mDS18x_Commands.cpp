
#include "mDS18X.h" 

/***
 * New joint motion triggered class, all future motion events will also trigger a response from this class (no rules required)
 * */

#ifdef USE_MODULE_SENSORS_DS18X 


void mDS18X::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  // Using a desired address, the sensor is searched for, then index (id) is updated
  if(jtok = obj[PM_JSON_SENSORADDRESS].getObject()[D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR]){
     
    JsonParserArray array_group = obj[PM_JSON_SENSORADDRESS].getObject()[D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR];
      
    #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 D_PARSING_MATCHED "%s count %d"), F(D_JSON_SENSORADDRESS),array_group.size()); 
    #endif // LOG_LEVEL_COMMANDS
    
    uint8_t address_temp[8];
    uint8_t address_index = 0;
    uint8_t original_device_id = 0;
    
    for(auto group_iter : array_group) {

      JsonParserArray array_sensor_address_iter = group_iter;
      memset(address_temp,0,sizeof(address_temp));
      address_index = 0;
            
      for(auto address_id : array_sensor_address_iter) {
        int address = address_id.getInt();
        // #ifdef ENABLE_LOG_LEVEL_DEBUG_LOWLEVEL
        //AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_DB18 "address = %d"),address); 
        // #endif
        address_temp[address_index++] = address;
        // if(address_index>7){ break; } //error!
      }

      AddLog_Array(LOG_LEVEL_COMMANDS, "address", address_temp, (uint8_t)8);
      SetIDWithAddress(original_device_id++, address_temp);
      Serial.println();

    }
    
  }



}



#endif
