

#ifndef _MRULEENGINE_H
#define _MRULEENGINE_H

#define D_UNIQUE_MODULE_CORE_RULES_FRIENDLY_ID 6

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE_RULES

#include <stdint.h>
#include "3_Network/MQTT/mMQTT.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"

#include "1_TaskerManager/mTaskerInterface.h"

class mRuleEngine :
  public mTaskerInterface
{
    
  private:
    /* Prevent others from being created */
    // RuleEngine(RuleEngine const& other) = delete;
    // RuleEngine(RuleEngine&& other) = delete;
    /* Private constructor to prevent instancing. */
    // RuleEngine(){};
    
  public:
    #define D_MAX_RULES 5
    
    static const char* PM_MODULE_CORE_RULES_CTR;
    static const char* PM_MODULE_CORE_RULES_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_RULES_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_RULES_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_RULES_FRIENDLY_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mRuleEngine);
    };
    #endif

    struct SETTINGS{

bool loaded_default_for_moduled = false;
}settings;  

    uint8_t* data = nullptr;
    uint16_t _dataLen = 0;
    uint16_t _dataUsedLen = 0;

    struct JSONCOMMANDS{
      char data[255] = {0}; //use | delims
      uint8_t bytes_used = 0;
      int8_t delims_used = 0;

    }jsonbuffer;

    // need a group of functions to "add" and remove from the buffer,
    // what about buffers? they can includes ids


    // mRuleEngine(uint16_t _data_buffer_size){

    //   _dataLen = _data_buffer_size;
    //   bool result = allocateData(_data_buffer_size);

    // };
    mRuleEngine(){};

    // ~mRuleEngine(){
    //   deallocateData();
    // };

    // bool allocateData(uint16_t len){
    //   // Serial.println("allocateData");
    //   if (data && _dataLen == len) return true; //already allocated
    //   deallocateData();
    //   data = new (std::nothrow) byte[len];
    //   if (!data) return false; //allocation failed
    //   _dataLen = len;
    //   for(int i=0;i<_dataLen;i++){data[i] = 0;}
    //   _dataUsedLen = 0;
    //   //   memset(data, 0, len*sizeof(TYPE));
    //   return true;
    // }

    // void deallocateData(){
    //   delete[] data;
    //   data = nullptr;
    //   _dataLen = 0;
    // };

    // // External function to get instance
    // static RuleEngine* GetInstance();
    // /* Here will be the instance stored. */
    // static RuleEngine* instance;

    // Allocate data buffer using pointers
    void AllocateEventBuffer(uint8_t* p_to_buffer, uint8_t bytes_to_add){

      // uint8_t index_start = _dataUsedLen + 1;
      p_to_buffer = &data[_dataUsedLen];
      _dataUsedLen += bytes_to_add;

    }
    
    /**
     * creating a few templates, can be done in comment only, then generated at runtime within scope
     * // Relay 
     * struct RELAY{
     *  uint8_t index;
     *  uint8_t state;
     *  uint8_t index;
     * }
     * */


    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    void parse_JSONCommand(JsonParserObject obj);
 
    enum RULE_DATA_ENCODING_IDS{
        ENCODING_BYTES_ID,
        RULE_DATA_ENCODING_8B8B16B_BYTES_ID,
        RULE_DATA_ENCODING_16BIT_ID,
        RULE_DATA_ENCODING_LENGTH_ID
    };
