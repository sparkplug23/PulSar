#ifndef _USE_MODULE_DRIVERS_PWM_H
#define _USE_MODULE_DRIVERS_PWM_H 0.3

#include "1_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_DRIVERS_PWM

#include "2_CoreSystem/InterfaceController/mInterfaceController.h"

#include <ArduinoJson.h>

#include <string.h>
#include <strings.h>

class mPWM{

  private:
  public:
    mPWM(){};
    int8_t Tasker(uint8_t function);
    int8_t Tasker(uint8_t function, JsonObjectConst obj);   

    int8_t Tasker_Web(uint8_t function);

    #define WEB_HANDLE_PWM_SLIDER "pwm_fan_speed"

// // const char kSonoffIfanCommands[] PROGMEM = "|"  // No prefix
// //   D_CMND_FANSPEED;

// // void (* const SonoffIfanCommand[])(void) PROGMEM = {
// //   &CmndFanspeed };

// uint8_t ifan_fanspeed_timer = 0;
// uint8_t ifan_fanspeed_goal = 0;
// bool ifan_receive_flag = false;
// bool ifan_restart_flag = true;


int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
int8_t parsesub_Commands(JsonObjectConst obj);

uint16_t test_val = 0;
uint8_t dir = 0;

void init();
void pre_init();
int8_t pin = -1;

struct SETTINGS{
  uint8_t fEnableModule = false;
  uint8_t fShowManualSlider = false;
}settings;

// uint8_t GetLightState(void);
// void SetLightState(uint8_t state);


// bool IsModuleIfan(void);
// uint8_t MaxFanspeed(void);
// uint8_t GetFanspeed(void);
// void SonoffIFanSetFanspeed(uint8_t fanspeed, bool sequence);
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

//     int8_t parse_JSONCommand();
//     //void AddToJsonObject_AddHardware(JsonObject root);
//     void AddToHardwareMessage();

    
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

    const char* postfix_topic_settings = "settings";
    struct handler<mPWM> mqtthandler_settings_teleperiod;
    
    const char* postfix_topic_sensors = "power";
    struct handler<mPWM> mqtthandler_sensor_ifchanged;
    struct handler<mPWM> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif