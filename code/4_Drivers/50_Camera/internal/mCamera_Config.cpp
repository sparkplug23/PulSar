#include "../mCamera.h"

#ifdef USE_MODULE_DRIVERS__CAMERA


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


/************************************************************************************************
 * SECTION: Camera Configuration / Init / Commands
 ************************************************************************************************/

void mCamera::LoadDefaultConfig()
{
  ALOG_DBG(PSTR(D_LOG_CAMERA "Loading default config"));

  // Actually, cam is enabled at boot, just stream blocked. 
  // so this may not be required.
  // but template, "v", and file load should be performed for all future modules.
  
}

void mCamera::Driver_SetInterruptState(uint32_t state) {
  mSupport::AutoMutex localmutex(&WebcamMutex, "WcInterrupt", 20000);
  // Stop camera ISR if active to fix TG1WDT_SYS_RESET
  if (!rt.up) { return; }

  // why stop/start the server itself here?
  // stopping the cam interrupt should be enough?
  // SetStreamserver(state);
  if (state) {
    // Re-enable interrupts
    cam_start();
    ALOG_DBG(PSTR(D_LOG_CAMERA "cam_start()"));
    rt.disable_cam = 0;
  } else {
    // Stop interrupts
    rt.disable_cam = 1;
    cam_stop();
    ALOG_DBG(PSTR(D_LOG_CAMERA "cam_stop()"));
  }
}

void mCamera::Driver_SuspendForOTA() {
  // if (rt.taskHandle) {
  //   vTaskSuspend(rt.taskHandle);
    Driver_Shutdown();
  //   rt.up = false;
  //   vTaskDelete(rt.taskHandle);
  //   rt.taskHandle = nullptr;
  //   rt.taskRunning = 0;
  // }
}

bool mCamera::Access_WaitUntilZero(volatile int8_t *val, int8_t initial, int timeout_ms){
  int loops = timeout_ms/WC_WAIT_INTERVAL_MS;
  if (!loops) loops = 1;
  *val = initial;
  while(*val && loops--){
    vTaskDelay(WC_WAIT_INTERVAL_MS / portTICK_PERIOD_MS);
  }
  if (*val) return false;
  return true; 
}

void mCamera::Access_WaitUntilEnabled(){
  int timeout_ms = 1000;
  int loops = timeout_ms/WC_WAIT_INTERVAL_MS;
  if (!loops) loops = 1;
  while(rt.disable_cam && loops--){
    vTaskDelay(WC_WAIT_INTERVAL_MS / portTICK_PERIOD_MS);
  }
}

bool mCamera::Pins_AreConfigured(void) 
{


  bool pin_used = true;
  for (uint32_t i = 0; i < 8; i++) {
    if (!tkr_pins->PinUsed(GPIO_WEBCAM_DATA, i)) {
      pin_used = false;
    }
//    if (i < MAX_WEBCAM_HSD) {
//      if (!PinUsed(GPIO_WEBCAM_HSD, i)) {
//        pin_used = false;
//      }
//    }
  }

  ALOG_DBG(PSTR(D_LOG_CAMERA "i2c_enabled: %d"), tkr_set->runtime.i2c_enabled/*[1]*/);

  if (
    !tkr_pins->PinUsed(GPIO_WEBCAM_XCLK) || 
    !tkr_pins->PinUsed(GPIO_WEBCAM_PCLK) ||
    !tkr_pins->PinUsed(GPIO_WEBCAM_VSYNC) || 
    !tkr_pins->PinUsed(GPIO_WEBCAM_HREF) ||
    ((!tkr_pins->PinUsed(GPIO_WEBCAM_SIOD) || !tkr_pins->PinUsed(GPIO_WEBCAM_SIOC)) && !tkr_set->runtime.i2c_enabled/*[1]*/)    // preferred option is to reuse and share I2Cbus 2
  ) {
        pin_used = false;
  }



  return pin_used;
}

void mCamera::Sensor_ApplyFeatureMode(int32_t value) {
  mSupport::AutoMutex localmutex(&WebcamMutex, "WcFeature", 200);
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
  ALOG_DBG(PSTR(D_LOG_CAMERA "Feature: %d"), value);
}

void mCamera::Sensor_ApplyStoredSettings() {
  DEBUG_LINE_HERE3
  mSupport::AutoMutex localmutex(&WebcamMutex, "WcApplySettings", 200);
  DEBUG_LINE_HERE3


  sensor_t * wc_s = esp_camera_sensor_get();
  DEBUG_LINE_HERE3


  if (!wc_s) { 
  DEBUG_LINE_HERE3
  return; }
  DEBUG_LINE_HERE3

  wc_s->set_vflip(wc_s, tkr_iDrivers->webcam_config.flip);
  wc_s->set_hmirror(wc_s, tkr_iDrivers->webcam_config.mirror);

  wc_s->set_framesize(wc_s, (framesize_t)tkr_iDrivers->webcam_config.resolution);
  
  wc_s->set_brightness(wc_s, tkr_iDrivers->webcam_config.brightness - 2);
  wc_s->set_saturation(wc_s, tkr_iDrivers->webcam_config.saturation - 2);
  wc_s->set_contrast(wc_s, tkr_iDrivers->webcam_config.contrast - 2);

  wc_s->set_special_effect(wc_s, tkr_iDrivers->webcam_config2.special_effect);

  wc_s->set_whitebal(wc_s, tkr_iDrivers->webcam_config.awb);
  wc_s->set_wb_mode(wc_s, tkr_iDrivers->webcam_config2.wb_mode);
  wc_s->set_awb_gain(wc_s, tkr_iDrivers->webcam_config.awb_gain);

  wc_s->set_exposure_ctrl(wc_s, tkr_iDrivers->webcam_config.aec);
  wc_s->set_aec_value(wc_s, tkr_iDrivers->webcam_config2.aec_value - 2);
  wc_s->set_ae_level(wc_s, tkr_iDrivers->webcam_config2.ae_level);
  wc_s->set_aec2(wc_s, tkr_iDrivers->webcam_config.aec2);

  wc_s->set_gain_ctrl(wc_s, tkr_iDrivers->webcam_config.agc);
  wc_s->set_agc_gain(wc_s, tkr_iDrivers->webcam_config2.agc_gain);
  wc_s->set_gainceiling(wc_s, (gainceiling_t)tkr_iDrivers->webcam_config2.gainceiling);

  wc_s->set_raw_gma(wc_s, tkr_iDrivers->webcam_config.raw_gma);
  wc_s->set_lenc(wc_s, tkr_iDrivers->webcam_config.lenc);
  wc_s->set_wpc(wc_s, tkr_iDrivers->webcam_config.wpc);
  wc_s->set_dcw(wc_s, tkr_iDrivers->webcam_config.dcw);
  wc_s->set_bpc(wc_s, tkr_iDrivers->webcam_config.bpc);

  Sensor_ApplyFeatureMode(tkr_iDrivers->webcam_config.feature);

  ALOG_DBG(PSTR(D_LOG_CAMERA "Settings updated"));
}

void mCamera::Defaults_Apply(uint32_t upgrade) {
  if (!upgrade) {
    tkr_iDrivers->webcam_config.flip = 0;
    tkr_iDrivers->webcam_config.mirror = 0;

    tkr_iDrivers->webcam_config.saturation = 2; // = 0
    tkr_iDrivers->webcam_config.brightness = 2; // = 0
    tkr_iDrivers->webcam_config.contrast = 2;   // = 0
  }

  
  tkr_iDrivers->webcam_config.resolution = 12;

  tkr_iDrivers->webcam_config2.special_effect = 0;
  tkr_iDrivers->webcam_config.colorbar = 0;

  tkr_iDrivers->webcam_config.awb = 1;        // white balance
  tkr_iDrivers->webcam_config2.wb_mode = 0;   // white balance mode
  tkr_iDrivers->webcam_config.awb_gain = 1;   // white blance gain

  tkr_iDrivers->webcam_config.aec = 1;          // autoexposure (sensor)
  tkr_iDrivers->webcam_config.aec2 = 1;         // autoexposure (dsp)
  tkr_iDrivers->webcam_config2.ae_level = 2;    // autoexposure level (-2 - +2, default 0)
  tkr_iDrivers->webcam_config2.aec_value = 204; // manual exposure value

  tkr_iDrivers->webcam_config.agc = 1;          // auto gain control
  tkr_iDrivers->webcam_config2.agc_gain = 5;    // manual gain control
  tkr_iDrivers->webcam_config2.gainceiling = 0; // auto gain ceiling

  tkr_iDrivers->webcam_config.raw_gma = 1;      // gamma correct
  tkr_iDrivers->webcam_config.lenc = 1;         // lens correction
  tkr_iDrivers->webcam_config.wpc = 1;          // white pixel correct
  tkr_iDrivers->webcam_config.dcw = 1;          // downsize en
  tkr_iDrivers->webcam_config.bpc = 0;          // black pixel correct?

  tkr_iDrivers->webcam_config.feature = 0;
  
  #ifdef USE_WEBCAM_MOTION
    Motion_SetDefaults();
  #endif

  ALOG_DBG(PSTR(D_LOG_CAMERA "Defaults set"));
  
  if (rt.up) { Sensor_ApplyStoredSettings(); }
}

