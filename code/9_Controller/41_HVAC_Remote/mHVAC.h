/*
  mHeating_Commands.cpp - Home Heating Controller

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
*/
#ifndef MHEATING2_H
#define MHEATING2_H

#define D_UNIQUE_MODULE_CONTROLLER_HVAC_ID   131  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_CONTROLLER_HVAC_ID    1    // Numerical accesending order of module within a group

/**
 * Moving forward, try making this heating work for all systems. "Heating" as a controller should accept any sensor, with any relays out
 * I will use this with one sensor and one relay to controller a room electric heater, this will be my development testbed
 * 
 * Create "Zones". Each will have its own temp, timers etc attached to it
 * Create heating methods as its own class
 * 
 * 
 * */
#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE

#include "9_Controller/40_HVAC/mProgramTimer.h"
class ProgramTimer;
#ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
// Disabling program temperatures until ready to integrate properly and stable timers
#include "9_Controller/40_HVAC/mProgramTemperatue.h"
class ProgramTemperature;
#endif // ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

#include <NeoPixelBus.h>


#ifndef HEATING_DEVICE_MAX
#define HEATING_DEVICE_MAX 4
#endif

#include <stdio.h>
#include <stdlib.h>

#include "2_CoreSystem/02_Time/mTime.h"

#include "2_CoreSystem/05_Logging/mLogging.h"


#define USE_FAILSAFES
#define USE_HVAC_TIMERS
#define USE_HVAC_TEMPS


//disable  for debugging
#define ENABLE_RELAY_CONTROLS

#define D_HVAC_PROGRAM_TEMP "Program Temp > "


DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR) "program/timers";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR) "program/temps";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR) "program/overview";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR) "sensors/zone";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_ZONE_COLOUR_CTR) "sensors/zone/colours";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR) "relays";

#define D_TEMP_MODE_OFF_CTR "Mode Off"

DEFINE_PGM_CTR(PM_TEMP_MODE_OFF_CTR)  D_TEMP_MODE_OFF_CTR;

DEFINE_PGM_CTR(PM_HVAC_SCHEDULED_NAME_OFF_CTR)            D_JSON_OFF;
DEFINE_PGM_CTR(PM_HVAC_SCHEDULED_NAME_SET_CTR)            D_JSON_SCHEDULED_SET;
DEFINE_PGM_CTR(PM_HVAC_SCHEDULED_NAME_ON_CTR)             D_JSON_SCHEDULED_ON;
DEFINE_PGM_CTR(PM_HVAC_SCHEDULED_NAME_MANUAL_ON_CTR)      D_JSON_MANUAL_ON;



enum SCH_MODE_IDS{
  SCHEDULED_OFF_ID=1,
  SCHEDULED_SET_ID,
  SCHEDULED_ON_ID,
  SCHEDULED_MANUAL_ON_ID
};

/**
 * Instead of numerical IDs, BITS_PACKED allows exclusive bit OR
 * */
enum ZONE_MODES_8BITS_PACKED{
  ZONE_MODE_DISABLED_8BITS=0, // no outputs set
  ZONE_MODE_HEAT_8BITS=1,
  ZONE_MODE_COOL_8BITS=2,
  ZONE_MODE_LENGTH_8BITS=128
};


class mHVAC :
  public mTaskerInterface
{

  private:
  public:
    mHVAC(){};
    
    static const char* PM_MODULE_CONTROLLER_HVAC_CTR;
    static const char* PM_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_HVAC_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_HVAC_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mHVAC);
    };
    #endif

    // Leave as single zone for now, scale to array

    struct Zone{
      #ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
      ProgramTemperature* program_temp_method = nullptr;   // Change to vectors later
      #endif
      ProgramTimer*       program_timer_method = nullptr;  // Change to vectors later
      /**
       * @brief active mode set, heating, cool, off
       * */
      uint8_t active_mode = 0;
      /**
       * @brief modes that are allowed.. maybe contain by driver type?
       * bit0 - heating
       * bit1 - cool
       * bit2 - ?
       * */
      uint8_t bitpacked_modes_enabled = 0x00;

      struct SENSOR_STATE
      {
        //Add ROC here?
        float temperature = 0; // delete these? inside programtemperature, why twice?
        float humidity = 0;
        /**
         * @brief This is the sensor module id (ie dht, db18s20, bme) so it knows which class to ask for temp readings 
         * */
        int16_t module_id = -1; // -1 not set
        /**
         * @brief The index within the module I am using. This will need to be found using the unique name
         * */
        int8_t index = -1; // -1 not set
      }sensor;
      struct DRIVER_STATE
      {
        /**
         * @brief Store multiple modules (Drivers) incase the devices are not the same type (eg, Relays, PWM, iFans03)
         * */
        std::vector<uint16_t> module_ids;
        /**
         * @brief Store multiple indexes if more than one driver is needed per zone (ie, relay for heating and another for cooling)
         * */
        std::vector<uint8_t> index;
        /**
         * @brief Driver type, what the thing attempts to control not what it is (ie, Relay can control lights, heaters, coolers etc, so I want to know what the purpose of the relay is)
         * */
        std::vector<uint8_t> driver_type;
      }output;
    }zone[HEATING_DEVICE_MAX];


    void Pre_Init(void);
    void init(void);    
    
    struct SETTINGS{
      uint8_t  fEnableModule = true;
      uint8_t active_zones = 0;
    }settings;

    void init_program_timers();
    void init_program_temps();
    void init_program_scheduling(void);

    void Web_Append_Program_Timers_Buttons();
    void Web_Append_Program_Temps_Buttons();

    void WebPage_Root_SendBody();
    void WebSendBody_Liveview();

    void WebAppend_Root_Draw_Program_Buttons();
    void WebAppend_Root_Draw_PageTable();

    struct HEATING_STATUS{
      char message_ctr[100];
      char message_len = 0;
    }heating_status[HEATING_DEVICE_MAX];


    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebPage_Root_AddHandlers();
    void WebAppend_Root_Status_Table();
    void Web_Root_Draw(AsyncWebServerRequest *request);
    #endif // USE_MODULE_NETWORK_WEBSERVER


    void HandleTimerConfiguration(void);
    void HandleProgramTimerConfiguration();

    void Every_Second();

    int mapHeatingTempToHueColour(int val);
    int mapHeatingTempToBrightness(int val);
    

    // *************** RELAYS ****************************************************************************************************

    enum RELAY_STATES_ON{RELAY_STATE_OFF=0,RELAY_STATE_ON};
    void SetHeatingRelay(uint8_t device_id, uint8_t state);
    uint8_t GetHeatingRelay(uint8_t device_id);
    uint8_t GetAnyHeatingRelay();

    uint8_t ConstructJSON_HeatingRelays(uint8_t json_level = 0);
    
    
    const char* GetActiveProgramNameCtrbyID(uint8_t activeprogram_id, char* buffer, uint8_t buflen);

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    void parse_JSONCommand(JsonParserObject obj);
    
    void CommandSet_ProgramTimer_TimeOn(uint8_t zone_id, uint8_t value);

    #ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
    void CommandSet_ProgramTemperature_Desired_Temperature(uint8_t zone_id, float value);
    void CommandSet_ProgramTemperature_Schedule_Mode(uint8_t zone_id, int8_t value);
    void CommandSet_ProgramTemperature_TimeRunning_Limit(uint8_t zone_id, uint8_t value); 
    void CommandSet_ProgramTemperature_Mode(uint8_t zone_id, uint8_t mode);
    #endif // ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

    void SubTasker_HeatingTimers(void);
    void SubTasker_HeatingTemps(void);
    void SubTasker_ScheduledEvents(void);

    const char* GetScheduleNameCtrbyID(uint8_t mode, char* buffer, uint8_t buflen);
    const char* GetDeviceNameLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buflen);
    const char* GetDeviceNameUpperCaseCtrbyID(uint8_t device_id, char* buffer, uint8_t buflen);

    void SetHighestImportance(uint8_t* importanceset, int8_t thisvalue);

    void SetHeater(uint8_t device_name, uint8_t state);

    /**
     * This will trigger a zone to be set as active, and internally will decide if that should heat or cool depending on zone mode
     * */
    void SetZoneActive(uint8_t zone_id, uint8_t state = 0);

    int8_t Tasker_PredictManualHeating();

    #define FAILSAFE_MINUTES_WARNING 60*5
    #define FAILSAFE_MINUTES_ERROR 60*10

    uint8_t rateSavedSendActive = 1;

    const char* GetTempActiveProgramByDeviceIDCtr(uint8_t device, char* buffer, uint8_t buflen);
    
    int8_t GetScheduleModeIDByCtr(const char* c);

    struct FAILSAFES{
      // uint32_t tSaved;
      int tSavedMinuteFailSafe;
    }failsafes;

    const char* GetSensorNameByID(uint8_t sensor_id, char* buffer, uint8_t buflen);
    const char* GetSensorNameLongbyID(uint8_t sensor_id, char* buffer, uint8_t buflen);

    uint8_t ConstructJSON_ProgramTimers(uint8_t json_level = 0);
    uint8_t ConstructJSON_ProgramTemps(uint8_t json_level = 0);

    uint8_t ConstructJSON_ZoneSensors(uint8_t json_level = 0);
    uint8_t ConstructJSON_ZoneSensors_ROC1m(uint8_t json_level = 0);
    uint8_t ConstructJSON_ZoneSensors_ROC10m(uint8_t json_level = 0);

    uint8_t ConstructJSON_ProgramActive(uint8_t json_level = 0);
    uint8_t ConstructJSON_HardwareInfo(uint8_t json_level = 0);

    uint32_t tSavedSendRateOfChange10s;

    enum VOICEREQUEST_ID{VOICEREQUEST_WATERTEMP_ID=1,VOICEREQUEST_CLIMATETEMP_ID,VOICEREQUEST_SCHEDULE_ID,VOICEREQUEST_OVERVIEW_ID};
    int8_t GetVoiceRequestIDbyName(const char* c);

    int8_t GetClimateSensorIDbyShortName(const char* c);
    const char* GetClimateSensorNameLongbyIDCtr(uint8_t sensor_id, char* buffer, uint8_t buflen);
    float GetClimateTempsRawByID(uint8_t sensor_id);

    uint8_t ConstructJSON_Settings(uint8_t json_level, bool json_object_start_end_required);


    void FunctionHandler_Init();
    void FunctionHandler_Loop();
  

    struct functionhandler<mHVAC> functionhandler_status_message;
    void FunctionHandler_Program_Status();
    struct functionhandler<mHVAC> functionhandler_failsafe;
    void FunctionHandler_FailSafe(void);
    struct functionhandler<mHVAC> functionhandler_hvac_profiles;
    void FunctionHandler_HVAC_Profiles(void);
    struct functionhandler<mHVAC> functionhandler_relay_status;
    void FunctionHandler_Relay_Status(void);
    struct functionhandler<mHVAC> functionhandler_programs_timers;
    void FunctionHandler_Programs_Timers(void);
    #ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
    struct functionhandler<mHVAC> functionhandler_programs_temps;
    void FunctionHandler_Programs_Temps(void);
    #endif // ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
    struct functionhandler<mHVAC> functionhandler_update_sensors;
    void FunctionHandler_Update_Sensors(void);

    struct functionhandler<mHVAC>* functionhandler_list[
      5
      #ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
      +1
      #endif    
      #ifdef USE_HVAC_PROFILE_ESTIMATION
      +1
      #endif // USE_HVAC_PROFILE_ESTIMATION    
    ] = 
    {
      &functionhandler_failsafe,
      &functionhandler_status_message,
      #ifdef USE_HVAC_PROFILE_ESTIMATION
      &functionhandler_hvac_profiles,
      #endif //#ifdef USE_HVAC_PROFILE_ESTIMATION
      &functionhandler_relay_status,
      &functionhandler_programs_timers,
      #ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
      &functionhandler_programs_temps,
      #endif 
      &functionhandler_update_sensors
    };
    

  
  #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_PROGRAM_TIMERS_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_PROGRAM_TIMERS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_PROGRAM_TEMPS_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_PROGRAM_TEMPS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_SENSOR_ZONE_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_SENSOR_ZONE_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_SENSOR_ZONE_ROC1M_ID,
      MQTT_HANDLER_MODULE_SENSOR_ZONE_ROC10M_ID,
      MQTT_HANDLER_MODULE_DRIVERS_RELAYS_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_DRIVERS_RELAYS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };

    struct handler<mHVAC> mqtthandler_settings_teleperiod;
    struct handler<mHVAC> mqtthandler_program_timers_ifchanged;
    struct handler<mHVAC> mqtthandler_program_timers_teleperiod;
    struct handler<mHVAC> mqtthandler_program_temps_ifchanged;
    struct handler<mHVAC> mqtthandler_program_temps_teleperiod;
    struct handler<mHVAC> mqtthandler_program_overview_ifchanged;
    struct handler<mHVAC> mqtthandler_program_overview_teleperiod;
    struct handler<mHVAC> mqtthandler_sensor_zone_ifchanged;
    struct handler<mHVAC> mqtthandler_sensor_zone_teleperiod;
    struct handler<mHVAC> mqtthandler_sensor_zone_roc1m;
    struct handler<mHVAC> mqtthandler_sensor_zone_roc10m;
    struct handler<mHVAC> mqtthandler_relays_ifchanged;
    struct handler<mHVAC> mqtthandler_relays_teleperiod;
  
  struct handler<mHVAC>* mqtthandler_list[13] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_program_timers_ifchanged,
    &mqtthandler_program_timers_teleperiod,
    &mqtthandler_program_temps_ifchanged,
    &mqtthandler_program_temps_teleperiod,
    &mqtthandler_program_overview_ifchanged,
    &mqtthandler_program_overview_teleperiod,
    &mqtthandler_sensor_zone_ifchanged,
    &mqtthandler_sensor_zone_teleperiod,
    &mqtthandler_sensor_zone_roc1m,
    &mqtthandler_sensor_zone_roc10m,
    &mqtthandler_relays_ifchanged,
    &mqtthandler_relays_teleperiod
  };

  #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
