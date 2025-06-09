#include "mCamera.h"

#ifdef USE_MODULE_DRIVERS__CAMERA_2025


SemaphoreHandle_t WebcamMutex = nullptr;

// these are the frame intervals at 20mhz
// empirically, they directly scale with frequency.
// for 30mhz, 50fps -> 75fps; for 15 mhz, 37.5fps, 
// Q: what happens at 30 mains?
const int nativeIntervals20ms[14] = {
  20, //0 = FRAMESIZE_96X96,    // 96x96 (50fps, ~48fps) - at wcclock=30 (75, ~53)
  20, //1 = FRAMESIZE_QQVGA,    // 160x120 (50fps ~47fps)
  20, //2 = FRAMESIZE_QCIF,     // 176x144 (50fps ~47fps)
  20, //3 = FRAMESIZE_HQVGA,    // 240x176 (50fps ~37fps?)
  20, //4 = FRAMESIZE_240X240,  // 240x240 (50fps ~39fps)
  20, //5 = FRAMESIZE_QVGA,     // 320x240 (50fps ~35fps)
  20, //6 = FRAMESIZE_CIF,      // 400x296 (50fps ~31fps)
  20, //7 = FRAMESIZE_HVGA,     // 480x320 (25fps ~24fps)
  40, //8 = FRAMESIZE_VGA,      // 640x480 (25fps ~19fps)
  40, //9 = FRAMESIZE_SVGA,     // 800x600 (25fps ~15.5fps)
  80, //10 = FRAMESIZE_XGA,      // 1024x768 (12.5fps, ~10fps)
  80, //11 = FRAMESIZE_HD,       // 1280x720 (12.5fps, ~9fps)
  80, //12 = FRAMESIZE_SXGA,     // 1280x1024 (12.5fps, ~7.5fps)
  80, //13 = FRAMESIZE_UXGA,     // 1600x1200 (12.5fps, ~5fps)
};

/*********************************************************************************************\
 * ESP32 webcam based on example in Arduino-ESP32 library
 *
 * Template as used on ESP32-CAM WiFi + bluetooth Camera Module Development Board ESP32 With Camera Module OV2640 Geekcreit for Arduino
 * {"NAME":"AITHINKER CAM","GPIO":[4992,1,672,1,416,5088,1,1,1,6720,736,704,1,1,5089,5090,0,5091,5184,5152,0,5120,5024,5056,0,0,0,0,4928,1,5094,5095,5092,0,0,5093],"FLAG":0,"BASE":2}
 *
 * Supported commands:
 * WcInterrupt     = Control streaming, 0 = stop, 1 = start
 * WcResolution = Set resolution (max framerates at wcclock=20)
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
 15 = CAMERA DISABLED (from wcresolution -1)

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

 * WcGetframeN    = read a picture from camera and store in slot N - return {"addr":123456,"len":12345,"w":160,"h":120, "format":5}
 * WcSavepicN     = save a picture 1-4 after WcGetframe or WcSavepic0 to get frame 1 and save. argument is filename
 * WcAppendpicN   = append a picture 1-4 to a file after WcGetframe or WcSavepic0 to get frame 1 and save. argument is filename
 * WcGetpistoreN  = read buff, addr, len of a stored image, returns json (e.g. for berry) {"addr":123456,"len":12345,"w":160,"h":120, "format":5}
 *    if N==0 take a picture to 1 first.
 * WcGetmotionpixelsN = (N=1..4) read addr, len, w, h as JSON {"addr":123456,"len":12345,"w":160,"h":120, "format":4} 
 *    motion(1)
 *    difference(2) buffer - e.g for berry
 *    mask(3)
 *    background(4)
 *     e.g. could be used to read pixels, or change pixels from berry.

 * WcMenuvideodisable 0 = turn on video in main menu
 * WcMenuvideodisable 1 = turn off video in main menu

 * WcSetOptionsNN  = call WcSetOptions function with (index, payload).
 *   - can oprate many of the functions above, plus:
 *     WcSetoptions24 <frames to skip at input> - e.g. WcSetoptions24 2 will give you only every 3rd frame
 *     WcSetoptions25 <camPixelFormat> - espcam format + 1.  0->default->JPEG.   1:2BPP/RGB565, 2:2BPP/YUV422, 3:1.5BPP/YUV420, 4:1BPP/GRAYSCALE 5:JPEG/COMPRESSED 6:3BPP/RGB888 7:RAW 8:3BP2P/RGB444 9:3BP2P/RGB555
 *        Maybe useful for tensorflow or similar?
 *        ***** I COULD NOT GET THIS TO WORK ON MY AiThinker style cam ******

 * WcConvertFrameN <format> <scale> - convert a wcgetframe in picstore from jpeg to <format> (0=2BPP/RGB565, 3=1BPP/GRAYSCALE, 5=3BPP/RGB888), <scale> (0-3)
 *     converts in place, replacing the stored frame with the new format.  Data can be retrieved using wcgetpicstoreN (e.g. for use in berry)
 *     will fail if it can't convert or allocate.
 * 
 * WcPowerOff - power down camera.  WcInit will bring it back up.
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

New commands:
WcGetFrame <n> or WcGetFrame<n> - reads the next frame into slot n (1-4)
WcSavePic<n> fname
WcAppendPic<n> fname
(n is 0-4) - 0 will trigger a read of a picture now,
These will save or append a picture to a file.  The picture must have been first read with WcGetFrame <n> if n > 0


*/
/*********************************************************************************************/



#ifdef ENABLE_RTSPSERVER

#include <OV2640.h>
#include <SimStreamer.h>
#include <OV2640Streamer.h>
#include <CRtspSession.h>
#ifndef RTSP_FRAME_TIME
#define RTSP_FRAME_TIME 100
#endif // RTSP_FRAME_TIME

// RTSP streamer class
class localOV2640Streamer : public CStreamer {
    BufPtr f_ptr; // temp pointer to frame buffer
    int f_len;
public:
    localOV2640Streamer(SOCKET aClient, int width, int height);
    void setframe(BufPtr ptr, int len);
    void clearframe();
    virtual void    streamImage(uint32_t curMsec);
};

localOV2640Streamer::localOV2640Streamer(SOCKET aClient, int width, int height) : CStreamer(aClient, width, height) {
  clearframe();
#ifdef DEBUG_DRIVERS__CAMERA_2025  
  ALOG_DBG(PSTR("CAM:RTSP w%d h%d"), width, height);
#endif
}
void localOV2640Streamer::setframe(BufPtr ptr, int len) {
  f_ptr = ptr;
  f_len = len;
}
void localOV2640Streamer::clearframe() {
  f_ptr = nullptr;
  f_len = 0;
}
void localOV2640Streamer::streamImage(uint32_t curMsec){
  if (!f_ptr) return;
  streamFrame(f_ptr, f_len, curMsec);
  //ALOG_DBG(PSTR(D_LOG_CAMERA "RTSP Stream Frame %d"), f_len);
}

typedef struct tag_wc_rtspclient {
  localOV2640Streamer * volatile camStreamer;
  CRtspSession * volatile rtsp_session;
  WiFiClient rtsp_client;
  tag_wc_rtspclient * volatile p_next;
} wc_rtspclient;
#endif //ENABLE_RTSPSERVER


bool mCamera::pic_alloc(struct mCamera::PICSTORE *ps, int width, int height, int jpegsize, int format, int allocstyle){
  int len = 0;
  bool res = false;
  uint8_t* orgbuf = nullptr;
  int orglen = 0;

  if (!ps)
  {
    #ifdef DEBUG_DRIVERS__CAMERA_2025  
    ALOG_ERR(PSTR(D_LOG_CAMERA "pic_alloc ps null"));
    #endif
    return false;
  }

  switch(format){
    default:                  len = 0;                  break;
    case PIXFORMAT_JPEG:      len = jpegsize;           break;
    case PIXFORMAT_GRAYSCALE: len = width*height + 4;   break;
    case PIXFORMAT_RGB888:    len = width*height*3 + 4; break;
    case PIXFORMAT_RGB565:    len = width*height*2 + 4; break;
  }

  if (!len)
  {
    #ifdef DEBUG_DRIVERS__CAMERA_2025
    ALOG_ERR(PSTR(D_LOG_CAMERA "pic_alloc invalid format or len 0"));
    #endif    
    return false;
  }

  switch(allocstyle & 0xf){
    case WC_ALLOC_ALWAYS: // just free & allocate, unless no change
      if (len != ps->allocatedLen){
        ps->allocatedLen = -1; // force realloc
      }
      break;
    case WC_REALLOC_IF_SMALLER: // allocate if buf not big enough
      // will use ps->allocatedLen to check below
      break;
    case WC_REALLOC_WITH_COPY: // allocate and copy data if buf not big enough
      orgbuf = ps->buff;
      orglen = ps->allocatedLen;
      break;
  }

  if (ps->allocatedLen < len){
    free(ps->buff);
    memset(ps, 0, sizeof(*ps));
  }
  if (!ps->buff){
    if (len){
      if ((allocstyle & WC_USE_FAST_MEM) || !Wc.psram){
        ps->buff = (uint8_t *)malloc(len);
      } else {
        ps->buff = (uint8_t *)heap_caps_aligned_alloc(4, len, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
      }
      if (ps->buff) ps->allocatedLen = len;
      if (orgbuf && orglen){
        memcpy(ps->buff, orgbuf, orglen);
      }
    }
  }
  if (ps->buff){
    ps->len = len;
    // don't change  if 0 in, as jpeg decode sets at start only
    if (width) ps->width = width;
    if (height) ps->height = height;
    ps->format = format;
  }

  if (ps->buff) res = true;
  return res;
}



// return true if pic had content.
bool mCamera::pic_free(struct mCamera::PICSTORE *ps){
  bool res = false;
  if (ps){
    if (ps->buff){
      free(ps->buff);
      res = true;
    }
    memset(ps, 0, sizeof(*ps));
  }
  return res;
}

bool mCamera::pic_alloc_p(struct mCamera::PICSTORE **pps, int width, int height, int jpegsize, int format, int allocstyle){
  if (!*pps){
    *pps = (struct mCamera::PICSTORE *)malloc(sizeof(struct mCamera::PICSTORE));
    memset(*pps, 0, sizeof(struct mCamera::PICSTORE));
  }
  if (*pps){
    return pic_alloc(*pps, width, height, jpegsize, format, allocstyle);
  }
  return false;
}
bool mCamera::pic_free_p(struct mCamera::PICSTORE **pps){
  bool res = false;
  if (*pps){
    res = pic_free(*pps);
    free(*pps);
    *pps = nullptr;
  }
  return res;
}


bool mCamera::wc_check_format(int format){
  switch(format){
    case PIXFORMAT_JPEG:      return true;
    case PIXFORMAT_GRAYSCALE: return true;
#ifdef WC_USE_RGB_DECODE    
    case PIXFORMAT_RGB888:    return true;
    case PIXFORMAT_RGB565:    return true;
#endif
  }
  return false;
}



#ifdef USE_WEBCAM_MOTION

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

extern WC_Motion wc_motion;
#endif



/*********************************************************************************************/

// this is called from outside to prevent conflicts in the esp.
// specifically from 
// xdrv_50_filesystem.ino - bool TfsSaveFile(const char *fname, const uint8_t *buf, uint32_t len)
// support_esp.ino - void NvmSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen)
void mCamera::WcInterrupt(uint32_t state) {
  mSupport::TasAutoMutex localmutex(&WebcamMutex, "WcInterrupt", 20000);
  // Stop camera ISR if active to fix TG1WDT_SYS_RESET
  if (!Wc.up) { return; }

  // why stop/start the server itself here?
  // stopping the cam interrupt should be enough?
  //WcSetStreamserver(state);
  if (state) {
    // Re-enable interrupts
    cam_start();
#ifdef DEBUG_DRIVERS__CAMERA_2025  
    ALOG_DBG(PSTR(D_LOG_CAMERA "cam_start()"));
#endif    
    Wc.disable_cam = 0;
  } else {
    // Stop interrupts
    Wc.disable_cam = 1;
    cam_stop();
#ifdef DEBUG_DRIVERS__CAMERA_2025  
    ALOG_DBG(PSTR(D_LOG_CAMERA "cam_stop()"));
#endif
  }
}


// wait timeout_ms for the value to clear
// use to wait for task to do something
#define WC_WAIT_INTERVAL_MS 10
bool mCamera::WcWaitZero(volatile int8_t *val, int8_t initial, int timeout_ms){
  int loops = timeout_ms/WC_WAIT_INTERVAL_MS;
  if (!loops) loops = 1;
  *val = initial;
  while(*val && loops--){
    vTaskDelay(WC_WAIT_INTERVAL_MS / portTICK_PERIOD_MS);
  }
  if (*val) return false;
  return true; 
}

// TAS will disable us for short periods...
// this wait for TAS to re-enable it
void mCamera::WcWaitEnable(){
  int timeout_ms = 1000;
  int loops = timeout_ms/WC_WAIT_INTERVAL_MS;
  if (!loops) loops = 1;
  while(Wc.disable_cam && loops--){
    vTaskDelay(WC_WAIT_INTERVAL_MS / portTICK_PERIOD_MS);
  }
}

bool mCamera::WcPinUsed(void) {
  bool pin_used = true;
  for (uint32_t i = 0; i < GPIO_WEBCAM_DATA8; i++) {
    if (!tkr_pins->PinUsed(GPIO_WEBCAM_DATA1, i)) {
      pin_used = false;
    }
//    if (i < MAX_WEBCAM_HSD) {
//      if (!PinUsed(GPIO_WEBCAM_HSD, i)) {
//        pin_used = false;
//      }
//    }
  }

#ifdef DEBUG_DRIVERS__CAMERA_2025  
  ALOG_DBG(PSTR(D_LOG_CAMERA "i2c_enabled_2: %d"), tkr_set->runtime.i2c_enabled/*[1]*/);
#endif

  if (!tkr_pins->PinUsed(GPIO_WEBCAM_XCLK) || !tkr_pins->PinUsed(GPIO_WEBCAM_PCLK) ||
      !tkr_pins->PinUsed(GPIO_WEBCAM_VSYNC) || !tkr_pins->PinUsed(GPIO_WEBCAM_HREF) ||
      ((!tkr_pins->PinUsed(GPIO_WEBCAM_SIOD) || !tkr_pins->PinUsed(GPIO_WEBCAM_SIOC)) && !tkr_set->runtime.i2c_enabled/*[1]*/)    // preferred option is to reuse and share I2Cbus 2
      ) {
        pin_used = false;
  }
  return pin_used;
}

void mCamera::WcFeature(int32_t value) {
  mSupport::TasAutoMutex localmutex(&WebcamMutex, "WcFeature", 200);
  sensor_t * wc_s = esp_camera_sensor_get();
  if (!wc_s) { return; }

  if (value != 1) {
      // CLKRC: Set Clock Divider to 0 = fullspeed
      wc_s->set_reg(wc_s, 0x111, 0x3f, 0x00);
      vTaskDelay(200 / portTICK_PERIOD_MS);
  }
  if (value != 2) {
      // Stop Nightmode
      wc_s->set_reg(wc_s, 0x103, 0xff, 0x0a);   // COM1: Reset dummy frames
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x43);   // Reserved Reg
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x4b);   // Reserved Reg
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x43);   // Reserved Reg
  }

  switch(value) {
    case 1:
      // Reduce FPS
      // CLKRC: Set Clock Divider to 2
      wc_s->set_reg(wc_s, 0x111, 0x3f, 0x02);
      break;
    case 2:
      // Start Nightmode
      wc_s->set_reg(wc_s, 0x10f, 0xff, 0x4b);   // Reserved Reg
      wc_s->set_reg(wc_s, 0x103, 0xff, 0xcf);   // COM1: Allow 7 dummy frames
      break;
  }
#ifdef DEBUG_DRIVERS__CAMERA_2025  
  ALOG_DBG(PSTR(D_LOG_CAMERA "Feature: %d"), value);
#endif  
}

void mCamera::WcApplySettings() {
  mSupport::TasAutoMutex localmutex(&WebcamMutex, "WcApplySettings", 200);
  sensor_t * wc_s = esp_camera_sensor_get();
  if (!wc_s) { return; }

  wc_s->set_vflip(wc_s, tkr_set->Settings.webcam_config.flip);
  wc_s->set_hmirror(wc_s, tkr_set->Settings.webcam_config.mirror);

  wc_s->set_brightness(wc_s, tkr_set->Settings.webcam_config.brightness - 2);
  wc_s->set_saturation(wc_s, tkr_set->Settings.webcam_config.saturation - 2);
  wc_s->set_contrast(wc_s, tkr_set->Settings.webcam_config.contrast - 2);

  wc_s->set_special_effect(wc_s, tkr_set->Settings.webcam_config2.special_effect);

  wc_s->set_whitebal(wc_s, tkr_set->Settings.webcam_config.awb);
  wc_s->set_wb_mode(wc_s, tkr_set->Settings.webcam_config2.wb_mode);
  wc_s->set_awb_gain(wc_s, tkr_set->Settings.webcam_config.awb_gain);

  wc_s->set_exposure_ctrl(wc_s, tkr_set->Settings.webcam_config.aec);
  wc_s->set_aec_value(wc_s, tkr_set->Settings.webcam_config2.aec_value - 2);
  wc_s->set_ae_level(wc_s, tkr_set->Settings.webcam_config2.ae_level);
  wc_s->set_aec2(wc_s, tkr_set->Settings.webcam_config.aec2);

  wc_s->set_gain_ctrl(wc_s, tkr_set->Settings.webcam_config.agc);
  wc_s->set_agc_gain(wc_s, tkr_set->Settings.webcam_config2.agc_gain);
  wc_s->set_gainceiling(wc_s, (gainceiling_t)tkr_set->Settings.webcam_config2.gainceiling);

  wc_s->set_raw_gma(wc_s, tkr_set->Settings.webcam_config.raw_gma);
  wc_s->set_lenc(wc_s, tkr_set->Settings.webcam_config.lenc);
  wc_s->set_wpc(wc_s, tkr_set->Settings.webcam_config.wpc);
  wc_s->set_dcw(wc_s, tkr_set->Settings.webcam_config.dcw);
  wc_s->set_bpc(wc_s, tkr_set->Settings.webcam_config.bpc);

  WcFeature(tkr_set->Settings.webcam_config.feature);

  ALOG_DBG(PSTR(D_LOG_CAMERA "Settings updated"));
}

void mCamera::WcSetDefaults(uint32_t upgrade) {
  if (!upgrade) {
    tkr_set->Settings.webcam_config.flip = 0;
    tkr_set->Settings.webcam_config.mirror = 0;

    tkr_set->Settings.webcam_config.saturation = 2; // = 0
    tkr_set->Settings.webcam_config.brightness = 2; // = 0
    tkr_set->Settings.webcam_config.contrast = 2;   // = 0
  }

  tkr_set->Settings.webcam_config2.special_effect = 0;
  tkr_set->Settings.webcam_config.colorbar = 0;

  tkr_set->Settings.webcam_config.awb = 1;        // white balance
  tkr_set->Settings.webcam_config2.wb_mode = 0;   // white balance mode
  tkr_set->Settings.webcam_config.awb_gain = 1;   // white blance gain

  tkr_set->Settings.webcam_config.aec = 1;          // autoexposure (sensor)
  tkr_set->Settings.webcam_config.aec2 = 1;         // autoexposure (dsp)
  tkr_set->Settings.webcam_config2.ae_level = 2;    // autoexposure level (-2 - +2, default 0)
  tkr_set->Settings.webcam_config2.aec_value = 204; // manual exposure value

  tkr_set->Settings.webcam_config.agc = 1;          // auto gain control
  tkr_set->Settings.webcam_config2.agc_gain = 5;    // manual gain control
  tkr_set->Settings.webcam_config2.gainceiling = 0; // auto gain ceiling

  tkr_set->Settings.webcam_config.raw_gma = 1;      // gamma correct
  tkr_set->Settings.webcam_config.lenc = 1;         // lens correction
  tkr_set->Settings.webcam_config.wpc = 1;          // white pixel correct
  tkr_set->Settings.webcam_config.dcw = 1;          // downsize en
  tkr_set->Settings.webcam_config.bpc = 0;          // black pixel correct?

  tkr_set->Settings.webcam_config.feature = 0;
  
  #ifdef USE_WEBCAM_MOTION
    WcSetMotionDefaults();
  #endif

#ifdef DEBUG_DRIVERS__CAMERA_2025  
  ALOG_DBG(PSTR(D_LOG_CAMERA "Defaults set"));
#endif
  if (Wc.up) { WcApplySettings(); }
}


