#ifndef MHEATING_H
#define MHEATING_H 0.4

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_CUSTOM_HEATING


#include <NeoPixelBus.h>

#include <ArduinoJson.h>
#include "1_TaskerManager/mTaskerManager.h"


#include <stdio.h>
#include <stdlib.h>

#include "2_CoreSystem/Time/mTime.h"

  #define USE_FAILSAFES
#define USE_HEATING_TIMERS
#define USE_HEATING_TEMPS

#define D_TASKNAME_HEATING "heating"

#define WEB_HANDLE_HEATING_TIMER "heat_timer"

//disable  for debugging
#define ENABLE_RELAY_CONTROLS

#define WEB_HANDLE_BUTTON_NAME_TEMP_SET "btte"
#define WEB_HANDLE_BUTTON_NAME_TIMER_SET "btti"

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Upstairs RELAY"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Downstairs RELAY"
  #define D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "Immersion RELAY"
  #define D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "Water Boiler RELAY"

  #define D_DHT_NAME_DOWNSTAIRS "Downstairs DHT"
  #define D_DHT_NAME_UPSTAIRS "Upstairs DHT"

  #define D_DB18_NAME_DOWNSTAIRS_PIPE "Downstairs_Pipe"
  #define D_DB18_NAME_UPSTAIRS_PIPE "Upstairs_Pipe"
  #define D_DB18_NAME_BOILER_PIPE "Boiler_Pipe"
  #define D_DB18_NAME_IMMERSION_HEATER "Immersion_Heater"
  #define D_DB18_NAME_TANK_TOP "Tank_Top"
  #define D_DB18_NAME_TANK_MIDDLE "Tank_Middle"
  #define D_DB18_NAME_TANK_BOTTOM "Tank_Bottom"
  #define D_DB18_NAME_TANK_OUT "Tank_Out"



#define D_HEATING_PROGRAM_TEMP "Program Temp > "
#define D_HEATING_PROFILES "Profiles > "



#define D_HEATING_SENSOR_NAME_SHORT_DS_LOWERCASE "ds"
#define D_HEATING_SENSOR_NAME_SHORT_US_LOWERCASE "us"
#define D_HEATING_SENSOR_NAME_SHORT_WB_LOWERCASE "wb"
#define D_HEATING_SENSOR_NAME_SHORT_IH_LOWERCASE "ih"
#define D_HEATING_SENSOR_NAME_SHORT_TT_LOWERCASE "tt"
#define D_HEATING_SENSOR_NAME_SHORT_TM_LOWERCASE "tm"
#define D_HEATING_SENSOR_NAME_SHORT_TB_LOWERCASE "tb"
#define D_HEATING_SENSOR_NAME_SHORT_TO_LOWERCASE "to"



#define D_HEATING_SENSOR_NAME_SHORT_DS "DS"
#define D_HEATING_SENSOR_NAME_SHORT_US "US"
#define D_HEATING_SENSOR_NAME_SHORT_WB "WB"
#define D_HEATING_SENSOR_NAME_SHORT_IH "IH"
#define D_HEATING_SENSOR_NAME_SHORT_TT "TT"
#define D_HEATING_SENSOR_NAME_SHORT_TM "TM"
#define D_HEATING_SENSOR_NAME_SHORT_TB "TB"
#define D_HEATING_SENSOR_NAME_SHORT_TO "TO"

DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_SHORT_DS_CTR)           D_HEATING_SENSOR_NAME_SHORT_DS;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_SHORT_US_CTR)           D_HEATING_SENSOR_NAME_SHORT_US;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_SHORT_WB_CTR)           D_HEATING_SENSOR_NAME_SHORT_WB;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_SHORT_IH_CTR)           D_HEATING_SENSOR_NAME_SHORT_IH;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_SHORT_TT_CTR)           D_HEATING_SENSOR_NAME_SHORT_TT;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_SHORT_TM_CTR)           D_HEATING_SENSOR_NAME_SHORT_TM;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_SHORT_TB_CTR)           D_HEATING_SENSOR_NAME_SHORT_TB;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_SHORT_TO_CTR)           D_HEATING_SENSOR_NAME_SHORT_TO;