uint32_t mCamera::Driver_InitFromResolution(int32_t frame_size) {

  rt.camera_init = false;

  // we must stall until re-enabled
  Access_WaitUntilEnabled();

  mSupport::AutoMutex localmutex(&WebcamMutex, "WcSetup", 200);

  ALOG_DBG(PSTR(D_LOG_CAMERA "Setup"));
  
  // if 15, make it -1, so disableing
  if (frame_size >= FRAMESIZE_FHD) { frame_size = -1; }

  if (frame_size < 0) {
    if (rt.up){    
      esp_camera_deinit();
      ALOG_DBG(PSTR(D_LOG_CAMERA "Deinit frame_size %d"), frame_size);
      rt.up = 0;
    }
    rt.lastCamError = 0x1;
    return 0;
  }

  if (rt.up) {
    esp_camera_deinit();
    
    ALOG_DBG(PSTR(D_LOG_CAMERA "Deinit"));
    
    //return rt.up;
  }
  rt.up = 0;

//esp_log_level_set("*", ESP_LOG_VERBOSE);

  // camera_config_t config;

  memset(&config, 0, sizeof(config));

  ALOG_DBG(PSTR("Pins_AreConfigured() = %d"), Pins_AreConfigured());


  if (Pins_AreConfigured()) {
    config.pin_d0       = tkr_pins->Pin(GPIO_WEBCAM_DATA, 0);        // Y2_GPIO_NUM;
    config.pin_d1       = tkr_pins->Pin(GPIO_WEBCAM_DATA, 1);     // Y3_GPIO_NUM;
    config.pin_d2       = tkr_pins->Pin(GPIO_WEBCAM_DATA, 2);     // Y4_GPIO_NUM;
    config.pin_d3       = tkr_pins->Pin(GPIO_WEBCAM_DATA, 3);     // Y5_GPIO_NUM;
    config.pin_d4       = tkr_pins->Pin(GPIO_WEBCAM_DATA, 4);     // Y6_GPIO_NUM;
    config.pin_d5       = tkr_pins->Pin(GPIO_WEBCAM_DATA, 5);     // Y7_GPIO_NUM;
    config.pin_d6       = tkr_pins->Pin(GPIO_WEBCAM_DATA, 6);     // Y8_GPIO_NUM;
    config.pin_d7       = tkr_pins->Pin(GPIO_WEBCAM_DATA, 7);     // Y9_GPIO_NUM;
    config.pin_xclk     = tkr_pins->Pin(GPIO_WEBCAM_XCLK);      // XCLK_GPIO_NUM;
    config.pin_pclk     = tkr_pins->Pin(GPIO_WEBCAM_PCLK);      // PCLK_GPIO_NUM;
    config.pin_vsync    = tkr_pins->Pin(GPIO_WEBCAM_VSYNC);    // VSYNC_GPIO_NUM;
    config.pin_href     = tkr_pins->Pin(GPIO_WEBCAM_HREF);      // HREF_GPIO_NUM;
    config.pin_sccb_sda = tkr_pins->Pin(GPIO_WEBCAM_SIOD);  // SIOD_GPIO_NUM; - unset to use shared I2C bus 2
    config.pin_sccb_scl = tkr_pins->Pin(GPIO_WEBCAM_SIOC);  // SIOC_GPIO_NUM;
    if(tkr_set->runtime.i2c_enabled/*[1]*/){              // configure SIOD and SIOC as SDA,2 and SCL,2
      config.sccb_i2c_port = 1;                   // reuse initialized bus 2, can be shared now
      if(config.pin_sccb_sda < 0){                // GPIO_WEBCAM_SIOD must not be set to really make it happen
        
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "Use I2C bus2"));
        
      }
    }
    config.pin_pwdn = tkr_pins->Pin(GPIO_WEBCAM_PWDN);       // PWDN_GPIO_NUM;
    config.pin_reset = tkr_pins->Pin(GPIO_WEBCAM_RESET);    // RESET_GPIO_NUM;
    
    ALOG_DBG(PSTR(D_LOG_CAMERA "Template pin config"));
    
  } else if (Y2_GPIO_NUM != -1) {
    // Modell is set in camera_pins.h
    // config.pin_d0 = Y2_GPIO_NUM;
    // config.pin_d1 = Y3_GPIO_NUM;
    // config.pin_d2 = Y4_GPIO_NUM;
    // config.pin_d3 = Y5_GPIO_NUM;
    // config.pin_d4 = Y6_GPIO_NUM;
    // config.pin_d5 = Y7_GPIO_NUM;
    // config.pin_d6 = Y8_GPIO_NUM;
    // config.pin_d7 = Y9_GPIO_NUM;
    // config.pin_xclk = XCLK_GPIO_NUM;
    // config.pin_pclk = PCLK_GPIO_NUM;
    // config.pin_vsync = VSYNC_GPIO_NUM;
    // config.pin_href = HREF_GPIO_NUM;
    // config.pin_sscb_sda = SIOD_GPIO_NUM;
    // config.pin_sscb_scl = SIOC_GPIO_NUM;
    // config.pin_pwdn = PWDN_GPIO_NUM;
    // config.pin_reset = RESET_GPIO_NUM;
    
    // ALOG_DBG(PSTR(D_LOG_CAMERA "Compile flag pin config"));

    /*******************************************************************************************\
     * Compile flag camera pin config
     *
     * The camera pins are provided by camera_pins.h.
     * Register them back into the pin table using the generic GPIO_WEBCAM_* functions so they
     * appear in pin telemetry and allocation state.
    \*******************************************************************************************/

    mPins::PinAllocationFlags camera_flags;
    camera_flags.data = 0;
    camera_flags.grouped = 1;
    camera_flags.locked = 1;
    camera_flags.sensitive_to_probe = 1;

    const uint16_t owner_id = GetModuleUniqueID();

    config.pin_d0    = Y2_GPIO_NUM;
    config.pin_d1    = Y3_GPIO_NUM;
    config.pin_d2    = Y4_GPIO_NUM;
    config.pin_d3    = Y5_GPIO_NUM;
    config.pin_d4    = Y6_GPIO_NUM;
    config.pin_d5    = Y7_GPIO_NUM;
    config.pin_d6    = Y8_GPIO_NUM;
    config.pin_d7    = Y9_GPIO_NUM;
    config.pin_xclk  = XCLK_GPIO_NUM;
    config.pin_pclk  = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href  = HREF_GPIO_NUM;

    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;

    config.pin_pwdn  = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    tkr_pins->PinTable_SerialPrint("before camera AllocatePin");

    if(config.pin_d0    >= 0) { tkr_pins->AllocatePin(config.pin_d0,    PIGPIO(GPIO_WEBCAM_DATA, 0),  owner_id, camera_flags); }
    if(config.pin_d1    >= 0) { tkr_pins->AllocatePin(config.pin_d1,    PIGPIO(GPIO_WEBCAM_DATA, 1),  owner_id, camera_flags); }
    if(config.pin_d2    >= 0) { tkr_pins->AllocatePin(config.pin_d2,    PIGPIO(GPIO_WEBCAM_DATA, 2),  owner_id, camera_flags); }
    if(config.pin_d3    >= 0) { tkr_pins->AllocatePin(config.pin_d3,    PIGPIO(GPIO_WEBCAM_DATA, 3),  owner_id, camera_flags); }
    if(config.pin_d4    >= 0) { tkr_pins->AllocatePin(config.pin_d4,    PIGPIO(GPIO_WEBCAM_DATA, 4),  owner_id, camera_flags); }
    if(config.pin_d5    >= 0) { tkr_pins->AllocatePin(config.pin_d5,    PIGPIO(GPIO_WEBCAM_DATA, 5),  owner_id, camera_flags); }
    if(config.pin_d6    >= 0) { tkr_pins->AllocatePin(config.pin_d6,    PIGPIO(GPIO_WEBCAM_DATA, 6),  owner_id, camera_flags); }
    if(config.pin_d7    >= 0) { tkr_pins->AllocatePin(config.pin_d7,    PIGPIO(GPIO_WEBCAM_DATA, 7),  owner_id, camera_flags); }

    if(config.pin_xclk  >= 0) { tkr_pins->AllocatePin(config.pin_xclk,  PGPIO(GPIO_WEBCAM_XCLK),      owner_id, camera_flags); }
    if(config.pin_pclk  >= 0) { tkr_pins->AllocatePin(config.pin_pclk,  PGPIO(GPIO_WEBCAM_PCLK),      owner_id, camera_flags); }
    if(config.pin_vsync >= 0) { tkr_pins->AllocatePin(config.pin_vsync, PGPIO(GPIO_WEBCAM_VSYNC),     owner_id, camera_flags); }
    if(config.pin_href  >= 0) { tkr_pins->AllocatePin(config.pin_href,  PGPIO(GPIO_WEBCAM_HREF),      owner_id, camera_flags); }

    if(config.pin_sccb_sda >= 0) { tkr_pins->AllocatePin(config.pin_sccb_sda, PGPIO(GPIO_WEBCAM_SIOD), owner_id, camera_flags); }
    if(config.pin_sccb_scl >= 0) { tkr_pins->AllocatePin(config.pin_sccb_scl, PGPIO(GPIO_WEBCAM_SIOC), owner_id, camera_flags); }

    if(config.pin_pwdn  >= 0) { tkr_pins->AllocatePin(config.pin_pwdn,  PGPIO(GPIO_WEBCAM_PWDN),      owner_id, camera_flags); }
    if(config.pin_reset >= 0) { tkr_pins->AllocatePin(config.pin_reset, PGPIO(GPIO_WEBCAM_RESET),     owner_id, camera_flags); }

    tkr_pins->PinTable_SerialPrint("after camera AllocatePin");

    ALOG_DBG(PSTR(D_LOG_CAMERA "Compile flag pin config"));
    
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

  ALOG_DBG("CAM: get ledc channel");
  

  #ifdef ENABLE_DEVFEATURE_ANALOG_WRITE_EMULATION_VERSION2
  int32_t ledc_channel = analogAttach(config.pin_xclk);/*added by me*/
  #else
  int32_t ledc_channel = 0;
  analogAttach(config.pin_xclk,ledc_channel);/*added by me*/
  #endif
  if (ledc_channel < 0) {
    AddLog(LOG_LEVEL_ERROR, "CAM: cannot allocated ledc channel, remove a PWM GPIO");
  }
  config.ledc_channel = (ledc_channel_t) ledc_channel;
  
  AddLog(LOG_LEVEL_INFO, "CAM: XCLK on GPIO %i using ledc channel %i", config.pin_xclk, config.ledc_channel);
  
  config.ledc_timer = LEDC_TIMER_0;
//  config.xclk_freq_hz = 20000000;
  if (!tkr_iDrivers->webcam_clk) tkr_iDrivers->webcam_clk = 20;
  config.xclk_freq_hz = tkr_iDrivers->webcam_clk * 1000000;
  int pixFormat = PIXFORMAT_JPEG;
/* 2023-05-28 - AiThinker type cam module marked DM.
  tried everything here, and it seems you cannot get anything other than JPEG.
  even if it's set to another value at power on.  
  Maybe it will work with a different camera, or a board with a CAM_RESET line.
*/
  if (rt.camPixelFormat) pixFormat = (rt.camPixelFormat-1);
  
  config.pixel_format = (pixformat_t)pixFormat;
  if (config.pixel_format != PIXFORMAT_JPEG)
  {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "Pixel format is %d, not JPEG"), config.pixel_format);
  }
  //;
  //esp_log_level_set("*", ESP_LOG_INFO);

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.

  ALOG_DBG("CAM: get psram");

  AddLog(LOG_LEVEL_INFO, "PSRAM: Found=%d Useable=%d", SupportESP32::FoundPSRAM(), SupportESP32::UsePSRAM());
  AddLog(LOG_LEVEL_INFO, "Heap free: %d, PSRAM free: %d", ESP.getFreeHeap(), ESP.getFreePsram());

  rt.psram = SupportESP32::UsePSRAM();
  if (rt.psram) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    ALOG_DBG(PSTR(D_LOG_CAMERA "PSRAM found"));
  } else {
    config.frame_size = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_DRAM;
    ALOG_DBG(PSTR(D_LOG_CAMERA "PSRAM not found"));
  }

  // ALOG_DBG(PSTR(D_LOG_CAMERA "Waiting for stable test %d"), millis());
  // delay(4000);
  // ALOG_DBG(PSTR(D_LOG_CAMERA "Waiting for stable test... continue %d"), millis());

  DEBUG_LINE_HERE3;

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
    rt.lastCamError = err;
    return 0;
  }

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "heap check 2: %d"),ESP.getFreeHeap());

  DEBUG_WAIT_POINT_MS(2000);


  // drop down frame size for higher initial frame rate
  sensor_t * wc_s = esp_camera_sensor_get();
  // seems config.frame_size has no effect?
  // so this is where we set framesize....
  wc_s->set_framesize(wc_s, (framesize_t)frame_size);

  camera_fb_t *wc_fb = esp_camera_fb_get();
  if (!wc_fb) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "Init failed !frame on time"));
    rt.lastCamError = 2;
    return 0;
  }

  if(wc_fb)
  {
    rt.camera_init = true;
    ALOG_DBG(PSTR(D_LOG_CAMERA "Camera init success, frame size %d, fb size %d"), wc_fb->width, wc_fb->len);
  }else
  {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Camera init FAILED"));//, frame size %d, fb size %d"), wc_fb->width, wc_fb->len);

    return 0;

  }


  rt.width = wc_fb->width;
  rt.height = wc_fb->height;
  esp_camera_fb_return(wc_fb);

  DEBUG_LINE_HERE3

  Sensor_ApplyStoredSettings();

  DEBUG_LINE_HERE3
  camera_sensor_info_t *info = esp_camera_sensor_get_info(&wc_s->id);

  DEBUG_LINE_HERE3
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "%s Initialized"), info->name);
  tkr_set->runtime.camera_initialized = true;
  rt.up = 1;
  if (rt.psram) { rt.up = 2; }

  rt.frameIntervalsus = (uint32_t)(((float)nativeIntervals20ms[frame_size]/((float)tkr_iDrivers->webcam_clk/20.0))*1000.0);
  stats.maxfps = (uint32_t)((float)1000000.0/(float)rt.frameIntervalsus);

  rt.lastCamError = ESP_OK;
  return rt.up;
}

