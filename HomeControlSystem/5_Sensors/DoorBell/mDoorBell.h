#ifndef _mDOORBELL_H
#define _mDOORBELL_H 0.1

#include "0_ConfigUser/mUserConfig.h"

//doorbellwallchime/set/pixels/mode/notif
//{"pixelnum":[0,8,9,10,11],"hue":30,"sat":60,"brt":100,"white":255}


#ifdef USE_MODULE_SENSORS_DOORCHIME

#include "1_TaskerManager/mTaskerManager.h"


class mDoorBell{
  public:
	  mDoorBell(){};  
    void pre_init(void);

    uint8_t BellSwitch_OnOff();
    const char* BellSwitch_OnOff_Ctr();
    void BellChime_Toggle();
    void BellChime_Set(uint8_t state);

    int8_t pin_doorbell_button = -1;
    int8_t pin_relay_chime = -1;

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
  void RingDoorBellSet(uint8_t freq, uint8_t seconds);
  uint8_t RingDoorBellLoop(uint8_t reset = false);

  enum BELLCHIME_STATE{BELLCHIME_OFF=0,BELLCHIME_ON=1};

  void WebCommand_Parse();

  struct RINGER{
    uint32_t start_millis;
    uint32_t end_millis;
    uint32_t toggle_millis;
    uint8_t fIsRinging = false;
    uint8_t freq = 1;
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

    
int8_t Tasker(uint8_t function, JsonObjectConst obj);
int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
void parse_JSONCommand(JsonObjectConst obj);


    void parsesub_Settings();
    void parsesub_Command();

    typedef struct GPIO_DETECT{
      uint8_t state = false;
      uint8_t isactive = false;
      uint8_t ischanged = false;
      struct datetime changedtime;
      uint32_t tSaved;
      uint32_t tDetectTimeforDebounce;      
      char friendly_name_ctr[15];
      char trigger_time_ctr[10]; //HH:MM:SS\0  IMPORTANT! COPIED INTO GARAGE LIGHT, CAUSES ERROR IF CHANGED
    };
    GPIO_DETECT doorbell_switch;

    int8_t Tasker(uint8_t function);

    //#ifdef MQTT
      void SubTasker_MQTTSender();
      void MQTTSendDoorSensorIfChanged();
      void MQQTSendDoorUpdate();
      uint32_t tSavedSendDoorSensor;
      uint8_t fUpdateSendDoorSensor;
    //#endif

    void WebPage_Root_AddHandlers();

    void EveryLoop();

    void WebAppend_Root_Status_Table_Draw();
    void WebAppend_Root_Status_Table_Data();

};

#endif

#endif
