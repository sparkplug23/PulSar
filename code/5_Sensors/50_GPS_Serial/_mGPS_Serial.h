#ifndef _USE_MODULE_SENSORS_GPS_SERIAL_H
#define _USE_MODULE_SENSORS_GPS_SERIAL_H

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_SENSORS__GPS_SERIAL_ID 5050 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_SENSORS_GPS_SERIAL

#include <string.h>
#include <strings.h>

#define UBLOX_PARSE_STATUS
#define UBLOX_PARSE_TIMEGPS
#define UBLOX_PARSE_TIMEUTC
#define UBLOX_PARSE_POSLLH
#define UBLOX_PARSE_DOP
#define UBLOX_PARSE_PVT
#define UBLOX_PARSE_VELNED
#define UBLOX_PARSE_SVINFO
#define UBLOX_PARSE_TIMEUTC
#define GPS_FIX_TIME
#define GPS_FIX_DATE

#define NMEAGPS_PARSE_SATELLITES

#include "internal/configs/NeoGPS_cfg.h"
#include "internal/ublox/ubxGPS.h"
#include "internal/Streamers.h"

#include "Web/Gen/generated_web.h"


DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MINIMAL_CTR) "gpspacket_minimal";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_REQUIRED_CTR) "gpspacket_required";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_DEBUG_CTR)    "debug/parsing";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL_CTR)    "debug/all";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_MICRO_CTR)    "micro";


class mGPS_Serial :
  public mTaskerInterface
{

  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mGPS_Serial(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);

    static constexpr const char* PM_MODULE_SENSORS__GPS_SERIAL_CTR = D_MODULE_SENSORS__GPS_SERIAL__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS__GPS_SERIAL_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS__GPS_SERIAL_ID; }    
        
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void Load_Module(bool erase);
    void Save_Module(void);
    bool Default_Module(void);

    struct MODULE_RUNTIME{ // these will be saved and recovered on boot
      uint32_t last_message = 0;
      uint8_t valid_timeout_seconds = 0;
      uint32_t valid_fix_timeout_ms = 3000;
    }rt;


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    uint32_t tTimeout = 0;

    void Handle__Loop_Input_Stream();
    
    HardwareSerial* port = nullptr;

    void EveryLoop_InputMethod_PollingSerial_Internal();
    void EveryLoop_InputMethod_PollingSerial_Bytes();
    void EveryLoop_InputMethod_PollingSerial_BytesToBuffer();
    void EveryLoop_PollForGPSData(Stream& port);
    void EveryLoop_InputMethod_PollingSerial_BytesFromBuffer();

    #ifdef ENABLE_DEVFEATURE_GPS__PARSER_TEST1
    void GPS_Incoming_Stream(Stream& stream);
    bool GPS_ProcessByte(uint8_t c);
    bool GPS_CommitFixIfAccepted(const gps_fix& candidate);
    #endif


    struct STATS{
      uint32_t last_message_received_time = 0;
      uint32_t last_valid_message_received_time = 0;
      uint32_t packets_received = 0;
      uint32_t bytes_received = 0;
    }stats;
        

    void UBX_Polling_BasicViaParser();

    void UBX_Polling();
    void UBX_ProcessPacketMetrics_OnDecodeCompleted(NMEAGPS::decode_t res);
    void Splash_Packet_Metrics();
    const char* GetNmeaMessageName(uint8_t nmea_msg);
    const char* GetUbxMessageName(uint8_t cls, uint8_t id);
    void UBX_ProcessAvailableFix();

