#ifndef _MCEILINGFAN_H
#define _MCEILINGFAN_H 0.3

#include "1_ConfigUser/mUserConfig.h"

#ifdef USE_SONOFF_IFAN

#include "2_CoreSystem/InterfaceController/mInterfaceController.h"


#include <ArduinoJson.h>

#include <string.h>
#include <strings.h>

// #define D_TASKNAME_CEILINGFAN "ceilingfan" //Used as part of mqtt command

const uint8_t MAX_FAN_SPEED = 4;            // Max number of iFan02 fan speeds (0 .. 3)

const uint8_t kIFan02Speed[MAX_FAN_SPEED] = { 0x00, 0x01, 0x03, 0x05 };
const uint8_t kIFan03Speed[MAX_FAN_SPEED +2] = { 0x00, 0x01, 0x03, 0x04, 0x05, 0x06 };
const uint8_t kIFan03Sequence[MAX_FAN_SPEED][MAX_FAN_SPEED] = {{0, 2, 2, 2}, {0, 1, 2, 4}, {1, 1, 2, 5}, {4, 4, 5, 3}};


DEFINE_PROGMEM_CTR(kListFanControls) "Light|Off|Low|Medium|High";

class mSonoffIFan {

  private:
  public:
    mSonoffIFan(){};
    int8_t Tasker(uint8_t function);
    int8_t Tasker(uint8_t function, JsonObjectConst obj);   

int8_t Tasker_Web(uint8_t function);

// const char kSonoffIfanCommands[] PROGMEM = "|"  // No prefix
//   D_CMND_FANSPEED;

// void (* const SonoffIfanCommand[])(void) PROGMEM = {
//   &CmndFanspeed };

uint8_t ifan_fanspeed_timer = 0;
uint8_t ifan_fanspeed_goal = 0;
bool ifan_receive_flag = false;
bool ifan_restart_flag = true;


int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
int8_t parsesub_Commands(JsonObjectConst obj);

void init();

uint8_t GetLightState(void);
void SetLightState(uint8_t state);


bool IsModuleIfan(void);
uint8_t MaxFanspeed(void);
uint8_t GetFanspeed(void);
void SonoffIFanSetFanspeed(uint8_t fanspeed, bool sequence);
void SonoffIfanReceived(void);
bool SonoffIfanSerialInput(void);
void CmndFanspeed(void);
void SonoffIfanUpdate(void);


    // #define CEILINGFAN_TOGGLE  0xA55595
    // #define CEILINGFAN_DIMM    0xA55955
    // #define CEILINGFAN_SPEED0  0xA55655
    // #define CEILINGFAN_SPEED1  0xA55557
    // #define CEILINGFAN_SPEED2  0xA55565
    // #define CEILINGFAN_SPEED3  0xA5655B
    // #define ONE_BIT_TIME 1252 //+- 30 std
    // #define ZERO_BIT_TIME 439 //+- 30 std

    int8_t parse_JSONCommand();
    //void AddToJsonObject_AddHardware(JsonObject root);
    void AddToHardwareMessage();

    
void WebCommand_Parse(void);


void WebAppend_Root_Draw_PageTable();
void WebAppend_Root_Status_Table();


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mSonoffIFan>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    const char* postfix_topic_settings = "settings";
    struct handler<mSonoffIFan> mqtthandler_settings_teleperiod;
    
    const char* postfix_topic_sensors = "power";
    struct handler<mSonoffIFan> mqtthandler_sensor_ifchanged;
    struct handler<mSonoffIFan> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif