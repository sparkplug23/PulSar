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

#ifdef ENABLE_RTSPSERVER
  #include <OV2640.h>
  #include <SimStreamer.h>
  #include <OV2640Streamer.h>
  #include <CRtspSession.h>
  #ifndef RTSP_FRAME_TIME
    #define RTSP_FRAME_TIME 100
  #endif // RTSP_FRAME_TIME
#endif

// #include "cam_hal.h"
// #include "esp_camera.h"
// #include "sensor.h"
// #include "fb_gfx.h"
// #include "camera_pins.h"
// #include "esp_jpg_decode.h"
//#include "img_converters.h"


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


    #ifdef ENABLE_RTSPSERVER
      class localOV2640Streamer : public CStreamer {
        BufPtr f_ptr; // Frame pointer
        int f_len;
    
      public:
        localOV2640Streamer(SOCKET aClient, int width, int height);
        void setframe(BufPtr ptr, int len);
        void clearframe();
        virtual void streamImage(uint32_t curMsec);
      };
    
      localOV2640Streamer* rtsp_streamer = nullptr;
    
      void RTSP_SetFrame(camera_fb_t* fb);
      void RTSP_ClearFrame();
      void RTSP_StreamTick(uint32_t now);
    #endif
    
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

#ifdef ENABLE_RTSPSERVER
void WcEndRTSP();

typedef struct tag_wc_rtspclient {
  localOV2640Streamer * volatile camStreamer;
  CRtspSession * volatile rtsp_session;
  WiFiClient rtsp_client;
  tag_wc_rtspclient * volatile p_next;
} wc_rtspclient;
#endif //ENABLE_RTSPSERVER


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

camera_config_t config;

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
  wc_rtspclient * volatile rtsp_client;
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

  struct ActiveClients {
    uint8_t web;
    uint8_t rtsp;
  } activeClients;

  char name[7] = "Webcam";
} stats;

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


bool LoadDefaultConfig();

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
    // static void WCOperationTask(void *pvParameters);
    void WCOperationTask();
    static void WCOperationTaskS(void* pvParameters);

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

    
#ifdef ENABLE_CAMERA__MOTION_DETECTION


/*********************************************************************************************\
 * ESP32 webcam motion routines
 *
 * WcGetmotionpixelsN = (N=1..4) read addr, len, w, h as JSON {"addr":123456,"len":12345,"w":160,"h":120, "format":4} 
 *    motion(1)
 *    difference(2) buffer - e.g for berry
 *    mask(3)
 *    background(4)
 *     e.g. could be used to read pixels, or change pixels from berry.

 * WcConvertFrameN <format> <scale> - convert a wcgetframe in picstore from jpeg to <format> (0=2BPP/RGB565, 3=1BPP/GRAYSCALE, 5=3BPP/RGB888), <scale> (0-3)
 *     converts in place, replacing the stored frame with the new format.  Data can be retrieved using wcgetpicstoreN (e.g. for use in berry)
 *     will fail if it can't convert or allocate.
 * 
 * WcSetPicture - SetPictureN (N=1-MAX_PICTORE) expects 'addr len format [width height]
 *   use to populate a frame in Wc.picstore from Berry.  e.g. to put a JPEG mask there
 *   so you can then decode it, get it's address, get the address of the current mask, and 
 *   copy data across.
 *   if sending JPEG (format=0|5), width and height are calculated on decode.
 *   if sending pixels (format=4(GRAY)|6(RGB)|1(RGB565)), width and height are required, and used to allocate.
 *   binary data is copied from addr.  i.e. you can send the addr/len from Berry introspect bytes.
 *    ideas: could be used to set background image based on time of day.

### Enable motion detection interval
WCsetMotiondetect <timems>
WCsetMotiondetect 2000

### disable motion detection
WCsetMotiondetect 0

### return overall normalised pixel difference.
WCsetMotiondetect -1

### return overall normalised brightness.
WCsetMotiondetect -2

### motion detect via jpeg frame size (% change)
Does not run motion detect frame scan!
(i.e. you can turn off WCsetMotiondetect 0 - and detect at 50fps)
WCsetMotiondetect2 <fsizediff percent limit>
WCsetMotiondetect2 20

### Pixel diff value threshold
if set > 0, pixels with a difference > threshold will be counted
if a difference buffer is enabled, then pixels in it are raised to 255.
WCsetMotiondetect3 <pixel diff threshold 1-255>
WCsetMotiondetect3 10

### Pixels over threshold trigger
if the number of pixels changed (per 10000) is > limit, motion is triggered.
set 0 to disable.
WCsetMotiondetect4 <pixel count trigger thresh, in 1/10000>
WCsetMotiondetect4 10

### set scale for motion detect image ref camera res
the number 0-7 = 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128
values 0-3 use scaling on jpeg decode (fast).
values 4-7 add software scaling (not much performance gain, but some) 
WCsetMotiondetect5 0-7

### enable difference buffer
enable/disable the difference buffer
WCsetMotiondetect6 0-1

### Set motion trigger threshold (accumulated pixels difference)
normalised value, try 1000...
WCsetMotiondetect7 0-nnnnn


Endpoints:
http://IP/motionlbuff.jpg - last motion buffer as jpeg (fully scaled) - if motion enabled, else 404
http://IP/motionbuff.jpg - motion buffer as jpeg (only jpeg scaled) - if motion enabled, else 404
http://IP/motiondiff.jpg - last difference buffer as jpeg (fully scaled) - if enabled, else 404

http://IP:81/diff.mjpeg - motion or difference buffer stream as mjpeg (fully scaled, buf is motion if diff not enabled) - if enabled, else no data

 */

/*********************************************************************************************/

/*********************************************************************************************/
/*
Berry usage:
if you turn on motion detect with "wcsetmotiondetect 1000" (number is ms between detect)
or it you turn on frame sending using "wcberryframes 1"
then berry will be called expecting to find a driver which has a method "webcam", e.g.:

var piccount = 0
class MyWebcamDriver
  #- create a method for adding a button to the main menu -#
  def webcam(cmd, idx, payload)
    print("webcam "+cmd+" payload:"+payload)
    if (cmd == "motion")
      #split payload at space
      var val = int(payload)
      if (val > 1000)
        piccount = piccount+1
        var tcmd = "wcsavepic0 /myfile" .. piccount .. ".jpg"
        tasmota.cmd(tcmd)
        print("webcam motion -> "+tcmd)
      end
    end
  end
end

This  will be called with 
"motion <framediff> <<framebrightness>"
and/or
"frame"

*/
/*********************************************************************************************/


// extern SemaphoreHandle_t WebcamMutex;


#define DEFAULT_INITIAL_JPEG_LEN 16384



// size_t WcJpegEncoderStore_jpg_out_cb(void * arg, size_t index, const void* data, size_t len);
bool WcencodeToJpeg(uint8_t *src, size_t srclen, int width, int height, int format, uint8_t quality, struct PICSTORE *dest);

void Wcencode_reset(struct PICSTORE *dest);

struct WC_Motion {
  /////////////////////////////////////
  // configured by user
  uint16_t motion_detect; // time between detections
  uint32_t motion_trigger_limit; // last amount of difference measured (~100 for none, > ~1000 for motion?)
  uint8_t scale; /*0=native, 1=/2, 2=/4, 3=/8*/
  uint8_t swscale; // skips pixels 0=native, 1=/2, 2=/4, 3=/8 - after scale
  uint8_t enable_diffbuff; // enable create of a buffer containing the last difference image
  uint8_t enable_backgroundbuff;
  uint8_t capture_background;

  uint8_t pixelThreshold;
  uint32_t  pixel_trigger_limit; // pertenthousand changed pixels

  uint8_t enable_mask; // enable mask buffer
  uint32_t auto_mask; // number of motion runs to run automask over
  uint8_t auto_mask_pixel_threshold; // pixel change threshold to add pixel to mask
  uint8_t auto_mask_pixel_expansion; // number of pixels atound the detected pixel to set in mask (square)

  ////////////////////////////////////
  // variables used in detection

  // set to 0 each any time we restart (new last_motion_buffer), goes to after first processing 
  // set to -1 on malloc failure - will happen with larger frames
  int8_t motion_state;
  uint32_t motion_ltime;  // time of last detect
  uint32_t motion_trigger; // last amount of difference measured (~100 for none, > ~1000 for motion?)
  uint32_t motion_brightness; // last frame brightness read (~15000)

  // jpeg is decoded (with scale) into here.
  struct PICSTORE *frame;
  // the others are both scale and swscale
  // the last image - to compare against.
  struct PICSTORE *last_motion;
  // optional - the last difference.
  struct PICSTORE *diff;
  // optional - a mask to stop differences in set pixels triggering motion
  struct PICSTORE *mask;
  // Optional static background image - to compare against.
  struct PICSTORE *background;

  int scaledwidth;
  int scaledheight;
  uint32_t changedPixelPertenthousand;

  uint32_t required_motion_buffer_len; // required frame buffer len - used to prevent continual reallocation after failure

  ////////////////////////////////////
  // triggers picked up by wcloop()
  volatile uint8_t motion_processed; // set to 1 each time it's processed.
  volatile uint8_t motion_triggered; // motion was over trigger limit

  ////////////////////////////////////
  // status/debug
  int32_t last_duration;
};
WC_Motion wc_motion;

void WcSetMotionDefaults();
void HandleImagemotionmask();
void HandleImagemotiondiff();
void HandleImagemotionbuff();
void HandleImagemotionlbuff();
void HandleImagemotionbackgroundbuff();
uint32_t WcSetMotionDetect(int32_t value);
bool WcConvertFrame(int32_t bnum_i, int format, int scale);
void WcMotionLog();

typedef struct {
        uint16_t width;
        uint16_t height;
        uint16_t data_offset;
        const uint8_t *input;
        struct PICSTORE *poutput;
} wc_rgb_jpg_decoder;

// unsigned int wc_jpg_read(void * arg, size_t index, uint8_t *buf, size_t len);
static bool _mono_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);
#ifdef WC_USE_RGB_DECODE
static bool wc_rgb_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);
static bool wc_rgb565_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);
#endif
bool wc_jpg2mono(const uint8_t *src, size_t src_len, struct PICSTORE * out, int scale);
bool convertJpegToPixels(const uint8_t *src_buf, size_t src_len, int width, int height, int scale, int format, struct PICSTORE *out);
void WcAutoMask();
void WcDetectMotionFn(uint8_t *_jpg_buf, int _jpg_buf_len);
void CmndWebcamConvertFrame(void);
void CmndWebcamSetPicture(void);
void CmndWebcamSetMotionDetect(void);
void CmndWebcamGetMotionPixels(void);

#endif


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj);

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

    #define D_CMND_WC_STARTTASK "Starttask"
    #define D_CMND_WC_STOPTASK "Stoptask"

    #define D_CMND_WC_MENUVIDEODISABLE "MenuVideoDisable"

    // for testing to see what happens after cam_stop()
    #define D_CMND_WC_INTERRUPT "Interrupt"

    // mainly for testing functions which could be used by scripts.
    #define D_CMND_WC_SETMOTIONDETECT "Setmotiondetect"
    #define D_CMND_WC_GETFRAME "Getframe"
    #define D_CMND_WC_GETPICSTORE "Getpicstore"

    #define D_CMND_WC_BERRYFRAMES "Berryframes"

    #define D_CMND_WC_SAVEPIC "SavePic"
    #define D_CMND_WC_APPENDPIC "AppendPic"

    #define D_CMND_WC_GETMOTIONPIXELS "GetMotionPixels"

    #define D_CMND_WC_SETOPTIONS "SetOptions"
    #define D_CMND_WC_CONVERTFRAME "ConvertFrame"
    #define D_CMND_WC_SETPICTURE "SetPicture"

    #define D_CMND_WC_POWEROFF "Poweroff"

    void CmndWebcamResolution(uint8_t resolution);
    void CmndWebcamMirror(bool mirror);
    void CmndWebcamFlip(bool flip);        
    void CmndWebcamSaturation(int8_t val);
    void CmndWebcamBrightness(int8_t val);
    void CmndWebcamContrast(int8_t val);
    void CmndWebcamSpecialEffect(uint8_t val);
    void CmndWebcamAWB(bool val);
    void CmndWebcamWBMode(uint8_t val);
    void CmndWebcamAWBGain(bool val);
    void CmndWebcamAEC(bool val);
    void CmndWebcamAECValue(uint16_t val);
    void CmndWebcamAELevel(int8_t val);
    void CmndWebcamAEC2(bool val);
    void CmndWebcamAGC(bool val);
    void CmndWebcamAGCGain(uint8_t val);
    void CmndWebcamGainCeiling(uint8_t val);
    void CmndWebcamGammaCorrect(bool val);
    void CmndWebcamLensCorrect(bool val);
    void CmndWebcamWPC(bool val);
    void CmndWebcamDCW(bool val);
    void CmndWebcamBPC(bool val);
    void CmndWebcamColorbar(bool val);
    void CmndWebcamFeature(uint8_t val);
    void CmndWebcamAuth(bool val);
    void CmndWebcamClock(uint16_t val);
    void CmndWebcamCamStartStop(bool val);
    void CmndWebcamSetDefaults();

    void CmndWebcamGetFrame(int bnum);
    void CmndWebcamGetPicStore(int bnum);

    void CmndWebcamPowerOff(void);
    void CmndWebcamTaskEnable(bool val);

    void CmndWebcamInit();
    void CmndWebRtsp(bool val);

    void WcStopTask();

    void SuspendAndShutdownCameraForOTA();


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