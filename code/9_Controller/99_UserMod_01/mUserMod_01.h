#ifndef _MCUSE_MODULE_CONTROLLER_USERMOD_01_H
#define _MCUSE_MODULE_CONTROLLER_USERMOD_01_H 0.3

#define D_UNIQUE_MODULE_CONTROLLER_USERMOD_01_ID 145

// PWM Fan and Sonoff Fan to be brought together? One fan controller, different output methods

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_USERMOD_01

#ifdef ESP32
#include <esp8266toEsp32.h>
#endif


class mUserMod_01 :
  public mTaskerInterface
{

  private:
  public:
    mUserMod_01(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static const char* PM_MODULE_CONTROLLER_USERMOD_01_CTR;
    static const char* PM_MODULE_CONTROLLER_USERMOD_01_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_USERMOD_01_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_USERMOD_01_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_USERMOD_01_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSonoffIFan);
    };
    #endif

    void parse_JSONCommand(JsonParserObject obj);

    int8_t Tasker_Web(uint8_t function);
    void EveryLoop();
    void EveryLoop_EmulatePic32Measurement();

    void Init();
    void Pre_Init();

    int8_t pin = -1;

    
    struct PIC32_EMULATOR{
      uint16_t adc_low = 0;
      uint16_t adc_high = 4096;
      uint16_t adc_value = 0;
      uint16_t adc_dir = 0;
      uint32_t tSaved_AdcRampTick = 0;
      uint32_t tSaved_MeasureTest = 0;
      uint16_t frames_in_super_frame;
      uint8_t tx_num = 0;
      uint32_t sequencenum = 0;



    }pic32;


    char buffer[1000] = {0};

    uint8_t set_fan_speed = 0;//0,1,2,3
    uint16_t set_USERMOD_01 = 0;//0,1,2,3


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


    //void AddToJsonObject_AddHardware(JsonObject root);
    void AddToHardwareMessage();


    void CommandSet_FanSpeed_Manual(uint16_t value);


    #ifdef USE_MODULE_NETWORK_WEBSERVER23
    void WebCommand_Parse(void);
    void WebAppend_Root_Draw_PageTable();
    void WebAppend_Root_Status_Table();
    #endif// USE_MODULE_NETWORK_WEBSERVER23

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mUserMod_01> mqtthandler_settings_teleperiod;
    struct handler<mUserMod_01> mqtthandler_sensor_ifchanged;
    struct handler<mUserMod_01> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;

    struct handler<mUserMod_01>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

};

#endif

#endif