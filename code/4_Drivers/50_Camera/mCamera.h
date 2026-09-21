#ifndef HEADER_MODULE_DRIVERS__CAMERA_H
#define HEADER_MODULE_DRIVERS__CAMERA_H
  
#define D_UNIQUE_MODULE_DRIVERS__CAMERA_ID 4050 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS__CAMERA

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

#define WC_WAIT_INTERVAL_MS 10

#define WEBCAM_CORE 0


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
    
    static constexpr const char* PM_MODULE_DRIVERS__CAMERA_CTR = D_MODULE__DRIVERS__CAMERA__CTR;
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

    SemaphoreHandle_t WebcamMutex = nullptr;


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
            
    // use mutex like:
    // mSupport::AutoMutex localmutex(&WebcamMutex, "somename", 200);
    // in any function.  Will wait for mutex to be clear, and auto-release when the function exits.

    #define BOUNDARY "e8b8c539-047d-4777-a985-fbba6edff11e"

    #ifndef MAX_PICSTORE
    #define MAX_PICSTORE 4
    #endif

    #ifdef ENABLE_RTSPSERVER
    void EndRTSP();

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
      WebServer *CamServer;
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


      bool camera_init = false;

    } rt;

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

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    /************************************************************************************************
     * SECTION: Runtime State / Module Loop
     ************************************************************************************************/
    void Runtime_Reset(void);
    void EveryLoop(void);
    void Stats_Update(void);

    /************************************************************************************************
     * SECTION: Access / Pin Validation
     ************************************************************************************************/
    bool Access_WaitUntilZero(volatile int8_t* val, int8_t initial, int timeout_ms);
    void Access_WaitUntilEnabled(void);
    bool Pins_AreConfigured(void);
    void Driver_SetInterruptState(uint32_t state);

    /************************************************************************************************
     * SECTION: Defaults / Stored Configuration
     ************************************************************************************************/
    void Defaults_LoadOrCreate(void);
    void Defaults_Apply(uint32_t upgrade);
    void LoadDefaultConfig(void);

    /************************************************************************************************
     * SECTION: Driver Lifetime / Hardware Init
     ************************************************************************************************/
    uint32_t Driver_InitFromResolution(int32_t frame_size);
    void Driver_Shutdown(void);
    void Driver_SuspendForOTA(void);

    /************************************************************************************************
     * SECTION: Sensor Register Configuration
     ************************************************************************************************/
    void Sensor_ApplyStoredSettings(void);
    void Sensor_ApplyFeatureMode(int32_t mode);
    int32_t Options_Set(uint32_t option_id, int32_t value);

    /************************************************************************************************
     * SECTION: Frame Task / Runtime Capture Loop
     ************************************************************************************************/
    void FrameTask_Start(void);
    void FrameTask_Stop(void);
    void FrameTask_Loop(void);
    static void FrameTask_StaticLoop(void* pvParameters);

    void Frame_WaitForNext(int max_time_ms);
    uint32_t Frame_GetWidth(void);
    uint32_t Frame_GetHeight(void);
    uint32_t Frame_RequestCaptureToStore(int32_t store_index);

    /************************************************************************************************
     * SECTION: Picture Store / Image Buffer Management
     ************************************************************************************************/
    bool Picture_Alloc(struct mCamera::PICSTORE* ps, int width, int height, int jpeg_size, int format, int alloc_style);
    bool Picture_Free(struct mCamera::PICSTORE* ps);
    bool Picture_AllocPtr(struct mCamera::PICSTORE** pps, int width, int height, int jpeg_size, int format, int alloc_style);
    bool Picture_FreePtr(struct mCamera::PICSTORE** pps);
    bool Picture_IsPixelFormatSupported(int format);

    uint32_t PictureStore_GetBuffer(int32_t store_index, uint8_t** buffer);
    uint32_t PictureStore_Get(int32_t store_index, struct mCamera::PICSTORE** picture);

    /************************************************************************************************
     * SECTION: Stream Server / MJPEG / RTSP
     ************************************************************************************************/
    uint32_t Stream_SetEnabled(uint32_t stream_enabled);
    void Stream_End(void);
    void Stream_RemoveDeadClients(void);

    void Stream_HandleMjpeg(int stream_type);
    static void Stream_HandleMjpegRoot(void);
    void Stream_HandleMjpegDiff(void);

    #ifdef ENABLE_RTSPSERVER
    void Rtsp_SetFrame(void);
    void Rtsp_ClearFrame(void);
    void Rtsp_StreamTick(void);
    void Rtsp_End(void);
    #endif

    /************************************************************************************************
     * SECTION: Web UI / HTTP Endpoints
     ************************************************************************************************/
    bool Web_Authenticate(void);
    bool Web_CheckPrivilegedAccess(bool auto_request_auth = true);

    void Web_AddStreamButton(void);
    static void Web_HandleRoot(void);
    void Web_HandleImage(void);
    void Web_SendPictureStore(struct mCamera::PICSTORE* ps);
    void Web_HandleJpegCapture(AsyncWebServerRequest* request);

    #ifdef ENABLE_CAMERA__MOTION_DETECTION

      /************************************************************************************************
       * SECTION: Motion Configuration / State
       ************************************************************************************************/
      void Motion_DefaultsApply(void);
      uint32_t Motion_SetDetectionInterval(int32_t value);
      void Motion_LogState(void);

      /************************************************************************************************
       * SECTION: Motion Processing
       ************************************************************************************************/
      void Motion_ProcessJpegFrame(uint8_t* jpg_buffer, int jpg_len);
      void Motion_AutoMaskUpdate(void);
      bool Motion_ConvertStoredFrame(int32_t store_index, int format, int scale);

      /************************************************************************************************
       * SECTION: Codec / JPEG Conversion
       ************************************************************************************************/
      bool Codec_EncodeToJpeg(uint8_t* src, size_t src_len, int width, int height, int format, uint8_t quality, struct PICSTORE* dest);
      void Codec_ResetEncodedPicture(struct PICSTORE* dest);

      static bool Codec_JpegDecodeWriteMono(void* arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data);
      static bool Codec_JpegDecodeWriteRgb888(void* arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data);
      static bool Codec_JpegDecodeWriteRgb565(void* arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t* data);

      bool Codec_JpegToMono(const uint8_t* src, size_t src_len, struct PICSTORE* out, int scale);
      bool Codec_JpegToRgb888(const uint8_t* src, size_t src_len, struct PICSTORE* out, int scale);
      bool Codec_JpegToRgb565(const uint8_t* src, size_t src_len, struct PICSTORE* out, int scale);
      bool Codec_JpegToPixels(const uint8_t* src_buf, size_t src_len, int width, int height, int scale, int format, struct PICSTORE* out);

      /************************************************************************************************
       * SECTION: Motion Web Endpoints
       ************************************************************************************************/
      void Web_HandleMotionMaskImage(void);
      void Web_HandleMotionDiffImage(void);
      void Web_HandleMotionBufferImage(void);
      void Web_HandleMotionLastBufferImage(void);
      void Web_HandleMotionBackgroundImage(void);

  #endif // ENABLE_CAMERA__MOTION_DETECTION
        
    // #ifdef ENABLE_CAMERA__MOTION_DETECTION


    // /*********************************************************************************************\
    //  * ESP32 webcam motion routines
    //  *
    //  * WcGetmotionpixelsN = (N=1..4) read addr, len, w, h as JSON {"addr":123456,"len":12345,"w":160,"h":120, "format":4} 
    //  *    motion(1)
    //  *    difference(2) buffer - e.g for berry
    //  *    mask(3)
    //  *    background(4)
    //  *     e.g. could be used to read pixels, or change pixels from berry.

    // * WcConvertFrameN <format> <scale> - convert a wcgetframe in picstore from jpeg to <format> (0=2BPP/RGB565, 3=1BPP/GRAYSCALE, 5=3BPP/RGB888), <scale> (0-3)
    // *     converts in place, replacing the stored frame with the new format.  Data can be retrieved using wcgetpicstoreN (e.g. for use in berry)
    // *     will fail if it can't convert or allocate.
    // * 
    // * WcSetPicture - SetPictureN (N=1-MAX_PICTORE) expects 'addr len format [width height]
    // *   use to populate a frame in Wc.picstore from Berry.  e.g. to put a JPEG mask there
    // *   so you can then decode it, get it's address, get the address of the current mask, and 
    // *   copy data across.
    // *   if sending JPEG (format=0|5), width and height are calculated on decode.
    // *   if sending pixels (format=4(GRAY)|6(RGB)|1(RGB565)), width and height are required, and used to allocate.
    // *   binary data is copied from addr.  i.e. you can send the addr/len from Berry introspect bytes.
    // *    ideas: could be used to set background image based on time of day.

    // ### Enable motion detection interval
    // WCsetMotiondetect <timems>
    // WCsetMotiondetect 2000

    // ### disable motion detection
    // WCsetMotiondetect 0

    // ### return overall normalised pixel difference.
    // WCsetMotiondetect -1

    // ### return overall normalised brightness.
    // WCsetMotiondetect -2

    // ### motion detect via jpeg frame size (% change)
    // Does not run motion detect frame scan!
    // (i.e. you can turn off WCsetMotiondetect 0 - and detect at 50fps)
    // WCsetMotiondetect2 <fsizediff percent limit>
    // WCsetMotiondetect2 20

    // ### Pixel diff value threshold
    // if set > 0, pixels with a difference > threshold will be counted
    // if a difference buffer is enabled, then pixels in it are raised to 255.
    // WCsetMotiondetect3 <pixel diff threshold 1-255>
    // WCsetMotiondetect3 10

    // ### Pixels over threshold trigger
    // if the number of pixels changed (per 10000) is > limit, motion is triggered.
    // set 0 to disable.
    // WCsetMotiondetect4 <pixel count trigger thresh, in 1/10000>
    // WCsetMotiondetect4 10

    // ### set scale for motion detect image ref camera res
    // the number 0-7 = 1, 1/2, 1/4, 1/8, 1/16, 1/32, 1/64, 1/128
    // values 0-3 use scaling on jpeg decode (fast).
    // values 4-7 add software scaling (not much performance gain, but some) 
    // WCsetMotiondetect5 0-7

    // ### enable difference buffer
    // enable/disable the difference buffer
    // WCsetMotiondetect6 0-1

    // ### Set motion trigger threshold (accumulated pixels difference)
    // normalised value, try 1000...
    // WCsetMotiondetect7 0-nnnnn


    // Endpoints:
    // http://IP/motionlbuff.jpg - last motion buffer as jpeg (fully scaled) - if motion enabled, else 404
    // http://IP/motionbuff.jpg - motion buffer as jpeg (only jpeg scaled) - if motion enabled, else 404
    // http://IP/motiondiff.jpg - last difference buffer as jpeg (fully scaled) - if enabled, else 404

    // http://IP:81/diff.mjpeg - motion or difference buffer stream as mjpeg (fully scaled, buf is motion if diff not enabled) - if enabled, else no data

    // */

    // /*********************************************************************************************/

    // /*********************************************************************************************/
    // /*
    // Berry usage:
    // if you turn on motion detect with "wcsetmotiondetect 1000" (number is ms between detect)
    // or it you turn on frame sending using "wcberryframes 1"
    // then berry will be called expecting to find a driver which has a method "webcam", e.g.:

    // var piccount = 0
    // class MyWebcamDriver
    //   #- create a method for adding a button to the main menu -#
    //   def webcam(cmd, idx, payload)
    //     print("webcam "+cmd+" payload:"+payload)
    //     if (cmd == "motion")
    //       #split payload at space
    //       var val = int(payload)
    //       if (val > 1000)
    //         piccount = piccount+1
    //         var tcmd = "wcsavepic0 /myfile" .. piccount .. ".jpg"
    //         tasmota.cmd(tcmd)
    //         print("webcam motion -> "+tcmd)
    //       end
    //     end
    //   end
    // end

    // This  will be called with 
    // "motion <framediff> <<framebrightness>"
    // and/or
    // "frame"

    // */
    // /*********************************************************************************************/


    // // extern SemaphoreHandle_t WebcamMutex;


    // #define DEFAULT_INITIAL_JPEG_LEN 16384



    // // size_t WcJpegEncoderStore_jpg_out_cb(void * arg, size_t index, const void* data, size_t len);
    // bool WcencodeToJpeg(uint8_t *src, size_t srclen, int width, int height, int format, uint8_t quality, struct PICSTORE *dest);

    // void Wcencode_reset(struct PICSTORE *dest);

    // struct WC_Motion {
    //   /////////////////////////////////////
    //   // configured by user
    //   uint16_t motion_detect; // time between detections
    //   uint32_t motion_trigger_limit; // last amount of difference measured (~100 for none, > ~1000 for motion?)
    //   uint8_t scale; /*0=native, 1=/2, 2=/4, 3=/8*/
    //   uint8_t swscale; // skips pixels 0=native, 1=/2, 2=/4, 3=/8 - after scale
    //   uint8_t enable_diffbuff; // enable create of a buffer containing the last difference image
    //   uint8_t enable_backgroundbuff;
    //   uint8_t capture_background;

    //   uint8_t pixelThreshold;
    //   uint32_t  pixel_trigger_limit; // pertenthousand changed pixels

    //   uint8_t enable_mask; // enable mask buffer
    //   uint32_t auto_mask; // number of motion runs to run automask over
    //   uint8_t auto_mask_pixel_threshold; // pixel change threshold to add pixel to mask
    //   uint8_t auto_mask_pixel_expansion; // number of pixels atound the detected pixel to set in mask (square)

    //   ////////////////////////////////////
    //   // variables used in detection

    //   // set to 0 each any time we restart (new last_motion_buffer), goes to after first processing 
    //   // set to -1 on malloc failure - will happen with larger frames
    //   int8_t motion_state;
    //   uint32_t motion_ltime;  // time of last detect
    //   uint32_t motion_trigger; // last amount of difference measured (~100 for none, > ~1000 for motion?)
    //   uint32_t motion_brightness; // last frame brightness read (~15000)

    //   // jpeg is decoded (with scale) into here.
    //   struct PICSTORE *frame;
    //   // the others are both scale and swscale
    //   // the last image - to compare against.
    //   struct PICSTORE *last_motion;
    //   // optional - the last difference.
    //   struct PICSTORE *diff;
    //   // optional - a mask to stop differences in set pixels triggering motion
    //   struct PICSTORE *mask;
    //   // Optional static background image - to compare against.
    //   struct PICSTORE *background;

    //   int scaledwidth;
    //   int scaledheight;
    //   uint32_t changedPixelPertenthousand;

    //   uint32_t required_motion_buffer_len; // required frame buffer len - used to prevent continual reallocation after failure

    //   ////////////////////////////////////
    //   // triggers picked up by wcloop()
    //   volatile uint8_t motion_processed; // set to 1 each time it's processed.
    //   volatile uint8_t motion_triggered; // motion was over trigger limit

    //   ////////////////////////////////////
    //   // status/debug
    //   int32_t last_duration;
    // };
    // WC_Motion wc_motion;
    // inline void Motion_ClearStruct(){ memset(&wc_motion, 0, sizeof(wc_motion)); }

    // void Motion_SetDefaults();
    // void HandleImagemotionmask();
    // void HandleImagemotiondiff();
    // void HandleImagemotionbuff();
    // void HandleImagemotionlbuff();
    // void HandleImagemotionbackgroundbuff();
    // uint32_t WcSetMotionDetect(int32_t value);
    // bool WcConvertFrame(int32_t bnum_i, int format, int scale);
    // void WcMotionLog();

    // typedef struct {
    //         uint16_t width;
    //         uint16_t height;
    //         uint16_t data_offset;
    //         const uint8_t *input;
    //         struct PICSTORE *poutput;
    // } wc_rgb_jpg_decoder;

    // // unsigned int wc_jpg_read(void * arg, size_t index, uint8_t *buf, size_t len);
    // static bool _mono_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);
    // #ifdef WC_USE_RGB_DECODE
    // static bool wc_rgb_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);
    // static bool wc_rgb565_write(void * arg, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *data);
    // #endif
    // bool wc_jpg2mono(const uint8_t *src, size_t src_len, struct PICSTORE * out, int scale);
    // bool convertJpegToPixels(const uint8_t *src_buf, size_t src_len, int width, int height, int scale, int format, struct PICSTORE *out);
    // void WcAutoMask();
    // void WcDetectMotionFn(uint8_t *_jpg_buf, int _jpg_buf_len);
    // void CommandSet_ConvertFrame(void);
    // void CommandSet_SetPicture(void);
    // void CommandSet_SetMotionDetect(void);
    // void CommandSet_GetMotionPixels(void);

    // #endif


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj);

    void CommandSet_Resolution(uint8_t resolution);
    void CommandSet_Mirror(bool mirror);
    void CommandSet_Flip(bool flip);
    void CommandSet_Saturation(int8_t val);
    void CommandSet_Brightness(int8_t val);
    void CommandSet_Contrast(int8_t val);
    void CommandSet_SpecialEffect(uint8_t val);

    void CommandSet_AWB(bool val);
    void CommandSet_WBMode(uint8_t val);
    void CommandSet_AWBGain(bool val);

    void CommandSet_AEC(bool val);
    void CommandSet_AECValue(uint16_t val);
    void CommandSet_AELevel(int8_t val);
    void CommandSet_AEC2(bool val);

    void CommandSet_AGC(bool val);
    void CommandSet_AGCGain(uint8_t val);
    void CommandSet_GainCeiling(uint8_t val);

    void CommandSet_GammaCorrect(bool val);
    void CommandSet_LensCorrect(bool val);
    void CommandSet_WPC(bool val);
    void CommandSet_DCW(bool val);
    void CommandSet_BPC(bool val);
    void CommandSet_Colorbar(bool val);

    void CommandSet_Feature(uint8_t val);
    void CommandSet_Auth(bool val);
    void CommandSet_Clock(uint16_t val);
    void CommandSet_CamStartStop(bool val);
    void CommandSet_SetDefaults(void);
    void CommandSet_Init(void);
    void CommandSet_RtspActive(bool val);

    void CommandSet_PowerOff(void);
    void CommandSet_TaskEnable(bool val);

    void CommandSet_GetFrame(int store_index);
    void CommandSet_GetPicStore(int store_index);

    #ifdef ENABLE_CAMERA__MOTION_DETECTION
    void CommandSet_ConvertFrame(void);
    void CommandSet_SetPicture(void);
    void CommandSet_MotionDetect(void);
    void CommandSet_GetMotionPixels(void);
    #endif

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

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_method = 0, bool json_appending = true); //default is true
    
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT 
    void Telemetry_Init();
    std::vector<struct telemetry_handler<mCamera>*> telemetry_list;    
    struct telemetry_handler<mCamera> telemetry_settings;
    struct telemetry_handler<mCamera> telemetry_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT 


    
};

