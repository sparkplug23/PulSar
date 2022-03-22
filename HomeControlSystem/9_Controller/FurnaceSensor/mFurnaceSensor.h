#ifndef _MFURNACE_SENSOR_LEVEL_H
#define _MFURNACE_SENSOR_LEVEL_H

#define D_UNIQUE_MODULE_CONTROLLER_FURNACE_SENSOR_ID 175

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_FURNACE_SENSOR

// #include <driver/adc.h>

#include <Arduino.h>

class mFurnaceSensor :
  public mTaskerInterface
{
  public:
    mFurnaceSensor(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Init(void);
  void Pre_Init(void);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t fSensorCount= 0; 
    }settings;
    
    void EveryLoop();

    // struct STATE{
    //   bool iswithin_temperature_limit = false;
    //   float threshold_maximum_temperature = 100;
    //   float threshold_minimum_temperature = 30;
    //   float temperature_current = -1;
    // }state;
    
    // typedef enum 
    // {
    //   ADC_CHANNEL_GROUP_1_ID,
    //   ADC_CHANNEL_GROUP_2_ID,
    //   ADC_CHANNEL_GROUP_UNSET_ID
    // }adc_channel_group;

    // typedef enum
    // {
    //   ADC_MODE_DISABLED_ID=0,
    //   ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID,
    //   ADC_MODE_LENGTH_ID,
    // }adc_mode;

    // #define ADC_CHANNELS_MAX 2
    // struct ADC_CONFIG{
    //   uint8_t input_pin = 0;
    //   adc_channel_t channel_id = ADC_CHANNEL_0;
    //   adc_channel_group channel_group =  ADC_CHANNEL_GROUP_UNSET_ID;    
    //   adc_atten_t attentuation_db_level = ADC_ATTEN_DB_11;
    //   adc_bits_width_t adc_width_bit = ADC_WIDTH_BIT_12;
    //   adc_mode mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;
    // }adc_config[ADC_CHANNELS_MAX];

    // struct ADCVALUES{
    //     uint16_t adc1  = 0;
    //     float litres  = 0;
    //     bool updated = false;
    // }adc_values;
    //     uint16_t adc_upper_boundary_index = 0;

    // const uint16_t adc_raw_calibration_pairs_readings_adc[17] =
    // {
    //   /**
    //    * ADC Level, Water (ml)
    //    * */
    //   4095,
    //   3112,
    //   2431,
    //   2152,
    //   1925,
    //   1762,
    //   1642,
    //   1580,
    //   1500,
    //   1405,
    //   1351,
    //   1283,
    //   1241,
    //   1203,
    //   1140,
    //   1090,
    //   1000,
    // };

    // const uint16_t adc_raw_calibration_pairs_readings_milliletres[17] =
    // {
    //   /**
    //    * ADC Level, Water (ml)
    //    * */
    //   300,
    //   500,
    //   600,
    //   700,
    //   800,
    //   900,
    //   1000,
    //   1100,
    //   1200,
    //   1300,
    //   1400,
    //   1500,
    //   1600,
    //   1700,
    //   1800,
    //   2000,
    //   2200
    // };


    static const char* PM_MODULE_CONTROLLER_FURNACE_SENSOR_CTR;
    static const char* PM_MODULE_CONTROLLER_FURNACE_SENSOR_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_FURNACE_SENSOR_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_FURNACE_SENSOR_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_FURNACE_SENSOR_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mFurnaceSensor);
    };
    #endif
    
    void SubTasker_MQTTSender();

    void   MeasureADCWithRelay();

    struct DATA{
      uint16_t level = 0;

    }data;

    void parse_JSONCommand(JsonParserObject obj);
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_State(uint8_t json_method = 0);
  
    #ifdef USE_MODULE_NETWORK_MQTT

      void MQTTHandler_Init();
      void MQTTHandler_Set_RefreshAll();
      void MQTTHandler_Set_TelePeriod();
      
      void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

      struct handler<mFurnaceSensor> mqtthandler_settings_teleperiod;
      struct handler<mFurnaceSensor> mqtthandler_state_ifchanged;
      struct handler<mFurnaceSensor> mqtthandler_state_teleperiod;

      // No specialised payload therefore use system default instead of enum
      const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
      
      struct handler<mFurnaceSensor>* mqtthandler_list[3] = {
        &mqtthandler_settings_teleperiod,
        &mqtthandler_state_ifchanged,
        &mqtthandler_state_teleperiod
      };

    #endif // USE_MODULE_NETWORK_MQTT

};

#endif //DEVICE_FURNACE_SENSOR_LEVEL

#endif



/**
 * This is a basic HVAC, that could technically be performed by rules only
 * 
 * This is basically a usermod_radiatordfan
 * 
 * 
 * */

// Rename to something generic

// Description: Temperature controlled relays
// Use example: Temp probe that measures radiator temperature and controls fan to assist in heat disspersion

// This is the equivalent of a simple rule, temp drops below threshold, relay off, temp goes above, relay on

// "mTemperature"

// can this be described as hvac? temp drops, relay on, though, its continuous and no shcedules (too complex for its need)

// mTemperatureControlledRelays()



// What this could be merged into: sensor controlled, threshold high and low (hysteresis), output (any driver)
// Actions/Rule

// Relay mode to use temp sensor to control state