#define D_TEMP_MODE_OFF_CTR "Mode Off"

DEFINE_PGM_CTR(PM_TEMP_MODE_OFF_CTR)  D_TEMP_MODE_OFF_CTR

#define D_HEATING_SENSOR_NAME_LONG_DS "Downstairs"
#define D_HEATING_SENSOR_NAME_LONG_US "Upstairs"
#define D_HEATING_SENSOR_NAME_LONG_WB "Boiler"
#define D_HEATING_SENSOR_NAME_LONG_IH "Immersion"
#define D_HEATING_SENSOR_NAME_LONG_TT "Tank Top"
#define D_HEATING_SENSOR_NAME_LONG_TM "Tank Middle"
#define D_HEATING_SENSOR_NAME_LONG_TB "Tank Bottom"
#define D_HEATING_SENSOR_NAME_LONG_TO "Tank Out"

// #define TEST_DEVICE_NAME


DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_LONG_DS_CTR)           D_HEATING_SENSOR_NAME_LONG_DS;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_LONG_US_CTR)           D_HEATING_SENSOR_NAME_LONG_US;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_LONG_WB_CTR)           D_HEATING_SENSOR_NAME_LONG_WB;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_LONG_IH_CTR)           D_HEATING_SENSOR_NAME_LONG_IH;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_LONG_TT_CTR)           D_HEATING_SENSOR_NAME_LONG_TT;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_LONG_TM_CTR)           D_HEATING_SENSOR_NAME_LONG_TM;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_LONG_TB_CTR)           D_HEATING_SENSOR_NAME_LONG_TB;
DEFINE_PROGMEM_CTR(PM_HEATING_SENSOR_NAME_LONG_TO_CTR)           D_HEATING_SENSOR_NAME_LONG_TO;

DEFINE_PROGMEM_CTR(PM_HEATING_SCHEDULED_NAME_OFF_CTR)            D_JSON_OFF;
DEFINE_PROGMEM_CTR(PM_HEATING_SCHEDULED_NAME_SET_CTR)            D_JSON_SCHEDULED_SET;
DEFINE_PROGMEM_CTR(PM_HEATING_SCHEDULED_NAME_ON_CTR)             D_JSON_SCHEDULED_ON;
DEFINE_PROGMEM_CTR(PM_HEATING_SCHEDULED_NAME_MANUAL_ON_CTR)      D_JSON_MANUAL_ON;


// Temp lookup table
// [15,16,17]
// if(15,16)
//   seconds = 120
// if(16,17)
//   seconds = 120

#define DEVICE_ID_TOTAL 4
enum DEVICE_HEATING_ID{
    DEVICE_US_ID=0,
    DEVICE_DS_ID=1,
    DEVICE_IH_ID=2,
    DEVICE_WB_ID=3
};

class mHeating{

  private:
  public:
    mHeating(){};
    void pre_init(void);
    uint8_t init_success = false;
    void init(void);    

    #include "9_Controller/Heating/mHeating_Web.h"

    

    
void init_dht22_sensor_parameters();

enum SENSORDB18_IDS{
  ID_DB18_DS=0,
  ID_DB18_US,
  ID_DB18_WB,
  ID_DB18_IH,
  ID_DB18_TT,
  ID_DB18_TM,
  ID_DB18_TB,
  ID_DB18_TO
};
    
enum SENSORDHT_IDS{
  ID_DHT_DS=0,
  ID_DHT_US,
};

enum DEVICELIST_ALL_IDS{
  DEVICELIST_RELAY_US=0,
  DEVICELIST_RELAY_DS,
  DEVICELIST_RELAY_IH,
  DEVICELIST_RELAY_WB,
  DEVICELIST_CLIMATE_US,
  DEVICELIST_CLIMATE_DS,
  DEVICELIST_WATER_US,
  DEVICELIST_WATER_DS,
  DEVICELIST_WATER_IH,
  DEVICELIST_WATER_WB,
  DEVICELIST_WATER_TT,
  DEVICELIST_WATER_TM,
  DEVICELIST_WATER_TB,
  DEVICELIST_WATER_TO,
  DEVICELIST_LENGTH
};

    
void init_db18_sensor_parameters();


void init_relay_driver_parameters();

    DeviceAddress thermo_addr_dsp = {0x28,0xFF,0x98,0xAB,0xC1,0x17,0x04,0xE7};
    DeviceAddress thermo_addr_usp = {0x28,0xFF,0x83,0x06,0xC2,0x17,0x04,0x3B};
    DeviceAddress thermo_addr_wbp = {0x28,0xFF,0xCC,0xE2,0xC1,0x17,0x04,0x1E};
    DeviceAddress thermo_addr_ih = {0x28,0xFF,0x88,0x69,0x35,0x16,0x04,0x72};
    DeviceAddress thermo_addr_tank_bottom = {0x28,0xFF,0x32,0xB0,0xC1,0x17,0x04,0xC5};
    DeviceAddress thermo_addr_tank_out = {0x28,0xFF,0xD8,0x6C,0x35,0x16,0x04,0x66};
    DeviceAddress thermo_addr_tank_top = {0x28,0xFF,0xA2,0xA7,0x35,0x16,0x04,0x1B};
    DeviceAddress thermo_addr_tank_middle = {0x28,0xFF,0xDB,0x5D,0x35,0x16,0x04,0xEF};

    // int8_t pin_ds = -1;
    // int8_t pin_us = -1;
    // int8_t pin_wb = -1;
    // int8_t pin_ih = -1;

    
    enum CLIMATESENSOR_DHT_IDS{
      DHT_DOWNSTAIRS_ID=0,
      DHT_UPSTAIRS_ID,
      DHT_COUNT_ID
    };

void Web_Append_Program_Timers_Buttons();
void Web_Append_Program_Temps_Buttons();

void WebPage_Root_SendBody();
void WebSendBody_Liveview();

void WebAppend_Root_Draw_Program_Buttons();
void WebAppend_Root_Draw_PageTable();


struct HEATING_STATUS{
  char message_ctr[50];
  char message_len = 0;
}heating_status[4];





void WebPage_Root_AddHandlers();

void WebAppend_Root_Status_Table();

void Web_Root_Draw(AsyncWebServerRequest *request);


void WebCommand_Parse(void);


uint8_t time_map_timers_upstairs[4];// = {0, 60, 90, 120};
uint8_t time_map_timers_downstairs[4];// = {0, 60, 90, 120};
uint8_t time_map_timers_immersion[4];// = {0, 60, 90, 120};
uint8_t time_map_timers_boiler[4];// = {0, 60, 90, 120};
uint8_t    time_map_temps_upstairs[4];//                  = {0, 20, 23, 25};
uint8_t    time_map_temps_downstairs[4];//                = {0, 20, 23, 25};
uint8_t    time_map_temps_immersion[4];//                 = {0, 30, 40, 50};
uint8_t    time_map_temps_boiler[4];//                    = {0, 30, 40, 50};


    struct SETTINGS{
      uint8_t  fEnableModule = true;
    }settings;



    
void HandleTimerConfiguration(void);
void HandleProgramTimerConfiguration();

    // *************** HEATINGPROFILES ****************************************************************************************************
    // This use existing heating and performance to estimate time until the desired temperature is reached
    #ifdef USE_HEATING_PROFILE_ESTIMATION
      #define HEATINGPROFILE_RESOLUTION 60
      struct HEATINGPROFILES{
        float temperature_max;
        float temperature_min;
        float temperature_step;
        float temperature[HEATINGPROFILE_RESOLUTION];
        uint32_t duration_secs[HEATINGPROFILE_RESOLUTION];
      }heating_profiles[4];
      void init_HeatingProfiles(); // read from memory
      void Save_HeatingProfiles();
      void Load_HeatingProfiles();
      uint32_t tSavedHeatingProfiles;
      void MQQTSendHeatingProfile_Raw_IfChanged(void);
      void ConstructJSON_HeatingProfile_Raw(uint8_t device_id);
      uint32_t GetHeatingProfilesTimeSeconds(uint8_t device_id, float temp_now, float temp_target);
      uint32_t GetHeatingProfilesTimeMinutes(uint8_t device_id, float temp_now, float temp_target);
    #endif

    // *************** TIMERS ****************************************************************************************************
    #define HEATING_DEVICE_TIMERS_MAX 4
    struct HEATING_DEVICE_TIMERS{
      uint8_t device_name;
      int16_t time_minutes_on;
      
      int16_t time_minutes_on_start;
      

      int16_t ischanged;
      int8_t user_id = -1; // who asked, -1 if not known
      
      uint8_t mapped_defaults[4];

    }program_timers[HEATING_DEVICE_TIMERS_MAX];
    uint32_t tSavedHeatingTimer;
    uint8_t isanychanged_timers=0;
    void HeatingTimers(void);
    uint8_t fForceHeatingTimerUpdate = false;

    
void AddToHardwareMessage();

    // *************** GOOGLE VOICE ****************************************************************************************************
    // enum GOOGLE_DEVICE_LIST_ID{GOOGLE_DEVICE_ID_1=0,GOOGLE_DEVICE_ID_2,GOOGLE_DEVICE_ID_3,GOOGLE_DEVICE_ID_4};
    // #define GOOGLE_DEVICE_NAME_1 "bedroom"
    // #define GOOGLE_DEVICE_NAME_2 "masterbedroom"
    // #define GOOGLE_DEVICE_NAME_3 "bathroom"
    // #define GOOGLE_DEVICE_NAME_4 "diningroom"

    // PHASING OUT NOT USEFUL

    // struct VOICESETTINGS{
    //   struct USER{
    //     struct DEVICE{
    //       uint8_t enabled = 0;
    //       uint8_t volume = 50;
    //       struct time_short send_time;
    //     }device[4]; //GOOGLE_DEVICE_LIST_ID
    //     // struct DEVICE bedroom;
    //     // struct DEVICE masterbedroom;
    //     // struct DEVICE bathroom;
    //     // struct DEVICE diningroom;
    //   }user[3]; // enum USERID{USERID_MICHAEL=1,USERID_DEIRDRE,USERID_RAYMOND};
    //   // struct USER michael;
    //   // struct USER deirdre;
    //   // struct USER raymond;
    // }voicesettings;

    // const char* GetGoogleDeviceNamebyIDCtr(uint8_t device_id);
    // const char* GetGoogleDeviceNameLongbyIDCtr(uint8_t device_id);
    // int8_t GetGoogleDeviceIDbyName(const char* c);

    // *************** TEMPS/SCHEDULES ****************************************************************************************************

    enum TEMP_MODE_IDS{
      TEMP_MODE_OFF_ID=0,
      TEMP_MODE_HEATING_ID,
      TEMP_MODE_MAINTAINING_ID,
      TEMP_MODE_SCHEDULED_ID,
      TEMP_MODE_SPLASH_RUN_TIME_ID,
      TEMP_MODE_LENGTH
    };
    enum SCH_MODE{SCHEDULED_OFF_ID=1,SCHEDULED_SET_ID,SCHEDULED_ON_ID,SCHEDULED_MANUAL_ON_ID};

    #define HEATING_DEVICE_TEMPS_MAX 4
    struct HEATING_DEVICE_TEMPS{
      uint8_t device_name; //phase out
      struct TIMERRUNNING{
        int16_t on=-1;
        int16_t limit=60;
      }time_running;
      struct TIMEMAINTAINING{ // all in minutes
        int16_t on=-1;
        int16_t limit=60;
      }time_maintaining;
      struct TIMETOHEAT{
        int16_t minutes=-1;
      }time_to_heat;
      struct SCHEDULE{ //change schedule to be MODE/Prorgame type
        struct datetime ontime;
        struct datetime untilontime;
        struct datetime offtime;
        uint8_t fHasRun = false;
        uint8_t mode_sch = SCHEDULED_OFF_ID;
        uint8_t mode_sch_aftersplash = SCHEDULED_OFF_ID;
        uint8_t fRunning = false;
      }schedule;
      struct TEMP{
        float current=0;
        float error=0;
        float desired=0;
        uint8_t mapped_defaults[4];
      }temp;
      struct STATUS{
        struct DATA{
          uint8_t ctr[80];
          uint8_t len = 0;
        }data;
        uint8_t mode = 0;
        uint8_t mode_next = 0; //stored for after splash
        uint32_t tSplashTime = millis();
        uint8_t msSplashtime = 2000; //seconds
      }status;
      struct MQTT{
        uint8_t fForceUpdate = false;
        uint32_t tSavedUpdate = millis();
        uint8_t rate = 5;
      }mqtt;
      int8_t user_id = -1; // who asked, -1 if not known
    }program_temps[HEATING_DEVICE_TEMPS_MAX];

    void SubTask_HeatingTemps_StatusMessage();
    void SubTask_HeatingTemps(void);
    void SubTask_HeatingTemps_Ticker();
    uint8_t fForceHeatingTempUpdate = false;

    void HeatingTemps(void);
    uint8_t fForceHeatingTempsUpdate = false;
    uint8_t fRunTemp = false;
    uint8_t fHeatingTempsChanged = false;
    uint8_t fForceHeatingTempsUpdateMQTT = true;
    uint8_t CheckIfROCExceedsNegativeLimit(uint8_t id);

// ***************************************** RGB COLOURS ***********************************************/

    #define LED_SIDE_VALUE 10 //brightness
    #define LED_PANEL_PIXELS 9
    uint8_t brightnessbycolour[12];
    uint32_t tLED;
    enum LED_NAME{LED_NAME_DOWN_LEFT=0,LED_NAME_DOWN_RIGHT=1,LED_NAME_UP_RIGHT=2,LED_NAME_UP_LEFT=3,LED_NAME_WB=4,LED_NAME_IH=5,LED_NAME_DS=6,LED_NAME_US=7};

    uint8_t GetPixelNumByName(const char* in);
    uint8_t fUpDownMode = SHOW_TANK;
    enum PANELMODEL{SHOW_TANK=1,SHOW_STORED_COLOURS,SHOW_OFF=3};
    void ConstructJSON_RGBPanelSettings();

    uint8_t GetRGBUpDownModeIDByName(const char* in);

    int mapHeatingTempToHueColour(int val);
    int mapHeatingTempToBrightness(int val);
    uint8_t ConstructSON_PipeTempsByColours(uint8_t json_level);
    void SubTasker_UnderStairsPixels();
    uint32_t tLED2;
    void SubTasker_TopDownPixels();
    uint8_t toggle_sender = false;
    

    uint8_t fForceUpdatePanel= true;
    uint32_t tSaved,tSaved2;

    struct colour{
      uint16_t h;
      uint8_t s,v;
    };

    struct STOREDINFO{
      float temp[12];
      uint8_t state[12];
    }stored_side;
    struct STOREDINFO stored_all;
    struct STOREDINFO stored_new;

    // struct PIXEL{
    //   colour c;
    // }pixel[LED_PANEL_PIXELS];

    // *************** RELAYS ****************************************************************************************************

    enum RELAY_STATES_ON{RELAY_STATE_OFF=0,RELAY_STATE_ON};
    void SetHeatingRelay(uint8_t device_id, uint8_t state);
    uint8_t GetHeatingRelay(uint8_t device_id);
    uint8_t GetAnyHeatingRelay();

    //PHASE OUT INTO RELAY DRIVERS (needs to be working first)
    struct HEATING_DEVICE_RELAYS{
      struct datetime ontime;
      struct datetime offtime; // can be used to find out how long its been off and "cold" for
      int16_t time_minutes_on;
      int16_t ischanged;
    }heating_device_relays[4];

    void MQQTSendHeatingRelaysIfChanged(void);
    uint8_t ConstructJSON_HeatingRelays(uint8_t json_level = 0);
    uint32_t tSavedHeatingRelays;

    // *************** PROGRAMS/GENERAL ****************************************************************************************************

    enum ACTICEPROGRAMMODE{ACTIVEP_OFF=0,ACTIVEP_ON=1,ACTIVEP_SCHEDULED=2};
    struct ACTIVEPRORGAMS2{
      struct PROGRAM{
        uint8_t state = 0;
      };
      PROGRAM relays;
      PROGRAM timers;
      PROGRAM temps;
    }activeprograms[4];
    const char* GetActiveProgramNameCtrbyID(uint8_t activeprogram_id, char* buffer);// = nullptr);

    void parse_JSONCommand(void);
    void parse_VoiceMQTT(void);

    void WebAppend_Root_ControlUI();


void WebAppend_Root_Draw_Table();

    void SubTasker_MQTTSender(void);

    void SendVoiceSettings(void);

    int8_t Tasker(uint8_t function);
    int8_t Tasker(uint8_t function, JsonObjectConst obj);

    
    void parsesub_CheckAll(JsonObjectConst obj);
    int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
    void parsesub_TopicCheck_JSONCommand(JsonObjectConst obj);
    
    void parsesub_ModeManual(JsonObjectConst obj);
    void parsesub_ProgramTimers(JsonObjectConst obj);
    void parsesub_ProgramTemps(JsonObjectConst obj);



    void SubTasker_HeatingTimers(void);
    void SubTasker_HeatingTemps(void);
    void SubTasker_ScheduledEvents(void);

    uint32_t tTick = millis();
    const char* GetScheduleNameCtrbyID(uint8_t mode, char* buffer, uint8_t buflen);// = nullptr);
    const char* GetDeviceNameLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buflen);// = nullptr);
    const char* GetDeviceNameUpperCaseCtrbyID(uint8_t device_id, char* buffer, uint8_t buflen);// = nullptr);

    void SetHighestImportance(uint8_t* importanceset, int8_t thisvalue);

    uint8_t fSendAllData = true;

    void SetHeater(uint8_t device_name, uint8_t state);
    int8_t GetHeater(uint8_t device);
    int8_t Tasker_PredictManualHeating();
    void SendVoiceMessage(unsigned char device_name, unsigned char state);
    void SendFormattedVoiceMessage(unsigned char device_name, unsigned char state);
    void SendMQTTAlertOnChangeofState(uint8_t device, uint8_t state);

    #define FAILSAFE_MINUTES_WARNING 150
    #define FAILSAFE_MINUTES_ERROR 170

    void MQQTSendPipesTempsByColoursIfChanged(void);

    uint8_t rateSavedSendActive = 1;

    void setfForceMQTTUpdate(uint8_t state);
    uint8_t getfForceMQTTUpdate(void);
    uint8_t getclearfForceMQTTUpdate(void);
    uint8_t fForceMQTTUpdate;

    const char* GetTempActiveProgramByDeviceIDCtr(uint8_t device, char* buffer);// = nullptr);

    uint32_t tSavedHeatingTemps,tSavedHeatingTemps2,tSavedHeatingTemps3;

    enum USERID{USERID_MICHAEL=0,USERID_DEIRDRE,USERID_RAYMOND};

    const char* GetDeviceNamebyIDCtr(uint8_t name, char* buffer, uint8_t buflen);// = nullptr);
    int8_t GetScheduleModeIDByCtr(const char* c);
    int8_t GetDeviceIDbyName(const char* c);
    int8_t GetUserIDbyName(const char* c);

    const char* GetUserNameByID(int8_t id, char* buffer);// = nullptr);

    struct FAILSAFES{
    uint32_t tSaved;
    int tSavedMinuteFailSafe;
    }failsafes;

    int8_t SubContructCtr_HardwareStatus();
    int8_t SubContructCtr_HardwareStatus_Long();


//MEMMROY CLEAN UP
    uint32_t tSavedSendActive;
    unsigned long tSavedSendClimate;
    unsigned long tSavedSlow;
    uint32_t tSavedSendRateOfChange=millis();
    uint8_t mSavedROC10m=0;
    uint32_t tSavedForceUpdate = millis();
    int cTicker;
    uint8_t fSendSingleFunctionData = false;
    const char* GetTempModeByDeviceIDCtr(uint8_t device, char* buffer, uint8_t buflen);// = nullptr);

    // *************** MQTT ****************************************************************************************************

    int8_t GetWaterSensorIDbyShortName(const char* c);
    float GetWaterTempsRawByID(uint8_t device_id);

    const char* GetSensorNameByID(uint8_t sensor_id, char* buffer);// = nullptr);
    const char* GetSensorNameLongbyID(uint8_t sensor_id, char* buffer);// = nullptr);

    void MQQTSendHealth();
    uint8_t ConstructJSON_Health(uint8_t json_level = 0);
    uint8_t ConstructJSON_ProgramTimers(uint8_t json_level = 0);
    uint8_t ConstructJSON_ProgramTemps(uint8_t json_level = 0);
    void MQQTSendRGBUpstairsStatus();
    uint8_t ConstructJSON_RGBUpstairsStatus();
    void MQQTSendProgramTimersIfChanged();
    void MQQTSendProgramTempsIfChanged();
    void MQQTSendProgramTempsForced();
    void MQQTSendPipesTempsIfChanged();
    uint8_t ConstructJSON_PipeTemps(uint8_t json_level = 0);
    void MQQTSendPipesTempsLatest1m();
    void MQQTSendPipesTempsLatest10m();
    void MQQTSendPipesTempsROC1m();
    uint8_t ConstructJSON_PipeTempsROC1m(uint8_t json_level = 0);
    void MQQTSendPipesTempsROC10m();
    uint8_t ConstructJSON_PipeTempsROC10m(uint8_t json_level = 0);
    void MQQTSendClimateTempsIfChanged();
    void MQQTSendClimateTempsLatest1m();
    void MQQTSendClimateTempsLatest10m();
    uint8_t ConstructJSON_ClimateTemps(uint8_t json_level = 0);
    void MQQTSendClimateTempsROC1m();
    uint8_t ConstructJSON_ClimateTempsROC1m(uint8_t json_level = 0);
    void MQQTSendClimateTempsROC10m();
    uint8_t ConstructJSON_ClimateTempsROC10m(uint8_t json_level = 0);
    void MQQTSendProgramsActive();
    uint8_t ConstructJSON_ProgramActive(uint8_t json_level = 0);
    void MQQTSendProgramTimersForced();
    void MQQTSendHardwareInfo();
    uint8_t ConstructJSON_HardwareInfo(uint8_t json_level = 0);
    void MQQTSendPipesTempsROC10s(void);
    uint8_t ConstructJSON_PipeTempsROC10s(uint8_t json_level = 0);

    uint32_t tSavedSendRateOfChange10s;

    enum VOICEREQUEST_ID{VOICEREQUEST_WATERTEMP_ID=1,VOICEREQUEST_CLIMATETEMP_ID,VOICEREQUEST_SCHEDULE_ID,VOICEREQUEST_OVERVIEW_ID};
    int8_t GetVoiceRequestIDbyName(const char* c);

    int8_t GetClimateSensorIDbyShortName(const char* c);
    const char* GetClimateSensorNameLongbyIDCtr(uint8_t sensor_id, char* buffer);// = nullptr);
    float GetClimateTempsRawByID(uint8_t sensor_id);

uint8_t ConstructJSON_Settings(uint8_t json_method);


//FunctionHandler_Init

    void FunctionHandler_Init();
    void FunctionHandler_Loop();
    struct functionhandler<mHeating>* functionhandler_ptr = nullptr;
  

    struct functionhandler<mHeating> functionhandler_status_message;
    void FunctionHandler_Program_Status();
    struct functionhandler<mHeating> functionhandler_failsafe;
    void FunctionHandler_FailSafe(void);
    struct functionhandler<mHeating> functionhandler_heating_profiles;
    void FunctionHandler_Heating_Profiles(void);
    struct functionhandler<mHeating> functionhandler_relay_status;
    void FunctionHandler_Relay_Status(void);
    struct functionhandler<mHeating> functionhandler_programs_timers;
    void FunctionHandler_Programs_Timers(void);
    struct functionhandler<mHeating> functionhandler_programs_temps;
    void FunctionHandler_Programs_Temps(void);


    // Hold list of handlers
    // struct functionhandler<mHeating>* functionhandler_list_ptr[] = {
    //   &functionhandler_status_message,
    //   &functionhandler_status_message
    // };
    

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mHeating>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mHeating> mqtthandler_settings_teleperiod;
    
    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_PROGRAM_TIMERS_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_PROGRAM_TIMERS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_PROGRAM_TEMPS_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_PROGRAM_TEMPS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_SENSOR_PIPES_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_SENSOR_PIPES_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_SENSOR_PIPES_ROC1M_ID,
      MQTT_HANDLER_MODULE_SENSOR_PIPES_ROC10M_ID,
      MQTT_HANDLER_MODULE_SENSOR_CLIMATE_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_SENSOR_CLIMATE_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_SENSOR_CLIMATE_ROC1M_ID,
      MQTT_HANDLER_MODULE_SENSOR_CLIMATE_ROC10M_ID,
      MQTT_HANDLER_MODULE_SENSOR_PIPES_COLOURS_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_SENSOR_PIPES_COLOURS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_DRIVERS_RELAYS_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_DRIVERS_RELAYS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };

    // const char* postfix_topic_program_timers = "program/timers";
    struct handler<mHeating> mqtthandler_program_timers_ifchanged;
    struct handler<mHeating> mqtthandler_program_timers_teleperiod;
    // const char* postfix_topic_program_temps = "program/temps";
    struct handler<mHeating> mqtthandler_program_temps_ifchanged;
    struct handler<mHeating> mqtthandler_program_temps_teleperiod;
    // const char* postfix_topic_program_overview = "program/overview"; //active
    struct handler<mHeating> mqtthandler_program_overview_ifchanged;
    struct handler<mHeating> mqtthandler_program_overview_teleperiod;
    // const char* postfix_topic_sensor_pipes = "sensors/pipes";
    struct handler<mHeating> mqtthandler_sensor_pipes_ifchanged;
    struct handler<mHeating> mqtthandler_sensor_pipes_teleperiod;
    struct handler<mHeating> mqtthandler_sensor_pipes_roc1m;
    struct handler<mHeating> mqtthandler_sensor_pipes_roc10m;
    // const char* postfix_topic_sensor_climate = "sensors/climate";
    struct handler<mHeating> mqtthandler_sensor_climate_ifchanged;
    struct handler<mHeating> mqtthandler_sensor_climate_teleperiod;
    struct handler<mHeating> mqtthandler_sensor_climate_roc1m;
    struct handler<mHeating> mqtthandler_sensor_climate_roc10m;
    // const char* postfix_topic_sensor_pipes_colours = "sensors/pipes/colours";
    struct handler<mHeating> mqtthandler_sensor_pipes_colours_ifchanged;
    struct handler<mHeating> mqtthandler_sensor_pipes_colours_teleperiod;
    // const char* postfix_topic_relays = "relays";
    struct handler<mHeating> mqtthandler_relays_ifchanged;
    struct handler<mHeating> mqtthandler_relays_teleperiod;
  //#endif


};

#endif

#endif
