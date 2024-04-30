#ifndef MSOLARLUNAR_H
#define MSOLARLUNAR_H

#define D_UNIQUE_MODULE_SENSORS_SOLAR_LUNAR_ID ((5*1000)+22)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_SOLAR_LUNAR
#include "3_Network/10_MQTT/mMQTT.h"

#include <cmath> 
/* fmod example */
#include <stdio.h>      /* printf */
#include <math.h>       /* fmod */
  
#ifndef M_PI
#define M_PI (3.14159265358979323846264338327950288)
#endif /* M_PI */

#include "1_TaskerManager/mTaskerInterface.h"

class mSolarLunar :
  public mTaskerInterface
{
  
  private:
  /* data */
  public:
    mSolarLunar(/* args */){};

  
    void Init(void);
    void Pre_Init();
    
    int8_t Tasker(uint8_t function, JsonParserObject obj);
    
    static const char* PM_MODULE_SENSORS_SOLAR_LUNAR_CTR;
    static const char* PM_MODULE_SENSORS_SOLAR_LUNAR_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_SOLAR_LUNAR_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_SOLAR_LUNAR_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_SOLAR_LUNAR_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mSolarLunar);    };
    #endif

    
    struct SETTINGS{
      uint8_t  fEnableSensor = false;
      // uint8_t  nSensorsFound = 0; // count of sensors found    n:number found, c:case number for switches
      // uint16_t rate_measure_ms = 1000;
      // uint8_t  group_count = 0;
    }settings;

    void parse_JSONCommand(JsonParserObject obj);

    void Update_Solar_Tracking_Data();

    

    
		/**
		 * @brief 
		 * 
		 */
		enum SOLAR_POSITION_DESCRIPTION{
			SOLAR_POSITION_DAYTIME_RISING_ID, // before noon
			SOLAR_POSITION_DAYTIME_FALLING_ID, //after noon but still daylight
			SOLAR_POSITION_DAYTIME_SUNSET_ID,
			SOLAR_POSITION_DAYTIME_DUSK_ID, // sun below horizon, but still brightness (E< -5), ie, no orange left, deepening blue
			SOLAR_POSITION_DAYTIME_NIGHT_ID,
			SOLAR_POSITION_DAYTIME_DAWN_ID, // E>-10 and E<-5, ie, no "orange" in the sky but its a lighter blue
			SOLAR_POSITION_DAYTIME_SUNRISE_ID, // E>-5 and E<5
			SOLAR_POSITION_DAYTIME_LENGTH_ID
		};

    struct solar_position_t
    {
      double azimuth = 0;
      double elevation = 0;
      double elevation_min = -29;
      double elevation_max = 40;
      struct DIRECTION_OF_CHANGE{
        uint8_t is_ascending = true;
      }direction;
      uint32_t tUpdated_millis = 0;
      bool isvalid = false;

    }solar_position;

    struct DEBUG
    {
      bool enabled = false;
      double azimuth = 0;
      double elevation = 0;
    }debug;

    double GetAzimuth()
    {
      #ifdef ENABLE_DEBUGFEATURE__SENSOR_SOLARLUNAR
        if(debug.enabled)
          return debug.azimuth;
        else
          return solar_position.azimuth;
      #else
        return solar_position.azimuth;
      #endif
    }

    double GetElevation()
    {
      #ifdef ENABLE_DEBUGFEATURE__SENSOR_SOLARLUNAR
        if(debug.enabled)
          return debug.elevation;
        else
          return solar_position.elevation;
      #else
        return solar_position.elevation;
      #endif
    }




    solar_position_t solar_position_testing;

    float elevation_adjusted_for_debugging = 0;


    void SolarAzEl(time_t utc_time_point, double Lat, double Lon, double Alt, double* Az, double* El);

    double julian_day(time_t utc_time_point);


    uint8_t GetSensorCount(void) override
    {
      return 1;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      value->sensor_type.push_back(SENSOR_TYPE_SUN_AZIMUTH_ID);
      value->data_f.push_back(solar_position.azimuth);
      value->sensor_type.push_back(SENSOR_TYPE_SUN_ELEVATION_ID);
      value->data_f.push_back(solar_position.elevation);
      value->sensor_id = index;
    };
    


  
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender();
    
    struct handler<mSolarLunar> mqtthandler_settings_teleperiod;
    struct handler<mSolarLunar> mqtthandler_sensor_ifchanged;
    struct handler<mSolarLunar> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    

    std::vector<struct handler<mSolarLunar>*> mqtthandler_list;    
    #endif // USE_MODULE_NETWORK_MQTT 


};

#endif // USE_MODULE_SENSORS_SOLAR_LUNAR

#endif // HEADER GUARD
