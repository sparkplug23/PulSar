#include "mMAVLink.h"

#ifdef USE_MODULE__DRIVERS_MAVLINK

const char* mMAVLink::PM_MODULE__DRIVERS_MAVLINK__CTR = D_MODULE__DRIVERS_MAVLINK__CTR;
const char* mMAVLink::PM_MODULE__DRIVERS_MAVLINK__FRIENDLY_CTR = D_MODULE__DRIVERS_MAVLINK__FRIENDLY_CTR;

int8_t mMAVLink::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  // some functions must run regardless
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
   case FUNC_LOOP:
   {
    PollMAVLink_Stream();
   }
   break;
    case FUNC_EVERY_100_MSECOND:
      // BuzzerEvery100mSec();
    break;
    case FUNC_EVERY_SECOND:
      // AddLog(LOG_LEVEL_TEST, PSTR("pinMode %d"),Buzzer.pin);
      // digitalWrite(Buzzer.pin, !digitalRead(Buzzer.pin));

      // ALOG_INF(PSTR("PinUsed = %d"), pCONT_pins->PinUsed(GPIO_BUZZER_ID));


    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


void mMAVLink::Pre_Init(void)
{

  ALOG_INF(PSTR("mMAVLink::Pre_Init(void)"));

  // if (pCONT_pins->PinUsed(GPIO_BUZZER_ID)) {
  //   pinMode(pCONT_pins->GetPin(GPIO_BUZZER_ID), OUTPUT);
  //   Buzzer.pin = pCONT_pins->GetPin(GPIO_BUZZER_ID);
  // ALOG_INF(PSTR("mMAVLink::GPIO_BUZZER_ID(void)"));
  //   BuzzerSet(0);
  //   settings.fEnableSensor = true;
  // } else
  // if (pCONT_pins->PinUsed(GPIO_BUZZER_INV_ID)) {
  //   pinMode(pCONT_pins->GetPin(GPIO_BUZZER_INV_ID), OUTPUT);
  //   Buzzer.pin = pCONT_pins->GetPin(GPIO_BUZZER_INV_ID);
  // ALOG_INF(PSTR("mMAVLink::GPIO_BUZZER_INV_ID(void)"));
  //   Buzzer.inverted = true;
  //   BuzzerSet(0);
    settings.fEnableSensor = true;
  // }   
  // else {
  //   settings.fEnableSensor = false;
  // }

  
  _MAVSerial = &Serial2;
  MILLIG_TO_MS2 = 9.80665 / 1000.0;
  system_id = 1; // Your i.e. Arduino sysid
  component_id = 158; // Your i.e. Arduino compid
  type = MAV_TYPE_QUADROTOR;
  autopilot =  MAV_AUTOPILOT_INVALID;


}


void mMAVLink::Init(void)
{

    Serial.println("Init");
  while(!begin()){
    Serial.println("Not Connected: Waiting for Heartbeat from ardupilot");
    delay(1000);
  }

  StreamF();

  // delay(2000);

  // Serial.println("mMAVLink::Init DONE");

}

 

/*
  mMAVLink.cpp - Library for using Arduino to recieve Pixhawk's sensor data as well as some other usefull data which you might need.
  Created by Shashi Kant, June 23, 2018.
  Using MAVLink C headers files generated from the ardupilotmega.xml with the help of mavgenerator.
*/

// #include "mMAVLink.h"

// mMAVLink::mMAVLink(HardwareSerial &hs){
// }

bool mMAVLink::begin(){
  _MAVSerial->begin(57600);
  if(_MAVSerial->available()<=0){

    
    int flag=1;
    Serial.println("Sending Heartbeats...");
    mavlink_message_t msghb;
    mavlink_heartbeat_t heartbeat;
    uint8_t bufhb[MAVLINK_MAX_PACKET_LEN];
    mavlink_msg_heartbeat_pack(system_id, component_id, &msghb, type, autopilot, MAV_MODE_PREFLIGHT, 0, MAV_STATE_STANDBY);
    uint16_t lenhb = mavlink_msg_to_send_buffer(bufhb, &msghb);
    delay(1000);
    _MAVSerial->write(bufhb,lenhb);
    Serial.println("Heartbeats sent! Now will check for recieved heartbeats to record sysid and compid...");

    
    return 0;
  }else{
    return 1;
  }
}

// At first we will send some HeartBeats to Pixhawk to check whether it's available or not??
// After that we will check for whether we are recieving HeartBeats back from Pixhawk if Yes,
// We will note down its sysid and compid to send it a req to Stream Data.
void mMAVLink::StreamF(){
  delay(2000);
  int flag=1;
  Serial.println("Sending Heartbeats...");
  mavlink_message_t msghb;
  mavlink_heartbeat_t heartbeat;
  uint8_t bufhb[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_heartbeat_pack(system_id, component_id, &msghb, type, autopilot, MAV_MODE_PREFLIGHT, 0, MAV_STATE_STANDBY);
  uint16_t lenhb = mavlink_msg_to_send_buffer(bufhb, &msghb);
  delay(1000);
  _MAVSerial->write(bufhb,lenhb);
  Serial.println("Heartbeats sent! Now will check for recieved heartbeats to record sysid and compid...");

  // Looping untill we get the required data.
  while(flag==1){
    delay(1);
    while(_MAVSerial->available()>0){
      mavlink_message_t msgpx;
      mavlink_status_t statuspx;
      uint8_t ch = _MAVSerial->read();
      if(mavlink_parse_char(MAVLINK_COMM_0, ch, &msgpx, &statuspx)){
        Serial.println("Message Parsing Done!");
        switch(msgpx.msgid){
          case MAVLINK_MSG_ID_HEARTBEAT:
          {
            mavlink_heartbeat_t packet;
            mavlink_msg_heartbeat_decode(&msgpx, &packet);
            received_sysid = msgpx.sysid; // Pixhawk sysid
            received_compid = msgpx.compid; // Pixhawk compid
            Serial.println("sysid and compid successfully recorded");
            flag = 0;
            break;
          }
        }
      }
    }
  }

  // Sending request for data stream...
  Serial.println("Now sending request for data stream...");
  delay(2000);
  mavlink_message_t msgds;
  uint8_t bufds[MAVLINK_MAX_PACKET_LEN];
  mavlink_msg_request_data_stream_pack(system_id, component_id, &msgds, received_sysid, received_compid, MAV_DATA_STREAM_ALL , 0x05, 1);
  uint16_t lends = mavlink_msg_to_send_buffer(bufds, &msgds);
  delay(1000);
  _MAVSerial->write(bufds,lends);
  Serial.println("Request sent! Now you are ready to recieve datas...");

}

void mMAVLink::PollMAVLink_Stream()
{
  
  
  while(_MAVSerial->available() > 0)
  {
  
    mavlink_message_t msg;
    mavlink_status_t status1;
    uint8_t ch = _MAVSerial->read();
    
    if(mavlink_parse_char(MAVLINK_COMM_0, ch, &msg, &status1))
    {
      // Serial.println("Message Parsing Done!");
      // Serial.println(msg.msgid);


      if(std::find(unique_msg_id_list.begin(), unique_msg_id_list.end(), msg.msgid) != unique_msg_id_list.end()) {
        /* v contains x */
      } else {
        /* v does not contain x */
        unique_msg_id_list.push_back(msg.msgid);
        sort(unique_msg_id_list.begin(), unique_msg_id_list.end());
      }

      bool parsed_packet = true; //default will reset
      switch(msg.msgid){
        default:          parsed_packet = false;  break; // Not handled 
        case MAVLINK_MSG_ID_AHRS:                         mavlink_msg_ahrs_decode(&msg, &pkt.ahrs);                                           break;
        case MAVLINK_MSG_ID_AHRS2:                        mavlink_msg_ahrs2_decode(&msg, &pkt.ahrs2);                                         break;
        case MAVLINK_MSG_ID_ATTITUDE:                     mavlink_msg_attitude_decode(&msg, &pkt.attitude);                                   break;
        case MAVLINK_MSG_ID_AUTOPILOT_VERSION_REQUEST:    mavlink_msg_autopilot_version_decode(&msg, &pkt.autopilot_version);                 break;
        case MAVLINK_MSG_ID_BATTERY_STATUS:               mavlink_msg_battery_status_decode(&msg, &pkt.battery_status);                       break;
        case MAVLINK_MSG_ID_EKF_STATUS_REPORT:            mavlink_msg_ekf_status_report_decode(&msg, &pkt.ekf_status_report);                 break;
        case MAVLINK_MSG_ID_FENCE_STATUS:                 mavlink_msg_fence_status_decode(&msg, &pkt.fence_status);                           break;
        case MAVLINK_MSG_ID_GIMBAL_REPORT:                mavlink_msg_gimbal_report_decode(&msg, &pkt.gimbal_report);                         break;
        case MAVLINK_MSG_ID_GIMBAL_TORQUE_CMD_REPORT:     mavlink_msg_gimbal_torque_cmd_report_decode(&msg, &pkt.gimbal_torque_cmd_report);   break;
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:          mavlink_msg_global_position_int_decode(&msg, &pkt.global_position_int);             break;
        case MAVLINK_MSG_ID_GOPRO_HEARTBEAT:              mavlink_msg_gopro_heartbeat_decode(&msg, &pkt.gopro_heartbeat);                     break;
        case MAVLINK_MSG_ID_GPS_RAW_INT:                  mavlink_msg_gps_raw_int_decode(&msg, &pkt.gps_raw_int);                             break;
        case MAVLINK_MSG_ID_HEARTBEAT:                    mavlink_msg_heartbeat_decode(&msg, &pkt.heartbeat);                                 break;
        case MAVLINK_MSG_ID_HWSTATUS:                     mavlink_msg_hwstatus_decode(&msg, &pkt.hwstatus);                                   break;
        case MAVLINK_MSG_ID_MEMINFO:                      mavlink_msg_meminfo_decode(&msg, &pkt.meminfo);                                     break;
        case MAVLINK_MSG_ID_MISSION_CURRENT:              mavlink_msg_mission_current_decode(&msg, &pkt.mission_current);                     break;
        case MAVLINK_MSG_ID_MOUNT_STATUS:                 mavlink_msg_mount_status_decode(&msg, &pkt.mount_status);                           break;
        case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:        mavlink_msg_nav_controller_output_decode(&msg, &pkt.nav_controller_output);         break;
        case MAVLINK_MSG_ID_PARAM_VALUE:                  mavlink_msg_param_value_decode(&msg, &pkt.param_value);                             break;
        case MAVLINK_MSG_ID_POWER_STATUS:                 mavlink_msg_power_status_decode(&msg, &pkt.power_status);                           break;
        case MAVLINK_MSG_ID_RAW_IMU:                      mavlink_msg_raw_imu_decode(&msg, &pkt.raw_imu);                                     break;
        case MAVLINK_MSG_ID_RC_CHANNELS:                  mavlink_msg_rc_channels_decode(&msg, &pkt.rc_channels);                             break;
        case MAVLINK_MSG_ID_RC_CHANNELS_RAW:              mavlink_msg_rc_channels_raw_decode(&msg, &pkt.rc_channels_raw);                     break;
        case MAVLINK_MSG_ID_REQUEST_DATA_STREAM:          mavlink_msg_request_data_stream_decode(&msg, &pkt.request_data_stream);             break;
        case MAVLINK_MSG_ID_SCALED_IMU2:                  mavlink_msg_scaled_imu2_decode(&msg, &pkt.scaled_imu2);                             break;
        case MAVLINK_MSG_ID_SCALED_IMU3:                  mavlink_msg_scaled_imu3_decode(&msg, &pkt.scaled_imu3);                             break;     
        case MAVLINK_MSG_ID_SCALED_PRESSURE:              mavlink_msg_scaled_pressure_decode(&msg, &pkt.scaled_pressure);                     break;     
        case MAVLINK_MSG_ID_SCALED_PRESSURE2:             mavlink_msg_scaled_pressure2_decode(&msg, &pkt.scaled_pressure2);                   break;     
        case MAVLINK_MSG_ID_SENSOR_OFFSETS:               mavlink_msg_sensor_offsets_decode(&msg, &pkt.sensor_offsets);                       break;     
        case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:             mavlink_msg_servo_output_raw_decode(&msg, &pkt.servo_output_raw);                   break;     
        case MAVLINK_MSG_ID_STATUSTEXT:                   mavlink_msg_statustext_decode(&msg, &pkt.statustext);                               break;     
        case MAVLINK_MSG_ID_SYS_STATUS:                   mavlink_msg_sys_status_decode(&msg, &pkt.sys_status);                               break;     
        case MAVLINK_MSG_ID_SYSTEM_TIME:                  mavlink_msg_system_time_decode(&msg, &pkt.system_time);                             break;     
        case MAVLINK_MSG_ID_TIMESYNC:                     mavlink_msg_timesync_decode(&msg, &pkt.timesync);                                   break;     
        case MAVLINK_MSG_ID_VFR_HUD:                      mavlink_msg_vfr_hud_decode(&msg, &pkt.vfr_hud);                                     break;        
        case MAVLINK_MSG_ID_VIBRATION:                    mavlink_msg_vibration_decode(&msg, &pkt.vibration);                                 break;     
      }

      if(parsed_packet)
      {
        // ALOG_INF(PSTR("Aparsed_packet = %d, id = %d, msg = \"%S\""), parsed_packet, msg.msgid, MavLink_Msg_FriendlyName_By_ID(msg.msgid, buffer, sizeof(buffer)));
      }

    }
  }


}


const char* mMAVLink::MavLink_Msg_FriendlyName_By_ID(uint16_t id, char* buffer, uint8_t buflen)
{

  if(buffer == nullptr){ return PM_SEARCH_NOMATCH;}
  switch(id){
    default: 
    
    case MAVLINK_MSG_ID_ATTITUDE_QUATERNION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE_QUATERNION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE_QUATERNION__CTR)); break;
    
    case MAVLINK_MSG_ID_BATTERY_STATUS:          memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__BATTERY_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__BATTERY_STATUS__CTR)); break;
    

    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_POSITION_INT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_POSITION_INT__CTR)); break;
    
    case MAVLINK_MSG_ID_HIGHRES_IMU:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIGHRES_IMU__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIGHRES_IMU__CTR)); break;
    
    
    
    case LOG_LEVEL_ALL:            memcpy_P(buffer, PM_LOG_LEVEL_ALL_SHORT_CTR, sizeof(PM_LOG_LEVEL_ALL_SHORT_CTR)); break;
  }

  return buffer;

}


  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
void mMAVLink::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  JsonParserObject jobj = 0;
 
  if(jtok = obj["beeps"]){
    AddLog(LOG_LEVEL_TEST, PSTR("bbeeps=%d"),jtok.getInt());
    // pinMode(Buzzer.pin, jtok.getInt());

  }

}


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mMAVLink::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();

    
  JBI->Array_Start("msg list");
  for(int i=0;i<unique_msg_id_list.size();i++)
  {
    JBI->Add(unique_msg_id_list[i]);

  }

  JBI->Array_End();


  return JBI->End();

}


uint8_t mMAVLink::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_JSON_VOLTAGE, 0);
  return JBI->End();
    
}



uint8_t mMAVLink::ConstructJSON_ahrs(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("omegaIx", pkt.ahrs.omegaIx);
    JBI->Add("omegaIy", pkt.ahrs.omegaIy);
    JBI->Add("omegaIz", pkt.ahrs.omegaIz);
    JBI->Add("accel_weight", pkt.ahrs.accel_weight);
    JBI->Add("renorm_val", pkt.ahrs.renorm_val);
    JBI->Add("error_rp", pkt.ahrs.error_rp);
    JBI->Add("error_yaw", pkt.ahrs.error_yaw);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_ahrs2(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("roll", pkt.ahrs2.roll);
    JBI->Add("pitch", pkt.ahrs2.pitch);
    JBI->Add("yaw", pkt.ahrs2.yaw);
    JBI->Add("altitude", pkt.ahrs2.altitude);
    JBI->Add("lat", pkt.ahrs2.lat);
    JBI->Add("lng", pkt.ahrs2.lng);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_attitude(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_boot_ms", pkt.attitude.time_boot_ms);
    JBI->Add("roll", pkt.attitude.roll);
    JBI->Add("pitch", pkt.attitude.pitch);
    JBI->Add("yaw", pkt.attitude.yaw);
    JBI->Add("rollspeed", pkt.attitude.rollspeed);
    JBI->Add("pitchspeed", pkt.attitude.pitchspeed);
    JBI->Add("yawspeed", pkt.attitude.yawspeed);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_autopilot_version_request(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("capabilities", pkt.autopilot_version.capabilities);
    JBI->Add("uid", pkt.autopilot_version.uid);
    JBI->Add("flight_sw_version", pkt.autopilot_version.flight_sw_version);
    JBI->Add("middleware_sw_version", pkt.autopilot_version.middleware_sw_version);
    JBI->Add("os_sw_version", pkt.autopilot_version.os_sw_version);
    JBI->Add("board_version", pkt.autopilot_version.board_version);
    JBI->Add("vendor_id", pkt.autopilot_version.vendor_id);
    JBI->Add("product_id", pkt.autopilot_version.product_id);
    JBI->Array_AddArray("flight_custom_version", pkt.autopilot_version.flight_custom_version, sizeof(pkt.autopilot_version.flight_custom_version));
    JBI->Array_AddArray("middleware_custom_version", pkt.autopilot_version.middleware_custom_version, sizeof(pkt.autopilot_version.middleware_custom_version));
    JBI->Array_AddArray("os_custom_version", pkt.autopilot_version.os_custom_version, sizeof(pkt.autopilot_version.os_custom_version));
    JBI->Array_AddArray("uid2", pkt.autopilot_version.uid2, sizeof(pkt.autopilot_version.uid2));
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_battery_status(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("current_consumed", pkt.battery_status.current_consumed);
    JBI->Add("energy_consumed", pkt.battery_status.energy_consumed);
    JBI->Add("temperature", pkt.battery_status.temperature);
    JBI->Array_AddArray("voltages", pkt.battery_status.voltages, ARRAY_SIZE(pkt.battery_status.voltages));
    JBI->Add("current_battery", pkt.battery_status.current_battery);
    JBI->Add("id", pkt.battery_status.id);
    JBI->Add("battery_function", pkt.battery_status.battery_function);
    JBI->Add("type", pkt.battery_status.type);
    JBI->Add("battery_remaining", pkt.battery_status.battery_remaining);
    JBI->Add("time_remaining", pkt.battery_status.time_remaining);
    JBI->Add("charge_state", pkt.battery_status.charge_state);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_ekf_status_report(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("velocity_variance", pkt.ekf_status_report.velocity_variance);
    JBI->Add("pos_horiz_variance", pkt.ekf_status_report.pos_horiz_variance);
    JBI->Add("pos_vert_variance", pkt.ekf_status_report.pos_vert_variance);
    JBI->Add("compass_variance", pkt.ekf_status_report.compass_variance);
    JBI->Add("terrain_alt_variance", pkt.ekf_status_report.terrain_alt_variance);
    JBI->Add("flags", pkt.ekf_status_report.flags);
    JBI->Add("airspeed_variance", pkt.ekf_status_report.airspeed_variance);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_fence_status(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("breach_time", pkt.fence_status.breach_time);
    JBI->Add("breach_count", pkt.fence_status.breach_count);
    JBI->Add("breach_status", pkt.fence_status.breach_status);
    JBI->Add("breach_type", pkt.fence_status.breach_type);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_gimbal_report(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("delta_time", pkt.gimbal_report.delta_time);
    JBI->Add("delta_angle_x", pkt.gimbal_report.delta_angle_x);
    JBI->Add("delta_angle_y", pkt.gimbal_report.delta_angle_y);
    JBI->Add("delta_angle_z", pkt.gimbal_report.delta_angle_z);
    JBI->Add("delta_velocity_x", pkt.gimbal_report.delta_velocity_x);
    JBI->Add("delta_velocity_y", pkt.gimbal_report.delta_velocity_y);
    JBI->Add("delta_velocity_z", pkt.gimbal_report.delta_velocity_z);
    JBI->Add("joint_roll", pkt.gimbal_report.joint_roll);
    JBI->Add("joint_el", pkt.gimbal_report.joint_el);
    JBI->Add("joint_az", pkt.gimbal_report.joint_az);
    JBI->Add("target_system", pkt.gimbal_report.target_system);
    JBI->Add("target_component", pkt.gimbal_report.target_component);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_gimbal_torque_cmd_report(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("rl_torque_cmd", pkt.gimbal_torque_cmd_report.rl_torque_cmd);
    JBI->Add("el_torque_cmd", pkt.gimbal_torque_cmd_report.el_torque_cmd);
    JBI->Add("az_torque_cmd", pkt.gimbal_torque_cmd_report.az_torque_cmd);
    JBI->Add("target_system", pkt.gimbal_torque_cmd_report.target_system);
    JBI->Add("target_component", pkt.gimbal_torque_cmd_report.target_component);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_global_position_int(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_boot_ms", pkt.global_position_int.time_boot_ms);
    JBI->Add("lat", pkt.global_position_int.lat);
    JBI->Add("lon", pkt.global_position_int.lon);
    JBI->Add("alt", pkt.global_position_int.alt);
    JBI->Add("relative_alt", pkt.global_position_int.relative_alt);
    JBI->Add("vx", pkt.global_position_int.vx);
    JBI->Add("vy", pkt.global_position_int.vy);
    JBI->Add("vz", pkt.global_position_int.vz);
    JBI->Add("hdg", pkt.global_position_int.hdg);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_gopro_heartbeat(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("status", pkt.gopro_heartbeat.status);
    JBI->Add("capture_mode", pkt.gopro_heartbeat.capture_mode);
    JBI->Add("flags", pkt.gopro_heartbeat.flags);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_gps_raw_int(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_usec", pkt.gps_raw_int.time_usec);
    JBI->Add("lat", pkt.gps_raw_int.lat);
    JBI->Add("lon", pkt.gps_raw_int.lon);
    JBI->Add("alt", pkt.gps_raw_int.alt);
    JBI->Add("eph", pkt.gps_raw_int.eph);
    JBI->Add("epv", pkt.gps_raw_int.epv);
    JBI->Add("vel", pkt.gps_raw_int.vel);
    JBI->Add("cog", pkt.gps_raw_int.cog);
    JBI->Add("fix_type", pkt.gps_raw_int.fix_type);
    JBI->Add("satellites_visible", pkt.gps_raw_int.satellites_visible);
    JBI->Add("alt_ellipsoid", pkt.gps_raw_int.alt_ellipsoid);
    JBI->Add("h_acc", pkt.gps_raw_int.h_acc);
    JBI->Add("v_acc", pkt.gps_raw_int.v_acc);
    JBI->Add("vel_acc", pkt.gps_raw_int.vel_acc);
    JBI->Add("hdg_acc", pkt.gps_raw_int.hdg_acc);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_heartbeat(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("custom_mode", pkt.heartbeat.custom_mode);
    JBI->Add("type", pkt.heartbeat.type);
    JBI->Add("autopilot", pkt.heartbeat.autopilot);
    JBI->Add("base_mode", pkt.heartbeat.base_mode);
    JBI->Add("system_status", pkt.heartbeat.system_status);
    JBI->Add("mavlink_version", pkt.heartbeat.mavlink_version);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_hwstatus(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("Vcc", pkt.hwstatus.Vcc);
    JBI->Add("I2Cerr", pkt.hwstatus.I2Cerr);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_meminfo(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("brkval", pkt.meminfo.brkval);
    JBI->Add("freemem", pkt.meminfo.freemem);
    JBI->Add("freemem32", pkt.meminfo.freemem32);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_mission_current(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("seq", pkt.mission_current.seq);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_mount_status(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("pointing_a", pkt.mount_status.pointing_a);
    JBI->Add("pointing_b", pkt.mount_status.pointing_b);
    JBI->Add("pointing_c", pkt.mount_status.pointing_c);
    JBI->Add("target_system", pkt.mount_status.target_system);
    JBI->Add("target_component", pkt.mount_status.target_component);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_nav_controller_output(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("nav_roll", pkt.nav_controller_output.nav_roll);
    JBI->Add("nav_pitch", pkt.nav_controller_output.nav_pitch);
    JBI->Add("alt_error", pkt.nav_controller_output.alt_error);
    JBI->Add("aspd_error", pkt.nav_controller_output.aspd_error);
    JBI->Add("xtrack_error", pkt.nav_controller_output.xtrack_error);
    JBI->Add("nav_bearing", pkt.nav_controller_output.nav_bearing);
    JBI->Add("target_bearing", pkt.nav_controller_output.target_bearing);
    JBI->Add("wp_dist", pkt.nav_controller_output.wp_dist);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_param_value(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("param_value", pkt.param_value.param_value);
    JBI->Add("param_count", pkt.param_value.param_count);
    JBI->Add("param_index", pkt.param_value.param_index);
    JBI->Array_AddArray("param_id", pkt.param_value.param_id, sizeof(pkt.param_value.param_id));
    JBI->Add("param_type", pkt.param_value.param_type);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_power_status(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("Vcc", pkt.power_status.Vcc);
    JBI->Add("Vservo", pkt.power_status.Vservo);
    JBI->Add("flags", pkt.power_status.flags);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_raw_imu(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_usec", pkt.raw_imu.time_usec);
    JBI->Add("xacc", pkt.raw_imu.xacc);
    JBI->Add("yacc", pkt.raw_imu.yacc);
    JBI->Add("zacc", pkt.raw_imu.zacc);
    JBI->Add("xgyro", pkt.raw_imu.xgyro);
    JBI->Add("ygyro", pkt.raw_imu.ygyro);
    JBI->Add("zgyro", pkt.raw_imu.zgyro);
    JBI->Add("xmag", pkt.raw_imu.xmag);
    JBI->Add("ymag", pkt.raw_imu.ymag);
    JBI->Add("zmag", pkt.raw_imu.zmag);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_rc_channels(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_boot_ms", pkt.rc_channels.time_boot_ms);
    JBI->Add("chan1_raw", pkt.rc_channels.chan1_raw);
    JBI->Add("chan2_raw", pkt.rc_channels.chan2_raw);
    JBI->Add("chan3_raw", pkt.rc_channels.chan3_raw);
    JBI->Add("chan4_raw", pkt.rc_channels.chan4_raw);
    JBI->Add("chan5_raw", pkt.rc_channels.chan5_raw);
    JBI->Add("chan6_raw", pkt.rc_channels.chan6_raw);
    JBI->Add("chan7_raw", pkt.rc_channels.chan7_raw);
    JBI->Add("chan8_raw", pkt.rc_channels.chan8_raw);
    JBI->Add("chan9_raw", pkt.rc_channels.chan9_raw);
    JBI->Add("chan10_raw", pkt.rc_channels.chan10_raw);
    JBI->Add("chan11_raw", pkt.rc_channels.chan11_raw);
    JBI->Add("chan12_raw", pkt.rc_channels.chan12_raw);
    JBI->Add("chan13_raw", pkt.rc_channels.chan13_raw);
    JBI->Add("chan14_raw", pkt.rc_channels.chan14_raw);
    JBI->Add("chan15_raw", pkt.rc_channels.chan15_raw);
    JBI->Add("chan16_raw", pkt.rc_channels.chan16_raw);
    JBI->Add("chan17_raw", pkt.rc_channels.chan17_raw);
    JBI->Add("chan18_raw", pkt.rc_channels.chan18_raw);
    JBI->Add("chancount", pkt.rc_channels.chancount);
    JBI->Add("rssi", pkt.rc_channels.rssi);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_rc_channels_raw(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_boot_ms", pkt.rc_channels_raw.time_boot_ms);
    JBI->Add("chan1_raw", pkt.rc_channels_raw.chan1_raw);
    JBI->Add("chan2_raw", pkt.rc_channels_raw.chan2_raw);
    JBI->Add("chan3_raw", pkt.rc_channels_raw.chan3_raw);
    JBI->Add("chan4_raw", pkt.rc_channels_raw.chan4_raw);
    JBI->Add("chan5_raw", pkt.rc_channels_raw.chan5_raw);
    JBI->Add("chan6_raw", pkt.rc_channels_raw.chan6_raw);
    JBI->Add("chan7_raw", pkt.rc_channels_raw.chan7_raw);
    JBI->Add("chan8_raw", pkt.rc_channels_raw.chan8_raw);    
    JBI->Add("port", pkt.rc_channels_raw.port);
    JBI->Add("rssi", pkt.rc_channels_raw.rssi);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_request_data_stream(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("req_message_rate", pkt.request_data_stream.req_message_rate);
    JBI->Add("target_system", pkt.request_data_stream.target_system);
    JBI->Add("target_component", pkt.request_data_stream.target_component);
    JBI->Add("req_stream_id", pkt.request_data_stream.req_stream_id);
    JBI->Add("start_stop", pkt.request_data_stream.start_stop);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_imu2(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_boot_ms", pkt.scaled_imu2.time_boot_ms);
    JBI->Add("xacc", pkt.scaled_imu2.xacc);
    JBI->Add("yacc", pkt.scaled_imu2.yacc);
    JBI->Add("zacc", pkt.scaled_imu2.zacc);
    JBI->Add("xgyro", pkt.scaled_imu2.xgyro);
    JBI->Add("ygyro", pkt.scaled_imu2.ygyro);
    JBI->Add("zgyro", pkt.scaled_imu2.zgyro);
    JBI->Add("xmag", pkt.scaled_imu2.xmag);
    JBI->Add("ymag", pkt.scaled_imu2.ymag);
    JBI->Add("zmag", pkt.scaled_imu2.zmag);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_imu3(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_boot_ms", pkt.scaled_imu3.time_boot_ms);
    JBI->Add("xacc", pkt.scaled_imu3.xacc);
    JBI->Add("yacc", pkt.scaled_imu3.yacc);
    JBI->Add("zacc", pkt.scaled_imu3.zacc);
    JBI->Add("xgyro", pkt.scaled_imu3.xgyro);
    JBI->Add("ygyro", pkt.scaled_imu3.ygyro);
    JBI->Add("zgyro", pkt.scaled_imu3.zgyro);
    JBI->Add("xmag", pkt.scaled_imu3.xmag);
    JBI->Add("ymag", pkt.scaled_imu3.ymag);
    JBI->Add("zmag", pkt.scaled_imu3.zmag);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_scaled_pressure(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_boot_ms", pkt.scaled_pressure.time_boot_ms);
    JBI->Add("press_abs", pkt.scaled_pressure.press_abs);
    JBI->Add("press_diff", pkt.scaled_pressure.press_diff);
    JBI->Add("temperature", pkt.scaled_pressure.temperature);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_scaled_pressure2(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_boot_ms", pkt.scaled_pressure2.time_boot_ms);
    JBI->Add("press_abs", pkt.scaled_pressure2.press_abs);
    JBI->Add("press_diff", pkt.scaled_pressure2.press_diff);
    JBI->Add("temperature", pkt.scaled_pressure2.temperature);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_sensor_offsets(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("mag_declination", pkt.sensor_offsets.mag_declination);
    JBI->Add("raw_press", pkt.sensor_offsets.raw_press);
    JBI->Add("raw_temp", pkt.sensor_offsets.raw_temp);
    JBI->Add("gyro_cal_x", pkt.sensor_offsets.gyro_cal_x);
    JBI->Add("gyro_cal_y", pkt.sensor_offsets.gyro_cal_y);
    JBI->Add("gyro_cal_z", pkt.sensor_offsets.gyro_cal_z);
    JBI->Add("accel_cal_x", pkt.sensor_offsets.accel_cal_x);
    JBI->Add("accel_cal_y", pkt.sensor_offsets.accel_cal_y);
    JBI->Add("accel_cal_z", pkt.sensor_offsets.accel_cal_z);
    JBI->Add("mag_ofs_x", pkt.sensor_offsets.mag_ofs_x);
    JBI->Add("mag_ofs_y", pkt.sensor_offsets.mag_ofs_y);
    JBI->Add("mag_ofs_z", pkt.sensor_offsets.mag_ofs_z);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_servo_output_raw(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_usec", pkt.servo_output_raw.time_usec);
    JBI->Add("servo1_raw", pkt.servo_output_raw.servo1_raw);
    JBI->Add("servo2_raw", pkt.servo_output_raw.servo2_raw);
    JBI->Add("servo3_raw", pkt.servo_output_raw.servo3_raw);
    JBI->Add("servo4_raw", pkt.servo_output_raw.servo4_raw);
    JBI->Add("servo5_raw", pkt.servo_output_raw.servo5_raw);
    JBI->Add("servo6_raw", pkt.servo_output_raw.servo6_raw);
    JBI->Add("servo7_raw", pkt.servo_output_raw.servo7_raw);
    JBI->Add("servo8_raw", pkt.servo_output_raw.servo8_raw);
    JBI->Add("port", pkt.servo_output_raw.port);
    JBI->Add("servo9_raw", pkt.servo_output_raw.servo9_raw);
    JBI->Add("servo10_raw", pkt.servo_output_raw.servo10_raw);
    JBI->Add("servo11_raw", pkt.servo_output_raw.servo11_raw);
    JBI->Add("servo12_raw", pkt.servo_output_raw.servo12_raw);
    JBI->Add("servo13_raw", pkt.servo_output_raw.servo13_raw);
    JBI->Add("servo14_raw", pkt.servo_output_raw.servo14_raw);
    JBI->Add("servo15_raw", pkt.servo_output_raw.servo15_raw);
    JBI->Add("servo16_raw", pkt.servo_output_raw.servo16_raw);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_statustext(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("severity", pkt.statustext.severity);
    JBI->Add("text", pkt.statustext.text);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_sys_status(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("sensorsC", pkt.sys_status.onboard_control_sensors_present);
    JBI->Add("sensorsE", pkt.sys_status.onboard_control_sensors_enabled);
    JBI->Add("sensorsH", pkt.sys_status.onboard_control_sensors_health);
    JBI->Add("load", pkt.sys_status.load);
    JBI->Add("voltage_battery", pkt.sys_status.voltage_battery);
    JBI->Add("current_battery", pkt.sys_status.current_battery);
    JBI->Add("drop_rate_comm", pkt.sys_status.drop_rate_comm);
    JBI->Add("errors_comm", pkt.sys_status.errors_comm);
    JBI->Add("errors_count1", pkt.sys_status.errors_count1);
    JBI->Add("errors_count2", pkt.sys_status.errors_count2);
    JBI->Add("errors_count3", pkt.sys_status.errors_count3);
    JBI->Add("errors_count4", pkt.sys_status.errors_count4);
    JBI->Add("battery_remaining", pkt.sys_status.battery_remaining);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_system_time(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_unix_usec", pkt.system_time.time_unix_usec);
    JBI->Add("time_boot_ms", pkt.system_time.time_boot_ms);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_timesync(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("tc1", pkt.timesync.tc1);
    JBI->Add("ts1", pkt.timesync.ts1);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_vfr_hud(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("airspeed", pkt.vfr_hud.airspeed);
    JBI->Add("groundspeed", pkt.vfr_hud.groundspeed);
    JBI->Add("alt", pkt.vfr_hud.alt);
    JBI->Add("climb", pkt.vfr_hud.climb);
    JBI->Add("heading", pkt.vfr_hud.heading);
    JBI->Add("throttle", pkt.vfr_hud.throttle);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_vibration(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("time_usec", pkt.vibration.time_usec);
    JBI->Add("vibration_x", pkt.vibration.vibration_x);
    JBI->Add("vibration_y", pkt.vibration.vibration_y);
    JBI->Add("vibration_z", pkt.vibration.vibration_z);
    JBI->Add("clipping_0", pkt.vibration.clipping_0);
    JBI->Add("clipping_1", pkt.vibration.clipping_1);
    JBI->Add("clipping_2", pkt.vibration.clipping_2);
  return JBI->End();    
}




  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mMAVLink::MQTTHandler_Init()
{

  struct handler<mMAVLink>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_MIN; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_Sensor;

  // 163,AHRS
  ptr = &mqtthandler_mavlink_packet__ahrs;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__AHRS__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_ahrs;

  // 178,AHRS2
  ptr = &mqtthandler_mavlink_packet__ahrs2;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__AHRS2__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_ahrs2;

  // 30,ATTITUDE
  ptr = &mqtthandler_mavlink_packet__attitude;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_attitude;

  // 183,AUTOPILOT_VERSION_REQUEST
  ptr = &mqtthandler_mavlink_packet__autopilot_version_request;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__AUTOPILOT_VERSION_REQUEST__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_autopilot_version_request;

  //147,BATTERY_STATUS
  ptr = &mqtthandler_mavlink_packet__battery_status;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__BATTERY_STATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_battery_status;

  // 193,EKF_STATUS_REPORT
  ptr = &mqtthandler_mavlink_packet__ekf_status_report;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__EKF_STATUS_REPORT__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_ekf_status_report;

  // 162,FENCE_STATUS
  ptr = &mqtthandler_mavlink_packet__fence_status;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__FENCE_STATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_fence_status;

  // 200,GIMBAL_REPORT
  ptr = &mqtthandler_mavlink_packet__gimbal_report;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_REPORT__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_gimbal_report;

  // 214,GIMBAL_TORQUE_CMD_REPORT
  ptr = &mqtthandler_mavlink_packet__gimbal_torque_cmd_report;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_TORQUE_CMD_REPORT__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_gimbal_torque_cmd_report;

  // 33,GLOBAL_POSITION_INT
  ptr = &mqtthandler_mavlink_packet__global_position_int;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_POSITION_INT__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_global_position_int;
  
  // 215,GOPRO_HEARTBEAT
  ptr = &mqtthandler_mavlink_packet__gopro_heartbeat;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__GOPRO_HEARTBEAT__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_gopro_heartbeat;
  
  // 24,GPS_RAW_INT
  ptr = &mqtthandler_mavlink_packet__gps_raw_int;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__GPS_RAW_INT__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_gps_raw_int;
  
  // 0,HEARTBEAT
  ptr = &mqtthandler_mavlink_packet__heartbeat;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__HEARTBEAT__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_heartbeat;
  
  // 165,HWSTATUS
  ptr = &mqtthandler_mavlink_packet__hwstatus;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__HWSTATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_hwstatus;
  
  // 152,MEMINFO
  ptr = &mqtthandler_mavlink_packet__meminfo;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__MEMINFO__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_meminfo;
  
  // 42,MISSION_CURRENT
  ptr = &mqtthandler_mavlink_packet__mission_current;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__MISSION_CURRENT__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_mission_current;
  
  // 158,MOUNT_STATUS
  ptr = &mqtthandler_mavlink_packet__mount_status;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__MOUNT_STATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_mount_status;
  
  // 62,NAV_CONTROLLER_OUTPUT
  ptr = &mqtthandler_mavlink_packet__nav_controller_output;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__NAV_CONTROLLER_OUTPUT__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_nav_controller_output;
  
  // 22,PARAM_VALUE
  ptr = &mqtthandler_mavlink_packet__param_value;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__PARAM_VALUE__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_param_value;
  
  // 125,POWER_STATUS
  ptr = &mqtthandler_mavlink_packet__power_status;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__POWER_STATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_power_status;
  
  // 27,RAW_IMU
  ptr = &mqtthandler_mavlink_packet__raw_imu;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__RAW_IMU__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_raw_imu;   
  
  // 65,RC_CHANNELS
  ptr = &mqtthandler_mavlink_packet__rc_channels;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_rc_channels;
  
  // 35,RC_CHANNELS_RAW
  ptr = &mqtthandler_mavlink_packet__rc_channels_raw;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_RAW__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_rc_channels_raw;
  
  // 66,REQUEST_DATA_STREAM
  ptr = &mqtthandler_mavlink_packet__request_data_stream;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__REQUEST_DATA_STREAM__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_request_data_stream;
  
  // 116,SCALED_IMU2
  ptr = &mqtthandler_mavlink_packet__scaled_imu2;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU2__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_imu2;
  
  // 129,SCALED_IMU3
  ptr = &mqtthandler_mavlink_packet__scaled_imu3;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU3__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_imu3;
  
  // 29,SCALED_PRESSURE
  ptr = &mqtthandler_mavlink_packet__scaled_pressure;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_scaled_pressure;
  
  // 137,SCALED_PRESSURE2
  ptr = &mqtthandler_mavlink_packet__scaled_pressure2;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE2__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_scaled_pressure2;
  
  // 150,SENSOR_OFFSETS
  ptr = &mqtthandler_mavlink_packet__sensor_offsets;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SENSOR_OFFSETS__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_sensor_offsets;
  
  // 36,SERVO_OUTPUT_RAW
  ptr = &mqtthandler_mavlink_packet__servo_output_raw;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SERVO_OUTPUT_RAW__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_servo_output_raw;
  
  // 253,STATUSTEXT
  ptr = &mqtthandler_mavlink_packet__statustext;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__STATUSTEXT__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_statustext;
  
  // 1,SYS_STATUS
  ptr = &mqtthandler_mavlink_packet__sys_status;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SYS_STATUS__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_sys_status;
  
  // 2,SYSTEM_TIME
  ptr = &mqtthandler_mavlink_packet__system_time;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__SYSTEM_TIME__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_system_time;
  
  // 111,TIMESYNC
  ptr = &mqtthandler_mavlink_packet__timesync;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__TIMESYNC__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_timesync;
  
  // 74,VFR_HUD
  ptr = &mqtthandler_mavlink_packet__vfr_hud;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__VFR_HUD__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_vfr_hud;
  
  // 241,VIBRATION
  ptr = &mqtthandler_mavlink_packet__vibration;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MAVLINK_MSG_PACKET_NAME__VIBRATION__CTR;
  ptr->ConstructJSON_function = &mMAVLink::ConstructJSON_vibration;
  
} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mMAVLink::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mMAVLink::MQTTHandler_Set_DefaultPeriodRate()
{
  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  // }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mMAVLink::MQTTHandler_Sender(uint8_t id)
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE__DRIVERS_MAVLINK__ID, handle, id);
  }
}
  
#endif // USE_MODULE_NETWORK_MQTT






#endif
