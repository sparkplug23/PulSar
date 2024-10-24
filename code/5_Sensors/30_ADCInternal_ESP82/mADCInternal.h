/*
  mHVAC.cpp - mADCInternal

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
//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html

#ifndef mADCInternal_esp32_H
#define mADCInternal_esp32_H

#define D_UNIQUE_MODULE_SENSORS_ADC_INTERNAL_ID 5030 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP32

#include "1_TaskerManager/mTaskerManager.h"

#include "1_TaskerManager/mTaskerInterface.h"

#ifdef ESP32
#include <driver/adc.h>
#endif

// C:\Users\Michael Doone\.platformio\packages\framework-arduinoespressif32\tools\sdk\include\driver\driver
#include <vector>


/**
 * As a one off, making this global, as I need it to have fast timings inside the ISR
 * ISR measurement system recording into small buffers
 * adc will continue to write into buffer, but will not reset it, this is handled in another module (ie seriallogger)
 * Using an index_buffer, and active_buffer_to_write_to_index, this will be written into until full, then stops
 * */
// extern struct ISR_DUAL_CAPTURE isr_capture;



#include "DoubleEMAFilterLib.h"






    static void IRAM_ATTR ISR_TriggerGPIO_ADC_Record();

class mADCInternal :
  public mTaskerInterface
{
  
  public:
    mADCInternal(){};


    void Start(void (*ISR_callback)(void));
    void IRAM_ATTR button_ISR();

    uint8_t interruptPin = 12;



    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_SENSORS_ADC_INTERNAL_CTR = D_MODULE_SENSORS_ADC_INTERNAL_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_ADC_INTERNAL_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_ADC_INTERNAL_ID; }


    struct SETTINGS{
      uint8_t fEnableSensor= false;
      uint8_t fSensorCount= 0; 
      uint8_t sModuleStatus =0;
      uint16_t measure_rate_ms = 1000;
    }settings;

  

#ifdef ESP32
struct ISR_DUAL_CAPTURE{
  struct ADC_PIN1{
    uint16_t buffer_ch6[40];
    uint16_t buffer_ch7[40];
  }adc_readings[2]; //single struct, 2 buffers
  
  /**
   * One buffer will be filled, while the other is read from 
   * In seriallogger, this needs to happen because the previous recorded syncframe on the pic32 is transmitted during the next syncframe interval
   * */
  uint8_t within_buffer_iter_counter = 0;
  uint8_t active_buffer_to_write_to_index = 0;

}isr_capture;
  
// typedef enum {
//     ADC1_CHANNEL_0 = 0, /*!< ADC1 channel 0 is GPIO36 */
//     ADC1_CHANNEL_1,     /*!< ADC1 channel 1 is GPIO37 */
//     ADC1_CHANNEL_2,     /*!< ADC1 channel 2 is GPIO38 */
//     ADC1_CHANNEL_3,     /*!< ADC1 channel 3 is GPIO39 */
//     ADC1_CHANNEL_4,     /*!< ADC1 channel 4 is GPIO32 */
//     ADC1_CHANNEL_5,     /*!< ADC1 channel 5 is GPIO33 */
//     ADC1_CHANNEL_6,     /*!< ADC1 channel 6 is GPIO34 */
//     ADC1_CHANNEL_7,     /*!< ADC1 channel 7 is GPIO35 */
//     ADC1_CHANNEL_MAX,
// } adc1_channel_t;

// typedef enum {
//     ADC2_CHANNEL_0 = 0, /*!< ADC2 channel 0 is GPIO4 */
//     ADC2_CHANNEL_1,     /*!< ADC2 channel 1 is GPIO0 */
//     ADC2_CHANNEL_2,     /*!< ADC2 channel 2 is GPIO2 */
//     ADC2_CHANNEL_3,     /*!< ADC2 channel 3 is GPIO15 */
//     ADC2_CHANNEL_4,     /*!< ADC2 channel 4 is GPIO13 */
//     ADC2_CHANNEL_5,     /*!< ADC2 channel 5 is GPIO12 */
//     ADC2_CHANNEL_6,     /*!< ADC2 channel 6 is GPIO14 */
//     ADC2_CHANNEL_7,     /*!< ADC2 channel 7 is GPIO27 */
//     ADC2_CHANNEL_8,     /*!< ADC2 channel 8 is GPIO25 */
//     ADC2_CHANNEL_9,     /*!< ADC2 channel 9 is GPIO26 */
//     ADC2_CHANNEL_MAX,
// } adc2_channel_t;
  
  //When VDD_A is 3.3V:
  // 0dB attenuaton (ADC_ATTEN_0db) gives full-scale voltage 1.1V
  // 2.5dB attenuation (ADC_ATTEN_2_5db) gives full-scale voltage 1.5V
  // 6dB attenuation (ADC_ATTEN_6db) gives full-scale voltage 2.2V
  // 11dB attenuation (ADC_ATTEN_11db) gives full-scale voltage 3.9V (see note below)
  // adc1_config_width(ADC_WIDTH_BIT_12);
  // adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_2_5);

  typedef enum 
  {
    ADC_CHANNEL_GROUP_1_ID,
    ADC_CHANNEL_GROUP_2_ID,
    ADC_CHANNEL_GROUP_UNSET_ID
  }adc_channel_group;

  typedef enum
  {
    ADC_MODE_DISABLED_ID=0,
    ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID,
    ADC_MODE_LENGTH_ID,
  }adc_mode;


    uint32_t tSaved_adctest = 0;

    #define ADC_CHANNELS_MAX 2
    struct ADC_CONFIG{
      uint8_t input_pin = 0;
      adc_channel_t channel_id = ADC_CHANNEL_0;
      adc_channel_group channel_group =  ADC_CHANNEL_GROUP_UNSET_ID;    
      adc_atten_t attentuation_db_level = ADC_ATTEN_DB_11;
      adc_bits_width_t adc_width_bit = ADC_WIDTH_BIT_12;
      adc_mode mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;

      // bool flag_external_interrupt_triggered_reading = false;


      //measure_rate
      //use_tasker_interrupt_exact_readings
      //use multiple_samples per reading



    }adc_config[ADC_CHANNELS_MAX];


    bool flag_adc_capture_interrupt = false;

    struct INTERRUPT_EXTERNAL_ADC_TRIGGER{
      uint8_t flag_pin_active = false;
      uint8_t trigger_pin = 12;
      bool flag_enabled = false;

    }external_interrupt;

