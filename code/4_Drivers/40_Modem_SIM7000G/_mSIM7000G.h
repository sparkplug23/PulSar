#ifndef _USE_MODULE_DRIVERS_MODEM_7000G_H
#define _USE_MODULE_DRIVERS_MODEM_7000G_H

#define D_UNIQUE_MODULE_DRIVERS_MODEM_7000G_ID 4080 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_MODEM_7000G


// https://shop.marcomweb.it/media/kunena/attachments/42/wm02_an60_atc_timeouts_v05.pdf

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

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

#ifndef UART_CELLULAR_BAUD
#define UART_CELLULAR_BAUD 115200
#endif

static constexpr uint32_t MODEM_TARGET_BAUD          = UART_CELLULAR_BAUD;

static constexpr uint32_t MODEM_BOOT_WAIT_MS         = 8000;   // SIM7000 needs real boot time
static constexpr uint32_t MODEM_PWRKEY_HOLD_MS       = 1200;
static constexpr uint32_t MODEM_PWRKEY_SETTLE_MS     = 500;

static constexpr uint32_t UART_SETTLE_MS             = 200;
static constexpr uint32_t UART_REOPEN_DELAY_MS       = 250;
static constexpr uint32_t AT_STREAM_TIMEOUT_MS       = 500;

static constexpr uint16_t AT_PROBE_TIMEOUT_FAST_MS   = 350;
static constexpr uint16_t AT_PROBE_TIMEOUT_SLOW_MS   = 1000;
static constexpr uint16_t AT_CFG_TIMEOUT_MS          = 1000;



#define DEFAULT_AT_COMMAND_RESPONSE_WAIT 3000

#define AT_COMMAND_RESPONSE_TIMEOUT__CFUN 1000
#define AT_COMMAND_RESPONSE_TIMEOUT__CNMI 1000

#include "5_Sensors/00_Interface/custom_types.h"


//https://cplusplus.com/reference/cstdio/sscanf/
//even though celullar here can report GPS, GPS should be rolled into driver/sensor? probably sensor, since its location data

//https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html

// https://www.google.co.uk/maps/place/@52.410555,-0.5150276,12/ 17

// Or you can use OpenStreetMap directly in a Frame widget with a pin like this —

// https://www.openstreetmap.org/?mlat=54.02227&mlon=-0.66649#map=19/54.02227/-0.66649


  //https://github.com/vshymanskyy/TinyGSM/pull/260/files#diff-49f12f4a048fa1f63d160e1adb91526d97e2a16cd3ba3898525ac5d1a44ddb99

#include <StreamDebugger.h>

class mSIM7000G :
  public mTaskerInterface
{

  private:
  public:
    mSIM7000G(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Pre_Init();
    void Init(void);

    static constexpr const char* PM_MODULE__DRIVERS_MODEM_7000G__CTR = D_MODULE__DRIVERS_MODEM_7000G__CTR;
    PGM_P GetModuleName(){          return PM_MODULE__DRIVERS_MODEM_7000G__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_MODEM_7000G_ID; }
        
    struct ClassState
    {
      uint8_t devices = 0;
      uint8_t mode = ModuleStatus::Initialising;
    } module_state;
    
    // -------------------------------------------------------------------------------------------------
    // Modem bring-up (migration API)
    // Called by mCellular. This performs UART + baud sync + TinyGSM init/restart.
    // Does NOT bring up data context or do network attach (those will migrate later).
    // -------------------------------------------------------------------------------------------------

    struct MODEM_UART_RUNTIME
    {
      int8_t rx_pin = -1;
      int8_t tx_pin = -1;
      int8_t pwrkey_pin = -1;
      int8_t uart_port = -1;
    } pins;
    HardwareSerial* modem_serial = nullptr;



    
#ifdef ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  StreamDebugger* stream_debugger = nullptr;
#endif


// Progress init state machine; call from TaskSecond / TaskEvery50ms etc.
void ModemInit_Tick(uint32_t now_ms);

// True when UART+baud+AT is stable and basic probe completed
bool ModemInit_IsReady() const;

// Optional: true if init is actively in progress
bool ModemInit_IsBusy() const;

// Optional: request a hard reset and restart init
void ModemInit_ForceRestart();
struct modem_probe_t {
  bool at_ok;
  bool sim_present;
  bool sim_ready;
  uint32_t caps;
  char model[24];
  int csq;
};
const modem_probe_t& ModemInit_GetProbe() const;


#ifdef ENABLE_FEATURE_SIM__SMS

struct sms2_t
{
  // bool enabled = true;
  bool init_done = false;

  // Queue of stored SMS indices to process (CMTI/CMGL)
  std::vector<int> pending_idx;

  // CMGR state (single-reader async)
  bool     cmgr_inflight = false;
  int      cmgr_idx      = -1;
  uint32_t cmgr_t0_ms    = 0;

  // Delete scheduling
  bool     delete_pending = false;
  int      delete_idx     = -1;

  // Optional periodic sweep
  uint32_t t_last_cmgl_ms = 0;
};

sms2_t sms2;

// ---- init / control ----
void SMS2_Init_NotifyOnly();              // CMGF/CPMS/CNMI=3,1,0,0,0 (+verify)
void SMS2_Enable();
void SMS2_Disable();

bool SMS2_Init_NotifyOnly_Blocking();

// ---- parser hook (call from parse_ATCommands) ----
void SMS2_ATParse_Line(char* buffer, uint16_t buflen, uint8_t response_loglevel);

// ---- task/tick ----
void SMS2_Task_Tick(uint32_t now_ms);

// ---- operations ----
void SMS2_Request_List_Unread();          // AT+CMGL="REC UNREAD" (optional sweep)
void SMS2_QueueIndexUnique(int idx);

    struct SMS_STATUS
    {
      timereached_t tReached_Update;
      // bool enabled = 0; // 0 disabled, 1 enabled       
      bool configured = false;
      std::vector<uint8_t> messages_incoming_index_list;     
    }sms;
    void SMS_Enable();
    void SMS_Disable();






    bool SMS_SendText(const char* to_number, const char* body);
    void ATParse_CMGD__CommandNameInTextDeleteMessage(char* buffer, uint8_t buflen, uint8_t response_loglevel);
    

#endif


void DataNetwork_LogDiagnostics(void);
    sensorset_location_t location;
    // uint8_t GetSensorCount(void) override
    // {
    //   return settings.sensor_count;
    // }    
    // void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    // {
    //   if(index > settings.sensor_count-1) {value->sensor_type.push_back(0); return ;}
    //   value->sensor_type.push_back(SENSOR_TYPE_LATITUDE_ID);
    //   value->data_f.push_back(location.latitude);
    //   value->sensor_type.push_back(SENSOR_TYPE_LONGITUDE_ID);
    //   value->data_f.push_back(location.longitude);
    //   value->sensor_type.push_back(SENSOR_TYPE_SPEED_ID);
    //   value->data_f.push_back(location.speed);
    //   value->sensor_type.push_back(SENSOR_TYPE_ALTITUDE_ID);
    //   value->data_f.push_back(location.altitude);
    //   value->sensor_id = index;
    // };


// -------------------------------------------------------------------------------------------------
// Non-blocking modem init state machine (driver-owned)
// -------------------------------------------------------------------------------------------------
enum class modem_init_state_t : uint8_t {
  IDLE = 0,
  POWER_CYCLE_BEGIN,
  POWER_CYCLE_WAIT,
  UART_BEGIN_BOOT_BAUD,
  UART_SET_MODEM_BAUD,
  UART_REOPEN_TARGET_BAUD,
  AT_SYNC,
  BASIC_CFG,
  PROBE_ID,
  READY,
  FAILED
};

struct modem_init_sm_t {
  modem_init_state_t state = modem_init_state_t::IDLE;
  uint32_t t_enter_ms = 0;
  uint8_t  attempts = 0;
  bool     restart_requested = false;
  bool     busy = false;
  bool     ready = false;
  bool probe_done;

};

modem_init_sm_t modem_sm_;
modem_probe_t   modem_probe_ = {0};


// static constexpr uint32_t MODEM_BOOT_BAUD            = 115200;
// static constexpr uint32_t POWER_OFF_MIN_MS           = 1200;
// static constexpr uint32_t MODEM_BOOT_WAIT_MS         = 1200;   // SIM7000G can be slower; 1200–2500 typical
// static constexpr uint32_t UART_SETTLE_MS             = 60;
// static constexpr uint32_t UART_REOPEN_DELAY_MS       = 60;

// static constexpr uint32_t AT_STREAM_TIMEOUT_MS       = 100;

static constexpr uint16_t AT_SYNC_STEP_TIMEOUT_MS    = 80;     // per micro-attempt
// static constexpr uint16_t AT_SYNC_TOTAL_WINDOW_MS    = 1500;   // before retrying power cycle
// static constexpr uint8_t  AT_SYNC_ATTEMPTS_PER_TICK  = 2;

// static constexpr uint16_t AT_CFG_STEP_TIMEOUT_MS     = 120;

// static constexpr uint8_t  INIT_MAX_ATTEMPTS          = 10;

void Query_Modem_Status();

    // StreamDebugger debugger(SerialAT, Serial);
    // TinyGsm modem(debugger);

    // -------------------------------------------------------------------------------------------------
// Driver helpers for higher-level policy (mCellular)
// -------------------------------------------------------------------------------------------------

// Alias for your init SM readiness
bool IsReady() const { return ModemInit_IsReady(); }

// “data plane” = PDP/GPRS connected (not just registered to tower)
bool DataNetwork_IsConnected() const
{
  return (modem != nullptr) ? modem->isGprsConnected() : false;
}

// “tower side” registration (optional, sometimes useful for diagnostics)
bool Network_IsConnected() const
{
  // true;
  return (modem != nullptr) ? ModemInit_IsReady() : false;
  // return (modem != nullptr) ? modem->isNetworkConnected() : false;
}

// Ensure a TinyGsmClient exists for external network users
TinyGsmClient* DataNetwork_GetOrCreateClient(bool force_recreate = false);

void SubTask_SMS__Check_Any_SMS_Saved();

    bool Modem__Running(uint16_t wait_millis = 1000);
    bool Modem__PowerUntilRunning(uint16_t wait_millis = 5000);
    bool SimNetwork__InitConfig();
    bool SimNetwork__StartConnection();
    bool SimNetwork__CheckConnection();

    bool DataNetwork__InitConfig();
    bool DataNetwork__StartConnection();
    bool Modem_CheckAndRestartUnresponsiveModem();


void SubTask_SMS__Read_Any_Waiting_Messages();

bool SMS_Handle_IncomingText(const char* from_number, const char* body);
static void SMS_NormaliseText(char* s);




    float GetSignalQualityPower();
    float GetSignalQualityPower(int16_t signal_quality_raw);

    
    bool SendAT(const char* buffer, uint16_t wait_millis = 0);
    bool SendAT_F(uint16_t wait_millis, PGM_P formatP, ...);
    bool SendAT_ATParseResponse_F(uint16_t wait_millis, uint8_t response_loglevel, PGM_P formatP, ...);


    TinyGsm* modem = nullptr;
    TinyGsmClient* gsm_client = nullptr;

    int counter = 0, lastIndex = 0, numberOfPieces = 24;
    String pieces[24], input;


    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    struct sim7000g_gnss_ctrl_t
    {
      bool enabled = true;
      bool configured= false;
      bool fix_valid = false;

      uint32_t tReached_Update = 0;
      uint32_t last_fix_millis = 0;
      uint32_t last_fail_millis = 0;
      uint32_t last_config_attempt_millis = 0;

      uint16_t config_retry_ms = 5000;
      uint16_t update_period_ms = 1000;
      uint16_t fail_count = 0;
    }gps;
    void GNSS_Enable();
    void GNSS_Disable();
    bool GNSS_Configure();
    void GNSS_Update();

    void SMS_GPSLocation();
    void SMS_GPSLocationAuto();
    void SMS_BatteryDetailed();


    struct SMSAuto_GPS_Messages{
      uint16_t rate_seconds = 0; //0 is disable
      uint32_t tSaved_LastSent = 0;
    }smsauto_gps_messages;
    void AutoSMS_Messages_Handle();


    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

    
    void SMS_CommandIntoJSONCommand(char* command);


int  SendAT_WaitResponse__SMSNumberStored(uint32_t timeout_ms);
bool SendAT_WaitResponse__SMSDeleteAll(uint32_t timeout_ms);


    void GPRS_UpdateConnectionState(bool state);

    bool sms2_waiting_cmgr_body = false;
char sms2_cmgr_header[200]  = {0};

bool SMS2_ExtractNumberFromCMGRHeader(const char* header, char* out_from, size_t out_from_sz);
    
    struct GPRS_STATUS
    {
      timereached_t tReached_Update;
      bool enabled = 0; // 0 disabled, 1 enabled  
      uint32_t last_comms_millis_updated = 0; 
      float signal_quality_rssi_dbm = 0;
      int16_t signal_quality_raw = 0;   
      uint32_t connected_seconds = 0;      
      uint16_t downtime_secs = 0;

      bool connected = true;


      uint16_t reconnect_init_counts = 0;

      uint16_t apn_connect_called = 0;

    }gprs;
    void GPRS_Disable();
    void ModemUpdate_GPRS();
    void Modem_Enable();


    // to delete
    #ifdef ENABLE_DEVFEATURE_SMS__REMOVE_OLD
    void SMSReadAndEraseSavedSMS();
    void SMS_PollAndDrainBacklog();
    void ModemUpdate_SMS();
    bool SMS_ApplyCNMI_Forwarding();
    bool SMS_ApplyConfig();
    void SMS_Send_TimedHeartbeat2();  // keeping this version out, a new one with asking all modules TASK_APPEND_SMS_STATUS_HEARTBEAT
    void SMS_SendBootMessage();
    bool SMS_Parse_CMGR(const char* cmgr_block,
                      char* out_from, size_t out_from_sz,
                      char* out_body, size_t out_body_sz);
    #endif


    // -------------------------------------------------------------------------------------------------
// SMS2 RX state (line-based URC/response parsing)
// -------------------------------------------------------------------------------------------------
#ifdef ENABLE_FEATURE_SIM__SMS


// When parsing multi-line constructs (CMGR/CMT), we need to remember the header
// and consume the very next line as the SMS body.
bool sms2_waiting_body = false;

// What body are we waiting for?
enum sms2_wait_kind_t : uint8_t
{
  SMS2_WAIT_NONE = 0,
  SMS2_WAIT_CMGR_BODY,
  SMS2_WAIT_CMT_BODY
};
sms2_wait_kind_t sms2_wait_kind = SMS2_WAIT_NONE;

// Saved header line for CMGR/CMT
char sms2_header_line[220] = {0};

// For CMGR, we also keep the index we asked for so we can delete after reading.
int16_t sms2_cmgr_pending_idx = -1;


bool SMS2_ExtractNumberFromHeader(const char* header_line,
                                            char* out_number,
                                            size_t out_number_sz) const;
                                            
bool ATParse__CMTI(char* line, uint16_t buflen, uint8_t response_loglevel);
bool ATParse__CMGR_Header(char* line, uint16_t buflen, uint8_t response_loglevel);
bool ATParse__CMT_Header(char* line, uint16_t buflen, uint8_t response_loglevel);
bool ATParse__BodyLine(char* line, uint16_t buflen, uint8_t response_loglevel);

#endif // ENABLE_FEATURE_SIM__SMS


// Fast-path flag: set when we see a bare "OK" line.
// Use it if you want to suppress OK spam in logs but still know OK happened.
bool last_at_ok_seen = false;

bool parse_ATCommands(char* buffer, uint16_t buflen, uint8_t response_loglevel);


bool ATParse__OK(char* line, uint16_t buflen, uint8_t response_loglevel);

bool ATParse__CMGL(char* line, uint16_t buflen, uint8_t response_loglevel);
bool ATParse__CPMS(char* line, uint16_t buflen, uint8_t response_loglevel);
bool ATParse__CMGD(char* line, uint16_t buflen, uint8_t response_loglevel);


/*******************
 * SECITON: Boot Sequence
 */
bool Modem_OpenUart(uint32_t baud);
void Modem_FlushUartRx(uint32_t drain_ms = 20);

void Modem_PulsePowerKey(uint32_t hold_ms = MODEM_PWRKEY_HOLD_MS);

bool Modem_ATProbe(uint16_t timeout_ms = AT_PROBE_TIMEOUT_FAST_MS);
bool Modem_FindResponsiveBaud(uint32_t* found_baud);
bool Modem_LockBaud(uint32_t current_baud, uint32_t target_baud);

bool Modem_EnsurePowerOnAndBaud(uint32_t target_baud = MODEM_TARGET_BAUD);













    bool Handler_ModemResponses(uint8_t response_loglevel, uint16_t wait_millis = 0);



    void Get_Modem_Hardware();
  void ModemInit_SM_Enter(modem_init_sm_t& sm, modem_init_state_t st, uint32_t now_ms);
  bool ModemInit_ATSyncTry();
  void ModemInit_ProbeFillBasic();
    #ifdef ENABLE_FEATURE_SIM__SMS
    void     SMS_Enable_Basic();
  void ATParse_CMT(char* buffer);
  bool SMS_Parse_CMT(const char* block,
                     char* out_from, size_t from_sz,
                     char* out_body, size_t body_sz);
#endif
#ifdef ENABLE_FEATURE_SIM__SMS
#endif


    struct DATA
    {
      struct batt_status
      {
        uint16_t volts_mv    = 0;
        int8_t   percentage = 0;
        uint8_t  charge_state = 0;
        bool isvalid = false;
      }
      battery;

    }modem_status;

    struct SMS_PDU_Message{

      uint8_t length_of_smsc_information = 0;


    }sms_pdu_message;

    


    bool flag_modem_initialized = false;

    // bool parse_ATCommands(char* buffer, uint16_t buflen, uint8_t log_level = 6);
     
    void ModemUpdate_BatteryStatus();

    void SMS_Send_TimedHeartbeat();
      


    void modemPowerOn();
    void modemPowerOff();
    void modemRestart();

    void SendATCommand_SMSFormatAscii();
    void SendATCommand_SMSFormatPDU();
    void SendATCommand_SMSImmediateForwardOverSerial();
    void SendATCommand_FunctionalityMode_Minimum();
    void SendATCommand_FunctionalityMode_Full();

    char* ATResponse_Parse_CMT(char* incoming, char *parsed_buf, uint16_t parsed_buflen);


    void EveryLoop();

    
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
  
    void Telemetry_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    void MQTTHandler_Sender();
    std::vector<struct telemetry_handler<mSIM7000G>*> telemetry_list;
    
    struct telemetry_handler<mSIM7000G> telemetry_settings;
    struct telemetry_handler<mSIM7000G> telemetry_state_ifchanged;

    // No specialised payload therefore use system default instead of enum
      
    
};



// ================
// /**
//  * @brief 
//  * 
//  * Short term ignoring this file for direct access to sub folder
//  * Longer, perhaps creating a parent class that future Celullar stuff can easily call indirectly
//  * 
//  */


// //parent class of BusDigital, BusPwm, and BusNetwork
// class mSIM7000G  :
//   public mTaskerInterface
// {
//   public:
//     mSIM7000G()
//     {
//     };

//     // Cellular(uint8_t type, uint16_t start, uint8_t aw)
//     // : _type(255)
//     // {
//     //   _type = type;
//     // };
    
//     int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    
//     struct SETTINGS{
//       uint8_t fEnableSensor = false;
//       uint8_t leds_found = 0;
//     }settings;


//     // virtual ~Cellular() {}

//     // virtual void     show() = 0;
//     // virtual bool     canShow() { return true; }
//     // virtual void     setStatusPixel(uint32_t c) {}
    
//     // virtual void     setPixelColorNew(uint16_t pix, uint16_t c) = 0;
//     // virtual uint16_t getPixelColorNew(uint16_t pix) { return 0; }

//     // virtual void     cleanup() = 0;
//     // virtual uint8_t  skippedLeds() { return 0; }

//     // private:
//     //     uint8_t _type = 0;

// };


// // class BusDigital : public Cellular {
// //   public:
// //     BusDigital(uint8_t &bc, uint8_t nr, const uint8_t &com);

// //     inline void show();
// //     bool canShow();
// //     void setStatusPixel(uint32_t c);

// //     void setPixelColorNew(uint16_t pix, uint16_t c);
// //     uint16_t getPixelColorNew(uint16_t pix);


// //     uint8_t skippedLeds() 
// //     {
// //       return _skip;
// //     }

// //     void reinit();

// //     void cleanup();

// //     ~BusDigital() {
// //       cleanup();
// //     }

// //   private:
// //     uint8_t _skip = 0;
// // };

      // SendATCommand_SMSFormatAscii(); //tmp

      /**
       * @brief For debugging, lets check states
       * 
       */
      // modem->sendAT("+CMFG?"); // 1= ascii mode //expected 1
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 


      // modem->sendAT("+CPMS?"); // How many SMS are waiting?
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 


    // "AT+CMGD=,4",// DELETE ALL MESSAGES (Read or not)
    // "AT+CMGR=1",
    // "AT+CMGL=\"REC UNREAD\""
    // "AT+CPMS?"     // Number of stored SMS

/*


{
  "ATCommands": [
    "AT+CMGR=1",
    "AT+CNMI=3,2,0,0,0",
    "AT+CMGL=\"REC UNREAD\""
  ]
}

*/




#endif

#endif