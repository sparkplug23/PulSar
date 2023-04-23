#ifndef _MODULE__DRIVERS_MAVLINK__H
#define _MODULE__DRIVERS_MAVLINK__H

#define D_UNIQUE_MODULE__DRIVERS_MAVLINK__ID   ((4*1000)+05)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE__DRIVERS_MAVLINK

// #include "internal/PixhawkArduinoMAVLink.h"


#include "internal/ardupilotmega/mavlink.h"
#include "internal/checksum.h"
#include "internal/mavlink_types.h"
#include "internal/protocol.h"
#include <Arduino.h>
#include <HardwareSerial.h>

#include <vector>

/*
{
  "msg list": [
    163,AHRS
    178,AHRS2
    30,ATTITUDE
    183,AUTOPILOT_VERSION_REQUEST
    147,BATTERY_STATUS
    193,EKF_STATUS_REPORT
    162,FENCE_STATUS
    200,GIMBAL_REPORT
    214,GIMBAL_TORQUE_CMD_REPORT
    33,GLOBAL_POSITION_INT
    215,GOPRO_HEARTBEAT
    24,GPS_RAW_INT
    0,HEARTBEAT
    165,HWSTATUS
    152,MEMINFO
    42,MISSION_CURRENT
    158,MOUNT_STATUS
    62,NAV_CONTROLLER_OUTPUT
    22,PARAM_VALUE
    125,POWER_STATUS
    27,RAW_IMU
    65,RC_CHANNELS
    35,RC_CHANNELS_RAW
    66,REQUEST_DATA_STREAM
    116,SCALED_IMU2
    129,SCALED_IMU3
    29,SCALED_PRESSURE
    137,SCALED_PRESSURE2
    150,SENSOR_OFFSETS
    36,SERVO_OUTPUT_RAW
    253,STATUSTEXT
    1,SYS_STATUS
    2,SYSTEM_TIME
    111,TIMESYNC
    74,VFR_HUD
    241,VIBRATION
  ]
*/