//command source will be useful for rules, 
// enum CommandSource { SRC_IGNORE, SRC_MQTT, SRC_RESTART, SRC_BUTTON, SRC_SWITCH, SRC_BACKLOG, SRC_SERIAL, SRC_WEBGUI, SRC_WEBCOMMAND, SRC_WEBCONSOLE, SRC_PULSETIMER,
//                      SRC_TIMER, SRC_RULE, SRC_MAXPOWER, SRC_MAXENERGY, SRC_LIGHT, SRC_KNX, SRC_DISPLAY, SRC_WEMO, SRC_HUE, SRC_RETRY, SRC_MAX };
// const char kCommandSource[] PROGMEM = "I|MQTT|Restart|Button|Switch|Backlog|Serial|WebGui|WebCommand|WebConsole|PulseTimer|Timer|Rule|MaxPower|MaxEnergy|Light|Knx|Display|Wemo|Hue|Retry";

    

    struct EVENT_PART{
      /**
       * Buttons, Relays
       * */
      uint16_t module_id;
      /**
       * InputChange, Timer
       */
      uint16_t function_id;
      /**
       * ?? or buffer[0]
       * */
      uint8_t device_id;
      /**
       * Values
       * */
      struct VALUES{
        uint8_t encoding = 0;
        uint8_t data[5];
        uint8_t length = 0;
      }value;
      /**
       * jsoncommands stored in a dlist
       * */
      int8_t json_commands_dlist_id = -1;
    };

    struct RULES{
      bool enabled = false;
      EVENT_PART trigger;
      EVENT_PART command; //introduce multiple commands? Create a vector of rules?
    }rules[D_MAX_RULES];

    // This will need to become an array or queue, that way consecutive rules can trigger at the same time
    EVENT_PART event_triggered;


    uint8_t rules_active_index = 0;



    //move into class
    // void Reset(){
    //   Reset(&EventTriggered);
    // }
    void Reset(EVENT_PART* e){
      e->module_id = 0;
      e->function_id = 0;
      // e->index = 0;
      // e->state = 0;
    };
    void Add_All(EVENT_PART* e, uint16_t _module_id=0, uint8_t _index=0, uint8_t _state=0){
      e->module_id = _module_id;
      // e->function_id = _function_id;
      // e->index = _index;
      // e->state = _state;
    };
    void Encoding(EVENT_PART* e, uint8_t encoding){
      e->value.encoding = encoding;
    };

    // If no event is passed, assume its the triggered_event
    void Reset(){
      event_triggered.module_id = 0;
      event_triggered.function_id = 0;
      // event_triggered.index = 0;
      // event_triggered.state = 0;
    };
    // Maybe Addlog_p method for share "arg" list
    void Add_All(uint16_t _module_id=0, uint8_t _index=0, uint8_t _state=0){
      event_triggered.module_id = _module_id;
      event_triggered.device_id = _index;
      event_triggered.value.data[0] = _state;
    };
    void NewEvent(uint16_t _module_id=0, uint8_t _index=0, uint8_t _state=0){
      Reset();
      event_triggered.module_id = _module_id;
      event_triggered.device_id = _index;
      event_triggered.value.data[0] = _state;
    };
    void Encoding( uint8_t encoding){
      event_triggered.value.encoding = encoding;
    };
    //create arg version
    void New_Event(uint16_t _module_id=0, uint8_t _device_id=0, uint8_t data1=0, uint8_t data2=0){
      Reset();
      event_triggered.module_id = _module_id;
      event_triggered.device_id = _device_id;
      event_triggered.value.data[0] = data1;
      event_triggered.value.data[1] = data2;
      event_triggered.value.encoding = 0; //bytes
      event_triggered.value.length = 1; //bytes


    }

    void DefaultRuleForModule();

    #ifdef USE_MODULE_TEMPLATE_SONOFF_IFAN03
    void DefaultRule_Sonoff_iFan03();
    #endif // USE_MODULE_TEMPLATE_SONOFF_IFAN03
    #ifdef USE_MODULE_TEMPLATE_SHELLY_DIMMER2
    void DefaultRule_Shelly_Dimmer2();
    #endif // USE_MODULE_TEMPLATE_SHELLY_DIMMER2
    #ifdef USE_MODULE_TEMPLATE_SHELLY_2P5
    void DefaultRule_Shelly_2p5();
    #endif // USE_MODULE_TEMPLATE_SHELLY_2P5

    
    void parsesub_Rule_Part(JsonParserObject jobj, mRuleEngine::EVENT_PART* event);

    
    void Tasker_Rules_Interface(uint16_t function);
    // void Tasker_Rules_Init();

    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);


  #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    struct handler<mRuleEngine> mqtthandler_settings_teleperiod;
    // struct handler<mRuleEngine> mqtthandler_sensor_ifchanged;
    struct handler<mRuleEngine> mqtthandler_sensor_teleperiod;
    // struct handler<mRuleEngine> mqtthandler_scheduled_teleperiod;

    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_SCHEDULED_TELEPERIOD_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };

    
    struct handler<mRuleEngine>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_teleperiod
    };
  #endif //  USE_MODULE_NETWORK_MQTT

};

// #define mRuleEngineI RuleEngine::GetInstance()
// #define REI RuleEngineI



#endif

#endif //ifdef USE_MODULE_CORE_RULES