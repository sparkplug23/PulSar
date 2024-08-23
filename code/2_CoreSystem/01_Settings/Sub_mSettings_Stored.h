#pragma once

#include <stdint.h>
#include "2_CoreSystem/04_HardwareTemplates/mHardwareTemplates.h"



#define MAX_PWMS 5       



struct Template_Config{
  uint8_t flags;
  uint8_t base;       
  mytmplt hardware;   
};


struct SystemName{ 
  char          friendly[50]; // Used in titles, set by templates "FriendlyName"
  char          device[50];   // USed as mqtt topic by default, set by templates "Name"
};


#define PARAM8_SIZE 18            // Number of param bytes (SetOption)
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint32_t save_state : 1;               // bit 0              - SetOption0  - Save power state and use after restart
    uint32_t button_restrict : 1;          // bit 1              - SetOption1  - Control button multipress
    uint32_t value_units : 1;              // bit 2              - SetOption2  - Add units to JSON status messages
    uint32_t mqtt_enabled : 1;             // bit 3              - SetOption3  - Control MQTT
    uint32_t mqtt_response : 1;            // bit 4              - SetOption4  - Switch between MQTT RESULT or COMMAND
    uint32_t mqtt_power_retain : 1;        // bit 5              - CMND_POWERRETAIN
    uint32_t mqtt_button_retain : 1;       // bit 6              - CMND_BUTTONRETAIN
    uint32_t mqtt_switch_retain : 1;       // bit 7              - CMND_SWITCHRETAIN
    uint32_t temperature_conversion : 1;   // bit 8              - SetOption8  - Switch between Celsius or Fahrenheit
    uint32_t mqtt_sensor_retain : 1;       // bit 9              - CMND_SENSORRETAIN
    uint32_t mqtt_offline : 1;             // bit 10             - SetOption10 - Control MQTT LWT message format
    uint32_t button_swap : 1;              // bit 11 (v5.1.6)    - SetOption11 - Swap button single and double press functionality
    uint32_t stop_flash_rotate : 1;        // bit 12 (v5.2.0)    - SetOption12 - Switch between dynamic or fixed slot flash save location
    uint32_t button_single : 1;            // bit 13 (v5.4.0)    - SetOption13 - Support only single press to speed up button press recognition
    uint32_t interlock : 1;                // bit 14 (v5.6.0)    - CMND_INTERLOCK
    uint32_t pwm_control : 1;              // bit 15 (v5.8.1)    - SetOption15 - Switch between commands PWM or COLOR/DIMMER/CT/CHANNEL
    uint32_t ws_clock_reverse : 1;         // bit 16 (v5.8.1)    - SetOption16 - Switch between clockwise or counter-clockwise
    uint32_t decimal_text : 1;             // bit 17 (v5.8.1)    - SetOption17 - Switch between decimal or hexadecimal output
    uint32_t light_signal : 1;             // bit 18 (v5.10.0c)  - SetOption18 - Pair light signal with CO2 sensor
    uint32_t hass_discovery : 1;           // bit 19 (v5.11.1a)  - SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
    uint32_t not_power_linked : 1;         // bit 20 (v5.11.1f)  - SetOption20 - Control power in relation to Dimmer/Color/Ct changes
    uint32_t no_power_on_check : 1;        // bit 21 (v5.11.1i)  - SetOption21 - Show voltage even if powered off
    uint32_t mqtt_serial : 1;              // bit 22 (v5.12.0f)  - CMND_SERIALSEND and CMND_SERIALLOG
    uint32_t mqtt_serial_raw : 1;          // bit 23 (v6.1.1c)   - CMND_SERIALSEND3
    uint32_t pressure_conversion : 1;      // bit 24 (v6.3.0.2)  - SetOption24 - Switch between hPa or mmHg pressure unit
    uint32_t knx_enabled : 1;              // bit 25 (v5.12.0l)  - CMND_KNX_ENABLED
    uint32_t device_index_enable : 1;      // bit 26 (v5.13.1a)  - SetOption26 - Switch between POWER or POWER1
    uint32_t knx_enable_enhancement : 1;   // bit 27 (v5.14.0a)  - CMND_KNX_ENHANCED
    uint32_t rf_receive_decimal : 1;       // bit 28 (v6.0.0a)   - SetOption28 - RF receive data format
    uint32_t ir_receive_decimal : 1;       // bit 29 (v6.0.0a)   - SetOption29 - IR receive data format
    uint32_t hass_light : 1;               // bit 30 (v6.0.0b)   - SetOption30 - Enforce HAss autodiscovery as light
    uint32_t global_state : 1;             // bit 31 (v6.1.0)    - SetOption31 - Control link led blinking
  };
} SysBitfield_System;


typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption50 .. SetOption81
    uint32_t timers_enable : 1;            // bit 0 (v6.1.1b)    - CMND_TIMERS
    uint32_t user_esp8285_enable : 1;      // bit 1 (v6.1.1.14)  - SetOption51 - Enable ESP8285 user GPIO's
    uint32_t time_append_timezone : 1;     // bit 2 (v6.2.1.2)   - SetOption52 - Append timezone to JSON time
    uint32_t gui_hostname_ip : 1;          // bit 3 (v6.2.1.20)  - SetOption53 - Show hostanme and IP address in GUI main menu
    uint32_t tuya_apply_o20 : 1;           // bit 4 (v6.3.0.4)   - SetOption54 - Apply SetOption20 settings to Tuya device
    uint32_t mdns_enabled : 1;             // bit 5 (v6.4.1.4)   - SetOption55 - Control mDNS service
    uint32_t use_wifi_scan : 1;            // bit 6 (v6.3.0.10)  - SetOption56 - Scan wifi network at restart for configured AP's
    uint32_t use_wifi_rescan : 1;          // bit 7 (v6.3.0.10)  - SetOption57 - Scan wifi network every 44 minutes for configured AP's
    uint32_t receive_raw : 1;              // bit 8 (v6.3.0.11)  - SetOption58 - Add IR Raw data to JSON message
    uint32_t hass_tele_on_power : 1;       // bit 9 (v6.3.0.13)  - SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
    uint32_t sleep_normal : 1;             // bit 10 (v6.3.0.15) - SetOption60 - Enable normal sleep instead of dynamic sleep
    uint32_t button_switch_force_local : 1;// bit 11 (v6.3.0.16) - SetOption61 - Force local operation when button/switch topic is set
    uint32_t no_hold_retain : 1;           // bit 12 (v6.4.1.19) - SetOption62 - Don't use retain flag on HOLD messages
    uint32_t no_power_feedback : 1;        // bit 13 (v6.5.0.9)  - SetOption63 - Don't scan relay power state at restart
    uint32_t use_underscore : 1;           // bit 14 (v6.5.0.12) - SetOption64 - Enable "_" instead of "-" as sensor index separator
    uint32_t fast_power_cycle_disable : 1; // bit 15 (v6.6.0.20) - SetOption65 - Disable fast power cycle detection for device reset
    uint32_t tuya_serial_mqtt_publish : 1; // bit 16 (v6.6.0.21) - SetOption66 - Enable TuyaMcuReceived messages over Mqtt
    //uint32_t buzzer_enable : 1;            // bit 17 (v6.6.0.1)  - SetOption67 - Enable buzzer when available
    uint32_t pwm_multi_channels : 1;       // bit 18 (v6.6.0.3)  - SetOption68 - Enable multi-channels PWM instead of Color PWM
    uint32_t ex_tuya_dimmer_min_limit : 1; // bit 19 (v6.6.0.5)  - SetOption69 - Limits Tuya dimmers to minimum of 10% (25) when enabled.
    uint32_t energy_weekend : 1;           // bit 20 (v6.6.0.8)  - CMND_TARIFF
    uint32_t dds2382_model : 1;            // bit 21 (v6.6.0.14) - SetOption71 - Select different Modbus registers for Active Energy (#6531)
    uint32_t hardware_energy_total : 1;    // bit 22 (v6.6.0.15) - SetOption72 - Enable hardware energy total counter as reference (#6561)
    uint32_t ex_cors_enabled : 1;          // bit 23 (v7.0.0.1)  - SetOption73 - Enable HTTP CORS
    uint32_t ds18x20_internal_pullup : 1;  // bit 24 (v7.0.0.1)  - SetOption74 - Enable internal pullup for single DS18x20 sensor
    uint32_t grouptopic_mode : 1;          // bit 25 (v7.0.0.1)  - SetOption75 - GroupTopic replaces %topic% (0) or fixed topic cmnd/grouptopic (1)
    uint32_t bootcount_update : 1;         // bit 26 (v7.0.0.4)  - SetOption76 - Enable incrementing bootcount when deepsleep is enabled
    uint32_t slider_dimmer_stay_on : 1;    // bit 27 (v7.0.0.6)  - SetOption77 - Do not power off if slider moved to far left
    uint32_t compatibility_check : 1;      // bit 28 (v7.1.2.6)  - SetOption78 - Disable OTA compatibility check
    uint32_t counter_reset_on_tele : 1;    // bit 29 (v8.1.0.1)  - SetOption79 - Enable resetting of counters after telemetry was sent
    // uint32_t shutter_mode : 1;             // bit 30 (v6.6.0.14) - SetOption80 - Enable shutter support
    // uint32_t pcf8574_ports_inverted : 1;   // bit 31 (v6.6.0.14) - SetOption81 - Invert all ports on PCF8574 devices
    uint32_t network_wifi : 1;             // bit 13 (v8.3.1.3)  - CMND_WIFI
    uint32_t network_ethernet : 1;         // bit 14 (v8.3.1.3)  - CMND_ETHERNET


  };
} SysBitfield_Network;

// Due to the muliple connections required, mqtt connection settings will be moved into its own file as json format
/**
 * @brief Instead this will be used for "ActiveMQTT" and hence will be loaded from memory. Though will need to be a vector to hold multiple, so 
 * this likely needs totally moved into the module itself (and class header)
 * 
 */
// struct SettingsMQTT
// {
// };


typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint8_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint8_t buzzer_freq_mode : 1;               // bit 0              - SetOption0  - Save power state and use after restart
    uint8_t buzzer_enable : 1;
    uint8_t reserved : 6;          // bit 1              - SetOption1  - Control button multipress
   
  };
} SysBitfield_Drivers;


typedef union {
  uint16_t data;
  struct {
    uint16_t clear_on_reboot : 1;
    uint16_t reserved : 15;     
  };
} SysBitfield_Animations;


typedef union {
  uint16_t data;
  struct {
    uint16_t spare0 : 1;
    uint16_t reserved : 15;     
  };
} SysBitfield_Lighting;


struct LightSettings{
  uint8_t light_brightness_as_percentage;
  uint8_t light_fade;          // 4A1
  uint8_t light_speed;         // 4A2
  uint8_t light_scheme;        // 4A3
  uint8_t light_width;         // 4A4
  uint16_t light_rotation;     // 39E
  uint16_t light_pixels;       // 496
  uint8_t light_correction;    // 49D
  uint8_t light_dimmer;        // 49E
  uint16_t light_wakeup;       // 4A6
  uint8_t type; //phase out now with multipin?
  uint16_t size;
};


