#ifndef _USE_MODULE_CORE_UPDATES_H
#define _USE_MODULE_CORE_UPDATES_H 0.3

#define D_UNIQUE_MODULE_CORE_UPDATES_ID 2014 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE_UPDATES

#include "1_TaskerManager/mTaskerInterface.h"


#ifdef ESP32
  #include <AsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#endif
#ifdef ESP8266
  #include <ESPAsyncTCP.h>
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  #include <ESPAsyncWebServer.h>
  #endif
#endif

extern void SafeMode_StartAndAwaitOTA(uint8_t seconds_to_wait = 0 /*default of zero, is indefinitely */);

// Methods for disable (returning from loop early) until an uptime, network established, or network uptime > x amount
#define DEBUG_OTA_FLASH_BLOCKER_UNTIL_STABLE_RETURN_ZERO()   if(tkr_time->RtcTime.seconds_nonreset < 120){ return 0; }
#define DEBUG_OTA_FLASH_BLOCKER_UNTIL_UPTIME_X_RETURN_ZERO(X)   if(tkr_time->RtcTime.seconds_nonreset < X){ return 0; }
// #define DEBUG_OTA_FLASH_BLOCKER_UNTIL_NETWORK_UPTIME_X_RETURN_ZERO(X)   if(tkr_time->RtcTime.seconds_nonreset < X){ return 0; }



class mUpdates :
  public mTaskerInterface
{

  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mUpdates(){};
    void Init(void){};
    void Pre_Init(void){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_CORE_UPDATES_CTR = D_MODULE_CORE_UPDATES_CTR;    
    PGM_P GetModuleName(){         return PM_MODULE_CORE_UPDATES_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_UPDATES_ID; }
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    bool update_reboot_pending = false;
    uint32_t update_reboot_at_ms = 0;

    bool update_upload_started = false;
    bool update_upload_failed = false;

    void HandleFirmwareUpload(
      AsyncWebServerRequest *request,
      String filename,
      size_t index,
      uint8_t *data,
      size_t len,
      bool final
    );

    void WebPage_Root_AddHandlers();
    #endif

    #ifdef USE_ARDUINO_OTA
      /*********************************************************************************************\
       * Allow updating via the Arduino OTA-protocol.
       *
       * - Once started disables current wifi clients and udp
       * - Perform restart when done to re-init wifi clients
      \*********************************************************************************************/

      bool arduino_ota_triggered = false;
      uint16_t arduino_ota_progress_dot_count = 0;
      bool ota_init_success = false;

      void ArduinoOTAInit(void);
      void ArduinoOtaLoop(void);
    #endif // USE_ARDUINO_OTA

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);
    
};

#endif

#endif