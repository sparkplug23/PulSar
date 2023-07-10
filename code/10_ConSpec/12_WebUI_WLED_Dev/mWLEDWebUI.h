#ifndef _mWLEDWebUI_DEVELOPER_H
#define _mWLEDWebUI_DEVELOPER_H

#define D_UNIQUE_MODULE_CONTROLLER_CUSTOM__WLED_WEBUI_DEVELOPER__ID   ((10*1000)+12)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER


#include "webpages/html_ui.h"
#ifdef WLED_ENABLE_SIMPLE_UI
  #include "webpages/html_simple.h"
#endif
#include "webpages/html_settings.h"
#include "webpages/html_other.h"
#ifdef WLED_ENABLE_PIXART
  #include "webpages/html_pixart.h"
#endif
#ifndef WLED_DISABLE_PXMAGIC
  #include "webpages/html_pxmagic.h"
#endif
#include "webpages/html_cpal.h"



class mWLEDWebUI :
  public mTaskerInterface
{
  public:
    mWLEDWebUI(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Init(void);

    struct SETTINGS{
      bool enabled_module = true;
    }settings;


    static const char* PM_MODULE_CONTROLLER_CUSTOM__WLED_WEBUI_DEVELOPER__CTR;
    static const char* PM_MODULE_CONTROLLER_CUSTOM__WLED_WEBUI_DEVELOPER__FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_CUSTOM__WLED_WEBUI_DEVELOPER__CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_CUSTOM__WLED_WEBUI_DEVELOPER__FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_CUSTOM__WLED_WEBUI_DEVELOPER__ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mWLEDWebUI);
    };
    #endif
    
    void SubTasker_MQTTSender();

    void parse_JSONCommand(JsonParserObject obj);
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT

      void MQTTHandler_Init();
      void MQTTHandler_Set_RefreshAll();
      void MQTTHandler_Set_DefaultPeriodRate();
      
      void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

      struct handler<mWLEDWebUI> mqtthandler_settings_teleperiod;
      struct handler<mWLEDWebUI> mqtthandler_state_ifchanged;
      struct handler<mWLEDWebUI> mqtthandler_state_teleperiod;

      // No specialised payload therefore use system default instead of enum
      const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
      
      struct handler<mWLEDWebUI>* mqtthandler_list[3] = {
        &mqtthandler_settings_teleperiod,
        &mqtthandler_state_ifchanged,
        &mqtthandler_state_teleperiod
      };

    #endif // USE_MODULE_NETWORK_MQTT

};

#endif //DEVICE_RADIATORFAN

#endif