uint32_t mCamera::WcSetup(int32_t fsiz) {
  // we must stall until re-enabled
  WcWaitEnable();

  mSupport::TasAutoMutex localmutex(&WebcamMutex, "WcSetup", 200);

#ifdef DEBUG_DRIVERS__CAMERA_2025  
  ALOG_DBG(PSTR(D_LOG_CAMERA "WcSetup"));
#endif  
  // if 15, make it -1, so disableing
  if (fsiz >= FRAMESIZE_FHD) { fsiz = -1; }

  if (fsiz < 0) {
    if (Wc.up){    
      esp_camera_deinit();
      ALOG_DBG(PSTR(D_LOG_CAMERA "Deinit fsiz %d"), fsiz);
      Wc.up = 0;
    }
    Wc.lastCamError = 0x1;
    return 0;
  }

  if (Wc.up) {
    esp_camera_deinit();
#ifdef DEBUG_DRIVERS__CAMERA_2025  
    ALOG_DBG(PSTR(D_LOG_CAMERA "Deinit"));
#endif    
    //return Wc.up;
  }
  Wc.up = 0;

//esp_log_level_set("*", ESP_LOG_VERBOSE);

  camera_config_t config;

  memset(&config, 0, sizeof(config));

  if (WcPinUsed()) {
    config.pin_d0       = tkr_pins->Pin(GPIO_WEBCAM_DATA1);        // Y2_GPIO_NUM;
    config.pin_d1       = tkr_pins->Pin(GPIO_WEBCAM_DATA1, 1);     // Y3_GPIO_NUM;
    config.pin_d2       = tkr_pins->Pin(GPIO_WEBCAM_DATA1, 2);     // Y4_GPIO_NUM;
    config.pin_d3       = tkr_pins->Pin(GPIO_WEBCAM_DATA1, 3);     // Y5_GPIO_NUM;
    config.pin_d4       = tkr_pins->Pin(GPIO_WEBCAM_DATA1, 4);     // Y6_GPIO_NUM;
    config.pin_d5       = tkr_pins->Pin(GPIO_WEBCAM_DATA1, 5);     // Y7_GPIO_NUM;
    config.pin_d6       = tkr_pins->Pin(GPIO_WEBCAM_DATA1, 6);     // Y8_GPIO_NUM;
    config.pin_d7       = tkr_pins->Pin(GPIO_WEBCAM_DATA1, 7);     // Y9_GPIO_NUM;
    config.pin_xclk     = tkr_pins->Pin(GPIO_WEBCAM_XCLK);      // XCLK_GPIO_NUM;
    config.pin_pclk     = tkr_pins->Pin(GPIO_WEBCAM_PCLK);      // PCLK_GPIO_NUM;
    config.pin_vsync    = tkr_pins->Pin(GPIO_WEBCAM_VSYNC);    // VSYNC_GPIO_NUM;
    config.pin_href     = tkr_pins->Pin(GPIO_WEBCAM_HREF);      // HREF_GPIO_NUM;
    config.pin_sccb_sda = tkr_pins->Pin(GPIO_WEBCAM_SIOD);  // SIOD_GPIO_NUM; - unset to use shared I2C bus 2
    config.pin_sccb_scl = tkr_pins->Pin(GPIO_WEBCAM_SIOC);  // SIOC_GPIO_NUM;
    if(tkr_set->runtime.i2c_enabled/*[1]*/){              // configure SIOD and SIOC as SDA,2 and SCL,2
      config.sccb_i2c_port = 1;                   // reuse initialized bus 2, can be shared now
      if(config.pin_sccb_sda < 0){                // GPIO_WEBCAM_SIOD must not be set to really make it happen
#ifdef DEBUG_DRIVERS__CAMERA_2025  
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "Use I2C bus2"));
#endif        
      }
    }
    config.pin_pwdn = tkr_pins->Pin(GPIO_WEBCAM_PWDN);       // PWDN_GPIO_NUM;
    config.pin_reset = tkr_pins->Pin(GPIO_WEBCAM_RESET);    // RESET_GPIO_NUM;
#ifdef DEBUG_DRIVERS__CAMERA_2025  
    ALOG_DBG(PSTR(D_LOG_CAMERA "Template pin config"));
#endif    
  } else if (Y2_GPIO_NUM != -1) {
    // Modell is set in camera_pins.h
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
#ifdef DEBUG_DRIVERS__CAMERA_2025  
    ALOG_DBG(PSTR(D_LOG_CAMERA "Compile flag pin config"));
#endif    
  } else {
    // no valid config found -> abort
    ALOG_DBG(PSTR(D_LOG_CAMERA "No pin config"));
    return 0;
  }

  // always power cycle the camera
  // this adds 400ms to start delay, but is worth it to solve random 0x105
  if (config.pin_pwdn >= 0){
    ALOG_DBG(PSTR(D_LOG_CAMERA "pwdn pin %d"), config.pin_pwdn);
    // this is only done in driver first init
    // so first run, we should configure as they do.
    gpio_config_t conf = { 0 };
    conf.pin_bit_mask = 1LL << config.pin_pwdn;
    conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&conf);

    int power_delay = 200;
    // if currently powered
    if (!gpio_get_level((gpio_num_t)config.pin_pwdn)){
      // power off for 200ms
      gpio_set_level((gpio_num_t)config.pin_pwdn, 1);
      vTaskDelay(power_delay / portTICK_PERIOD_MS);
    }

    // if camera currently powered off, power it up and wait for 200ms
    // this is a trial to see if it reduces 0x105
    // - and it solves the 0x105 issue!!!
    if (gpio_get_level((gpio_num_t)config.pin_pwdn)){
      gpio_set_level((gpio_num_t)config.pin_pwdn, 0);
      vTaskDelay(power_delay / portTICK_PERIOD_MS);
    }
  }

#ifdef DEBUG_DRIVERS__CAMERA_2025  
  ALOG_DBG("CAM: get ledc channel");
#endif

  int32_t ledc_channel = analogAttach(config.pin_xclk);/*added by me*/
  if (ledc_channel < 0) {
#ifdef DEBUG_DRIVERS__CAMERA_2025  
    AddLog(LOG_LEVEL_ERROR, "CAM: cannot allocated ledc channel, remove a PWM GPIO");
#endif    
  }
  config.ledc_channel = (ledc_channel_t) ledc_channel;
#ifdef DEBUG_DRIVERS__CAMERA_2025  
  AddLog(LOG_LEVEL_DEBUG_MORE, "CAM: XCLK on GPIO %i using ledc channel %i", config.pin_xclk, config.ledc_channel);
#endif  
  config.ledc_timer = LEDC_TIMER_0;
//  config.xclk_freq_hz = 20000000;
  if (!tkr_set->Settings.webcam_clk) tkr_set->Settings.webcam_clk = 20;
  config.xclk_freq_hz = tkr_set->Settings.webcam_clk * 1000000;
  int pixFormat = PIXFORMAT_JPEG;
/* 2023-05-28 - AiThinker type cam module marked DM.
  tried everything here, and it seems you cannot get anything other than JPEG.
  even if it's set to another value at power on.  
  Maybe it will work with a different camera, or a board with a CAM_RESET line.
*/
  if (Wc.camPixelFormat) pixFormat = (Wc.camPixelFormat-1);
  
  config.pixel_format = (pixformat_t)pixFormat;
  if (config.pixel_format != PIXFORMAT_JPEG){
#ifdef DEBUG_DRIVERS__CAMERA_2025  
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "Pixel format is %d, not JPEG"), config.pixel_format);
#endif    
  }
  //;
  //esp_log_level_set("*", ESP_LOG_INFO);

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.

#ifdef DEBUG_DRIVERS__CAMERA_2025  
  ALOG_DBG("CAM: get psram");
#endif

AddLog(LOG_LEVEL_INFO, "PSRAM: Found=%d Useable=%d", 
  SupportESP32::FoundPSRAM(), 
  SupportESP32::UsePSRAM());
  AddLog(LOG_LEVEL_INFO, "Heap free: %d, PSRAM free: %d", 
    ESP.getFreeHeap(), 
    ESP.getFreePsram());



  Wc.psram = SupportESP32::UsePSRAM();
  if (Wc.psram) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
#ifdef DEBUG_DRIVERS__CAMERA_2025  
    ALOG_DBG(PSTR(D_LOG_CAMERA "PSRAM found"));
#endif
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_DRAM;
    ALOG_DBG(PSTR(D_LOG_CAMERA "PSRAM not found"));
  }


  esp_err_t err;
  // cannot hurt to retry...
  for (int i = 0; i < 3; i++){
    err = esp_camera_init(&config);
    if (err != ESP_OK) {
      AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "InitErr 0x%x try %d"), err, (i+1));
      esp_camera_deinit();
      if (err == 0x105 && (config.pin_pwdn >= 0)){
        // try a longer power off... and retry
        // power off for 500ms
        gpio_set_level((gpio_num_t)config.pin_pwdn, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }
    } else {
      if (i){
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "InitOK try %d"), (i+1));
      }
      break;
    }
  }

  if (err != ESP_OK) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "InitErr 0x%x"), err);
    Wc.lastCamError = err;
    return 0;
  }

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "heap check 2: %d"),ESP.getFreeHeap());

  // drop down frame size for higher initial frame rate
  sensor_t * wc_s = esp_camera_sensor_get();
  // seems config.frame_size has no effect?
  // so this is where we set framesize....
  wc_s->set_framesize(wc_s, (framesize_t)fsiz);

  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "Init failed !frame on time"));
    Wc.lastCamError = 2;
    return 0;
  }
  Wc.width = wc_fb->width;
  Wc.height = wc_fb->height;
  esp_camera_fb_return(wc_fb);

  WcApplySettings();

  camera_sensor_info_t *info = esp_camera_sensor_get_info(&wc_s->id);

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "%s Initialized"), info->name);
  tkr_set->runtime.camera_initialized = true;
  Wc.up = 1;
  if (Wc.psram) { Wc.up = 2; }

  Wc.frameIntervalsus = (uint32_t)(((float)nativeIntervals20ms[fsiz]/((float)tkr_set->Settings.webcam_clk/20.0))*1000.0);
  WcStats.maxfps = (uint32_t)((float)1000000.0/(float)Wc.frameIntervalsus);

  Wc.lastCamError = ESP_OK;
  return Wc.up;
}


