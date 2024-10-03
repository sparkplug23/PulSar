#ifndef _mTankVolume_H
#define _mTankVolume_H 0.7

/*
This Harlequin 1450VT single skin polyethylene oil storage tank is manufactured by Clarehill Plastics who were
established in 1981 and have become one of Europe's leading manufacturers of rotationally moulded fuel storage
products under the Harlequin brand. This tank is suitable for heating oil.

Specification:

Capacity:	1406 Litres / 309 Gallons
Length in mm:	1350
Width  in mm:	1350
Height in mm:	1260
Weight in kg when empty:	45
Outlet:	1" BSP Female
Fill Point:	2" BSP with cap & chain
Inspection Access:	Yes
OFCERT:	Yes
Manufacturer Warranty	2 Years

*/

// #define USE_MQTT_OILFURNACE

#define D_UNIQUE_MODULE_CONTROLLER_TANKVOLUME_ID   9008 // [(Folder_Number*100)+ID_File]

//https://www.weather.gov/media/epz/wxcalc/speedOfSound.pdf
/*
To convert between degrees Celsius (°C) and Kelvin (K):
Tc = Tk - 273.15
Tk = Tc + 273.15
Where: Tc is temperature in Celsius
T K is temperature in Kelvin



v_sound_inknots = 643.855 * pow((T_inkelvin/273.15),0.5)
Tcelius = Tkelvin - 273.

*/

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_TANKVOLUME

#include <math.h>


DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_LITRES_CTR) "litres";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_FURNACE_CTR) "furnace";

// const char kTitle_TableTitles_Root[] PROGMEM = 
//   "Row1" "|" 
//   "Update Rate/Speed" "|" 
//   "Update Amount" "|" 
//   "Pattern" "|" 
//   "Mode" "|" 
//   "Flasher Function" "|"
//   "Lighting Power" "|" 
//   "Lights Auto Off Timer";


//OilFurnace to be renamed "TankVolume" to be generic to oil or water

class mTankVolume :
  public mTaskerInterface
{ 
  public:

    mTankVolume(){};
    
    static constexpr const char* PM_MODULE_CONTROLLER_TANKVOLUME_CTR = D_MODULE_CONTROLLER_TANKVOLUME_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_TANKVOLUME_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_TANKVOLUME_ID; }


    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    #define ULTRA_ECHO_PIN D1
    #define ULTRA_TRIG_PIN D2
    // #define FURNACE_MONITOR_PIN D3
    #define LED_BLUE_PIN D4   // OUTPUT ONLY GPIO15
    #define DB_SENSOR_PIN D5

    
    #define WEB_HANDLE_JSON_OIL_SENSOR_TABLE "/fetch/tab_oil_specific.json"
// void WebPage_Root_AddHandlers();
// void WebSend_JSON_Table(AsyncWebServerRequest *request);
// void ConstructRoot_JSON_Table(JsonObject root);


  #ifdef DEVICE_LIVINGROOMSENSOR
    #define MAX_ULTRASONIC_MS 40000
    #define MIN_ULTRASONIC_MS 0
  #else
    #define MAX_ULTRASONIC_MS 8000
    #define MIN_ULTRASONIC_MS 3000
  #endif

// #ifdef FURNACE_MONITOR_PIN
//     // Reed switch active high (low when magnet is near)
//     #define FURNACEACTIVE() !digitalRead(FURNACE_MONITOR_PIN) //opened when high
//     #define FURNACEACTIVE_INIT() pinMode(FURNACE_MONITOR_PIN,INPUT_PULLUP)
//     #define FURNACEACTIVECTR FURNACEACTIVE() ? "On" : "Off"
//     struct FURNACEACTIVE_DETECT{
//       uint8_t state = false;
//       uint8_t isactive = false;
//       uint8_t ischanged = false;
//       uint32_t tSaved;
//       uint32_t tDetectTime;
//       //mytime event_time; // create function that gets and saves this ie event_time = TIMENOW();
//     }furnace_detect;
//  #endif

    // void (* const mof_Handlers[])(void) = { &pCONT->mof->init } ;



    // #define PIR_DETECT_INIT() pinMode(PIR_DETECT_PIN,INPUT_PULLUP)
    // #define PIR_DETECTED()    digitalRead(PIR_DETECT_PIN) //opened when high
    // Length in mm:	1350
    // Width  in mm:	1350
    // Height in mm:	1260
    /*
    Convert cubic centimeters to liters. To do this, use the conversion rate {\displaystyle 1\;{\text{liter}}=1,000cm^{3}}.
    Dividing the volume (in cubic centimeters) of the shape by 1,000 will give you the volume in liters (L).[6]
    Converting Cubic Centimeters to Liters Example
    If the volume of the fish tank, in cubic centimeters, is 20,975, to find the volume in liters, calculate
    {\displaystyle 20,975\div 1,000=20.975}. So, a fish tank that is 40.64 cm long, 25.4 cm wide, and 20.32 tall has a volume of 20.975 L.

    All measurements in mm from the bottom
    ___   Tank top = 1200mm
       |
       |
       |  Radius = 689mm
       |
       |
       | main/bottom threshold change height = 170mm
     |
     |
     -- pipe out = 90mm
     |
     |  Radius = 636mm
   __



        */
    //  #define TANK_HEIGHT 126 //to top

    #define TANK_HEIGHT_MM    1160
    #define TANK_DIAMETER_MM  1350
    #define TANK_RADIUS_MM 675

    #define TANK_HEIGHT_CM    120

    #define TANK_HEIGHT 120 //to sensor
    #define TANK_DIAMETER 135
    #define TANK_RADIUS 67.5

    // #define TANK_MAIN_RADIUS_MM 689.141*0.95
    // #define TANK_MAIN_RADIUS_CM 68.9141*0.95
    #define TANK_MAIN_RADIUS_MM 689.141//*0.95
    #define TANK_MAIN_RADIUS_CM 68.9141//*0.95
    #define TANK_BOTTOM_RADIUS_MM 636.62*0.96
    #define TANK_BOTTOM_RADIUS_CM 63.662*0.96

    #define TANK_MAINBOTTOM_THRESHOLD_HEIGHT_MM 170
    #define TANK_MAINBOTTOM_THRESHOLD_HEIGHT_CM 17
    #define TANK_PIPEOUT_HEIGHT_MM 90

    #define TANK_MAIN_RADIUSSQUARED_TIMES_PI_CM (3.14*TANK_MAIN_RADIUS_CM*TANK_MAIN_RADIUS_CM)
    #define TANK_BOTTOM_RADIUSSQUARED_TIMES_PI_CM (3.14*TANK_BOTTOM_RADIUS_CM*TANK_BOTTOM_RADIUS_CM)
    #define TANK_MAIN_RADIUSSQUARED_TIMES_PI_MM (3.14*TANK_MAIN_RADIUS_MM*TANK_MAIN_RADIUS_MM)
    #define TANK_BOTTOM_RADIUSSQUARED_TIMES_PI_MM (3.14*TANK_BOTTOM_RADIUS_MM*TANK_BOTTOM_RADIUS_MM)
    #define TANK_LITRES_BELOW_PIPE (3.14*TANK_BOTTOM_RADIUS_MM*TANK_BOTTOM_RADIUS_MM*90)/1000000 // ie 1000cm3 = 1L so 1000cm3 to 1000000mm3

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Init(void);
    void Pre_Init(void);
    uint32_t tUpdateOilReading;
    uint32_t tSaved;
    //float GetSpeedOfSoundInMetres();
    float stored_speedofsound_inmetres=0;
    uint32_t tCheckTime = millis(), tReadStoredLitres=millis();

    int GetDurationReading();
    int GetDurationReading2();
    float GetDistanceMMReading();
    float GetDistanceCMReading();
    // void SubTask_UltraSonicAverage();
    float GetOilHeightMMReading();
    float GetOilHeightCMReading();

    void init_ultrasonic_sensor_parameters();

    void EverySecond();

    float GetHeightToVolume_Custom_MyTank(float height);

    void SubTask_UpdateTankVolume();

        
    // void parsesub_CheckAll(JsonObjectConst obj);
    // int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
    void parse_JSONCommand(JsonParserObject obj);



    #define ADCSENSORS_MAX 60

    #define ADCSENSORS_SMOOTHSLOW_NUMREADINGS ADCSENSORS_MAX // @1hz ie 1 minute
    #define ADCSENSORS_SMOOTHSUPERSLOW_NUMREADINGS ADCSENSORS_MAX // @ EVERY 60 SECONDS hz ie 1 hour

    uint16_t pressure;
    void SubTask_CalculateOilVolume();
    void SubTask_CalculateOilVolumeAdjusted();
    void MQQTDataBuilder_TestingArray();
    void MQQTDataBuilder_UltraSonic2();
    float GetOilHeightMMReadingAdjustedFromTemp();
    // float GetDistanceMMReadingAdjustedForTemp();

    void WebAppend_Root_Draw_Table();
    void WebAppend_Root_Status_Table();
    void WebAppend_Root_ControlUI();

    uint8_t fUpdateCalculations = false;
    //uint8_t fFirstRun = true; // so I can clear captured

        // struct ULTRASONICSENSOR{
        //   uint32_t tReadLast;
        //   uint32_t tUltraSonicSensorReadLast;
        //   int duration;
        //   float temperature;
        //   uint32_t tPermitTempUpdate = 0;
        //   float speedofsound;
        //   uint8_t isvalid = false;
        //   uint8_t ischanged = false;
        //   struct ACCURACY{
        //     uint32_t insidecount;
        //     uint32_t outsidecount;
        //     float percent = 0;
        //   }accuracy;
        //   struct THRESHOLD{
        //     uint8_t setpercent = 0; //percent of original
        //     uint8_t narrowpercent = 0; // high precision PERCENT
        //     uint8_t widepercent = 0; // open to more wide values
        //     float lowervalue = 0; // open to more wide values
        //     float uppervalue = 0; // open to more wide values
        //     float ratio_pos = 0; // -1 = bottom of, 0 = exactly, +1 top of theshold
        //     float ratio_neg = 0; // -1 = bottom of, 0 = exactly, +1 top of theshold
        //     float relative = 0; // -1 = bottom of, 0 = exactly, +1 top of theshold
        //     uint8_t insidecount = 0;
        //     uint8_t outsidecount = 0;
        //   }threshold;
        // }ultrasonic;

    struct TANK{

      // struct DISTANCE_SETTINGS{
      //   bool flag_distance_is_relative_to_far_object = true; // ie, not distance from sensor, but relative to distant object eg tank bottom

      // }distance;

      float height_of_tank_cm = 0;

      float volume_litres = 0;
      /**
       * @brief Outlet pipe from tank is above the bottom, where that liquid is not usable
       * 
       */
      float volume_litres_notusable = 0;
      float volume_litres_usable = 0;

    }tank;



    // struct OILTANK{
    //   struct SENSOR{
    //     uint8_t isvalid = false;  //only valid after its been ran once completely
    //     uint8_t ischanged = false;
    //     struct ULTRASONIC_READINGS{
    //       float distance_cm;
    //       float distance_mm;
    //       uint32_t tLastChanged;
    //     }ultrasonic_readings;
    //     struct CAPTURED{
    //        float readings[ADCSENSORS_MAX];      // the readings from the analog input
    //        int readIndex = 0;              // the index of the current reading
    //        float total = 0;                  // the running total
    //        float average = 0;                // the average
    //        uint32_t tSaved;
    //        uint32_t tLastChanged;
    //    }captured;
    //   //  struct AVERAGING{
    //   //     float deviationarray[ADCSENSORS_MAX];
    //   //     float deviationsum = 0;
    //   //     float deviationaverage=0;
    //   //     int isoutlier[ADCSENSORS_MAX];
    //   //     int outliercount = 0;
    //   //     int usablecount = 0;
    //   //     int usableratio = 0;
    //   //     uint32_t tLastChanged;
    //   //  }averaging;
    //    struct FINAL{ // dont store cm, only mm
    //       float distance_from_bottom_cm;
    //       float distance_from_bottom_mm;
    //       float volume_of_oil_cm3; // not useful
    //       float volume_of_oil_mm3;
    //       float litres_in_tank;
    //       float millilitres_in_tank;
    //       float litres_of_usable_oil;
    //       uint32_t tLastChanged;
    //     }final;
    //   };
    //   struct SENSOR instant;
    //   struct SENSOR smooth_1m;
    //   struct SENSOR smooth_1hr;
    //   // struct SENSOR smooth_1hr_from_1m;
    //   struct SENSOR* ptr;
    // }oiltank; //caled averaged in ultra

/***
 * 
Move volume conversion to be a callback, so I can make them different by devices
 */

    void SubTask_CopyAveragedSensorValues();

    void SubTask_UltraSonicAverageToOilTank();

float GetOilHeightCMReading(int duration);
float GetOilHeightMMReading(int duration);
float GetOilHeightMMReadingAdjustedFromTemp(int duration);
float GetOilHeightCMReadingAdjustedFromTemp(int duration);


// void MQQTHandler_IfChanged_Basic_Litres(void);
// void MQQTHandler_IfChanged_Detailed_Litres(void);
// void ConstructJSON_Detailed(void);
// void ConstructJSON_Basic(void);


    // EEPROM ADDRESS
    // ints are 2 bytes
    #define EEPROM_IDX_DAYS_MAX 365
    #define EEPROM_IDX_DAYS_VARIABLE_SIZE sizeof(int) // for ints
    #define EEPROM_IDX_DAYS_START 0
    #define EEPROM_IDX_DAYS_END (EEPROM_IDX_DAYS_MAX*EEPROM_IDX_DAYS_VARIABLE_SIZE)
    #define EEPROM_GET_DAYS_ADDRESS(x) (x<=EEPROM_IDX_DAYS_END) ? (x*EEPROM_IDX_DAYS_VARIABLE_SIZE)+EEPROM_IDX_DAYS_START : -1 // ie return -1 if not within valid memory block

    #define EEPROM_IDX_POS_MAX 1
    #define EEPROM_IDX_POS_VARIABLE_SIZE sizeof(int) // for ints
    #define EEPROM_IDX_POS_START 0
    #define EEPROM_IDX_POS_END (EEPROM_IDX_POS_MAX*EEPROM_IDX_POS_VARIABLE_SIZE)
    #define EEPROM_GET_POS_ADDRESS(x) (x<=EEPROM_IDX_POS_END) ? (x*EEPROM_IDX_POS_VARIABLE_SIZE)+EEPROM_IDX_POS_START : -1 // ie return -1 if not within valid memory block

    void SubTask_RecordLitresOverDays();
    uint32_t tRecordLitresOverDays;


        struct HISTORY{
          struct PERWEEKDAYS{
            int litres_in_tank;
            float litres_change;
          }perweekdays[7];
          struct OVERVIEW{
            float litres_change;
          }overview;
          float litres_per_day;
          float days_remaining;
        }history;













        // #ifdef USE_MQTT_OILFURNACE
          void SubTasker_MQTTSender();
          uint8_t mqtt_start_data_sent = false;
          void MQQTSendSensorHardwareInfo(void);
          void MQQTDataBuilder_SensorHardwareInfo();
          void MQQTSendDB18TempsIfChanged();
          void MQQTSendDB18TempsLatest1m();
          uint32_t tTick = millis();

            uint32_t tSavedSendIfChanged,tSavedSendMinutes,tSavedSendOilTank;
            uint8_t mSavedSendMinutes;
            void MQQTSendUltraSonic(void);
            void MQQTSendHistoryLitresPerWeekday(void);
            void MQQTSendPracticalLitresInfo(void);
            void MQQTSendFurnaceIfChanged(void);
            void MQQTSendFurnaceLatest1m(void);
            void MQQTDataBuilder_Furnace(void);
            void MQQTRGBDataBuilder_Averaged1m();
            void MQQTSendSensorAveraged1m();
            void MQQTDataBuilder_UltraSonic();
            void MQQTDataBuilder_HistoryLitresPerWeekday();
            void MQQTDataBuilder_PracticalLitresInfo();
            void MQQTDataBuilder_PracticalLitresInfoAdjusted();
            void MQQTDataBuilder_UltraSonicAdjusted();
            void MQQTDataBuilder_SensorInfo();
            void MQQTDataBuilder_UltraSonic_Smooth1m_Captured();
            void MQQTDataBuilder_UltraSonic_Smooth1m_Basic();
            void MQQTDataBuilder_UltraSonic_Smooth1m_Detailed();
            void MQQTDataBuilder_LitresDetailed();
            void MQQTSendLitresDetailed();

      


/************************************************************************************************************************************************


******************************************************************************************************************************************************************************************************************/




    uint8_t ConstructJSON_Litres(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Furnace(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);


  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    struct handler<mTankVolume>* ptr;
    void MQTTHandler_Sender();

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mTankVolume> mqtthandler_settings;
    
    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_LITRES_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_LITRES_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_FURNACE_IFCHANGED_ID,
      MQTT_HANDLER_MODULE_FURNACE_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };

    // const char* postfix_topic_litres = "litres";
    struct handler<mTankVolume> mqtthandler_litres_ifchanged;
    struct handler<mTankVolume> mqtthandler_litres_teleperiod;
    // const char* postfix_topic_furnace = "furnace";
    struct handler<mTankVolume> mqtthandler_furnace_ifchanged;
    struct handler<mTankVolume> mqtthandler_furnace_teleperiod;
  //#endif

    struct handler<mTankVolume>* mqtthandler_list[5] = {
      &mqtthandler_settings,
      &mqtthandler_litres_ifchanged,
      &mqtthandler_litres_teleperiod,
      &mqtthandler_furnace_ifchanged,
      &mqtthandler_furnace_teleperiod
    };






};


#endif //module gaurd

#endif //header gaurd
