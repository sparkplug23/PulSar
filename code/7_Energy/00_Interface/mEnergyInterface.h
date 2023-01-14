#ifndef _DRIVER_ENERGY_H
#define _DRIVER_ENERGY_H 0.1

#define D_UNIQUE_MODULE_ENERGY_INTERFACE_ID 130

#include "1_TaskerManager/mTaskerManager.h"

#include <vector>
// #include "2_CoreSystem/mBaseConfig.h"

// #include "2_CoreSystem/mFirmwareDefaults.h"
// #include "0_ConfigUser/mFirmwareCustom_Secret.h"
// #include "2_CoreSystem/mSystemConfig.h"

// // #define USE_MODULE_ENERGY_INTERFACE

#ifdef USE_MODULE_ENERGY_INTERFACE

// #include "1_TaskerManager/mTaskerManager.h"

#define ENABLE_PARAMETER_THRESHOLDS



DEFINE_PGM_CTR(PM_DLIM_LIST_TABLE_HEADERS)
"Voltage (V)" "|" 
"Current (A)" "|" 
"Active Power (W)" "|" 
"Frequency (Hz)" "|" 
"Power Factor" "|" 
"Energy (kWh)";


DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_ENERGY_STATS_CTR) "energystats";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_THRESHOLDLIMITS_CTR) "thresholdlimits";


#include "3_Network/01_MQTT/mMQTT.h"

class mEnergyInterface :
  public mTaskerInterface
{
  public:
    mEnergyInterface(){};
    void Init(void);
    void Pre_Init(void);
    //#define D_MODULE_TOPIC_NAME "energy"

    static const char* PM_MODULE_ENERGY_INTERFACE_CTR;
    static const char* PM_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_ENERGY_INTERFACE_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_ENERGY_INTERFACE_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){ return sizeof(mEnergyInterface); };
    #endif

    #ifndef MAX_ENERGY_SENSORS
    #define MAX_ENERGY_SENSORS 12
    #endif
    
    char* name_buffer = nullptr;//[200]; //use single 1D klist array in memory

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);//, uint8_t optional_id = 0);
    
// int8_t Tasker(uint8_t function, JsonObjectConst obj);

    void SetIDWithAddress(uint8_t address_id, uint8_t address_to_save);//, uint8_t address_length);
    uint8_t GetAddressWithID(uint8_t address_id);
    // uint8_t address[MAX_ENERGY_SENSORS][4];// = {{0},{0}};
    std::vector<uint8_t> address;
    void SetEnergyDeviceCount(uint8_t address_length);
    

    #ifdef ENABLE_PARAMETER_THRESHOLDS

    typedef union {
      uint16_t data; // allows full manipulating
      struct { 
        // Send over mqtt if value transitions ANY thresholds
        uint16_t mqtt_report_threshold_transition : 1;
        /**
         * Threshold limit units 
         * 0 - default : amps, volts, watts
         * 1 - milli (1/thousand)
         * */
        uint16_t limit_units : 1; 


      };
    } PARAMETER_THRESHOLD_FLAGS;
    
    struct PARAMETER_THRESHOLDS{
      PARAMETER_THRESHOLD_FLAGS flags;
      struct VOLTAGE{
        float upper_limit;
        float lower_limit;
        uint16_t over_limit_seconds_counter;
        uint16_t over_limit_seconds_trigger_value;
        uint8_t over_limit_seconds_trigger_value_exceeded = false;
      }voltage;
      struct CURRENT{
        float upper_limit;
        float lower_limit;
        uint16_t over_limit_seconds_counter;
        uint16_t over_limit_seconds_trigger_value;
        uint8_t over_limit_seconds_trigger_value_exceeded = false;
      }current;
      struct ACTIVE_POWER{
        float upper_limit;
        float lower_limit;
        uint16_t over_limit_seconds_counter;
        uint16_t over_limit_seconds_trigger_value;
        uint8_t over_limit_seconds_trigger_value_exceeded = false;
      }active_power;
      struct FREQUENCY{
        float upper_limit;
        float lower_limit;
        uint16_t over_limit_seconds_counter;
        uint16_t over_limit_seconds_trigger_value;
        uint8_t over_limit_seconds_trigger_value_exceeded = false;
      }frequency;
      struct POWER_FACTOR{
        float upper_limit;
        float lower_limit;
        uint16_t over_limit_seconds_counter;
        uint16_t over_limit_seconds_trigger_value;
        uint8_t over_limit_seconds_trigger_value_exceeded = false;
      }power_factor;
      struct ENERGY{
        float upper_limit;
        float lower_limit;
        uint16_t over_limit_seconds_counter;
        uint16_t over_limit_seconds_trigger_value;
        uint8_t over_limit_seconds_trigger_value_exceeded = false;
      }energy;
    }parameter_thresholds[MAX_ENERGY_SENSORS];

    void UpdateThresholdLimits();


    #endif
    

    void WebCommand_Parse();
    
void WebAppend_Root_Draw_Table();
void WebAppend_Root_Status_Table();
    
    //phase out, in headers
    enum ENERGY_MODULE_ID{
      ENERGY_MODULE_NONE_ID = 0,
      // ENERGY_MODULE_PZEM004TV3_ID=5,
      // EM_ENERGY_MODULE_ D_GROUP_MODULE_ENERGY_ADE7953_ID
    };

    
void Settings_Load();
void Settings_Default();
void Settings_Save();

    //#define ENERGY_NONE            0
    #define ENERGY_WATCHDOG        4        // Allow up to 4 seconds before deciding no valid data presen

    #define D_JSON_POWERCAL "PowerCal"
    #define D_JSON_VOLTAGECAL "VoltageCal"
    #define D_JSON_CURRENTCAL "CurrentCal"
    #define D_JSON_TARIFF "Tariff"
    #define D_JSON_MODULEADDRESS "ModuleAddress"

    struct ENERGY {

      //move these into their own struct, so I can set the struct of them to 0
      float voltage[MAX_ENERGY_SENSORS];// = { 0, 0, 0, 0 };               // 123.1 V
      float current[MAX_ENERGY_SENSORS];// = { 0, 0, 0 };               // 123.123 A
      float active_power[MAX_ENERGY_SENSORS];// = { 0, 0, 0 };          // 123.1 W
      float apparent_power[MAX_ENERGY_SENSORS];// = { NAN, NAN, NAN };  // 123.1 VA
      float reactive_power[MAX_ENERGY_SENSORS];// = { NAN, NAN, NAN };  // 123.1 VAr
      float power_factor[MAX_ENERGY_SENSORS];// = { NAN, NAN, NAN };    // 0.12
      float frequency[MAX_ENERGY_SENSORS];// = { NAN, NAN, NAN };       // 123.1 Hz
      float energy_read[MAX_ENERGY_SENSORS];// = { NAN, NAN, NAN };       // 123.1 Hz

      float start_energy = 0;                       // 12345.12345 kWh total previous
      float daily = 0;                              // 123.123 kWh
      float total = 0;                              // 12345.12345 kWh total energy
      float export_active = NAN;                    // 123.123 KWh

      unsigned long kWhtoday_delta = 0;             // 1212312345 Wh 10^-5 (deca micro Watt hours) - Overflows to Energy.kWhtoday (HLW and CSE only)
      unsigned long kWhtoday_offset = 0;            // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily
      unsigned long kWhtoday;                       // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily
      unsigned long period = 0;                     // 12312312 Wh * 10^-2 (deca milli Watt hours) - 5764 = 0.05764 kWh = 0.058 kWh = Energy.daily

      uint8_t fifth_second = 0;
      uint8_t command_code = 0;
      uint8_t data_valid[MAX_ENERGY_SENSORS];// = { 0, 0, 0 };

      uint8_t phase_count = 0;                      // Number of phases active
      bool voltage_common = false;                  // Use single voltage and frequency

      bool voltage_available = true;                // Enable if voltage is measured
      bool current_available = true;                // Enable if current is measured

      bool type_dc = false;
      bool power_on = true;

      // #ifdef USE_ENERGY_MARGIN_DETECTION
        uint16_t power_history[MAX_ENERGY_SENSORS];// = { 0 };
        uint8_t power_steady_counter = 8;  // Allow for power on stabilization
        bool power_delta = false;
        bool min_power_flag = false;
        bool max_power_flag = false;
        bool min_voltage_flag = false;
        bool max_voltage_flag = false;
        bool min_current_flag = false;
        bool max_current_flag = false;

        // #ifdef USE_ENERGY_POWER_LIMIT
          uint16_t mplh_counter = 0;
          uint16_t mplw_counter = 0;
          uint8_t mplr_counter = 0;
          uint8_t max_energy_state  = 0;
        // #endif  // USE_ENERGY_POWER_LIMIT
      // #endif  // USE_ENERGY_MARGIN_DETECTION

      
      float energy = 0; //pzem
      float last_energy = 0; //pzem

      struct STATS{
        float kwh_per_minute[60];
        uint8_t kwh_per_minute_index = 0;
        float kwh_each_minute[60];
        float last_minutes_energy = 0;
        float current_energy = 0;
        float this_minutes_energy = 0;
      }stats;

    } Energy;

  
    void UpdateEnergyUsagePerMinute();
    bool EnergyMargin(bool type, uint16_t margin, uint16_t value, bool &flag);//, bool &save_flag);
    void EnergyMarginCheck(void);
    uint32_t tSavedLoop = millis();
    Ticker ticker_energy;

  void EnergySnsInit(void);


  // const char* EnergyFormatIndex(char* result, char* input, bool json, uint32_t index, bool single = false);
  // const char* EnergyFormat(char* result, char* input, bool json, bool single = false);
  // void EnergyShow(bool json);

      
  /********************************************************************************************/

  bool EnergyTariff1Active();
  void EnergyUpdateToday(void);
  void EnergyUpdateTotal(float value, bool kwh);

  /*********************************************************************************************/

  void Energy200ms(void);
  void EnergySaveState(void);
  #ifdef USE_ENERGY_MARGIN_DETECTION
  bool EnergyMargin(bool type, uint16_t margin, uint16_t value, bool &flag, bool &save_flag);
  void EnergyMarginCheck(void);
  void EnergyMqttShow(void);
  #endif
  void EnergyEverySecond(void);
  void EnergyCommandCalResponse(uint32_t nvalue);
  
  void parse_JSONCommand(JsonParserObject obj);

  // Commands
  void CmndEnergyReset(void);
  void CmndTariff(void);
  void CmndPowerCal(void);
  void CmndVoltageCal(void);
  void CmndCurrentCal(void);
  void CmndPowerSet(void);
  void CmndVoltageSet(void);
  void CmndCurrentSet(void);
  void CmndFrequencySet(void);
  void CmndModuleAddress(void);
  //#ifdef USE_ENERGY_MARGIN_DETECTION
    void CmndPowerDelta(void);
    void CmndPowerLow(void);
    void CmndPowerHigh(void);
    void CmndVoltageLow(void);
    void CmndVoltageHigh(void);
    void CmndCurrentLow(void);
    void CmndCurrentHigh(void);
    //  #ifdef USE_ENERGY_POWER_LIMIT
      void CmndMaxPower(void);
      void CmndMaxPowerHold(void);
      void CmndMaxPowerWindow(void);
      void CmndSafePower(void);
      void CmndSafePowerHold(void);
      void CmndSafePowerWindow(void);
      void CmndMaxEnergy(void);
      void CmndMaxEnergyStart(void);
  //     #endif  // USE_ENERGY_POWER_LIMIT
  // #endif  // USE_ENERGY_MARGIN_DETECTION

void CommandSet_MQTT_Reporting_Priority(uint8_t value);
  void parse_JSONCommand();
  void parse_JSONCommand_BootSafe(JsonParserObject obj);


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

// const char kEnergyPhases[] PROGMEM = "|%s / %s|%s / %s / %s||[%s,%s]|[%s,%s,%s]";

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_object_start_end_required = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_object_start_end_required = true);
    uint8_t ConstructJSON_EnergyStats(uint8_t json_level = 0, bool json_object_start_end_required = true);
    uint8_t ConstructJSON_ThresholdLimits(uint8_t json_level, bool json_object_start_end_required);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    struct handler<mEnergyInterface> mqtthandler_settings_teleperiod;
    struct handler<mEnergyInterface> mqtthandler_sensor_ifchanged;
    struct handler<mEnergyInterface> mqtthandler_sensor_teleperiod;
    struct handler<mEnergyInterface> mqtthandler_thresholdlimits_ifchanged; //will contain alert status
    struct handler<mEnergyInterface> mqtthandler_thresholdlimits_teleperiod;
    struct handler<mEnergyInterface> mqtthandler_energystats_ifchanged;
    struct handler<mEnergyInterface> mqtthandler_energystats_teleperiod;
    
    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_ENERGYSTATS_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_ENERGYSTATS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_THRESHOLDLIMITS_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_THRESHOLDLIMITS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };
    
    struct handler<mEnergyInterface>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
      // ,
      // &mqtthandler_energystats_ifchanged,  
      // &mqtthandler_energystats_teleperiod
      // ,  
      // &mqtthandler_thresholdlimits_ifchanged,  
      // &mqtthandler_thresholdlimits_teleperiod  
    };


  //#endif


};
#endif

#endif