#ifdef ENABLE_FEATURE_GPSSERIAL__PACKET_METRICS

  struct nmea_msg_state_t
  {
    uint32_t count_rx = 0;
    uint32_t last_seen_ms = 0;

    void Init()
    {
      count_rx = 0;
      last_seen_ms = 0;
    }

    void UpdateData()
    {
      count_rx++;
      last_seen_ms = millis();
    }

    void ClearCounters()
    {
      count_rx = 0;
      last_seen_ms = 0;
    }
  };

  struct ubx_msg_state_t
  {
    uint8_t  msg_class = 0;
    uint8_t  msg_id = 0;
    uint32_t count_rx = 0;
    uint32_t last_seen_ms = 0;
    uint16_t last_payload_len = 0;

    void Init(uint8_t cls, uint8_t id)
    {
      msg_class = cls;
      msg_id = id;
      count_rx = 0;
      last_seen_ms = 0;
      last_payload_len = 0;
    }

    void UpdateData(uint16_t payload_len)
    {
      count_rx++;
      last_seen_ms = millis();
      last_payload_len = payload_len;
    }

    void ClearCounters()
    {
      count_rx = 0;
      last_seen_ms = 0;
      last_payload_len = 0;
    }
  };

  struct gps_packet_metrics_t
  {
    std::vector<nmea_msg_state_t> nmea_msgs;
    std::vector<ubx_msg_state_t>  ubx_msgs;

    int16_t NmeaMessageToIndex(uint8_t nmea_msg)
    {
      if (nmea_msg == NMEAGPS::NMEA_UNKNOWN) {
        return -1;
      }

      int16_t index = (int16_t)nmea_msg - 1;

      if (index < 0) {
        return -1;
      }

      if (index >= ((int16_t)NMEAGPS::NMEAMSG_END - 1)) {
        return -1;
      }

      return index;
    }

    void Init()
    {
      nmea_msgs.clear();
      ubx_msgs.clear();
    }

    void InitNmeaVector()
    {
      if (!nmea_msgs.empty()) {
        return;
      }

      nmea_msgs.resize((size_t)NMEAGPS::NMEAMSG_END - 1);
      for (auto &msg : nmea_msgs) {
        msg.Init();
      }
    }

    void FreeData()
    {
      nmea_msgs.clear();
      ubx_msgs.clear();
    }

    void ClearCounters()
    {
      for (auto &msg : nmea_msgs) {
        msg.ClearCounters();
      }

      for (auto &msg : ubx_msgs) {
        msg.ClearCounters();
      }
    }

    // -----------------------------
    // NMEA
    // -----------------------------

    // READ/WRITE helper: creates NMEA vector if needed
    nmea_msg_state_t* GetNmeaMessage(uint8_t nmea_msg)
    {
      int16_t index = NmeaMessageToIndex(nmea_msg);
      if (index < 0) {
        return nullptr;
      }

      if (nmea_msgs.empty()) {
        InitNmeaVector();
      }

      return &nmea_msgs[(size_t)index];
    }

    // READ-ONLY helper: never creates
    nmea_msg_state_t* FindNmeaMessage(uint8_t nmea_msg)
    {
      if (nmea_msgs.empty()) {
        return nullptr;
      }

      int16_t index = NmeaMessageToIndex(nmea_msg);
      if (index < 0) {
        return nullptr;
      }

      return &nmea_msgs[(size_t)index];
    }

    bool UpdateNmeaMessage(uint8_t nmea_msg)
    {
      nmea_msg_state_t* msg = GetNmeaMessage(nmea_msg);
      if (msg == nullptr) {
        return false;
      }

      msg->UpdateData();
      return true;
    }

    // -----------------------------
    // UBX
    // -----------------------------

    // READ-ONLY helper: never creates
    ubx_msg_state_t* FindUbxMessage(uint8_t cls, uint8_t id)
    {
      for (auto &msg : ubx_msgs) {
        if ((msg.msg_class == cls) && (msg.msg_id == id)) {
          return &msg;
        }
      }
      return nullptr;
    }

    // WRITE helper: creates only when parser/update path calls it
    ubx_msg_state_t* FindOrCreateUbxMessage(uint8_t cls, uint8_t id)
    {
      ubx_msg_state_t* msg = FindUbxMessage(cls, id);
      if (msg != nullptr) {
        return msg;
      }

      ubx_msg_state_t temp;
      temp.Init(cls, id);
      ubx_msgs.push_back(temp);
      return &ubx_msgs.back();
    }

    bool UpdateUbxMessage(uint8_t cls, uint8_t id, uint16_t payload_len)
    {
      ubx_msg_state_t* msg = FindOrCreateUbxMessage(cls, id);
      if (msg == nullptr) {
        return false;
      }

      msg->UpdateData(payload_len);
      return true;
    }

  } packet_metrics;

#endif // ENABLE_FEATURE_GPSSERIAL__PACKET_METRICS


    struct gps_packet_config_ubx_nav_t
    {
      bool enabled = false;
      bool desired_enabled = false;
    };

    struct gps_packet_config_nmea_t
    {
      bool enabled = false;
      bool desired_enabled = false;
    };

    struct gps_packet_config_t
    {
      gps_packet_config_ubx_nav_t ubx_nav[ARRAY_SIZE(ublox::UBX_NAV_LIST)];
      gps_packet_config_nmea_t nmea[(uint8_t)NMEAGPS::NMEAMSG_END - 1];

      bool pending_apply = false;
      uint8_t desired_rate = 1;

      uint32_t baud_current = 0;   // current baud we believe host/GPS are using
      uint32_t baud_target = 0;   // requested baud from UI / config
      bool pending_baud_apply = false;

      void Init()
      {
        pending_apply = false;
        desired_rate = 1;

        baud_current = 0;
        baud_target = 0;
        pending_baud_apply = false;

        for (uint8_t i = 0; i < ARRAY_SIZE(ublox::UBX_NAV_LIST); i++) {
          ubx_nav[i].enabled = false;
          ubx_nav[i].desired_enabled = false;
        }

        for (uint8_t i = 0; i < ((uint8_t)NMEAGPS::NMEAMSG_END - 1); i++) {
          nmea[i].enabled = false;
          nmea[i].desired_enabled = false;
        }
      }
    } packet_config;


    void Set_Default_PacketConfig(void);
    void ApplyPacketConfig_Pending(bool update_all = false);
    void ApplyGPSBaud_Pending();

    bool SetGPSModuleBaud(uint32_t desired_baud);
    bool GPS_ProbeBaud(uint32_t baud, uint32_t timeout_ms);
    bool GPS_FindWorkingBaud(uint32_t& found_baud);
    bool GPS_ChangeDeviceBaud(uint32_t current_baud, uint32_t new_baud);
    bool GPS_SendRawUbxCfgPrt_SetBaud(uint32_t new_baud);


    ubloxGPS*  ubx_parser = nullptr;

    /**
     * @note Holds a partial result during parsing, only to be merged with the stored fix is valid
     * */
    gps_fix   fix_parsing;
    /**
     * @note Stores a valid solution, merged from the parsing fix
     * */
    gps_fix   fix_valid;


    enum GPS_INPUT_STREAM_METHOD_IDS{
      UBX_POLLING_BYTE_STREAM__DIRECT_SERIAL = 1,
      UBX_POLLING_BYTE_STREAM__DIRECT_SERIAL_BASIC_VIA_PARSER,
      UBX_POLLING_BYTE_STREAM__MODULE_SERIAL_CLAIMED,
      GPS_INPUT_STREAM_METHOD__MODULE_SERIAL_CLAIMED_RINGBUFFERS_ID,
      GPS_INPUT_STREAM_METHOD_LENGTH_ID
    };
    uint8_t read_gps_method = UBX_POLLING_BYTE_STREAM__DIRECT_SERIAL;
    
    bool Init_BeginStream();
    

    struct CONNECTION_STATUS{
      /**
       * @note Any activity at the expected baud rate
       * */
      uint32_t last_message_received_time = 0;
      /**
       * @note Parsed and valid messages on the expected baud, valid fix not required
       * */
      uint32_t last_valid_message_received_time = 0;


    }connection_status;

    
    /************************************************************************************************
     * SECTION: Web
     ************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebPage_Root_AddHandlers();
    #ifdef ENABLE_FEATURE_WEBSERVER__GPS_CONFIG_DEVICE
    void Serve_Submodule_GPSConfigDevice(AsyncWebServerRequest* request);
    void Serve_Submodule_GPSConfigDevice_Post(AsyncWebServerRequest* request);
    void Serve_Submodule_GPSConfigDevice_JS(AsyncWebServerRequest* request);
    void Serve_Submodule_GPSConfigDevice_Page(AsyncWebServerRequest* request);
    #endif // ENABLE_FEATURE_WEBSERVER__GPS_CONFIG_DEVICE
    #ifdef ENABLE_FEATURE_WEBSERVER__GPS_OVERVIEW
    void Serve_Submodule_GPSOverview(AsyncWebServerRequest* request);
    void Serve_Submodule_GPSOverview_Page(AsyncWebServerRequest* request);
    void Serve_Submodule_GPSOverview_JS(AsyncWebServerRequest* request);
    #endif
    #endif // USE_MODULE_NETWORK_WEBSERVER

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Required(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Minimal(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Debug(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_Micro(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_All(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_All1(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_All2(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_All3(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_GPSPacket_All4(uint8_t json_level = 0, bool json_appending = true);

    uint8_t ConstructJSON_NAV_PVT(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void Telemetry_Init();
    std::vector<struct telemetry_handler<mGPS_Serial>*> telemetry_list;
    struct telemetry_handler<mGPS_Serial> telemetry_settings;
    struct telemetry_handler<mGPS_Serial> telemetry_gpspacket_minimal_teleperiod;
    struct telemetry_handler<mGPS_Serial> telemetry_gpspacket_required;
    struct telemetry_handler<mGPS_Serial> telemetry_gpspacket_all1;                   CTRP(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL1_CTR) "debug/all1";
    struct telemetry_handler<mGPS_Serial> telemetry_gpspacket_all2;                   CTRP(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL2_CTR) "debug/all2";
    struct telemetry_handler<mGPS_Serial> telemetry_gpspacket_all3;                   CTRP(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL3_CTR) "debug/all3";
    struct telemetry_handler<mGPS_Serial> telemetry_gpspacket_all4;                   CTRP(PM_MQTT_HANDLER_POSTFIX_TOPIC_GPSPACKET_ALL4_CTR) "debug/all4";
    struct telemetry_handler<mGPS_Serial> telemetry_gpspacket_micro;
    struct telemetry_handler<mGPS_Serial> telemetry_gpspacket_debug;
    struct telemetry_handler<mGPS_Serial> telemetry_nav__pvt;                         CTRP(PM_MQTT_HANDLER_POSTFIX_TOPIC__NAV_PVT) "nav/pvt";
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif

