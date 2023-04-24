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

      ALOG_INF(PSTR("Battery %d%%"), pkt.battery_status.data.battery_remaining);


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
  
  _MAVSerial->begin(921600);
  _MAVSerial->println("BOOT");
  
  
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
  _MAVSerial->begin(921600);
  delay(1000);
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
      switch(msg.msgid)
      {
        default:
          parsed_packet = false;  
        break; // Not parsed 
        case MAVLINK_MSG_ID_AHRS:                         
          mavlink_msg_ahrs_decode(&msg, &pkt.ahrs.data);                                      
          pkt.ahrs.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_AHRS2:                         
          mavlink_msg_ahrs2_decode(&msg, &pkt.ahrs2.data);                                      
          pkt.ahrs2.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_ATTITUDE:                         
          mavlink_msg_attitude_decode(&msg, &pkt.attitude.data);                                      
          pkt.attitude.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_AUTOPILOT_VERSION_REQUEST:                         
          mavlink_msg_autopilot_version_decode(&msg, &pkt.autopilot_version.data);                                      
          pkt.autopilot_version.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_BATTERY_STATUS:                         
          mavlink_msg_battery_status_decode(&msg, &pkt.battery_status.data);                                      
          pkt.battery_status.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_EKF_STATUS_REPORT:                         
          mavlink_msg_ekf_status_report_decode(&msg, &pkt.ekf_status_report.data);                                      
          pkt.ekf_status_report.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_FENCE_STATUS:                         
          mavlink_msg_fence_status_decode(&msg, &pkt.fence_status.data);                                      
          pkt.fence_status.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_GIMBAL_REPORT:                         
          mavlink_msg_gimbal_report_decode(&msg, &pkt.gimbal_report.data);                                      
          pkt.gimbal_report.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_GIMBAL_TORQUE_CMD_REPORT:                         
          mavlink_msg_gimbal_torque_cmd_report_decode(&msg, &pkt.gimbal_torque_cmd_report.data);                                      
          pkt.gimbal_torque_cmd_report.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:                         
          mavlink_msg_global_position_int_decode(&msg, &pkt.global_position_int.data);                                      
          pkt.global_position_int.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_GOPRO_HEARTBEAT:                         
          mavlink_msg_gopro_heartbeat_decode(&msg, &pkt.gopro_heartbeat.data);                                      
          pkt.gopro_heartbeat.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_GOPRO_SET_RESPONSE:                         
          mavlink_msg_gopro_set_response_decode(&msg, &pkt.gopro_set_response.data);                                      
          pkt.gopro_set_response.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_GPS_RAW_INT:                         
          mavlink_msg_gps_raw_int_decode(&msg, &pkt.gps_raw_int.data);                                      
          pkt.gps_raw_int.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_HEARTBEAT:                         
          mavlink_msg_heartbeat_decode(&msg, &pkt.heartbeat.data);                                      
          pkt.heartbeat.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_HWSTATUS:                         
          mavlink_msg_hwstatus_decode(&msg, &pkt.hwstatus.data);                                      
          pkt.hwstatus.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_MEMINFO:                         
          mavlink_msg_meminfo_decode(&msg, &pkt.meminfo.data);                                      
          pkt.meminfo.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_MISSION_CURRENT:                         
          mavlink_msg_mission_current_decode(&msg, &pkt.mission_current.data);                                      
          pkt.mission_current.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_MOUNT_STATUS:                         
          mavlink_msg_mount_status_decode(&msg, &pkt.mount_status.data);                                      
          pkt.mount_status.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:                         
          mavlink_msg_nav_controller_output_decode(&msg, &pkt.nav_controller_output.data);                                      
          pkt.nav_controller_output.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_PARAM_REQUEST_READ:                         
          mavlink_msg_param_request_read_decode(&msg, &pkt.param_request_read.data);                                      
          pkt.param_request_read.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:                         
          mavlink_msg_param_request_list_decode(&msg, &pkt.param_request_list.data);                                      
          pkt.param_request_list.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_PARAM_VALUE:                         
          mavlink_msg_param_value_decode(&msg, &pkt.param_value.data);                                      
          pkt.param_value.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_POWER_STATUS:                         
          mavlink_msg_power_status_decode(&msg, &pkt.power_status.data);                                      
          pkt.power_status.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_RAW_IMU:                         
          mavlink_msg_raw_imu_decode(&msg, &pkt.raw_imu.data);                                      
          pkt.raw_imu.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_RC_CHANNELS:                         
          mavlink_msg_rc_channels_decode(&msg, &pkt.rc_channels.data);                                      
          pkt.rc_channels.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_RC_CHANNELS_RAW:                         
          mavlink_msg_rc_channels_raw_decode(&msg, &pkt.rc_channels_raw.data);                                      
          pkt.rc_channels_raw.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_REMOTE_LOG_BLOCK_STATUS:                         
          mavlink_msg_remote_log_block_status_decode(&msg, &pkt.remote_log_block_status.data);                                      
          pkt.remote_log_block_status.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_REQUEST_DATA_STREAM:                         
          mavlink_msg_request_data_stream_decode(&msg, &pkt.request_data_stream.data);                                      
          pkt.request_data_stream.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_SCALED_IMU2:                         
          mavlink_msg_scaled_imu2_decode(&msg, &pkt.scaled_imu2.data);                                      
          pkt.scaled_imu2.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_SCALED_IMU3:                         
          mavlink_msg_scaled_imu3_decode(&msg, &pkt.scaled_imu3.data);                                      
          pkt.scaled_imu3.tUpdate = millis();     
        break;  
        case MAVLINK_MSG_ID_SCALED_PRESSURE:                         
          mavlink_msg_scaled_pressure_decode(&msg, &pkt.scaled_pressure.data);                                      
          pkt.scaled_pressure.tUpdate = millis();     
        break;
        case MAVLINK_MSG_ID_SCALED_PRESSURE2:                         
          mavlink_msg_scaled_pressure2_decode(&msg, &pkt.scaled_pressure2.data);                                      
          pkt.scaled_pressure2.tUpdate = millis();     
        break;  
        case MAVLINK_MSG_ID_SENSOR_OFFSETS:                         
          mavlink_msg_sensor_offsets_decode(&msg, &pkt.sensor_offsets.data);                                      
          pkt.sensor_offsets.tUpdate = millis();     
        break;  
        case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:                         
          mavlink_msg_servo_output_raw_decode(&msg, &pkt.servo_output_raw.data);                                      
          pkt.servo_output_raw.tUpdate = millis();     
        break;  
        case MAVLINK_MSG_ID_SET_MODE:                         
          mavlink_msg_set_mode_decode(&msg, &pkt.set_mode.data);                                      
          pkt.set_mode.tUpdate = millis();     
        break;  
        case MAVLINK_MSG_ID_STATUSTEXT:                         
          mavlink_msg_statustext_decode(&msg, &pkt.statustext.data);                                      
          pkt.statustext.tUpdate = millis();     
        break;     
        case MAVLINK_MSG_ID_SYS_STATUS:                         
          mavlink_msg_sys_status_decode(&msg, &pkt.sys_status.data);                                      
          pkt.sys_status.tUpdate = millis();     
        break;     
        case MAVLINK_MSG_ID_SYSTEM_TIME:                         
          mavlink_msg_system_time_decode(&msg, &pkt.system_time.data);                                      
          pkt.system_time.tUpdate = millis();     
        break;     
        case MAVLINK_MSG_ID_TIMESYNC:                         
          mavlink_msg_timesync_decode(&msg, &pkt.timesync.data);                                      
          pkt.timesync.tUpdate = millis();     
        break;     
        case MAVLINK_MSG_ID_TERRAIN_REPORT:                         
          mavlink_msg_terrain_report_decode(&msg, &pkt.terrain_report.data);                                      
          pkt.terrain_report.tUpdate = millis();     
        break;  
        case MAVLINK_MSG_ID_VFR_HUD:                         
          mavlink_msg_vfr_hud_decode(&msg, &pkt.vfr_hud.data);                                      
          pkt.vfr_hud.tUpdate = millis();     
        break;  
        case MAVLINK_MSG_ID_VIBRATION:                         
          mavlink_msg_vibration_decode(&msg, &pkt.vibration.data);                                      
          pkt.vibration.tUpdate = millis();     
        break;      
      }

      if(!parsed_packet)
      {
        ALOG_INF(PSTR("NOT parsed_packet = %d, id = %d, msg = \"%S\""), parsed_packet, msg.msgid, MavLink_Msg_FriendlyName_By_ID(msg.msgid, buffer, sizeof(buffer)));
      }

    }
  }

}


const char* mMAVLink::MavLink_Msg_FriendlyName_By_ID(uint16_t id, char* buffer, uint8_t buflen)
{

  if(buffer == nullptr){ return PM_SEARCH_NOMATCH;}
  switch(id){
    default:     memcpy_P(buffer, PM_SEARCH_NOMATCH, sizeof(PM_SEARCH_NOMATCH)); break;
    case MAVLINK_MSG_ID_ACTUATOR_CONTROL_TARGET:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ACTUATOR_CONTROL_TARGET__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ACTUATOR_CONTROL_TARGET__CTR)); break;
    case MAVLINK_MSG_ID_ADAP_TUNING:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ADAP_TUNING__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ADAP_TUNING__CTR)); break;
    case MAVLINK_MSG_ID_ADSB_VEHICLE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ADSB_VEHICLE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__AHRS__CTR)); break;
    case MAVLINK_MSG_ID_AHRS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__AHRS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__AHRS__CTR)); break;
    case MAVLINK_MSG_ID_AHRS2:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__AHRS2__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__AHRS2__CTR)); break;
    case MAVLINK_MSG_ID_AHRS3:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__AHRS3__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__AHRS3__CTR)); break;
    case MAVLINK_MSG_ID_AIRSPEED_AUTOCAL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__AIRSPEED_AUTOCAL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__AIRSPEED_AUTOCAL__CTR)); break;
    case MAVLINK_MSG_ID_ALTITUDE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ALTITUDE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ALTITUDE__CTR)); break;
    case MAVLINK_MSG_ID_AOA_SSA:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__AOA_SSA__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__AOA_SSA__CTR)); break;
    case MAVLINK_MSG_ID_AP_ADC:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__AP_ADC__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__AP_ADC__CTR)); break;
    case MAVLINK_MSG_ID_ATTITUDE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE__CTR)); break;
    case MAVLINK_MSG_ID_ATTITUDE_QUATERNION:        memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE_QUATERNION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE_QUATERNION__CTR)); break;
    case MAVLINK_MSG_ID_ATTITUDE_QUATERNION_COV:    memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE_QUATERNION_COV__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE_QUATERNION_COV__CTR)); break;
    case MAVLINK_MSG_ID_ATTITUDE_TARGET:            memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE_TARGET__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE_TARGET__CTR)); break;
    case MAVLINK_MSG_ID_ATT_POS_MOCAP:              memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ATT_POS_MOCAP__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ATT_POS_MOCAP__CTR)); break;
    case MAVLINK_MSG_ID_AUTH_KEY:                   memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__AUTH_KEY__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__AUTH_KEY__CTR)); break;
    case MAVLINK_MSG_ID_AUTOPILOT_VERSION:          memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__AUTOPILOT_VERSION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__AUTOPILOT_VERSION__CTR)); break;
    case MAVLINK_MSG_ID_AUTOPILOT_VERSION_REQUEST:  memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__AUTOPILOT_VERSION_REQUEST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__AUTOPILOT_VERSION_REQUEST__CTR)); break;
    case MAVLINK_MSG_ID_BATTERY2:                   memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__BATTERY2__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__BATTERY2__CTR)); break;
    case MAVLINK_MSG_ID_BATTERY_STATUS:             memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__BATTERY_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__BATTERY_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_BUTTON_CHANGE:              memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__BUTTON_CHANGE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__BUTTON_CHANGE__CTR)); break;
    case MAVLINK_MSG_ID_CAMERA_CAPTURE_STATUS:      memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__CAMERA_CAPTURE_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_CAPTURE_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_CAMERA_FEEDBACK:            memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__CAMERA_FEEDBACK__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_FEEDBACK__CTR)); break;
    case MAVLINK_MSG_ID_CAMERA_IMAGE_CAPTURED:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__CAMERA_IMAGE_CAPTURED__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_IMAGE_CAPTURED__CTR)); break;
    case MAVLINK_MSG_ID_CAMERA_INFORMATION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__CAMERA_INFORMATION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_INFORMATION__CTR)); break;
    case MAVLINK_MSG_ID_CAMERA_SETTINGS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__CAMERA_SETTINGS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_SETTINGS__CTR)); break;
    case MAVLINK_MSG_ID_CAMERA_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__CAMERA_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_CAMERA_TRIGGER:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__CAMERA_TRIGGER__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_TRIGGER__CTR)); break;
    case MAVLINK_MSG_ID_CHANGE_OPERATOR_CONTROL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__CHANGE_OPERATOR_CONTROL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__CHANGE_OPERATOR_CONTROL__CTR)); break;
    case MAVLINK_MSG_ID_CHANGE_OPERATOR_CONTROL_ACK:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__CHANGE_OPERATOR_CONTROL_ACK__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__CHANGE_OPERATOR_CONTROL_ACK__CTR)); break;
    case MAVLINK_MSG_ID_COLLISION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__COLLISION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__COLLISION__CTR)); break;
    case MAVLINK_MSG_ID_COMMAND_ACK:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__COMMAND_ACK__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__COMMAND_ACK__CTR)); break;
    case MAVLINK_MSG_ID_COMMAND_INT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__COMMAND_INT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__COMMAND_INT__CTR)); break;
    case MAVLINK_MSG_ID_COMMAND_LONG:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__COMMAND_LONG__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__COMMAND_LONG__CTR)); break;
    case MAVLINK_MSG_ID_COMPASSMOT_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__COMPASSMOT_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__COMPASSMOT_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_CONTROL_SYSTEM_STATE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__CONTROL_SYSTEM_STATE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__CONTROL_SYSTEM_STATE__CTR)); break;
    case MAVLINK_MSG_ID_DATA16:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DATA16__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DATA16__CTR)); break;
    case MAVLINK_MSG_ID_DATA32:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DATA32__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DATA32__CTR)); break;
    case MAVLINK_MSG_ID_DATA64:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DATA64__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DATA64__CTR)); break;
    case MAVLINK_MSG_ID_DATA96:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DATA96__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DATA96__CTR)); break;
    case MAVLINK_MSG_ID_DATA_STREAM:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DATA_STREAM__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DATA_STREAM__CTR)); break;
    case MAVLINK_MSG_ID_DATA_TRANSMISSION_HANDSHAKE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DATA_TRANSMISSION_HANDSHAKE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DATA_TRANSMISSION_HANDSHAKE__CTR)); break;
    case MAVLINK_MSG_ID_DEBUG:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DEBUG__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DEBUG__CTR)); break;
    case MAVLINK_MSG_ID_DEBUG_VECT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DEBUG_VECT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DEBUG_VECT__CTR)); break;
    case MAVLINK_MSG_ID_DEEPSTALL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DEEPSTALL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DEEPSTALL__CTR)); break;
    case MAVLINK_MSG_ID_DEVICE_OP_READ:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_READ__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_READ__CTR)); break;
    case MAVLINK_MSG_ID_DEVICE_OP_READ_REPLY:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_READ_REPLY__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_READ_REPLY__CTR)); break;
    case MAVLINK_MSG_ID_DEVICE_OP_WRITE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_WRITE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_WRITE__CTR)); break;
    case MAVLINK_MSG_ID_DEVICE_OP_WRITE_REPLY:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_WRITE_REPLY__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_WRITE_REPLY__CTR)); break;
    case MAVLINK_MSG_ID_DIGICAM_CONFIGURE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DIGICAM_CONFIGURE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DIGICAM_CONFIGURE__CTR)); break;
    case MAVLINK_MSG_ID_DIGICAM_CONTROL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DIGICAM_CONTROL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DIGICAM_CONTROL__CTR)); break;
    case MAVLINK_MSG_ID_DISTANCE_SENSOR:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__DISTANCE_SENSOR__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__DISTANCE_SENSOR__CTR)); break;
    case MAVLINK_MSG_ID_EKF_STATUS_REPORT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__EKF_STATUS_REPORT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__EKF_STATUS_REPORT__CTR)); break;
    case MAVLINK_MSG_ID_ENCAPSULATED_DATA:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ENCAPSULATED_DATA__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ENCAPSULATED_DATA__CTR)); break;
    case MAVLINK_MSG_ID_ESTIMATOR_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ESTIMATOR_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ESTIMATOR_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_EXTENDED_SYS_STATE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__EXTENDED_SYS_STATE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__EXTENDED_SYS_STATE__CTR)); break;
    case MAVLINK_MSG_ID_FENCE_FETCH_POINT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__FENCE_FETCH_POINT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__FENCE_FETCH_POINT__CTR)); break;
    case MAVLINK_MSG_ID_FENCE_POINT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__FENCE_POINT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__FENCE_POINT__CTR)); break;
    case MAVLINK_MSG_ID_FENCE_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__FENCE_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__FENCE_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_FILE_TRANSFER_PROTOCOL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__FILE_TRANSFER_PROTOCOL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__FILE_TRANSFER_PROTOCOL__CTR)); break;
    case MAVLINK_MSG_ID_FLIGHT_INFORMATION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__FLIGHT_INFORMATION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__FLIGHT_INFORMATION__CTR)); break;
    case MAVLINK_MSG_ID_FOLLOW_TARGET:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__FOLLOW_TARGET__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__FOLLOW_TARGET__CTR)); break;
    case MAVLINK_MSG_ID_GIMBAL_CONTROL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_CONTROL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_CONTROL__CTR)); break;
    case MAVLINK_MSG_ID_GIMBAL_REPORT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_REPORT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_REPORT__CTR)); break;
    case MAVLINK_MSG_ID_GIMBAL_TORQUE_CMD_REPORT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_TORQUE_CMD_REPORT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_TORQUE_CMD_REPORT__CTR)); break;
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_POSITION_INT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_POSITION_INT__CTR)); break;
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT_COV:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_POSITION_INT_COV__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_POSITION_INT_COV__CTR)); break;
    case MAVLINK_MSG_ID_GLOBAL_VISION_POSITION_ESTIMATE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_VISION_POSITION_ESTIMATE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_VISION_POSITION_ESTIMATE__CTR)); break;
    case MAVLINK_MSG_ID_GOPRO_GET_REQUEST:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GOPRO_GET_REQUEST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GOPRO_GET_REQUEST__CTR)); break;
    case MAVLINK_MSG_ID_GOPRO_GET_RESPONSE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GOPRO_GET_RESPONSE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GOPRO_GET_RESPONSE__CTR)); break;
    case MAVLINK_MSG_ID_GOPRO_HEARTBEAT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GOPRO_HEARTBEAT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GOPRO_HEARTBEAT__CTR)); break;
    case MAVLINK_MSG_ID_GOPRO_SET_REQUEST:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GOPRO_SET_REQUEST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GOPRO_SET_REQUEST__CTR)); break;
    case MAVLINK_MSG_ID_GOPRO_SET_RESPONSE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GOPRO_SET_RESPONSE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GOPRO_SET_RESPONSE__CTR)); break;
    case MAVLINK_MSG_ID_GPS2_RAW:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GPS2_RAW__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GPS2_RAW__CTR)); break;
    case MAVLINK_MSG_ID_GPS2_RTK:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GPS2_RTK__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GPS2_RTK__CTR)); break;
    case MAVLINK_MSG_ID_GPS_GLOBAL_ORIGIN:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GPS_GLOBAL_ORIGIN__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GPS_GLOBAL_ORIGIN__CTR)); break;
    case MAVLINK_MSG_ID_GPS_INJECT_DATA:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GPS_INJECT_DATA__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GPS_INJECT_DATA__CTR)); break;
    case MAVLINK_MSG_ID_GPS_INPUT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GPS_INPUT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GPS_INPUT__CTR)); break;
    case MAVLINK_MSG_ID_GPS_RAW_INT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GPS_RAW_INT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GPS_RAW_INT__CTR)); break;
    case MAVLINK_MSG_ID_GPS_RTCM_DATA:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GPS_RTCM_DATA__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GPS_RTCM_DATA__CTR)); break;
    case MAVLINK_MSG_ID_GPS_RTK:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GPS_RTK__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GPS_RTK__CTR)); break;
    case MAVLINK_MSG_ID_GPS_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__GPS_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__GPS_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_HEARTBEAT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HEARTBEAT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HEARTBEAT__CTR)); break;
    case MAVLINK_MSG_ID_HIGHRES_IMU:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIGHRES_IMU__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIGHRES_IMU__CTR)); break;
    case MAVLINK_MSG_ID_HIGH_LATENCY:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIGH_LATENCY__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIGH_LATENCY__CTR)); break;
    case MAVLINK_MSG_ID_HIGH_LATENCY2:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIGH_LATENCY2__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIGH_LATENCY2__CTR)); break;
    case MAVLINK_MSG_ID_HIL_ACTUATOR_CONTROLS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIL_ACTUATOR_CONTROLS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIL_ACTUATOR_CONTROLS__CTR)); break;
    case MAVLINK_MSG_ID_HIL_CONTROLS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIL_CONTROLS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIL_CONTROLS__CTR)); break;
    case MAVLINK_MSG_ID_HIL_GPS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIL_GPS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIL_GPS__CTR)); break;
    case MAVLINK_MSG_ID_HIL_OPTICAL_FLOW:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIL_OPTICAL_FLOW__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIL_OPTICAL_FLOW__CTR)); break;
    case MAVLINK_MSG_ID_HIL_RC_INPUTS_RAW:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIL_RC_INPUTS_RAW__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIL_RC_INPUTS_RAW__CTR)); break;
    case MAVLINK_MSG_ID_HIL_SENSOR:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIL_SENSOR__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIL_SENSOR__CTR)); break;
    case MAVLINK_MSG_ID_HIL_STATE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIL_STATE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIL_STATE__CTR)); break;
    case MAVLINK_MSG_ID_HIL_STATE_QUATERNION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HIL_STATE_QUATERNION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HIL_STATE_QUATERNION__CTR)); break;
    case MAVLINK_MSG_ID_HOME_POSITION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HOME_POSITION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HOME_POSITION__CTR)); break;
    case MAVLINK_MSG_ID_HWSTATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__HWSTATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__HWSTATUS__CTR)); break;
    case MAVLINK_MSG_ID_ICAROUS_HEARTBEAT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ICAROUS_HEARTBEAT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ICAROUS_HEARTBEAT__CTR)); break;
    case MAVLINK_MSG_ID_ICAROUS_KINEMATIC_BANDS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ICAROUS_KINEMATIC_BANDS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ICAROUS_KINEMATIC_BANDS__CTR)); break;
    case MAVLINK_MSG_ID_LANDING_TARGET:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LANDING_TARGET__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LANDING_TARGET__CTR)); break;
    case MAVLINK_MSG_ID_LED_CONTROL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LED_CONTROL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LED_CONTROL__CTR)); break;
    case MAVLINK_MSG_ID_LIMITS_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LIMITS_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LIMITS_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_LOCAL_POSITION_NED:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOCAL_POSITION_NED__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOCAL_POSITION_NED__CTR)); break;
    case MAVLINK_MSG_ID_LOCAL_POSITION_NED_COV:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOCAL_POSITION_NED_COV__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOCAL_POSITION_NED_COV__CTR)); break;
    case MAVLINK_MSG_ID_LOCAL_POSITION_NED_SYSTEM_GLOBAL_OFFSET:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOCAL_POSITION_NED_SYSTEM_GLOBAL_OFFSET__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOCAL_POSITION_NED_SYSTEM_GLOBAL_OFFSET__CTR)); break;
    case MAVLINK_MSG_ID_LOGGING_ACK:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOGGING_ACK__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOGGING_ACK__CTR)); break;
    case MAVLINK_MSG_ID_LOGGING_DATA:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOGGING_DATA__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOGGING_DATA__CTR)); break;
    case MAVLINK_MSG_ID_LOGGING_DATA_ACKED:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOGGING_DATA_ACKED__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOGGING_DATA_ACKED__CTR)); break;
    case MAVLINK_MSG_ID_LOG_DATA:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOG_DATA__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOG_DATA__CTR)); break;
    case MAVLINK_MSG_ID_LOG_ENTRY:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOG_ENTRY__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOG_ENTRY__CTR)); break;
    case MAVLINK_MSG_ID_LOG_ERASE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOG_ERASE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOG_ERASE__CTR)); break;
    case MAVLINK_MSG_ID_LOG_REQUEST_DATA:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOG_REQUEST_DATA__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOG_REQUEST_DATA__CTR)); break;
    case MAVLINK_MSG_ID_LOG_REQUEST_END:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOG_REQUEST_END__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOG_REQUEST_END__CTR)); break;
    case MAVLINK_MSG_ID_LOG_REQUEST_LIST:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__LOG_REQUEST_LIST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__LOG_REQUEST_LIST__CTR)); break;
    case MAVLINK_MSG_ID_MAG_CAL_PROGRESS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MAG_CAL_PROGRESS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MAG_CAL_PROGRESS__CTR)); break;
    case MAVLINK_MSG_ID_MAG_CAL_REPORT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MAG_CAL_REPORT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MAG_CAL_REPORT__CTR)); break;
    case MAVLINK_MSG_ID_MANUAL_CONTROL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MANUAL_CONTROL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MANUAL_CONTROL__CTR)); break;
    case MAVLINK_MSG_ID_MANUAL_SETPOINT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MANUAL_SETPOINT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MANUAL_SETPOINT__CTR)); break;
    case MAVLINK_MSG_ID_MEMINFO:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MEMINFO__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MEMINFO__CTR)); break;
    case MAVLINK_MSG_ID_MEMORY_VECT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MEMORY_VECT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MEMORY_VECT__CTR)); break;
    case MAVLINK_MSG_ID_MESSAGE_INTERVAL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MESSAGE_INTERVAL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MESSAGE_INTERVAL__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_ACK:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_ACK__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_ACK__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_CLEAR_ALL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_CLEAR_ALL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_CLEAR_ALL__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_COUNT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_COUNT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_COUNT__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_CURRENT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_CURRENT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_CURRENT__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_ITEM:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_ITEM__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_ITEM__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_ITEM_INT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_ITEM_INT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_ITEM_INT__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_ITEM_REACHED:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_ITEM_REACHED__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_ITEM_REACHED__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_REQUEST:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_REQUEST_INT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST_INT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST_INT__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_REQUEST_LIST:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST_LIST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST_LIST__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_REQUEST_PARTIAL_LIST:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST_PARTIAL_LIST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST_PARTIAL_LIST__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_SET_CURRENT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_SET_CURRENT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_SET_CURRENT__CTR)); break;
    case MAVLINK_MSG_ID_MISSION_WRITE_PARTIAL_LIST:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MISSION_WRITE_PARTIAL_LIST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MISSION_WRITE_PARTIAL_LIST__CTR)); break;
    case MAVLINK_MSG_ID_MOUNT_CONFIGURE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MOUNT_CONFIGURE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MOUNT_CONFIGURE__CTR)); break;
    case MAVLINK_MSG_ID_MOUNT_CONTROL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MOUNT_CONTROL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MOUNT_CONTROL__CTR)); break;
    case MAVLINK_MSG_ID_MOUNT_ORIENTATION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MOUNT_ORIENTATION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MOUNT_ORIENTATION__CTR)); break;
    case MAVLINK_MSG_ID_MOUNT_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__MOUNT_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__MOUNT_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_NAMED_VALUE_FLOAT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__NAMED_VALUE_FLOAT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__NAMED_VALUE_FLOAT__CTR)); break;
    case MAVLINK_MSG_ID_NAMED_VALUE_INT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__NAMED_VALUE_INT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__NAMED_VALUE_INT__CTR)); break;
    case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__NAV_CONTROLLER_OUTPUT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__NAV_CONTROLLER_OUTPUT__CTR)); break;
    case MAVLINK_MSG_ID_OBSTACLE_DISTANCE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__OBSTACLE_DISTANCE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__OBSTACLE_DISTANCE__CTR)); break;
    case MAVLINK_MSG_ID_ODOMETRY:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__ODOMETRY__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__ODOMETRY__CTR)); break;
    case MAVLINK_MSG_ID_OPTICAL_FLOW:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__OPTICAL_FLOW__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__OPTICAL_FLOW__CTR)); break;
    case MAVLINK_MSG_ID_OPTICAL_FLOW_RAD:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__OPTICAL_FLOW_RAD__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__OPTICAL_FLOW_RAD__CTR)); break;
    case MAVLINK_MSG_ID_PARAM_EXT_ACK:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_ACK__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_ACK__CTR)); break;
    case MAVLINK_MSG_ID_PARAM_EXT_REQUEST_LIST:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_REQUEST_LIST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_REQUEST_LIST__CTR)); break;
    case MAVLINK_MSG_ID_PARAM_EXT_REQUEST_READ:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_REQUEST_READ__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_REQUEST_READ__CTR)); break;
    case MAVLINK_MSG_ID_PARAM_EXT_SET:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_SET__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_SET__CTR)); break;
    case MAVLINK_MSG_ID_PARAM_EXT_VALUE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_VALUE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_VALUE__CTR)); break;
    case MAVLINK_MSG_ID_PARAM_MAP_RC:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PARAM_MAP_RC__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PARAM_MAP_RC__CTR)); break;
    case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PARAM_REQUEST_LIST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PARAM_REQUEST_LIST__CTR)); break;
    case MAVLINK_MSG_ID_PARAM_REQUEST_READ:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PARAM_REQUEST_READ__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PARAM_REQUEST_READ__CTR)); break;
    case MAVLINK_MSG_ID_PARAM_SET:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PARAM_SET__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PARAM_SET__CTR)); break;
    case MAVLINK_MSG_ID_PARAM_VALUE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PARAM_VALUE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PARAM_VALUE__CTR)); break;
    case MAVLINK_MSG_ID_PID_TUNING:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PID_TUNING__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PID_TUNING__CTR)); break;
    case MAVLINK_MSG_ID_PING:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PING__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PING__CTR)); break;
    case MAVLINK_MSG_ID_PLAY_TUNE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PLAY_TUNE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PLAY_TUNE__CTR)); break;
    case MAVLINK_MSG_ID_POSITION_TARGET_GLOBAL_INT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__POSITION_TARGET_GLOBAL_INT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__POSITION_TARGET_GLOBAL_INT__CTR)); break;
    case MAVLINK_MSG_ID_POSITION_TARGET_LOCAL_NED:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__POSITION_TARGET_LOCAL_NED__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__POSITION_TARGET_LOCAL_NED__CTR)); break;
    case MAVLINK_MSG_ID_POWER_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__POWER_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__POWER_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_PROTOCOL_VERSION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__PROTOCOL_VERSION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__PROTOCOL_VERSION__CTR)); break;
    case MAVLINK_MSG_ID_RADIO:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RADIO__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RADIO__CTR)); break;
    case MAVLINK_MSG_ID_RADIO_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RADIO_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RADIO_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_RALLY_FETCH_POINT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RALLY_FETCH_POINT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RALLY_FETCH_POINT__CTR)); break;
    case MAVLINK_MSG_ID_RALLY_POINT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RALLY_POINT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RALLY_POINT__CTR)); break;
    case MAVLINK_MSG_ID_RANGEFINDER:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RANGEFINDER__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RANGEFINDER__CTR)); break;
    case MAVLINK_MSG_ID_RAW_IMU:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RAW_IMU__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RAW_IMU__CTR)); break;
    case MAVLINK_MSG_ID_RAW_PRESSURE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RAW_PRESSURE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RAW_PRESSURE__CTR)); break;
    case MAVLINK_MSG_ID_RC_CHANNELS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS__CTR)); break;
    case MAVLINK_MSG_ID_RC_CHANNELS_OVERRIDE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_OVERRIDE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_OVERRIDE__CTR)); break;
    case MAVLINK_MSG_ID_RC_CHANNELS_RAW:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_RAW__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_RAW__CTR)); break;
    case MAVLINK_MSG_ID_RC_CHANNELS_SCALED:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_SCALED__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_SCALED__CTR)); break;
    case MAVLINK_MSG_ID_REMOTE_LOG_BLOCK_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__REMOTE_LOG_BLOCK_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__REMOTE_LOG_BLOCK_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_REMOTE_LOG_DATA_BLOCK:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__REMOTE_LOG_DATA_BLOCK__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__REMOTE_LOG_DATA_BLOCK__CTR)); break;
    case MAVLINK_MSG_ID_REQUEST_DATA_STREAM:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__REQUEST_DATA_STREAM__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__REQUEST_DATA_STREAM__CTR)); break;
    case MAVLINK_MSG_ID_RESOURCE_REQUEST:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RESOURCE_REQUEST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RESOURCE_REQUEST__CTR)); break;
    case MAVLINK_MSG_ID_RPM:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__RPM__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__RPM__CTR)); break;
    case MAVLINK_MSG_ID_SAFETY_ALLOWED_AREA:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SAFETY_ALLOWED_AREA__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SAFETY_ALLOWED_AREA__CTR)); break;
    case MAVLINK_MSG_ID_SAFETY_SET_ALLOWED_AREA:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SAFETY_SET_ALLOWED_AREA__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SAFETY_SET_ALLOWED_AREA__CTR)); break;
    case MAVLINK_MSG_ID_SCALED_IMU:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU__CTR)); break;
    case MAVLINK_MSG_ID_SCALED_IMU2:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU2__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU2__CTR)); break;
    case MAVLINK_MSG_ID_SCALED_IMU3:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU3__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU3__CTR)); break;
    case MAVLINK_MSG_ID_SCALED_PRESSURE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE__CTR)); break;
    case MAVLINK_MSG_ID_SCALED_PRESSURE2:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE2__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE2__CTR)); break;
    case MAVLINK_MSG_ID_SCALED_PRESSURE3:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE3__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE3__CTR)); break;
    case MAVLINK_MSG_ID_SENSOR_OFFSETS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SENSOR_OFFSETS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SENSOR_OFFSETS__CTR)); break;
    case MAVLINK_MSG_ID_SERIAL_CONTROL:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SERIAL_CONTROL__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SERIAL_CONTROL__CTR)); break;
    case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SERVO_OUTPUT_RAW__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SERVO_OUTPUT_RAW__CTR)); break;
    case MAVLINK_MSG_ID_SETUP_SIGNING:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SETUP_SIGNING__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SETUP_SIGNING__CTR)); break;
    case MAVLINK_MSG_ID_SET_ACTUATOR_CONTROL_TARGET:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SET_ACTUATOR_CONTROL_TARGET__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SET_ACTUATOR_CONTROL_TARGET__CTR)); break;
    case MAVLINK_MSG_ID_SET_ATTITUDE_TARGET:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SET_ATTITUDE_TARGET__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SET_ATTITUDE_TARGET__CTR)); break;
    case MAVLINK_MSG_ID_SET_GPS_GLOBAL_ORIGIN:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SET_GPS_GLOBAL_ORIGIN__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SET_GPS_GLOBAL_ORIGIN__CTR)); break;
    case MAVLINK_MSG_ID_SET_HOME_POSITION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SET_HOME_POSITION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SET_HOME_POSITION__CTR)); break;
    case MAVLINK_MSG_ID_SET_MAG_OFFSETS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SET_MAG_OFFSETS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SET_MAG_OFFSETS__CTR)); break;
    case MAVLINK_MSG_ID_SET_MODE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SET_MODE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SET_MODE__CTR)); break;
    case MAVLINK_MSG_ID_SET_POSITION_TARGET_GLOBAL_INT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SET_POSITION_TARGET_GLOBAL_INT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SET_POSITION_TARGET_GLOBAL_INT__CTR)); break;
    case MAVLINK_MSG_ID_SET_POSITION_TARGET_LOCAL_NED:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SET_POSITION_TARGET_LOCAL_NED__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SET_POSITION_TARGET_LOCAL_NED__CTR)); break;
    case MAVLINK_MSG_ID_SET_VIDEO_STREAM_SETTINGS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SET_VIDEO_STREAM_SETTINGS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SET_VIDEO_STREAM_SETTINGS__CTR)); break;
    case MAVLINK_MSG_ID_SIMSTATE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SIMSTATE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SIMSTATE__CTR)); break;
    case MAVLINK_MSG_ID_SIM_STATE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SIM_STATE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SIM_STATE__CTR)); break;
    case MAVLINK_MSG_ID_STATUSTEXT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__STATUSTEXT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__STATUSTEXT__CTR)); break;
    case MAVLINK_MSG_ID_STORAGE_INFORMATION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__STORAGE_INFORMATION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__STORAGE_INFORMATION__CTR)); break;
    case MAVLINK_MSG_ID_SYSTEM_TIME:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SYSTEM_TIME__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SYSTEM_TIME__CTR)); break;
    case MAVLINK_MSG_ID_SYS_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__SYS_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__SYS_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_TERRAIN_CHECK:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_CHECK__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_CHECK__CTR)); break;
    case MAVLINK_MSG_ID_TERRAIN_DATA:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_DATA__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_DATA__CTR)); break;
    case MAVLINK_MSG_ID_TERRAIN_REPORT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_REPORT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_REPORT__CTR)); break;
    case MAVLINK_MSG_ID_TERRAIN_REQUEST:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_REQUEST__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_REQUEST__CTR)); break;
    case MAVLINK_MSG_ID_TIMESYNC:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__TIMESYNC__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__TIMESYNC__CTR)); break;
    case MAVLINK_MSG_ID_TRAJECTORY:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__TRAJECTORY__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__TRAJECTORY__CTR)); break;
    case MAVLINK_MSG_ID_UAVCAN_NODE_INFO:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__UAVCAN_NODE_INFO__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__UAVCAN_NODE_INFO__CTR)); break;
    case MAVLINK_MSG_ID_UAVCAN_NODE_STATUS:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__UAVCAN_NODE_STATUS__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__UAVCAN_NODE_STATUS__CTR)); break;
    case MAVLINK_MSG_ID_UAVIONIX_ADSB_OUT_CFG:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__UAVIONIX_ADSB_OUT_CFG__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__UAVIONIX_ADSB_OUT_CFG__CTR)); break;
    case MAVLINK_MSG_ID_UAVIONIX_ADSB_OUT_DYNAMIC:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__UAVIONIX_ADSB_OUT_DYNAMIC__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__UAVIONIX_ADSB_OUT_DYNAMIC__CTR)); break;
    case MAVLINK_MSG_ID_UAVIONIX_ADSB_TRANSCEIVER_HEALTH_REPORT:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__UAVIONIX_ADSB_TRANSCEIVER_HEALTH_REPORT__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__UAVIONIX_ADSB_TRANSCEIVER_HEALTH_REPORT__CTR)); break;
    case MAVLINK_MSG_ID_V2_EXTENSION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__V2_EXTENSION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__V2_EXTENSION__CTR)); break;
    case MAVLINK_MSG_ID_VFR_HUD:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__VFR_HUD__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__VFR_HUD__CTR)); break;
    case MAVLINK_MSG_ID_VIBRATION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__VIBRATION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__VIBRATION__CTR)); break;
    case MAVLINK_MSG_ID_VICON_POSITION_ESTIMATE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__VICON_POSITION_ESTIMATE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__VICON_POSITION_ESTIMATE__CTR)); break;
    case MAVLINK_MSG_ID_VIDEO_STREAM_INFORMATION:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__VIDEO_STREAM_INFORMATION__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__VIDEO_STREAM_INFORMATION__CTR)); break;
    case MAVLINK_MSG_ID_VISION_POSITION_DELTA:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__VISION_POSITION_DELTA__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__VISION_POSITION_DELTA__CTR)); break;
    case MAVLINK_MSG_ID_VISION_POSITION_ESTIMATE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__VISION_POSITION_ESTIMATE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__VISION_POSITION_ESTIMATE__CTR)); break;
    case MAVLINK_MSG_ID_VISION_SPEED_ESTIMATE:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__VISION_SPEED_ESTIMATE__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__VISION_SPEED_ESTIMATE__CTR)); break;
    case MAVLINK_MSG_ID_WIFI_CONFIG_AP:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__WIFI_CONFIG_AP__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__WIFI_CONFIG_AP__CTR)); break;
    case MAVLINK_MSG_ID_WIND:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__WIND__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__WIND__CTR)); break;
    case MAVLINK_MSG_ID_WIND_COV:           memcpy_P(buffer, PM_MAVLINK_MSG_PACKET_NAME__WIND_COV__CTR, sizeof(PM_MAVLINK_MSG_PACKET_NAME__WIND_COV__CTR)); break;
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

    
  JBI->Add("msg_count", unique_msg_id_list.size());

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
    JBI->Add("t", millis()-pkt.ahrs.tUpdate);
    JBI->Add("omegaIx", pkt.ahrs.data.omegaIx);
    JBI->Add("omegaIy", pkt.ahrs.data.omegaIy);
    JBI->Add("omegaIz", pkt.ahrs.data.omegaIz);
    JBI->Add("accel_weight", pkt.ahrs.data.accel_weight);
    JBI->Add("renorm_val", pkt.ahrs.data.renorm_val);
    JBI->Add("error_rp", pkt.ahrs.data.error_rp);
    JBI->Add("error_yaw", pkt.ahrs.data.error_yaw);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_ahrs2(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.ahrs2.tUpdate);
    JBI->Add("roll", pkt.ahrs2.data.roll);
    JBI->Add("pitch", pkt.ahrs2.data.pitch);
    JBI->Add("yaw", pkt.ahrs2.data.yaw);
    JBI->Add("altitude", pkt.ahrs2.data.altitude);
    JBI->Add("lat", pkt.ahrs2.data.lat);
    JBI->Add("lng", pkt.ahrs2.data.lng);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_attitude(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.attitude.tUpdate);
    JBI->Add("time_boot_ms", pkt.attitude.data.time_boot_ms);
    JBI->Add("roll", pkt.attitude.data.roll);
    JBI->Add("pitch", pkt.attitude.data.pitch);
    JBI->Add("yaw", pkt.attitude.data.yaw);
    JBI->Add("rollspeed", pkt.attitude.data.rollspeed);
    JBI->Add("pitchspeed", pkt.attitude.data.pitchspeed);
    JBI->Add("yawspeed", pkt.attitude.data.yawspeed);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_autopilot_version_request(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.autopilot_version.tUpdate);
    JBI->Add("capabilities", pkt.autopilot_version.data.capabilities);
    JBI->Add("uid", pkt.autopilot_version.data.uid);
    JBI->Add("flight_sw_version", pkt.autopilot_version.data.flight_sw_version);
    JBI->Add("middleware_sw_version", pkt.autopilot_version.data.middleware_sw_version);
    JBI->Add("os_sw_version", pkt.autopilot_version.data.os_sw_version);
    JBI->Add("board_version", pkt.autopilot_version.data.board_version);
    JBI->Add("vendor_id", pkt.autopilot_version.data.vendor_id);
    JBI->Add("product_id", pkt.autopilot_version.data.product_id);
    JBI->Array_AddArray("flight_custom_version", pkt.autopilot_version.data.flight_custom_version, ARRAY_SIZE(pkt.autopilot_version.data.flight_custom_version));
    JBI->Array_AddArray("middleware_custom_version", pkt.autopilot_version.data.middleware_custom_version, ARRAY_SIZE(pkt.autopilot_version.data.middleware_custom_version));
    JBI->Array_AddArray("os_custom_version", pkt.autopilot_version.data.os_custom_version, ARRAY_SIZE(pkt.autopilot_version.data.os_custom_version));
    JBI->Array_AddArray("uid2", pkt.autopilot_version.data.uid2, ARRAY_SIZE(pkt.autopilot_version.data.uid2));
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_battery_status(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.battery_status.tUpdate);
    JBI->Add("current_consumed", pkt.battery_status.data.current_consumed);
    JBI->Add("energy_consumed", pkt.battery_status.data.energy_consumed);
    JBI->Add("temperature", pkt.battery_status.data.temperature);
    JBI->Array_AddArray("voltages", pkt.battery_status.data.voltages, ARRAY_SIZE(pkt.battery_status.data.voltages));
    JBI->Add("current_battery", pkt.battery_status.data.current_battery);
    JBI->Add("id", pkt.battery_status.data.id);
    JBI->Add("battery_function", pkt.battery_status.data.battery_function);
    JBI->Add("type", pkt.battery_status.data.type);
    JBI->Add("battery_remaining", pkt.battery_status.data.battery_remaining);
    JBI->Add("time_remaining", pkt.battery_status.data.time_remaining);
    JBI->Add("charge_state", pkt.battery_status.data.charge_state);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_ekf_status_report(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.ekf_status_report.tUpdate);
    JBI->Add("velocity_variance", pkt.ekf_status_report.data.velocity_variance);
    JBI->Add("pos_horiz_variance", pkt.ekf_status_report.data.pos_horiz_variance);
    JBI->Add("pos_vert_variance", pkt.ekf_status_report.data.pos_vert_variance);
    JBI->Add("compass_variance", pkt.ekf_status_report.data.compass_variance);
    JBI->Add("terrain_alt_variance", pkt.ekf_status_report.data.terrain_alt_variance);
    JBI->Add("flags", pkt.ekf_status_report.data.flags);
    JBI->Add("airspeed_variance", pkt.ekf_status_report.data.airspeed_variance);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_fence_status(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.fence_status.tUpdate);
    JBI->Add("breach_time", pkt.fence_status.data.breach_time);
    JBI->Add("breach_count", pkt.fence_status.data.breach_count);
    JBI->Add("breach_status", pkt.fence_status.data.breach_status);
    JBI->Add("breach_type", pkt.fence_status.data.breach_type);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_gimbal_report(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.gimbal_report.tUpdate);
    JBI->Add("delta_time", pkt.gimbal_report.data.delta_time);
    JBI->Add("delta_angle_x", pkt.gimbal_report.data.delta_angle_x);
    JBI->Add("delta_angle_y", pkt.gimbal_report.data.delta_angle_y);
    JBI->Add("delta_angle_z", pkt.gimbal_report.data.delta_angle_z);
    JBI->Add("delta_velocity_x", pkt.gimbal_report.data.delta_velocity_x);
    JBI->Add("delta_velocity_y", pkt.gimbal_report.data.delta_velocity_y);
    JBI->Add("delta_velocity_z", pkt.gimbal_report.data.delta_velocity_z);
    JBI->Add("joint_roll", pkt.gimbal_report.data.joint_roll);
    JBI->Add("joint_el", pkt.gimbal_report.data.joint_el);
    JBI->Add("joint_az", pkt.gimbal_report.data.joint_az);
    JBI->Add("target_system", pkt.gimbal_report.data.target_system);
    JBI->Add("target_component", pkt.gimbal_report.data.target_component);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_gimbal_torque_cmd_report(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.gimbal_torque_cmd_report.tUpdate);
    JBI->Add("rl_torque_cmd", pkt.gimbal_torque_cmd_report.data.rl_torque_cmd);
    JBI->Add("el_torque_cmd", pkt.gimbal_torque_cmd_report.data.el_torque_cmd);
    JBI->Add("az_torque_cmd", pkt.gimbal_torque_cmd_report.data.az_torque_cmd);
    JBI->Add("target_system", pkt.gimbal_torque_cmd_report.data.target_system);
    JBI->Add("target_component", pkt.gimbal_torque_cmd_report.data.target_component);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_global_position_int(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.global_position_int.tUpdate);
    JBI->Add("time_boot_ms", pkt.global_position_int.data.time_boot_ms);
    JBI->Add("lat", pkt.global_position_int.data.lat);
    JBI->Add("lon", pkt.global_position_int.data.lon);
    JBI->Add("alt", pkt.global_position_int.data.alt);
    JBI->Add("relative_alt", pkt.global_position_int.data.relative_alt);
    JBI->Add("vx", pkt.global_position_int.data.vx);
    JBI->Add("vy", pkt.global_position_int.data.vy);
    JBI->Add("vz", pkt.global_position_int.data.vz);
    JBI->Add("hdg", pkt.global_position_int.data.hdg);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_gopro_heartbeat(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.gopro_heartbeat.tUpdate);
    JBI->Add("status", pkt.gopro_heartbeat.data.status);
    JBI->Add("capture_mode", pkt.gopro_heartbeat.data.capture_mode);
    JBI->Add("flags", pkt.gopro_heartbeat.data.flags);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_gps_raw_int(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.gps_raw_int.tUpdate);
    JBI->Add("time_usec", pkt.gps_raw_int.data.time_usec);
    JBI->Add("lat", pkt.gps_raw_int.data.lat);
    JBI->Add("lon", pkt.gps_raw_int.data.lon);
    JBI->Add("alt", pkt.gps_raw_int.data.alt);
    JBI->Add("eph", pkt.gps_raw_int.data.eph);
    JBI->Add("epv", pkt.gps_raw_int.data.epv);
    JBI->Add("vel", pkt.gps_raw_int.data.vel);
    JBI->Add("cog", pkt.gps_raw_int.data.cog);
    JBI->Add("fix_type", pkt.gps_raw_int.data.fix_type);
    JBI->Add("satellites_visible", pkt.gps_raw_int.data.satellites_visible);
    JBI->Add("alt_ellipsoid", pkt.gps_raw_int.data.alt_ellipsoid);
    JBI->Add("h_acc", pkt.gps_raw_int.data.h_acc);
    JBI->Add("v_acc", pkt.gps_raw_int.data.v_acc);
    JBI->Add("vel_acc", pkt.gps_raw_int.data.vel_acc);
    JBI->Add("hdg_acc", pkt.gps_raw_int.data.hdg_acc);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_heartbeat(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.heartbeat.tUpdate);
    JBI->Add("custom_mode", pkt.heartbeat.data.custom_mode);
    JBI->Add("type", pkt.heartbeat.data.type);
    JBI->Add("autopilot", pkt.heartbeat.data.autopilot);
    JBI->Add("base_mode", pkt.heartbeat.data.base_mode);
    JBI->Add("system_status", pkt.heartbeat.data.system_status);
    JBI->Add("mavlink_version", pkt.heartbeat.data.mavlink_version);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_hwstatus(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.hwstatus.tUpdate);
    JBI->Add("Vcc", pkt.hwstatus.data.Vcc);
    JBI->Add("I2Cerr", pkt.hwstatus.data.I2Cerr);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_meminfo(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.meminfo.tUpdate);
    JBI->Add("brkval", pkt.meminfo.data.brkval);
    JBI->Add("freemem", pkt.meminfo.data.freemem);
    JBI->Add("freemem32", pkt.meminfo.data.freemem32);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_mission_current(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.mission_current.tUpdate);
    JBI->Add("seq", pkt.mission_current.data.seq);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_mount_status(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.mount_status.tUpdate);
    JBI->Add("pointing_a", pkt.mount_status.data.pointing_a);
    JBI->Add("pointing_b", pkt.mount_status.data.pointing_b);
    JBI->Add("pointing_c", pkt.mount_status.data.pointing_c);
    JBI->Add("target_system", pkt.mount_status.data.target_system);
    JBI->Add("target_component", pkt.mount_status.data.target_component);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_nav_controller_output(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.nav_controller_output.tUpdate);
    JBI->Add("nav_roll", pkt.nav_controller_output.data.nav_roll);
    JBI->Add("nav_pitch", pkt.nav_controller_output.data.nav_pitch);
    JBI->Add("alt_error", pkt.nav_controller_output.data.alt_error);
    JBI->Add("aspd_error", pkt.nav_controller_output.data.aspd_error);
    JBI->Add("xtrack_error", pkt.nav_controller_output.data.xtrack_error);
    JBI->Add("nav_bearing", pkt.nav_controller_output.data.nav_bearing);
    JBI->Add("target_bearing", pkt.nav_controller_output.data.target_bearing);
    JBI->Add("wp_dist", pkt.nav_controller_output.data.wp_dist);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_param_value(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.param_value.tUpdate);
    JBI->Add("param_value", pkt.param_value.data.param_value);
    JBI->Add("param_count", pkt.param_value.data.param_count);
    JBI->Add("param_index", pkt.param_value.data.param_index);
    JBI->Array_AddArray("param_id", pkt.param_value.data.param_id, ARRAY_SIZE(pkt.param_value.data.param_id));
    JBI->Add("param_type", pkt.param_value.data.param_type);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_power_status(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.power_status.tUpdate);
    JBI->Add("Vcc", pkt.power_status.data.Vcc);
    JBI->Add("Vservo", pkt.power_status.data.Vservo);
    JBI->Add("flags", pkt.power_status.data.flags);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_raw_imu(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.raw_imu.tUpdate);
    JBI->Add("time_usec", pkt.raw_imu.data.time_usec);
    JBI->Add("xacc", pkt.raw_imu.data.xacc);
    JBI->Add("yacc", pkt.raw_imu.data.yacc);
    JBI->Add("zacc", pkt.raw_imu.data.zacc);
    JBI->Add("xgyro", pkt.raw_imu.data.xgyro);
    JBI->Add("ygyro", pkt.raw_imu.data.ygyro);
    JBI->Add("zgyro", pkt.raw_imu.data.zgyro);
    JBI->Add("xmag", pkt.raw_imu.data.xmag);
    JBI->Add("ymag", pkt.raw_imu.data.ymag);
    JBI->Add("zmag", pkt.raw_imu.data.zmag);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_rc_channels(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.rc_channels.tUpdate);
    JBI->Add("time_boot_ms", pkt.rc_channels.data.time_boot_ms);
    JBI->Add("chan1_raw", pkt.rc_channels.data.chan1_raw);
    JBI->Add("chan2_raw", pkt.rc_channels.data.chan2_raw);
    JBI->Add("chan3_raw", pkt.rc_channels.data.chan3_raw);
    JBI->Add("chan4_raw", pkt.rc_channels.data.chan4_raw);
    JBI->Add("chan5_raw", pkt.rc_channels.data.chan5_raw);
    JBI->Add("chan6_raw", pkt.rc_channels.data.chan6_raw);
    JBI->Add("chan7_raw", pkt.rc_channels.data.chan7_raw);
    JBI->Add("chan8_raw", pkt.rc_channels.data.chan8_raw);
    JBI->Add("chan9_raw", pkt.rc_channels.data.chan9_raw);
    JBI->Add("chan10_raw", pkt.rc_channels.data.chan10_raw);
    JBI->Add("chan11_raw", pkt.rc_channels.data.chan11_raw);
    JBI->Add("chan12_raw", pkt.rc_channels.data.chan12_raw);
    JBI->Add("chan13_raw", pkt.rc_channels.data.chan13_raw);
    JBI->Add("chan14_raw", pkt.rc_channels.data.chan14_raw);
    JBI->Add("chan15_raw", pkt.rc_channels.data.chan15_raw);
    JBI->Add("chan16_raw", pkt.rc_channels.data.chan16_raw);
    JBI->Add("chan17_raw", pkt.rc_channels.data.chan17_raw);
    JBI->Add("chan18_raw", pkt.rc_channels.data.chan18_raw);
    JBI->Add("chancount", pkt.rc_channels.data.chancount);
    JBI->Add("rssi", pkt.rc_channels.data.rssi);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_rc_channels_raw(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.rc_channels_raw.tUpdate);
    JBI->Add("time_boot_ms", pkt.rc_channels_raw.data.time_boot_ms);
    JBI->Add("chan1_raw", pkt.rc_channels_raw.data.chan1_raw);
    JBI->Add("chan2_raw", pkt.rc_channels_raw.data.chan2_raw);
    JBI->Add("chan3_raw", pkt.rc_channels_raw.data.chan3_raw);
    JBI->Add("chan4_raw", pkt.rc_channels_raw.data.chan4_raw);
    JBI->Add("chan5_raw", pkt.rc_channels_raw.data.chan5_raw);
    JBI->Add("chan6_raw", pkt.rc_channels_raw.data.chan6_raw);
    JBI->Add("chan7_raw", pkt.rc_channels_raw.data.chan7_raw);
    JBI->Add("chan8_raw", pkt.rc_channels_raw.data.chan8_raw);    
    JBI->Add("port", pkt.rc_channels_raw.data.port);
    JBI->Add("rssi", pkt.rc_channels_raw.data.rssi);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_request_data_stream(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.request_data_stream.tUpdate);
    JBI->Add("req_message_rate", pkt.request_data_stream.data.req_message_rate);
    JBI->Add("target_system", pkt.request_data_stream.data.target_system);
    JBI->Add("target_component", pkt.request_data_stream.data.target_component);
    JBI->Add("req_stream_id", pkt.request_data_stream.data.req_stream_id);
    JBI->Add("start_stop", pkt.request_data_stream.data.start_stop);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_imu2(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.scaled_imu2.tUpdate);
    JBI->Add("time_boot_ms", pkt.scaled_imu2.data.time_boot_ms);
    JBI->Add("xacc", pkt.scaled_imu2.data.xacc);
    JBI->Add("yacc", pkt.scaled_imu2.data.yacc);
    JBI->Add("zacc", pkt.scaled_imu2.data.zacc);
    JBI->Add("xgyro", pkt.scaled_imu2.data.xgyro);
    JBI->Add("ygyro", pkt.scaled_imu2.data.ygyro);
    JBI->Add("zgyro", pkt.scaled_imu2.data.zgyro);
    JBI->Add("xmag", pkt.scaled_imu2.data.xmag);
    JBI->Add("ymag", pkt.scaled_imu2.data.ymag);
    JBI->Add("zmag", pkt.scaled_imu2.data.zmag);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_imu3(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.scaled_imu3.tUpdate);
    JBI->Add("time_boot_ms", pkt.scaled_imu3.data.time_boot_ms);
    JBI->Add("xacc", pkt.scaled_imu3.data.xacc);
    JBI->Add("yacc", pkt.scaled_imu3.data.yacc);
    JBI->Add("zacc", pkt.scaled_imu3.data.zacc);
    JBI->Add("xgyro", pkt.scaled_imu3.data.xgyro);
    JBI->Add("ygyro", pkt.scaled_imu3.data.ygyro);
    JBI->Add("zgyro", pkt.scaled_imu3.data.zgyro);
    JBI->Add("xmag", pkt.scaled_imu3.data.xmag);
    JBI->Add("ymag", pkt.scaled_imu3.data.ymag);
    JBI->Add("zmag", pkt.scaled_imu3.data.zmag);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_scaled_pressure(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.scaled_pressure.tUpdate);
    JBI->Add("time_boot_ms", pkt.scaled_pressure.data.time_boot_ms);
    JBI->Add("press_abs", pkt.scaled_pressure.data.press_abs);
    JBI->Add("press_diff", pkt.scaled_pressure.data.press_diff);
    JBI->Add("temperature", pkt.scaled_pressure.data.temperature);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_scaled_pressure2(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.scaled_pressure2.tUpdate);
    JBI->Add("time_boot_ms", pkt.scaled_pressure2.data.time_boot_ms);
    JBI->Add("press_abs", pkt.scaled_pressure2.data.press_abs);
    JBI->Add("press_diff", pkt.scaled_pressure2.data.press_diff);
    JBI->Add("temperature", pkt.scaled_pressure2.data.temperature);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_sensor_offsets(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.sensor_offsets.tUpdate);
    JBI->Add("mag_declination", pkt.sensor_offsets.data.mag_declination);
    JBI->Add("raw_press", pkt.sensor_offsets.data.raw_press);
    JBI->Add("raw_temp", pkt.sensor_offsets.data.raw_temp);
    JBI->Add("gyro_cal_x", pkt.sensor_offsets.data.gyro_cal_x);
    JBI->Add("gyro_cal_y", pkt.sensor_offsets.data.gyro_cal_y);
    JBI->Add("gyro_cal_z", pkt.sensor_offsets.data.gyro_cal_z);
    JBI->Add("accel_cal_x", pkt.sensor_offsets.data.accel_cal_x);
    JBI->Add("accel_cal_y", pkt.sensor_offsets.data.accel_cal_y);
    JBI->Add("accel_cal_z", pkt.sensor_offsets.data.accel_cal_z);
    JBI->Add("mag_ofs_x", pkt.sensor_offsets.data.mag_ofs_x);
    JBI->Add("mag_ofs_y", pkt.sensor_offsets.data.mag_ofs_y);
    JBI->Add("mag_ofs_z", pkt.sensor_offsets.data.mag_ofs_z);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_servo_output_raw(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.servo_output_raw.tUpdate);
    JBI->Add("time_usec", pkt.servo_output_raw.data.time_usec);
    JBI->Add("servo1_raw", pkt.servo_output_raw.data.servo1_raw);
    JBI->Add("servo2_raw", pkt.servo_output_raw.data.servo2_raw);
    JBI->Add("servo3_raw", pkt.servo_output_raw.data.servo3_raw);
    JBI->Add("servo4_raw", pkt.servo_output_raw.data.servo4_raw);
    JBI->Add("servo5_raw", pkt.servo_output_raw.data.servo5_raw);
    JBI->Add("servo6_raw", pkt.servo_output_raw.data.servo6_raw);
    JBI->Add("servo7_raw", pkt.servo_output_raw.data.servo7_raw);
    JBI->Add("servo8_raw", pkt.servo_output_raw.data.servo8_raw);
    JBI->Add("port", pkt.servo_output_raw.data.port);
    JBI->Add("servo9_raw", pkt.servo_output_raw.data.servo9_raw);
    JBI->Add("servo10_raw", pkt.servo_output_raw.data.servo10_raw);
    JBI->Add("servo11_raw", pkt.servo_output_raw.data.servo11_raw);
    JBI->Add("servo12_raw", pkt.servo_output_raw.data.servo12_raw);
    JBI->Add("servo13_raw", pkt.servo_output_raw.data.servo13_raw);
    JBI->Add("servo14_raw", pkt.servo_output_raw.data.servo14_raw);
    JBI->Add("servo15_raw", pkt.servo_output_raw.data.servo15_raw);
    JBI->Add("servo16_raw", pkt.servo_output_raw.data.servo16_raw);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_statustext(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.statustext.tUpdate);
    JBI->Add("severity", pkt.statustext.data.severity);
    JBI->Add("text", pkt.statustext.data.text);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_sys_status(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.sys_status.tUpdate);
    JBI->Add("sensorsC", pkt.sys_status.data.onboard_control_sensors_present);
    JBI->Add("sensorsE", pkt.sys_status.data.onboard_control_sensors_enabled);
    JBI->Add("sensorsH", pkt.sys_status.data.onboard_control_sensors_health);
    JBI->Add("load", pkt.sys_status.data.load);
    JBI->Add("voltage_battery", pkt.sys_status.data.voltage_battery);
    JBI->Add("current_battery", pkt.sys_status.data.current_battery);
    JBI->Add("drop_rate_comm", pkt.sys_status.data.drop_rate_comm);
    JBI->Add("errors_comm", pkt.sys_status.data.errors_comm);
    JBI->Add("errors_count1", pkt.sys_status.data.errors_count1);
    JBI->Add("errors_count2", pkt.sys_status.data.errors_count2);
    JBI->Add("errors_count3", pkt.sys_status.data.errors_count3);
    JBI->Add("errors_count4", pkt.sys_status.data.errors_count4);
    JBI->Add("battery_remaining", pkt.sys_status.data.battery_remaining);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_system_time(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.system_time.tUpdate);
    JBI->Add("time_unix_usec", pkt.system_time.data.time_unix_usec);
    JBI->Add("time_boot_ms", pkt.system_time.data.time_boot_ms);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_timesync(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.timesync.tUpdate);
    JBI->Add("tc1", pkt.timesync.data.tc1);
    JBI->Add("ts1", pkt.timesync.data.ts1);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_vfr_hud(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.vfr_hud.tUpdate);
    JBI->Add("airspeed", pkt.vfr_hud.data.airspeed);
    JBI->Add("groundspeed", pkt.vfr_hud.data.groundspeed);
    JBI->Add("alt", pkt.vfr_hud.data.alt);
    JBI->Add("climb", pkt.vfr_hud.data.climb);
    JBI->Add("heading", pkt.vfr_hud.data.heading);
    JBI->Add("throttle", pkt.vfr_hud.data.throttle);
  return JBI->End();    
}

uint8_t mMAVLink::ConstructJSON_vibration(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("t", millis()-pkt.vibration.tUpdate);
    JBI->Add("time_usec", pkt.vibration.data.time_usec);
    JBI->Add("vibration_x", pkt.vibration.data.vibration_x);
    JBI->Add("vibration_y", pkt.vibration.data.vibration_y);
    JBI->Add("vibration_z", pkt.vibration.data.vibration_z);
    JBI->Add("clipping_0", pkt.vibration.data.clipping_0);
    JBI->Add("clipping_1", pkt.vibration.data.clipping_1);
    JBI->Add("clipping_2", pkt.vibration.data.clipping_2);
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
