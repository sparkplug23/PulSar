#ifndef _CELLULAR_H
#define _CELLULAR_H

#define D_UNIQUE_MODULE__NETWORK_CELLULAR__ID  3005 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_CELLULAR

// https://shop.marcomweb.it/media/kunena/attachments/42/wm02_an60_atc_timeouts_v05.pdf

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

// -------------------------------------------------------------------------------------------------
// Modem selection (compile-time)
// Exactly one should be true in a given build configuration.
// -------------------------------------------------------------------------------------------------

#if defined(USE_MODULE_DRIVERS_MODEM_7000G)
  #define tkr_modem tkr_modem_sim7000g
#elif defined(USE_MODULE_DRIVERS_MODEM_800L)
  #define tkr_modem tkr_sim800l
#else
  #error "No modem selected. Define USE_MODULE_DRIVERS_MODEM_7000G or USE_MODULE_DRIVERS_MODEM_800L."
#endif



#define TINY_GSM_MODEM_SIM7000

// debug inside library
#define TINY_GSM_DEBUG Serial

#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
// #define SerialAT Serial1

// See all AT commands, if wanted
#define DUMP_AT_COMMANDS

/*
   Tests enabled
*/
#define TINY_GSM_TEST_GPRS    true
#define TINY_GSM_TEST_GPS     true
#define TINY_GSM_POWERDOWN    true

// set GSM PIN, if any
#define GSM_PIN ""

#include <TinyGsmClient.h>
#include <SPI.h>
#include <SD.h>
#include <Ticker.h>

#include <CStringWriter.h>

#ifdef ESP32
#include <driver/adc.h>
#endif

#define uS_TO_S_FACTOR 1000000ULL  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  60          // Time ESP32 will go to sleep (in seconds)

// #define UART_CELLULAR_BAUD   921600
// #define PIN_DTR     25
// #define PIN_TX      27
// #define PIN_RX      26
// #define PWR_PIN     4

// #define SD_MISO     2
// #define SD_MOSI     15
// #define SD_SCLK     14
// #define SD_CS       13
// #define LED_PIN     12

#define DEFAULT_AT_COMMAND_RESPONSE_WAIT 3000

#define AT_COMMAND_RESPONSE_TIMEOUT__CFUN 1000
#define AT_COMMAND_RESPONSE_TIMEOUT__CNMI 1000


//https://cplusplus.com/reference/cstdio/sscanf/
//even though celullar here can report GPS, GPS should be rolled into driver/sensor? probably sensor, since its location data

//https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html

// https://www.google.co.uk/maps/place/@52.410555,-0.5150276,12/ 17

// Or you can use OpenStreetMap directly in a Frame widget with a pin like this —

// https://www.openstreetmap.org/?mlat=54.02227&mlon=-0.66649#map=19/54.02227/-0.66649


  //https://github.com/vshymanskyy/TinyGSM/pull/260/files#diff-49f12f4a048fa1f63d160e1adb91526d97e2a16cd3ba3898525ac5d1a44ddb99

// #include <StreamDebugger.h>

class mCellular :
  public mTaskerInterface
{

  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mCellular(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE__NETWORK_CELLULAR__CTR = D_MODULE__NETWORK_CELLULAR__CTR;
    PGM_P GetModuleName(){          return PM_MODULE__NETWORK_CELLULAR__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__NETWORK_CELLULAR__ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/


    // -------------------------------------------------------------------------------------------------
    // Cellular policy state machine (1Hz tick)
    // -------------------------------------------------------------------------------------------------
    enum class cellular_conn_state_t : uint8_t
    {
      WAIT_MODEM_READY = 0,
      INIT_CONFIG,
      START_CONNECTION,
      ONLINE,
      BACKOFF
    };

    struct cellular_conn_sm_t
    {
      cellular_conn_state_t state = cellular_conn_state_t::WAIT_MODEM_READY;

      uint32_t t_enter_ms = 0;
      uint32_t t_next_action_ms = 0;

      uint8_t  attempts = 0;
      uint8_t gprs_drop_count = 0;

      bool     init_config_done = false;

      // Optional: track last known link state to detect transitions cleanly
      bool     last_gprs_connected = false;
    };

    cellular_conn_sm_t conn_sm_;

    static inline void _conn_enter(cellular_conn_sm_t& sm, cellular_conn_state_t st, uint32_t now_ms)
    {
      sm.state = st;
      sm.t_enter_ms = now_ms;
    }

    void Cellular_ConnMgr_Reset();
    void Cellular_ConnMgr_Tick_1s(uint32_t now_ms);
    void Cellular_ConnMgr_LogStatus_30s(void);

    bool Cellular_HasExternalConnectivity(void);
    Client* Cellular_GetNetworkClient(void);


    void parse_JSONCommand(JsonParserObject obj);


    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
      
    void Telemetry_Init();
    std::vector<struct telemetry_handler<mCellular>*> telemetry_list;    
    struct telemetry_handler<mCellular> telemetry_settings;
    struct telemetry_handler<mCellular> telemetry_state_ifchanged;

};


#endif //  USE_MODULE_NETWORK_CELLULAR

#endif //  _CELLULAR_H