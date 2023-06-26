/**
 * @file mHVAC_Sensors.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-01-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mHVAC.h"

#ifdef USE_MODULE_CONTROLLER_HVAC

/**
 * @brief Read from sensors to update HVAC sensor values
 * 
 */
void mHVAC::FunctionHandler_Update_Sensors()
{

  for(int zone_id=0; zone_id<settings.active_zones; zone_id++)
  {
    if(zone[zone_id].sensor.module_id != 0) // Only if set
    {
      sensors_reading_t reading; 
      mTaskerInterface* pMod = nullptr; 

      if((pMod = pCONT->GetModuleObjectbyUniqueID(zone[zone_id].sensor.module_id)) != nullptr)
      {
        
        pMod->GetSensorReading(&reading, zone[zone_id].sensor.index);
        
        if(reading.Valid())
        {   
          zone[zone_id].sensor.temperature = reading.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
          zone[zone_id].sensor.humidity    = reading.GetFloat(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
          // Serial.println(zone[zone_id].sensor.temperature);
          // Serial.println(zone[zone_id].sensor.humidity);
        }
      }

    }

  }

}

#endif
