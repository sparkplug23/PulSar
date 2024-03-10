#ifndef _MCUSE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_H
#define _MCUSE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_H

#define D_UNIQUE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_ID     ((10*1000)+01)
/**
 * Using sensor readings, to generate colour palettes
 * This will be part of the new "sensor_struct" to hold all types
 *   Should it do like wled effects, memory buffer with different struct types?
 *    GetSensor(ptr,TEMP_ID) which will use memory location 0, to know how the struct is encoded and return the temperature from bytes 1-5 as float for example
 * This way, one struct pointer can be used to pass "sensor X" as a task source for this custom controller
 * */

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR

class mImmersionTankColour :
  public mTaskerInterface
{

  private:
  public:
    mImmersionTankColour(){};


    static const char* PM_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_CTR;
    static const char* PM_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mImmersionTankColour);
    };
    #endif



    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    // int8_t Tasker(uint8_t function, JsonObjectConst obj);   

    void EverySecond();

    void Event_Handle_Light_Toggle_Button();

    void parse_JSONCommand(JsonParserObject obj);
    int8_t Tasker_Web(uint8_t function);

    //move back into function long term for stability as its only temp needed
      uint8_t encoded_gradient_temp_array[200];

    void init();
    void Pre_Init();

    struct SETTINGS{
      uint8_t fEnableModule = false;
    }settings;

    enum MODE_SHOW_COLOURS_IDS{
      MODE_SHOW_COLOURS_GRADIENT_COLOUR_TEMP_ON_ID,
      MODE_SHOW_COLOURS_OFF_ID,
      MODE_SHOW_COLOURS_LENGTH_ID
    };

    struct DATA{

      uint8_t mode = MODE_SHOW_COLOURS_GRADIENT_COLOUR_TEMP_ON_ID;

      Decounter<uint16_t>* time_on = nullptr;//new Decounter();



    }data;

    void SubTask_StripSet_Showing();


    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebCommand_Parse(void);
    void WebAppend_Root_Draw_PageTable();
    void WebAppend_Root_Status_Table();
    #endif// USE_MODULE_NETWORK_WEBSERVER


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    struct handler<mImmersionTankColour>* ptr;
    void MQTTHandler_Sender();

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mImmersionTankColour> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
    struct handler<mImmersionTankColour> mqtthandler_sensor_ifchanged;
    struct handler<mImmersionTankColour> mqtthandler_sensor_teleperiod;
    
  struct handler<mImmersionTankColour>* mqtthandler_list[3] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif