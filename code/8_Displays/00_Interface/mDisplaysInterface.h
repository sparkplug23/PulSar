#ifndef _MODULE_DISPLAYS_INTERFACE_H
#define _MODULE_DISPLAYS_INTERFACE_H

#define D_UNIQUE_MODULE_DISPLAYS_INTERFACE_ID   131  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_DISPLAYS_INTERFACE_ID    1    // Numerical accesending order of module within a group

// #define USE_MODULE_EXAMPLE_CLASS

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DISPLAYS_INTERFACE

#include <renderer.h>

#define ENABLE_FEATURE_DISPLAY_LOG_BUFFER

enum EM_DISPLAY_MODE_IDS{
  EM_DISPLAY_MODE_USER_TEXT_SERIALISED_ID,
  EM_DISPLAY_MODE_UTC_TIME_ID,
  
  // Phase out?
  EM_DISPLAY_MODE_LOCAL1_ID,
  EM_DISPLAY_MODE_LOCAL2_ID,
  EM_DISPLAY_MODE_MQTT1_ID,
  EM_DISPLAY_MODE_MQTT2_ID,
  

  /**
   * Logs that are scrolling, values added to buffer will be appended
   * */
  EM_DISPLAY_MODE_LOG_SCROLLING_ID,
  /**
   * Full log is shown across the display, no scrolling, buffer will be shown and can be updated directly (inline replace) since the whole log is updated
   * */
  EM_DISPLAY_MODE_LOG_STATIC_ID,
  /**
   * When advanced commands manually write to the display
   * */
  EM_DISPLAY_MODE_USER_TEXT_ADVANCED_JSON_ID,
  EM_DISPLAY_MODE_LENGTH_ID
};


DEFINE_PGM_CTR(PM_JSON_ADDLOG)   D_JSON_ADDLOG;
DEFINE_PGM_CTR(PM_JSON_CLEARLOG) D_JSON_CLEARLOG;
DEFINE_PGM_CTR(PM_JSON_DISPLAY) D_JSON_DISPLAY;
DEFINE_PGM_CTR(PM_JSON_MODEL)   D_JSON_MODEL;


class mDisplaysInterface :
  public mTaskerInterface
{
  public:
	  mDisplaysInterface(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE_DISPLAYS_INTERFACE_CTR;
    static const char* PM_MODULE_DISPLAYS_INTERFACE_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DISPLAYS_INTERFACE_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DISPLAYS_INTERFACE_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DISPLAYS_INTERFACE_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mDisplaysInterface);
    };
    #endif
    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();

    Renderer *renderer = nullptr;

    uint32_t tSaved_RefreshDisplay = millis();

    enum ColorType { COLOR_BW, COLOR_COLOR };

    #ifndef MAX_TOUCH_BUTTONS
    #define MAX_TOUCH_BUTTONS 16
    #endif

    #ifdef USE_UFILESYS
    extern FS *ufsp;
    extern FS *ffsp;
    #endif

    #ifdef USE_TOUCH_BUTTONS
    VButton *buttons[MAX_TOUCH_BUTTONS];
    #endif

    // drawing color is WHITE
    // on epaper the whole display buffer is transfered inverted this results in white paper
    uint16_t fg_color = 1;
    uint16_t bg_color = 0;
    uint8_t color_type = COLOR_BW;
    uint8_t auto_draw = 1;
    int16_t disp_xpos = 0;
    int16_t disp_ypos = 0;

    #ifdef USE_MULTI_DISPLAY
    struct MULTI_DISP {
      Renderer *display;
      uint16_t fg_color;
      uint16_t bg_color;
      int16_t disp_xpos;
      int16_t disp_ypos;
      uint8_t color_type;
      uint8_t auto_draw;
    } displays[3];
    uint8_t cur_display;
    Renderer *Init_uDisplay(const char *desc, int8_t cs);

    void Set_display(uint8_t index) {
      displays[index].display = renderer;
      displays[index].fg_color = fg_color;
      displays[index].bg_color = bg_color;
      displays[index].color_type = color_type;
      displays[index].auto_draw = auto_draw;
      displays[index].disp_xpos = disp_xpos;
      displays[index].disp_ypos = disp_ypos;
      cur_display = index;
    }

    void Get_display(uint8_t index) {
      renderer = displays[index].display;
      fg_color = displays[index].fg_color;
      bg_color = displays[index].bg_color;
      color_type = displays[index].color_type;
      auto_draw = displays[index].auto_draw;
      disp_xpos = displays[index].disp_xpos;
      disp_ypos = displays[index].disp_ypos;
      if (renderer) renderer->setDrawMode(auto_draw >> 1);
      cur_display = index;
    }
    #endif // USE_MULTI_DISPLAY

    const uint8_t DISPLAY_MAX_DRIVERS = 32;        // Max number of display drivers/models supported by xdsp_interface.ino
    const uint8_t DISPLAY_MAX_COLS = 64;           // Max number of columns allowed with command DisplayCols
    const uint8_t DISPLAY_MAX_ROWS = 64;           // Max number of lines allowed with command DisplayRows
    const uint8_t DISPLAY_LOG_ROWS = 32;           // Number of lines in display log buffer


    enum DisplayInitModes { DISPLAY_INIT_MODE, DISPLAY_INIT_PARTIAL, DISPLAY_INIT_FULL };


    #ifdef USE_GRAPH

    typedef union {
      uint8_t data;
      struct {
          uint8_t overlay : 1;
          uint8_t draw : 1;
          uint8_t nu3 : 1;
          uint8_t nu4 : 1;
          uint8_t nu5 : 1;
          uint8_t nu6 : 1;
          uint8_t nu7 : 1;
          uint8_t nu8 : 1;
      };
    } GFLAGS;

    struct GRAPH {
      uint16_t xp;
      uint16_t yp;
      uint16_t xs;
      uint16_t ys;
      float ymin;
      float ymax;
      float range;
      uint32_t x_time;       // time per x slice in milliseconds
      uint32_t last_ms;
      uint32_t last_ms_redrawn;
      int16_t decimation; // decimation or graph duration in minutes
      uint16_t dcnt;
      uint32_t summ;
      uint16_t xcnt;
      uint8_t *values;
      uint8_t xticks;
      uint8_t yticks;
      uint8_t last_val;
      uint8_t color_index;
      GFLAGS flags;
    };

    struct GRAPH *graph[NUM_GRAPHS];
    #endif // USE_GRAPH

    char *dsp_str;

    uint16_t dsp_x;
    uint16_t dsp_y;
    uint16_t dsp_x2;
    uint16_t dsp_y2;
    uint16_t dsp_rad;
    uint16_t dsp_color;
    int16_t dsp_len;

    uint8_t disp_power = 0;
    uint8_t disp_device = 0;
    uint8_t disp_refresh = 1;
    uint8_t disp_autodraw = 1;
    uint8_t dsp_init;
    uint8_t dsp_font;
    uint8_t dsp_flag;
    uint8_t dsp_on;

    #define PREDEF_INDEXCOLORS 19
    uint16_t index_colors[MAX_INDEXCOLORS - PREDEF_INDEXCOLORS];

    struct LOG_BUFFER{
      char **ptr = nullptr;
      uint8_t ptr_index = 0;
      uint8_t cols = 0;
      uint8_t index = 0;
    }log_buffer;

    struct SCREEN_BUFFER{
      char **ptr = nullptr;
      uint8_t cols = 0;
      uint8_t rows = 0;
    }screen_buffer;

    #define DISPLAY_BUFFER_COLS    128          // Max number of characters in linebuf

    bool disp_subscribed = false;


    void InitDriver(void);

    void ScreenBuffer_Clear(void);
    void ScreenBuffer_Free(void);
    void ScreenBuffer_Alloc(void);
    void ScreenBuffer_ReAlloc(void);

    void ScreenBuffer_SetUnusedRowCharsToSpaceChar(uint32_t line);
    void LogBuffer_Clear(void);
    void LogBuffer_Free(void);
    void LogBuffer_Alloc(void);
    void LogBuffer_ReAlloc(void);
    void LogBuffer_Add(char* txt);
    void LogBuffer_AddRow(char* txt, uint8_t row_index = 0);
    char* LogBuffer_GetRowPointer(char temp_code);
    void LogBuffer_Init(void);

    char* LogBuffer_GetRowPointerByRowIndex(uint8_t row_index);

    void SetPower(void);

    void Init(uint8_t mode);
    void Clear(void);
    void DrawStringAt(uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t flag);


    #ifdef ENABLE_DISPLAY_MODE_USER_TEXT_SERIALISED
    void DisplayText(const char* buffer);
    #endif

    void CmndDisplayText(const char* buffer) ;
        
    void CommandSet_DisplayText_Advanced_JSON(JsonParserObject jobj) ;

    uint8_t fatoiv(char *cp,float *res);
    uint8_t atoiv(char *cp, int16_t *res);
    uint8_t atoiV(char *cp, uint16_t *res);

    uint32_t decode_te(char *line);

    void alignright(char *string);
    char *get_string(char *buff,uint8_t len,char *cp);

    void DisplayOnOff(uint8_t on);


    void CommandSet_DisplayAddLog(const char* c);
    void CommandSet_DisplayClearLog(bool d);


    void SetCursor(uint8_t x, uint8_t y);
    void SetTextSize(uint8_t font_size);
    void SetDisplayMode(uint8_t mode);


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mDisplaysInterface> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mDisplaysInterface> mqtthandler_sensor_ifchanged;
    struct handler<mDisplaysInterface> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers example
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    //with extra handlers example
    
    uint8_t list_ids[3] = {
      MQTT_HANDLER_SETTINGS_ID, 
      MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
      MQTT_HANDLER_SENSOR_TELEPERIOD_ID
    };
    
    struct handler<mDisplaysInterface>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