// Buffer that stores names of sensors as delimeter list
#ifndef DEVICENAMEBUFFER_NAME_INDEX_LENGTH // Memory reduction
#define DEVICENAMEBUFFER_NAME_INDEX_LENGTH 70 
#endif // DEVICENAMEBUFFER_NAME_INDEX_LENGTH
#ifndef DEVICENAMEBUFFER_NAME_BUFFER_LENGTH // Memory reduction
#define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 400 
#endif // DEVICENAMEBUFFER_NAME_BUFFER_LENGTH
struct DeviceNameBuffer{ // size(230)
  // delimeter name list
  char name_buffer[DEVICENAMEBUFFER_NAME_BUFFER_LENGTH];
  // index array that holds name_list sensor (class name, sensor number)
  /**
   * This needs to be the large unique identifier
   * "Unused" will be the largest possible value, instead of using signed value and setting to "-1", which makes -2->minimum wasted 
   * */
  uint16_t class_id[DEVICENAMEBUFFER_NAME_INDEX_LENGTH]  = { D_MAX_UINT16 }; //hold class id
  uint8_t  device_id[DEVICENAMEBUFFER_NAME_INDEX_LENGTH] = { D_MAX_UINT8  };  //max of X sensors per module
};


typedef union {
  uint8_t data;
  struct {
    uint8_t spare0 : 1;
    uint8_t spare1 : 1;
    uint8_t bh1750_1_resolution : 2;       // Sensor10 1,2,3
    uint8_t bh1750_2_resolution : 2;
    uint8_t hx711_json_weight_change : 1;  // Sensor34 8,x - Enable JSON message on weight change
    uint8_t mhz19b_abc_disable : 1;        // Disable ABC (Automatic Baseline Correction for MHZ19(B) (0 = Enabled (default), 1 = Disabled with Sensor15 command)
  };
} SensorCfg1;

