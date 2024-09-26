#include "mMAVLink_Decoder.h"

#ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER


int8_t mMAVLink_Decoder::Tasker(uint8_t function, JsonParserObject obj)
{
  
  int8_t function_result = 0;
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP:
      Maintain_Connection();
    break;
    case TASK_EVERY_SECOND:
      // ALOG_INF(PSTR("Battery %d%%"), pkt.battery_status.data.battery_remaining);
      

      if((millis()-tSaved_Last_Response)>10000)
      {
        ALOG_ERR(PSTR("MAVLINK No Data!!! %d secs"),(millis()-tSaved_Last_Response)/1000);
      }else{
        ALOG_INF(PSTR("MAVLINK %d ms"),millis()-tSaved_Last_Response);
      }
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
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function




void mMAVLink_Decoder::Pre_Init(void)
{

  // ALOG_INF(PSTR("mMAVLink_Decoder::Pre_Init(void) %d"), tkr_Serial->settings.uart2.initialised);

  module_state.mode = ModuleStatus::Initialising;
  
  connection.system_id = 255; // ESP32 sysid
  connection.component_id = 0; // ESP32 compid
  connection.type = MAV_TYPE_FIXED_WING;
  connection.autopilot =  MAV_AUTOPILOT_GENERIC;

  ALOG_INF(PSTR("mMAVLink_Decoder::Pre_Init(void) DONE"));

}

void mMAVLink_Decoder::Send_Heartbeat()
{
  
    uint8_t system_id = 255;//connection.system_id;
    uint8_t component_id = 0;//connection.component_id;
    uint8_t target_system = 1;//connection.received_sysid;
    uint8_t target_component = 1;//connection.received_compid;


  // mavlink_message_t msghb;
  // mavlink_heartbeat_t heartbeat;
  // uint8_t bufhb[MAVLINK_MAX_PACKET_LEN];
  // mavlink_msg_heartbeat_pack(
  //   connection.system_id,      // system_id – ID of this system
  //   connection.component_id,   // component_id – ID of this component (e.g. 200 for IMU)
  //   &msghb,                    // msg – The MAVLink message to compress the data into
  //   connection.type,           // type – Type of the MAV (quadrotor, helicopter, etc., up to 15 types, defined in MAV_TYPE ENUM)
  //   connection.autopilot,      // autopilot – Autopilot type / class. defined in MAV_AUTOPILOT ENUM
  //   MAV_MODE_PREFLIGHT,        // base_mode – System mode bitfield, as defined by MAV_MODE_FLAG enum
  //   0,                         // custom_mode – A bitfield for use for autopilot-specific flags
  //   MAV_STATE_STANDBY          // system_status – System status flag, as defined by MAV_STATE enum
  // );

  
  mavlink_message_t msghb;
  mavlink_heartbeat_t heartbeat;
  uint8_t bufhb[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_heartbeat_pack(
    255,      // system_id – ID of this system
    0,   // component_id – ID of this component (e.g. 200 for IMU)
    &msghb,                    // msg – The MAVLink message to compress the data into
    connection.type,           // type – Type of the MAV (quadrotor, helicopter, etc., up to 15 types, defined in MAV_TYPE ENUM)
    connection.autopilot,      // autopilot – Autopilot type / class. defined in MAV_AUTOPILOT ENUM
    MAV_MODE_PREFLIGHT,        // base_mode – System mode bitfield, as defined by MAV_MODE_FLAG enum
    0,                         // custom_mode – A bitfield for use for autopilot-specific flags
    MAV_STATE_STANDBY          // system_status – System status flag, as defined by MAV_STATE enum
  );


  uint16_t lenhb = mavlink_msg_to_send_buffer(bufhb, &msghb);
  _MAVSerial->write(bufhb,lenhb);
  ALOG_INF(PSTR(">> Send Heartbeat: sys %d, comp %d"), connection.system_id, connection.component_id);
}


void mMAVLink_Decoder::Init(void)
{
  
  #ifdef USE_MODULE_CORE__SERIAL
  if(tkr_Serial->settings.uart2.initialised)
  {
    _MAVSerial = tkr_Serial->HWSerial2;//tkr_Serial->GetSerial(2);
    module_state.mode = ModuleStatus::Running;
  }
  #else
  _MAVSerial = &Serial2;
  _MAVSerial->begin(921600, SERIAL_8N1, 16, 17);
  // _MAVSerial->begin(921600);
  #endif

  // _MAVSerial->print("Boot");

  ALOG_INF(PSTR("mMAVLink_Decoder::Init(void) DONE"));

  
  
}


void mMAVLink_Decoder::handleMessage(const mavlink_message_t& msg) {

  ALOG_DBM(PSTR("handleMessage %d"), msg.msgid);

  switch (msg.msgid) {
    case MAVLINK_MSG_ID_AHRS:
    {
      mavlink_ahrs_t data;
      mavlink_msg_ahrs_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_AHRS, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__AHRS__CTR);
      break;
    }
    case MAVLINK_MSG_ID_AHRS2:
    {
      mavlink_ahrs2_t data;
      mavlink_msg_ahrs2_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_AHRS2, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__AHRS2__CTR);
      break;
    }
    case MAVLINK_MSG_ID_ATTITUDE:
    {
      mavlink_attitude_t data;
      mavlink_msg_attitude_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_ATTITUDE, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE__CTR);
      break;
    }
    case MAVLINK_MSG_ID_AUTOPILOT_VERSION_REQUEST:
    {
      mavlink_autopilot_version_request_t data;
      mavlink_msg_autopilot_version_request_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_AUTOPILOT_VERSION_REQUEST, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__AUTOPILOT_VERSION_REQUEST__CTR);
      break;
    }
    case MAVLINK_MSG_ID_BATTERY_STATUS:
    {
      mavlink_battery_status_t data;
      mavlink_msg_battery_status_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_BATTERY_STATUS, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__BATTERY_STATUS__CTR);
      break;
    }
    case MAVLINK_MSG_ID_CAMERA_FEEDBACK:
    {
      mavlink_camera_feedback_t data;
      mavlink_msg_camera_feedback_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_CAMERA_FEEDBACK, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__CAMERA_FEEDBACK__CTR);
      break;
    }
    case MAVLINK_MSG_ID_COMMAND_LONG:
    {
      mavlink_command_long_t data;
      mavlink_msg_command_long_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_COMMAND_LONG, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__COMMAND_LONG__CTR);
      break;
    }
    case MAVLINK_MSG_ID_EKF_STATUS_REPORT:
    {
      mavlink_ekf_status_report_t data;
      mavlink_msg_ekf_status_report_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_EKF_STATUS_REPORT, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__EKF_STATUS_REPORT__CTR);
      break;
    }
    case MAVLINK_MSG_ID_FENCE_STATUS:
    {
      mavlink_fence_status_t data;
      mavlink_msg_fence_status_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_FENCE_STATUS, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__FENCE_STATUS__CTR);
      break;
    }
    case MAVLINK_MSG_ID_GIMBAL_REPORT:
    {
      mavlink_gimbal_report_t data;
      mavlink_msg_gimbal_report_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_GIMBAL_REPORT, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_REPORT__CTR);
      break;
    }
    case MAVLINK_MSG_ID_GIMBAL_TORQUE_CMD_REPORT:
    {
      mavlink_gimbal_torque_cmd_report_t data;
      mavlink_msg_gimbal_torque_cmd_report_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_GIMBAL_TORQUE_CMD_REPORT, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_TORQUE_CMD_REPORT__CTR);
      break;
    }
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
    {
      mavlink_global_position_int_t data;
      mavlink_msg_global_position_int_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_GLOBAL_POSITION_INT, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_POSITION_INT__CTR);
      break;
    }
    case MAVLINK_MSG_ID_GOPRO_HEARTBEAT:
    {
      mavlink_gopro_heartbeat_t data;
      mavlink_msg_gopro_heartbeat_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_GOPRO_HEARTBEAT, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__GOPRO_HEARTBEAT__CTR);
      break;
    }
    case MAVLINK_MSG_ID_GOPRO_SET_RESPONSE:
    {
      mavlink_gopro_set_response_t data;
      mavlink_msg_gopro_set_response_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_GOPRO_SET_RESPONSE, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__GOPRO_SET_RESPONSE__CTR);
      break;
    }
    case MAVLINK_MSG_ID_GPS_GLOBAL_ORIGIN:
    {
      mavlink_gps_global_origin_t data;
      mavlink_msg_gps_global_origin_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_GPS_GLOBAL_ORIGIN, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__GPS_GLOBAL_ORIGIN__CTR);
      break;
    }
    case MAVLINK_MSG_ID_GPS_RAW_INT:
    {
      mavlink_gps_raw_int_t data;
      mavlink_msg_gps_raw_int_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_GPS_RAW_INT, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__GPS_RAW_INT__CTR);
      break;
    }
    case MAVLINK_MSG_ID_HEARTBEAT:
    {
      mavlink_heartbeat_t data;
      mavlink_msg_heartbeat_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_HEARTBEAT, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__HEARTBEAT__CTR);
      break;
    }
    case MAVLINK_MSG_ID_HOME_POSITION:
    {
      mavlink_home_position_t data;
      mavlink_msg_home_position_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_HOME_POSITION, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__HOME_POSITION__CTR);
      break;
    }
    case MAVLINK_MSG_ID_HWSTATUS:
    {
      mavlink_hwstatus_t data;
      mavlink_msg_hwstatus_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_HWSTATUS, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__HWSTATUS__CTR);
      break;
    }
    case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
    {
      mavlink_local_position_ned_t data;
      mavlink_msg_local_position_ned_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_LOCAL_POSITION_NED, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__LOCAL_POSITION_NED__CTR);
      break;
    }
    case MAVLINK_MSG_ID_MEMINFO:
    {
      mavlink_meminfo_t data;
      mavlink_msg_meminfo_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_MEMINFO, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__MEMINFO__CTR);
      break;
    }
    case MAVLINK_MSG_ID_MISSION_CURRENT:
    {
      mavlink_mission_current_t data;
      mavlink_msg_mission_current_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_MISSION_CURRENT, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__MISSION_CURRENT__CTR);
      break;
    }
    case MAVLINK_MSG_ID_MOUNT_STATUS:
    {
      mavlink_mount_status_t data;
      mavlink_msg_mount_status_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_MOUNT_STATUS, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__MOUNT_STATUS__CTR);
      break;
    }
    case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:
    {
      mavlink_nav_controller_output_t data;
      mavlink_msg_nav_controller_output_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__NAV_CONTROLLER_OUTPUT__CTR);
      break;
    }
    case MAVLINK_MSG_ID_PARAM_REQUEST_READ:
    {
      mavlink_param_request_read_t data;
      mavlink_msg_param_request_read_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_PARAM_REQUEST_READ, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__PARAM_REQUEST_READ__CTR);
      break;
    }
    case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
    {
      mavlink_param_request_list_t data;
      mavlink_msg_param_request_list_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_PARAM_REQUEST_LIST, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__PARAM_REQUEST_LIST__CTR);
      break;
    }
    case MAVLINK_MSG_ID_PARAM_VALUE:
    {
      mavlink_param_value_t data;
      mavlink_msg_param_value_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_PARAM_VALUE, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__PARAM_VALUE__CTR);
      break;
    }
    case MAVLINK_MSG_ID_POWER_STATUS:
    {
      mavlink_power_status_t data;
      mavlink_msg_power_status_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_POWER_STATUS, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__POWER_STATUS__CTR);
      break;
    }
    case MAVLINK_MSG_ID_RAW_IMU:
    {
      mavlink_raw_imu_t data;
      mavlink_msg_raw_imu_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_RAW_IMU, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__RAW_IMU__CTR);
      break;
    }
    case MAVLINK_MSG_ID_RC_CHANNELS:
    {
      mavlink_rc_channels_t data;
      mavlink_msg_rc_channels_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_RC_CHANNELS, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS__CTR);
      break;
    }
    case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
    {
      mavlink_rc_channels_raw_t data;
      mavlink_msg_rc_channels_raw_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_RC_CHANNELS_RAW, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_RAW__CTR);
      break;
    }
    case MAVLINK_MSG_ID_RC_CHANNELS_SCALED:
    {
      mavlink_rc_channels_scaled_t data;
      mavlink_msg_rc_channels_scaled_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_RC_CHANNELS_SCALED, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_SCALED__CTR);
      break;
    }
    case MAVLINK_MSG_ID_REMOTE_LOG_BLOCK_STATUS:
    {
      mavlink_remote_log_block_status_t data;
      mavlink_msg_remote_log_block_status_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_REMOTE_LOG_BLOCK_STATUS, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__REMOTE_LOG_BLOCK_STATUS__CTR);
      break;
    }
    case MAVLINK_MSG_ID_REQUEST_DATA_STREAM:
    {
      mavlink_request_data_stream_t data;
      mavlink_msg_request_data_stream_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_REQUEST_DATA_STREAM, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__REQUEST_DATA_STREAM__CTR);
      break;
    }
    case MAVLINK_MSG_ID_SCALED_IMU2:
    {
      mavlink_scaled_imu2_t data;
      mavlink_msg_scaled_imu2_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_SCALED_IMU2, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU2__CTR);
      break;
    }
    case MAVLINK_MSG_ID_SCALED_IMU3:
    {
      mavlink_scaled_imu3_t data;
      mavlink_msg_scaled_imu3_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_SCALED_IMU3, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU3__CTR);
      break;
    }
    case MAVLINK_MSG_ID_SCALED_PRESSURE:
    {
      mavlink_scaled_pressure_t data;
      mavlink_msg_scaled_pressure_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_SCALED_PRESSURE, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE__CTR);
      break;
    }
    case MAVLINK_MSG_ID_SCALED_PRESSURE2:
    {
      mavlink_scaled_pressure2_t data;
      mavlink_msg_scaled_pressure2_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_SCALED_PRESSURE2, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE2__CTR);
      break;
    }
    case MAVLINK_MSG_ID_SENSOR_OFFSETS:
    {
      mavlink_sensor_offsets_t data;
      mavlink_msg_sensor_offsets_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_SENSOR_OFFSETS, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__SENSOR_OFFSETS__CTR);
      break;
    }
    case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:
    {
      mavlink_servo_output_raw_t data;
      mavlink_msg_servo_output_raw_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_SERVO_OUTPUT_RAW, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__SERVO_OUTPUT_RAW__CTR);
      break;
    }
    case MAVLINK_MSG_ID_SET_MODE:
    {
      mavlink_set_mode_t data;
      mavlink_msg_set_mode_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_SET_MODE, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__SET_MODE__CTR);
      break;
    }
    case MAVLINK_MSG_ID_STATUSTEXT:
    {
      mavlink_statustext_t data;
      mavlink_msg_statustext_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_STATUSTEXT, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__STATUSTEXT__CTR);
      break;
    }
    case MAVLINK_MSG_ID_SYS_STATUS:
    {
      mavlink_sys_status_t data;
      mavlink_msg_sys_status_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_SYS_STATUS, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__SYS_STATUS__CTR);
      break;
    }
    case MAVLINK_MSG_ID_SYSTEM_TIME:
    {
      mavlink_system_time_t data;
      mavlink_msg_system_time_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_SYSTEM_TIME, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__SYSTEM_TIME__CTR);
      break;
    }
    case MAVLINK_MSG_ID_TIMESYNC:
    {
      mavlink_timesync_t data;
      mavlink_msg_timesync_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_TIMESYNC, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__TIMESYNC__CTR);
      break;
    }
    case MAVLINK_MSG_ID_TERRAIN_REPORT:
    {
      mavlink_terrain_report_t data;
      mavlink_msg_terrain_report_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_TERRAIN_REPORT, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_REPORT__CTR);
      break;
    }
    case MAVLINK_MSG_ID_VFR_HUD:
    {
      mavlink_vfr_hud_t data;
      mavlink_msg_vfr_hud_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_VFR_HUD, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__VFR_HUD__CTR);
      break;
    }
    case MAVLINK_MSG_ID_VIBRATION:
    {
      mavlink_vibration_t data;
      mavlink_msg_vibration_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_VIBRATION, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__VIBRATION__CTR);
      break;
    }
    case MAVLINK_MSG_ID_WIND:
    {
      mavlink_wind_t data;
      mavlink_msg_wind_decode(&msg, &data);
      addOrUpdatePacket(MAVLINK_MSG_ID_WIND, &data, sizeof(data), PM_MAVLINK_MSG_PACKET_NAME__WIND__CTR);
      break;
    }
    default:
      ALOG_INF(PSTR("Unknown message ID: %d"), msg.msgid);
      break;
  }
}


mMAVLink_Decoder::PacketData* mMAVLink_Decoder::findPacketData(uint16_t packetID) {
    for (auto& packet : packets) {
        if (packet.packetID == packetID) {
            return &packet;
        }
    }
    return nullptr;
}


void mMAVLink_Decoder::addOrUpdatePacket(uint16_t packetID, void* data, uint32_t dataSize, const char* packetName) {
    for (auto& packet : packets) {
        if (packet.packetID == packetID) {
            memcpy(packet.data, data, dataSize);
            packet.tUpdate = millis();
            packet.packet_name = packetName; // Update the packet name if necessary
            
            ALOG_DBM(PSTR("UpdatePacket = %d \"%S\""), packetID, packetName);
            return;
        }
    }
    PacketData newPacket;
    newPacket.packetID = packetID;
    newPacket.data = malloc(dataSize);
    memcpy(newPacket.data, data, dataSize);
    newPacket.tUpdate = millis();
    newPacket.packet_name = packetName;
    packets.push_back(newPacket);

    ALOG_INF(PSTR("NEW ========================== addPacket = %d \"%S\""), packetID, packetName);
}


void mMAVLink_Decoder::Send_MAVLink_Stream_Pack__Enable_All()
{

  // mavlink_message_t msgds;
  // uint8_t bufds[MAVLINK_MAX_PACKET_LEN];
  // // mavlink_msg_request_data_stream_pack(connection.system_id, connection.component_id, &msgds, connection.received_sysid, connection.received_compid, MAV_DATA_STREAM_ALL , 0x05, 1);
  // // mavlink_msg_request_data_stream_pack(connection.system_id, connection.component_id, &msgds, connection.received_sysid, connection.received_compid, MAV_DATA_STREAM_ALL , 30, 1);  
  // // mavlink_msg_request_data_stream_pack(connection.system_id, connection.component_id, &msgds, connection.received_sysid, connection.received_compid, MAV_DATA_STREAM_RAW_SENSORS , 10, 1);
  // mavlink_msg_request_data_stream_pack(connection.system_id, connection.component_id, &msgds, connection.received_sysid, connection.received_compid, MAV_DATA_STREAM_ALL , 0x05, 0);
  // uint16_t lends = mavlink_msg_to_send_buffer(bufds, &msgds);
  // _MAVSerial->write(bufds,lends);


    uint8_t system_id = 255;//connection.system_id;
    uint8_t component_id = 0;//connection.component_id;
    uint8_t target_system = 1;//connection.received_sysid;
    uint8_t target_component = 1;//connection.received_compid;

    // Disable all data streams
    disableAllDataStreams(system_id, component_id, target_system, target_component, _MAVSerial);
    delay(100); // Increase delay to ensure command is processed

    // Set rates for specific message types
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_ATTITUDE, 100000, _MAVSerial); // 10 Hz
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_HEARTBEAT, 1000000, _MAVSerial); // 1 Hz
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_SYS_STATUS, 500000, _MAVSerial); // 2 Hz

    
    // // Turn off each MAVLink message stream
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_AHRS, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_AHRS2, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_ATTITUDE, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_AUTOPILOT_VERSION_REQUEST, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_BATTERY_STATUS, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_CAMERA_FEEDBACK, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_COMMAND_LONG, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_EKF_STATUS_REPORT, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_FENCE_STATUS, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_GIMBAL_REPORT, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_GIMBAL_TORQUE_CMD_REPORT, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_GPS_GLOBAL_ORIGIN, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_GLOBAL_POSITION_INT, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_GOPRO_HEARTBEAT, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_GOPRO_SET_RESPONSE, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_GPS_RAW_INT, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_HEARTBEAT, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_HOME_POSITION, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_HWSTATUS, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_LOCAL_POSITION_NED, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_MEMINFO, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_MISSION_CURRENT, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_MOUNT_STATUS, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_PARAM_REQUEST_READ, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_PARAM_REQUEST_LIST, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_PARAM_VALUE, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_POWER_STATUS, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_RAW_IMU, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_RC_CHANNELS, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_RC_CHANNELS_RAW, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_RC_CHANNELS_SCALED, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_REMOTE_LOG_BLOCK_STATUS, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_REQUEST_DATA_STREAM, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_SCALED_IMU2, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_SCALED_IMU3, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_SCALED_PRESSURE, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_SCALED_PRESSURE2, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_SENSOR_OFFSETS, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_SERVO_OUTPUT_RAW, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_SET_MODE, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_STATUSTEXT, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_SYS_STATUS, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_SYSTEM_TIME, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_TIMESYNC, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_TERRAIN_REPORT, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_VFR_HUD, 0, _MAVSerial);
    // setMessageRate(system_id, component_id, target_system, target_component, MAVLINK_MSG_ID_VIBRATION, 0, _MAVSerial);


  ALOG_HGL(PSTR(DEBUG_INSERT_PAGE_BREAK "Send_MAVLink_Stream_Pack__Enable_All"));

}

// void mMAVLink_Decoder::disableAllDataStreams(uint8_t system_id, uint8_t component_id, uint8_t target_system, uint8_t target_component, HardwareSerial* serial) {
//     mavlink_message_t msg;
//     uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    
//     mavlink_msg_request_data_stream_pack(system_id, component_id, &msg, target_system, target_component, MAV_DATA_STREAM_ALL, 0, 1);
//     uint16_t len = mavlink_msg_to_send_buffer(buffer, &msg);
//     serial->write(buffer, len);
//     Serial.println("Disabled all data streams.");
// }


// Function to request data stream with a specific rate
void mMAVLink_Decoder::requestDataStream(uint8_t system_id, uint8_t component_id, uint8_t target_system, uint8_t target_component, uint8_t stream_id, uint16_t message_rate, uint8_t start_stop, HardwareSerial* serial) {
    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    mavlink_msg_request_data_stream_pack(system_id, component_id, &msg, target_system, target_component, stream_id, message_rate, start_stop);
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    serial->write(buf, len);
}
// Function to disable all data streams
void mMAVLink_Decoder::disableAllDataStreams(uint8_t system_id, uint8_t component_id, uint8_t target_system, uint8_t target_component, HardwareSerial* serial) {
    // Stop all streams by setting their rate to 0
    requestDataStream(system_id, component_id, target_system, target_component, MAV_DATA_STREAM_ALL, 0, 0, serial);
}



void mMAVLink_Decoder::setMessageRate(uint8_t system_id, uint8_t component_id, uint8_t target_system, uint8_t target_component, uint16_t message_id, uint32_t interval_us, HardwareSerial* serial) {
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    
    mavlink_msg_message_interval_pack(system_id, component_id, &msg, message_id, interval_us);
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &msg);
    serial->write(buffer, len);
    Serial.print("Set message rate for message ID ");
    Serial.print(message_id);
    Serial.print(" to ");
    Serial.print(interval_us);
    Serial.println(" us.");
}


void mMAVLink_Decoder::Maintain_Connection()
{

  if(!_MAVSerial){
    ALOG_ERR(PSTR("No Serial"));
    return;
  }

  if(mTime::TimeReached(&connection.timereached_heartbeat, 5000))
  {
    // Send_Heartbeat();
    Send_MAVLink_Stream_Pack__Enable_All();
    
    /**
     * @brief Send to config stream if no X has been received
     * 
     */
    PacketData* packet_ahrs = findPacketData(MAVLINK_MSG_ID_AHRS2);
    if (packet_ahrs != nullptr && packet_ahrs->data != nullptr) {
        if (abs(millis() - packet_ahrs->tUpdate) > 30000) {
            Send_MAVLink_Stream_Pack__Enable_All();
        }
    }

  }

  PollMAVLink_Stream();  

}


void mMAVLink_Decoder::PollMAVLink_Stream()
{
    
  while(_MAVSerial->available() > 0)
  {
    // Serial.println("PollMAVLink_Stream");  
    mavlink_message_t msg;
    mavlink_status_t status1;
    uint8_t ch = _MAVSerial->read();    
    if(mavlink_parse_char(MAVLINK_COMM_0, ch, &msg, &status1))
    {
      /**
       * @brief Debugging to check what the connected device settings are
       * This may be the missing step required for my future commands back to work, to update the saved information.
       */
      if (msg.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
          uint8_t system_id = msg.sysid;
          uint8_t component_id = msg.compid;
          
          Serial.print("Received HEARTBEAT from system ID: ");
          Serial.print(system_id);
          Serial.print(", component ID: ");
          Serial.println(component_id);
      }

      tSaved_Last_Response = millis();
      handleMessage(msg);
    }
  }

}

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
void mMAVLink_Decoder::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  JsonParserObject jobj = 0;
 
}


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mMAVLink_Decoder::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();


  return JBI->End();

}

uint8_t mMAVLink_Decoder::ConstructJSON_Overview_01(uint8_t json_level, bool json_appending) {
  JBI->Start();

  // Heartbeat
  PacketData* packet_heartbeat = findPacketData(MAVLINK_MSG_ID_HEARTBEAT);
  if (packet_heartbeat != nullptr && packet_heartbeat->data != nullptr) {
    mavlink_heartbeat_t* data = static_cast<mavlink_heartbeat_t*>(packet_heartbeat->data);
    JBI->Object_Start("heartbeat");
    JBI->Add("t", millis() - packet_heartbeat->tUpdate);
    JBI->Add("system_status", data->system_status);
    JBI->Object_End();
  }

  // Statustext
  PacketData* packet_statustext = findPacketData(MAVLINK_MSG_ID_STATUSTEXT);
  if (packet_statustext != nullptr && packet_statustext->data != nullptr) {
    mavlink_statustext_t* data = static_cast<mavlink_statustext_t*>(packet_statustext->data);
    JBI->Object_Start("statustext");
    JBI->Add("severity", data->severity);
    // JBI->Add("text", data->text);
    JBI->Object_End();
  }

  // AHRS2
  PacketData* packet_ahrs2 = findPacketData(MAVLINK_MSG_ID_AHRS2);
  if (packet_ahrs2 != nullptr && packet_ahrs2->data != nullptr) {
    mavlink_ahrs2_t* data = static_cast<mavlink_ahrs2_t*>(packet_ahrs2->data);
    JBI->Object_Start("ahrs2");
    Serial.println(data->roll);
    JBI->Add("roll", data->roll);
    JBI->Add("pitch", data->pitch);
    JBI->Add("yaw", data->yaw);
    JBI->Add("altitude", data->altitude);
    #ifdef ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
    JBI->Add("lat", float(data->lat) / 10000000);
    #else
    JBI->Add("lat", data->lat);
    #endif
    #ifdef ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
    JBI->Add("lng", float(data->lng) / 10000000);
    #else
    JBI->Add("lng", data->lng);
    #endif
    JBI->Object_End();
  }

  // Attitude
  PacketData* packet_attitude = findPacketData(MAVLINK_MSG_ID_ATTITUDE);
  if (packet_attitude != nullptr && packet_attitude->data != nullptr) {
    mavlink_attitude_t* data = static_cast<mavlink_attitude_t*>(packet_attitude->data);
    JBI->Object_Start("attitude");
    JBI->Add("roll", data->roll);
    JBI->Add("pitch", data->pitch);
    JBI->Add("yaw", data->yaw);
    JBI->Add("rollspeed", data->rollspeed);
    JBI->Add("pitchspeed", data->pitchspeed);
    JBI->Add("yawspeed", data->yawspeed);
    JBI->Object_End();
  }

  // Battery Status
  PacketData* packet_battery_status = findPacketData(MAVLINK_MSG_ID_BATTERY_STATUS);
  if (packet_battery_status != nullptr && packet_battery_status->data != nullptr) {
    mavlink_battery_status_t* data = static_cast<mavlink_battery_status_t*>(packet_battery_status->data);
    JBI->Object_Start("battery_status");
    JBI->Add("current_consumed", data->current_consumed);
    JBI->Add("energy_consumed", data->energy_consumed);
    JBI->Add("temperature", data->temperature);
    JBI->Array_AddArray("voltages", data->voltages, ARRAY_SIZE(data->voltages));
    JBI->Add("current_battery", data->current_battery);
    JBI->Add("id", data->id);
    JBI->Add("battery_function", data->battery_function);
    JBI->Add("type", data->type);
    JBI->Add("battery_remaining", data->battery_remaining);
    JBI->Add("time_remaining", data->time_remaining);
    JBI->Add("charge_state", data->charge_state);
    JBI->Object_End();
  }

  // GoPro Heartbeat
  PacketData* packet_gopro_heartbeat = findPacketData(MAVLINK_MSG_ID_GOPRO_HEARTBEAT);
  if (packet_gopro_heartbeat != nullptr && packet_gopro_heartbeat->data != nullptr) {
    mavlink_gopro_heartbeat_t* data = static_cast<mavlink_gopro_heartbeat_t*>(packet_gopro_heartbeat->data);
    JBI->Object_Start("gopro_heartbeat");
    JBI->Add("status", data->status);
    JBI->Add("capture_mode", data->capture_mode);
    JBI->Object_End();
  }

  // GPS Raw Int
  PacketData* packet_gps_raw_int = findPacketData(MAVLINK_MSG_ID_GPS_RAW_INT);
  if (packet_gps_raw_int != nullptr && packet_gps_raw_int->data != nullptr) {
    mavlink_gps_raw_int_t* data = static_cast<mavlink_gps_raw_int_t*>(packet_gps_raw_int->data);
    JBI->Object_Start("gps_raw_int");
    JBI->Add("lat", data->lat);
    JBI->Add("lon", data->lon);
    JBI->Add("alt", data->alt);
    JBI->Add("eph", data->eph);
    JBI->Add("epv", data->epv);
    JBI->Add("vel", data->vel);
    JBI->Add("cog", data->cog);
    JBI->Add("fix_type", data->fix_type);
    JBI->Add("satellites_visible", data->satellites_visible);
    JBI->Add("alt_ellipsoid", data->alt_ellipsoid);
    JBI->Add("h_acc", data->h_acc);
    JBI->Add("v_acc", data->v_acc);
    JBI->Add("vel_acc", data->vel_acc);
    JBI->Add("hdg_acc", data->hdg_acc);
    JBI->Object_End();
  }

  // HW Status
  PacketData* packet_hwstatus = findPacketData(MAVLINK_MSG_ID_HWSTATUS);
  if (packet_hwstatus != nullptr && packet_hwstatus->data != nullptr) {
    mavlink_hwstatus_t* data = static_cast<mavlink_hwstatus_t*>(packet_hwstatus->data);
    JBI->Object_Start("hwstatus");
    JBI->Add("Vcc", data->Vcc);
    JBI->Object_End();
  }

  // Meminfo
  PacketData* packet_meminfo = findPacketData(MAVLINK_MSG_ID_MEMINFO);
  if (packet_meminfo != nullptr && packet_meminfo->data != nullptr) {
    mavlink_meminfo_t* data = static_cast<mavlink_meminfo_t*>(packet_meminfo->data);
    JBI->Object_Start("meminfo");
    JBI->Add("brkval", data->brkval);
    JBI->Add("freemem", data->freemem);
    JBI->Add("freemem32", data->freemem32);
    JBI->Object_End();
  }

  // Mission Current
  PacketData* packet_mission_current = findPacketData(MAVLINK_MSG_ID_MISSION_CURRENT);
  if (packet_mission_current != nullptr && packet_mission_current->data != nullptr) {
    mavlink_mission_current_t* data = static_cast<mavlink_mission_current_t*>(packet_mission_current->data);
    JBI->Object_Start("mission_current");
    JBI->Add("seq", data->seq);
    JBI->Object_End();
  }

  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_Overview_02(uint8_t json_level, bool json_appending) 
{
  JBI->Start();

  // Nav Controller Output
  PacketData* packet_nav_controller_output = findPacketData(MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT);
  if (packet_nav_controller_output != nullptr && packet_nav_controller_output->data != nullptr) {
    mavlink_nav_controller_output_t* data = static_cast<mavlink_nav_controller_output_t*>(packet_nav_controller_output->data);
    JBI->Object_Start("nav_controller_output");
    JBI->Add("nav_roll", data->nav_roll);
    JBI->Add("nav_pitch", data->nav_pitch);
    JBI->Add("alt_error", data->alt_error);
    JBI->Add("aspd_error", data->aspd_error);
    JBI->Add("xtrack_error", data->xtrack_error);
    JBI->Add("nav_bearing", data->nav_bearing);
    JBI->Add("target_bearing", data->target_bearing);
    JBI->Add("wp_dist", data->wp_dist);
    JBI->Object_End();
  }

  // RC Channels
  PacketData* packet_rc_channels = findPacketData(MAVLINK_MSG_ID_RC_CHANNELS);
  if (packet_rc_channels != nullptr && packet_rc_channels->data != nullptr) {
    mavlink_rc_channels_t* data = static_cast<mavlink_rc_channels_t*>(packet_rc_channels->data);
    JBI->Object_Start("rc_channels");
    JBI->Add("chancount", data->chancount);
    JBI->Add("rssi", data->rssi);
    JBI->Object_End();
  }

  // Scaled IMU2
  PacketData* packet_scaled_imu2 = findPacketData(MAVLINK_MSG_ID_SCALED_IMU2);
  if (packet_scaled_imu2 != nullptr && packet_scaled_imu2->data != nullptr) {
    mavlink_scaled_imu2_t* data = static_cast<mavlink_scaled_imu2_t*>(packet_scaled_imu2->data);
    JBI->Object_Start("scaled_imu2");
    JBI->Add("xacc", data->xacc);
    JBI->Add("yacc", data->yacc);
    JBI->Add("zacc", data->zacc);
    JBI->Add("xgyro", data->xgyro);
    JBI->Add("ygyro", data->ygyro);
    JBI->Add("zgyro", data->zgyro);
    JBI->Add("xmag", data->xmag);
    JBI->Add("ymag", data->ymag);
    JBI->Add("zmag", data->zmag);
    JBI->Object_End();
  }

  // Scaled Pressure
  PacketData* packet_scaled_pressure = findPacketData(MAVLINK_MSG_ID_SCALED_PRESSURE);
  if (packet_scaled_pressure != nullptr && packet_scaled_pressure->data != nullptr) {
    mavlink_scaled_pressure_t* data = static_cast<mavlink_scaled_pressure_t*>(packet_scaled_pressure->data);
    JBI->Object_Start("scaled_pressure");
    JBI->Add("press_abs", data->press_abs);
    JBI->Add("press_diff", data->press_diff);
    JBI->Add("temperature", data->temperature);
    JBI->Object_End();
  }

  // Sys Status
  PacketData* packet_sys_status = findPacketData(MAVLINK_MSG_ID_SYS_STATUS);
  if (packet_sys_status != nullptr && packet_sys_status->data != nullptr) {
    mavlink_sys_status_t* data = static_cast<mavlink_sys_status_t*>(packet_sys_status->data);
    JBI->Object_Start("sys_status");
    JBI->Add("sensorsC", data->onboard_control_sensors_present);
    JBI->Add("sensorsE", data->onboard_control_sensors_enabled);
    JBI->Add("sensorsH", data->onboard_control_sensors_health);
    JBI->Add("load", data->load);
    JBI->Add("voltage_battery", data->voltage_battery);
    JBI->Add("current_battery", data->current_battery);
    JBI->Add("drop_rate_comm", data->drop_rate_comm);
    JBI->Add("errors_comm", data->errors_comm);
    JBI->Add("battery_remaining", data->battery_remaining);
    JBI->Object_End();
  }

  // Terrain Report
  PacketData* packet_terrain_report = findPacketData(MAVLINK_MSG_ID_TERRAIN_REPORT);
  if (packet_terrain_report != nullptr && packet_terrain_report->data != nullptr) {
    mavlink_terrain_report_t* data = static_cast<mavlink_terrain_report_t*>(packet_terrain_report->data);
    JBI->Object_Start("terrain_report");
    JBI->Add("lat", data->lat);
    JBI->Add("lon", data->lon);
    JBI->Add("terrain_height", data->terrain_height);
    JBI->Add("current_height", data->current_height);
    JBI->Add("spacing", data->spacing);
    JBI->Add("pending", data->pending);
    JBI->Add("loaded", data->loaded);
    JBI->Object_End();
  }

  // VFR HUD
  PacketData* packet_vfr_hud = findPacketData(MAVLINK_MSG_ID_VFR_HUD);
  if (packet_vfr_hud != nullptr && packet_vfr_hud->data != nullptr) {
    mavlink_vfr_hud_t* data = static_cast<mavlink_vfr_hud_t*>(packet_vfr_hud->data);
    JBI->Object_Start("vfr_hud");
    JBI->Add("airspeed", data->airspeed);
    JBI->Add("groundspeed", data->groundspeed);
    JBI->Add("alt", data->alt);
    JBI->Add("climb", data->climb);
    JBI->Add("heading", data->heading);
    JBI->Add("throttle", data->throttle);
    JBI->Object_End();
  }

  return JBI->End();
}

#ifdef ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS

uint8_t mMAVLink_Decoder::ConstructJSON_ahrs(uint8_t json_level, bool json_appending)
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_AHRS2);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_ahrs_t* data = static_cast<mavlink_ahrs_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("omegaIx", data->omegaIx);
    JBI->Add("omegaIy", data->omegaIy);
    JBI->Add("omegaIz", data->omegaIz);
    JBI->Add("accel_weight", data->accel_weight);
    JBI->Add("renorm_val", data->renorm_val);
    JBI->Add("error_rp", data->error_rp);
    JBI->Add("error_yaw", data->error_yaw);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_ahrs2(uint8_t json_level, bool json_appending)
{

  PacketData* packet = findPacketData(MAVLINK_MSG_ID_AHRS2);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_ahrs2_t* data = static_cast<mavlink_ahrs2_t*>(packet->data);

  JBI->Start();
    JBI->Add("t_test", millis() - packet->tUpdate);
    JBI->Add("roll", data->roll);
    JBI->Add("pitch", data->pitch);
    JBI->Add("yaw", data->yaw);
    JBI->Add("altitude", data->altitude);
    JBI->Add("lat", float(data->lat) / 10000000);
    JBI->Add("lng", float(data->lng) / 10000000);
  return JBI->End();

}

uint8_t mMAVLink_Decoder::ConstructJSON_Debug_ReceiveStats(uint8_t json_level, bool json_appending) 
{
  JBI->Start();
    JBI->Object_Start("ElapsedMessageTime");
      for (const auto& packet : packets) {
        JBI->Add(packet.packet_name, millis() - packet.tUpdate);
      }
    JBI->Object_End();
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_attitude(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_ATTITUDE);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_attitude_t* data = static_cast<mavlink_attitude_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_boot_ms", data->time_boot_ms);
    JBI->Add("roll", data->roll);/*< Roll angle (rad, -pi..+pi)*/
    JBI->Add("pitch", data->pitch); /*< Pitch angle (rad, -pi..+pi)*/
    JBI->Add("yaw", data->yaw);/*< Yaw angle (rad, -pi..+pi)*/
    JBI->Add("rollspeed", data->rollspeed);/*< Roll angular speed (rad/s)*/
    JBI->Add("pitchspeed", data->pitchspeed);/*< Pitch angular speed (rad/s)*/
    JBI->Add("yawspeed", data->yawspeed);/*< Yaw angular speed (rad/s)*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_autopilot_version(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_AUTOPILOT_VERSION);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_autopilot_version_t* data = static_cast<mavlink_autopilot_version_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("capabilities", data->capabilities);
    JBI->Add("uid", data->uid);
    JBI->Add("flight_sw_version", data->flight_sw_version);
    JBI->Add("middleware_sw_version", data->middleware_sw_version);
    JBI->Add("os_sw_version", data->os_sw_version);
    JBI->Add("board_version", data->board_version);
    JBI->Add("vendor_id", data->vendor_id);
    JBI->Add("product_id", data->product_id);
    JBI->Array_AddArray("flight_custom_version", data->flight_custom_version, ARRAY_SIZE(data->flight_custom_version));
    JBI->Array_AddArray("middleware_custom_version", data->middleware_custom_version, ARRAY_SIZE(data->middleware_custom_version));
    JBI->Array_AddArray("os_custom_version", data->os_custom_version, ARRAY_SIZE(data->os_custom_version));
    JBI->Array_AddArray("uid2", data->uid2, ARRAY_SIZE(data->uid2));
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_battery_status(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_BATTERY_STATUS);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_battery_status_t* data = static_cast<mavlink_battery_status_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("current_consumed", data->current_consumed); /*< Consumed charge, in milliampere hours (1 = 1 mAh), -1: autopilot does not provide mAh consumption estimate*/
    JBI->Add("energy_consumed", data->energy_consumed);/*< Consumed energy, in HectoJoules (intergrated U*I*dt)  (1 = 100 Joule), -1: autopilot does not provide energy consumption estimate*/
    JBI->Add("temperature", data->temperature);/*< Temperature of the battery in centi-degrees celsius. INT16_MAX for unknown temperature.*/
    JBI->Array_AddArray("voltages", data->voltages, ARRAY_SIZE(data->voltages));/*< Battery voltage of cells, in millivolts (1 = 1 millivolt). Cells above the valid cell count for this battery should have the UINT16_MAX value.*/
    JBI->Add("current_battery", data->current_battery);/*< Battery current, in 10*milliamperes (1 = 10 milliampere), -1: autopilot does not measure the current*/
    JBI->Add("id", data->id);/*< Battery ID*/
    JBI->Add("battery_function", data->battery_function);/*< Function of the battery*/
    JBI->Add("type", data->type);/*< Type (chemistry) of the battery*/
    JBI->Add("battery_remaining", data->battery_remaining);/*< Remaining battery energy: (0%: 0, 100%: 100), -1: autopilot does not estimate the remaining battery*/
    JBI->Add("time_remaining", data->time_remaining);/*< Remaining battery time, in seconds (1 = 1s = 0% energy left), 0: autopilot does not provide remaining battery time estimate*/
    JBI->Add("charge_state", data->charge_state);/*< State for extent of discharge, provided by autopilot for warning or external reactions*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_ekf_status_report(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_EKF_STATUS_REPORT);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_ekf_status_report_t* data = static_cast<mavlink_ekf_status_report_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("velocity_variance", data->velocity_variance);
    JBI->Add("pos_horiz_variance", data->pos_horiz_variance);
    JBI->Add("pos_vert_variance", data->pos_vert_variance);
    JBI->Add("compass_variance", data->compass_variance);
    JBI->Add("terrain_alt_variance", data->terrain_alt_variance);
    JBI->Add("flags", data->flags);
    JBI->Add("airspeed_variance", data->airspeed_variance);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_fence_status(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_FENCE_STATUS);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_fence_status_t* data = static_cast<mavlink_fence_status_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("breach_time", data->breach_time);/*< time of last breach in milliseconds since boot*/
    JBI->Add("breach_count", data->breach_count);/*< number of fence breaches*/
    JBI->Add("breach_status", data->breach_status);/*< 0 if currently inside fence, 1 if outside*/
    JBI->Add("breach_type", data->breach_type);/*< last breach type (see FENCE_BREACH_* enum)*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_gimbal_report(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_GIMBAL_REPORT);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_gimbal_report_t* data = static_cast<mavlink_gimbal_report_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("delta_time", data->delta_time);
    JBI->Add("delta_angle_x", data->delta_angle_x);
    JBI->Add("delta_angle_y", data->delta_angle_y);
    JBI->Add("delta_angle_z", data->delta_angle_z);
    JBI->Add("delta_velocity_x", data->delta_velocity_x);
    JBI->Add("delta_velocity_y", data->delta_velocity_y);
    JBI->Add("delta_velocity_z", data->delta_velocity_z);
    JBI->Add("joint_roll", data->joint_roll);/*< Joint ROLL (radians)*/
    JBI->Add("joint_el", data->joint_el); /*< Joint EL (radians)*/
    JBI->Add("joint_az", data->joint_az);/*< Joint AZ (radians)*/
    JBI->Add("target_system", data->target_system);
    JBI->Add("target_component", data->target_component);
  return JBI->End();
}
uint8_t mMAVLink_Decoder::ConstructJSON_gimbal_torque_cmd_report(uint8_t json_level, bool json_appending) {
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_GIMBAL_TORQUE_CMD_REPORT);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_gimbal_torque_cmd_report_t* data = static_cast<mavlink_gimbal_torque_cmd_report_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("rl_torque_cmd", data->rl_torque_cmd);
    JBI->Add("el_torque_cmd", data->el_torque_cmd);
    JBI->Add("az_torque_cmd", data->az_torque_cmd);
    JBI->Add("target_system", data->target_system);
    JBI->Add("target_component", data->target_component);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_global_position_int(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_GLOBAL_POSITION_INT);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_global_position_int_t* data = static_cast<mavlink_global_position_int_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_boot_ms", data->time_boot_ms);
    JBI->Add("lat", data->lat);/*< Latitude, expressed as degrees * 1E7*/
    JBI->Add("lon", data->lon);/*< Longitude, expressed as degrees * 1E7*/
    JBI->Add("alt", data->alt);/*< Altitude in meters, expressed as * 1000 (millimeters), AMSL (not WGS84 - note that virtually all GPS modules provide the AMSL as well)*/
    JBI->Add("relative_alt", data->relative_alt);/*< Altitude above ground in meters, expressed as * 1000 (millimeters)*/
    JBI->Add("vx", data->vx);/*< Ground X Speed (Latitude, positive north), expressed as m/s * 100*/
    JBI->Add("vy", data->vy); /*< Ground Y Speed (Longitude, positive east), expressed as m/s * 100*/
    JBI->Add("vz", data->vz);/*< Ground Z Speed (Altitude, positive down), expressed as m/s * 100*/
    JBI->Add("hdg", data->hdg);/*< Vehicle heading (yaw angle) in degrees * 100, 0.0..359.99 degrees. If unknown, set to: UINT16_MAX*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_gopro_heartbeat(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_GOPRO_HEARTBEAT);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_gopro_heartbeat_t* data = static_cast<mavlink_gopro_heartbeat_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("status", data->status);
    JBI->Add("capture_mode", data->capture_mode);
    JBI->Add("flags", data->flags);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_gps_global_origin(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_GPS_GLOBAL_ORIGIN);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_gps_global_origin_t* data = static_cast<mavlink_gps_global_origin_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("latitude", data->latitude); /*< Latitude (WGS84), in degrees * 1E7*/
    JBI->Add("longitude", data->longitude); /*< Longitude (WGS84), in degrees * 1E7*/
    JBI->Add("altitude", data->altitude); /*< Altitude (AMSL), in meters * 1000 (positive for up)*/
    JBI->Add("time_usec", data->time_usec); /*< Timestamp (microseconds since UNIX epoch or microseconds since system boot)*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_gps_raw_int(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_GPS_RAW_INT);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_gps_raw_int_t* data = static_cast<mavlink_gps_raw_int_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_usec", data->time_usec);
    JBI->Add("lat", data->lat);/*< Latitude (WGS84, EGM96 ellipsoid), in degrees * 1E7*/
    JBI->Add("lon", data->lon);/*< Longitude (WGS84, EGM96 ellipsoid), in degrees * 1E7*/
    JBI->Add("alt", data->alt);/*< Altitude (AMSL, NOT WGS84), in meters * 1000 (positive for up). Note that virtually all GPS modules provide the AMSL altitude in addition to the WGS84 altitude.*/
    JBI->Add("eph", data->eph);/*< GPS HDOP horizontal dilution of position (unitless). If unknown, set to: UINT16_MAX*/
    JBI->Add("epv", data->epv);/*< GPS VDOP vertical dilution of position (unitless). If unknown, set to: UINT16_MAX*/
    JBI->Add("vel", data->vel); /*< GPS ground speed (m/s * 100). If unknown, set to: UINT16_MAX*/
    JBI->Add("cog", data->cog);/*< Course over ground (NOT heading, but direction of movement) in degrees * 100, 0.0..359.99 degrees. If unknown, set to: UINT16_MAX*/
    JBI->Add("fix_type", data->fix_type); /*< See the GPS_FIX_TYPE enum.*/
    JBI->Add("satellites_visible", data->satellites_visible);/*< Number of satellites visible. If unknown, set to 255*/
    JBI->Add("alt_ellipsoid", data->alt_ellipsoid);/*< Altitude (above WGS84, EGM96 ellipsoid), in meters * 1000 (positive for up).*/
    JBI->Add("h_acc", data->h_acc);/*< Position uncertainty in meters * 1000 (positive for up).*/
    JBI->Add("v_acc", data->v_acc);/*< Altitude uncertainty in meters * 1000 (positive for up).*/
    JBI->Add("vel_acc", data->vel_acc);/*< Speed uncertainty in meters * 1000 (positive for up).*/
    JBI->Add("hdg_acc", data->hdg_acc);/*< Heading / track uncertainty in degrees * 1e5.*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_heartbeat(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_HEARTBEAT);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_heartbeat_t* data = static_cast<mavlink_heartbeat_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("custom_mode", data->custom_mode);/*< A bitfield for use for autopilot-specific flags*/
    JBI->Add("type", data->type);/*< Type of the MAV (quadrotor, helicopter, etc., up to 15 types, defined in MAV_TYPE ENUM)*/
    JBI->Add("autopilot", data->autopilot);/*< Autopilot type / class. defined in MAV_AUTOPILOT ENUM*/
    JBI->Add("base_mode", data->base_mode);/*< System mode bitfield, as defined by MAV_MODE_FLAG enum*/
    JBI->Add("system_status", data->system_status); /*< System status flag, as defined by MAV_STATE enum*/
    JBI->Add("mavlink_version", data->mavlink_version);/*< MAVLink version, not writable by user, gets added by protocol because of magic data type: uint8_t_mavlink_version*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_home_position(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_HOME_POSITION);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_home_position_t* data = static_cast<mavlink_home_position_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("latitude", data->latitude); /*< Latitude (WGS84), in degrees * 1E7*/
    JBI->Add("longitude", data->longitude); /*< Longitude (WGS84, in degrees * 1E7*/
    JBI->Add("altitude", data->altitude); /*< Altitude (AMSL), in meters * 1000 (positive for up)*/
    JBI->Add("x", data->x); /*< Local X position of this position in the local coordinate frame*/
    JBI->Add("y", data->y); /*< Local Y position of this position in the local coordinate frame*/
    JBI->Add("z", data->z); /*< Local Z position of this position in the local coordinate frame*/
    JBI->Array_AddArray("q", data->q, ARRAY_SIZE(data->q)); /*< World to surface normal and heading transformation of the takeoff position. Used to indicate the heading and slope of the ground*/
    JBI->Add("approach_x", data->approach_x); /*< Local X position of the end of the approach vector. Multicopters should set this position based on their takeoff path. Grass-landing fixed wing aircraft should set it the same way as multicopters. Runway-landing fixed wing aircraft should set it to the opposite direction of the takeoff, assuming the takeoff happened from the threshold / touchdown zone.*/
    JBI->Add("approach_y", data->approach_y); /*< Local Y position of the end of the approach vector. Multicopters should set this position based on their takeoff path. Grass-landing fixed wing aircraft should set it the same way as multicopters. Runway-landing fixed wing aircraft should set it to the opposite direction of the takeoff, assuming the takeoff happened from the threshold / touchdown zone.*/
    JBI->Add("approach_z", data->approach_z); /*< Local Z position of the end of the approach vector. Multicopters should set this position based on their takeoff path. Grass-landing fixed wing aircraft should set it the same way as multicopters. Runway-landing fixed wing aircraft should set it to the opposite direction of the takeoff, assuming the takeoff happened from the threshold / touchdown zone.*/
    JBI->Add("time_usec", data->time_usec); /*< Timestamp (microseconds since UNIX epoch or microseconds since system boot)*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_hwstatus(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_HWSTATUS);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_hwstatus_t* data = static_cast<mavlink_hwstatus_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("Vcc", data->Vcc);/*< board voltage (mV)*/
    JBI->Add("I2Cerr", data->I2Cerr); /*< I2C error count*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_local_position_ned(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_LOCAL_POSITION_NED);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_local_position_ned_t* data = static_cast<mavlink_local_position_ned_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_boot_ms", data->time_boot_ms); /*< Timestamp (milliseconds since system boot)*/
    JBI->Add("x", data->x); /*< X Position*/
    JBI->Add("y", data->y); /*< Y Position*/
    JBI->Add("z", data->z); /*< Z Position*/
    JBI->Add("vx", data->vx);/*< X Speed*/
    JBI->Add("vy", data->vy); /*< Y Speed*/
    JBI->Add("vz", data->vz); /*< Z Speed*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_meminfo(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_MEMINFO);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_meminfo_t* data = static_cast<mavlink_meminfo_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("brkval", data->brkval);/*< heap top*/
    JBI->Add("freemem", data->freemem);/*< free memory*/
    JBI->Add("freemem32", data->freemem32);/*< free memory (32 bit)*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_mission_current(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_MISSION_CURRENT);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_mission_current_t* data = static_cast<mavlink_mission_current_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("seq", data->seq);/*< Sequence*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_mount_status(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_MOUNT_STATUS);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_mount_status_t* data = static_cast<mavlink_mount_status_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("pointing_a", data->pointing_a);
    JBI->Add("pointing_b", data->pointing_b);
    JBI->Add("pointing_c", data->pointing_c);
    JBI->Add("target_system", data->target_system);
    JBI->Add("target_component", data->target_component);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_nav_controller_output(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_nav_controller_output_t* data = static_cast<mavlink_nav_controller_output_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("nav_roll", data->nav_roll);/*< Current desired roll in degrees*/
    JBI->Add("nav_pitch", data->nav_pitch); /*< Current desired pitch in degrees*/
    JBI->Add("alt_error", data->alt_error);/*< Current altitude error in meters*/
    JBI->Add("aspd_error", data->aspd_error);/*< Current airspeed error in meters/second*/
    JBI->Add("xtrack_error", data->xtrack_error); /*< Current crosstrack error on x-y plane in meters*/
    JBI->Add("nav_bearing", data->nav_bearing);/*< Current desired heading in degrees*/
    JBI->Add("target_bearing", data->target_bearing);/*< Bearing to current waypoint/target in degrees*/
    JBI->Add("wp_dist", data->wp_dist); /*< Distance to active waypoint in meters*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_param_value(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_PARAM_VALUE);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_param_value_t* data = static_cast<mavlink_param_value_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("param_value", data->param_value);
    JBI->Add("param_count", data->param_count);
    JBI->Add("param_index", data->param_index);
    char buffer[16];
    memcpy(buffer, data->param_id, sizeof(data->param_id));
    for(int i = 0; i < sizeof(data->param_id); i++) {
      if(buffer[i] == 0) { buffer[i] = '_'; }
    }
    JBI->Array_AddArray("param_id", buffer, ARRAY_SIZE(data->param_id));
    JBI->Add("param_type", data->param_type);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_power_status(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_POWER_STATUS);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_power_status_t* data = static_cast<mavlink_power_status_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("Vcc", data->Vcc);/*< 5V rail voltage in millivolts*/
    JBI->Add("Vservo", data->Vservo);/*< servo rail voltage in millivolts*/
    JBI->Add("flags", data->flags);/*< power supply status flags (see MAV_POWER_STATUS enum)*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_raw_imu(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_RAW_IMU);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_raw_imu_t* data = static_cast<mavlink_raw_imu_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_usec", data->time_usec);
    JBI->Add("xacc", data->xacc);
    JBI->Add("yacc", data->yacc);
    JBI->Add("zacc", data->zacc);
    JBI->Add("xgyro", data->xgyro);
    JBI->Add("ygyro", data->ygyro);
    JBI->Add("zgyro", data->zgyro);
    JBI->Add("xmag", data->xmag);
    JBI->Add("ymag", data->ymag);
    JBI->Add("zmag", data->zmag);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_rc_channels(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_RC_CHANNELS);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_rc_channels_t* data = static_cast<mavlink_rc_channels_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_boot_ms", data->time_boot_ms);
    JBI->Add("chan1_raw", data->chan1_raw);
    JBI->Add("chan2_raw", data->chan2_raw);
    JBI->Add("chan3_raw", data->chan3_raw);
    JBI->Add("chan4_raw", data->chan4_raw);
    JBI->Add("chan5_raw", data->chan5_raw);
    JBI->Add("chan6_raw", data->chan6_raw);
    JBI->Add("chan7_raw", data->chan7_raw);
    JBI->Add("chan8_raw", data->chan8_raw);
    JBI->Add("chan9_raw", data->chan9_raw);
    JBI->Add("chan10_raw", data->chan10_raw);
    JBI->Add("chan11_raw", data->chan11_raw);
    JBI->Add("chan12_raw", data->chan12_raw);
    JBI->Add("chan13_raw", data->chan13_raw);
    JBI->Add("chan14_raw", data->chan14_raw);
    JBI->Add("chan15_raw", data->chan15_raw);
    JBI->Add("chan16_raw", data->chan16_raw);
    JBI->Add("chan17_raw", data->chan17_raw);
    JBI->Add("chan18_raw", data->chan18_raw);
    JBI->Add("chancount", data->chancount);/*< Total number of RC channels being received. This can be larger than 18, indicating that more channels are available but not given in this message. This value should be 0 when no RC channels are available.*/
    JBI->Add("rssi", data->rssi);/*< Receive signal strength indicator, 0: 0%, 100: 100%, 255: invalid/unknown.*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_rc_channels_raw(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_RC_CHANNELS_RAW);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_rc_channels_raw_t* data = static_cast<mavlink_rc_channels_raw_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_boot_ms", data->time_boot_ms);
    JBI->Add("chan1_raw", data->chan1_raw);
    JBI->Add("chan2_raw", data->chan2_raw);
    JBI->Add("chan3_raw", data->chan3_raw);
    JBI->Add("chan4_raw", data->chan4_raw);
    JBI->Add("chan5_raw", data->chan5_raw);
    JBI->Add("chan6_raw", data->chan6_raw);
    JBI->Add("chan7_raw", data->chan7_raw);
    JBI->Add("chan8_raw", data->chan8_raw);
    JBI->Add("port", data->port);
    JBI->Add("rssi", data->rssi);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_rc_channels_scaled(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_RC_CHANNELS_SCALED);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_rc_channels_scaled_t* data = static_cast<mavlink_rc_channels_scaled_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_boot_ms", data->time_boot_ms); /*< Timestamp (milliseconds since system boot)*/
    JBI->Add("chan1_scaled", data->chan1_scaled); /*< RC channel 1 value scaled, (-100%) -10000, (0%) 0, (100%) 10000, (invalid) INT16_MAX.*/
    JBI->Add("chan2_scaled", data->chan2_scaled); /*< RC channel 2 value scaled, (-100%) -10000, (0%) 0, (100%) 10000, (invalid) INT16_MAX.*/
    JBI->Add("chan3_scaled", data->chan3_scaled); /*< RC channel 3 value scaled, (-100%) -10000, (0%) 0, (100%) 10000, (invalid) INT16_MAX.*/
    JBI->Add("chan4_scaled", data->chan4_scaled); /*< RC channel 4 value scaled, (-100%) -10000, (0%) 0, (100%) 10000, (invalid) INT16_MAX.*/
    JBI->Add("chan5_scaled", data->chan5_scaled); /*< RC channel 5 value scaled, (-100%) -10000, (0%) 0, (100%) 10000, (invalid) INT16_MAX.*/
    JBI->Add("chan6_scaled", data->chan6_scaled); /*< RC channel 6 value scaled, (-100%) -10000, (0%) 0, (100%) 10000, (invalid) INT16_MAX.*/
    JBI->Add("chan7_scaled", data->chan7_scaled); /*< RC channel 7 value scaled, (-100%) -10000, (0%) 0, (100%) 10000, (invalid) INT16_MAX.*/
    JBI->Add("chan8_scaled", data->chan8_scaled); /*< RC channel 8 value scaled, (-100%) -10000, (0%) 0, (100%) 10000, (invalid) INT16_MAX.*/
    JBI->Add("port", data->port); /*< Servo output port (set of 8 outputs = 1 port). Most MAVs will just use one, but this allows for more than 8 servos.*/
    JBI->Add("rssi", data->rssi); /*< Receive signal strength indicator, 0: 0%, 100: 100%, 255: invalid/unknown.*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_request_data_stream(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_REQUEST_DATA_STREAM);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_request_data_stream_t* data = static_cast<mavlink_request_data_stream_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("req_message_rate", data->req_message_rate);
    JBI->Add("target_system", data->target_system);
    JBI->Add("target_component", data->target_component);
    JBI->Add("req_stream_id", data->req_stream_id);
    JBI->Add("start_stop", data->start_stop);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_imu2(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_SCALED_IMU2);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_scaled_imu2_t* data = static_cast<mavlink_scaled_imu2_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_boot_ms", data->time_boot_ms);
    JBI->Add("xacc", data->xacc); /*< X acceleration (mg)*/
    JBI->Add("yacc", data->yacc); /*< y acceleration (mg)*/
    JBI->Add("zacc", data->zacc); /*< Z acceleration (mg)*/
    JBI->Add("xgyro", data->xgyro);/*< Angular speed around X axis (millirad /sec)*/
    JBI->Add("ygyro", data->ygyro);/*< Angular speed around y axis (millirad /sec)*/
    JBI->Add("zgyro", data->zgyro);/*< Angular speed around Z axis (millirad /sec)*/
    JBI->Add("xmag", data->xmag);/*< X Magnetic field (milli tesla)*/
    JBI->Add("ymag", data->ymag);/*<   y Magnetic field (milli tesla)*/
    JBI->Add("zmag", data->zmag);/*< Z Magnetic field (milli tesla)*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_imu3(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_SCALED_IMU3);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_scaled_imu3_t* data = static_cast<mavlink_scaled_imu3_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_boot_ms", data->time_boot_ms);
    JBI->Add("xacc", data->xacc);
    JBI->Add("yacc", data->yacc);
    JBI->Add("zacc", data->zacc);
    JBI->Add("xgyro", data->xgyro);
    JBI->Add("ygyro", data->ygyro);
    JBI->Add("zgyro", data->zgyro);
    JBI->Add("xmag", data->xmag);
    JBI->Add("ymag", data->ymag);
    JBI->Add("zmag", data->zmag);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_scaled_pressure(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_SCALED_PRESSURE);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_scaled_pressure_t* data = static_cast<mavlink_scaled_pressure_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_boot_ms", data->time_boot_ms);
    JBI->Add("press_abs", data->press_abs);/*< Absolute pressure (hectopascal)*/
    JBI->Add("press_diff", data->press_diff);/*< Differential pressure 1 (hectopascal)*/
    JBI->Add("temperature", data->temperature);/*< Temperature measurement (0.01 degrees celsius)*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_scaled_pressure2(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_SCALED_PRESSURE2);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_scaled_pressure2_t* data = static_cast<mavlink_scaled_pressure2_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_boot_ms", data->time_boot_ms);
    JBI->Add("press_abs", data->press_abs);
    JBI->Add("press_diff", data->press_diff);
    JBI->Add("temperature", data->temperature);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_sensor_offsets(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_SENSOR_OFFSETS);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_sensor_offsets_t* data = static_cast<mavlink_sensor_offsets_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("mag_declination", data->mag_declination);
    JBI->Add("raw_press", data->raw_press);
    JBI->Add("raw_temp", data->raw_temp);
    JBI->Add("gyro_cal_x", data->gyro_cal_x);
    JBI->Add("gyro_cal_y", data->gyro_cal_y);
    JBI->Add("gyro_cal_z", data->gyro_cal_z);
    JBI->Add("accel_cal_x", data->accel_cal_x);
    JBI->Add("accel_cal_y", data->accel_cal_y);
    JBI->Add("accel_cal_z", data->accel_cal_z);
    JBI->Add("mag_ofs_x", data->mag_ofs_x);
    JBI->Add("mag_ofs_y", data->mag_ofs_y);
    JBI->Add("mag_ofs_z", data->mag_ofs_z);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_servo_output_raw(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_SERVO_OUTPUT_RAW);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_servo_output_raw_t* data = static_cast<mavlink_servo_output_raw_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_usec", data->time_usec);
    JBI->Add("servo1_raw", data->servo1_raw);
    JBI->Add("servo2_raw", data->servo2_raw);
    JBI->Add("servo3_raw", data->servo3_raw);
    JBI->Add("servo4_raw", data->servo4_raw);
    JBI->Add("servo5_raw", data->servo5_raw);
    JBI->Add("servo6_raw", data->servo6_raw);
    JBI->Add("servo7_raw", data->servo7_raw);
    JBI->Add("servo8_raw", data->servo8_raw);
    JBI->Add("port", data->port);
    JBI->Add("servo9_raw", data->servo9_raw);
    JBI->Add("servo10_raw", data->servo10_raw);
    JBI->Add("servo11_raw", data->servo11_raw);
    JBI->Add("servo12_raw", data->servo12_raw);
    JBI->Add("servo13_raw", data->servo13_raw);
    JBI->Add("servo14_raw", data->servo14_raw);
    JBI->Add("servo15_raw", data->servo15_raw);
    JBI->Add("servo16_raw", data->servo16_raw);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_statustext(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_STATUSTEXT);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_statustext_t* data = static_cast<mavlink_statustext_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("severity", data->severity);/*< Severity of status. Relies on the definitions within RFC-5424. See enum MAV_SEVERITY.*/
    JBI->Add("text", data->text);/*< Status text message, without null termination character*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_sys_status(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_SYS_STATUS);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_sys_status_t* data = static_cast<mavlink_sys_status_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("sensorsC", data->onboard_control_sensors_present);/*< Bitmask showing which onboard controllers and sensors are present. Value of 0: not present. Value of 1: present. Indices defined by ENUM MAV_SYS_STATUS_SENSOR*/
    JBI->Add("sensorsE", data->onboard_control_sensors_enabled);/*< Bitmask showing which onboard controllers and sensors are enabled:  Value of 0: not enabled. Value of 1: enabled. Indices defined by ENUM MAV_SYS_STATUS_SENSOR*/
    JBI->Add("sensorsH", data->onboard_control_sensors_health);/*< Bitmask showing which onboard controllers and sensors are operational or have an error:  Value of 0: not enabled. Value of 1: enabled. Indices defined by ENUM MAV_SYS_STATUS_SENSOR*/
    JBI->Add("load", data->load);/*< Maximum usage in percent of the mainloop time, (0%: 0, 100%: 1000) should be always below 1000*/
    JBI->Add("voltage_battery", data->voltage_battery); /*< Battery voltage, in millivolts (1 = 1 millivolt)*/
    JBI->Add("current_battery", data->current_battery);/*< Battery current, in 10*milliamperes (1 = 10 milliampere), -1: autopilot does not measure the current*/
    JBI->Add("drop_rate_comm", data->drop_rate_comm);/*< Communication drops in percent, (0%: 0, 100%: 10'000), (UART, I2C, SPI, CAN), dropped packets on all links (packets that were corrupted on reception on the MAV)*/
    JBI->Add("errors_comm", data->errors_comm);/*< Communication errors (UART, I2C, SPI, CAN), dropped packets on all links (packets that were corrupted on reception on the MAV)*/
    JBI->Add("errors_count1", data->errors_count1);/*< Autopilot-specific errors*/
    JBI->Add("errors_count2", data->errors_count2);/*< Autopilot-specific errors*/
    JBI->Add("errors_count3", data->errors_count3);/*< Autopilot-specific errors*/
    JBI->Add("errors_count4", data->errors_count4);/*< Autopilot-specific errors*/
    JBI->Add("battery_remaining", data->battery_remaining);/*< Remaining battery energy: (0%: 0, 100%: 100), -1: autopilot estimate the remaining battery*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_system_time(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_SYSTEM_TIME);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_system_time_t* data = static_cast<mavlink_system_time_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_unix_usec", data->time_unix_usec);
    JBI->Add("time_boot_ms", data->time_boot_ms);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_timesync(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_TIMESYNC);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_timesync_t* data = static_cast<mavlink_timesync_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("tc1", data->tc1);
    JBI->Add("ts1", data->ts1);
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_terrain_report(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_TERRAIN_REPORT);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_terrain_report_t* data = static_cast<mavlink_terrain_report_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);    
    JBI->Add("lat", data->lat); /*< Latitude (degrees *10^7)*/
    JBI->Add("lon", data->lon); /*< Longitude (degrees *10^7)*/
    JBI->Add("terrain_height", data->terrain_height); /*< Terrain height in meters AMSL*/
    JBI->Add("current_height", data->current_height); /*< Current vehicle height above lat/lon terrain height (meters)*/
    JBI->Add("spacing", data->spacing); /*< grid spacing (zero if terrain at this location unavailable)*/
    JBI->Add("pending", data->pending); /*< Number of 4x4 terrain blocks waiting to be received or read from disk*/
    JBI->Add("loaded", data->loaded); /*< Number of 4x4 terrain blocks in memory*/
  return JBI->End();
}


uint8_t mMAVLink_Decoder::ConstructJSON_vfr_hud(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_VFR_HUD);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_vfr_hud_t* data = static_cast<mavlink_vfr_hud_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("airspeed", data->airspeed);/*< Current airspeed in m/s*/
    JBI->Add("groundspeed", data->groundspeed);/*< Current ground speed in m/s*/
    JBI->Add("alt", data->alt);/*< Current altitude (MSL), in meters*/
    JBI->Add("climb", data->climb);/*< Current climb rate in meters/second*/
    JBI->Add("heading", data->heading);/*< Current heading in degrees, in compass units (0..360, 0=north)*/
    JBI->Add("throttle", data->throttle);/*< Current throttle setting in integer percent, 0 to 100*/
  return JBI->End();
}

uint8_t mMAVLink_Decoder::ConstructJSON_vibration(uint8_t json_level, bool json_appending) 
{
  PacketData* packet = findPacketData(MAVLINK_MSG_ID_VIBRATION);
  if (packet == nullptr || packet->data == nullptr){ return 0; }
  mavlink_vibration_t* data = static_cast<mavlink_vibration_t*>(packet->data);

  JBI->Start();
    JBI->Add("t", millis() - packet->tUpdate);
    JBI->Add("time_usec", data->time_usec);
    JBI->Add("vibration_x", data->vibration_x);
    JBI->Add("vibration_y", data->vibration_y);
    JBI->Add("vibration_z", data->vibration_z);
    JBI->Add("clipping_0", data->clipping_0);
    JBI->Add("clipping_1", data->clipping_1);
    JBI->Add("clipping_2", data->clipping_2);
  return JBI->End();
}

#endif // ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mMAVLink_Decoder::MQTTHandler_Init()
{

  struct handler<mMAVLink_Decoder>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_overview_01;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_OVERVIEW_01_CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_Overview_01;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_overview_02;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_OVERVIEW_02_CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_Overview_02;
  mqtthandler_list.push_back(ptr);

  #ifdef ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS

  // 163,AHRS
  ptr = &mqtthandler_mavlink_packet__ahrs;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__AHRS__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_ahrs;
  mqtthandler_list.push_back(ptr);

  // 178,AHRS2
  ptr = &mqtthandler_mavlink_packet__ahrs2;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__AHRS2__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_ahrs2;
  mqtthandler_list.push_back(ptr);

  // 30,ATTITUDE
  ptr = &mqtthandler_mavlink_packet__attitude;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_attitude;
  mqtthandler_list.push_back(ptr);

  // 183,AUTOPILOT_VERSION_REQUEST
  ptr = &mqtthandler_mavlink_packet__autopilot_version;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__AUTOPILOT_VERSION_REQUEST__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_autopilot_version;
  mqtthandler_list.push_back(ptr);

  //147,BATTERY_STATUS
  ptr = &mqtthandler_mavlink_packet__battery_status;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__BATTERY_STATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_battery_status;
  mqtthandler_list.push_back(ptr);

  // 193,EKF_STATUS_REPORT
  ptr = &mqtthandler_mavlink_packet__ekf_status_report;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__EKF_STATUS_REPORT__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_ekf_status_report;
  mqtthandler_list.push_back(ptr);

  // 162,FENCE_STATUS
  ptr = &mqtthandler_mavlink_packet__fence_status;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__FENCE_STATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_fence_status;
  mqtthandler_list.push_back(ptr);

  // 200,GIMBAL_REPORT
  ptr = &mqtthandler_mavlink_packet__gimbal_report;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_REPORT__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_gimbal_report;
  mqtthandler_list.push_back(ptr);

  // 214,GIMBAL_TORQUE_CMD_REPORT
  ptr = &mqtthandler_mavlink_packet__gimbal_torque_cmd_report;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_TORQUE_CMD_REPORT__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_gimbal_torque_cmd_report;
  mqtthandler_list.push_back(ptr);

  // 33,GLOBAL_POSITION_INT
  ptr = &mqtthandler_mavlink_packet__global_position_int;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_POSITION_INT__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_global_position_int;
  mqtthandler_list.push_back(ptr);
  
  // 215,GOPRO_HEARTBEAT
  ptr = &mqtthandler_mavlink_packet__gopro_heartbeat;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__GOPRO_HEARTBEAT__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_gopro_heartbeat;
  mqtthandler_list.push_back(ptr);
  
  // 24,GPS_RAW_INT
  ptr = &mqtthandler_mavlink_packet__gps_raw_int;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__GPS_RAW_INT__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_gps_raw_int;
  mqtthandler_list.push_back(ptr);
  
  // 0,HEARTBEAT
  ptr = &mqtthandler_mavlink_packet__heartbeat;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__HEARTBEAT__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_heartbeat;
  mqtthandler_list.push_back(ptr);
  
  // 0,HEARTBEAT
  ptr = &mqtthandler_mavlink_packet__home_position;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__HOME_POSITION__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_home_position;
  mqtthandler_list.push_back(ptr);
  
  // 165,HWSTATUS
  ptr = &mqtthandler_mavlink_packet__hwstatus;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__HWSTATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_hwstatus;
  mqtthandler_list.push_back(ptr);
  
  // 165,HWSTATUS
  ptr = &mqtthandler_mavlink_packet__local_position_ned;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__LOCAL_POSITION_NED__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_local_position_ned;
  mqtthandler_list.push_back(ptr);
  
  // 152,MEMINFO
  ptr = &mqtthandler_mavlink_packet__meminfo;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__MEMINFO__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_meminfo;
  mqtthandler_list.push_back(ptr);
  
  // 42,MISSION_CURRENT
  ptr = &mqtthandler_mavlink_packet__mission_current;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__MISSION_CURRENT__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_mission_current;
  mqtthandler_list.push_back(ptr);
  
  // 158,MOUNT_STATUS
  ptr = &mqtthandler_mavlink_packet__mount_status;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__MOUNT_STATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_mount_status;
  mqtthandler_list.push_back(ptr);
  
  // 62,NAV_CONTROLLER_OUTPUT
  ptr = &mqtthandler_mavlink_packet__nav_controller_output;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__NAV_CONTROLLER_OUTPUT__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_nav_controller_output;
  mqtthandler_list.push_back(ptr);
  
  // 22,PARAM_VALUE
  ptr = &mqtthandler_mavlink_packet__param_value;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__PARAM_VALUE__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_param_value;
  mqtthandler_list.push_back(ptr);
  
  // 125,POWER_STATUS
  ptr = &mqtthandler_mavlink_packet__power_status;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__POWER_STATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_power_status;
  mqtthandler_list.push_back(ptr);
  
  // 27,RAW_IMU
  ptr = &mqtthandler_mavlink_packet__raw_imu;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__RAW_IMU__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_raw_imu;   
  mqtthandler_list.push_back(ptr);
  
  // 65,RC_CHANNELS
  ptr = &mqtthandler_mavlink_packet__rc_channels;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_rc_channels;
  mqtthandler_list.push_back(ptr);
  
  // 35,RC_CHANNELS_RAW
  ptr = &mqtthandler_mavlink_packet__rc_channels_raw;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_RAW__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_rc_channels_raw;
  mqtthandler_list.push_back(ptr);
  
  // 34,RC_CHANNELS_SCALED
  ptr = &mqtthandler_mavlink_packet__rc_channels_scaled;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_SCALED__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_rc_channels_scaled;
  mqtthandler_list.push_back(ptr);
  
  // 66,REQUEST_DATA_STREAM
  ptr = &mqtthandler_mavlink_packet__request_data_stream;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__REQUEST_DATA_STREAM__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_request_data_stream;
  mqtthandler_list.push_back(ptr);
  
  // 116,SCALED_IMU2
  ptr = &mqtthandler_mavlink_packet__scaled_imu2;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU2__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_imu2;
  mqtthandler_list.push_back(ptr);
  
  // 129,SCALED_IMU3
  ptr = &mqtthandler_mavlink_packet__scaled_imu3;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU3__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_imu3;
  mqtthandler_list.push_back(ptr);
  
  // 29,SCALED_PRESSURE
  ptr = &mqtthandler_mavlink_packet__scaled_pressure;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_scaled_pressure;
  mqtthandler_list.push_back(ptr);
  
  // 137,SCALED_PRESSURE2
  ptr = &mqtthandler_mavlink_packet__scaled_pressure2;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE2__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_scaled_pressure2;
  mqtthandler_list.push_back(ptr);
  
  // 150,SENSOR_OFFSETS
  ptr = &mqtthandler_mavlink_packet__sensor_offsets;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SENSOR_OFFSETS__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_sensor_offsets;
  mqtthandler_list.push_back(ptr);
  
  // 36,SERVO_OUTPUT_RAW
  ptr = &mqtthandler_mavlink_packet__servo_output_raw;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SERVO_OUTPUT_RAW__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_servo_output_raw;
  mqtthandler_list.push_back(ptr);
  
  // 253,STATUSTEXT
  ptr = &mqtthandler_mavlink_packet__statustext;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__STATUSTEXT__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_statustext;
  mqtthandler_list.push_back(ptr);
  
  // 60,SYS_STATUS
  ptr = &mqtthandler_mavlink_packet__sys_status;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SYS_STATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_sys_status;
  mqtthandler_list.push_back(ptr);
  
  // 2,SYSTEM_TIME
  ptr = &mqtthandler_mavlink_packet__system_time;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SYSTEM_TIME__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_system_time;
  mqtthandler_list.push_back(ptr);
  
  // 111,TIMESYNC
  ptr = &mqtthandler_mavlink_packet__timesync;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__TIMESYNC__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_timesync;
  mqtthandler_list.push_back(ptr);
  
  // 
  ptr = &mqtthandler_mavlink_packet__terrain_report;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_REPORT__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_terrain_report;
  mqtthandler_list.push_back(ptr);

  // 74,VFR_HUD
  ptr = &mqtthandler_mavlink_packet__vfr_hud;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__VFR_HUD__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_vfr_hud;
  mqtthandler_list.push_back(ptr);
  
  // 241,VIBRATION
  ptr = &mqtthandler_mavlink_packet__vibration;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__VIBRATION__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_vibration;
  mqtthandler_list.push_back(ptr);

  // debug_receive_stats
  ptr = &mqtthandler_mavlink_packet__debug_receive_stats;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_RECEIVE_STATS__CTR;
  ptr->ConstructJSON_function = &mMAVLink_Decoder::ConstructJSON_Debug_ReceiveStats;
  mqtthandler_list.push_back(ptr);

  #endif // ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
  
} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mMAVLink_Decoder::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mMAVLink_Decoder::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mMAVLink_Decoder::MQTTHandler_Sender()
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}
  
#endif // USE_MODULE_NETWORK_MQTT






#endif
