#include "mCamera.h"

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
// #define D_CMND_WC_INIT "Init"
// #define D_CMND_RTSP "Rtsp"

// const char kWCCommands[] PROGMEM =  D_PRFX_WEBCAM "|"  // Prefix
//   "|" D_CMND_WC_STREAM "|" D_CMND_WC_RESOLUTION "|" D_CMND_WC_MIRROR "|" D_CMND_WC_FLIP "|"
//   D_CMND_WC_SATURATION "|" D_CMND_WC_BRIGHTNESS "|" D_CMND_WC_CONTRAST "|" D_CMND_WC_INIT
// #ifdef ENABLE_RTSPSERVER
//   "|" D_CMND_RTSP
// #endif // ENABLE_RTSPSERVER
//   ;

// void (* const WCCommand[])(void) PROGMEM = {
//   &CmndWebcam, &CmndWebcamStream, &CmndWebcamResolution, &CmndWebcamMirror, &CmndWebcamFlip,
//   &CmndWebcamSaturation, &CmndWebcamBrightness, &CmndWebcamContrast, &CmndWebcamInit
// #ifdef ENABLE_RTSPSERVER
//   , &CmndWebRtsp
// #endif // ENABLE_RTSPSERVER
//   };

// void CmndWebcam(void) {
//   Response_P(PSTR("{\"" D_PRFX_WEBCAM "\":{\"" D_CMND_WC_STREAM "\":%d,\"" D_CMND_WC_RESOLUTION "\":%d,\"" D_CMND_WC_MIRROR "\":%d,\""
//     D_CMND_WC_FLIP "\":%d,\""
//     D_CMND_WC_SATURATION "\":%d,\"" D_CMND_WC_BRIGHTNESS "\":%d,\"" D_CMND_WC_CONTRAST "\":%d"
// #ifdef ENABLE_RTSPSERVER
//   ",\"" D_CMND_RTSP "\":%d"
// #endif // ENABLE_RTSPSERVER
//   "}}"),
//     pCONT_set->Settings.webcam_config.stream, pCONT_set->Settings.webcam_config.resolution, pCONT_set->Settings.webcam_config.mirror,
//     pCONT_set->Settings.webcam_config.flip,
//     pCONT_set->Settings.webcam_config.saturation -2, pCONT_set->Settings.webcam_config.brightness -2, pCONT_set->Settings.webcam_config.contrast -2
// #ifdef ENABLE_RTSPSERVER
//   , pCONT_set->Settings.webcam_config.rtsp
// #endif // ENABLE_RTSPSERVER
//   );
// }

// void CmndWebcamStream(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     pCONT_set->Settings.webcam_config.stream = XdrvMailbox.payload;
//     if (!pCONT_set->Settings.webcam_config.stream) { WcStreamControl(); }  // Stop stream
//   }
//   ResponseCmndStateText(pCONT_set->Settings.webcam_config.stream);
// }

// void CmndWebcamResolution(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < FRAMESIZE_FHD)) {
//     pCONT_set->Settings.webcam_config.resolution = XdrvMailbox.payload;
//     WcSetOptions(0, pCONT_set->Settings.webcam_config.resolution);
//   }
//   ResponseCmndNumber(pCONT_set->Settings.webcam_config.resolution);
// }

// void CmndWebcamMirror(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     pCONT_set->Settings.webcam_config.mirror = XdrvMailbox.payload;
//     WcSetOptions(3, pCONT_set->Settings.webcam_config.mirror);
//   }
//   ResponseCmndStateText(pCONT_set->Settings.webcam_config.mirror);
// }

// void CmndWebcamFlip(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     pCONT_set->Settings.webcam_config.flip = XdrvMailbox.payload;
//     WcSetOptions(2, pCONT_set->Settings.webcam_config.flip);
//   }
//   ResponseCmndStateText(pCONT_set->Settings.webcam_config.flip);
// }

// void CmndWebcamSaturation(void) {
//   if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
//     pCONT_set->Settings.webcam_config.saturation = XdrvMailbox.payload +2;
//     WcSetOptions(6, pCONT_set->Settings.webcam_config.saturation -2);
//   }
//   ResponseCmndNumber(pCONT_set->Settings.webcam_config.saturation -2);
// }

// void CmndWebcamBrightness(void) {
//   if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
//     pCONT_set->Settings.webcam_config.brightness = XdrvMailbox.payload +2;
//     WcSetOptions(5, pCONT_set->Settings.webcam_config.brightness -2);
//   }
//   ResponseCmndNumber(pCONT_set->Settings.webcam_config.brightness -2);
// }

// void CmndWebcamContrast(void) {
//   if ((XdrvMailbox.payload >= -2) && (XdrvMailbox.payload <= 2)) {
//     pCONT_set->Settings.webcam_config.contrast = XdrvMailbox.payload +2;
//     WcSetOptions(4, pCONT_set->Settings.webcam_config.contrast -2);
//   }
//   ResponseCmndNumber(pCONT_set->Settings.webcam_config.contrast -2);
// }

// void CmndWebcamInit(void) {
//   WcStreamControl();
//   ResponseCmndDone();
// }

// #ifdef ENABLE_RTSPSERVER
// void CmndWebRtsp(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     pCONT_set->Settings.webcam_config.rtsp = XdrvMailbox.payload;
//     TasmotaGlobal.restart_flag = 2;
//   }
//   ResponseCmndStateText(pCONT_set->Settings.webcam_config.rtsp);
// }
// #endif // ENABLE_RTSPSERVER