#ifndef _mDOORBELL_H
#define _mDOORBELL_H 0.1

#define D_UNIQUE_MODULE_CONTROLLER_DOORBELL_ID 170

#include "1_TaskerManager/mTaskerManager.h"

// Consider moving the doorbell to controller, as buttons are inputs and relays are outputs

//doorbellwallchime/set/pixels/mode/notif
//{"pixelnum":[0,8,9,10,11],"hue":30,"sat":60,"brt":100,"white":255}


#ifdef USE_MODULE_CONTROLLER_DOORCHIME


class mDoorBell :
  public mTaskerInterface
{
  public:
	  mDoorBell(){};  
    void pre_init(void);

    uint8_t BellSwitch_OnOff();
    const char* BellSwitch_OnOff_Ctr();
    void BellChime_Toggle();
    void BellChime_Set(uint8_t state);

    
void EveryLoop();

    int8_t pin_doorbell_button = -1;
    int8_t pin_relay_chime = -1;
    
    static const char* PM_MODULE_CONTROLLER_DOORBELL_CTR;
    static const char* PM_MODULE_CONTROLLER_DOORBELL_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_DOORBELL_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_DOORBELL_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_DOORBELL_ID; }

    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mDoorBell);
    };
    #endif


    #define ON_LOGIC_LEVEL LOW

      #define BELLSWITCH_INIT()      pinMode(BELLSWITCH_PIN,INPUT) //Need to add extra external pullup
      #define BELLSWITCH_ONOFF()     !digitalRead(BELLSWITCH_PIN) //opened when high
      #define BELLSWITCH_ON()        digitalWrite(BELLSWITCH_PIN,ON_LOGIC_LEVEL) //opened when high
      #define BELLSWITCH_OFF()       digitalWrite(BELLSWITCH_PIN,!ON_LOGIC_LEVEL) //opened when high
      #define BELLSWITCH_SET(X)      digitalWrite(BELLSWITCH_PIN,!X) //opened when high
      #define BELLSWITCH_ONOFF_CTR() BELLSWITCH_ONOFF() ? "ON" : "OFF"   

      #define BELLCHIME_INIT()      pinMode(BELLCHIME_PIN,OUTPUT) //Need to add extra external pullup
      #define BELLCHIME_SET(X)      digitalWrite(BELLCHIME_PIN,!X) //on when low
      #define BELLCHIME_TOGGLE()    digitalWrite(BELLCHIME_PIN,!digitalRead(BELLCHIME_PIN)) //on when low


    //#endif

  uint8_t test_state = 0;
  void RingDoorBellSet(uint8_t seconds = 2, uint16_t freq = 500); //default half second ie 500ms
  uint8_t RingDoorBellLoop(uint8_t reset = false);

  enum BELLCHIME_STATE{BELLCHIME_OFF=0,BELLCHIME_ON=1};

  void WebCommand_Parse();

  struct RINGER{
    uint32_t start_millis;
    uint32_t end_millis;
    uint32_t toggle_millis;
    uint8_t fIsRinging = false;
    uint16_t freq = 1;
    uint8_t seconds = 1;
    uint32_t closed_millis_end; // when "on" is reset

    char friendly_name_ctr[15];
    char trigger_time_ctr[10]; //HH:MM:SS\0  IMPORTANT! COPIED INTO GARAGE LIGHT, CAUSES ERROR IF CHANGED

    
  }ringer; 

    void init(void);

    // Going forward, user definable values will be contained in "Settings"
    struct SETTINGS{
      uint8_t fEnable_Switch_Relay_Binding = false; //after testing its true


    }settings;

    
    int8_t Tasker(uint8_t function);
    int8_t CheckAndExecute_JSONCommands(void);
    void parse_JSONCommand(void);


    void parsesub_Settings();
    void parsesub_Command();

    typedef struct GPIO_DETECT{
      uint8_t state = false;
      uint8_t isactive = false;
      uint8_t ischanged = false;
      // struct datetime changedtime;
      uint32_t tSaved;
      uint32_t tDetectTimeforDebounce;    
  time_short_t detected_time;  
      char friendly_name_ctr[15];
      char trigger_time_ctr[10]; //HH:MM:SS\0  IMPORTANT! COPIED INTO GARAGE LIGHT, CAUSES ERROR IF CHANGED
    };
    GPIO_DETECT doorbell_switch;


    //#ifdef MQTT
      // void SubTasker_MQTTSender();
      // void MQTTSendDoorSensorIfChanged();
      // void MQQTSendDoorUpdate();
      uint32_t tSavedSendDoorSensor;
      uint8_t fUpdateSendDoorSensor;
    //#endif

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
    
    struct handler<mDoorBell>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mDoorBell> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
    struct handler<mDoorBell> mqtthandler_sensor_ifchanged;
    struct handler<mDoorBell> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif
