#ifndef _DRIVERS__CAMERA_TASMOTA_H
#define _DRIVERS__CAMERA_TASMOTA_H


/*********************************************************************************************\
 * ESP32 webcam based on example in Arduino-ESP32 library
 *
 * Template as used on ESP32-CAM WiFi + bluetooth Camera Module Development Board ESP32 With Camera Module OV2640 Geekcreit for Arduino
 * {"NAME":"AITHINKER CAM","GPIO":[4992,1,672,1,416,5088,1,1,1,6720,736,704,1,1,5089,5090,0,5091,5184,5152,0,5120,5024,5056,0,0,0,0,4928,1,5094,5095,5092,0,0,5093],"FLAG":0,"BASE":2}
 *
 * Supported commands:
 * WcInterrupt     = Control streaming, 0 = stop, 1 = start
 * WcResolution = Set resolution
 0 = FRAMESIZE_96X96,    // 96x96
 1 = FRAMESIZE_QQVGA,    // 160x120
 2 = FRAMESIZE_QCIF,     // 176x144
 3 = FRAMESIZE_HQVGA,    // 240x176
 4 = FRAMESIZE_240X240,  // 240x240
 5 = FRAMESIZE_QVGA,     // 320x240
 6 = FRAMESIZE_CIF,      // 400x296
 7 = FRAMESIZE_HVGA,     // 480x320
 8 = FRAMESIZE_VGA,      // 640x480
 9 = FRAMESIZE_SVGA,     // 800x600
 10 = FRAMESIZE_XGA,      // 1024x768
 11 = FRAMESIZE_HD,       // 1280x720
 12 = FRAMESIZE_SXGA,     // 1280x1024
 13 = FRAMESIZE_UXGA,     // 1600x1200
 // 3MP Sensors above this no yet supported with this driver
 14 = FRAMESIZE_FHD,      // 1920x1080
 15 = FRAMESIZE_P_HD,     //  720x1280
 16 = FRAMESIZE_P_3MP,    //  864x1536
 17 = FRAMESIZE_QXGA,     // 2048x1536
 // 5MP Sensors
 18 = FRAMESIZE_QHD,      // 2560x1440
 19 = FRAMESIZE_WQXGA,    // 2560x1600
 20 = FRAMESIZE_P_FHD,    // 1080x1920
 21 = FRAMESIZE_QSXGA,    // 2560x1920
 22 = FRAMESIZE_INVALID

 * WcMirror     = Mirror picture, 0 = no, 1 = yes
 * WcFlip       = Flip picture, 0 = no, 1 = yes
 * WcSaturation = Set picture Saturation -2 ... +2
 * WcBrightness = Set picture Brightness -2 ... +2
 * WcContrast   = Set picture Contrast -2 ... +2
 * WcSpecialEffekt = Set Special Picture Effect: 0 = off, 1 = , 2 = , 3 = , 4 = , 5 = , 6 =
 * WcAWB          = Auto White Balance, 0 = no, 1 = yes
 * WcWBMode       = White Balance Mode, 0 = auto, 1 =
 * WcAWBGain      = Auto White Balance Gain, 0 = no, 1 = yes
 * WcAEC          = Auto exposure control (Sensor), 0 = no, 1 = yes
 * WcAECDSP       = Auto exposure control (DSP), 0 = no, 1 = yes
 * WcAECValue     = Auto exposure control value, 0 ... 1024
 * WcAECLevel     = Auto exposure control level, -2 ... +2
 * WcAGC          = Auto gain control, 0 = no, 1 = yes
 * WcAGCGain      = Auto gain control gain, 0 .. 30
 * WcGainCeiling  = Gain ceiling, 0 .. 6 (0 = x2, 1 = x4, 2 = x8, 3 = x16, 4 = x32, 5 = x64, 6 = x128)
 * WcGammaCorrect = Auto Gamma Correct, 0 = no, 1 = yes
 * WcLensCorrect  = Auto Lens Correct, 0 = no, 1 = yes
 * WcWPC          = White Pixel Correct, 0 = no, 1 = yes
 * WcDCW          = Downscale, 0 = no, 1 = yes
 * WcBPC          = Black Pixel Correct, 0 = no, 1 = yes
 * WcColorbar     = Show Colorbar, 0 = no, 1 = yes
 * WcFeature      = Set extended Feature, 0 = off, 1 = reduce FPS, 2 = Nightmode
 * WcStats        = Show Statistics
 * WcInit         = Init Camera Interface
 * WcRtsp         = Control RTSP Server, 0=disable, 1=enable (forces restart) (if defined ENABLE_RTSPSERVER)
 *
 * WcFeature Explanation
 * See https://hobbylad.files.wordpress.com/2020/02/ov2640-camera-module-software-application-notes.pdf
 * for more Information
 * 0: Normal Operation
 * 1: Reduced FPS: Set XCLK Divisor to 2. Reduces Framerate and also increases exposure time. This causes
 *    better low light performance. See above document, page 6
 * 2: Night Mode: Further increase exposure time and lower the Framerate depending on available light.
 *    See above Document, Page 8
 *
 * Only boards with PSRAM should be used.
 * To speed up cam processing cpu frequency should be better set to 240Mhz
 *
 * remarks for AI-THINKER
 * GPIO0 zero must be disconnected from any wire after programming because this pin drives the cam clock and does
 * not tolerate any capictive load
 * flash led = gpio 4
 * red led = gpio 33
 * optional rtsp url: rtsp://xxx.xxx.xxx.xxx:8554/mjpeg/1
 */


#define D_UNIQUE_MODULE_DRIVERS__CAMERA_TASMOTA__ID ((4*1000)+61)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS__CAMERA_TASMOTA

#include "1_TaskerManager/mTaskerInterface.h"

// #include "cam_hal.h" // https://github.com/espressif/esp32-camera
#include "esp_camera.h"
#include "sensor.h"
#include "fb_gfx.h"
#include "camera_pins.h"

#ifdef ENABLE_RTSPSERVER
#include <OV2640.h>
#include <SimStreamer.h>
#include <OV2640Streamer.h>
#include <CRtspSession.h>
#ifndef RTSP_FRAME_TIME
#define RTSP_FRAME_TIME 100
#endif // RTSP_FRAME_TIME
#endif // ENABLE_RTSPSERVER

#ifdef ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER
#include <ESP8266WebServer.h>
#endif

class mWebCamera :
  public mTaskerInterface
{
  public:
    mWebCamera(){};

    static constexpr const char* PM_MODULE_DRIVERS__CAMERA_TASMOTA__CTR = D_MODULE_DRIVERS__CAMERA_TASMOTA__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS__CAMERA_TASMOTA__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS__CAMERA_TASMOTA__ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){    return sizeof(mWebCamera); };
    #endif

    struct SETTINGS    { 
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();
    void EverySecond();
    void Pre_Init();
    void Init();
    void parse_JSONCommand(JsonParserObject obj);

    #define MAX_WEBCAM_DATA  8

    bool HttpCheckPriviledgedAccess(bool);
    #ifdef ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER
    extern ESP8266WebServer *Webserver;
    #endif

    #define BOUNDARY "e8b8c539-047d-4777-a985-fbba6edff11e"

    #ifndef MAX_PICSTORE
    #define MAX_PICSTORE 4
    #endif
    struct PICSTORE {
      uint8_t *buff;
      uint32_t len;
    };

    
    struct {
      uint8_t  up;
      uint16_t width;
      uint16_t height;
      uint8_t  stream_active;
      WiFiClient client;
      #ifdef ENABLE_DEVFEATURE_CAMERA_TASMOTA_INCLUDE_WEBSERVER
      ESP8266WebServer *CamServer;
      #endif
      struct PICSTORE picstore[MAX_PICSTORE];
    #ifdef ENABLE_RTSPSERVER
      WiFiServer *rtspp;
      CStreamer *rtsp_streamer;
      CRtspSession *rtsp_session;
      WiFiClient rtsp_client;
      uint8_t rtsp_start;
      OV2640 cam;
      uint32_t rtsp_lastframe_time;
    #endif // ENABLE_RTSPSERVER
    } Wc;

    struct {
      uint32_t camcnt = 0;
      uint32_t camfps = 0;
      uint32_t camfail = 0;
      uint32_t jpegfail = 0;
      uint32_t clientfail = 0;
      char name[7] = "Webcam";
    } WcStats;




/*********************************************************************************************/

void WcInterrupt(uint32_t state);
bool WcPinUsed(void);
void WcFeature(int32_t value);

void WcApplySettings();
void WcSetDefaults(uint32_t upgrade);
uint32_t WcSetup(int32_t fsiz);
/*********************************************************************************************/

int32_t WcSetOptions(uint32_t sel, int32_t value);

uint32_t WcGetWidth(void);
uint32_t WcGetHeight(void);
/*********************************************************************************************/

struct WC_Motion {
uint16_t motion_detect;
uint32_t motion_ltime;
uint32_t motion_trigger;
uint32_t motion_brightness;
uint8_t *last_motion_buffer;
} wc_motion;


uint32_t WcSetMotionDetect(int32_t value);
// optional motion detector
void WcDetectMotion(void) ;

#ifdef COPYFRAME
struct PICSTORE tmp_picstore;
#endif

uint32_t WcGetPicstore(int32_t num, uint8_t **buff);
uint32_t WcGetFrame(int32_t bnum);

//////////////// Handle authentication /////////////////


bool WebcamAuthenticate(void);

bool WebcamCheckPriviledgedAccess(bool autorequestauth = true);

///////////////////////////////////////////////////

void HandleImage(void);
void HandleImageBasic(void);

void HandleWebcamMjpeg(void);
void HandleWebcamMjpegTask(void);

void HandleWebcamRoot(void);

/*********************************************************************************************/

uint32_t WcSetStreamserver(uint32_t flag);
void WcInterruptControl();
/*********************************************************************************************/

void WcPicSetup(void);
void WcShowStream(void);

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

#define D_PRFX_WEBCAM "WC"
#define D_CMND_WC_STREAM "Stream"
#define D_CMND_WC_RESOLUTION "Resolution"
#define D_CMND_WC_MIRROR "Mirror"
#define D_CMND_WC_FLIP "Flip"
#define D_CMND_WC_SATURATION "Saturation"
#define D_CMND_WC_BRIGHTNESS "Brightness"
#define D_CMND_WC_CONTRAST "Contrast"
#define D_CMND_WC_SPECIALEFFECT "SpecialEffect"

#define D_CMND_WC_AWB "AWB"
#define D_CMND_WC_WB_MODE "WBMode"
#define D_CMND_WC_AWB_GAIN "AWBGain"

#define D_CMND_WC_AEC "AEC"
#define D_CMND_WC_AEC_VALUE "AECValue"
#define D_CMND_WC_AE_LEVEL "AELevel"
#define D_CMND_WC_AEC2 "AECDSP"

#define D_CMND_WC_AGC "AGC"
#define D_CMND_WC_AGC_GAIN "AGCGain"
#define D_CMND_WC_GAINCEILING "GainCeiling"

#define D_CMND_WC_RAW_GMA "GammaCorrect"
#define D_CMND_WC_LENC "LensCorrect"

#define D_CMND_WC_WPC "WPC"
#define D_CMND_WC_DCW "DCW"
#define D_CMND_WC_BPC "BPC"

#define D_CMND_WC_COLORBAR "Colorbar"

#define D_CMND_WC_FEATURE "Feature"
#define D_CMND_WC_SETDEFAULTS "SetDefaults"
#define D_CMND_WC_STATS "Stats"

#define D_CMND_WC_INIT "Init"
#define D_CMND_RTSP "Rtsp"

#define D_CMND_WC_AUTH "Auth"
#define D_CMND_WC_CLK "Clock"

void CmndWebcam(void);
void CmndWebcamStream(void);
void CmndWebcamResolution(void);
void CmndWebcamMirror(void);
void CmndWebcamFlip(void);
void CmndWebcamSaturation(void);
void CmndWebcamBrightness(void);
void CmndWebcamContrast(void);
void CmndWebcamSpecialEffect(void);
void CmndWebcamAWB(void);
void CmndWebcamWBMode(void);
void CmndWebcamAWBGain(void);
void CmndWebcamAEC(void);
void CmndWebcamAECValue(void);
void CmndWebcamAELevel(void);
void CmndWebcamAEC2(void);
void CmndWebcamAGC(void);
void CmndWebcamAGCGain(void);
void CmndWebcamGainCeiling(void);
void CmndWebcamGammaCorrect(void);
void CmndWebcamLensCorrect(void);
void CmndWebcamWPC(void);
void CmndWebcamDCW(void);
void CmndWebcamBPC(void);
void CmndWebcamColorbar(void);
void CmndWebcamFeature(void);
void CmndWebcamAuth(void);
void CmndWebcamClock(void);
void CmndWebcamInit(void);
void CmndWebcamSetDefaults(void);

#define D_WEBCAM_STATS_FPS "FPS"
#define D_WEBCAM_STATS_CAMFAIL "CamFail"
#define D_WEBCAM_STATS_JPEGFAIL "JpegFail"
#define D_WEBCAM_STATS_CLIENTFAIL "ClientFail"

void CmndWebcamStats(void);

#ifdef ENABLE_RTSPSERVER
void CmndWebRtsp(void);
#endif // ENABLE_RTSPSERVER

void WcUpdateStats(void);
void WcStatsShow(void);



    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    void MQTTHandler_Sender();
    
    struct handler<mWebCamera> mqtthandler_settings;
    struct handler<mWebCamera> mqtthandler_sensor_ifchanged;
    struct handler<mWebCamera> mqtthandler_sensor_teleperiod;
 
    struct handler<mWebCamera>* mqtthandler_list[3] = {
      &mqtthandler_settings,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    // No specialised payload therefore use system default instead of enum
    
    
    #endif // USE_MODULE_NETWORK_MQTT
    
};

#endif // USE_MODULE_DRIVERS__CAMERA_TASMOTA

#endif // _DRIVERS__CAMERA_TASMOTA_H