/* In alphabetical order
{ "ACTUATOR_CONTROL_TARGET", 140 }
{ "ADAP_TUNING", 11010 }
{ "ADSB_VEHICLE", 246 }
{ "AHRS", 163 }
{ "AHRS2", 178 }
{ "AHRS3", 182 }
{ "AIRSPEED_AUTOCAL", 174 }
{ "ALTITUDE", 141 }
{ "AOA_SSA", 11020 }
{ "AP_ADC", 153 }
{ "ATTITUDE", 30 }
{ "ATTITUDE_QUATERNION", 31 }
{ "ATTITUDE_QUATERNION_COV", 61 }
{ "ATTITUDE_TARGET", 83 }
{ "ATT_POS_MOCAP", 138 }
{ "AUTH_KEY", 7 }
{ "AUTOPILOT_VERSION", 148 }
{ "AUTOPILOT_VERSION_REQUEST", 183 }
{ "BATTERY2", 181 }
{ "BATTERY_STATUS", 147 }
{ "BUTTON_CHANGE", 257 }
{ "CAMERA_CAPTURE_STATUS", 262 }
{ "CAMERA_FEEDBACK", 180 }
{ "CAMERA_IMAGE_CAPTURED", 263 }
{ "CAMERA_INFORMATION", 259 }
{ "CAMERA_SETTINGS", 260 }
{ "CAMERA_STATUS", 179 }
{ "CAMERA_TRIGGER", 112 }
{ "CHANGE_OPERATOR_CONTROL", 5 }
{ "CHANGE_OPERATOR_CONTROL_ACK", 6 }
{ "COLLISION", 247 }
{ "COMMAND_ACK", 77 }
{ "COMMAND_INT", 75 }
{ "COMMAND_LONG", 76 }
{ "COMPASSMOT_STATUS", 177 }
{ "CONTROL_SYSTEM_STATE", 146 }
{ "DATA16", 169 }
{ "DATA32", 170 }
{ "DATA64", 171 }
{ "DATA96", 172 }
{ "DATA_STREAM", 67 }
{ "DATA_TRANSMISSION_HANDSHAKE", 130 }
{ "DEBUG", 254 }
{ "DEBUG_VECT", 250 }
{ "DEEPSTALL", 195 } 
{ "DEVICE_OP_READ", 11000 }
{ "DEVICE_OP_READ_REPLY", 11001 }
{ "DEVICE_OP_WRITE", 11002 }
{ "DEVICE_OP_WRITE_REPLY", 11003 }
{ "DIGICAM_CONFIGURE", 154 }
{ "DIGICAM_CONTROL", 155 }
{ "DISTANCE_SENSOR", 132 }
{ "EKF_STATUS_REPORT", 193 }
{ "ENCAPSULATED_DATA", 131 }
{ "ESTIMATOR_STATUS", 230 }
{ "EXTENDED_SYS_STATE", 245 }
{ "FENCE_FETCH_POINT", 161 }
{ "FENCE_POINT", 160 }
{ "FENCE_STATUS", 162 }
{ "FILE_TRANSFER_PROTOCOL", 110 }
{ "FLIGHT_INFORMATION", 264 }
{ "FOLLOW_TARGET", 144 }
{ "GIMBAL_CONTROL", 201 }
{ "GIMBAL_REPORT", 200 }
{ "GIMBAL_TORQUE_CMD_REPORT", 214 }
{ "GLOBAL_POSITION_INT", 33 }
{ "GLOBAL_POSITION_INT_COV", 63 }
{ "GLOBAL_VISION_POSITION_ESTIMATE", 101 }
{ "GOPRO_GET_REQUEST", 216 }
{ "GOPRO_GET_RESPONSE", 217 }
{ "GOPRO_HEARTBEAT", 215 }
{ "GOPRO_SET_REQUEST", 218 }
{ "GOPRO_SET_RESPONSE", 219 }
{ "GPS2_RAW", 124 }
{ "GPS2_RTK", 128 }
{ "GPS_GLOBAL_ORIGIN", 49 }
{ "GPS_INJECT_DATA", 123 }
{ "GPS_INPUT", 232 }
{ "GPS_RAW_INT", 24 }
{ "GPS_RTCM_DATA", 233 }
{ "GPS_RTK", 127 }
{ "GPS_STATUS", 25 }
{ "HEARTBEAT", 0 }
{ "HIGHRES_IMU", 105 }
{ "HIGH_LATENCY", 234 }
{ "HIGH_LATENCY2", 235 }
{ "HIL_ACTUATOR_CONTROLS", 93 }
{ "HIL_CONTROLS", 91 }
{ "HIL_GPS", 113 }
{ "HIL_OPTICAL_FLOW", 114 }
{ "HIL_RC_INPUTS_RAW", 92 }
{ "HIL_SENSOR", 107 }
{ "HIL_STATE", 90 }
{ "HIL_STATE_QUATERNION", 115 }
{ "HOME_POSITION", 242 }
{ "HWSTATUS", 165 }
{ "ICAROUS_HEARTBEAT", 42000 } 
{ "ICAROUS_KINEMATIC_BANDS", 42001 }
{ "LANDING_TARGET", 149 }
{ "LED_CONTROL", 186 }
{ "LIMITS_STATUS", 167 }
{ "LOCAL_POSITION_NED", 32 }
{ "LOCAL_POSITION_NED_COV", 64 }
{ "LOCAL_POSITION_NED_SYSTEM_GLOBAL_OFFSET", 89 }
{ "LOGGING_ACK", 268 }
{ "LOGGING_DATA", 266 }
{ "LOGGING_DATA_ACKED", 267 }
{ "LOG_DATA", 120 }
{ "LOG_ENTRY", 118 }
{ "LOG_ERASE", 121 }
{ "LOG_REQUEST_DATA", 119 }
{ "LOG_REQUEST_END", 122 }
{ "LOG_REQUEST_LIST", 117 }
{ "MAG_CAL_PROGRESS", 191 }
{ "MAG_CAL_REPORT", 192 }
{ "MANUAL_CONTROL", 69 }
{ "MANUAL_SETPOINT", 81 }
{ "MEMINFO", 152 }
{ "MEMORY_VECT", 249 }
{ "MESSAGE_INTERVAL", 244 }
{ "MISSION_ACK", 47 }
{ "MISSION_CLEAR_ALL", 45 }
{ "MISSION_COUNT", 44 }
{ "MISSION_CURRENT", 42 }
{ "MISSION_ITEM", 39 }
{ "MISSION_ITEM_INT", 73 }
{ "MISSION_ITEM_REACHED", 46 }
{ "MISSION_REQUEST", 40 }
{ "MISSION_REQUEST_INT", 51 }
{ "MISSION_REQUEST_LIST", 43 }
{ "MISSION_REQUEST_PARTIAL_LIST", 37 }
{ "MISSION_SET_CURRENT", 41 }
{ "MISSION_WRITE_PARTIAL_LIST", 38 }
{ "MOUNT_CONFIGURE", 156 }
{ "MOUNT_CONTROL", 157 }
{ "MOUNT_ORIENTATION", 265 }
{ "MOUNT_STATUS", 158 }
{ "NAMED_VALUE_FLOAT", 251 }
{ "NAMED_VALUE_INT", 252 }
{ "NAV_CONTROLLER_OUTPUT", 62 }
{ "OBSTACLE_DISTANCE", 330 }
{ "ODOMETRY", 331 }
{ "OPTICAL_FLOW", 100 }
{ "OPTICAL_FLOW_RAD", 106 }
{ "PARAM_EXT_ACK", 324 }
{ "PARAM_EXT_REQUEST_LIST", 321 }
{ "PARAM_EXT_REQUEST_READ", 320 }
{ "PARAM_EXT_SET", 323 }
{ "PARAM_EXT_VALUE", 322 }
{ "PARAM_MAP_RC", 50 }
{ "PARAM_REQUEST_LIST", 21 }
{ "PARAM_REQUEST_READ", 20 }
{ "PARAM_SET", 23 }
{ "PARAM_VALUE", 22 }
{ "PID_TUNING", 194 }
{ "PING", 4 }
{ "PLAY_TUNE", 258 }
{ "POSITION_TARGET_GLOBAL_INT", 87 }
{ "POSITION_TARGET_LOCAL_NED", 85 }
{ "POWER_STATUS", 125 }
{ "PROTOCOL_VERSION", 300 }
{ "RADIO", 166 }
{ "RADIO_STATUS", 109 }
{ "RALLY_FETCH_POINT", 176 }
{ "RALLY_POINT", 175 } 
{ "RANGEFINDER", 173 }
{ "RAW_IMU", 27 }
{ "RAW_PRESSURE", 28 }
{ "RC_CHANNELS", 65 }
{ "RC_CHANNELS_OVERRIDE", 70 }
{ "RC_CHANNELS_RAW", 35 }
{ "RC_CHANNELS_SCALED", 34 }
{ "REMOTE_LOG_BLOCK_STATUS", 185 }
{ "REMOTE_LOG_DATA_BLOCK", 184 }
{ "REQUEST_DATA_STREAM", 66 }
{ "RESOURCE_REQUEST", 142 }
{ "RPM", 226 }
{ "SAFETY_ALLOWED_AREA", 55 }
{ "SAFETY_SET_ALLOWED_AREA", 54 }
{ "SCALED_IMU", 26 }
{ "SCALED_IMU2", 116 }
{ "SCALED_IMU3", 129 }
{ "SCALED_PRESSURE", 29 }
{ "SCALED_PRESSURE2", 137 }
{ "SCALED_PRESSURE3", 143 }
{ "SENSOR_OFFSETS", 150 }
{ "SERIAL_CONTROL", 126 }
{ "SERVO_OUTPUT_RAW", 36 }
{ "SETUP_SIGNING", 256 }
{ "SET_ACTUATOR_CONTROL_TARGET", 139 }
{ "SET_ATTITUDE_TARGET", 82 }
{ "SET_GPS_GLOBAL_ORIGIN", 48 }
{ "SET_HOME_POSITION", 243 }
{ "SET_MAG_OFFSETS", 151 }
{ "SET_MODE", 11 }
{ "SET_POSITION_TARGET_GLOBAL_INT", 86 }
{ "SET_POSITION_TARGET_LOCAL_NED", 84 }
{ "SET_VIDEO_STREAM_SETTINGS", 270 }
{ "SIMSTATE", 164 }
{ "SIM_STATE", 108 }
{ "STATUSTEXT", 253 }
{ "STORAGE_INFORMATION", 261 }
{ "SYSTEM_TIME", 2 }
{ "SYS_STATUS", 1 }
{ "TERRAIN_CHECK", 135 }
{ "TERRAIN_DATA", 134 }
{ "TERRAIN_REPORT", 136 }
{ "TERRAIN_REQUEST", 133 }
{ "TIMESYNC", 111 }
{ "TRAJECTORY", 332 }
{ "UAVCAN_NODE_INFO", 311 }
{ "UAVCAN_NODE_STATUS", 310 }
{ "UAVIONIX_ADSB_OUT_CFG", 10001 }
{ "UAVIONIX_ADSB_OUT_DYNAMIC", 10002 }
{ "UAVIONIX_ADSB_TRANSCEIVER_HEALTH_REPORT", 10003 }
{ "V2_EXTENSION", 248 }
{ "VFR_HUD", 74 }
{ "VIBRATION", 241 }
{ "VICON_POSITION_ESTIMATE", 104 }
{ "VIDEO_STREAM_INFORMATION", 269 }
{ "VISION_POSITION_DELTA", 11011 }
{ "VISION_POSITION_ESTIMATE", 102 }
{ "VISION_SPEED_ESTIMATE", 103 }
{ "WIFI_CONFIG_AP", 299 }
{ "WIND", 168 }
{ "WIND_COV", 231 }
*/








DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ACTUATOR_CONTROL_TARGET__CTR) "ACTUATOR_CONTROL_TARGET";
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ADAP_TUNING__CTR) "ADAP_TUNING"; // 11010 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ADSB_VEHICLE__CTR) "ADSB_VEHICLE"; // 246 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__AHRS__CTR) "AHRS"; // 163 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__AHRS2__CTR) "AHRS2"; // 178 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__AHRS3__CTR) "AHRS3"; // 182 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__AIRSPEED_AUTOCAL__CTR) "AIRSPEED_AUTOCAL"; // 174 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ALTITUDE__CTR) "ALTITUDE"; // 141 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__AOA_SSA__CTR) "AOA_SSA"; // 11020 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__AP_ADC__CTR) "AP_ADC"; // 153 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE__CTR) "ATTITUDE"; // 30 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE_QUATERNION__CTR) "ATTITUDE_QUATERNION"; // 31 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE_QUATERNION_COV__CTR) "ATTITUDE_QUATERNION_COV"; // 61 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ATTITUDE_TARGET__CTR) "ATTITUDE_TARGET"; // 83 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ATT_POS_MOCAP__CTR) "ATT_POS_MOCAP"; // 138 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__AUTH_KEY__CTR) "AUTH_KEY"; // 7 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__AUTOPILOT_VERSION__CTR) "AUTOPILOT_VERSION"; // 148 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__AUTOPILOT_VERSION_REQUEST__CTR) "AUTOPILOT_VERSION_REQUEST"; // 183 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__BATTERY2__CTR) "xxxx"; // 181 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__BATTERY_STATUS__CTR) "BATTERY_STATUS"; // 147 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__BUTTON_CHANGE__CTR) "xxxx"; // 257 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_CAPTURE_STATUS__CTR) "xxxx"; // 262 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_FEEDBACK__CTR) "xxxx"; // 180 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_IMAGE_CAPTURED__CTR) "xxxx"; // 263 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_INFORMATION__CTR) "xxxx"; // 259 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_SETTINGS__CTR) "xxxx"; // 260 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_STATUS__CTR) "xxxx"; // 179 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__CAMERA_TRIGGER__CTR) "xxxx"; // 112 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__CHANGE_OPERATOR_CONTROL__CTR) "xxxx"; // 5 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__CHANGE_OPERATOR_CONTROL_ACK__CTR) "xxxx"; // 6 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__COLLISION__CTR) "xxxx"; // 247 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__COMMAND_ACK__CTR) "xxxx"; // 77 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__COMMAND_INT__CTR) "xxxx"; // 75 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__COMMAND_LONG__CTR) "xxxx"; // 76 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__COMPASSMOT_STATUS__CTR) "xxxx"; // 177 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__CONTROL_SYSTEM_STATE__CTR) "xxxx"; // 146 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DATA16__CTR) "xxxx"; // 169 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DATA32__CTR) "xxxx"; // 170 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DATA64__CTR) "xxxx"; // 171 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DATA96__CTR) "xxxx"; // 172 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DATA_STREAM__CTR) "xxxx"; // 67 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DATA_TRANSMISSION_HANDSHAKE__CTR) "xxxx"; // 130 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DEBUG__CTR) "xxxx"; // 254 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DEBUG_VECT__CTR) "xxxx"; // 250 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DEEPSTALL__CTR) "xxxx"; // 195  
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_READ__CTR) "xxxx"; // 11000 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_READ_REPLY__CTR) "xxxx"; // 11001 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_WRITE__CTR) "xxxx"; // 11002 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DEVICE_OP_WRITE_REPLY__CTR) "xxxx"; // 11003 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DIGICAM_CONFIGURE__CTR) "xxxx"; // 154 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DIGICAM_CONTROL__CTR) "xxxx"; // 155 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__DISTANCE_SENSOR__CTR) "xxxx"; // 132 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__EKF_STATUS_REPORT__CTR) "EKF_STATUS_REPORT"; // 193 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ENCAPSULATED_DATA__CTR) "xxxx"; // 131 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ESTIMATOR_STATUS__CTR) "xxxx"; // 230 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__EXTENDED_SYS_STATE__CTR) "xxxx"; // 245 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__FENCE_FETCH_POINT__CTR) "xxxx"; // 161 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__FENCE_POINT__CTR) "xxxx"; // 160 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__FENCE_STATUS__CTR) "FENCE_STATUS"; // 162 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__FILE_TRANSFER_PROTOCOL__CTR) "xxxx"; // 110 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__FLIGHT_INFORMATION__CTR) "xxxx"; // 264 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__FOLLOW_TARGET__CTR) "xxxx"; // 144 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_CONTROL__CTR) "xxxx"; // 201 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_REPORT__CTR) "GIMBAL_REPORT"; // 200 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GIMBAL_TORQUE_CMD_REPORT__CTR) "GIMBAL_TORQUE_CMD_REPORT"; // 214 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_POSITION_INT__CTR) "GLOBAL_POSITION_INT"; // 33 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_POSITION_INT_COV__CTR) "xxxx"; // 63 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GLOBAL_VISION_POSITION_ESTIMATE__CTR) "xxxx"; // 101 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GOPRO_GET_REQUEST__CTR) "xxxx"; // 216 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GOPRO_GET_RESPONSE__CTR) "xxxx"; // 217 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GOPRO_HEARTBEAT__CTR) "GOPRO_HEARTBEAT"; // 215 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GOPRO_SET_REQUEST__CTR) "xxxx"; // 218 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GOPRO_SET_RESPONSE__CTR) "xxxx"; // 219 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GPS2_RAW__CTR) "xxxx"; // 124 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GPS2_RTK__CTR) "xxxx"; // 128 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GPS_GLOBAL_ORIGIN__CTR) "xxxx"; // 49 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GPS_INJECT_DATA__CTR) "xxxx"; // 123 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GPS_INPUT__CTR) "xxxx"; // 232 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GPS_RAW_INT__CTR) "GPS_RAW_INT"; // 24 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GPS_RTCM_DATA__CTR) "xxxx"; // 233 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GPS_RTK__CTR) "xxxx"; // 127 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__GPS_STATUS__CTR) "xxxx"; // 25 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HEARTBEAT__CTR) "HEARTBEAT"; // 0 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HIGHRES_IMU__CTR) "xxxx"; // 105 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HIGH_LATENCY__CTR) "xxxx"; // 234 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HIGH_LATENCY2__CTR) "xxxx"; // 235 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HIL_ACTUATOR_CONTROLS__CTR) "xxxx"; // 93 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HIL_CONTROLS__CTR) "xxxx"; // 91 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HIL_GPS__CTR) "xxxx"; // 113 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HIL_OPTICAL_FLOW__CTR) "xxxx"; // 114 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HIL_RC_INPUTS_RAW__CTR) "xxxx"; // 92 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HIL_SENSOR__CTR) "xxxx"; // 107 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HIL_STATE__CTR) "xxxx"; // 90 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HIL_STATE_QUATERNION__CTR) "xxxx"; // 115 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HOME_POSITION__CTR) "xxxx"; // 242 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__HWSTATUS__CTR) "HWSTATUS"; // 165 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ICAROUS_HEARTBEAT__CTR) "xxxx"; // 42000  
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ICAROUS_KINEMATIC_BANDS__CTR) "xxxx"; // 42001 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LANDING_TARGET__CTR) "xxxx"; // 149 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LED_CONTROL__CTR) "xxxx"; // 186 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LIMITS_STATUS__CTR) "xxxx"; // 167 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOCAL_POSITION_NED__CTR) "xxxx"; // 32 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOCAL_POSITION_NED_COV__CTR) "xxxx"; // 64 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOCAL_POSITION_NED_SYSTEM_GLOBAL_OFFSET__CTR) "xxxx"; // 89 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOGGING_ACK__CTR) "xxxx"; // 268 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOGGING_DATA__CTR) "xxxx"; // 266 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOGGING_DATA_ACKED__CTR) "xxxx"; // 267 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOG_DATA__CTR) "xxxx"; // 120 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOG_ENTRY__CTR) "xxxx"; // 118 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOG_ERASE__CTR) "xxxx"; // 121 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOG_REQUEST_DATA__CTR) "xxxx"; // 119 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOG_REQUEST_END__CTR) "xxxx"; // 122 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__LOG_REQUEST_LIST__CTR) "xxxx"; // 117 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MAG_CAL_PROGRESS__CTR) "xxxx"; // 191 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MAG_CAL_REPORT__CTR) "xxxx"; // 192 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MANUAL_CONTROL__CTR) "xxxx"; // 69 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MANUAL_SETPOINT__CTR) "xxxx"; // 81 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MEMINFO__CTR) "MEMINFO"; // 152 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MEMORY_VECT__CTR) "xxxx"; // 249 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MESSAGE_INTERVAL__CTR) "xxxx"; // 244 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_ACK__CTR) "xxxx"; // 47 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_CLEAR_ALL__CTR) "xxxx"; // 45 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_COUNT__CTR) "xxxx"; // 44 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_CURRENT__CTR) "MISSION_CURRENT"; // 42 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_ITEM__CTR) "xxxx"; // 39 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_ITEM_INT__CTR) "xxxx"; // 73 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_ITEM_REACHED__CTR) "xxxx"; // 46 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST__CTR) "xxxx"; // 40 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST_INT__CTR) "xxxx"; // 51 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST_LIST__CTR) "xxxx"; // 43 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_REQUEST_PARTIAL_LIST__CTR) "xxxx"; // 37 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_SET_CURRENT__CTR) "xxxx"; // 41 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MISSION_WRITE_PARTIAL_LIST__CTR) "xxxx"; // 38 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MOUNT_CONFIGURE__CTR) "xxxx"; // 156 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MOUNT_CONTROL__CTR) "xxxx"; // 157 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MOUNT_ORIENTATION__CTR) "xxxx"; // 265 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__MOUNT_STATUS__CTR) "MOUNT_STATUS"; // 158 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__NAMED_VALUE_FLOAT__CTR) "xxxx"; // 251 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__NAMED_VALUE_INT__CTR) "xxxx"; // 252 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__NAV_CONTROLLER_OUTPUT__CTR) "NAV_CONTROLLER_OUTPUT"; // 62 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__OBSTACLE_DISTANCE__CTR) "xxxx"; // 330 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__ODOMETRY__CTR) "xxxx"; // 331 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__OPTICAL_FLOW__CTR) "xxxx"; // 100 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__OPTICAL_FLOW_RAD__CTR) "xxxx"; // 106 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_ACK__CTR) "xxxx"; // 324 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_REQUEST_LIST__CTR) "xxxx"; // 321 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_REQUEST_READ__CTR) "xxxx"; // 320 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_SET__CTR) "xxxx"; // 323 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PARAM_EXT_VALUE__CTR) "xxxx"; // 322 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PARAM_MAP_RC__CTR) "xxxx"; // 50 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PARAM_REQUEST_LIST__CTR) "xxxx"; // 21 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PARAM_REQUEST_READ__CTR) "xxxx"; // 20 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PARAM_SET__CTR) "xxxx"; // 23 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PARAM_VALUE__CTR) "PARAM_VALUE"; // 22 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PID_TUNING__CTR) "xxxx"; // 194 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PING__CTR) "xxxx"; // 4 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PLAY_TUNE__CTR) "xxxx"; // 258 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__POSITION_TARGET_GLOBAL_INT__CTR) "xxxx"; // 87 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__POSITION_TARGET_LOCAL_NED__CTR) "xxxx"; // 85 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__POWER_STATUS__CTR) "POWER_STATUS"; // 125 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__PROTOCOL_VERSION__CTR) "xxxx"; // 300 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RADIO__CTR) "xxxx"; // 166 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RADIO_STATUS__CTR) "xxxx"; // 109 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RALLY_FETCH_POINT__CTR) "xxxx"; // 176 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RALLY_POINT__CTR) "xxxx"; // 175  
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RANGEFINDER__CTR) "xxxx"; // 173 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RAW_IMU__CTR) "RAW_IMU"; // 27 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RAW_PRESSURE__CTR) "xxxx"; // 28 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS__CTR) "RC_CHANNELS"; // 65 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_OVERRIDE__CTR) "xxxx"; // 70 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_RAW__CTR) "RC_CHANNELS_RAW"; // 35 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RC_CHANNELS_SCALED__CTR) "xxxx"; // 34 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__REMOTE_LOG_BLOCK_STATUS__CTR) "xxxx"; // 185 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__REMOTE_LOG_DATA_BLOCK__CTR) "xxxx"; // 184 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__REQUEST_DATA_STREAM__CTR) "REQUEST_DATA_STREAM"; // 66 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RESOURCE_REQUEST__CTR) "xxxx"; // 142 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__RPM__CTR) "xxxx"; // 226 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SAFETY_ALLOWED_AREA__CTR) "xxxx"; // 55 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SAFETY_SET_ALLOWED_AREA__CTR) "xxxx"; // 54 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU__CTR) "SCALED_IMU"; // 26 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU2__CTR) "SCALED_IMU2"; // 116 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SCALED_IMU3__CTR) "SCALED_IMU3"; // 129 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE__CTR) "SCALED_PRESSURE"; // 29 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE2__CTR) "SCALED_PRESSURE2"; // 137 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SCALED_PRESSURE3__CTR) "SCALED_PRESSURE3"; // 143 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SENSOR_OFFSETS__CTR) "SENSOR_OFFSETS"; // 150 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SERIAL_CONTROL__CTR) "xxxx"; // 126 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SERVO_OUTPUT_RAW__CTR) "SERVO_OUTPUT_RAW"; // 36 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SETUP_SIGNING__CTR) "xxxx"; // 256 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SET_ACTUATOR_CONTROL_TARGET__CTR) "xxxx"; // 139 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SET_ATTITUDE_TARGET__CTR) "xxxx"; // 82 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SET_GPS_GLOBAL_ORIGIN__CTR) "xxxx"; // 48 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SET_HOME_POSITION__CTR) "xxxx"; // 243 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SET_MAG_OFFSETS__CTR) "xxxx"; // 151 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SET_MODE__CTR) "xxxx"; // 11 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SET_POSITION_TARGET_GLOBAL_INT__CTR) "xxxx"; // 86 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SET_POSITION_TARGET_LOCAL_NED__CTR) "xxxx"; // 84 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SET_VIDEO_STREAM_SETTINGS__CTR) "xxxx"; // 270 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SIMSTATE__CTR) "xxxx"; // 164 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SIM_STATE__CTR) "xxxx"; // 108 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__STATUSTEXT__CTR) "STATUSTEXT"; // 253 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__STORAGE_INFORMATION__CTR) "xxxx"; // 261 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SYSTEM_TIME__CTR) "SYSTEM_TIME"; // 2 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__SYS_STATUS__CTR) "SYS_STATUS"; // 1 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_CHECK__CTR) "xxxx"; // 135 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_DATA__CTR) "xxxx"; // 134 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_REPORT__CTR) "xxxx"; // 136 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__TERRAIN_REQUEST__CTR) "xxxx"; // 133 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__TIMESYNC__CTR) "TIMESYNC"; // 111 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__TRAJECTORY__CTR) "xxxx"; // 332 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__UAVCAN_NODE_INFO__CTR) "xxxx"; // 311 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__UAVCAN_NODE_STATUS__CTR) "xxxx"; // 310 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__UAVIONIX_ADSB_OUT_CFG__CTR) "UAVIONIX_ADSB_OUT_CFG"; // 10001 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__UAVIONIX_ADSB_OUT_DYNAMIC__CTR) "UAVIONIX_ADSB_OUT_DYNAMIC"; // 10002 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__UAVIONIX_ADSB_TRANSCEIVER_HEALTH_REPORT__CTR) "UAVIONIX_ADSB_TRANSCEIVER_HEALTH_REPORT"; // 10003 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__V2_EXTENSION__CTR) "V2_EXTENSION"; // 248 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__VFR_HUD__CTR) "VFR_HUD"; // 74 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__VIBRATION__CTR) "VIBRATION"; // 241 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__VICON_POSITION_ESTIMATE__CTR) "VICON_POSITION_ESTIMATE"; // 104 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__VIDEO_STREAM_INFORMATION__CTR) "VIDEO_STREAM_INFORMATION"; // 269 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__VISION_POSITION_DELTA__CTR) "VISION_POSITION_DELTA"; // 11011 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__VISION_POSITION_ESTIMATE__CTR) "VISION_POSITION_ESTIMATE"; // 102 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__VISION_SPEED_ESTIMATE__CTR) "VISION_SPEED_ESTIMATE"; // 103 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__WIFI_CONFIG_AP__CTR) "WIFI_CONFIG_AP"; // 299 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__WIND__CTR) "WIND"; // 168 
DEFINE_PGM_CTR(PM_MAVLINK_MSG_PACKET_NAME__WIND_COV__CTR) "WIND_COV"; // 231 



class mMAVLink :
  public mTaskerInterface
{
  public:
	  mMAVLink(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE__DRIVERS_MAVLINK__CTR;
    static const char* PM_MODULE__DRIVERS_MAVLINK__FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE__DRIVERS_MAVLINK__CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE__DRIVERS_MAVLINK__FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__DRIVERS_MAVLINK__ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mMAVLink);
    };
    #endif

    std::vector<uint16_t> unique_msg_id_list;





    char buffer[100] = {0};
    const char* MavLink_Msg_FriendlyName_By_ID(uint16_t id, char* buffer, uint8_t buflen);

    HardwareSerial* hs; // &hs = Serial2;
    // PixhawkArduinoMAVLink* mav; //(hs);

    struct DATA{
      mavlink_ahrs_t                      ahrs = {0};       
      mavlink_ahrs2_t                     ahrs2 = {0};    
      mavlink_attitude_t                  attitude = {0};    
      mavlink_autopilot_version_t         autopilot_version = {0};
      mavlink_battery_status_t            battery_status = {0};                  
      mavlink_ekf_status_report_t         ekf_status_report = {0};                
      mavlink_fence_status_t              fence_status = {0};                     
      mavlink_gimbal_report_t             gimbal_report = {0};                        
      mavlink_gimbal_torque_cmd_report_t  gimbal_torque_cmd_report = {0};
      mavlink_global_position_int_t       global_position_int = {0};  
      mavlink_gopro_heartbeat_t           gopro_heartbeat = {0};                
      mavlink_gps_raw_int_t               gps_raw_int = {0};                 
      mavlink_heartbeat_t                 heartbeat = {0};                   
      mavlink_hwstatus_t                  hwstatus = {0};                   
      mavlink_meminfo_t                   meminfo = {0};                            
      mavlink_mission_current_t           mission_current = {0};                 
      mavlink_mount_status_t              mount_status = {0};                          
      mavlink_nav_controller_output_t     nav_controller_output = {0};        
      mavlink_param_value_t               param_value = {0};                     
      mavlink_power_status_t              power_status = {0};               
      mavlink_raw_imu_t                   raw_imu = {0};                        
      mavlink_rc_channels_t               rc_channels = {0};                        
      mavlink_rc_channels_raw_t           rc_channels_raw = {0};                    
      mavlink_request_data_stream_t       request_data_stream = {0};            
      mavlink_scaled_imu2_t               scaled_imu2 = {0};                    
      mavlink_scaled_imu3_t               scaled_imu3 = {0};                        
      mavlink_scaled_pressure_t           scaled_pressure = {0};                     
      mavlink_scaled_pressure2_t          scaled_pressure2 = {0};                  
      mavlink_sensor_offsets_t            sensor_offsets = {0};                       
      mavlink_servo_output_raw_t          servo_output_raw = {0};               
      mavlink_statustext_t                statustext = {0};                    
      mavlink_sys_status_t                sys_status = {0};                     
      mavlink_system_time_t               system_time = {0};                 
      mavlink_timesync_t                  timesync = {0};                   
      mavlink_vfr_hud_t                   vfr_hud = {0};                      
      mavlink_vibration_t                 vibration = {0};                                
    }pkt;


    HardwareSerial* _MAVSerial;
    
    double MILLIG_TO_MS2;
    uint8_t system_id;
    uint8_t component_id;
    uint8_t type;
    uint8_t autopilot;
    uint8_t received_sysid; // Pixhawk sysid
    uint8_t received_compid; // Pixhawk compid

    
    bool begin();
    void StreamF();
    void PollMAVLink_Stream();


    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();    

    
    void parse_JSONCommand(JsonParserObject obj);
  

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
    

      struct handler<mMAVLink> mqtthandler_mavlink_packet__ahrs;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__ahrs2;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__attitude;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__autopilot_version_request;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__battery_status;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__ekf_status_report;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__fence_status;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__gimbal_report;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__gimbal_torque_cmd_report;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__global_position_int;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__gopro_heartbeat;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__gps_raw_int;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__heartbeat;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__hwstatus;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__meminfo;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__mission_current;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__mount_status;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__nav_controller_output;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__param_value;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__power_status;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__raw_imu;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__rc_channels;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__rc_channels_raw;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__request_data_stream;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__scaled_imu2;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__scaled_imu3;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__scaled_pressure;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__scaled_pressure2;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__sensor_offsets;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__servo_output_raw;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__statustext;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__sys_status;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__system_time;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__timesync;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__vfr_hud;
      struct handler<mMAVLink> mqtthandler_mavlink_packet__vibration;


      
      uint8_t ConstructJSON_ahrs(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_ahrs2(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_attitude(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_autopilot_version_request(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_battery_status(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_ekf_status_report(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_fence_status(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_gimbal_report(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_gimbal_torque_cmd_report(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_global_position_int(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_gopro_heartbeat(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_gps_raw_int(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_heartbeat(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_hwstatus(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_meminfo(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_mission_current(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_mount_status(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_nav_controller_output(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_param_value(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_power_status(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_raw_imu(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_rc_channels(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_rc_channels_raw(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_request_data_stream(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_imu2(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_imu3(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_scaled_pressure(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_scaled_pressure2(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_sensor_offsets(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_servo_output_raw(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_statustext(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_sys_status(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_system_time(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_timesync(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_vfr_hud(uint8_t json_level = 0, bool json_appending = true);
      uint8_t ConstructJSON_vibration(uint8_t json_level = 0, bool json_appending = true);



    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mMAVLink> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mMAVLink> mqtthandler_sensor_ifchanged;
    struct handler<mMAVLink> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    struct handler<mMAVLink>* mqtthandler_list[39] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod,
      &mqtthandler_mavlink_packet__ahrs,
      &mqtthandler_mavlink_packet__ahrs2,
      &mqtthandler_mavlink_packet__attitude,
      &mqtthandler_mavlink_packet__autopilot_version_request,
      &mqtthandler_mavlink_packet__battery_status,
      &mqtthandler_mavlink_packet__ekf_status_report,
      &mqtthandler_mavlink_packet__fence_status,
      &mqtthandler_mavlink_packet__gimbal_report,
      &mqtthandler_mavlink_packet__gimbal_torque_cmd_report,
      &mqtthandler_mavlink_packet__global_position_int,
      &mqtthandler_mavlink_packet__gopro_heartbeat,
      &mqtthandler_mavlink_packet__gps_raw_int,
      &mqtthandler_mavlink_packet__heartbeat,
      &mqtthandler_mavlink_packet__hwstatus,
      &mqtthandler_mavlink_packet__meminfo,
      &mqtthandler_mavlink_packet__mission_current,
      &mqtthandler_mavlink_packet__mount_status,
      &mqtthandler_mavlink_packet__nav_controller_output,
      &mqtthandler_mavlink_packet__param_value,
      &mqtthandler_mavlink_packet__power_status,
      &mqtthandler_mavlink_packet__raw_imu,
      &mqtthandler_mavlink_packet__rc_channels,
      &mqtthandler_mavlink_packet__rc_channels_raw,
      &mqtthandler_mavlink_packet__request_data_stream,
      &mqtthandler_mavlink_packet__scaled_imu2,
      &mqtthandler_mavlink_packet__scaled_imu3,
      &mqtthandler_mavlink_packet__scaled_pressure,
      &mqtthandler_mavlink_packet__scaled_pressure2,
      &mqtthandler_mavlink_packet__sensor_offsets,
      &mqtthandler_mavlink_packet__servo_output_raw,
      &mqtthandler_mavlink_packet__statustext,
      &mqtthandler_mavlink_packet__sys_status,
      &mqtthandler_mavlink_packet__system_time,
      &mqtthandler_mavlink_packet__timesync,
      &mqtthandler_mavlink_packet__vfr_hud,
      &mqtthandler_mavlink_packet__vibration
    };


    //No extra handlers example
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    //with extra handlers example
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