void mCamera::WcRemoveDeadCients(){
  // iterate over clients removing dead ones
  mCamera::wc_client *client = Wc.client_p;
  mCamera::wc_client **prev = &Wc.client_p;
  while(client){
    if (!client->active){
      // just in case...
      mSupport::TasAutoMutex localmutex(&WebcamMutex, "WcLoop", 200);
      *prev = client->p_next;
      client->client.stop();
      mCamera::wc_client *next = client->p_next;
      delete client;
      client = next;
    } else {
      prev = &client->p_next;
      client = client->p_next;
    }
  }
}


// kill all http streaming clients
void mCamera::WcEndStream(){
  // we should use a mutext here, in case we are currently sending
  mSupport::TasAutoMutex localmutex(&WebcamMutex, "WcLoop2", 20000);
  // if http streaming is active
  mCamera::wc_client *client = Wc.client_p;
  // iterate over clients
  while(client){
    client->active = 0;
    if (client->client){
      client->client.stop();
    }
    client = client->p_next;
  }
  WcRemoveDeadCients();
}

// deinit and power down camera
void mCamera::WcCamOff() {
  mSupport::TasAutoMutex localmutex(&WebcamMutex, "WcCamOff", 30000);
  // deinit camera
  WcSetup(-1);
  // kill any existing clients
  WcEndStream();
#ifdef ENABLE_RTSPSERVER
  // kill any existing rtsp clients
  WcEndRTSP();
#endif
  int pin_pwdn = -1;
  if (tkr_pins->PinUsed(GPIO_WEBCAM_PWDN)){
    pin_pwdn = tkr_pins->Pin(GPIO_WEBCAM_PWDN);
  }
/*   else {
    pin_pwdn = (gpio_num_t)Pin(PWDN_GPIO_NUM);
  }*/
  if (pin_pwdn >= 0){
    gpio_config_t conf = { 0 };
    conf.pin_bit_mask = 1LL << pin_pwdn;
    conf.mode = GPIO_MODE_OUTPUT;
    gpio_config(&conf);

    // careful, logic is inverted compared to reset pin
    gpio_set_level((gpio_num_t)pin_pwdn, 1);
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
  Wc.lastCamError = 0x3;
}

/*********************************************************************************************/

int32_t mCamera::WcSetOptions(uint32_t sel, int32_t value) {
  int32_t res = 0;
  mSupport::TasAutoMutex localmutex(&WebcamMutex, "WcSetOptions", 200);

  // don;t want sensor for this one.
  switch (sel) {
    case 24:
      // frames to ignore between reads
      if (value >= 0) { Wc.skipFrames = value; }
      return value;
      break;
    case 25:
      // pixelformat - native formats + 1, 0->jpeg
      if (value >= 0) { Wc.camPixelFormat = value; }
      if (Wc.up){
        WcSetup(tkr_set->Settings.webcam_config.resolution);
      }
      return value;
      break;
  }

  sensor_t *s = esp_camera_sensor_get();
  if (!s) { return -99; }

  switch (sel) {
    case 0:
      if (value >= 0) { s->set_framesize(s, (framesize_t)value); }
      res = s->status.framesize;
      Wc.width = 0;
      Wc.height = 0;
      Wc.last_frame_len = 0;
      Wc.frameIntervalsus = (uint32_t)(((float)nativeIntervals20ms[value]/((float)tkr_set->Settings.webcam_clk/20.0))*1000.0);
      WcStats.maxfps = (uint32_t)((float)1000000.0/(float)Wc.frameIntervalsus);

      // WcFeature is lost on resolution change
      WcApplySettings();
      break;
    case 1:
      if (value >= 0) { s->set_special_effect(s, value); }
      res = s->status.special_effect;
      break;
    case 2:
      if (value >= 0) { s->set_vflip(s, value); }
      res = s->status.vflip;
      break;
    case 3:
      if (value >= 0) { s->set_hmirror(s, value); }
      res = s->status.hmirror;
      break;
    case 4:
      if (value >= -4) { s->set_contrast(s, value); }
      res = s->status.contrast;
      break;
    case 5:
      if (value >= -4) { s->set_brightness(s, value); }
      res = s->status.brightness;
      break;
    case 6:
      if (value >= -4) { s->set_saturation(s,value); }
      res = s->status.saturation;
      break;
    case 7:
      if (value >= 0) { s->set_whitebal(s, value); }
      res = s->status.awb;
      break;
    case 8:
      if (value >= 0) { s->set_wb_mode(s, value); }
      res = s->status.wb_mode;
      break;
    case 9:
      if (value >= 0) { s->set_awb_gain(s, value); }
      res = s->status.awb_gain;
      break;
    case 10:
      if (value >= 0) { s->set_exposure_ctrl(s, value); }
      res = s->status.aec;
      break;
    case 11:
      if (value >= 0) { s->set_aec_value(s, value); }
      res = s->status.aec_value;
      break;
    case 12:
      if (value >= 0) { s->set_ae_level(s, value); }
      res = s->status.ae_level;
      break;
    case 13:
      if (value >= 0) { s->set_aec2(s, value); }
      res = s->status.aec2;
      break;
    case 14:
      if (value >= 0) { s->set_gain_ctrl(s, value); }
      res = s->status.agc;
      break;
    case 15:
      if (value >= 0) { s->set_agc_gain(s, value); }
      res = s->status.agc_gain;
      break;
    case 16:
      if (value >= 0) { s->set_gainceiling(s, (gainceiling_t)value); }
      res = s->status.gainceiling;
      break;
    case 17:
      if (value >= 0) { s->set_raw_gma(s, value); }
      res = s->status.raw_gma;
      break;
    case 18:
      if (value >= 0) { s->set_lenc(s, value); }
      res = s->status.lenc;
      break;
    case 19:
      if (value >= 0) { s->set_wpc(s, value); }
      res = s->status.wpc;
      break;
    case 20:
      if (value >= 0) { s->set_dcw(s, value); }
      res = s->status.dcw;
      break;
    case 21:
      // blackpixelcontrol
      if (value >= 0) { s->set_bpc(s, value); }
      res = s->status.bpc;
      break;
    case 22:
      if (value >= 0) { s->set_colorbar(s, value); }
      res = s->status.colorbar;
      break;
    case 23:
      if (value >= 0) { WcFeature(value); }
      break;
  }

  return res;
}

// wait for a frame to be read, if thread is running
void mCamera::WcWaitFrame(int maxtime_ms) {
  if (!Wc.taskRunning) return;
  // force a wait for a read
  WcWaitZero(&Wc.taskTakePic, -1, maxtime_ms);
}

uint32_t mCamera::WcGetWidth(void) {
  if (Wc.taskRunning){
    if (!Wc.width){
      WcWaitFrame(1000);
    }
  }
  return Wc.width;
}

uint32_t mCamera::WcGetHeight(void) {
  if (Wc.taskRunning){
    if (!Wc.width){
      WcWaitFrame(1000);
    }
  }
  return Wc.width;
}

/*********************************************************************************************/


uint32_t mCamera::WcGetPicstore(int32_t num, uint8_t **buff) {
  if (num<0) { return MAX_PICSTORE; }
  *buff = Wc.picstore[num].buff;
  return Wc.picstore[num].len;
}
uint32_t mCamera::WcGetPicstorePtr(int32_t num, struct mCamera::PICSTORE **p) {
  if (num < 0) { 
    *p = nullptr;
    return MAX_PICSTORE; 
  }
  if (num >= MAX_PICSTORE) {
    *p = nullptr;
    return MAX_PICSTORE; 
  } 
  *p = &Wc.picstore[num];
  return Wc.picstore[num].len;
}


uint32_t mCamera::WcGetFrame(int32_t bnum) {
  if (bnum < 0) {
    if (bnum < -MAX_PICSTORE) { bnum=-1; }
    bnum = -bnum;
    // check before decrement
    if (bnum == Wc.lastBnum){
      Wc.lastBnum = 0;
    }
    bnum--;
    mSupport::TasAutoMutex localmutex(&WebcamMutex, "WcGetFrame", 200);
    if (Wc.picstore[bnum].buff) { free(Wc.picstore[bnum].buff); }
    memset(&Wc.picstore[bnum], 0, sizeof(Wc.picstore[bnum]));
    Wc.picstore[bnum].len = 0;
    return 0;
  }

  if (!bnum) {    
    // used to be used to trigger read a frame to get width/height
    // can still get here via scripts
    return 0;
  }

  if (Wc.taskRunning){
    WcWaitZero(&Wc.taskGetFrame, bnum, 1000);
    return Wc.picstore[bnum - 1].len;
  }
  return 0;
}




//////////////// Handle authentication /////////////////


bool mCamera::WebcamAuthenticate(void)
{
  // if (strlen(SettingsText(SET_WEBPWD)) && (HTTP_MANAGER_RESET_ONLY != Web.state)) {
  //   return Wc.CamServer->authenticate(WEB_USERNAME, SettingsText(SET_WEBPWD));
  // } else {
    return true;
  // }
}

bool mCamera::WebcamCheckPriviledgedAccess(bool autorequestauth)
{
  // if(tkr_set->Settings.webcam_config2.auth == 0){
  //   return true;
  // }
  // if (autorequestauth && !WebcamAuthenticate()) {
  //   Wc.CamServer->requestAuthentication();
  //   return false;
  // }
  return true;
}

///////////////////////////////////////////////////

void mCamera::HandleImage(void) {
  // if (!HttpCheckPriviledgedAccess()) { return; }

  uint32_t bnum = Webserver->arg(F("p")).toInt();
  if ((bnum < 0) || (bnum > MAX_PICSTORE)) { bnum= 1; }
  int format = (int)PIXFORMAT_JPEG;

  if (bnum){
    // no picture present at this index
    if (!Wc.picstore[bnum-1].buff){
      Webserver->send(404,"",""); 
      return;
    } 
    format = (int)Wc.picstore[bnum-1].format;
  }

  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-disposition: inline; filename=cap.";
  if(format == (int)PIXFORMAT_JPEG){
    response += "jpg\r\nContent-type: image/jpeg\r\n";
  } else {
    response += "bin\r\nContent-type: image/x-tas-binary\r\n";
  }
  response += "Content-Length: ";
  char tmp[12];

  if (!bnum) {
    if (Wc.taskRunning == 1){
      WcWaitZero(&Wc.taskTakePic, 1, 1000);
      mSupport::TasAutoMutex localmutex(&WebcamMutex, "HandleImage", 200);
      if (Wc.snapshotStore.len) {
        response += itoa(Wc.snapshotStore.len, tmp, 10);
        response += "\r\n\r\n";

        WiFiClient client = Webserver->client();
        Webserver->sendContent(response);
        client.write((char *)Wc.snapshotStore.buff, Wc.snapshotStore.len);
        client.stop();
        pic_free(&Wc.snapshotStore);
      } else {
        Webserver->send(404,"",""); 
        ALOG_DBG(PSTR(D_LOG_CAMERA "No img #: %d"), bnum);
        return;
      }
    } else {
      Webserver->send(404,"",""); 
      return;
    }
  } else {
    mSupport::TasAutoMutex localmutex(&WebcamMutex, "HandleImage", 200);
    bnum--;
    if (!Wc.picstore[bnum].len) {
      Webserver->send(404,"",""); 
      ALOG_DBG(PSTR(D_LOG_CAMERA "No img #: %d"), bnum);
      return;
    }
    response += itoa(Wc.picstore[bnum].len, tmp, 10);
    response += "\r\n\r\n";
    WiFiClient client = Webserver->client();
    Webserver->sendContent(response);
    client.write((char *)Wc.picstore[bnum].buff, Wc.picstore[bnum].len);
    client.stop();
  }

#ifdef DEBUG_DRIVERS__CAMERA_2025  
  AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_CAMERA "Sending image #: %d"), bnum+1);
