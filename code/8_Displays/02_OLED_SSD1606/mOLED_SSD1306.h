#ifndef _MODULE_DISPLAYS_OLED_SSD1306_H
#define _MODULE_DISPLAYS_OLED_SSD1306_H

#define D_UNIQUE_MODULE_DISPLAYS_OLED_SSD1306_ID   8002 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306

#define OLED_RESET 4

#define SPRINT(A) char str[32];sprintf(str,"val: %d ",A);Serial.println((char*)str);

#define XI2C_04                4            // See I2CDEVICES.md

#define OLED_ADDRESS1          0x3C         // Oled 128x32 I2C address
#define OLED_ADDRESS2          0x3D         // Oled 128x64 I2C address

#define OLED_BUFFER_COLS       40           // Max number of columns in display shadow buffer
#define OLED_BUFFER_ROWS       16           // Max number of lines in display shadow buffer

#define OLED_FONT_WIDTH        6
#define OLED_FONT_HEIGTH       8


#include <Wire.h>
#include <renderer.h>
#include <Adafruit_SSD1306.h>

class mOLED_SSD1306 :
  public mTaskerInterface
{
  public:
	  mOLED_SSD1306(){};
    void Pre_Init(void);
    void Init(void);
    
    static constexpr const char* PM_MODULE_DISPLAYS_OLED_SSD1306_CTR = D_MODULE_DISPLAYS_OLED_SSD1306_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DISPLAYS_OLED_SSD1306_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DISPLAYS_OLED_SSD1306_ID; }

    void parse_JSONCommand(JsonParserObject obj);

    Adafruit_SSD1306 *oled1306;

    void InitDriver(void);
    void ShowScrollingLog(void);
    void ShowStaticLog();
    void ShowUTCTime(void);
    void EverySecond(void);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();
    struct handler<mOLED_SSD1306> mqtthandler_settings;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    
    
    uint8_t list_ids[1] = {
      MQTT_HANDLER_SETTINGS_ID
    };
    
    struct handler<mOLED_SSD1306>* list_ptr[1] = {
      &mqtthandler_settings
    };
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