// Bitfield to be used for any SetOption50 .. SetOption81 persistent single bit
typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption50 .. SetOption81
    uint32_t timers_enable : 1;            // bit 0 (v6.1.1b)    - CMND_TIMERS
    uint32_t user_esp8285_enable : 1;      // bit 1 (v6.1.1.14)  - SetOption51  - (GPIO) Enable ESP8285 user GPIO's (1)
    uint32_t time_append_timezone : 1;     // bit 2 (v6.2.1.2)   - SetOption52  - (Time) Append timezone to JSON time (1)
    uint32_t gui_hostname_ip : 1;          // bit 3 (v6.2.1.20)  - SetOption53  - (GUI) Show hostname and IP address in GUI main menu (1)
    uint32_t tuya_apply_o20 : 1;           // bit 4 (v6.3.0.4)   - SetOption54  - (Tuya) Apply SetOption20 settings to Tuya device (1)
    uint32_t mdns_enabled : 1;             // bit 5 (v6.4.1.4)   - SetOption55  - (mDNS) Service on (1) or off (0)
    uint32_t use_wifi_scan : 1;            // bit 6 (v6.3.0.10)  - SetOption56  - (Wifi) Scan network at restart for configured AP's (1) or used stored AP (0)
    uint32_t use_wifi_rescan : 1;          // bit 7 (v6.3.0.10)  - SetOption57  - (Wifi) Scan network every 44 minutes for configured AP's (1)
    uint32_t receive_raw : 1;              // bit 8 (v6.3.0.11)  - SetOption58  - (IR) Add IR Raw data to JSON message (1)
    uint32_t hass_tele_on_power : 1;       // bit 9 (v6.3.0.13)  - SetOption59  - (MQTT) Send tele/%topic%/STATE in addition to stat/%topic%/RESULT (1)
    uint32_t sleep_normal : 1;             // bit 10 (v6.3.0.15) - SetOption60  - (Sleep) Enable normal sleep (1) instead of dynamic sleep (0)
    uint32_t button_switch_force_local : 1;// bit 11 (v6.3.0.16) - SetOption61  - (Button, Switch) Force local operation (1) when button/switch topic is set
    uint32_t no_hold_retain : 1;           // bit 12 (v6.4.1.19) - SetOption62  - (MQTT) Don't use retain flag on HOLD messages (1)
    uint32_t no_power_feedback : 1;        // bit 13 (v6.5.0.9)  - SetOption63  - (Power) Don't scan relay power state at restart (1)
    uint32_t use_underscore : 1;           // bit 14 (v6.5.0.12) - SetOption64  - (JSON) Enable "_" (1) instead of "-" (0) as sensor index separator
    uint32_t fast_power_cycle_disable : 1; // bit 15 (v6.6.0.20) - SetOption65  - (QPC) Disable (1) fast power cycle detection for device reset
    uint32_t tuya_serial_mqtt_publish : 1; // bit 16 (v6.6.0.21) - SetOption66  - (Tuya) Enable (1) TuyaMcuReceived messages over Mqtt
    uint32_t buzzer_enable : 1;            // bit 17 (v6.6.0.1)  - SetOption67  - (Buzzer) Enable (1) buzzer when available
    uint32_t pwm_multi_channels : 1;       // bit 18 (v6.6.0.3)  - SetOption68  - (Light) Enable multi-channels PWM (1) instead of Color PWM (0)
    uint32_t ex_tuya_dimmer_min_limit : 1; // bit 19 (v6.6.0.5)  - SetOption69  - (not used) Limits Tuya dimmers to minimum of 10% (25) when enabled
    uint32_t energy_weekend : 1;           // bit 20 (v6.6.0.8)  - CMND_TARIFF
    uint32_t dds2382_model : 1;            // bit 21 (v6.6.0.14) - SetOption71  - (DDS2382) Select different Modbus registers (1) for Active Energy (#6531)
    uint32_t hardware_energy_total : 1;    // bit 22 (v6.6.0.15) - SetOption72  - (Energy) Enable (1) hardware energy total counter as reference (#6561)
    uint32_t mqtt_buttons : 1;             // bit 23 (v8.2.0.3)  - SetOption73  - (Button) Detach buttons from relays (1) and enable MQTT action state for multipress
    uint32_t ds18x20_internal_pullup : 1;  // bit 24 (v7.0.0.1)  - SetOption74  - (DS18x20) Enable internal pullup (1) for single DS18x20 sensor
    uint32_t grouptopic_mode : 1;          // bit 25 (v7.0.0.1)  - SetOption75  - (MQTT) GroupTopic replaces %topic% (0) or fixed topic cmnd/grouptopic (1)
    uint32_t bootcount_update : 1;         // bit 26 (v7.0.0.4)  - SetOption76  - (Deepsleep) Enable incrementing bootcount (1) when deepsleep is enabled
    uint32_t slider_dimmer_stay_on : 1;    // bit 27 (v7.0.0.6)  - SetOption77  - (Light) Do not power off (1) if slider moved to far left
    uint32_t ex_compatibility_check : 1;   // bit 28 (v7.1.2.6)  - SetOption78  - (not used) Disable OTA compatibility check
    uint32_t counter_reset_on_tele : 1;    // bit 29 (v8.1.0.1)  - SetOption79  - (Counter) Enable resetting of counters (1) after telemetry was sent
    uint32_t shutter_mode : 1;             // bit 30 (v6.6.0.14) - SetOption80  - (Shutter) Enable shutter support (1)
    uint32_t pcf8574_ports_inverted : 1;   // bit 31 (v6.6.0.14) - SetOption81  - (PCF8574) Invert all ports on PCF8574 devices (1)
  };
} SOBitfield3;





typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint8_t data;                           // Allow bit manipulation using SetOption
  struct {                                 // SetOption0 .. SetOption31
    uint8_t mqtt_retain : 1;               // bit 0              - SetOption0  - Save power state and use after restart
    uint8_t button_restrict : 1;          // bit 1              - SetOption1  - Control button multipress
    uint8_t decimal_precision : 2;        // bit 2,3   4 levels [0,1,2,3]

  };
} SysBitfield_Sensors;



