#ifndef _MCUSE_MODULE_CUSTOM_SENSORCOLOURS_H
#define _MCUSE_MODULE_CUSTOM_SENSORCOLOURS_H 0.3

/**
 * Using sensor readings, to generate colour palettes
 * This will be part of the new "sensor_struct" to hold all types
 *   Should it do like wled effects, memory buffer with different struct types?
 *    GetSensor(ptr,TEMP_ID) which will use memory location 0, to know how the struct is encoded and return the temperature from bytes 1-5 as float for example
 * This way, one struct pointer can be used to pass "sensor X" as a task source for this custom controller
 * */

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CUSTOM_SENSORCOLOURS

class mSensorColours {

  private:
  public:
    mSensorColours(){};
    int8_t Tasker(uint8_t function);
    // int8_t Tasker(uint8_t function, JsonObjectConst obj);   

    void EverySecond();

int8_t Tasker_Web(uint8_t function);

// const char kSonoffIfanCommands[] PROGMEM = "|"  // No prefix
//   D_CMND_FANSPEED;

// void (* const SonoffIfanCommand[])(void) PROGMEM = {
//   &CmndFanspeed };

uint8_t ifan_fanspeed_timer = 0;
uint8_t ifan_fanspeed_goal = 0;
bool ifan_receive_flag = false;
bool ifan_restart_flag = true;


    int8_t CheckAndExecute_JSONCommands(void);
    void parse_JSONCommand(void);

void init();
void pre_init();

int8_t pin = -1;

uint16_t pwm_range_max = 1023;
uint16_t pwm_range_min = 1;

uint8_t set_fan_speed = 0;//0,1,2,3
uint16_t set_fan_pwm = 0;//0,1,2,3


struct SETTINGS{
  uint8_t fEnableModule = false;
}settings;

uint8_t GetLightState(void);
void SetLightState(uint8_t state);


bool IsModuleIfan(void);
uint8_t MaxFanspeed(void);
uint8_t GetFanspeed(void);
void SetFanspeed(uint8_t fanspeed, bool sequence);
void SonoffIfanReceived(void);
bool SerialInput(void);
void CmndFanspeed(void);
void SpeedRefresh(void);


    // #define CEILINGFAN_TOGGLE  0xA55595
    // #define CEILINGFAN_DIMM    0xA55955
    // #define CEILINGFAN_SPEED0  0xA55655
    // #define CEILINGFAN_SPEED1  0xA55557
    // #define CEILINGFAN_SPEED2  0xA55565
    // #define CEILINGFAN_SPEED3  0xA5655B
    // #define ONE_BIT_TIME 1252 //+- 30 std
    // #define ZERO_BIT_TIME 439 //+- 30 std

    //void AddToJsonObject_AddHardware(JsonObject root);
    void AddToHardwareMessage();

    
void CommandSet_FanSpeed_Manual(uint16_t value);


  #ifdef USE_MODULE_NETWORK_WEBSERVER
void WebCommand_Parse(void);


void WebAppend_Root_Draw_PageTable();
void WebAppend_Root_Status_Table();
  #endif// USE_MODULE_NETWORK_WEBSERVER


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mSensorColours>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mSensorColours> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
    struct handler<mSensorColours> mqtthandler_sensor_ifchanged;
    struct handler<mSensorColours> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif