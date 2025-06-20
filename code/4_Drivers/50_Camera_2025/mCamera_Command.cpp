#include "mCamera.h"

#ifdef USE_MODULE_DRIVERS__CAMERA_2025


void mCamera::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok_sub = 0; 
  int8_t tmp_id = 0;

  if(jtok = obj["Cam"].getObject()["Init"])
  {
    Init();
  }
  if(jtok = obj["Cam"].getObject()["psramInit"])
  {
    psramInit();               // initialize PSRAM
        
    ALOG_INF(PSTR("psramFound: %d\n"), psramFound());
    ALOG_INF(PSTR("esp_spiram_is_initialized: %d\n"), esp_spiram_is_initialized());
    ALOG_INF(PSTR("Free PSRAM: %u\n"), ESP.getFreePsram());
  }

  JsonParserObject jobj = 0; 
  
  if(!(jobj = obj["Cam"].getObject()))
  {
    return;
  }

  if(jtok = jobj["Resolution"])
  {
    ALOG_INF(PSTR(D_LOG_CAMERA "Resolution %d"), jtok.getInt());
    CmndWebcamResolution(jtok.getInt());
  }
  if(jtok = jobj["Mirror"])
  {
    ALOG_INF(PSTR(D_LOG_CAMERA "Mirror %d"), jtok.getInt());
    CmndWebcamMirror(jtok.getBool());
  }
}
  
// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/

// #define D_PRFX_WEBCAM "WC"
// #define D_CMND_WC_STREAM "Stream"
// #define D_CMND_WC_RESOLUTION "Resolution"
// #define D_CMND_WC_MIRROR "Mirror"
// #define D_CMND_WC_FLIP "Flip"
// #define D_CMND_WC_SATURATION "Saturation"
// #define D_CMND_WC_BRIGHTNESS "Brightness"
// #define D_CMND_WC_CONTRAST "Contrast"
// #define D_CMND_WC_SPECIALEFFECT "SpecialEffect"

// #define D_CMND_WC_AWB "AWB"
// #define D_CMND_WC_WB_MODE "WBMode"
// #define D_CMND_WC_AWB_GAIN "AWBGain"

// #define D_CMND_WC_AEC "AEC"
// #define D_CMND_WC_AEC_VALUE "AECValue"
// #define D_CMND_WC_AE_LEVEL "AELevel"
// #define D_CMND_WC_AEC2 "AECDSP"

// #define D_CMND_WC_AGC "AGC"
// #define D_CMND_WC_AGC_GAIN "AGCGain"
// #define D_CMND_WC_GAINCEILING "GainCeiling"

// #define D_CMND_WC_RAW_GMA "GammaCorrect"
// #define D_CMND_WC_LENC "LensCorrect"

// #define D_CMND_WC_WPC "WPC"
// #define D_CMND_WC_DCW "DCW"
// #define D_CMND_WC_BPC "BPC"

// #define D_CMND_WC_COLORBAR "Colorbar"

// #define D_CMND_WC_FEATURE "Feature"
// #define D_CMND_WC_SETDEFAULTS "SetDefaults"
// #define D_CMND_WC_STATS "Stats"

// #define D_CMND_WC_INIT "Init"
// #define D_CMND_RTSP "Rtsp"

// #define D_CMND_WC_AUTH "Auth"
// #define D_CMND_WC_CLK "Clock"

// #define D_CMND_WC_STARTTASK "Starttask"
// #define D_CMND_WC_STOPTASK "Stoptask"

// #define D_CMND_WC_MENUVIDEODISABLE "MenuVideoDisable"

// // for testing to see what happens after cam_stop()
// #define D_CMND_WC_INTERRUPT "Interrupt"

// // mainly for testing functions which could be used by scripts.
// #define D_CMND_WC_SETMOTIONDETECT "Setmotiondetect"
// #define D_CMND_WC_GETFRAME "Getframe"
// #define D_CMND_WC_GETPICSTORE "Getpicstore"

// #define D_CMND_WC_BERRYFRAMES "Berryframes"

// #define D_CMND_WC_SAVEPIC "SavePic"
// #define D_CMND_WC_APPENDPIC "AppendPic"

// #define D_CMND_WC_GETMOTIONPIXELS "GetMotionPixels"

// #define D_CMND_WC_SETOPTIONS "SetOptions"
// #define D_CMND_WC_CONVERTFRAME "ConvertFrame"
// #define D_CMND_WC_SETPICTURE "SetPicture"

// #define D_CMND_WC_POWEROFF "Poweroff"

// const char kWCCommands[] PROGMEM =  D_PRFX_WEBCAM "|"  // Prefix
//   "|" D_CMND_WC_STREAM "|" D_CMND_WC_RESOLUTION "|" D_CMND_WC_MIRROR "|" D_CMND_WC_FLIP "|"
//   D_CMND_WC_SATURATION "|" D_CMND_WC_BRIGHTNESS "|" D_CMND_WC_CONTRAST "|" D_CMND_WC_SPECIALEFFECT "|"
//   D_CMND_WC_AWB "|" D_CMND_WC_WB_MODE "|" D_CMND_WC_AWB_GAIN "|" D_CMND_WC_AEC "|"
//   D_CMND_WC_AEC_VALUE "|" D_CMND_WC_AE_LEVEL "|" D_CMND_WC_AEC2 "|" D_CMND_WC_AGC "|"
//   D_CMND_WC_AGC_GAIN "|" D_CMND_WC_GAINCEILING "|" D_CMND_WC_RAW_GMA "|" D_CMND_WC_LENC "|"
//   D_CMND_WC_WPC "|" D_CMND_WC_DCW "|" D_CMND_WC_BPC "|" D_CMND_WC_COLORBAR "|" D_CMND_WC_FEATURE "|"
//   D_CMND_WC_SETDEFAULTS "|" D_CMND_WC_STATS "|" D_CMND_WC_INIT "|" D_CMND_WC_AUTH "|" D_CMND_WC_CLK "|" 
//   D_CMND_WC_STARTTASK "|" D_CMND_WC_STOPTASK "|" D_CMND_WC_MENUVIDEODISABLE "|" 
//   D_CMND_WC_INTERRUPT "|" D_CMND_WC_GETFRAME "|" D_CMND_WC_GETPICSTORE "|" 
// #ifdef USE_WEBCAM_MOTION
//   D_CMND_WC_SETMOTIONDETECT "|" D_CMND_WC_GETMOTIONPIXELS "|"
//   D_CMND_WC_CONVERTFRAME "|" D_CMND_WC_SETPICTURE "|"  
// #endif
//   D_CMND_WC_BERRYFRAMES  "|" D_CMND_WC_SAVEPIC "|" D_CMND_WC_APPENDPIC  "|" 
//   D_CMND_WC_SETOPTIONS "|" D_CMND_WC_POWEROFF

// #ifdef ENABLE_RTSPSERVER
//   "|" D_CMND_RTSP
// #endif // ENABLE_RTSPSERVER
//   ;

// void (* const WCCommand[])(void) PROGMEM = {
//   &CmndWebcam, &CmndWebcamStream, &CmndWebcamResolution, &CmndWebcamMirror, &CmndWebcamFlip,
//   &CmndWebcamSaturation, &CmndWebcamBrightness, &CmndWebcamContrast, &CmndWebcamSpecialEffect,
//   &CmndWebcamAWB, &CmndWebcamWBMode, &CmndWebcamAWBGain, &CmndWebcamAEC, &CmndWebcamAECValue,
//   &CmndWebcamAELevel, &CmndWebcamAEC2, &CmndWebcamAGC, &CmndWebcamAGCGain, &CmndWebcamGainCeiling,
//   &CmndWebcamGammaCorrect, &CmndWebcamLensCorrect, &CmndWebcamWPC, &CmndWebcamDCW, &CmndWebcamBPC,
//   &CmndWebcamColorbar, &CmndWebcamFeature, &CmndWebcamSetDefaults,
//   &CmndWebcamStats, &CmndWebcamInit, &CmndWebcamAuth, &CmndWebcamClock,
//   &CmndWebcamStartTask, &CmndWebcamStopTask, &CmndWebcamMenuVideoDisable,
//   &CmndWebcamCamStartStop, &CmndWebcamGetFrame, &CmndWebcamGetPicStore,
// #ifdef USE_WEBCAM_MOTION
//   &CmndWebcamSetMotionDetect, &CmndWebcamGetMotionPixels, 
//   &CmndWebcamConvertFrame, &CmndWebcamSetPicture,
// #endif
//   &CmndWebcamBerryFrames,
//   &CmdWebcamSavePic, &CmdWebcamAppendPic,
//   &CmndWebcamSetOptions,
//   &CmndWebcamPowerOff

// #ifdef ENABLE_RTSPSERVER
//   , &CmndWebRtsp
// #endif // ENABLE_RTSPSERVER
//   };

// // plain 'WC' gives config
// void CmndWebcam(void) {
//   Response_P(PSTR("{\"" D_PRFX_WEBCAM "\":{\"" D_CMND_WC_STREAM "\":%d,\"" D_CMND_WC_RESOLUTION "\":%d,\"" D_CMND_WC_MIRROR "\":%d,\""
//     D_CMND_WC_FLIP "\":%d,\""
//     D_CMND_WC_SATURATION "\":%d,\"" D_CMND_WC_BRIGHTNESS "\":%d,\"" D_CMND_WC_CONTRAST "\":%d,\""
//     D_CMND_WC_SPECIALEFFECT "\":%d,\"" D_CMND_WC_AWB  "\":%d,\"" D_CMND_WC_WB_MODE "\":%d,\""
//     D_CMND_WC_AWB_GAIN "\":%d,\"" D_CMND_WC_AEC "\":%d,\"" D_CMND_WC_AEC_VALUE "\":%d,\""
//     D_CMND_WC_AE_LEVEL "\":%d,\"" D_CMND_WC_AEC2 "\":%d,\"" D_CMND_WC_AGC "\":%d,\""
//     D_CMND_WC_AGC_GAIN "\":%d,\"" D_CMND_WC_GAINCEILING "\":%d,\"" D_CMND_WC_RAW_GMA "\":%d,\""
//     D_CMND_WC_LENC "\":%d,\"" D_CMND_WC_WPC "\":%d,\"" D_CMND_WC_DCW "\":%d,\"" D_CMND_WC_BPC "\":%d,\""
//     D_CMND_WC_COLORBAR "\":%d,\"" D_CMND_WC_FEATURE "\":%d,\"" D_CMND_WC_AUTH "\":%d,\"" D_CMND_WC_CLK "\":%d,\""
//     D_CMND_WC_MENUVIDEODISABLE "\":%d"
// #ifdef ENABLE_RTSPSERVER
//   ",\"" D_CMND_RTSP "\":%d"
// #endif // ENABLE_RTSPSERVER
//   "}}"),
//     tkr_set->Settings.webcam_config.stream, tkr_set->Settings.webcam_config.resolution, tkr_set->Settings.webcam_config.mirror,
//     tkr_set->Settings.webcam_config.flip,
//     tkr_set->Settings.webcam_config.saturation -2, tkr_set->Settings.webcam_config.brightness -2, tkr_set->Settings.webcam_config.contrast -2,
//     tkr_set->Settings.webcam_config2.special_effect, tkr_set->Settings.webcam_config.awb, tkr_set->Settings.webcam_config2.wb_mode,
//     tkr_set->Settings.webcam_config.awb_gain, tkr_set->Settings.webcam_config.aec, tkr_set->Settings.webcam_config2.aec_value,
//     tkr_set->Settings.webcam_config2.ae_level -2, tkr_set->Settings.webcam_config.aec2, tkr_set->Settings.webcam_config.agc,
//     tkr_set->Settings.webcam_config2.agc_gain, tkr_set->Settings.webcam_config2.gainceiling, tkr_set->Settings.webcam_config.raw_gma,
//     tkr_set->Settings.webcam_config.lenc, tkr_set->Settings.webcam_config.wpc, tkr_set->Settings.webcam_config.dcw, tkr_set->Settings.webcam_config.bpc,
//     tkr_set->Settings.webcam_config.colorbar, tkr_set->Settings.webcam_config.feature, tkr_set->Settings.webcam_config2.auth,
//     tkr_set->Settings.webcam_clk, tkr_set->Settings.webcam_config.spare15
// #ifdef ENABLE_RTSPSERVER
//   , tkr_set->Settings.webcam_config.rtsp
// #endif // ENABLE_RTSPSERVER
//   );
// }

// // enable/disable informing Berry of frame arriving
// void CmndWebcamBerryFrames(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_camera->Wc.berryFrames = XdrvMailbox.payload;
//   }
//   ResponseCmndStateText(tkr_camera->Wc.berryFrames);
// }

// void CmndWebcamPowerOff(void){
//   WcCamOff();
//   ResponseCmndDone();
// }

// void CmndWebcamSetOptions(void){
//   int res = WcSetOptions(XdrvMailbox.index, XdrvMailbox.payload);
//   ResponseCmndNumber(res);
// }


// void CmndWebcamStartTask(void) {
//   if (tkr_camera->Wc.taskRunning == 0){
//     WCStartOperationTask();
//   }
//   ResponseCmndDone();
// }

// void WcStopTask(void){
//   if (tkr_camera->Wc.taskRunning == 1){
//     // set to 2, and wait until cleared
//     WcWaitZero(&tkr_camera->Wc.taskRunning, 2, 20000);
//     if (tkr_camera->Wc.taskHandle){
//       // why does this cause a problem?
//       //vTaskDelete(tkr_camera->Wc.taskHandle);
//       tkr_camera->Wc.taskHandle = nullptr;
//     }
//   }
// }

// void CmndWebcamStopTask(void) {
//   WcStopTask();
//   ResponseCmndDone();
// }


// // store a frame 1-4.  If frame '0' is requested, stores a frame in '1'
// void CmndWebcamGetFrame(void) {
//   // returns size
//   int bnum = 0;
//   if (XdrvMailbox.index){
//     bnum = XdrvMailbox.index;
//   } else {
//     bnum = XdrvMailbox.payload;
//   }
//   if (bnum == 0) bnum = 1;
//   uint32_t res = WcGetFrame(bnum);
//   struct PICSTORE *p = nullptr;
//   res = WcGetPicstorePtr(bnum-1, &p);
//   char resp[100] = "0";
//   if (p) {
//     snprintf_P(resp, sizeof(resp), PSTR("{\"buff\":%d,\"addr\":%d,\"len\":%d,\"w\":%d,\"h\":%d,\"format\":%d}"), 
//       bnum, p->buff, p->len, p->width, p->height, p->format+1);
//   } else {
//     snprintf_P(resp, sizeof(resp), PSTR("{\"maxstore\":%d}"), 
//       res);
//   }
//   Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, resp);

// #ifdef DEBUG_DRIVERS__CAMERA_2025  
//   ALOG_DBG(PSTR(D_LOG_CAMERA "Getframe %d -> %d"), bnum, res);
// #endif
//   ResponseCmndNumber((int)res);
// }

// // get a stored frame.  If frame '0' is requested, stores a frame in '1', and returns that
// void CmndWebcamGetPicStore(void) {
//   // returns size
//   // use a dummy for buffer ptr
//   uint8_t *t = nullptr;
//   int bnum = XdrvMailbox.payload;
//   if (bnum == -99){
//     bnum = XdrvMailbox.index;
//   }
//   if (bnum < 0 || bnum > MAX_PICSTORE) {
//     return;  // Command Error
//   }

//   // if given 0, then get frame 1 first, and use frame 1 (the first frame, index 0).
//   if (bnum == 0){
//     bnum = 1;
//     uint32_t res = WcGetFrame(bnum);
//   }
//   //uint32_t res = WcGetPicstore(bnum-1, &t);
//   struct PICSTORE *p = nullptr;
//   uint32_t res = WcGetPicstorePtr(bnum-1, &p);
// #ifdef DEBUG_DRIVERS__CAMERA_2025  
//   ALOG_DBG(PSTR(D_LOG_CAMERA "PicStore %d at 0x%x"), bnum, p);
// #endif
//   char resp[100] = "0";
//   if (p) {
//     snprintf_P(resp, sizeof(resp), PSTR("{\"buff\":%d,\"addr\":%d,\"len\":%d,\"w\":%d,\"h\":%d,\"format\":%d}"), 
//       bnum, p->buff, p->len, p->width, p->height, p->format+1);
//   } else {
//     snprintf_P(resp, sizeof(resp), PSTR("{\"maxstore\":%d}"), 
//       res);
//   }
//   Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, resp);
// }

// int WebcamSavePic(int append) {
//   // returns size
//   // use a dummy for buffer ptr
//   uint8_t *buf = nullptr;
//   int len = 0;
//   int bnum = XdrvMailbox.index;
//   if (bnum < 0){
// #ifdef DEBUG_DRIVERS__CAMERA_2025  
//     ALOG_DBG(PSTR(D_LOG_CAMERA "savePic bnum %d"), bnum);
// #endif
//     return 0;
//   }

//   if (0 == XdrvMailbox.data_len){
//     ALOG_DBG(PSTR(D_LOG_CAMERA "Save Pic no fname"));
//     return 0;
//   }

//   if (bnum == 0){
//     // if bnum 0, get a picture now....
//     // use the snapshot frame buffer
//     tkr_camera->Wc.taskTakePic = 1;
//     int loops = 100;
//     while (tkr_camera->Wc.taskTakePic && (loops--)){
//       vTaskDelay(10/ portTICK_PERIOD_MS);
//     }
//   }
//   mSupport::TasAutoMutex localmutex(&WebcamMutex, "SavePic", 2000);
//   if (bnum == 0){
//     buf = tkr_camera->Wc.snapshotStore.buff;
//     len = tkr_camera->Wc.snapshotStore.len;
// #ifdef DEBUG_DRIVERS__CAMERA_2025  
//     ALOG_DBG(PSTR(D_LOG_CAMERA "savePic snapshotstore %d"), len);
// #endif    
//   } else {
//     len = WcGetPicstore(bnum - 1, &buf);
// #ifdef DEBUG_DRIVERS__CAMERA_2025  
//     ALOG_DBG(PSTR(D_LOG_CAMERA "savePic PicStore %d -> %d"), bnum, len);
// #endif
//   }
//   if (len){
// #ifdef USE_UFILESYS
//     // ffsp should recognise /sd/ if sd card present
//     if (ffsp){
//       File f = ffsp->open(XdrvMailbox.data, append?FS_FILE_APPEND:FS_FILE_WRITE); //
//       if (f){
//         f.write(buf, len);
//         f.close();
// #ifdef DEBUG_DRIVERS__CAMERA_2025  
//         ALOG_DBG(PSTR(D_LOG_CAMERA "Save Pic %s"), XdrvMailbox.data);
// #endif        
//         if (bnum == 0){
//           pic_free(&tkr_camera->Wc.snapshotStore);
//         }
//         return 1;
//       }
//     }
//     ALOG_DBG(PSTR(D_LOG_CAMERA "Failed Save Pic %s"), XdrvMailbox.data);
// #else
//     ALOG_DBG(PSTR(D_LOG_CAMERA "Failed Save Pic %s - no USE_UFILESYS"), XdrvMailbox.data);
// #endif
//     if (bnum == 0){
//       pic_free(&tkr_camera->Wc.snapshotStore);
//     }
//     return 0;
//   } 
//   ALOG_DBG(PSTR(D_LOG_CAMERA "Failed Save Pic inv index %d"), XdrvMailbox.payload);
//   return 0;
// }
// // "WCSAVEPIC1 /temp.jpg" "WCSAVEPIC2 /temp.jpg"
// void CmdWebcamSavePic(){
//   if (WebcamSavePic(0)) {
//     ResponseCmndDone();
//   }    
// }
// // "WCAPPENDPIC1 /temp.jpg" "WCAPPENDPIC2 /temp.jpg"
// void CmdWebcamAppendPic(){
//   if (WebcamSavePic(1)) {
//     ResponseCmndDone();
//   }
// }

// void CmndWebcamMenuVideoDisable(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.spare15 = XdrvMailbox.payload;
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.spare15);
// }

// void CmndWebcamStream(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.stream = XdrvMailbox.payload;
//     WcSetStreamserver(tkr_set->Settings.webcam_config.stream);
//   } else {
//     // we use this from a menu
//     if (XdrvMailbox.index == 2){
//       tkr_set->Settings.webcam_config.stream = 1;
//       WcSetStreamserver(tkr_set->Settings.webcam_config.stream);
//     }
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.stream);
// }


void mCamera::CmndWebcamResolution(uint8_t resolution) 
{

    int8_t reinit = 0;
    
    tkr_set->Settings.webcam_config.resolution = resolution;
    if (reinit) {
      WcSetup(tkr_set->Settings.webcam_config.resolution);
    } else {
      WcSetOptions(0, tkr_set->Settings.webcam_config.resolution);
    }
    
    
}

void mCamera::CmndWebcamMirror(bool mirror) 
{
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
    tkr_set->Settings.webcam_config.mirror = mirror;
    WcSetOptions(3, tkr_set->Settings.webcam_config.mirror);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.mirror);
}

// void CmndWebcamFlip(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.flip = XdrvMailbox.payload;
//     WcSetOptions(2, tkr_set->Settings.webcam_config.flip);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.flip);
// }

// void CmndWebcamSaturation(void) {
//   if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
//     tkr_set->Settings.webcam_config.saturation = XdrvMailbox.payload +2;
//     WcSetOptions(6, tkr_set->Settings.webcam_config.saturation -2);
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_config.saturation -2);
// }

// void CmndWebcamBrightness(void) {
//   if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
//     tkr_set->Settings.webcam_config.brightness = XdrvMailbox.payload +2;
//     WcSetOptions(5, tkr_set->Settings.webcam_config.brightness -2);
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_config.brightness -2);
// }

// void CmndWebcamContrast(void) {
//   if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
//     tkr_set->Settings.webcam_config.contrast = XdrvMailbox.payload +2;
//     WcSetOptions(4, tkr_set->Settings.webcam_config.contrast -2);
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_config.contrast -2);
// }

// void CmndWebcamSpecialEffect(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
//     tkr_set->Settings.webcam_config2.special_effect = XdrvMailbox.payload;
//     WcSetOptions(1, tkr_set->Settings.webcam_config2.special_effect);
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_config2.special_effect);
// }

// void CmndWebcamAWB(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.awb = XdrvMailbox.payload;
//     WcSetOptions(7, tkr_set->Settings.webcam_config.awb);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.awb);
// }

// void CmndWebcamWBMode(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 4)) {
//     tkr_set->Settings.webcam_config2.wb_mode = XdrvMailbox.payload;
//     WcSetOptions(8, tkr_set->Settings.webcam_config2.wb_mode);
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_config2.wb_mode);
// }

// void CmndWebcamAWBGain(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.awb_gain = XdrvMailbox.payload;
//     WcSetOptions(9, tkr_set->Settings.webcam_config.awb_gain);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.awb_gain);
// }

// void CmndWebcamAEC(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.aec = XdrvMailbox.payload;
//     WcSetOptions(10, tkr_set->Settings.webcam_config.aec);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.aec);
// }

// void CmndWebcamAECValue(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1200)) {
//     tkr_set->Settings.webcam_config2.aec_value = XdrvMailbox.payload;
//     WcSetOptions(11, tkr_set->Settings.webcam_config2.aec_value);
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_config2.aec_value);
// }

// void CmndWebcamAELevel(void) {
//   if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
//     tkr_set->Settings.webcam_config2.ae_level = XdrvMailbox.payload + 2;
//     WcSetOptions(12, tkr_set->Settings.webcam_config2.ae_level - 2);
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_config2.ae_level - 2);
// }

// void CmndWebcamAEC2(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.aec2 = XdrvMailbox.payload;
//     WcSetOptions(13, tkr_set->Settings.webcam_config.aec2);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.aec2);
// }

// void CmndWebcamAGC(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.agc = XdrvMailbox.payload;
//     WcSetOptions(14, tkr_set->Settings.webcam_config.agc);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.agc);
// }

// void CmndWebcamAGCGain(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 30)) {
//     tkr_set->Settings.webcam_config2.agc_gain = XdrvMailbox.payload;
//     WcSetOptions(15, tkr_set->Settings.webcam_config2.agc_gain);
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_config2.agc_gain);
// }

// void CmndWebcamGainCeiling(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
//     tkr_set->Settings.webcam_config2.gainceiling = XdrvMailbox.payload;
//     WcSetOptions(16, tkr_set->Settings.webcam_config2.gainceiling);
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_config2.gainceiling);
// }

// void CmndWebcamGammaCorrect(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.raw_gma = XdrvMailbox.payload;
//     WcSetOptions(17, tkr_set->Settings.webcam_config.raw_gma);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.raw_gma);
// }

// void CmndWebcamLensCorrect(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.lenc = XdrvMailbox.payload;
//     WcSetOptions(18, tkr_set->Settings.webcam_config.lenc);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.lenc);
// }

// void CmndWebcamWPC(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.wpc = XdrvMailbox.payload;
//     WcSetOptions(19, tkr_set->Settings.webcam_config.wpc);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.wpc);
// }

// void CmndWebcamDCW(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.dcw = XdrvMailbox.payload;
//     WcSetOptions(20, tkr_set->Settings.webcam_config.dcw);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.dcw);
// }

// void CmndWebcamBPC(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.bpc = XdrvMailbox.payload;
//     WcSetOptions(21, tkr_set->Settings.webcam_config.bpc);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.bpc);
// }

// void CmndWebcamColorbar(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     tkr_set->Settings.webcam_config.colorbar = XdrvMailbox.payload;
//     WcSetOptions(22, tkr_set->Settings.webcam_config.colorbar);
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.colorbar);
// }

// void CmndWebcamFeature(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
//     tkr_set->Settings.webcam_config.feature = XdrvMailbox.payload;
//     WcSetOptions(23, tkr_set->Settings.webcam_config.feature);
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_config.feature);
// }

// void CmndWebcamAuth(void){
//   if((XdrvMailbox.payload >=0) && (XdrvMailbox.payload <= 1)){
//     tkr_set->Settings.webcam_config2.auth = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_config2.auth);
// }

// void CmndWebcamClock(void){
//   if((XdrvMailbox.payload >= 10) && (XdrvMailbox.payload <= 200)){
//     tkr_set->Settings.webcam_clk = XdrvMailbox.payload;
//     // if cam is up, must setup to apply
//     if (tkr_camera->Wc.up){
//       WcSetup(tkr_set->Settings.webcam_config.resolution);
//     }
//   }
//   ResponseCmndNumber(tkr_set->Settings.webcam_clk);
// }

// void CmndWebcamCamStartStop(void){
//   if((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)){
//     WcInterrupt(XdrvMailbox.payload);
//   }
//   ResponseCmndNumber(XdrvMailbox.payload);
// }


// void CmndWebcamInit(void) {
//   WcSetup(tkr_set->Settings.webcam_config.resolution);
//   WcSetStreamserver(tkr_set->Settings.webcam_config.stream);
//   ResponseCmndDone();
// }

// void CmndWebcamSetDefaults(void) {
//   WcSetDefaults(0);
//   ResponseCmndDone();
// }

// #define D_WEBCAM_STATS_FPS "FPS"
// #define D_WEBCAM_STATS_CAMFAIL "CamFail"
// #define D_WEBCAM_STATS_JPEGFAIL "JpegFail"
// #define D_WEBCAM_STATS_CLIENTFAIL "ClientFail"
// #define D_WEBCAM_STATS_AVGFPS "AvgFPS"
// #define D_WEBCAM_STATS_AVGPERIOD "AvgFPeriod"
// #define D_WEBCAM_STATS_AVGPROC "AvgFProcessing"
// #define D_WEBCAM_STATS_FRAMELEN "LastFrameBytes"
// #define D_WEBCAM_STATS_WEBCLIENTS "WebClients"
// #define D_WEBCAM_STATS_RTSPCLIENTS "RTSPClients"
// #define D_WEBCAM_STATS_LASTCAMINTERVAL "CamInterval"
// #define D_WEBCAM_STATS_CAMFRAMETIME "CamFrameTime"
// #define D_WEBCAM_STATS_LOOPSPERSEC "LoopsPerSec"

// void CmndWebcamStats(void) {
//   Response_P(PSTR("{\"" D_PRFX_WEBCAM D_CMND_WC_STATS "\":{\"" D_WEBCAM_STATS_FPS "\":%d,\""
//     D_WEBCAM_STATS_CAMFAIL "\":%d,\"" D_WEBCAM_STATS_JPEGFAIL "\":%d,\"" D_WEBCAM_STATS_CLIENTFAIL "\":%d,\""
//     D_WEBCAM_STATS_AVGFPS "\":%f,\""
//     D_WEBCAM_STATS_AVGPERIOD "\":%d,\""
//     D_WEBCAM_STATS_AVGPROC "\":%d,\""
//     D_WEBCAM_STATS_FRAMELEN "\":%d,\""
//     D_WEBCAM_STATS_WEBCLIENTS "\":%d,\""
// #ifdef ENABLE_RTSPSERVER
//     D_WEBCAM_STATS_RTSPCLIENTS "\":%d,\""
// #endif    
//     D_WEBCAM_STATS_LASTCAMINTERVAL "\":%d,\""
//     D_WEBCAM_STATS_CAMFRAMETIME "\":%d,\""
//     D_WEBCAM_STATS_LOOPSPERSEC "\":%d"
//   "}}"),
//   tkr_camera->WcStats.camfps, tkr_camera->WcStats.camfail, tkr_camera->WcStats.jpegfail, tkr_camera->WcStats.clientfail,
//   tkr_camera->WcStats.avgFPS, tkr_camera->WcStats.avgFrameMS, tkr_camera->WcStats.avgProcessingPerFrameMS,
//   tkr_camera->Wc.last_frame_len,
//   tkr_camera->WcStats.webclientcount,
// #ifdef ENABLE_RTSPSERVER
//   tkr_camera->WcStats.rtspclientcount,
// #endif  
//   tkr_camera->Wc.camtimediff,
//   tkr_camera->Wc.frameIntervalsus,
//   tkr_camera->Wc.loopspersec
//   );
// }

// #ifdef ENABLE_RTSPSERVER
// void CmndWebRtsp(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     if (tkr_set->Settings.webcam_config.rtsp != XdrvMailbox.payload){
//       tkr_set->Settings.webcam_config.rtsp = XdrvMailbox.payload;
//       if (!tkr_set->Settings.webcam_config.rtsp){
//         // existing sessions will get stopped, and the server deleted in the loop.
//       }
//     }
//   }
//   ResponseCmndStateText(tkr_set->Settings.webcam_config.rtsp);
// }
// #endif // ENABLE_RTSPSERVER

#endif