struct SensorSettings{
  uint16_t      configperiod_secs;
  uint16_t      teleperiod_secs;
  uint16_t      ifchanged_secs;
  // Percentage of possible values that signify a large enough change has occured worth reporting.
  // A value of '0' percent means anything at all
  uint8_t       ifchanged_change_percentage_threshold;
  uint8_t       teleperiod_json_level;
  uint8_t       ifchanged_json_level;
  uint8_t       teleperiod_retain_flag;
  /**
   * Stored as ints for data savings, flat change to int
   * */
  int16_t       altitude;            
  float           latitude;      //54.5 shall be 54500000
  float           longitude;               
  SysBitfield_Sensors flags;
};


struct EnergyUsageNew{
  uint32_t usage1_kWhtotal;
  uint32_t usage2_kWhtotal;
  uint32_t return1_kWhtotal;
  uint32_t return2_kWhtotal;
  uint32_t last_return_kWhtotal;
  uint32_t last_usage_kWhtotal;  
  uint8_t       energy_power_delta;        // 33F
  unsigned long energy_power_calibration;  // 364
  unsigned long energy_voltage_calibration;  // 368
  unsigned long energy_current_calibration;  // 36C
  unsigned long energy_kWhtoday;           // 370
  unsigned long energy_kWhyesterday;       // 374
  uint16_t      energy_kWhdoy;             // 378
  uint16_t      energy_min_power;          // 37A
  uint16_t      energy_max_power;          // 37C
  uint16_t      energy_min_voltage;        // 37E
  uint16_t      energy_max_voltage;        // 380
  uint16_t      energy_min_current;        // 382
  uint16_t      energy_max_current;        // 384
  uint16_t      energy_max_power_limit;    // 386 MaxPowerLimit
  uint16_t      energy_max_power_limit_hold;         // 388 MaxPowerLimitHold
  uint16_t      energy_max_power_limit_window;       // 38A MaxPowerLimitWindow
  uint16_t      energy_max_power_safe_limit;         // 38C MaxSafePowerLimit
  uint16_t      energy_max_power_safe_limit_hold;    // 38E MaxSafePowerLimitHold
  uint16_t      energy_max_power_safe_limit_window;  // 390 MaxSafePowerLimitWindow
  uint16_t      energy_max_energy;         // 392 MaxEnergy
  uint16_t      energy_max_energy_start;   // 394 MaxEnergyStart
  uint32_t      energy_kWhtotal_time;      // 7B4
  unsigned long energy_frequency_calibration;  // 7C8
  unsigned long energy_kWhtotal;           // 554
  uint16_t      tariff[4][2];              // E30
};// EnergyUsage;


// Sensors
typedef union {
  uint32_t data;                           // Allow bit manipulation
  struct {
    uint32_t spare00 : 1;
    uint32_t spare01 : 1;
    uint32_t spare02 : 1;
    uint32_t spare03 : 1;
    uint32_t spare04 : 1;
    uint32_t spare05 : 1;
    uint32_t calc_resolution : 3;
    uint32_t weight_resolution : 2;
    uint32_t frequency_resolution : 2;
    uint32_t axis_resolution : 2;
    uint32_t current_resolution : 2;
    uint32_t voltage_resolution : 2;
    uint32_t wattage_resolution : 2;
    uint32_t emulation : 2;
    uint32_t energy_resolution : 3;
    uint32_t pressure_resolution : 2;
    uint32_t humidity_resolution : 2;
    uint32_t temperature_resolution : 2;
  };
} SysBitfield_Power;

struct DisplaySettings{
  uint8_t       model; 
  uint8_t       mode;
  uint8_t       refresh;
  uint8_t       rows;
  uint8_t       cols[2];
  uint8_t       address[8];
  uint8_t       dimmer;
  uint8_t       size;
  uint8_t       font;
  uint8_t       rotate;
  uint16_t      width;
  uint16_t      height;
};


typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint32_t data;
  struct {
    uint32_t stream : 1;
    uint32_t mirror : 1;
    uint32_t flip : 1;
    uint32_t rtsp : 1;
    uint32_t awb : 1;
    uint32_t awb_gain : 1;
    uint32_t aec : 1;
    uint32_t aec2 : 1;
    uint32_t agc : 1;
    uint32_t raw_gma : 1;
    uint32_t lenc : 1;
    uint32_t colorbar : 1;
    uint32_t wpc : 1;
    uint32_t dcw : 1;
    uint32_t bpc : 1;
    uint32_t spare15 : 1;
    uint32_t spare16 : 1;
    uint32_t feature : 2;
    uint32_t contrast : 3;
    uint32_t brightness : 3;
    uint32_t saturation : 3;
    uint32_t resolution : 4;
  };
} WebCamCfg;

typedef union {
  uint32_t data;
  struct {
    uint32_t wb_mode : 3;
    uint32_t ae_level : 3;
    uint32_t aec_value : 11;
    uint32_t gainceiling : 3;
    uint32_t agc_gain: 5;
    uint32_t special_effect : 3;
    uint32_t auth : 1;
    uint32_t spare29 : 1;
    uint32_t spare30 : 1;
    uint32_t upgraded : 1;
  };
} WebCamCfg2;

typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
  uint16_t data;                           // Allow bit manipulation
  struct {
    uint16_t system_init : 1;              // Changing layout here needs adjustments in xdrv_10_rules.ino too
    uint16_t system_boot : 1;
    uint16_t time_init : 1;
    uint16_t time_set : 1;
    uint16_t mqtt_connected : 1;
    uint16_t mqtt_disconnected : 1;
    uint16_t wifi_connected : 1;
    uint16_t wifi_disconnected : 1;
    uint16_t eth_connected : 1;
    uint16_t eth_disconnected : 1;
    uint16_t http_init : 1;
    uint16_t shutter_moved : 1;
    uint16_t shutter_moving : 1;
    uint16_t spare13 : 1;
    uint16_t spare14 : 1;
    uint16_t spare15 : 1;
  };
} RulesBitfield;

struct LoggingSettings{
  uint8_t       serial_level;           // 09E
  uint16_t      sys_port;               // 1A8
  uint8_t       sys_level;              // 1AA
  uint8_t       web_level;              // 1AC
  uint8_t       telnet_level;              // 1AC
  uint8_t       time_isshort;  
};


struct SETTINGS {
  // Header (Minimal data load required to validate settings - order must never change)
  uint16_t      cfg_holder;                // 000 v6 header
  uint16_t      cfg_size;                  // 002
  uint32_t      save_flag;                 // 004
  uint32_t      version;                   // 008
  uint16_t      bootcount;              // 00C
  // Body (All other settings)
  // Modules
  uint16_t      bootcount_errors_only;     // E01
  uint8_t       module;                    // 474
  uint8_t       last_module;               // 399
  // Templates
  Template_Config user_template2;
  SystemName      system_name;
  char room_hint[50];
  SysBitfield_System   flag_system;                      // 010
  RulesBitfield rules_flag;                 // Rule state flags (16 bits)
  int16_t       save_data;                 // 014
  myio          module_pins;                     // 484     
  uint8_t       baudrate;                  // 09D  // saved as (/300) value. ie 9600/300 => 32, 115200=>384?? I want to change this to full uint32_t for higher speed bauds
  uint8_t       rule_stop;                 // 1A7
  LoggingSettings logging;
  DeviceNameBuffer        device_name_buffer; // this might need moving and then changed to use vectors, it would actually remove the need for delims
  uint8_t       enable_sleep;        // E03
  char          serial_delimiter;          // 451
  uint8_t       sbaudrate;                 // 452
  uint8_t       sleep;                     // 453
  uint8_t       setoption_255[PARAM8_SIZE];
  // Core
  // char debug[10];
  uint16_t      unified_interface_reporting_invalid_reading_timeout_seconds; // 0 is ignored, anything else is the seconds of age above which a sensor should not be reporting (ie is invalid)
  
  // Network
  uint8_t       sta_config;                // 09F
  uint8_t       sta_active;                // 0A0
  // char          sta_ssid[3][33];           // 0A1 - Keep together with sta_pwd as being copied as one chunck with reset 5
  // char          sta_pwd[3][65];            // 0E3 - Keep together with sta_ssid as being copied as one chunck with reset 5
  // char          hostname[33];              // 165
  // char          syslog_host[33];           // 186
  uint32_t      ip_address[5];             // 544
  uint8_t       wifi_channel;
  uint8_t       wifi_bssid[6];             // F0A
  SysBitfield_Network  flag_network;                     // 3A0
  // Webserver
  uint8_t       webserver;                 // 1AB
  // char          web_password[33];          // 4A9
  uint16_t      web_refresh;               // 7CC
  // MQTT
  // SettingsMQTT  mqtt; // Moved to mqtt module
  // Time
  int8_t        timezone;                  // 016
  uint8_t       timezone_minutes;          // 66D 
  int8_t        timezone2;                  // 016
  uint8_t       timezone_minutes2;          // 66D 
  uint8_t       ina219_mode;               // 531
  SysBitfield_Drivers    flag_drivers;  
  int16_t       toffset[2];                // 30E
  // char          ntp_server[3][33];         // 4CE
  char          text_pool[138];            // 017  Size is settings_text_size
  // Lighting
  SysBitfield_Lighting    flag_lighting;
  SysBitfield_Animations  flag_animations;
  LightSettings           light_settings;
  // Pulse Counter
  uint16_t      pulse_counter_type;        // 5D0
  uint16_t      pulse_counter_debounce;    // 5D2
  // Sensors
  SensorCfg1    SensorBits1;               // 717  On/Off settings used by Sensor Commands  
  uint16_t      button_debounce;           // 542
  uint16_t      switch_debounce;           // 66E
  uint8_t       switchmode[8];
  SensorSettings sensors;
  // Drivers
  uint16_t      ledmask;                   // 7BC
  uint8_t       ledstate;                  // 2FB
  uint8_t       ledpwm_mask;               // E8F  
  uint8_t       ledpwm_on;                 // F3F
  uint8_t       ledpwm_off;                // F40
  uint16_t      blinktime;                 // 39A
  uint16_t      blinkcount;                // 39C
  uint32_t      monitors;                  // 7A0
  uint16_t      pwm_range;                 // 342
  uint16_t      pwm_frequency;             // 2E6
  uint16_t      pwm_value[MAX_PWMS];       // 2EC
  uint16_t      rf_duplicate_time;         // 522
  // Power
  unsigned long power;                     // 2E8
  uint8_t       poweronstate;              // 398
  // Energy
  EnergyUsageNew   energy_usage;           // 77C 
  SysBitfield_Power  flag_power;           // 5BC
  // Displays
  DisplaySettings   display;  
  uint32_t      i2c_drivers[3];            // FEC
  uint64_t      rf_protocol_mask;          // FA8
  #ifdef ESP32
  WebCamCfg     webcam_config;             // 44C
  WebCamCfg2    webcam_config2;            // 460
  #endif
  #ifdef ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  char settings_holder_ctr[10];
  #endif
  uint32_t      bootcount_reset_time;      // FD4
  TimeRule      tflag[2];                  // 2E2
  SOBitfield3   flag3;                     // 3A0
  uint32_t      ipv4_address[5];           // 544
  uint32_t      ipv4_rgx_address;          // 558
  uint32_t      ipv4_rgx_subnetmask;       // 55C
  uint16_t      dns_timeout;               // 4C8
  #ifdef ENABLE_FEATURE_SETTINGS__ADD_LOCAL_TIME_AS_ASCII_FOR_SAVE_TIME_DEBUGGING
  char local_time_ascii_debug[20];
  #endif
  // E00 - FFF (4095 ie eeprom size) free locations
  uint32_t      cfg_timestamp;
  uint32_t      cfg_crc32;                 // 32 bit CRC, must remain at last 4 bytes
} Settings;