#endif //ESP32

    // Going to be the size of the superframe, ie 50 samples * channels (4) [3 active + guard] = 200
    #define STORED_VALUE_ADC_MAX 250
    struct READINGS{
      uint16_t adc_level = 0;
      uint8_t adcSampleCount_ = 10;
      // uint16_t max_channel_readings = 100;
      uint16_t samples_between_resets = 0;
      struct STORED_VALUES{
        uint16_t index = 0;
        // std::vector<uint16_t> 
        // static size for stability of stack
        uint16_t adc[STORED_VALUE_ADC_MAX] = {0};
      }stored_values;
    }readings[2];

    std::vector<int> samples;

    struct DoubleEMAFilter_VALUES{
      float value = 0;
      float alpha1 = 0;
      float alpha2 = 0;
      DoubleEMAFilter<float>* filter = nullptr;
    }average_DEMA;



void Update_Channel1_ADC_Readings();
    
    void AddToHardwareMessage();

    uint32_t tSavedMeasureClimate;

    void SplitTask_ReadSensor(uint8_t sensor_id, uint8_t require_completion);
    
    uint8_t fWithinLimit;
    unsigned long tWithinLimit;

    #define REQUIRE_COMPLETE true
    #define DONTREQUIRE_COMPLETE false

    #define MAX_SENSORS 1
    void EveryLoop();

    // void WebAppend_Root_Status_Table_Draw();
    // void WebAppend_Root_Status_Table_Data();
    // struct SENSORDATA{
    //   float temperature;
    //   float humidity;
    //   float pressure;
    //   float altitude;
    //   uint8_t isvalid=false;
    //   uint8_t ischanged=false;
    //   uint8_t ischanged_over_threshold=false;
    //   uint32_t ischangedtLast = millis();
    //   float heatIndex;
    //   float dewPoint;
    //   float cr;
    //   uint32_t tSavedMeasureClimate;
    //   uint8_t fWithinLimit;
    //   unsigned long tWithinLimit;
    //   uint8_t sReadSensor;
    //   // Adafruit_ADC_INTERNAL280* ADC_INTERNAL = NULL;
    // }sensor[MAX_SENSORS];

    
    uint8_t GetSensorCount(void) override
    {
      return settings.fSensorCount;
    }
    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      // Serial.printf("OVERRIDE ACCESSED DHT %d\n\r",index);Serial.println(sensor[index].instant.temperature);
      // if(index > MAX_SENSORS-1) {
      value->sensor_type.push_back(0);// return ;//}
      // value->type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
      // value->type.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
      value->data_f.push_back(0);
      // value->data.push_back(sensor[index].humidity);
      // value->sensor_id = index;
    };



        
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = false);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = false);
  
    #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    void MQTTHandler_Sender();
    
    struct handler<mADCInternal> mqtthandler_settings;
    struct handler<mADCInternal> mqtthandler_sensor_ifchanged;
    struct handler<mADCInternal> mqtthandler_sensor_teleperiod;
 
    std::vector<struct handler<mADCInternal>*> mqtthandler_list;

    // No specialised payload therefore use system default instead of enum
    
    
    #endif // USE_MODULE_NETWORK_MQTT

};
#endif

#endif


// Oh Sparkplug... That's an un-fun thing.  
// Typically what you want to do is create your "ISR" method on the class like you want to.  
// But, the problem is, you cannot attach it from within the class, you have to do so from without.  
// The other issue that I ran into is that even if you COULD use a lambda, I found that my project 
// would occasionally crash because the lambda was not "IRAM".

// Here's some snippets from my flight controller project that is creating a class that manages an 
// external I2C GPIO for my button presses.

// myclass.h
// public:
//     void Start(void (*ISR_callback)(void));
//     void IRAM_ATTR button_ISR();


// myclass.cpp
// void ButtonExternal::Start(void (*ISR_callback)(void)) {
//         pinMode(this->interruptPin, INPUT);
//         attachInterrupt(this->interruptPin, ISR_callback, FALLING);
// }


// main.cpp
//   ButtonExternal buttonExternal(0xffff, 4);

//   // Helper method that calls the internal ISR method
//   void IRAM_ATTR ISR_External_Pin_ADC_Config0_Trigger() {
//     buttonExternal.button_ISR();
//   }


//   void setup() {
//     // Start the button monitoring passing in the helper function.  You COULD make this a lambda here, BUT I found that the lambda not being "IRAM" would periodically crash the device.
//     buttonExternal.Start(ISR_External_Pin_ADC_Config0_Trigger);
//   }