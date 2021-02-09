#ifndef _MODULE_CONTROLLER_STATUS_LEDS_H
#define _MODULE_CONTROLLER_STATUS_LEDS_H 0.1

#include "2_CoreSystem/mBaseConfig.h"

/**
 * This module will allow status leds, control of basic digitalio pins/pwm for controlling leds using pwm driver
 * */

#ifdef USE_MODULE_CONTROLLER_STATUS_LEDS  //rename as "custom" as controllers

class mStatusLEDs {

  private:
  public:
    mStatusLEDs(){};
    int8_t Tasker(uint8_t function);
//     int8_t Tasker(uint8_t function, JsonObjectConst obj);   

// int8_t Tasker_Web(uint8_t function);

// // const char kSonoffIfanCommands[] PROGMEM = "|"  // No prefix
// //   D_CMND_FANSPEED;

// // void (* const SonoffIfanCommand[])(void) PROGMEM = {
// //   &CmndFanspeed };

// uint8_t ifan_fanspeed_timer = 0;
// uint8_t ifan_fanspeed_goal = 0;
// bool ifan_receive_flag = false;
// bool ifan_restart_flag = true;


// int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
// void parse_JSONCommand(JsonObjectConst obj);

// void init();
// void pre_init();

// int8_t pin = -1;

// uint16_t pwm_range_max = 1023;
// uint16_t pwm_range_min = 1;

// uint8_t set_fan_speed = 0;//0,1,2,3
// uint8_t set_fan_pwm = 0;//0,1,2,3


// struct SETTINGS{
//   uint8_t fEnableModule = false;
// }settings;

// uint8_t GetLightState(void);
// void SetLightState(uint8_t state);


// bool IsModuleIfan(void);
// uint8_t MaxFanspeed(void);
// uint8_t GetFanspeed(void);
// void SetFanspeed(uint8_t fanspeed, bool sequence);
// void SonoffIfanReceived(void);
// bool SonoffIfanSerialInput(void);
// void CmndFanspeed(void);
// void SonoffIfanUpdate(void);


//     // #define CEILINGFAN_TOGGLE  0xA55595
//     // #define CEILINGFAN_DIMM    0xA55955
//     // #define CEILINGFAN_SPEED0  0xA55655
//     // #define CEILINGFAN_SPEED1  0xA55557
//     // #define CEILINGFAN_SPEED2  0xA55565
//     // #define CEILINGFAN_SPEED3  0xA5655B
//     // #define ONE_BIT_TIME 1252 //+- 30 std
//     // #define ZERO_BIT_TIME 439 //+- 30 std

//     void parse_JSONCommand();
//     //void AddToJsonObject_AddHardware(JsonObject root);
//     void AddToHardwareMessage();

    
// void WebCommand_Parse(void);


// void WebAppend_Root_Draw_PageTable();
// void WebAppend_Root_Status_Table();


//     uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
//     uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
//   //#ifdef USE_CORE_MQTT 

//     void MQTTHandler_Init();
//     void MQTTHandler_Set_fSendNow();
//     void MQTTHandler_Set_TelePeriod();
    
//     struct handler<mStatusLEDs>* mqtthandler_ptr;
//     void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

//     // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
//     struct handler<mStatusLEDs> mqtthandler_settings_teleperiod;
    
//     // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
//     struct handler<mStatusLEDs> mqtthandler_sensor_ifchanged;
//     struct handler<mStatusLEDs> mqtthandler_sensor_teleperiod;
    
//     const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif