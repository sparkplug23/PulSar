#ifndef _MODULE_DISPLAYS_INTERFACE_H
#define _MODULE_DISPLAYS_INTERFACE_H

#define D_UNIQUE_MODULE_DISPLAYS_INTERFACE_ID   131  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_DISPLAYS_INTERFACE_ID    1    // Numerical accesending order of module within a group

// #define USE_MODULE_EXAMPLE_CLASS

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DISPLAYS_INTERFACE

#include <renderer.h>


enum EM_DISPLAY_MODE_IDS{
  EM_DISPLAY_MODE_USER_TEXT_SERIALISED_ID,
  EM_DISPLAY_MODE_UTC_TIME_ID,
  EM_DISPLAY_MODE_LOCAL1_ID,
  EM_DISPLAY_MODE_LOCAL2_ID,
  EM_DISPLAY_MODE_MQTT1_ID,
  EM_DISPLAY_MODE_MQTT2_ID,
  /**
   * Basic version optimised for speed, only redrawn on change, no formatting except new line
   * */
  EM_DISPLAY_MODE_BASIC_BUFFER_TEXT_DRAW,
  /**
   * When advanced commands manually write to the display
   * */
  EM_DISPLAY_MODE_USER_TEXT_ADVANCED_JSON_ID,
  EM_DISPLAY_MODE_LENGTH_ID
};



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
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DISPLAYS_INTERFACE_ID; }

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

    

Renderer *renderer;

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

#define D_PRFX_DISPLAY "Display"
#define D_CMND_DISP_ADDRESS "Address"
#define D_CMND_DISP_COLS "Cols"
#define D_CMND_DISP_DIMMER "Dimmer"
#define D_CMND_DISP_MODE "Mode"
#define D_CMND_DISP_MODEL "Model"
#define D_CMND_DISP_TYPE "Type"
#define D_CMND_DISP_REFRESH "Refresh"
#define D_CMND_DISP_ROWS "Rows"
#define D_CMND_DISP_SIZE "Size"
#define D_CMND_DISP_FONT "Font"
#define D_CMND_DISP_ROTATE "Rotate"
#define D_CMND_DISP_INVERT "Invert"
#define D_CMND_DISP_WIDTH "Width"
#define D_CMND_DISP_HEIGHT "Height"
#define D_CMND_DISP_BLINKRATE "Blinkrate"
#define D_CMND_DISP_BATCH "Batch"
#define D_CMND_DISP_TEXT "Text"

#define D_CMND_DISP_CLEAR "Clear"
#define D_CMND_DISP_NUMBER "Number"
#define D_CMND_DISP_FLOAT "Float"
#define D_CMND_DISP_NUMBERNC "NumberNC"               // NC - "No Clear"
#define D_CMND_DISP_FLOATNC "FloatNC"                 // NC - "No Clear"
#define D_CMND_DISP_RAW "Raw"
#define D_CMND_DISP_LEVEL "Level"
#define D_CMND_DISP_SEVENSEG_TEXT "SevensegText"
#define D_CMND_DISP_SEVENSEG_TEXTNC "SevensegTextNC"  // NC - "No Clear"
#define D_CMND_DISP_SCROLLDELAY "ScrollDelay"
#define D_CMND_DISP_CLOCK "Clock"
#define D_CMND_DISP_TEXTNC "TextNC"                   // NC - "No Clear"
#define D_CMND_DISP_SCROLLTEXT "ScrollText"
#define D_CMND_DISP_REINIT "reinit"

// enum XdspFunctions { FUNC_DISPLAY_INIT_DRIVER, FUNC_DISPLAY_INIT, FUNC_DISPLAY_EVERY_50_MSECOND, FUNC_DISPLAY_EVERY_SECOND,
//                      FUNC_DISPLAY_MODEL, FUNC_DISPLAY_MODE, FUNC_DISPLAY_POWER,
//                      FUNC_DISPLAY_CLEAR, FUNC_DISPLAY_DRAW_FRAME,
//                      FUNC_DISPLAY_DRAW_HLINE, FUNC_DISPLAY_DRAW_VLINE, FUNC_DISPLAY_DRAW_LINE,
//                      FUNC_DISPLAY_DRAW_CIRCLE, FUNC_DISPLAY_FILL_CIRCLE,
//                      FUNC_DISPLAY_DRAW_RECTANGLE, FUNC_DISPLAY_FILL_RECTANGLE,
//                      FUNC_DISPLAY_TEXT_SIZE, FUNC_DISPLAY_FONT_SIZE, FUNC_DISPLAY_ROTATION, FUNC_DISPLAY_DRAW_STRING,
//                      FUNC_DISPLAY_DIM, FUNC_DISPLAY_BLINKRATE,
// #ifdef USE_UFILESYS
//                      FUNC_DISPLAY_BATCH,
// #endif
//                      FUNC_DISPLAY_NUMBER, FUNC_DISPLAY_FLOAT, FUNC_DISPLAY_NUMBERNC, FUNC_DISPLAY_FLOATNC,
//                      FUNC_DISPLAY_RAW, FUNC_DISPLAY_LEVEL, FUNC_DISPLAY_SEVENSEG_TEXT, FUNC_DISPLAY_SEVENSEG_TEXTNC,
//                      FUNC_DISPLAY_SCROLLDELAY, FUNC_DISPLAY_CLOCK, FUNC_DISPLAY_SCROLLTEXT
//                    };

enum DisplayInitModes { DISPLAY_INIT_MODE, DISPLAY_INIT_PARTIAL, DISPLAY_INIT_FULL };

// const char kDisplayCommands[] PROGMEM = D_PRFX_DISPLAY "|"  // Prefix
//   "|" D_CMND_DISP_MODEL "|" D_CMND_DISP_TYPE "|" D_CMND_DISP_WIDTH "|" D_CMND_DISP_HEIGHT "|" D_CMND_DISP_MODE "|"
//   D_CMND_DISP_INVERT "|" D_CMND_DISP_REFRESH "|" D_CMND_DISP_DIMMER "|" D_CMND_DISP_COLS "|" D_CMND_DISP_ROWS "|"
//   D_CMND_DISP_SIZE "|" D_CMND_DISP_FONT "|" D_CMND_DISP_ROTATE "|" D_CMND_DISP_TEXT "|" D_CMND_DISP_ADDRESS "|" D_CMND_DISP_BLINKRATE "|"
// #ifdef USE_UFILESYS
//   D_CMND_DISP_BATCH "|"
// #endif
//   D_CMND_DISP_CLEAR "|" D_CMND_DISP_NUMBER "|" D_CMND_DISP_FLOAT "|" D_CMND_DISP_NUMBERNC "|" D_CMND_DISP_FLOATNC "|"
//   D_CMND_DISP_RAW "|" D_CMND_DISP_LEVEL "|" D_CMND_DISP_SEVENSEG_TEXT "|" D_CMND_DISP_SEVENSEG_TEXTNC "|"
//   D_CMND_DISP_SCROLLDELAY "|" D_CMND_DISP_CLOCK "|" D_CMND_DISP_TEXTNC "|" D_CMND_DISP_SCROLLTEXT "|" D_CMND_DISP_REINIT
//   ;

// void (* const DisplayCommand[])(void) PROGMEM = {
//   &CmndDisplay, &CmndDisplayModel, &CmndDisplayType, &CmndDisplayWidth, &CmndDisplayHeight, &CmndDisplayMode,
//   &CmndDisplayInvert, &CmndDisplayRefresh, &CmndDisplayDimmer, &CmndDisplayColumns, &CmndDisplayRows,
//   &CmndDisplaySize, &CmndDisplayFont, &CmndDisplayRotate, &CmndDisplayText, &CmndDisplayAddress, &CmndDisplayBlinkrate,
// #ifdef USE_UFILESYS
//   &CmndDisplayBatch,
// #endif
//   &CmndDisplayClear, &CmndDisplayNumber, &CmndDisplayFloat, &CmndDisplayNumberNC, &CmndDisplayFloatNC,
//   &CmndDisplayRaw, &CmndDisplayLevel, &CmndDisplaySevensegText, &CmndDisplaySevensegTextNC,
//   &CmndDisplayScrollDelay, &CmndDisplayClock, &CmndDisplayTextNC, &CmndDisplayScrollText,&DisplayReInitDriver
// };

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

#ifdef USE_DISPLAY_MODES1TO5

char **disp_log_buffer = nullptr;
char **disp_screen_buffer = nullptr;
char disp_temp[2];    // C or F
char disp_pres[5];   // hPa or mmHg

uint8_t disp_log_buffer_cols = 0;
uint8_t disp_log_buffer_idx = 0;
uint8_t disp_log_buffer_ptr = 0;
uint8_t disp_screen_buffer_cols = 0;
uint8_t disp_screen_buffer_rows = 0;
bool disp_subscribed = false;

#endif  // USE_DISPLAY_MODES1TO5

void DisplayInitDriver(void);

void DisplayClearScreenBuffer(void);
void DisplayFreeScreenBuffer(void);
void DisplayAllocScreenBuffer(void);
void DisplayReAllocScreenBuffer(void);
void DisplayFillScreen(uint32_t line);
void DisplayClearLogBuffer(void);
void DisplayFreeLogBuffer(void);
void DisplayAllocLogBuffer(void);
void DisplayReAllocLogBuffer(void);
void DisplayLogBufferAdd(char* txt);
char* DisplayLogBuffer(char temp_code);
void DisplayLogBufferInit(void);
void DisplaySetPower(void);

void DisplayInit(uint8_t mode);
void DisplayClear(void);
void DisplayDrawStringAt(uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t flag);


void DisplayText(const char* buffer);

void CmndDisplayText(const char* buffer) ;
    
// void DisplayText_Advanced_JSON(JsonParserObject jobj);

void CommandSet_DisplayText_Advanced_JSON(JsonParserObject jobj) ;

    
    
uint8_t atoiv(char *cp, int16_t *res);
uint8_t atoiV(char *cp, uint16_t *res);

uint32_t decode_te(char *line);

void alignright(char *string);
char *get_string(char *buff,uint8_t len,char *cp);

void DisplayOnOff(uint8_t on);


void SetCursor(uint8_t x, uint8_t y);
void SetTextSize(uint8_t font_size);
void SetDisplayMode(uint8_t mode);


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
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
    
    struct handler<mDisplaysInterface>* list_ptr[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