void mCamera::Stream_End(){
  // we should use a mutext here, in case we are currently sending
  mSupport::AutoMutex localmutex(&WebcamMutex, "WcLoop2", 20000);
  // if http streaming is active
  mCamera::wc_client *client = rt.client_p;
  // iterate over clients
  while(client){
    client->active = 0;
    if (client->client){
      client->client.stop();
    }
    client = client->p_next;
  }
  Stream_RemoveDeadClients();
}

void mCamera::Driver_Shutdown() {
  mSupport::AutoMutex localmutex(&WebcamMutex, "WcCamOff", 30000);
  // deinit camera
  Driver_InitFromResolution(-1);
  // kill any existing clients
  Stream_End();
#ifdef ENABLE_RTSPSERVER
  // kill any existing rtsp clients
  EndRTSP();
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
  rt.lastCamError = 0x3;
}

int32_t mCamera::Options_Set(uint32_t sel, int32_t value) {
  int32_t res = 0;
  mSupport::AutoMutex localmutex(&WebcamMutex, "Options_Set", 200);

  // don;t want sensor for this one.
  switch (sel) {
    case 24:
      // frames to ignore between reads
      if (value >= 0) { rt.skipFrames = value; }
      return value;
      break;
    case 25:
      // pixelformat - native formats + 1, 0->jpeg
      if (value >= 0) { rt.camPixelFormat = value; }
      if (rt.up){
        Driver_InitFromResolution(tkr_iDrivers->webcam_config.resolution);
      }
      return value;
      break;
  }


  return res;
}

