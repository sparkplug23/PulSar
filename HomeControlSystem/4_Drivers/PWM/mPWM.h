#ifndef _USE_MODULE_DRIVERS_PWM_H
#define _USE_MODULE_DRIVERS_PWM_H 0.3

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_PWM


#include <string.h>
#include <strings.h>

class mPWM{

  private:
  public:
    mPWM(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    // int8_t Tasker(uint8_t function, JsonObjectConst obj);   

    int8_t Tasker_Web(uint8_t function);

    #define WEB_HANDLE_PWM_SLIDER "fan_speed"
    
// int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
// void parse_JSONCommand(JsonObjectConst obj);

uint16_t test_val = 0;
uint8_t dir = 0;

void init();
void pre_init();
int8_t pin = -1;

struct SETTINGS{
  uint8_t fEnableModule = false;
  uint8_t fShowManualSlider = false;
}settings;


    
void WebCommand_Parse(void);


void WebAppend_Root_Draw_PageTable();
void WebAppend_Root_Status_Table();


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mPWM>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mPWM> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
    struct handler<mPWM> mqtthandler_sensor_ifchanged;
    struct handler<mPWM> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif