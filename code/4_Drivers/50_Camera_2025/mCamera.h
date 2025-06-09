/*
  mPZEM004T.h - PZEM004T v3 MODBUS

  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef HEADER_MODULE_DRIVERS__CAMERA_2025_H
#define HEADER_MODULE_DRIVERS__CAMERA_2025_H
  
#define D_UNIQUE_MODULE_DRIVERS__CAMERA_ID 4064 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS__CAMERA_2025

#include <Arduino.h>
#include <vector>

#include "1_TaskerManager/mTaskerInterface.h"

#include "cam_hal.h"

#include "esp_camera.h"
#include "sensor.h"
#include "fb_gfx.h"
#include "camera_pins.h"
#include "esp_jpg_decode.h"

#include <ESP8266WebServer.h>

#define WEBCAM_CORE 0

enum LedEffect {
  LED_OFF,
  LED_ON,
  LED_BLINK,
  LED_PULSE
};

typedef union {
  uint8_t data;
  struct {
    uint8_t network_down : 1;  // No wifi/ethernet connect
    uint8_t mqtt_down : 1;     // No mqtt connect
    uint8_t wifi_ap_active : 1;       // In AP mode, waiting config
    uint8_t show_power : 1;
    uint8_t spare04 : 1;
    uint8_t spare05 : 1;
    uint8_t slave : 1;     // Controlled by another task
    uint8_t manual : 1;    // Direct control
  };
} LedMode_BitField;

struct LedState {
  LedEffect effect;
  uint8_t state;            // Current state (HIGH or LOW)
  uint8_t count;            // Number of blinks/pulses
  uint16_t period;          // Period for both blinking and pulsing in milliseconds
  uint16_t groupPause;      // Pause time between blink/pulse groups in milliseconds
  uint8_t duration_secs;    // Duration in seconds for how long the LED should blink/pulse
  uint32_t lastUpdateTime;  // Time tracking for blinking/pulsing
  uint32_t startTime;       // To track when the blinking/pulsing started
  LedMode_BitField mode;
};


class mCamera :
  public mTaskerInterface
{

  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mCamera(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage();
    
    static constexpr const char* PM_MODULE_DRIVERS__CAMERA_CTR = D_MODULE_DRIVERS__CAMERA_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS__CAMERA_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS__CAMERA_ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/
    
// use mutex like:
// mSupport::TasAutoMutex localmutex(&WebcamMutex, "somename", 200);
// in any function.  Will wait for mutex to be clear, and auto-release when the function exits.

#define BOUNDARY "e8b8c539-047d-4777-a985-fbba6edff11e"

#ifndef MAX_PICSTORE
#define MAX_PICSTORE 4
#endif



typedef struct tag_wc_client {
  WiFiClient client;
  tag_wc_client *p_next;
  uint8_t active;
  uint8_t type;
} wc_client;

// our basic picture store
// we use this both in wc_picstore and elsewhere
struct PICSTORE {
  uint8_t * buff;              /*!< Pointer to the pixel data */
  size_t len;                 /*!< Length of the buffer in bytes */
  uint16_t width;               /*!< Width of the buffer in pixels */
  uint16_t height;              /*!< Height of the buffer in pixels */
  int8_t format;         /*!< Format of the pixel data */
  struct timeval timestamp;   /*!< Timestamp since boot of the first DMA buffer of the frame */  
  int allocatedLen; // -1 or 0 indicates failure to allocate
};


struct {
  uint32_t loopcounter;
  uint32_t loopspersec;

  // status variables
  uint8_t  up = 0;
  uint8_t  psram = 0; // indicates if we found psram or not. Suspect always true for esp32cam modules
  volatile uint16_t width; // last JPEG frame height received or 0
  volatile uint16_t height; // last JPEG frame height received or 0
  volatile uint32_t last_frame_len; // last JPEG frame length or 0
  volatile int8_t  frame_processed; // set when a frame is finished processing
  unsigned int lastCamError; // set to the last cam error, or 0
  volatile uint32_t camtimediff; // last camtime interval
  volatile uint32_t frameIntervalsus; // expected cam interval in us
  volatile uint16_t lenDiff;
  volatile uint16_t lenDiffTriggered;
  volatile uint8_t  lenDiffTrigger;

  // control variables
  volatile int8_t  disable_cam; // set to 1 when TAS is busy doning something and has turned the camera off
  volatile int8_t  taskRunning; // 0 = not started, 1 = running, 2 = request stop
  TaskHandle_t taskHandle; // the task handle
  volatile int8_t  taskGetFrame; // set to n to trigger capture of n frames to picstore
  volatile int8_t  taskTakePic; // set to n to trigger capture of n frames to picstore
  uint8_t  berryFrames;
  volatile uint16_t lenDiffLimit;
  volatile int32_t skipFrames;
  /* camPixelFormat is format +1, i.e.
  0->4->PIXFORMAT_JPEG
  1->0->PIXFORMAT_RGB565,    // 2BPP/RGB565 - valid?
  2->1->PIXFORMAT_YUV422,    // 2BPP/YUV422 - valid?
  3->2->PIXFORMAT_YUV420,    // 1.5BPP/YUV420 - invalid?
  4->3->PIXFORMAT_GRAYSCALE, // 1BPP/GRAYSCALE - valid?
  5->4->PIXFORMAT_JPEG,      // JPEG/COMPRESSED - valid
  6->5->PIXFORMAT_RGB888,    // 3BPP/RGB888 - invalid?
  7->6->PIXFORMAT_RAW,       // RAW - invalid?
  8->7->PIXFORMAT_RGB444,    // 3BP2P/RGB444 - invalid?
  9->8->>PIXFORMAT_RGB555,    // 3BP2P/RGB555 - invalid?
  */
  volatile uint8_t  camPixelFormat; // 

  // our (separate) webserver on port 81
  ESP8266WebServer *CamServer;
  // pointer to the first http streaming client in a list of multiple clients, or nullptr
  wc_client *client_p;
  struct PICSTORE picstore[MAX_PICSTORE];
  uint8_t lastBnum; // the last picture index stored
  struct PICSTORE snapshotStore;

  #ifdef ENABLE_RTSPSERVER
  // TCP server on port 8554
  WiFiServer *rtspp;
  // pointer to the first rtsp client in a list of multiple clients, or nullptr
  wc_rtspclient * volatile rtspclient;
  uint8_t rtsp_start;
  #endif // ENABLE_RTSPSERVER
} Wc;

struct {
  uint32_t camcnt = 0;
  uint32_t camfps = 0;
  uint32_t maxfps = 0;
  uint32_t camfail = 0;
  uint32_t jpegfail = 0;
  uint32_t clientfail = 0;
  float avgFPS = 0.0;
  uint16_t avgProcessingPerFrameMS;
  uint16_t avgFrameMS;
  uint32_t framesTotal;
  uint32_t framesLost;

  uint8_t webclientcount;
  uint8_t rtspclientcount;

  char name[7] = "Webcam";
} WcStats;

/*********************************************************************************************/
// functions to encode into a jpeg buffer.
// we keep ONE allocated buffer for this, and expand it if the jpeg does not fit.
// single expanding jpeg buffer for encode into
// call encode_reset() to maybe recover from failure to allocate.

#define DEFAULT_INITIAL_JPEG_LEN 16384

// static store for taking pictures
struct PICSTORE OurOneJpeg = {0};

// static store for video.
// we only re-allocate if the jpeg is larger, to stop malloc churn.
struct PICSTORE VideoJpeg = {0};



// allocate a PICSTORE buffer.
// for PIXFORMAT_JPEG:
//   set bpp_orsize to the size of the jpeg file
//   width and height are unused
// for pixel based formats
//   size is based on width*height*(bytes per pixel)+4
// allocstyle: 
//   0->always free & alloc
//   1->realloc only if not big enough
//   2->realloc only if not big enough, and copy data
#define WC_ALLOC_ALWAYS 0
#define WC_REALLOC_IF_SMALLER 1
#define WC_REALLOC_WITH_COPY 2

#define WC_USE_FAST_MEM 0x10






ESP8266WebServer *Webserver;



    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    bool pic_alloc(struct mCamera::PICSTORE *ps, int width, int height, int jpegsize, int format, int allocstyle);
    bool pic_free(struct mCamera::PICSTORE *ps);
    bool pic_alloc_p(struct mCamera::PICSTORE **pps, int width, int height, int jpegsize, int format, int allocstyle);
    bool pic_free_p(struct mCamera::PICSTORE **pps);
    bool wc_check_format(int format);

    void WCStartOperationTask();
    static void WCOperationTask(void *pvParameters);

    void WcUpdateStats(void);
        
    bool WcWaitZero(volatile int8_t *val, int8_t initial, int timeout_ms);
    void WcWaitEnable();
    bool WcPinUsed(void);
    void WcFeature(int32_t value);
    void WcApplySettings();
    void WcSetDefaults(uint32_t upgrade);
    uint32_t WcSetup(int32_t fsiz);
    void WcRemoveDeadCients();
    void WcEndStream();
    void WcCamOff();
    int32_t WcSetOptions(uint32_t sel, int32_t value);
    void WcWaitFrame(int maxtime_ms);
    uint32_t WcGetWidth(void);
    uint32_t WcGetHeight(void);
    
    uint32_t WcGetPicstore(int32_t num, uint8_t **buff);
    uint32_t WcGetPicstorePtr(int32_t num, struct mCamera::PICSTORE **p);
    uint32_t WcGetFrame(int32_t bnum);

    bool WebcamAuthenticate(void);
    bool WebcamCheckPriviledgedAccess(bool autorequestauth = true);
    void HandleImage(void);    
    void HandleImageAny(struct mCamera::PICSTORE *ps);
    void HandleWebcamMjpegFn(int type);
    static void HandleWebcamMjpeg(void);
    static void HandleWebcamMjpegDiff(void);
    static void HandleWebcamRoot(void);
    uint32_t WcSetStreamserver(uint32_t flag);

    void WcInterrupt(uint32_t state);

    void WcLoop(void);
    void WcPicSetup(void);
    void WcShowStream(void);
    void WcInit(void);


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_method = 0, bool json_appending = true); //default is true
    
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    std::vector<struct handler<mCamera>*> mqtthandler_list;    
    struct handler<mCamera> mqtthandler_settings;
    struct handler<mCamera> mqtthandler_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT 


    
};

#endif // USE_MODULE_DRIVERS__CAMERA_2025

#endif