uint32_t mCamera::Stream_SetEnabled(uint32_t flag) 
{
  ALOG_DBG(PSTR("=========================CAM: SetStreamserver %d"), flag);
  if (tkr_set->runtime.global_state.network_down) 
  {
    ALOG_DBG(PSTR("=========================CAM: Network down, cannot set stream server"));
    Stream_End();
    return 0; 
  }
  AddLog(LOG_LEVEL_INFO, PSTR("SetStreamserver %d"), flag);
  if (flag) {
    if (!rt.CamServer) {
      ALOG_DBG(PSTR("=========================CAM: if (!rt.CamServer) {"));
            
      mSupport::AutoMutex localmutex(&WebcamMutex, "HandleWebcamMjpeg", 20000);
      rt.CamServer = new WebServer(81);

      /***
       * Videos
       ***/
      SPGM_CTR(PM_URL_CAM_ROOT) "/";
      rt.CamServer->on(PM_URL_CAM_ROOT, [this](){ this->Web_HandleRoot(); });    
      AddURLtoListwPort(PM_URL_CAM_ROOT, HTTP_GET, 81);

      SPGM_CTR(PM_URL_CAM_DIFF_MJPEG) "/diff.mjpeg";
      rt.CamServer->on(PM_URL_CAM_DIFF_MJPEG, [this](){ this->Stream_HandleMjpegDiff(); });
      AddURLtoListwPort(PM_URL_CAM_DIFF_MJPEG, HTTP_GET, 81);

      SPGM_CTR(PM_URL_CAM_STREAM) "/stream";
      rt.CamServer->on(PM_URL_CAM_STREAM, [this](){ this->Stream_HandleMjpegRoot(); });
      AddURLtoListwPort(PM_URL_CAM_STREAM, HTTP_GET, 81);

      /***
       * Picture
       ***/
      SPGM_CTR(PM_URL_CAM_PICTURE) "/picture";
      rt.CamServer->on(PM_URL_CAM_PICTURE, [this](){ this->Web_HandleImage(); });
      AddURLtoListwPort(PM_URL_CAM_PICTURE, HTTP_GET, 81);

      #ifdef USE_WEBCAM_MOTION

      SPGM_CTR(PM_URL_CAM_MOTIONDIFF_JPG) "/motiondiff.jpg";
      rt.CamServer->on(PM_URL_CAM_MOTIONDIFF_JPG, [this](){ this->HandleImagemotiondiff(); });
      AddURLtoListwPort(PM_URL_CAM_MOTIONDIFF_JPG, HTTP_GET, 81);

      SPGM_CTR(PM_URL_CAM_MOTIONMASK_JPG) "/motionmask.jpg";
      rt.CamServer->on(PM_URL_CAM_MOTIONMASK_JPG, [this](){ this->HandleImagemotionmask(); });
      AddURLtoListwPort(PM_URL_CAM_MOTIONMASK_JPG, HTTP_GET, 81);

      SPGM_CTR(PM_URL_CAM_MOTIONBUFF_JPG) "/motionbuff.jpg";
      rt.CamServer->on(PM_URL_CAM_MOTIONBUFF_JPG, [this](){ this->HandleImagemotionbuff(); });
      AddURLtoListwPort(PM_URL_CAM_MOTIONBUFF_JPG, HTTP_GET, 81);

      SPGM_CTR(PM_URL_CAM_MOTIONLBUFF_JPG) "/motionlbuff.jpg";
      rt.CamServer->on(PM_URL_CAM_MOTIONLBUFF_JPG, [this](){ this->HandleImagemotionlbuff(); });
      AddURLtoListwPort(PM_URL_CAM_MOTIONLBUFF_JPG, HTTP_GET, 81);

      SPGM_CTR(PM_URL_CAM_MOTIONBACKGROUNDBUFF_JPG) "/motionbackgroundbuff.jpg";
      rt.CamServer->on(PM_URL_CAM_MOTIONBACKGROUNDBUFF_JPG, [this](){ this->HandleImagemotionbackgroundbuff(); });
      AddURLtoListwPort(PM_URL_CAM_MOTIONBACKGROUNDBUFF_JPG, HTTP_GET, 81);

      #endif  

      ALOG_DBG(PSTR(D_LOG_CAMERA "Strm init"));
      rt.CamServer->begin();

      
  
      tkr_web->AddURLasApplication(GetModuleUniqueID(), "stream", "Stream", 81);
      tkr_web->AddURLasApplication(GetModuleUniqueID(), "picture", "Picture", 81);

      #ifdef USE_MODULE_NETWORK_WEBSERVER
        tkr_web->AddURLasApplication(GetModuleUniqueID(), "sdedit", "SD Card");
      #endif

    }else{
      
      ALOG_DBG(PSTR("=========================CAM: else (!rt.CamServer) {"));
    }
  } else {
    ALOG_DBG(PSTR("=========================CAM: } else {"));
    if (rt.CamServer) {
      ALOG_DBG(PSTR("=========================CAM: } else {if (rt.CamServer) {"));
      mSupport::AutoMutex localmutex(&WebcamMutex, "HandleWebcamMjpeg", 20000);
      Stream_End();
      rt.CamServer->stop();
      delete rt.CamServer;
      rt.CamServer = NULL;
      ALOG_DBG(PSTR(D_LOG_CAMERA "Strm exit"));
    }
  }
  return 0;
}

void mCamera::Defaults_LoadOrCreate(void) {
  // .data is in union with the rest of the settings, so
  // this means 'i have no config'
  if (!tkr_iDrivers->webcam_config.data) {
    // set defaults...
    tkr_iDrivers->webcam_config.stream = 1;
    tkr_iDrivers->webcam_config.resolution = FRAMESIZE_QVGA;
    Defaults_Apply(0);
  }
  // previous webcam driver had only a small subset of possible config vars
  // in this case we have to only set the new variables to default values
  if(!tkr_iDrivers->webcam_config2.upgraded) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Upg settings"));
    Defaults_Apply(1);
    tkr_iDrivers->webcam_config2.upgraded = 1;
  }

}

void mCamera::Pre_Init(void)
{
  
  ALOG_DBG( PSTR("mCamera::Pre_Init") );

  memset(&rt, 0, sizeof(rt));

  #ifdef USE_WEBCAM_MOTION
  Motion_ClearStruct();
  Motion_SetDefaults();
  #endif
  Defaults_LoadOrCreate();
  ALOG_DBG(PSTR("\n\r\t\t\tCAM: Init webcam done"));

  module_state.mode = ModuleStatus::Initialising;

}

void mCamera::Init(void)
{
  
  Driver_InitFromResolution(tkr_iDrivers->webcam_config.resolution);
  Stream_SetEnabled(tkr_iDrivers->webcam_config.stream);
  FrameTask_Start();
  
  tkr_iDrivers->webcam_config.rtsp = 1;


  // Configured already
  module_state.mode = ModuleStatus::Running;

}

void mCamera::BootMessage()
{
  #ifdef ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
  // char buffer[100];
  // if(used_bitmask)
  // {
  //   mSupport::appendToBuffer(buffer, sizeof(buffer), "#%d ", UsedCount());  
  //   char buffer2[50];
  //   for(uint8_t sensor_id = 0; sensor_id<MODULE_LEDS_MAX; sensor_id++)
  //   {      
  //     //if bit0 is used as LEDSTATUS, then show this here too
  //     mSupport::appendToBuffer(buffer, sizeof(buffer), "%d, ", sensor_id);    
  //   }
  // }
  // else{
  //   mSupport::appendToBuffer(buffer, sizeof(buffer), "None");  
  // }
  // mSupport::removeTrailingComma(buffer);
  // ALOG_IMP(PSTR(D_LOG_CAMERA "%s"), buffer);
  #endif // ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
}

void mCamera::CommandSet_PowerOff(void){
  Driver_Shutdown();
}

void mCamera::CommandSet_TaskEnable(bool enable) {
  if(enable)
  {
    if (rt.taskRunning == 0){
      FrameTask_Start();
    }
  }
  else
  {
    FrameTask_Stop();
  }
}

void mCamera::FrameTask_Stop(void){
  if (rt.taskRunning == 1){
    // set to 2, and wait until cleared
    Access_WaitUntilZero(&rt.taskRunning, 2, 20000);
    if (rt.taskHandle){
      // why does this cause a problem?
      //vTaskDelete(rt.taskHandle);
      rt.taskHandle = nullptr;
    }
  }
}

void mCamera::CommandSet_Resolution(uint8_t resolution) 
{
      DEBUG_LINE_HERE3
    int8_t reinit = 0;
    
    tkr_iDrivers->webcam_config.resolution = resolution;
    if (reinit) {
      Driver_InitFromResolution(tkr_iDrivers->webcam_config.resolution);
    } else {
      // WcSetOptions(0, tkr_iDrivers->webcam_config.resolution);

      DEBUG_LINE_HERE3
      
      sensor_t* s = esp_camera_sensor_get();
      int32_t res = 0;
      DEBUG_LINE_HERE3
      if (resolution >= 0) { 
      DEBUG_LINE_HERE3
        s->set_framesize(s, (framesize_t)resolution);  // crash here
      DEBUG_LINE_HERE3
      }
      DEBUG_LINE_HERE3
      res = s->status.framesize;
      rt.width = 0;
      rt.height = 0;
      rt.last_frame_len = 0;
      DEBUG_LINE_HERE3
      rt.frameIntervalsus = (uint32_t)(((float)nativeIntervals20ms[resolution]/((float)tkr_iDrivers->webcam_clk/20.0))*1000.0);
      stats.maxfps = (uint32_t)((float)1000000.0/(float)rt.frameIntervalsus);

      DEBUG_LINE_HERE3
      // WcFeature is lost on resolution change
      DEBUG_LINE_HERE3
      Sensor_ApplyStoredSettings();
      DEBUG_LINE_HERE3
    }    
      DEBUG_LINE_HERE3
    
}

void mCamera::CommandSet_Mirror(bool val){
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
      tkr_iDrivers->webcam_config.mirror = val;
      s->set_hmirror(s, val);
    }
  }
}

void mCamera::CommandSet_Flip(bool val) 
{
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
      tkr_iDrivers->webcam_config.flip = val;
      s->set_vflip(s, val);
    }
  }
}

void mCamera::CommandSet_Saturation(int8_t val) 
{
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=-2 && val <= 2){
      tkr_iDrivers->webcam_config.saturation = val + 2;
      s->set_saturation(s, val);
    }
  }
}

void mCamera::CommandSet_Brightness(int8_t val) 
{
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=-2 && val <= 2){
      tkr_iDrivers->webcam_config.brightness = val + 2;
      s->set_brightness(s, val);
    }
  }
}

void mCamera::CommandSet_Contrast(int8_t val) 
{
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=-2 && val <= 2){
      tkr_iDrivers->webcam_config.contrast = val + 2;
      s->set_contrast(s, val);
    }
  }
}

void mCamera::CommandSet_SpecialEffect(uint8_t val) {  
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 6){
			tkr_iDrivers->webcam_config2.special_effect = val;
      s->set_special_effect(s, val);
    }
  }
}

void mCamera::CommandSet_AWB(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
			tkr_iDrivers->webcam_config.awb = val;
      s->set_whitebal(s, val);
    }
  }
}

void mCamera::CommandSet_WBMode(uint8_t val) {  
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 4){
			tkr_iDrivers->webcam_config2.wb_mode = val;
      s->set_wb_mode(s, val);
    }
  }
}

void mCamera::CommandSet_AWBGain(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
			tkr_iDrivers->webcam_config.awb_gain = val;
      s->set_awb_gain(s, val);
    }
  }
}

void mCamera::CommandSet_AEC(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
			tkr_iDrivers->webcam_config.aec = val;
      s->set_exposure_ctrl(s, val);
    }
  }
}

void mCamera::CommandSet_AECValue(uint16_t val) {  
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1200){
			tkr_iDrivers->webcam_config2.aec_value = val;
      s->set_aec_value(s, val);
    }
  }
}

void mCamera::CommandSet_AELevel(int8_t val) 
{
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=-2 && val <= 2){
      tkr_iDrivers->webcam_config2.ae_level = val + 2;
      s->set_ae_level(s, val);
    }
  }
}

void mCamera::CommandSet_AEC2(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
			tkr_iDrivers->webcam_config.aec2 = val;
      s->set_aec2(s, val);
    }
  }
}

void mCamera::CommandSet_AGC(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
			tkr_iDrivers->webcam_config.agc = val;
      s->set_gain_ctrl(s, val);
    }
  }
}

void mCamera::CommandSet_AGCGain(uint8_t val) 
{
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 30){
      tkr_iDrivers->webcam_config2.agc_gain = val;
      s->set_agc_gain(s, val);
    }
  }
}

void mCamera::CommandSet_GainCeiling(uint8_t val) 
{
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 30){
      tkr_iDrivers->webcam_config2.gainceiling = val;
      s->set_gainceiling(s, (gainceiling_t)val);
    }
  }
}

void mCamera::CommandSet_GammaCorrect(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
      tkr_iDrivers->webcam_config.raw_gma = val;
      s->set_raw_gma(s, val);
    }
  }
}

void mCamera::CommandSet_LensCorrect(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
      tkr_iDrivers->webcam_config.lenc = val;
      s->set_lenc(s, val);
    }
  }
}

void mCamera::CommandSet_WPC(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
      tkr_iDrivers->webcam_config.wpc = val;
      s->set_wpc(s, val);
    }
  }
}

void mCamera::CommandSet_DCW(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
      tkr_iDrivers->webcam_config.dcw = val;
      s->set_dcw(s, val);
    }
  }
}

void mCamera::CommandSet_BPC(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
      tkr_iDrivers->webcam_config.bpc = val;
      s->set_bpc(s, val); // blackpixelcontrol
    }
  }
}

void mCamera::CommandSet_Colorbar(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
      tkr_iDrivers->webcam_config.colorbar = val;
      s->set_colorbar(s, val);
    }
  }
}

void mCamera::CommandSet_Feature(uint8_t val) 
{
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 2){
      tkr_iDrivers->webcam_config2.agc_gain = val;
      Sensor_ApplyFeatureMode(val);
    }
  }
}

void mCamera::CommandSet_Auth(bool val) {
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >=0 && val <= 1){
      tkr_iDrivers->webcam_config2.auth = val;
    }
  }
}

void mCamera::CommandSet_Clock(uint16_t val) 
{
  if (sensor_t* s = esp_camera_sensor_get()){
    if(val >= 10 && val <= 200){
      tkr_iDrivers->webcam_clk = val;
      if (rt.up){
        Driver_InitFromResolution(tkr_iDrivers->webcam_config.resolution);
      }
    }
  }
}

void mCamera::CommandSet_CamStartStop(bool val) {
  if(val >= 0 && val <= 1){
    Driver_SetInterruptState(val);
  }
}

void mCamera::CommandSet_Init() {
  Driver_InitFromResolution(tkr_iDrivers->webcam_config.resolution);
  Stream_SetEnabled(tkr_iDrivers->webcam_config.stream);
}

void mCamera::CommandSet_SetDefaults() {
  Defaults_Apply(0);
}

void mCamera::CommandSet_RtspActive(bool val) {
  if(val >= 0 && val <= 1){
    tkr_iDrivers->webcam_config.rtsp = val;
  }
}

void mCamera::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok_sub = 0; 
  int8_t tmp_id = 0;

  if(jtok = obj[D_MODULE__DRIVERS__CAMERA__CTR].getObject()["Init"])
  {
    Init();
  }
  if(jtok = obj[D_MODULE__DRIVERS__CAMERA__CTR].getObject()["psramInit"])
  {
    psramInit();               // initialize PSRAM
        
    ALOG_DBG(PSTR("psramFound: %d\n"), psramFound());
    ALOG_DBG(PSTR("esp_spiram_is_initialized: %d\n"), esp_spiram_is_initialized());
    ALOG_DBG(PSTR("Free PSRAM: %u\n"), ESP.getFreePsram());
  }

  // Only allow commands if camera is initialised, else we could have mutex problems if we try to change settings mid-initialisation
  if(!rt.camera_init) return;

  JsonParserObject jobj = 0; 
  
  if(!(jobj = obj[D_MODULE__DRIVERS__CAMERA__CTR].getObject()))
  {
    ALOG_ERR(PSTR(D_LOG_CAMERA "No Cam object found"));
    return;
  }
	
  mSupport::AutoMutex localmutex(&WebcamMutex, "parse_JSONCommand", 200);

  if(jtok = jobj["Resolution"])
  {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Resolution %d"), jtok.getInt());
    CommandSet_Resolution(jtok.getInt());
  }

  if(jtok = jobj["Mirror"])
  {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Mirror %d"), jtok.getInt());
    CommandSet_Mirror(jtok.getInt());
  }

  if(jtok = jobj["Flip"])
  {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Flip %d"), jtok.getInt());
    CommandSet_Flip(jtok.getBool());
  }

  if(jtok = jobj["Saturation"])
  {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Saturation %d [±2]"), jtok.getInt());
    CommandSet_Saturation(jtok.getInt());
  }

  if (jtok = jobj["Brightness"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Brightness %d [±2]"), jtok.getInt());
    CommandSet_Brightness(jtok.getInt());
  }

  if (jtok = jobj["Contrast"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Contrast %d [±2]"), jtok.getInt());
    CommandSet_Contrast(jtok.getInt());
  }

  if (jtok = jobj["SpecialEffect"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "SpecialEffect %d [0-6]"), jtok.getInt());
    CommandSet_SpecialEffect(jtok.getInt());
  }

  if (jtok = jobj["AWB"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "AWB %d [0/1]"), jtok.getInt());
    CommandSet_AWB(jtok.getBool());
  }

  if (jtok = jobj["WBMode"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "WBMode %d [0-4]"), jtok.getInt());
    CommandSet_WBMode(jtok.getInt());
  }

  if (jtok = jobj["AWBGain"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "AWBGain %d [0/1]"), jtok.getInt());
    CommandSet_AWBGain(jtok.getBool());
  }

  if (jtok = jobj["AEC"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "AEC %d [0/1]"), jtok.getInt());
    CommandSet_AEC(jtok.getBool());
  }

  if (jtok = jobj["AECValue"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "AECValue %d [0-1200]"), jtok.getInt());
    CommandSet_AECValue(jtok.getInt());
  }

  if (jtok = jobj["AELevel"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "AELevel %d [±2]"), jtok.getInt());
    CommandSet_AELevel(jtok.getInt());
  }

  if (jtok = jobj["AEC2"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "AEC2 %d [0/1]"), jtok.getInt());
    CommandSet_AEC2(jtok.getBool());
  }

  if (jtok = jobj["AGC"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "AGC %d [0/1]"), jtok.getInt());
    CommandSet_AGC(jtok.getBool());
  }

  if (jtok = jobj["AGCGain"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "AGCGain %d [0-30]"), jtok.getInt());
    CommandSet_AGCGain(jtok.getInt());
  }

  if (jtok = jobj["GainCeiling"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "GainCeiling %d [0-6]"), jtok.getInt());
    CommandSet_GainCeiling(jtok.getInt());
  }

  if (jtok = jobj["GammaCorrect"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "GammaCorrect %d [0/1]"), jtok.getInt());
    CommandSet_GammaCorrect(jtok.getBool());
  }

  if (jtok = jobj["LensCorrect"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "LensCorrect %d [0/1]"), jtok.getInt());
    CommandSet_LensCorrect(jtok.getBool());
  }

  if (jtok = jobj["WPC"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "WPC %d [0/1]"), jtok.getInt());
    CommandSet_WPC(jtok.getBool());
  }

  if (jtok = jobj["DCW"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "DCW %d [0/1]"), jtok.getInt());
    CommandSet_DCW(jtok.getBool());
  }

  if (jtok = jobj["BPC"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "BPC %d [0/1]"), jtok.getInt());
    CommandSet_BPC(jtok.getBool());
  }

  if (jtok = jobj["Colorbar"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Colorbar %d [0/1]"), jtok.getInt());
    CommandSet_Colorbar(jtok.getBool());
  }

  if (jtok = jobj["Feature"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Feature %d [0–2]"), jtok.getInt());
    CommandSet_Feature(jtok.getInt());
  }

  if (jtok = jobj["Auth"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Auth %d [0/1]"), jtok.getInt());
    CommandSet_Auth(jtok.getBool());
  }

  if (jtok = jobj["Clock"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Clock %d [10–200]"), jtok.getInt());
    CommandSet_Clock(jtok.getInt());
  }

  if (jtok = jobj["CamStartStop"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "CamStartStop %d [0/1]"), jtok.getInt());
    CommandSet_CamStartStop(jtok.getBool());
  }
  
  if (jtok = jobj[D_CMND_WC_GETFRAME]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Frame_RequestCaptureToStore %d [1-4]"), jtok.getInt());
    CommandSet_GetFrame(jtok.getInt());
  }

  if (jtok = jobj[D_CMND_WC_GETPICSTORE]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "GetPicStore %d [1-4]"), jtok.getInt());
    CommandSet_GetPicStore(jtok.getInt());
  }

  if (jtok = jobj[D_CMND_WC_POWEROFF]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA D_CMND_WC_POWEROFF " %d [0/1]"), jtok.getInt());
    CommandSet_PowerOff();
  }

  if (jtok = jobj["StartTask"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "StartTask" " %d [0/1]"), jtok.getInt());
    CommandSet_TaskEnable(true);
  }

  if (jtok = jobj["StopTask"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "StopTask" " %d [0/1]"), jtok.getInt());
    CommandSet_TaskEnable(false);
  }

  if (jtok = jobj["Init"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "Init"));
    CommandSet_Init();
  }

  if (jtok = jobj["SetDefaults"]) {
    ALOG_DBG(PSTR(D_LOG_CAMERA "SetDefaults"));
    CommandSet_SetDefaults();
  }

}

uint8_t mCamera::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
  
  JBI->Object_Start("Pins");
    JBI->Add("d0", config.pin_d0);
    JBI->Add("d1", config.pin_d1);
    JBI->Add("d2", config.pin_d2);
    JBI->Add("d3", config.pin_d3);
    JBI->Add("d4", config.pin_d4);
    JBI->Add("d5", config.pin_d5);
    JBI->Add("d6", config.pin_d6);
    JBI->Add("d7", config.pin_d7);
    JBI->Add("xclk", config.pin_xclk);
    JBI->Add("pclk", config.pin_pclk);
    JBI->Add("vsync", config.pin_vsync);
    JBI->Add("href", config.pin_href);
    JBI->Add("sccb_sda", config.pin_sccb_sda);
    JBI->Add("sccb_scl", config.pin_sccb_scl);
  JBI->Object_End();

  return JBI->End();

}

uint8_t mCamera::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    // JBI->Add("LED1_INV", tkr_pins->GetPin(GPIO_LED1_INV,0));
    // JBI->Add("LED2_INV", tkr_pins->GetPin(GPIO_LED2_INV,0));

    size_t free_stack = uxTaskGetStackHighWaterMark(rt.taskHandle); // returns "word/U32" being used, so multiply by 4 to get bytes
// Serial.printf("Free stack: %d bytes\n", free_stack * 4);
  JBI->Add("FreeStack", (uint32_t)(free_stack * 4)); // If the number is small (<100 words), you’re about to overflow.




      // Webcam basic config
  JBI->Add(("stream"),        tkr_iDrivers->webcam_config.stream);
  JBI->Add(("mirror"),        tkr_iDrivers->webcam_config.mirror);
  JBI->Add(("flip"),          tkr_iDrivers->webcam_config.flip);
  JBI->Add(("awb"),           tkr_iDrivers->webcam_config.awb);
  JBI->Add(("awb_gain"),      tkr_iDrivers->webcam_config.awb_gain);
  JBI->Add(("aec"),           tkr_iDrivers->webcam_config.aec);
  JBI->Add(("aec2"),          tkr_iDrivers->webcam_config.aec2);
  JBI->Add(("agc"),           tkr_iDrivers->webcam_config.agc);
  JBI->Add(("raw_gma"),       tkr_iDrivers->webcam_config.raw_gma);
  JBI->Add(("lenc"),          tkr_iDrivers->webcam_config.lenc);
  JBI->Add(("colorbar"),      tkr_iDrivers->webcam_config.colorbar);
  JBI->Add(("wpc"),           tkr_iDrivers->webcam_config.wpc);
  JBI->Add(("dcw"),           tkr_iDrivers->webcam_config.dcw);
  JBI->Add(("bpc"),           tkr_iDrivers->webcam_config.bpc);
  JBI->Add(("feature"),       tkr_iDrivers->webcam_config.feature);
  JBI->Add(("contrast"),      (int32_t)tkr_iDrivers->webcam_config.contrast - 2);
  JBI->Add(("brightness"),    (int32_t)tkr_iDrivers->webcam_config.brightness - 2);
  JBI->Add(("saturation"),    (int32_t)tkr_iDrivers->webcam_config.saturation - 2);
  JBI->Add(("resolution"),    tkr_iDrivers->webcam_config.resolution);

  // Webcam advanced config
  JBI->Add(("special_effect"), tkr_iDrivers->webcam_config2.special_effect);
  JBI->Add(("wb_mode"),        tkr_iDrivers->webcam_config2.wb_mode);
  JBI->Add(("aec_value"),      tkr_iDrivers->webcam_config2.aec_value);
  JBI->Add(("agc_gain"),       tkr_iDrivers->webcam_config2.agc_gain);
  JBI->Add(("gainceiling"),    tkr_iDrivers->webcam_config2.gainceiling);
  JBI->Add(("auth"),           tkr_iDrivers->webcam_config2.auth);

  // Additional
  JBI->Add(("clk"),            tkr_iDrivers->webcam_clk);
  JBI->Add(("rtsp"),           tkr_iDrivers->webcam_config.rtsp);

  // Optional runtime-only (not config) if applicable
  JBI->Add(("up"),             rt.up);
  JBI->Add(("width"),          rt.width);
  JBI->Add(("height"),         rt.height);



  return JBI->End();

}

void mCamera::Telemetry_Init()
{

  struct telemetry_handler<mCamera>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCamera::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//tkr_mqtt->GetIfChangedPeriod(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_IFCHANGED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCamera::ConstructJSON_State;
  telemetry_list.push_back(ptr);

}

#endif // ENABLE_DRIVERS_CAMERA