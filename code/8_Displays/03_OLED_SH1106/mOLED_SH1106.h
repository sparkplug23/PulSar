#ifndef _MODULE_DISPLAYS_OLED_SH1106_H
#define _MODULE_DISPLAYS_OLED_SH1106_H

#define D_UNIQUE_MODULE_DISPLAYS_OLED_SH1106_ID   131  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_DISPLAYS_OLED_SH1106_ID    1    // Numerical accesending order of module within a group

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DISPLAYS_OLED_SH1106

#define OLED_RESET 4

// #define SPRINT(A) char str[32];sprintf(str,"val: %d ",A);Serial.println((char*)str);

#define XI2C_04                4            // See I2CDEVICES.md

#define OLED_ADDRESS1          0x3C         // Oled 128x32 I2C address
#define OLED_ADDRESS2          0x3D         // Oled 128x64 I2C address

#define OLED_BUFFER_COLS       40           // Max number of columns in display shadow buffer
#define OLED_BUFFER_ROWS       16           // Max number of lines in display shadow buffer

#define OLED_FONT_WIDTH        6
#define OLED_FONT_HEIGTH       8

#define USE_DISPLAY_MODES1TO5

#include <Wire.h>
#include <renderer.h>
#include <Adafruit_SH1106.h>

class mOLED_SH1106 :
  public mTaskerInterface
{
  public:
	  mOLED_SH1106(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE_DISPLAYS_OLED_SH1106_CTR;
    static const char* PM_MODULE_DISPLAYS_OLED_SH1106_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DISPLAYS_OLED_SH1106_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DISPLAYS_OLED_SH1106_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DISPLAYS_OLED_SH1106_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mOLED_SH1106);
    };
    #endif
    void parse_JSONCommand(JsonParserObject obj);

    Adafruit_SH1106 *oled1106;

    void InitDriver(void);
    void ShowScrollingLog(void);
    void ShowStaticLog();
    void ShowUTCTime(void);
    void EverySecond(void);
    void RefreshDisplay();

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender();
    
    struct handler<mOLED_SH1106> mqtthandler_settings_teleperiod;
 
    struct handler<mOLED_SH1106>* mqtthandler_list[1] = {
      &mqtthandler_settings_teleperiod
    };

    // No specialised payload therefore use system default instead of enum
    
    
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