#endif
}

void mCamera::HandleImageAny(struct mCamera::PICSTORE *ps){
  // if (!HttpCheckPriviledgedAccess()) { return; }

  if (!ps || !ps->buff) {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_CAMERA "img pres"));
    Webserver->send(404,"",""); 
    return;
  }
  // convert mono to rgb8

  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  // use a malloc that we don't free to save memory creep
  // it is re-mallcoed if the frame does not fit.
  bool conv = false;

  // allocate a new picture every time to avoid holding up task thread by using OurOneJpeg
  struct mCamera::PICSTORE psout = {0};

  if (ps->format != PIXFORMAT_JPEG) {
#ifdef USE_WEBCAM_MOTION
    ALOG_DBG(PSTR(D_LOG_CAMERA "image will be encoded from %d"), ps->format);

    mSupport::TasAutoMutex localmutex(&WebcamMutex, "HandleImagemotion", 2000);
    // use a malloc that we don't free to save memory creep
    // it is re-mallcoed if the frame does not fit.
    ALOG_DBG(PSTR(D_LOG_CAMERA "%x %d %d %d"), ps->buff, ps->len, ps->width, ps->height);
    conv = WcencodeToJpeg(ps->buff, ps->len, ps->width, ps->height, (int)ps->format, 80, &psout);
    if (!conv) ALOG_DBG(PSTR(D_LOG_CAMERA "image encode failed"));

    _jpg_buf_len = psout.len;
    _jpg_buf = psout.buff;
    ALOG_DBG(PSTR(D_LOG_CAMERA "encoded %X %d (%dx%d) -> %X %d (%d)"),
      ps->buff, ps->len, ps->width, ps->height,
      _jpg_buf, _jpg_buf_len,
      psout.allocatedLen
    );
#endif    
  } else {
    _jpg_buf_len = ps->len;
    _jpg_buf = ps->buff;
#ifdef DEBUG_DRIVERS__CAMERA_2025  
    ALOG_DBG(PSTR(D_LOG_CAMERA "already jpeg %X %d (%dx%d) (%d)"),
      ps->buff, ps->len, ps->width, ps->height,
      ps->allocatedLen
    );
#endif      
  }

  if (_jpg_buf_len){
    WiFiClient client = Webserver->client();
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-disposition: inline; filename=cap.jpg\r\n";
    response += "Content-type: image/jpeg\r\n\r\n";
    Webserver->sendContent(response);
    client.write((char *)_jpg_buf, _jpg_buf_len);
    client.stop();
    if (conv){
      pic_free(&psout);
    }
    return; // don't send 500
  }

#ifdef DEBUG_DRIVERS__CAMERA_2025  
  ALOG_DBG(PSTR(D_LOG_CAMERA "image could not be encoded"));
#endif
  if (conv){
    pic_free(&psout);
  }
  Webserver->send(500,"",""); 
  return;
}



void mCamera::HandleWebcamMjpegFn(int type) {
  if(!WebcamCheckPriviledgedAccess()){
    Wc.CamServer->send(403,"","");
    return;
  }
  mSupport::TasAutoMutex localmutex(&WebcamMutex, "HandleWebcamMjpeg", 200);
  mCamera::wc_client *client = new mCamera::wc_client;
  client->active = 1;
  client->type = type;
  client->p_next = Wc.client_p;
  client->client = Wc.CamServer->client();
  Wc.client_p = client;
#ifdef DEBUG_DRIVERS__CAMERA_2025  
  ALOG_DBG(PSTR(D_LOG_CAMERA "Create client"));
#endif
}

void mCamera::HandleWebcamMjpeg(void) {
  tkr_camera->HandleWebcamMjpegFn(0);
}

void mCamera::HandleWebcamMjpegDiff(void) {
  tkr_camera->HandleWebcamMjpegFn(1);
}

void mCamera::HandleWebcamRoot(void) {
  if(!tkr_camera->WebcamCheckPriviledgedAccess()){
    tkr_camera->Wc.CamServer->send(403,"","");
    return;
  }
  //CamServer->redirect("http://" + String(ip) + ":81/cam.mjpeg");
  tkr_camera->Wc.CamServer->sendHeader("Location", "/cam.mjpeg");
  tkr_camera->Wc.CamServer->send(302, "", "");
#ifdef DEBUG_DRIVERS__CAMERA_2025  
  ALOG_DBG(PSTR(D_LOG_CAMERA "Root called"));
#endif
}

/*********************************************************************************************/

uint32_t mCamera::WcSetStreamserver(uint32_t flag) {
  ALOG_INF(PSTR("=========================CAM: WcSetStreamserver %d"), flag);
  if (tkr_set->runtime.global_state.network_down) { 
    ALOG_INF(PSTR("=========================CAM: Network down, cannot set stream server"));
    WcEndStream();
    return 0; 
  }
#ifdef DEBUG_DRIVERS__CAMERA_2025  
  AddLog(LOG_LEVEL_INFO, PSTR("WcSetStreamserver %d"), flag);
#endif
  if (flag) {
    if (!Wc.CamServer) {
      ALOG_INF(PSTR("=========================CAM: if (!Wc.CamServer) {"));
      
    Webserver = new ESP8266WebServer(82);
    // Webserver->on("/cam.jpg", HandleImage);
    Webserver->begin();
      ALOG_DBG(PSTR("WebServer started on port 82"));


      mSupport::TasAutoMutex localmutex(&WebcamMutex, "HandleWebcamMjpeg", 20000);
      Wc.CamServer = new ESP8266WebServer(81);


      Wc.CamServer->on("/", HandleWebcamRoot);


      Wc.CamServer->on("/diff.mjpeg", HandleWebcamMjpegDiff);
      Wc.CamServer->on("/cam.mjpeg", HandleWebcamMjpeg);
      Wc.CamServer->on("/cam.jpg", HandleWebcamMjpeg);
      Wc.CamServer->on("/stream", HandleWebcamMjpeg);


      ALOG_DBG(PSTR(D_LOG_CAMERA "Strm init"));
      Wc.CamServer->begin();
    }else{
      
      ALOG_INF(PSTR("=========================CAM: else (!Wc.CamServer) {"));
    }
  } else {
    ALOG_INF(PSTR("=========================CAM: } else {"));
    if (Wc.CamServer) {
      ALOG_INF(PSTR("=========================CAM: } else {if (Wc.CamServer) {"));
      mSupport::TasAutoMutex localmutex(&WebcamMutex, "HandleWebcamMjpeg", 20000);
      WcEndStream();
      Wc.CamServer->stop();
      delete Wc.CamServer;
      Wc.CamServer = NULL;
      ALOG_DBG(PSTR(D_LOG_CAMERA "Strm exit"));
    }
  }
  return 0;
}


