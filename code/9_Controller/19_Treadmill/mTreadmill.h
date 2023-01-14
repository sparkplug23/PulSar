#ifndef _MCUSE_MODULE_CONTROLLER_TREADMILL_H
#define _MCUSE_MODULE_CONTROLLER_TREADMILL_H 0.3

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_TREADMILL


class mTreadmill {

  private:
  public:
    mTreadmill(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);  

    int8_t Tasker_Web(uint8_t function);
    
    struct MEASURED_PARAMETERS{
      float mps = 0;
      float mph = 0;
      float mph_last = 0;
      uint32_t millis_last_updated = 0;
      float metres_reseting = 0;
      float metres_nonreseting = 0;
      uint16_t reset_seconds_decounter = 60; // value to count down, when hits zero, reset distance
      uint16_t reset_seconds_reset_period = 60; // value to count down, when hits zero, reset distance
      AVERAGING_DATA<float>* averaging;
    }measured;

    


// uint8_t ifan_fanspeed_timer = 0;
// uint8_t ifan_fanspeed_goal = 0;
// bool ifan_receive_flag = false;
// bool ifan_restart_flag = true;

void EverySecond();


    int8_t CheckAndExecute_JSONCommands(void);
    void parse_JSONCommand(void);

void init();
void Pre_Init();

int8_t pin = -1;

// uint16_t pwm_range_max = 1023;
// uint16_t pwm_range_min = 1;

// uint8_t set_fan_speed = 0;//0,1,2,3
// uint16_t set_fan_pwm = 0;//0,1,2,3


struct SETTINGS{
  uint8_t fEnableModule = false;
}settings;

uint8_t GetLightState(void);
void SetLightState(uint8_t state);

float GetSpeed_Mph(uint8_t calibration_method);

float GetSpeed_Averaging_Mph(uint8_t calibration_method=0, bool averaging=0, bool ignore_transients=0);


struct AVE{
  float mx_in;
  float input_cal_l;
  float input_cal_h;
  float output_cal_l;
  float output_cal_h;
  float calib;
  float isfound = false;
}av;


// bool IsModuleIfan(void);
// uint8_t MaxFanspeed(void);
// uint8_t GetFanspeed(void);
// void SetFanspeed(uint8_t fanspeed, bool sequence);
// void SonoffIfanReceived(void);
// bool SerialInput(void);
// void CmndFanspeed(void);
// void SpeedRefresh(void);


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


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_object_start_end_required = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_object_start_end_required = true);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mTreadmill>* ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mTreadmill> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
    struct handler<mTreadmill> mqtthandler_sensor_ifchanged;
    struct handler<mTreadmill> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif