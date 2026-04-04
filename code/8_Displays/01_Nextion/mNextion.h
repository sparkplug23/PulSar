#ifndef _mNextion2_H
#define _mNextion2_H

#define D_UNIQUE_MODULE_DISPLAYS_NEXTION_ID   8001 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION

#define SERIAL_NEXTION_RX Serial2
#define SERIAL_NEXTION_TX Serial2


#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebServer.h>

#ifdef ESP32
  #include <WiFi.h>
  #ifndef DISABLE_NETWORK
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
  #endif // DISABLE_NETWORK
#elif defined(ESP8266)
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
#endif
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>


#include "Web/Gen/generated_web.h"


class mNextion :
  public mTaskerInterface
{
  public:
  /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mNextion(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);

    static constexpr const char* PM_MODULE_DISPLAYS_NEXTION_CTR = D_MODULE_DISPLAYS_NEXTION_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DISPLAYS_NEXTION_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DISPLAYS_NEXTION_ID; } 
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    struct SETTINGS{
      uint8_t brightness_percentage = 0;
      int8_t page = 0;
      uint8_t page_saved = 0; //used to return to after message is flashed      
      uint8_t dynamic_log_level = 10;//9;// LOG_LEVEL_DEBUG_MORE; // used for certain addlog that may only have elevated states (to block large serial prints on recursive array prints)
      struct TimeoutCheck{
        uint8_t timeout_period = 120;
      }timeout_check;
      bool transmit_addlog_message = 0;
    }settings;


    void Load_Module(bool erase);
    void Save_Module(void);
    bool Default_Module(void);



    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/



    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);


    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    void MQTTHandler_Sender();

    std::vector<struct handler<mNextion>*> mqtthandler_list;
    struct handler<mNextion> mqtthandler_settings;
    struct handler<mNextion> mqtthandler_sensor_ifchanged;
    struct handler<mNextion> mqtthandler_sensor_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT


    enum INSTRUCTION_SET_RETURN_CODES : uint8_t
    {
      // https://nextion.tech/instruction-set/
      /********************************************************************************************************************
       * @brief Nextion Instruction Set Return Codes (bkcmd-dependent and unsolicited events)
       *
       * Notes:
       *  - When bkcmd is 2 or 3, Nextion returns a 4-byte response for most command results:
       *      <code> 0x?? 0xFF 0xFF 0xFF </code>
       *  - Some messages are unsolicited/event-driven and have different lengths (e.g., touch events).
       *
       *********************************************************************************************************************/

      /********************************************************************************************************************
       * @section bkcmd-dependent return codes (bkcmd > 0)
       * @brief Standard command result responses (typically length 4).
       *********************************************************************************************************************/

      /**
       * @NO:          1
       * @BYTE:        0x00
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid Instruction
       * @FORMAT:      0x00 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when instruction sent by user has failed.
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_INSTRUCTION = 0x00,

      /**
       * @NO:          2
       * @BYTE:        0x01
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Instruction Successful
       * @FORMAT:      0x01 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when instruction sent by user was successful.
       */
      INSTRUCTION_SET_RETURN_CODE__INSTRUCTION_SUCCESSFUL = 0x01,

      /**
       * @NO:          3
       * @BYTE:        0x02
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid Component ID
       * @FORMAT:      0x02 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when invalid Component ID or name was used.
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_COMPONENT_ID = 0x02,

      /**
       * @NO:          4
       * @BYTE:        0x03
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid Page ID
       * @FORMAT:      0x03 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when invalid Page ID or name was used.
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_PAGE_ID = 0x03,

      /**
       * @NO:          5
       * @BYTE:        0x04
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid Picture ID
       * @FORMAT:      0x04 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when invalid Picture ID was used.
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_PICTURE_ID = 0x04,

      /**
       * @NO:          6
       * @BYTE:        0x05
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid Font ID
       * @FORMAT:      0x05 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when invalid Font ID was used.
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_FONT_ID = 0x05,

      /**
       * @NO:          7
       * @BYTE:        0x06
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid File Operation
       * @FORMAT:      0x06 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when File operation fails.
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_FILE_OPERATION = 0x06,

      /**
       * @NO:          8
       * @BYTE:        0x09
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid CRC
       * @FORMAT:      0x09 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when Instructions with CRC validation fails their CRC check.
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_CRC = 0x09,

      /**
       * @NO:          9
       * @BYTE:        0x11
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid Baud rate Setting
       * @FORMAT:      0x11 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when invalid Baud rate was used.
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_BAUD_RATE = 0x11,

      /**
       * @NO:          10
       * @BYTE:        0x12
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid Waveform ID or Channel #
       * @FORMAT:      0x12 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when invalid Waveform ID or Channel # was used.
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_WAVEFORM_ID_OR_CHANNEL = 0x12,

      /**
       * @NO:          11
       * @BYTE:        0x1A
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid Variable name or attribute
       * @FORMAT:      0x1A 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when invalid Variable name or invalid attribute was used.
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_VARIABLE_NAME_OR_ATTRIBUTE = 0x1A,

      /**
       * @NO:          12
       * @BYTE:        0x1B
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid Variable Operation
       * @FORMAT:      0x1B 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when Operation of Variable is invalid.
       *               Example: Text assignment t0.txt=abc or t0.txt=23,
       *                        Numeric assignment j0.val="50" or j0.val=abc
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_VARIABLE_OPERATION = 0x1B,

      /**
       * @NO:          13
       * @BYTE:        0x1C
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Assignment failed to assign
       * @FORMAT:      0x1C 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when attribute assignment failed to assign.
       */
      INSTRUCTION_SET_RETURN_CODE__ASSIGNMENT_FAILED = 0x1C,

      /**
       * @NO:          14
       * @BYTE:        0x1D
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     EEPROM Operation failed
       * @FORMAT:      0x1D 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when an EEPROM Operation has failed.
       */
      INSTRUCTION_SET_RETURN_CODE__EEPROM_OPERATION_FAILED = 0x1D,

      /**
       * @NO:          15
       * @BYTE:        0x1E
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Invalid Quantity of Parameters
       * @FORMAT:      0x1E 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when the number of instruction parameters is invalid.
       */
      INSTRUCTION_SET_RETURN_CODE__INVALID_PARAMETER_QUANTITY = 0x1E,

      /**
       * @NO:          16
       * @BYTE:        0x1F
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     IO Operation failed
       * @FORMAT:      0x1F 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when an IO operation has failed.
       */
      INSTRUCTION_SET_RETURN_CODE__IO_OPERATION_FAILED = 0x1F,

      /**
       * @NO:          17
       * @BYTE:        0x20
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Escape Character Invalid
       * @FORMAT:      0x20 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when an unsupported escape character is used.
       */
      INSTRUCTION_SET_RETURN_CODE__ESCAPE_CHARACTER_INVALID = 0x20,

      /**
       * @NO:          18
       * @BYTE:        0x23
       * @BKCMD:       2,3
       * @LENGTH:      4
       * @MEANING:     Variable name too long
       * @FORMAT:      0x23 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when variable name is too long.
       *               Max length is 29 characters: 14 for page + "." + 14 for component.
       */
      INSTRUCTION_SET_RETURN_CODE__VARIABLE_NAME_TOO_LONG = 0x23,

      /********************************************************************************************************************
       * @section bkcmd-independent / event return codes (unsolicited or always valid)
       * @brief These may be returned regardless of bkcmd settings.
       *********************************************************************************************************************/

      /**
       * @NO:          19
       * @BYTE:        0x24
       * @LENGTH:      4
       * @MEANING:     Serial Buffer Overflow
       * @FORMAT:      0x24 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when a Serial Buffer overflow occurs.
       *               Buffer will continue to receive the current instruction, all previous instructions are lost.
       */
      INSTRUCTION_SET_RETURN_CODE__SERIAL_BUFFER_OVERFLOW = 0x24,

      /**
       * @NO:          20
       * @BYTE:        0x65
       * @LENGTH:      7
       * @MEANING:     Touch Event
       * @FORMAT:      0x65 0x00 0x01 0x01 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when Touch occurs and component’s corresponding "Send Component ID"
       *               is enabled in the HMI design.
       *               Byte2: page number
       *               Byte3: component ID
       *               Byte4: event (0x01 Press, 0x00 Release)
       *               Example: 0x65 0x00 0x01 0x01 ...
       */
      INSTRUCTION_SET_RETURN_CODE__TOUCH_EVENT = 0x65,

      /**
       * @NO:          21
       * @BYTE:        0x66
       * @LENGTH:      5
       * @MEANING:     Current Page Number
       * @FORMAT:      0x66 0x01 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when the sendme command is used.
       *               Byte2: current page number.
       */
      INSTRUCTION_SET_RETURN_CODE__CURRENT_PAGE_NUMBER = 0x66,

      /**
       * @NO:          22
       * @BYTE:        0x67
       * @LENGTH:      9
       * @MEANING:     Touch Coordinate (awake)
       * @FORMAT:      0x67 xH xL yH yL evt 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when sendxy=1 and not in sleep mode.
       *               X and Y are big-endian 16-bit coordinates.
       *               evt: 0x01 Press, 0x00 Release
       */
      INSTRUCTION_SET_RETURN_CODE__TOUCH_COORDINATE_AWAKE = 0x67,

      /**
       * @NO:          23
       * @BYTE:        0x68
       * @LENGTH:      9
       * @MEANING:     Touch Coordinate (sleep)
       * @FORMAT:      0x68 xH xL yH yL evt 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when sendxy=1 and exiting sleep mode.
       *               X and Y are big-endian 16-bit coordinates.
       *               evt: 0x01 Press, 0x00 Release
       */
      INSTRUCTION_SET_RETURN_CODE__TOUCH_COORDINATE_SLEEP = 0x68,

      /**
       * @NO:          24
       * @BYTE:        0x70
       * @LENGTH:      Varied (string + 3 terminators)
       * @MEANING:     String Data Enclosed
       * @FORMAT:      0x70 <chars...> 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when using get command for a string.
       *               Each byte between 0x70 and terminators is ASCII char data.
       */
      INSTRUCTION_SET_RETURN_CODE__STRING_DATA_ENCLOSED = 0x70,

      /**
       * @NO:          25
       * @BYTE:        0x71
       * @LENGTH:      8
       * @MEANING:     Numeric Data Enclosed
       * @FORMAT:      0x71 b0 b1 b2 b3 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when get command returns a number.
       *               32-bit value is little-endian (b0 is LSB).
       */
      INSTRUCTION_SET_RETURN_CODE__NUMERIC_DATA_ENCLOSED = 0x71,

      /**
       * @NO:          26
       * @BYTE:        0x86
       * @LENGTH:      4
       * @MEANING:     Auto Entered Sleep Mode
       * @FORMAT:      0x86 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when Nextion enters sleep automatically.
       *               Using sleep=1 will not return 0x86.
       */
      INSTRUCTION_SET_RETURN_CODE__AUTO_ENTERED_SLEEP_MODE = 0x86,

      /**
       * @NO:          27
       * @BYTE:        0x87
       * @LENGTH:      4
       * @MEANING:     Auto Wake from Sleep
       * @FORMAT:      0x87 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when Nextion leaves sleep automatically.
       *               Using sleep=0 will not return 0x87.
       */
      INSTRUCTION_SET_RETURN_CODE__AUTO_WAKE_FROM_SLEEP = 0x87,

      /**
       * @NO:          28
       * @BYTE:        0x88
       * @LENGTH:      4
       * @MEANING:     Nextion Ready
       * @FORMAT:      0x88 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when Nextion has powered up and initialized successfully.
       *               Since Nextion Editor v1.65.0, this may be user-controlled via Program.s printh.
       */
      INSTRUCTION_SET_RETURN_CODE__NEXTION_READY = 0x88,

      /**
       * @NO:          29
       * @BYTE:        0x89
       * @LENGTH:      4
       * @MEANING:     Start microSD Upgrade
       * @FORMAT:      0x89 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when power on detects inserted microSD and begins upgrade-by-SD process.
       */
      INSTRUCTION_SET_RETURN_CODE__START_MICROSD_UPGRADE = 0x89,

      /**
       * @NO:          30
       * @BYTE:        0xFD
       * @LENGTH:      4
       * @MEANING:     Transparent Data Finished
       * @FORMAT:      0xFD 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when all requested bytes of Transparent Data mode have been received,
       *               and device is now leaving transparent data mode.
       */
      INSTRUCTION_SET_RETURN_CODE__TRANSPARENT_DATA_FINISHED = 0xFD,

      /**
       * @NO:          31
       * @BYTE:        0xFE
       * @LENGTH:      4
       * @MEANING:     Transparent Data Ready
       * @FORMAT:      0xFE 0xFF 0xFF 0xFF
       * @DESCRIPTION: Returned when requesting Transparent Data mode, and device is now ready to begin
       *               receiving the specified quantity of data.
       */
      INSTRUCTION_SET_RETURN_CODE__TRANSPARENT_DATA_READY = 0xFE,
    };


    /**
     * @brief Convert a Nextion return-code byte into a stable, zero-allocation name string.
     *
     * Design goals (per your request):
     *  - Standalone function (no caller-owned buffer needed)
     *  - Returns a pointer to static storage (string literal), safe to use immediately in logs
     *  - No heap allocation, no String usage
     *
     * Usage:
     *   const uint8_t code = nextionReturnBuffer[0];
     *   ALOG_INF(PSTR("Nextion RX: code=0x%02X (%s)"), code, GetInstructionName(code));
     */
    static inline const char* GetInstructionName(uint8_t code)
    {
      switch (code)
      {
        // ---- bkcmd-dependent result codes (length 4) ----
        case INSTRUCTION_SET_RETURN_CODE__INVALID_INSTRUCTION:               return "INVALID_INSTRUCTION";                 // 0x00
        case INSTRUCTION_SET_RETURN_CODE__INSTRUCTION_SUCCESSFUL:            return "INSTRUCTION_SUCCESSFUL";             // 0x01
        case INSTRUCTION_SET_RETURN_CODE__INVALID_COMPONENT_ID:              return "INVALID_COMPONENT_ID";               // 0x02
        case INSTRUCTION_SET_RETURN_CODE__INVALID_PAGE_ID:                   return "INVALID_PAGE_ID";                    // 0x03
        case INSTRUCTION_SET_RETURN_CODE__INVALID_PICTURE_ID:                return "INVALID_PICTURE_ID";                 // 0x04
        case INSTRUCTION_SET_RETURN_CODE__INVALID_FONT_ID:                   return "INVALID_FONT_ID";                    // 0x05
        case INSTRUCTION_SET_RETURN_CODE__INVALID_FILE_OPERATION:            return "INVALID_FILE_OPERATION";             // 0x06
        case INSTRUCTION_SET_RETURN_CODE__INVALID_CRC:                       return "INVALID_CRC";                        // 0x09
        case INSTRUCTION_SET_RETURN_CODE__INVALID_BAUD_RATE:                 return "INVALID_BAUD_RATE";                  // 0x11
        case INSTRUCTION_SET_RETURN_CODE__INVALID_WAVEFORM_ID_OR_CHANNEL:    return "INVALID_WAVEFORM_ID_OR_CHANNEL";     // 0x12
        case INSTRUCTION_SET_RETURN_CODE__INVALID_VARIABLE_NAME_OR_ATTRIBUTE:return "INVALID_VARIABLE_NAME_OR_ATTRIBUTE"; // 0x1A
        case INSTRUCTION_SET_RETURN_CODE__INVALID_VARIABLE_OPERATION:        return "INVALID_VARIABLE_OPERATION";         // 0x1B
        case INSTRUCTION_SET_RETURN_CODE__ASSIGNMENT_FAILED:                 return "ASSIGNMENT_FAILED";                  // 0x1C
        case INSTRUCTION_SET_RETURN_CODE__EEPROM_OPERATION_FAILED:           return "EEPROM_OPERATION_FAILED";            // 0x1D
        case INSTRUCTION_SET_RETURN_CODE__INVALID_PARAMETER_QUANTITY:        return "INVALID_PARAMETER_QUANTITY";         // 0x1E
        case INSTRUCTION_SET_RETURN_CODE__IO_OPERATION_FAILED:               return "IO_OPERATION_FAILED";                // 0x1F
        case INSTRUCTION_SET_RETURN_CODE__ESCAPE_CHARACTER_INVALID:          return "ESCAPE_CHARACTER_INVALID";           // 0x20
        case INSTRUCTION_SET_RETURN_CODE__VARIABLE_NAME_TOO_LONG:            return "VARIABLE_NAME_TOO_LONG";             // 0x23

        // ---- bkcmd-independent / event codes ----
        case INSTRUCTION_SET_RETURN_CODE__SERIAL_BUFFER_OVERFLOW:            return "SERIAL_BUFFER_OVERFLOW";             // 0x24
        case INSTRUCTION_SET_RETURN_CODE__TOUCH_EVENT:                       return "TOUCH_EVENT";                        // 0x65
        case INSTRUCTION_SET_RETURN_CODE__CURRENT_PAGE_NUMBER:               return "CURRENT_PAGE_NUMBER";                // 0x66
        case INSTRUCTION_SET_RETURN_CODE__TOUCH_COORDINATE_AWAKE:            return "TOUCH_COORDINATE_AWAKE";             // 0x67
        case INSTRUCTION_SET_RETURN_CODE__TOUCH_COORDINATE_SLEEP:            return "TOUCH_COORDINATE_SLEEP";             // 0x68
        case INSTRUCTION_SET_RETURN_CODE__STRING_DATA_ENCLOSED:              return "STRING_DATA_ENCLOSED";               // 0x70
        case INSTRUCTION_SET_RETURN_CODE__NUMERIC_DATA_ENCLOSED:             return "NUMERIC_DATA_ENCLOSED";              // 0x71
        case INSTRUCTION_SET_RETURN_CODE__AUTO_ENTERED_SLEEP_MODE:           return "AUTO_ENTERED_SLEEP_MODE";            // 0x86
        case INSTRUCTION_SET_RETURN_CODE__AUTO_WAKE_FROM_SLEEP:              return "AUTO_WAKE_FROM_SLEEP";               // 0x87
        case INSTRUCTION_SET_RETURN_CODE__NEXTION_READY:                     return "NEXTION_READY";                      // 0x88
        case INSTRUCTION_SET_RETURN_CODE__START_MICROSD_UPGRADE:             return "START_MICROSD_UPGRADE";              // 0x89
        case INSTRUCTION_SET_RETURN_CODE__TRANSPARENT_DATA_FINISHED:         return "TRANSPARENT_DATA_FINISHED";          // 0xFD
        case INSTRUCTION_SET_RETURN_CODE__TRANSPARENT_DATA_READY:            return "TRANSPARENT_DATA_READY";             // 0xFE

        default:                                                             return "UNKNOWN_RETURN_CODE";
      }
    }

    HardwareSerial* display = nullptr;

    void ShowMessage_On_FlashScreen(const char* msg, int8_t seconds = 5, uint32_t bg_rgb = 0, uint32_t fg_rgb = 0xFFFFFF);
    struct FlashScreenState {
      int8_t   seconds_left = -1;   // -1 = idle
      uint8_t  page_flash   = 7;
      uint8_t  page_return  = 0;
      bool     running      = false;
    } flash;


    void parsesub_FlashMessage();

    uint8_t fEnableImmediateButtonTime = false;
    uint8_t fEnableIgnoreNextOffEvent = false;    
    struct LAST_SCREEN_PRESS{
      uint8_t   page = 0;
      uint8_t   event = 0;
      uint16_t  duration = 0;
      uint32_t  tSavedButtonONEvent = 0;
      uint32_t  tSavedButtonOFFEvent = 0;
      uint32_t  tSavedButtonONDurationEvent = 0;
      uint8_t   fEnableImmediateButtonTime = false;
    }screen_press;


    #define LONG_PRESS_DURATION 1000

    void EverySecond_ActivityCheck();
    void EverySecond_FlashScreen();
    std::string& replace(std::string& s, const std::string& from, const std::string& to);
    void EveryLoop();
    void Show_ConnectionWorking();
    void Show_ConnectionNotWorking();
    void MQTTSend_LongPressEvent();
    uint32_t tSaved_MQTTSend_PressEvent = millis();
    void MQTTSend_PressEvent();
    void nextionHandleInput();  
    void nextionSetAttr(const char* hmiAttribute, const char* hmiValue);
    void nextionSetAttr(const char* hmiAttribute, uint32_t value);
    bool nextionOtaResponse();
    bool nextionConnect();
    void nextionSetSpeed();
    void nextionReset();
    bool update_in_progress = false;
    void nextionUpdateProgress(const unsigned int &progress, const unsigned int &total);
    void WebPage_AddHandlers();
    void CommandSet_Baud(uint32_t baud);    
    byte utf8ascii(byte ascii);
    String utf8ascii(String s);
    void nextionSendCmd_String(const String &nextionCmd);
    void Command_SetPage(uint8_t page);
    void Command_SetPageIfChanged(uint8_t page);    
    void SetAttribute_Txt(uint8_t page, uint8_t element_id, const char* ctr);
    void SetAttribute_BackgroundColour(uint8_t page, uint8_t element_id, uint32_t colour);
    void SetAttribute_FontColour(uint8_t page, uint8_t element_id, uint32_t colour);    
    void WebHandle_Nextion_Command(AsyncWebServerRequest* request);
    void webHandleNextionCmd(AsyncWebServerRequest* request);
    void webHandleNextionRebootPanel(AsyncWebServerRequest* request);
    void Command_SetPage(char* pagename);
    const char* GetObjectName_FromID(uint8_t id, char* objname, uint8_t objname_size);
    void SubParse_DisplaySet_JSON(JsonParserObject obj);
    uint16_t RGB888_to_RGB565(uint8_t r, uint8_t g, uint8_t b);
    uint16_t RGB888_to_RGB565(uint32_t rgb_888);
    void init_serial();    
    void nextionGetAttr(const char* c_str);
    void nextionSendCmd(const char* c_str); 
    void Command_SetBrightness(uint8_t brightness_percentage);
    void Command_SetBrightness255(uint8_t brightness);
    void Command_ToggleBrightness();
    void Template_Load_Init_Display_Command();
    char* ConvertBytetoASCII(byte* data, uint8_t len);

    void webHandleTftFileSize(AsyncWebServerRequest* request);
    void webHandleLcdUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
    uint32_t transmitted_bytes = 0;
    void nextionOtaStartDownload(AsyncWebServerRequest *request, const String &lcdOtaUrl);
    void webHandleLcdDownload(AsyncWebServerRequest* request);
    
        
    #define NEXTION_16BIT_COLOUR_RED_STR   "45056"
    #define NEXTION_16BIT_COLOUR_GREEN_STR "6116"

    #define CONTENT_LENGTH_UNKNOWN ((size_t) -1)
    #define CONTENT_LENGTH_NOT_SET ((size_t) -2)



    static constexpr uint16_t NEXTION_RX_MAX = 128;

    struct NextionFrame {
      uint8_t  data[NEXTION_RX_MAX];
      uint16_t len = 0;
      bool     complete = false;

      inline void reset() {
        len = 0;
        complete = false;
      }
    };
    NextionFrame incoming_frame;

    void ProcessInput();                         // dispatcher
    void ProcessInput__TouchEvent();             // 0x65
    void ProcessInput__CurrentPageNumber();      // 0x66
    void ProcessInput__TouchCoordinateData();    // 0x67
    void ProcessInput__TouchCoordinateData_Wake();// 0x68
    void ProcessInput__GetStringReturn();        // 0x70
    void ProcessInput__GetIntReturn();           // 0x71
    void ProcessInput__ComokResponse();           // 0x63 ... "comok"
    void ProcessInput__ErrorReturnCode();        // bucket for 0x04/0x05/... etc


    bool lcdConnected = false;
    unsigned long lcdVersion = 0;                       // Int to hold current LCD FW version number
    bool lcdVersionQueryFlag = false;                   // Flag to set if we've queried lcdVersion
    const String lcdVersionQuery = "p[0].b[2].val";       // Object ID for lcdVersion in HMI
    char nextionModel[16] = {0};
    byte nextionSuffix[3] = {0xFF, 0xFF, 0xFF};    // Standard suffix for Nextion commands
    uint32_t tftFileSize = 0;                           // Filesize for TFT firmware upload   
    unsigned long nextionAckTimer = 0;
    bool nextionAckEnable = false;
    bool nextionAckReceived = false;
    bool lcdBacklightQueryFlag = false;
    const unsigned long nextionAckTimeout = 1000;

  private:

    int serial_available()
    { 
      return display->available();
    }

    int serial_read()
    { 
      return display->read();
    }

    void serial_print(const char* c_str)
    { 
      display->print(c_str);
    }

    void serial_print(String s_str)
    { 
      display->print(s_str);
    }

    void serial_print_suffix()
    {
      display->write(nextionSuffix, sizeof(nextionSuffix));
      display->flush();
    }

    void serial_print_suffixed(const char* c_str)
    {
      display->print(c_str);
      display->write(nextionSuffix, sizeof(nextionSuffix));
      display->flush();
    }


};

#endif // USE_MODULE_DISPLAYS_NEXTION

#endif // header guard