#endif // USE_MODULE_DRIVERS__CAMERA

#endif


/*********************************************************************************************\
 * 

Driver_*
Sensor_*
Defaults_*
Runtime_*
FrameTask_*
Frame_*
Picture_*
Stream_*
Web_*
Motion_*Picture_Alloc
Codec_*
Access_*
Pins_*
Stats_*



 * ESP32 webcam based on example in Arduino-ESP32 library
 *
 * Template as used on ESP32-CAM WiFi + bluetooth Camera Module Development Board ESP32 With Camera Module OV2640 Geekcreit for Arduino
 * {"NAME":"AITHINKER CAM","GPIO":[4992,1,672,1,416,5088,1,1,1,6720,736,704,1,1,5089,5090,0,5091,5184,5152,0,5120,5024,5056,0,0,0,0,4928,1,5094,5095,5092,0,0,5093],"FLAG":0,"BASE":2}
 *
 * Supported commands:
 * Interrupt     = Control streaming, 0 = stop, 1 = start
 * Resolution = Set resolution (max framerates at wcclock=20)
 * FPS quoted is (max, withonewebclient) at wcclock=20 on AIThinker esp32cam equivalent
 * FPS is mostly limited by sending data to webclients & rtsp, not by the camera.
 0 = FRAMESIZE_96X96,    // 96x96 (50fps, ~48fps) - at wcclock=30 (75, ~53)
 1 = FRAMESIZE_QQVGA,    // 160x120 (50fps ~47fps)
 2 = FRAMESIZE_QCIF,     // 176x144 (50fps ~47fps)
 3 = FRAMESIZE_HQVGA,    // 240x176 (50fps ~37fps?)
 4 = FRAMESIZE_240X240,  // 240x240 (50fps ~39fps)
 5 = FRAMESIZE_QVGA,     // 320x240 (50fps ~35fps)
 6 = FRAMESIZE_CIF,      // 400x296 (50fps ~31fps)
 7 = FRAMESIZE_HVGA,     // 480x320 (25fps ~24fps)
 8 = FRAMESIZE_VGA,      // 640x480 (25fps ~19fps)
 9 = FRAMESIZE_SVGA,     // 800x600 (25fps ~15.5fps)
 10 = FRAMESIZE_XGA,      // 1024x768 (12.5fps, ~10fps)
 11 = FRAMESIZE_HD,       // 1280x720 (12.5fps, ~9fps)
 12 = FRAMESIZE_SXGA,     // 1280x1024 (12.5fps, ~7.5fps)
 13 = FRAMESIZE_UXGA,     // 1600x1200 (12.5fps, ~5fps)
 // 3MP Sensors above this no yet supported with this driver
 14 = FRAMESIZE_FHD,      // 1920x1080
 15 = CAMERA DISABLED (from resolution -1)
 // not accessible
 15 = FRAMESIZE_P_HD,     //  720x1280
 // our resolution variable is 4 bit, so we can;t use these.
 16 = FRAMESIZE_P_3MP,    //  864x1536
 17 = FRAMESIZE_QXGA,     // 2048x1536
 // 5MP Sensors
 18 = FRAMESIZE_QHD,      // 2560x1440
 19 = FRAMESIZE_WQXGA,    // 2560x1600
 20 = FRAMESIZE_P_FHD,    // 1080x1920
 21 = FRAMESIZE_QSXGA,    // 2560x1920
 22 = FRAMESIZE_INVALID

Framerate: I found that achievable streamed framerate is VERY dependent on wifi.
e.g. latest BT router, but with android screensharing with TV, and framerate for resolution 0 was scatty between 20 and 40fps!!!!
As soon as the screenshare ended, back to 53fps at 30mhz clock.

 * Mirror     = Mirror picture, 0 = no, 1 = yes
 * Flip       = Flip picture, 0 = no, 1 = yes
 * Saturation = Set picture Saturation -2 ... +2
 * Brightness = Set picture Brightness -2 ... +2
 * Contrast   = Set picture Contrast -2 ... +2
 * SpecialEffekt = Set Special Picture Effect: 0 = off, 1 = , 2 = , 3 = , 4 = , 5 = , 6 =
 * AWB          = Auto White Balance, 0 = no, 1 = yes
 * WBMode       = White Balance Mode, 0 = auto, 1 =
 * AWBGain      = Auto White Balance Gain, 0 = no, 1 = yes
 * AEC          = Auto exposure control (Sensor), 0 = no, 1 = yes
 * AECDSP       = Auto exposure control (DSP), 0 = no, 1 = yes
 * AECValue     = Auto exposure control value, 0 ... 1024
 * AECLevel     = Auto exposure control level, -2 ... +2
 * AGC          = Auto gain control, 0 = no, 1 = yes
 * AGCGain      = Auto gain control gain, 0 .. 30
 * GainCeiling  = Gain ceiling, 0 .. 6 (0 = x2, 1 = x4, 2 = x8, 3 = x16, 4 = x32, 5 = x64, 6 = x128)
 * GammaCorrect = Auto Gamma Correct, 0 = no, 1 = yes
 * LensCorrect  = Auto Lens Correct, 0 = no, 1 = yes
 * WPC          = White Pixel Correct, 0 = no, 1 = yes
 * DCW          = Downscale, 0 = no, 1 = yes
 * BPC          = Black Pixel Correct, 0 = no, 1 = yes
 * Colorbar     = Show Colorbar, 0 = no, 1 = yes
 * Feature      = Set extended Feature, 0 = off, 1 = reduce FPS, 2 = Nightmode
 * stats        = Show Statistics
 * Init         = Init Camera Interface
 * Rtsp         = Control RTSP Server, 0=disable, 1=enable (forces restart) (if defined ENABLE_RTSPSERVER)

 * GetframeN    = read a picture from camera and store in slot N - return {"addr":123456,"len":12345,"w":160,"h":120, "format":5}
 * SavepicN     = save a picture 1-4 after Getframe or Savepic0 to get frame 1 and save. argument is filename
 * AppendpicN   = append a picture 1-4 to a file after Getframe or Savepic0 to get frame 1 and save. argument is filename
 * GetpistoreN  = read buff, addr, len of a stored image, returns json (e.g. for berry) {"addr":123456,"len":12345,"w":160,"h":120, "format":5}
 *    if N==0 take a picture to 1 first.
 * GetmotionpixelsN = (N=1..4) read addr, len, w, h as JSON {"addr":123456,"len":12345,"w":160,"h":120, "format":4} 
 *    motion(1)
 *    difference(2) buffer - e.g for berry
 *    mask(3)
 *    background(4)
 *     e.g. could be used to read pixels, or change pixels from berry.

 * Menuvideodisable 0 = turn on video in main menu
 * Menuvideodisable 1 = turn off video in main menu

 * SetOptionsNN  = call SetOptions function with (index, payload).
 *   - can oprate many of the functions above, plus:
 *     Setoptions24 <frames to skip at input> - e.g. Setoptions24 2 will give you only every 3rd frame
 *     Setoptions25 <camPixelFormat> - espcam format + 1.  0->default->JPEG.   1:2BPP/RGB565, 2:2BPP/YUV422, 3:1.5BPP/YUV420, 4:1BPP/GRAYSCALE 5:JPEG/COMPRESSED 6:3BPP/RGB888 7:RAW 8:3BP2P/RGB444 9:3BP2P/RGB555
 *        Maybe useful for tensorflow or similar?
 *        ***** I COULD NOT GET THIS TO WORK ON MY AiThinker style cam ******

 * ConvertFrameN <format> <scale> - convert a wcgetframe in picstore from jpeg to <format> (0=2BPP/RGB565, 3=1BPP/GRAYSCALE, 5=3BPP/RGB888), <scale> (0-3)
 *     converts in place, replacing the stored frame with the new format.  Data can be retrieved using wcgetpicstoreN (e.g. for use in berry)
 *     will fail if it can't convert or allocate.
 * 
 * PowerOff - power down camera.  Init will bring it back up.
 * SetPicture - SetPictureN (N=1-MAX_PICTORE) expects 'addr len format [width height]
 *   use to populate a frame in .picstore from Berry.  e.g. to put a JPEG mask there
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
http://IP/wc.jpg - cam picture
http://IP/wc.mjpeg - cam picture
http://IP/snapshot.jpg - cam picture

http://IP:81/diff.mjpeg - motion or difference buffer stream as mjpeg (fully scaled, buf is motion if diff not enabled) - if enabled, else no data
http://IP:81/cam.mjpeg - cam stream.
http://IP:81/cam.jpg - cam stream.
http://IP:81/stream - cam stream.


Both webstream and RTSP can handle multiple clients.
Multiple clients will reduce the framerate.
Wifi use has a big effect on framerate.


 *
 * Feature Explanation
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
 * the AITHINKER module does not have CAM_RESET - so if you get the camera into a bad state, power off restart is the only way out.
 * flash led = gpio 4
 * red led = gpio 33
 * optional rtsp url: rtsp://xxx.xxx.xxx.xxx:8554/mjpeg/1
 * 
 * SH 2023-05-14 - added mutex for many webcam functions - this is to prevent multi-threaded access to the camera functions, which 
 * can case error 0x105 upon re-init.
 * Errors 0x103 and 0xffffffff could indicate CAM_PWDN incorrect.
 * Error 0x105 is likely caused by the camera not being powered long enough before init.
 * 
 * I2C use: if USE_I2C is enabled, you can set GPIO26 to I2c_SDA/2 and GPIO27 to I2C_SCL/2, and then use the shared I2C bus 2.
 * Then you can use cmd i2cscan2 to check for camera presence.
 */

/*********************************************************************************************/