void mCamera::WCStartOperationTask()
{

  if (Wc.taskRunning == 0)
  {

    ALOG_DBG(PSTR(D_LOG_CAMERA "Start operations"));

    xTaskCreatePinnedToCore(
      mCamera::WCOperationTask,    /* Function to implement the task */
      "WCOperationTask",  /* Name of the task */
      8192,               /* Stack size in bytes */
      NULL,               /* Task input parameter */
      0,                  /* Priority of the task */
      &Wc.taskHandle,     /* Task handle. */
      #ifdef CONFIG_FREERTOS_UNICORE
        0                 /* Core where the task should run */
      #else
        WEBCAM_CORE       /* Core where the task should run */
      #endif
    );

    // wait for task to start
    int loops = 10;
    while(!Wc.taskRunning && loops--){
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
  }

}


// this IS as task.
// used to disconnect webcam frame read from tas loop
void mCamera::WCOperationTask(void *pvParameters){
  unsigned long loopcount = 0;

#ifdef DEBUG_DRIVERS__CAMERA_2025  
  ALOG_DBG(PSTR(D_LOG_CAMERA "WCOperationTask: Start task"));
#endif
  int framecount = 0;
  unsigned long laststatmillis = millis();
  bool jpeg_converted = false;
  uint32_t processing_total = 0;
  uint64_t last_camtime = 0;
  tkr_camera->Wc.taskRunning = 1;
  tkr_camera->WcStats.framesTotal = 0;
  tkr_camera->WcStats.framesLost = 0;

  int32_t skipsWanted = tkr_camera->Wc.skipFrames;


  // we set to tkr_camera->Wc.taskRunning 2 to stop the task
  while (tkr_camera->Wc.taskRunning == 1){
    loopcount++;
    uint32_t thismillis = millis();
    uint32_t statdur = thismillis - laststatmillis;

    // storage and settings disable cam.
    // we must stall until re-enabled
    // it asserts the mutex whilst stopping the cam, so it can't stop it mid-processing
    tkr_camera->WcWaitEnable();
    // if camera is configured and working
    { // closure for auto mutex
      // note that this mutex can block the loop for a long time - 
      // e.g. if motion detect on a full big frame, up to a second
      mSupport::TasAutoMutex localmutex(&WebcamMutex, "WebcamMjpeg", 30000);

      // only do anything if cam us up, and cam has not remained stopped
      if (tkr_camera->Wc.up && !tkr_camera->Wc.disable_cam) {
        size_t _jpg_buf_len = 0;
        uint8_t * _jpg_buf = NULL;

        // read a frame buffer pointer.  this will block until a frame is available
        camera_fb_t *wc_fb = esp_camera_fb_get();

        if (!wc_fb) {
          // add framecount so we show this right away if we were showing frames.
          if (!(loopcount % 100) || (statdur > 5000) || framecount){
            ALOG_DBG(PSTR(D_LOG_CAMERA "Frame fail")); 
            laststatmillis = thismillis;
            framecount = 0;
          }
          tkr_camera->WcStats.camfail++;
          // if camera up, but just can't get a frame right now
          vTaskDelay(10/ portTICK_PERIOD_MS);
        } else {
          framecount++;
          uint32_t now = millis();
          uint64_t camtime = wc_fb->timestamp.tv_sec;
          camtime = camtime * 1000000;
          camtime += wc_fb->timestamp.tv_usec;

          if (!skipsWanted) skipsWanted = tkr_camera->Wc.skipFrames;

          if (last_camtime){
            uint32_t camdiff = camtime - last_camtime;
            // this is an accurate indication of framerate, but also frame skip
            tkr_camera->Wc.camtimediff = camdiff;
            int framesPassed = round(((float)camdiff)/(float)(tkr_camera->Wc.frameIntervalsus));
            if (framesPassed){
              tkr_camera->WcStats.framesTotal += framesPassed;
              tkr_camera->WcStats.framesLost += (framesPassed - 1);
              if (skipsWanted > 0){
                skipsWanted -= (framesPassed - 1);
              }
              if (skipsWanted > 0) skipsWanted --;
            } else {
#ifdef DEBUG_DRIVERS__CAMERA_2025  
              ALOG_DBG(PSTR(D_LOG_CAMERA "Duplicate time in frame? diff %d intv %d"), camdiff, tkr_camera->Wc.frameIntervalsus);
#endif              
            }
          }
          last_camtime = camtime;

          // every 100 frames or 5s
          if (!(loopcount % 100) || (statdur > 5000)){
            float framespersec = ((float)framecount)/(((float)(thismillis - laststatmillis))/1000.0);
            ALOG_DBG(PSTR(D_LOG_CAMERA "avFPS %f %s FS:%d(%d) f:%u s:%u"), 
              framespersec, 
              jpeg_converted?"raw":"jpg", 
              wc_fb->len, 
              tkr_camera->Wc.lenDiff,
              tkr_camera->WcStats.framesTotal,
              tkr_camera->WcStats.framesLost
              );
            tkr_camera->WcStats.avgProcessingPerFrameMS = processing_total/framecount;
            tkr_camera->WcStats.avgFPS = framespersec;
            tkr_camera->WcStats.avgFrameMS = (uint8_t) (1000/framespersec);
            processing_total = 0;
            laststatmillis = thismillis;
            framecount = 0;
          }

          // we got a frame
          tkr_camera->Wc.width = wc_fb->width;
          tkr_camera->Wc.height = wc_fb->height;
          tkr_camera->WcStats.camcnt++;

          // if we want to process this frame.
          // skipsWanted is the counter used for tkr_camera->Wc.skipFrames use
          if (skipsWanted <= 0) skipsWanted = 0;
          if (!skipsWanted){
#ifdef USE_WEBCAM_MOTION
            int detectMotion = 0;
            // if we want simple mootion detect,
            if (wc_motion.motion_detect){
              if ((now - wc_motion.motion_ltime) > wc_motion.motion_detect) {
                wc_motion.motion_ltime = now;
                detectMotion = 1;
              }
            }
#endif
            // if we need a frame for web stream or rtsp
            if(
              tkr_camera->Wc.client_p // pointer to first 
              #ifdef ENABLE_RTSPSERVER
              || tkr_camera->Wc.rtspclient // pointer to first
              #endif
              || tkr_camera->Wc.taskGetFrame // get one frame - from scripts
              || tkr_camera->Wc.taskTakePic // get one frame - from scripts
              #ifdef USE_WEBCAM_MOTION
              || detectMotion 
              #endif                
            ){

              // most cameras will supply as jpeg? it's what we ask for...
              jpeg_converted = false;
              bool free_jpeg = false;
              /* 2023-05-28 - AiThinker type cam module marked DM.
                Note: I tried everything to get pixels ina different form, but faiiled.
                So this code has NEVER run for me in anger directly off cam data.
              */

              if (wc_fb->format != PIXFORMAT_JPEG) 
              {
                #ifdef USE_WEBCAM_MOTION
                // note - don't free the jpeg, we re-use it.
                jpeg_converted = tkr_camera->WcencodeToJpeg(wc_fb->buf, wc_fb->len, wc_fb->width, wc_fb->height, (int)wc_fb->format, 80, &VideoJpeg);
                _jpg_buf_len = VideoJpeg.len;
                _jpg_buf = VideoJpeg.buff;

                // this function is incredibly expensive - always allocates 128kbytes
                //jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
                // free_jpeg = true; // if using frame2jpg, we must free
                #endif                
                if (!jpeg_converted)
                {
                  #ifdef DEBUG_DRIVERS__CAMERA_2025  
                  ALOG_DBG(PSTR(D_LOG_CAMERA "JPEG compression failed"));
                  #endif                  
                  tkr_camera->WcStats.jpegfail++;
                }
              } else {
                _jpg_buf_len = wc_fb->len;
                _jpg_buf = wc_fb->buf;
              }

              if (_jpg_buf)
              {
                int jpeg_len_diff = abs((int)tkr_camera->Wc.last_frame_len - (int)_jpg_buf_len);
                int jpeg_len_diff_percent = 100*jpeg_len_diff/_jpg_buf_len;
                tkr_camera->Wc.lenDiff = jpeg_len_diff_percent;
                if (tkr_camera->Wc.lenDiffLimit && (tkr_camera->Wc.lenDiffLimit < jpeg_len_diff_percent))
                {
                  tkr_camera->Wc.lenDiffTrigger = 1;
                  tkr_camera->Wc.lenDiffTriggered = jpeg_len_diff_percent;
                }

                tkr_camera->Wc.last_frame_len = _jpg_buf_len;
                // we use -1 to just detect that tkr_camera->Wc.width/height should have been set
                if (tkr_camera->Wc.taskTakePic)
                {
                  if (tkr_camera->Wc.taskTakePic > 0)
                  {
                    if (tkr_camera->pic_alloc(&tkr_camera->Wc.snapshotStore, 0, 0, _jpg_buf_len, PIXFORMAT_JPEG, 1))
                    {
                      memcpy(tkr_camera->Wc.snapshotStore.buff, _jpg_buf, _jpg_buf_len);
                    }
                  }
                  tkr_camera->Wc.taskTakePic = 0;
                }

                // request to store the frame into picstore
                if (tkr_camera->Wc.taskGetFrame)
                {
                  int bnum = tkr_camera->Wc.taskGetFrame;
                  if ((bnum < 1) || (bnum > MAX_PICSTORE)) { bnum = 1; }
                  bnum--;
                  if (tkr_camera->Wc.picstore[bnum].buff) { free(tkr_camera->Wc.picstore[bnum].buff); }
                  memset(&tkr_camera->Wc.picstore[bnum], 0, sizeof(tkr_camera->Wc.picstore[bnum]));
                  tkr_camera->Wc.picstore[bnum].buff = (uint8_t *)heap_caps_aligned_alloc(4, _jpg_buf_len+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
                  if (tkr_camera->Wc.picstore[bnum].buff) {
                    memcpy(tkr_camera->Wc.picstore[bnum].buff, _jpg_buf, _jpg_buf_len);
                    tkr_camera->Wc.picstore[bnum].len = _jpg_buf_len;
                    tkr_camera->Wc.lastBnum = bnum + 1;
                    tkr_camera->Wc.picstore[bnum].format = (int8_t)PIXFORMAT_JPEG;
                    tkr_camera->Wc.picstore[bnum].width = tkr_camera->Wc.width;
                    tkr_camera->Wc.picstore[bnum].height = tkr_camera->Wc.height;

                    #ifdef DEBUG_DRIVERS__CAMERA_2025  
                    ALOG_DBG(PSTR(D_LOG_CAMERA "Got frame %d"), tkr_camera->Wc.lastBnum);
                    #endif                    
                  } 
                  else 
                  {
                    #ifdef DEBUG_DRIVERS__CAMERA_2025  
                    ALOG_DBG(PSTR(D_LOG_CAMERA "Can't allocate picstore"));
                    #endif                    
                  }
                  tkr_camera->Wc.taskGetFrame = 0;
                }

                #ifdef USE_WEBCAM_MOTION
                // if motion detect triggered by timer
                if (detectMotion){
                  tkr_camera->WcDetectMotionFn(_jpg_buf, _jpg_buf_len);
                }
                #endif

                // if http streaming is active, we will have one or more clients
                mCamera::wc_client *client = tkr_camera->Wc.client_p;
                // iterate over clients
                uint8_t webclientcount = 0;

                #ifdef USE_WEBCAM_MOTION
                size_t diff_jpg_buf_len = 0;
                uint8_t *diff_jpg_buf = NULL;
                #endif

                while(client)
                {
                  if (client->active)
                  {
                    uint32_t client_start = millis();

                    if (!client->client.connected())
                    {
                      #ifdef DEBUG_DRIVERS__CAMERA_2025  
                      ALOG_DBG(PSTR(D_LOG_CAMERA "Client fail"));
                      #endif                      
                      client->active = 0;
                      tkr_camera->WcStats.clientfail++;
                    }
                    if (1 == client->active) {
                      client->client.flush();
                      client->client.setTimeout(3);

                      #ifdef DEBUG_DRIVERS__CAMERA_2025  
                      ALOG_DBG(PSTR(D_LOG_CAMERA "Start stream"));
                      #endif       

                      client->client.print("HTTP/1.1 200 OK\r\n"
                        "Content-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n"
                        "\r\n");
                      client->active = 2;
                    }

                    if (2 == client->active) 
                    {
                      int type = client->type;
                      uint8_t *src = _jpg_buf;
                      int len = _jpg_buf_len;

                      #ifdef USE_WEBCAM_MOTION
                      // if this client wants motion images
                      if (type == 1){
                        // if we already coded to jpeg for another client
                        if (diff_jpg_buf){
                          src = diff_jpg_buf;
                          len = diff_jpg_buf_len;
                        } else {
                          src = nullptr;
                          // only for frames where motion is being detected,
                          // e.g. once per sec
                          if (detectMotion){
                            struct PICSTORE *ps = nullptr;
                            // by default, we send the diff buffer if present
                            if (wc_motion.diff && wc_motion.diff->buff){
                              ps = wc_motion.diff;
                            }
                            if (!ps){
                              // else we send the motion buffer (mono of video, but slower)
                              if (wc_motion.last_motion && wc_motion.last_motion->buff){
                                ps = wc_motion.last_motion;
                              }
                            }
                            if (ps){
                              // use a malloc that we don't free to save memory creep
                              // it is re-mallcoed if the frame does not fit.
                              bool conv = tkr_camera->WcencodeToJpeg(ps->buff, ps->len, ps->width, ps->height, (int)ps->format, 80, &OurOneJpeg);

                              diff_jpg_buf_len = OurOneJpeg.len;
                              diff_jpg_buf = OurOneJpeg.buff;

                              if (conv && diff_jpg_buf_len){
                                src = diff_jpg_buf;
                                len = diff_jpg_buf_len;
                              }
                            }
                          }
                        }
                      }
                      #endif                      

                      if (src){
                        client->client.printf(
                          "--" BOUNDARY "\r\n"
                          "Content-Type: image/jpeg\r\n"
                          "Content-Length: %d\r\n"
                          "\r\n", static_cast<int>(len));
                        uint32_t tlen = client->client.write(src, len);
                        client->client.print("\r\n");
                      }
                    }
                    uint32_t client_end = millis();
                    // if it took more than 20s to send to the client, then kill it.
                    // this was observed on wifi rescan
                    if (client_end - client_start > 20000){
                      ALOG_DBG(PSTR(D_LOG_CAMERA "Cl timeout on send"));
                      tkr_camera->WcStats.clientfail++;
                      client->client.stop();
                      client->active = 0;
                    }
                  }
                  client = client->p_next;
                  webclientcount++;
                }
                // if we allocates a jppeg image for motion, free now
                // NO!!! This is now a long-lived buffer!!!
                //if (diff_jpg_buf) free(diff_jpg_buf);
                tkr_camera->WcStats.webclientcount = webclientcount;

                #ifdef ENABLE_RTSPSERVER
                // if rtsp is active, we will have one or more clients
                volatile wc_rtspclient *rtspclient = tkr_camera->Wc.rtspclient;
                uint8_t rtspclientcount = 0;
                while (rtspclient) {
                  if (rtspclient->camStreamer && rtspclient->rtsp_session){
                    rtspclient->camStreamer->setframe(_jpg_buf, _jpg_buf_len);
                    rtspclient->rtsp_session->broadcastCurrentFrame(now);
                    rtspclient->camStreamer->clearframe();
                  }
                  rtspclient = rtspclient->p_next;
                  rtspclientcount++;
                }
                tkr_camera->WcStats.rtspclientcount = rtspclientcount;
                #endif

                // free jpeg frame if we created it
                if (free_jpeg) { free(_jpg_buf); }

                // could trigger info to berry every fraem, if configured
                tkr_camera->Wc.frame_processed = 1;

                uint32_t processingend = millis();
                uint32_t processingdur = processingend - now;
                processing_total += processingdur;
              } else {
                tkr_camera->Wc.last_frame_len = 0;            
              }
            }
          }

          // free resources and return frame buffer for re-use AFTER we have used the data
          esp_camera_fb_return(wc_fb);
        }
      } else {
        if (tkr_camera->Wc.up){
          // if camera up, but just can't get a frame right now because
          // else we may conflict with files or settings store, delay just a shorter time
          vTaskDelay(10/ portTICK_PERIOD_MS);
        } else {
          // be sure to clear this if camera is not running, else we could wait forever
          // when asking for a frame
          tkr_camera->Wc.taskGetFrame = 0;
          // come around every 1/10s if not serving pictures
          vTaskDelay(100/ portTICK_PERIOD_MS);
        }
      }
    }
  }

  // this log sometimes causes guru mediation error. Maybe because 
  // temp storage is removed before it is serviced?
  ALOG_DBG(PSTR(D_LOG_CAMERA "Left task"));
  tkr_camera->Wc.taskRunning = 0;

  // wait 1/2 second for log to be done?
  vTaskDelay(500/ portTICK_PERIOD_MS);

  vTaskDelete( NULL );
}


void mCamera::WcLoop(void) 
{
  Wc.loopcounter++;
  { // closure for automutex
    // we don't need one here
    //TasAutoMutex localmutex(&WebcamMutex, "WcLoop", 200);

#ifdef USE_WEBCAM_MOTION
    // if wc_motion.motion_trigger > wc_motion.motion_triggerlimit
    // set wc_motion.motion_triggerlimit low if you want every time it's processed
    // NOTE: there is no 'retrigger hold off' time.
    if (wc_motion.motion_triggered){
#ifdef USE_BERRY
      char t[80];
      snprintf(t, (size_t)39, "{\"val\":%d,\"bri\":%d,\"pix\":%d}", wc_motion.motion_trigger, wc_motion.motion_brightness, wc_motion.changedPixelPertenthousand);
      callBerryEventDispatcher("webcam", "motion", 0, t, strlen(t));
#endif
#ifdef DEBUG_DRIVERS__CAMERA_2025  
      ALOG_DBG(PSTR(D_LOG_CAMERA "Motion Triggered"));
#endif      
      WcMotionLog();
      wc_motion.motion_triggered = 0;
    }
#endif    
    if (Wc.lenDiffTrigger){
#ifdef USE_BERRY
      char t[40];
      snprintf(t, (size_t)39, "{\"diff\":%d}", Wc.lenDiffTriggered);
      callBerryEventDispatcher("webcam", "framesizechange", 0, t, strlen(t));
#endif
#ifdef DEBUG_DRIVERS__CAMERA_2025  
      ALOG_DBG(PSTR(D_LOG_CAMERA "Framesize Change > %d = %d"), Wc.lenDiffLimit, Wc.lenDiffTriggered);
#endif      
      Wc.lenDiffTrigger = 0;
#ifdef USE_WEBCAM_MOTION
      WcMotionLog();
#endif      
    }

    if (Wc.frame_processed){
#ifdef USE_BERRY
      if (Wc.berryFrames){
        char t[80];
        snprintf(t, (size_t)39, "{\"len\":%d}", Wc.last_frame_len);
        callBerryEventDispatcher("webcam", "frame", 0, t, strlen(t));
      }
#endif
      Wc.frame_processed = 0;
    }
  } // end closure for automutex

  if (Wc.CamServer) {
    // pretty sure this would be safe
    //TasAutoMutex localmutex(&WebcamMutex, "WcLoop", 200);
    Wc.CamServer->handleClient();
    WcRemoveDeadCients();
  }

#ifdef ENABLE_RTSPSERVER
  if (tkr_set->Settings.webcam_config.rtsp){
    if (!TasmotaGlobal.global_state.wifi_down) {
      // pretty sure we don;t need the mutex here
      mSupport::TasAutoMutex localmutex(&WebcamMutex, "WcLoop2", 30000);
      if (!Wc.rtspp) {
        Wc.rtspp = new WiFiServer(8554);
        Wc.rtspp->begin();
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "RTSP init"));
      }

      wc_rtspclient * volatile rtspclient = Wc.rtspclient;
      wc_rtspclient * volatile *  prev = &Wc.rtspclient;
      uint8_t rtspclientcount = 0;
      while (rtspclient) {
        bool removed = false;
        if (rtspclient->rtsp_session){
          rtspclient->rtsp_session->handleRequests(0);
          // if a client has stopped, remove it.
          if (rtspclient->rtsp_session->m_stopped){
            delete rtspclient->rtsp_session;
            rtspclient->rtsp_session = nullptr;
            delete rtspclient->camStreamer;
            rtspclient->camStreamer = nullptr;
            //rtspclient->rtsp_client.stop();
            *prev = rtspclient->p_next;
            wc_rtspclient *next = rtspclient->p_next;
            delete rtspclient;
            rtspclient = next;
            AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "RTSP stop"));
            removed = true;
          }
        }
        if (!removed){
          rtspclientcount++;
          prev = &rtspclient->p_next;
          rtspclient = rtspclient->p_next;
        }
      }
      WcStats.rtspclientcount = rtspclientcount;

      // accept new rtsp clients
      WiFiClient rtsp_client = Wc.rtspp->accept();
      if (rtsp_client) {
        wc_rtspclient *client = new wc_rtspclient;
        client->p_next = Wc.rtspclient;
        client->rtsp_client = rtsp_client;
        client->camStreamer = new localOV2640Streamer(&client->rtsp_client, Wc.width, Wc.height);
        client->rtsp_session = new CRtspSession(&client->rtsp_client, client->camStreamer); // our threads RTSP session and state
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "RTSP str"));
        Wc.rtspclient = client;
        WcStats.rtspclientcount++;
      }
    } else {
      // rtsp not enabled
      if (Wc.rtspp){
        // if running, stop cleanly.
        // stop all clients
        WcEndRTSP();
        delete Wc.rtspp;
        Wc.rtspp = nullptr;
      }
    }
  }
#endif // ENABLE_RTSPSERVER
}

#ifdef ENABLE_RTSPSERVER

// kill all rtsp clients
void WcEndRTSP(){
  // we should use a mutext here, in case we are currently sending
  mSupport::TasAutoMutex localmutex(&WebcamMutex, "WcEndRTSP", 2000);
  wc_rtspclient * volatile rtspclient = Wc.rtspclient;
  wc_rtspclient * volatile * prev = &Wc.rtspclient;
  while (rtspclient) {
    delete rtspclient->rtsp_session;
    rtspclient->rtsp_session = nullptr;
    delete rtspclient->camStreamer;
    rtspclient->camStreamer = nullptr;
    //rtspclient->rtsp_client.stop();
    wc_rtspclient *next = rtspclient->p_next;
    delete rtspclient;
    rtspclient = next;
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "RTSP stop"));
  }
  Wc.rtspclient = nullptr;
  WcStats.rtspclientcount = 0;
}
#endif


void mCamera::WcPicSetup(void) {
  

tkr_web->server->on("/version", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", (String)PROJECT_VERSION);
});
// tkr_web->server->on(PSTR("/wc.jpg"), HTTP_ANY, [this](AsyncWebServerRequest *request){
//   this->HandleImage();
// });


//   WebServer_on(PSTR("/wc.jpg"), HandleImage);
//   WebServer_on(PSTR("/wc.mjpeg"), HandleImage);
//   WebServer_on(PSTR("/snapshot.jpg"), HandleImage);
// #ifdef USE_WEBCAM_MOTION
//   WebServer_on(PSTR("/motiondiff.jpg"), HandleImagemotiondiff);
//   WebServer_on(PSTR("/motionmask.jpg"), HandleImagemotionmask);
//   WebServer_on(PSTR("/motionbuff.jpg"), HandleImagemotionbuff);
//   WebServer_on(PSTR("/motionlbuff.jpg"), HandleImagemotionlbuff);
//   WebServer_on(PSTR("/motionbackgroundbuff.jpg"), HandleImagemotionbackgroundbuff);
// #endif  
// #endif
}


const char HTTP_WEBCAM_MENUVIDEOCONTROL[] PROGMEM = "<p></p><button onclick=\"fetch('/cs?c2=64&c1=%s').then(()=>{location.reload();});\" name>%s</button>";

void mCamera::WcShowStream(void) {
  // if streaming is enabled (1 or 2), start stream server
  if (tkr_set->Settings.webcam_config.stream) {
//    if (!Wc.CamServer || !Wc.up) {
    if (!Wc.CamServer) {
      WcSetStreamserver(tkr_set->Settings.webcam_config.stream);
    }
  }

  // if (!Wc.CamServer){
  //   WSContentSend_P(PSTR("<p></p><center>Cam Server Not Running'</center><p></p>"));
  //   WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcstream2", "Turn On Streaming");
  // } else {
  //   if (!Wc.up){
  //     WSContentSend_P(PSTR("<p></p><center>Cam Not Running Err 0x%x</center><p></p>"), Wc.lastCamError);
  //     WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcinit", "Try WCINIT");
  //   } else {
  //     if (tkr_set->Settings.webcam_config.spare15) {
  //       WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcmenuvideodisable%200", "Turn On Video");
  //     }
  //   }
  // }

  // // spare15 is 'hide cam on menu'
  // if (!tkr_set->Settings.webcam_config.spare15 && tkr_set->Settings.webcam_config.stream && Wc.CamServer && Wc.up!=0) {
  //   // Give the webcam webserver some time to prepare the stream - catch error in JS
  //   WSContentSend_P(PSTR("<p></p><center><img onerror='setTimeout(()=>{this.src=this.src;},1000)' src='http://%_I:81/stream' alt='Webcam stream' style='width:99%%;'></center><p></p>"),(uint32_t)WiFi.localIP());
  //   WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcmenuvideodisable%201", "Turn Off Video");
  // }
}

void mCamera::WcInit(void) {
  // .data is in union with the rest of the settings, so
  // this means 'i have no config'
  if (!tkr_set->Settings.webcam_config.data) {
    // set defaults...
    tkr_set->Settings.webcam_config.stream = 1;
    tkr_set->Settings.webcam_config.resolution = FRAMESIZE_QVGA;
    WcSetDefaults(0);
  }
  // previous webcam driver had only a small subset of possible config vars
  // in this case we have to only set the new variables to default values
  if(!tkr_set->Settings.webcam_config2.upgraded) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Upg settings"));
    WcSetDefaults(1);
    tkr_set->Settings.webcam_config2.upgraded = 1;
  }


}


void mCamera::WcUpdateStats(void) {
  WcStats.camfps = WcStats.camcnt;
  WcStats.camcnt = 0;

  // relies on us getting here each second !!!
  Wc.loopspersec = Wc.loopcounter;
  Wc.loopcounter = 0;
}

// void WcSensorStats(void) {
//   if (!Wc.up) { return; }

//   ResponseAppend_P(PSTR(",\"CAMERA\":{"
//                         "\"" D_WEBCAM_STATS_FPS "\":%d,"
//                         "\"" D_WEBCAM_STATS_CAMFAIL "\":%d,"
//                         "\"" D_WEBCAM_STATS_JPEGFAIL "\":%d,"
//                         "\"" D_WEBCAM_STATS_CLIENTFAIL "\":%d}"),
//                    WcStats.camfps, WcStats.camfail,
//                    WcStats.jpegfail, WcStats.clientfail);
// }

// #ifndef D_WEBCAM_STATE
// #define D_WEBCAM_STATE "State"
// #define D_WEBCAM_POWEREDOFF "PowerOff"
// #define D_WEBCAM_FRAMEFAIL "Frame Failed"
// #define D_WEBCAM_HWFAIL "H/W fail"
// #endif

// const char HTTP_WEBCAM_FPS[] PROGMEM = "{s}%s " D_FRAME_RATE "{m}%d/%d " D_UNIT_FPS  "{e}";
// const char HTTP_WEBCAM_STATE[] PROGMEM = "{s}%s" D_WEBCAM_STATE "{m}%s - %X{e}";

// void WcStatsShow(void) {
// #ifdef USE_WEBSERVER
//   switch(Wc.lastCamError){
//     case ESP_OK: WSContentSend_PD(HTTP_WEBCAM_FPS, WcStats.name, WcStats.camfps, WcStats.maxfps); break;
//     case 2: WSContentSend_PD(HTTP_WEBCAM_STATE, WcStats.name, D_WEBCAM_FRAMEFAIL, Wc.lastCamError); break;
//     case 3: WSContentSend_PD(HTTP_WEBCAM_STATE, WcStats.name, D_WEBCAM_POWEREDOFF, Wc.lastCamError); break;
//     default:  WSContentSend_PD(HTTP_WEBCAM_STATE, WcStats.name, D_WEBCAM_HWFAIL, Wc.lastCamError); break;
//   }
// #endif  // USE_WEBSERVER
// }





void mCamera::Pre_Init(void)
{
  
  ALOG_HGL( PSTR("D_LOG_STARTUP" "LED Init") );

  memset(&Wc, 0, sizeof(Wc));
  #ifdef USE_WEBCAM_MOTION
  memset(&wc_motion, 0, sizeof(wc_motion));
  WcSetMotionDefaults();
  #endif
  WcInit();
  ALOG_INF(PSTR("\n\r\t\t\tCAM: Init webcam done"));

  module_state.mode = ModuleStatus::Initialising;

}


void mCamera::Init(void)
{
  ALOG_INF(PSTR("\n\r\t\t\tCAM: WcSetup STARTED"));
  WcSetup(tkr_set->Settings.webcam_config.resolution);
  ALOG_INF(PSTR("\n\r\t\t\tCAM: WcSetStreamserver STARTED"));
  WcSetStreamserver(tkr_set->Settings.webcam_config.stream);
  ALOG_INF(PSTR("\n\r\t\t\tCAM: WCStartOperationTask STARTED"));
  WCStartOperationTask();
  ALOG_INF(PSTR("\n\r\t\t\tCAM: WCStartOperationTask DONE"));

  // Configured already
  module_state.mode = ModuleStatus::Running;


}


void mCamera::BootMessage()
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  char buffer[100];
  if(used_bitmask)
  {
    mSupport::appendToBuffer(buffer, sizeof(buffer), "#%d ", UsedCount());  
    char buffer2[50];
    for(uint8_t sensor_id = 0; sensor_id<MODULE_LEDS_MAX; sensor_id++)
    {      
      //if bit0 is used as LEDSTATUS, then show this here too
      mSupport::appendToBuffer(buffer, sizeof(buffer), "%d, ", sensor_id);    
    }
  }
  else{
    mSupport::appendToBuffer(buffer, sizeof(buffer), "None");  
  }
  mSupport::removeTrailingComma(buffer);
  ALOG_IMP(PSTR(D_LOG_LOG "%s"), buffer);
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mCamera::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok_sub = 0; 
  int8_t tmp_id = 0;


}
  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mCamera::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
  
  JBI->Add("LED1_INV", tkr_pins->GetPin(GPIO_LED1_INV_ID,0));
  return JBI->End();

}

uint8_t mCamera::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    JBI->Add("LED1_INV", tkr_pins->GetPin(GPIO_LED1_INV_ID,0));
    JBI->Add("LED2_INV", tkr_pins->GetPin(GPIO_LED2_INV_ID,0));

  return JBI->End();

}
  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mCamera::MQTTHandler_Init()
{

  struct handler<mCamera>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCamera::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->GetIfChangedPeriod(); 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCamera::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT


int8_t mCamera::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_BOOT_MESSAGE:
      BootMessage();
    break;
  }
  
  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }
  
  switch(function)
  {
    case TASK_LOOP:
      WcLoop();
    break;
    case TASK_WEB_ADD_HANDLER:
      WcPicSetup();
      break;
    case TASK_WEB_ADD_MAIN_BUTTON:
      WcShowStream();
    break;
    case TASK_EVERY_SECOND:

      if(!psramFound())
      {  
        Serial.printf("esp_spiram_is_initialized(): %d\n", esp_spiram_is_initialized());
        Serial.printf("ESP.getPsramSize(): %d\n", ESP.getPsramSize());            
        AddLog(LOG_LEVEL_INFO, "PSRAM: Found=%d Useable=%d", SupportESP32::FoundPSRAM(), SupportESP32::UsePSRAM());
        AddLog(LOG_LEVEL_INFO, "Heap free: %d, PSRAM free: %d", ESP.getFreeHeap(), ESP.getFreePsram());
      }
          
      WcUpdateStats();
    break;
    case TASK_UPTIME_30_SECONDS:
    // case TASK_WIFI_CONNECTED:
      ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "TASK_UPTIME_30_SECONDS"));

      tkr_set->Settings.webcam_config.stream=1;
      ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "\n\r\t\t\tCAM: WcSetStreamserver STARTED"));
      WcSetStreamserver(tkr_set->Settings.webcam_config.stream);

    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif
  }

  return FUNCTION_RESULT_UNKNOWN_ID;

} // END function


#endif
