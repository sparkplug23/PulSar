/*
  mLanguageDefault.h - Base language defines

  Copyright (C) 2020 my_name_here

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef mLanguageDefault_H
#define mLanguageDefault_H

#include "2_CoreSystem/mGlobalMacros.h"

#define STR_HELPER2(x)  #x
#ifndef STR2
#define STR2(x)  STR_HELPER2(x)
#endif

#ifdef ENABLE_LANGUAGE_DEFAULT_EXTENDED
#include "2_CoreSystem/11_Languages/mLanguageDefault_Extended.h"
#endif

// Module names
#define D_MODULE_CORE_HARDWAREPINS_CTR            "mHardwarePins"
#define D_MODULE_CORE_HARDWAREPINS_FRIENDLY_CTR   "hardwarepins"
#define D_MODULE_CORE_SETTINGS_CTR                "mSettings"
#define D_MODULE_CORE_SETTINGS_FRIENDLY_CTR       "settings"
#define D_MODULE_CORE_SUPPORT_CTR                  "mSupport"
#define D_MODULE_CORE_SUPPORT_FRIENDLY_CTR         "support"
#define D_MODULE_CORE_LOGGING_CTR                 "mLogging"
#define D_MODULE_CORE_LOGGING_FRIENDLY_CTR        "logging"
#define D_MODULE_CORE_TELEMETRY_CTR                "mTelemetry"
#define D_MODULE_CORE_TELEMETRY_FRIENDLY_CTR       "telemetry"
#define D_MODULE_CORE_TIME_CTR                     "mTime"
#define D_MODULE_CORE_TIME_FRIENDLY_CTR            "time"

#define D_MODULE_CORE_DEVELOPMENT_DEBUGGING_CTR                     "mDevelopmentDebugging"
#define D_MODULE_CORE_DEVELOPMENT_DEBUGGING_FRIENDLY_CTR            "debug_development"

#define D_MODULE_CORE_RULES_CTR            "Rules"
#define D_MODULE_CORE_RULES_FRIENDLY_CTR   "rules"

#define D_MODULE_CORE_UPDATES_CTR            "Updates"
#define D_MODULE_CORE_UPDATES_FRIENDLY_CTR   "update"

#define D_MODULE_SUBSYSTEM_SOLAR_LUNAR_CTR            "solarlunar"
#define D_MODULE_SUBSYSTEM_SOLAR_LUNAR_FRIENDLY_CTR   "solarlunar"


  
#define D_MODULE_NETWORK_WIFI_CTR              "mWiFi"
#define D_MODULE_NETWORK_WIFI_FRIENDLY_CTR              "wifi"
#define D_MODULE_NETWORK_MQTT_CTR              "mMQTT"
#define D_MODULE_NETWORK_MQTT_FRIENDLY_CTR              "system"
//#ifdef USE_MODULE_NETWORK_WEBSERVER
  #define D_MODULE_NETWORK_WEBSERVER_CTR              "mWebServer"
  #define D_MODULE_NETWORK_WEBSERVER_FRIENDLY_CTR              "webserver"
//#endif


// Displays (30-39
//#ifdef USE_MODULE_DISPLAYS_INTERFACE
  #define D_MODULE_DISPLAYS_INTERFACE_CTR              "displays_interface"
  #define D_MODULE_DISPLAYS_INTERFACE_FRIENDLY_CTR              "displays_interface"
//#endif
//#ifdef USE_MODULE_DISPLAYS_NEXTION
  #define D_MODULE_DISPLAYS_NEXTION_CTR              "mNextionPanel"
  #define D_MODULE_DISPLAYS_NEXTION_FRIENDLY_CTR              "nextion"
//#endif

  #define D_MODULE_DISPLAYS_OLED_SSD1306_CTR      "display_1306"
  #define D_MODULE_DISPLAYS_OLED_SSD1306_FRIENDLY_CTR              "display_1306"


  #define D_MODULE_DISPLAYS_OLED_SH1106_CTR      "display_sh1106"
  #define D_MODULE_DISPLAYS_OLED_SH1106_FRIENDLY_CTR              "display_sh1106"


// Drivers (Range 40-129
//#ifdef USE_MODULE_DRIVERS_INTERFACE
  // #define D_MODULE_DRIVERS_HBRIDGE_CTR              "mHBridge"
  // #define D_MODULE_DRIVERS_HBRIDGE_FRIENDLY_CTR              "hbridge"
//#endif
//#ifdef USE_MODULE_DRIVERS_HBRIDGE
  #define D_MODULE_DRIVERS_HBRIDGE_CTR              "mHBridge"
  #define D_MODULE_DRIVERS_HBRIDGE_FRIENDLY_CTR              "hbridge"
//#endif
//#ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
  #define D_MODULE_DRIVERS_IRTRANSCEIVER_CTR              "mIRtransceiver"
  #define D_MODULE_DRIVERS_IRTRANSCEIVER_FRIENDLY_CTR              "ir"
//#endif
//#ifdef USE_MODULE_DRIVERS_RELAY
  #define D_MODULE_DRIVERS_RELAY_CTR           "relays"
  #define D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR  "relays"
//#endif
//#ifdef USE_MODULE_DRIVERS_PWM
  #define D_MODULE_DRIVERS_PWM_CTR           "pwm"
  #define D_MODULE_DRIVERS_PWM_FRIENDLY_CTR  "pwm"
//#endif
  #define D_MODULE_DRIVERS_SDCARD_CTR           "sdcard"
  #define D_MODULE_DRIVERS_SDCARD_FRIENDLY_CTR  "sdcard"

  #define D_MODULE_DRIVERS_GPS_CTR           "GPS"
  #define D_MODULE_DRIVERS_GPS_FRIENDLY_CTR  "gps"

  #define D_MODULE_DRIVERS_SERIAL_UART_CTR           "serialuart"
  #define D_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR  "serialuart"

  #define D_MODULE_DRIVERS_SHELLY_DIMMER_CTR "shellydimmer"
  #define D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "shellydimmer"

  #define D_MODULE_DRIVERS_CAMERA_OV2640_CTR "Camera"
  #define D_MODULE_DRIVERS_CAMERA_OV2640_FRIENDLY_CTR "camera"
  
  #define D_MODULE_DRIVERS_CAMERA_WEBCAM_CTR "Camera"
  #define D_MODULE_DRIVERS_CAMERA_WEBCAM_FRIENDLY_CTR "camera"

  #define D_MODULE_DRIVERS_STATUS_LEDS_CTR              "mSTATUS_LEDS"
  #define D_MODULE_DRIVERS_STATUS_LEDS_FRIENDLY_CTR              "statusleds"

  #define D_MODULE_DRIVERS_FILESYSTEM_CTR "mFileSystem"
  #define D_MODULE_DRIVERS_FILESYSTEM_FRIENDLY_CTR "filesystem"

  #define D_MODULE_DRIVERS_BUZZER_CTR "mBuzzer"
  #define D_MODULE_DRIVERS_BUZZER_FRIENDLY_CTR "buzzer"
  
  #define D_MODULE_DRIVERS_RF433_RCSWITCH_CTR "rcswitch"
  #define D_MODULE_DRIVERS_RF433_RCSWITCH_FRIENDLY_CTR "rcswitch"

  #define D_MODULE_DRIVERS_LEDS_CTR "leds"
  #define D_MODULE_DRIVERS_LEDS_FRIENDLY_CTR "leds"


  #define D_MODULE_DRIVERS_IRREMOTE_CTR "irremote"
  #define D_MODULE_DRIVERS_IRREMOTE_FRIENDLY_CTR "irremote"

  #define D_MODULE_DRIVERS_FONA_CELLULAR_CTR "mFona_Cellular"
  #define D_MODULE_DRIVERS_FONA_CELLULAR_FRIENDLY_CTR "fona"

  #define D_MODULE_DRIVERS__CELLULAR_SIM7000__CTR          "CellularSIM7000"
  #define D_MODULE_DRIVERS__CELLULAR_SIM7000__FRIENDLY_CTR "CellularSIM7000"

//#ifdef USE_MODULE_DRIVERS_RF433MHZ
  // DEFINE_PGM_CTR(MSAW_MODULE_CTR              "mSAWMain"
  // DEFINE_PGM_CTR(MODULE_CORE_SETTINGS_FRIENDLY_CTR              "rcswitch"
//#endif
// Energy (Range 130-139
//#ifdef USE_MODULE_ENERGY_INTERFACE
  #define D_MODULE_ENERGY_INTERFACE_CTR              "mEnergyInterface"
  #define D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR     "energy_interface"

  #define D_MODULE_ENERGY_ADE7953_CTR              "ADE7953"
  #define D_MODULE_ENERGY_ADE7953_FRIENDLY_CTR     "ade7953"
//#endif
// Lights (Range 140-169
//#ifdef USE_MODULE_LIGHTS_INTERFACE
  #define D_MODULE_LIGHTS_INTERFACE_CTR              "mInterfaceLight"
  #define D_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR              "lightinterface"
//#endif
//#ifdef USE_MODULE_LIGHTS_ANIMATOR
  #define D_MODULE_LIGHTS_ADDRESSABLE_CTR              "mRGBAnimator"
  #define D_MODULE_LIGHTS_ADDRESSABLE_FRIENDLY_CTR              "addressable"

  #define D_MODULE_LIGHTS_ANIMATOR_CTR              "ANIMATOR"
  #define D_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR              "pixels"
//#endif
//#ifdef USE_MODULE_LIGHTS_PWMCONTROLLED

#define D_MODULE_LIGHTS_PWM_FRIENDLY_CTR "pwmlight"
#define D_MODULE_LIGHTS_PWM_CTR "Pwmlight"

#define D_MODULE_LIGHTS_WLED_EFFECTS_FRIENDLY_CTR "wled_effects"
#define D_MODULE_LIGHTS_WLED_EFFECTS_CTR "wled_effects"

#define D_MODULE_LIGHTS_WLED_FRIENDLY_CTR "wled_effects"
#define D_MODULE_LIGHTS_WLED_CTR "wled_effects"




  #define D_MODULE_LIGHTS_INTERFACE_CTR              "mInterfaceLight"
  #define D_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR              "lights_interface"
//#endif
// Sensors (Range 120-169

  #define D_MODULE_SENSORS_INTERFACE_CTR              "mSensorsInterface"
  #define D_MODULE_SENSORS_INTERFACE_FRIENDLY_CTR              "sensors_interface"
//#ifdef USE_MODULE_SENSORS_BUTTONS
  #define D_MODULE_SENSORS_BUTTONS_CTR              "mButtons"
  #define D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR              "buttons"
//#endif
//#ifdef USE_MODULE_SENSORS_SWITCHES
  #define D_MODULE_SENSORS_SWITCHES_CTR              "mSwitches"
  #define D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR              "switches"

  #define D_MODULE_SENSORS_LDR_BASIC_CTR              "mLDRBasic"
  #define D_MODULE_SENSORS_LDR_BASIC_FRIENDLY_CTR              "ldr_basic"
//#endif
//#ifdef USE_MODULE_SENSORS_SWITCHES
  #define D_MODULE_SENSORS_ANALOG_CTR              "mSensorsAnalog"
  #define D_MODULE_SENSORS_ANALOG_FRIENDLY_CTR              "analog"
//#endif
//#ifdef USE_MODULE_CONTROLLER_DOORCHIME
  #define D_MODULE_SENSORS_DOORBELL_CTR              "mDoorBell"
  #define D_MODULE_SENSORS_DOORBELL_FRIENDLY_CTR              "doorbell"
//#endif
//#ifdef USE_MODULE_ENERGY_PZEM004T_V3
  #define D_MODULE_ENERGY_PZEM004T_CTR              "pzem004t"
  #define D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR              "pzem004t"
//#endif
//#ifdef USE_MODULE_SENSORS_DHT
  #define D_MODULE_SENSORS_DHT_CTR               "mSensorsDHT"
  #define D_MODULE_SENSORS_DHT_FRIENDLY_CTR      "dht"
//#endif
//#ifdef USE_MODULE_SENSORS_BME
  #define D_MODULE_SENSORS_BME_CTR              "mBME"
  #define D_MODULE_SENSORS_BME_FRIENDLY_CTR     "bme"
//#endif
//#ifdef USE_MODULE_SENSORS_DS18X
  #define D_MODULE_SENSORS_DB18S20_CTR              "mDS18X"
  #define D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR              "db18s20"
//#endif
//#ifdef USE_MODULE_SENSORS_DS18X
  #define D_MODULE_SENSORS_INA219_CTR              "mEnergyINA219"
  #define D_MODULE_SENSORS_INA219_FRIENDLY_CTR              "ina219"
//#endif
//#ifdef USE_MODULE_SENSORS_ULTRASONICS
  #define D_MODULE_SENSORS_ULTRASONIC_CTR           "mUltraSonicSensor"
  #define D_MODULE_SENSORS_ULTRASONIC_FRIENDLY_CTR     "ultrasonic"
//#endif
//#ifdef USE_MODULE_SENSORS_DOOR
  #define D_MODULE_SENSORS_DOOR_CTR              "mDoorSensor"
  #define D_MODULE_SENSORS_DOOR_FRIENDLY_CTR              "doorsensor"
//#endif
//#ifdef USE_MODULE_SENSORS_MOTION
  #define D_MODULE_SENSORS_MOTION_CTR              "mMotionSensor"
  #define D_MODULE_SENSORS_MOTION_FRIENDLY_CTR              "motion"

  // #define D_MODULE_SENSORS_MOTION_CTR "MOTION"
  // #define D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "motion"
#define D_MODULE_SENSORS_LSM303D_CTR "lsm303d"
#define D_MODULE_SENSORS_LSM303D_FRIENDLY_CTR "lsm303d"

#define D_MODULE_SENSORS_L3G_FRIENDLY_CTR "l3g"
#define D_MODULE_SENSORS_L3G_CTR "l3g"

#define D_MODULE_SENSORS_MPU9250_CTR "mpu9250"
#define D_MODULE_SENSORS_MPU9250_FRIENDLY_CTR "mpu9250"

//#endif
//#ifdef USE_MODULE_SENSORS_MOISTURE
  #define D_MODULE_SENSORS_RESISTIVE_MOISTURE_CTR              "mMoistureSensor"
  #define D_MODULE_SENSORS_RESISTIVE_MOISTURE_FRIENDLY_CTR              "moisture"
//#endif

#define D_MODULE_SENSORS_ADC_INTERNAL_CTR "adc_internal"
#define D_MODULE_SENSORS_ADC_INTERNAL_FRIENDLY_CTR "adc_internal"

#define D_MODULE_SENSORS_ADC_I2S_INTERNAL_CTR "adc_i2s"
#define D_MODULE_SENSORS_ADC_I2S_INTERNAL_FRIENDLY_CTR "adc_i2s"


//#ifdef USE_MODULE_SENSORS_PULSE_COUNTER
  #define D_MODULE_SENSORS_PULSECOUNTER_CTR              "mPulseCounter"
  #define D_MODULE_SENSORS_PULSECOUNTER_FRIENDLY_CTR              "pulsecounter"

  #define D_MODULE_SENSORS_BH1750_CTR              "mBH1750"
  #define D_MODULE_SENSORS_BH1750_FRIENDLY_CTR              "bh1750"

  #define D_MODULE_SENSORS_SR04_CTR              "mSR04"
  #define D_MODULE_SENSORS_SR04_FRIENDLY_CTR              "sr04"

  #define D_MODULE_SENSORS_REMOTE_DEVICE_CTR "mRemoteDevice"
  #define D_MODULE_SENSORS_REMOTE_DEVICE_FRIENDLY_CTR "remotedevice"

  #define D_MODULE_SENSORS_ROTARY_ENCODER_CTR "mRotaryEncoder"
  #define D_MODULE_SENSORS_ROTARY_ENCODER_FRIENDLY_CTR "rotary_encoder"

//#endif
//#ifdef USE_MODULE_CONTROLLER_BLINDS
  #define D_MODULE_CONTROLLER_BLINDS_CTR              "mBlinds"
  #define D_MODULE_CONTROLLER_BLINDS_FRIENDLY_CTR              "blinds"
//#endif
//#ifdef USE_MODULE_CONTROLLER_HEATING
  #define D_MODULE_CONTROLLER_HVAC_CTR              "mHVAC"
  #define D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR              "hvac"
//#endif
//#ifdef USE_MODULE_CONTROLLER_RADIATORFAN
  #define D_MODULE_CONTROLLER_RADIATORFAN_CTR              "mRadiatorFan"
  #define D_MODULE_CONTROLLER_RADIATORFAN_FRIENDLY_CTR              "radiatorfan"
//#endif
//#ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
  #define D_MODULE_CONTROLLER_CEILINGFAN_CTR               "mCeilingFan"
  #define D_MODULE_CONTROLLER_CEILINGFAN_FRIENDLY_CTR      "ceilingfan"
//#endif
//#ifdef USE_MODULE_CONTROLLER_IRTRANSMITTER
  #define D_MODULE_CONTROLLER_IRTRANSMITTER_CTR              "infrared"
  #define D_MODULE_CONTROLLER_IRTRANSMITTER_FRIENDLY_CTR              "infrared"
//#endif
//#ifdef USE_MODULE_CONTROLLER_TANKVOLUME
  #define D_MODULE_CONTROLLER_TANKVOLUME_CTR              "mTankVolume"
  #define D_MODULE_CONTROLLER_TANKVOLUME_FRIENDLY_CTR              "tankvolume"
//#endif
//#ifdef USE_MODULE_CONTROLLER_EXERCISE_BIKE
  #define D_MODULE_CONTROLLER_EXERCISEBIKE_CTR              "mExerciseBike"
  #define D_MODULE_CONTROLLER_EXERCISEBIKE_FRIENDLY_CTR              "exercisebike"
//#endif



// currently fanpwm
  #define D_MODULE_CONTROLLER_FAN_PWM_CTR              "mFan"
  #define D_MODULE_CONTROLLER_FAN_PWM_FRIENDLY_CTR              "fan"

  #define D_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_CTR "hvac_strip_colour"
  #define D_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_FRIENDLY_CTR "hvac_strip_colour"

#define D_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_CTR "mImmersionTankColour"
#define D_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_FRIENDLY_CTR "sensor_colours"

#define D_MODULE_CONTROLLER_TREADMILL_CTR "mTreadmill"
#define D_MODULE_CONTROLLER_TREADMILL_FRIENDLY_CTR "treadmill"

#define D_MODULE_CONTROLLER_DOORBELL_CTR "doorbell"
#define D_MODULE_CONTROLLER_DOORBELL_FRIENDLY_CTR "doorbell"

#define D_MODULE_CONTROLLER_SDCARDLOGGER_CTR "sdcardlogger"
#define D_MODULE_CONTROLLER_SDCARDLOGGER_FRIENDLY_CTR "sdcardlogger"

#define D_MODULE_CONTROLLER_GPS_SD_LOGGER_CTR "sdcardlogger"
#define D_MODULE_CONTROLLER_GPS_SD_LOGGER_FRIENDLY_CTR "sdcardlogger"


#define D_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_CTR "serial_positional_logger"
#define D_MODULE_CONTROLLER_SERIAL_POSITIONAL_FRIENDLY_LOGGER_CTR "serial_positional_logger"

#define D_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_CTR "serial_calib_pic32"
#define D_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_FRIENDLY_LOGGER_CTR "serial_calib_pic32"

#define D_MODULE_CONTROLLER_USERMOD_01_CTR "usermod_01"
#define D_MODULE_CONTROLLER_USERMOD_01_FRIENDLY_CTR  "usermod_01"

#define D_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_CTR "imu_radiation_pattern"
#define D_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_FRIENDLY_CTR "imu_radiation_pattern"



#define D_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_CTR "bucket_water_level"
#define D_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_FRIENDLY_CTR "bucket_water_level"



#define D_MODULE_CONTROLLER_FURNACE_SENSOR_CTR "furnace_sensor"
#define D_MODULE_CONTROLLER_FURNACE_SENSOR_FRIENDLY_CTR "furnace_sensor"


#define D_MODULE_CONTROLLER_LOUVOLITE_HUB_CTR "louvolite"
#define D_MODULE_CONTROLLER_LOUVOLITE_HUB_FRIENDLY_CTR "louvolite"

#define D_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS_CTR "sidedoor_lights"
#define D_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS_FRIENDLY_CTR "sidedoor_lights"


/**
 * Time
 * */

// HTML (ISO 639-1  Language Code
#define D_HTML_LANGUAGE "en"

// "2017-03-07T11:08:02" - ISO8601:2004
#define D_YEAR_MONTH_SEPARATOR "-"
#define D_MONTH_DAY_SEPARATOR "-"
#define D_DATE_TIME_SEPARATOR "T"
#define D_HOUR_MINUTE_SEPARATOR ":"
#define D_MINUTE_SECOND_SEPARATOR ":"
#define D_DECIMAL_SEPARATOR "."

#define D_DAY3LIST "MonTueWedThuFriSatSun"
#define D_MONTH3LIST "JanFebMarAprMayJunJulAugSepOctNovDec"

/**
 * TaskerInterface Functions
 * */
// #define D_FUNC_MODULE_INIT_CTR                              "MODULE_INIT"
#define D_FUNC_POINTER_INIT_CTR                             "POINTER_INIT"
#define D_FUNC_TEMPLATE_LOAD_CTR                            "TEMPLATE_LOAD"
// #define D_FUNC_MODULE_INIT_CTR                              "MODULE_INIT"
#define D_FUNC_PRE_INIT_CTR                                 "PRE_INIT"
#define D_FUNC_INIT_CTR                                     "INIT"
#define D_FUNC_CONFIGURE_MODULES_FOR_DEVICE_CTR             "CONFIGURE_MODULES_FOR_DEVICE"
#define D_FUNC_LOOP_CTR                                     "LOOP"
#define D_FUNC_EVERY_50_MSECOND_CTR                         "EVERY_50_MSECOND"
#define D_FUNC_EVERY_100_MSECOND_CTR                        "EVERY_100_MSECOND"
#define D_FUNC_EVERY_250_MSECOND_CTR                        "EVERY_250_MSECOND"
#define D_FUNC_EVERY_SECOND_CTR                             "EVERY_SECOND"
#define D_FUNC_EVERY_MINUTE_CTR                             "EVERY_MINUTE" 
#define D_FUNC_EVERY_HOUR_CTR                               "EVERY_HOUR" 
#define D_FUNC_EVERY_MIDNIGHT_CTR                           "EVERY_MIDNIGHT"
#define D_FUNC_EVERY_MIDDAY_CTR                             "EVERY_MIDDAY"
#define D_FUNC_ON_SUCCESSFUL_BOOT_CTR                       "ON_SUCCESSFUL_BOOT"
#define D_FUNC_UPTIME_10_SECONDS_CTR                        "UPTIME_10_SECONDS"
#define D_FUNC_UPTIME_1_MINUTES_CTR                         "UPTIME_1_MINUTES"
#define D_FUNC_UPTIME_10_MINUTES_CTR                        "UPTIME_10_MINUTES"
#define D_FUNC_UPTIME_60_MINUTES_CTR                        "UPTIME_60_MINUTES"
#define D_FUNC_RESTART_SPLASH_INFORMATION_CTR               "RESTART_SPLASH_INFORMATION"
#define D_FUNC_PREP_BEFORE_TELEPERIOD_CTR                   "PREP_BEFORE_TELEPERIOD"
#define D_FUNC_JSON_APPEND_CTR                              "JSON_APPEND"
#define D_FUNC_SAVE_BEFORE_RESTART_CTR                      "SAVE_BEFORE_RESTART"
#define D_FUNC_SETTINGS_DEFAULT_CTR                         "SETTINGS_DEFAULT"
#define D_FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES_CTR      "SETTINGS_PRELOAD_DEFAULT_IN_MODULES"
#define D_FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR      "SETTINGS_OVERWRITE_SAVED_TO_DEFAULT"
#define D_FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR         "SETTINGS_LOAD_VALUES_INTO_MODULE"
#define D_FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR         "SETTINGS_SAVE_VALUES_FROM_MODULE"
#define D_FUNC_FUNCTION_LAMBDA_INIT_CTR                     "FUNCTION_LAMBDA_INIT"
#define D_FUNC_FUNCTION_LAMBDA_LOOP_CTR                     "FUNCTION_LAMBDA_LOOP"
#define D_FUNC_COMMAND_CTR                                  "COMMAND"
#define D_FUNC_COMMAND_SENSOR_CTR                           "COMMAND_SENSOR"
#define D_FUNC_COMMAND_DRIVER_CTR                           "COMMAND_DRIVER"
#define D_FUNC_JSON_COMMAND_CTR                             "JSON_COMMAND"
#define D_FUNC_JSON_COMMAND_OBJECT_CTR                      "JSON_COMMAND_OBJECT"
#define D_FUNC_WIFI_CONNECTED_CTR                           "WIFI_CONNECTED"
#define D_FUNC_WIFI_DISCONNECTED_CTR                        "WIFI_DISCONNECTED"
#define D_FUNC_MQTT_SUBSCRIBE_CTR                           "MQTT_SUBSCRIBE"
#define D_FUNC_MQTT_INIT_CTR                                "MQTT_INIT"
#define D_FUNC_MQTT_CONNECTED_CTR                           "MQTT_CONNECTED"
#define D_FUNC_MQTT_DISCONNECTED_CTR                        "MQTT_DISCONNECTED"
#define D_FUNC_MQTT_COMMAND_CTR                             "MQTT_COMMAND"
#define D_FUNC_MQTT_SENDER_CTR                              "MQTT_SENDER"
#define D_FUNC_MQTT_HANDLERS_RESET_CTR                      "MQTT_HANDLERS_RESET"
#define D_FUNC_MQTT_HANDLERS_INIT_CTR                       "MQTT_HANDLERS_INIT"
#define D_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR         "MQTT_HANDLERS_REFRESH_TELEPERIOD"
#define D_FUNC_SET_POWER_CTR                                "SET_POWER"
#define D_FUNC_SET_DEVICE_POWER_CTR                         "SET_DEVICE_POWER"
#define D_FUNC_SHOW_SENSOR_CTR                              "SHOW_SENSOR"
#define D_FUNC_RULES_PROCESS_CTR                            "RULES_PROCESS"
#define D_FUNC_SERIAL_CTR                                   "SERIAL"
#define D_FUNC_FREE_MEM_CTR                                 "FREE_MEM"
#define D_FUNC_BUTTON_PRESSED_CTR                           "BUTTON_PRESSED"
#define D_FUNC_ENERGY_RESET_CTR                             "ENERGY_RESET"
#define D_FUNC_SENSOR_UPDATED_CTR                           "SENSOR_UPDATED"
#define D_FUNC_STATUS_MESSAGE_APPEND_CTR                    "STATUS_MESSAGE_APPEND"
#define D_FUNC_JSON_HARDWARE_APPEND_CTR                     "JSON_HARDWARE_APPEND"
#define D_FUNC_SET_CHANNELS_CTR                             "SET_CHANNELS"
#define D_FUNC_WEB_ADD_MAIN_BUTTON_CTR                      "WEB_ADD_MAIN_BUTTON"
#define D_FUNC_WEB_ADD_BUTTON_CTR                           "WEB_ADD_BUTTON"
#define D_FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR           "WEB_ADD_BUTTON_SYSTEM_SETTINGS"
#define D_FUNC_WEB_ADD_HANDLER_CTR                          "WEB_ADD_HANDLER"
#define D_FUNC_WEB_ROOT_SEND_STYLE_CTR                      "WEB_ROOT_SEND_STYLE"
#define D_FUNC_WEB_ROOT_SEND_SCRIPT_CTR                     "WEB_ROOT_SEND_SCRIPT"
#define D_FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR  "WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING"
#define D_FUNC_WEB_ROOT_SEND_BODY_CTR                       "WEB_ROOT_SEND_BODY"
#define D_FUNC_WEB_ROOT_SEND_STATUS_CTR                     "WEB_ROOT_SEND_STATUS"
#define D_FUNC_WEB_ADD_ROOT_SHOWS_CTR                       "WEB_ADD_ROOT_SHOWS"
#define D_FUNC_WEB_ADD_JSON_FETCH_RESULT_CTR                "WEB_ADD_JSON_FETCH_RESULT"
#define D_FUNC_WEB_ADD_JSON_DATA_FETCH_URL_CTR              "WEB_ADD_JSON_DATA_FETCH_URL"
#define D_FUNC_WEB_ADD_ROOT_TABLE_ROWS_CTR                  "WEB_ADD_ROOT_TABLE_ROWS"
#define D_FUNC_WEB_ADD_ROOT_SCRIPT_CTR                      "WEB_ADD_ROOT_SCRIPT"
#define D_FUNC_WEB_ADD_ROOT_STYLE_CTR                       "WEB_ADD_ROOT_STYLE"
#define D_FUNC_WEB_APPEND_LOADTIME_ROOT_URLS_CTR            "WEB_APPEND_LOADTIME_ROOT_URLS"
#define D_FUNC_WEB_APPEND_LOADTIME_ROOT_RATES_CTR           "WEB_APPEND_LOADTIME_ROOT_RATES"
#define D_FUNC_WEB_APPEND_RUNTIME_ROOT_URLS_CTR             "WEB_APPEND_RUNTIME_ROOT_URLS"
#define D_FUNC_WEB_APPEND_RUNTIME_ROOT_RATES_CTR            "WEB_APPEND_RUNTIME_ROOT_RATES"
#define D_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR   "WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED"
#define D_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR      "WEB_APPEND_ROOT_STATUS_TABLE_FORCED"
#define D_FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR         "WEB_APPEND_ROOT_ADD_MAIN_BUTTONS"
#define D_FUNC_WEB_APPEND_ROOT_BUTTONS_CTR                  "WEB_APPEND_ROOT_BUTTONS"
#define D_FUNC_WEB_PAGEINFORMATION_SEND_MODULE_CTR          "WEB_PAGEINFORMATION_SEND_MODULE"
#define D_FUNC_WEB_COMMAND_CTR                              "WEB_COMMAND"
// #define D_FUNC_CHECK_POINTERS_CTR                           "CHECK_POINTERS"
#define D_FUNC_WEB_SYSTEM_INFO_CTR                          "WEB_SYSTEM_INFO"
#define D_FUNC_DEBUG_CONFIGURE_CTR                          "DEBUG_CONFIGURE"


#define D_FUNC_EVENT_MOTION_STARTED_CTR "MotionStarted"
#define D_FUNC_EVENT_MOTION_ENDED_CTR "MotionEnded"

#define   D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "StateChanged"
#define   D_FUNC_EVENT_SET_POWER_CTR   "SetPower"
#define   D_FUNC_EVENT_SET_SPEED_CTR   "SetSpeed"

/**
 * Common list - These are D_<exact letters in string> that are Capital first
 * */
#define D_ALTITUDE              "Altitude"
#define D_ADMIN                 "Admin"
#define D_AIR_QUALITY           "Air quality"
#define D_ANALOG_INPUT          "Analog"
#define D_AP                    "AP"
#define D_AS                    "As"
#define D_AUTO                  "AUTO"
#define D_BLINK                 "Blink"
#define D_BLINKOFF              "BlinkOff"
#define D_BOOT_COUNT            "Boot Count"
#define D_BRIGHTLIGHT           "Bright"
#define D_BSSID                 "BSSId"
#define D_BUTTON                "Button"
#define D_BY                    "by"                    
#define D_BYTES                 "Bytes"
#define D_CELSIUS               "Celsius"
#define D_CHANNEL               "Channel"
#define D_CO2                   "Carbon dioxide"
#define D_COLDLIGHT             "Cold"
#define D_COMMAND               "Command"
#define D_CONNECTED             "Connected"
#define D_COUNT                 "Count"
#define D_COUNTER               "Counter"
#define D_CURRENT               "Current"          
#define D_CYCLE "Cycle"
#define D_DATA                  "Data"
#define D_DARKLIGHT             "Dark"
#define D_DEBUG                 "Debug"
#define D_DEVICE                "Device"
#define D_DISABLED              "Disabled"
#define D_DESIRED               "Desired"
#define D_DISTANCE              "Distance"
#define D_DNS_SERVER            "DNS Server"
#define D_DONE                  "Done"
#define D_DURATION              "Duration"
#define D_DST_TIME              "DST"
#define D_EMULATION             "Emulation"
#define D_ENABLED               "Enabled"
#define D_ERASE                 "Erase"
#define D_ERROR                 "Error"
#define D_WARNING               "Warning"
#define D_FAHRENHEIT            "Fahrenheit"
#define D_FAILED                "Failed"
#define D_FALLBACK              "Fallback"
#define D_FALLBACK_TOPIC        "Fallback Topic"
#define D_FALSE                 "False"
#define D_FILE                  "File"
#define D_FLOW_RATE             "Flow rate"
#define D_FREE_MEMORY           "Free Memory"
#define D_FREQUENCY             "Frequency"
#define D_GAS                   "Gas"
#define D_GATEWAY               "Gateway"
#define D_GROUP                 "Group"
#define D_HOST                  "Host"
#define D_HOSTNAME              "Hostname"
#define D_HUM                   "Hum"
#define D_HUMIDITY              "Humidity"
#define D_ILLUMINANCE           "Illuminance"
#define D_IMMEDIATE             "Immediate"
#define D_IMPORTANCE            "Importance"
#define D_INDEX                 "Index"
#define D_INFO                  "Info"
#define D_INFRARED              "Infrared"
#define D_INITIALIZED           "Initialized"
#define D_IP_ADDRESS            "IP Address"
#define D_IP                    "IP"
#define D_ISVALID               "IsValid"
#define D_ISRUNNING             "IsRunning"
#define D_ISCHANGED             "IsChanged"
#define D_LIGHT                 "Light"
#define D_LIMIT                 "Limit"
#define D_LENGTH                "Length"
#define D_LIMIT                 "Limit"
#define D_DIMMER                "Dimmer"
#define D_LWT                   "LWT"
#define D_MEASURE               "Measure"
#define D_MESSAGE               "Message"
#define D_MAX                   "Max"
#define D_MIN                   "Min"
#define D_MODE                  "Mode"
#define D_STEP                  "Step"
#define D_MODULE                "Module"
#define D_MQTT                  "MQTT"
#define D_MULTI_PRESS           "multi-press"
#define D_NOISE                 "Noise"
#define D_NONE                  "None"
#define D_NOT_PRESSED           "Not Pressed"
#define D_OFF                   "Off"
#define D_OFFLINE               "Offline"
#define D_OK                    "Ok"
#define D_ON                    "On"
#define D_ONLINE                "Online"
#define D_ONTIME                "Ontime"
#define D_ONOFF                 "OnOff"
#define D_OFFTIME               "Offtime"
#define D_PASSWORD              "Password"
#define D_PARSING_NOMATCH       "Parsing NOT Matched "

// #define ENABLE_PARSING_DEBUG_LINE

#ifdef ENABLE_PARSING_DEBUG_LINE
#define D_PARSING_MATCHED       "Parsing Not Matched" String(__LINE__)
#else
#define D_PARSING_MATCHED       "Parsing Matched "
#endif
#define D_PORT                  "Port"
#define D_POWER_FACTOR          "Power Factor"
#define D_POWERUSAGE            "Power"
#define D_POWERUSAGE_ACTIVE     "Active Power"
#define D_POWERUSAGE_APPARENT   "Apparent Power"
#define D_POWERUSAGE_REACTIVE   "Reactive Power"
#define D_PRESSURE              "Pressure"
#define D_PRESSUREATSEALEVEL    "SeaPressure"
#define D_PRESSED               "Pressed"
#define D_PROGRAM_FLASH_SIZE    "Program Flash Size"
#define D_PROGRAM_SIZE          "Program Size"
#define D_PROJECT               "Project"
#define D_RAIN                  "Rain"
#define D_RGB                   "RGB"
#define D_READ                  "Read"
#define D_RELAY                 "Relay"
#define D_RECEIVED              "Received"
#define D_RESTART               "Restart"
#define D_RESTARTING            "Restarting"
#define D_RESTART_REASON        "Restart Reason"
#define D_RESTORE               "restore"
#define D_RETAINED              "retained"
#define D_RULE                  "Rule"
#define D_SAVE                  "Save"
#define D_SENSOR                "Sensor"
#define D_SET                   "Set"
#define D_SECS                  "Secs"
#define D_SIZE                  "Size"
#define D_SSID                  "SSId"
#define D_START                 "Start"
#define D_STATUS                "Status"
#define D_STD_TIME              "STD"
#define D_STOP                  "Stop"
#define D_SUBNET_MASK           "Subnet Mask"
#define D_SUBSCRIBE_TO          "Subscribe to"
#define D_UNSUBSCRIBE_FROM      "Unsubscribe from"
#define D_SUCCESSFUL            "Successful"
#define D_SUNRISE               "Sunrise"
#define D_SUNSET                "Sunset"
#define D_SYSTEM                "System"
#define D_FUNCTION_TASKER_INTERFACE "Tasker_Interface "
#define D_TEMP                  "Temp"
#define D_TEMPS                 "Temps"
#define D_TYPE                  "Type"
#define D_TEMPERATURE           "Temperature"
#define D_TO                    "to"
#define D_TOGGLE                "Toggle"
#define D_TIME_ON               "Time_On"
#define D_TOPIC                 "Topic"
#define D_TIMERS                "Timers"
#define D_PAYLOAD               "Payload"
#define D_TOTAL_USAGE           "Total Usage"
#define D_TRANSMIT              "Transmit"
#define D_TRUE                  "True"
#define D_UPGRADE               "Upgrade"
#define D_UPLOAD                "Upload"
#define D_UPTIME                "Uptime"
#define D_USER                  "User"
#define D_UTC_TIME              "UTC"
#define D_VERSION               "Version"
#define D_VOLTAGE               "Voltage"
#define D_VALUES                "Values"
#define D_WEIGHT                "Weight"
#define D_WARMLIGHT             "Warm"
#define D_WEB_SERVER            "Web Server"
#define D_LOAD                  "Load"
#define D_SAVE                  "Save"
#define D_TASKER "Tasker "
#define D_UNKNOWN "UNKNOWN"
#define D_DEBUG_FUNCTION "Debug Function: "
#define D_SENDING "Sending"


// main.cpp
#define D_SERIAL_LOGGING_DISABLED       "Serial logging disabled"
#define D_SYSLOG_LOGGING_REENABLED      "Syslog logging re-enabled"
#define D_SET_BAUDRATE_TO               "Set Baudrate to"

/**
 *  support
 * */
#define D_OSWATCH                       "osWatch"
#define D_BLOCKED_LOOP                  "Blocked Loop"
#define D_WPS_FAILED_WITH_STATUS        "WPSconfig FAILED with status"
#define D_ACTIVE_FOR_3_MINUTES          "active for 3 minutes"
#define D_FAILED_TO_START               "failed to start"
#define D_CONNECTING_TO_AP              "Connecting to AP"
#define D_IN_MODE                       "in mode"
#define D_CONNECT_FAILED_NO_IP_ADDRESS  "Connect failed as no IP address received"
#define D_CONNECT_FAILED_AP_NOT_REACHED "Connect failed as AP cannot be reached"
#define D_CONNECT_FAILED_WRONG_PASSWORD "Connect failed with AP incorrect password"
#define D_CONNECT_FAILED_AP_TIMEOUT     "Connect failed with AP timeout"
#define D_ATTEMPTING_CONNECTION         "Attempting connection..."
#define D_CHECKING_CONNECTION           "Checking connection..."
#define D_QUERY_DONE                    "Query done. MQTT services found"
#define D_MQTT_SERVICE_FOUND            "MQTT service found on"
#define D_FOUND_AT                      "found at"
#define D_SYSLOG_HOST_NOT_FOUND         "Syslog Host not found"
#define D_ERROR_UNSUPPORTED             "ERROR : Feature not currently supported"
#define D_LOG_SOME_SETTINGS_RESET "Some settings have been reset"

/**
 * settings
 * */
#define D_SAVED_TO_FLASH_AT             "Saved to flash at"
#define D_LOADED_FROM_FLASH_AT          "Loaded from flash at"
#define D_USE_DEFAULTS                  "Use defaults"
#define D_ERASED_SECTOR                 "Erased sector"

/**
 * webserver
 * */
#define D_NOSCRIPT "JavaScript Needed"
#define D_MINIMAL_FIRMWARE_PLEASE_UPGRADE "MINIMAL firmware<br/>please upgrade"
#define D_WEBSERVER_ACTIVE_ON "Web server active on"
#define D_WEBSERVER "WebServer"
#define D_WITH_IP_ADDRESS "with IP address"
#define D_WEBSERVER_STOPPED "Web server stopped"
#define D_FILE_NOT_FOUND "File Not Found"
#define D_REDIRECTED "Redirected to captive portal"
#define D_WIFIMANAGER_SET_ACCESSPOINT_AND_STATION "Wifimanager set AccessPoint and keep Station"
#define D_WIFIMANAGER_SET_ACCESSPOINT "Wifimanager set AccessPoint"
#define D_TRYING_TO_CONNECT "Trying to connect device to network"

#define D_RESTART_IN "Restart in"

#define D_SECONDS "Seconds"
#define D_MINUTES "Minutes"

#define D_DEVICE_WILL_RESTART "Device will restart in a few seconds"
#define D_BUTTON_TOGGLE "Toggle"
#define D_CONFIGURATION "Configuration"
#define D_INFORMATION "Information"
#define D_FIRMWARE_UPGRADE "Firmware Upgrade"
#define D_CONSOLE "Console"
#define D_CONFIRM_RESTART "Confirm Restart"
#define D_SYSTEM_SETTINGS "System Settings"

#define D_COLOUR_PALETTE_EDITOR "Colour Palette Editor"
#define D_COLOUR_MIXER_EDITOR "Colour Mixer Editor"
#define D_CONFIGURE_MODULE "Configure Module"
#define D_CONFIGURE_WIFI "Configure WiFi"
#define D_CONFIGURE_MQTT "Configure MQTT"
#define D_CONFIGURE_LOGGING "Configure Logging"
#define D_CONFIGURE_OTHER "Configure Other"
#define D_CONFIRM_RESET_CONFIGURATION "Confirm Reset Configuration"
#define D_RESET_CONFIGURATION "Reset Configuration"
#define D_BACKUP_CONFIGURATION "Backup Configuration"
#define D_RESTORE_CONFIGURATION "Restore Configuration"
#define D_MAIN_MENU "Main Menu"

#define D_MODULE_PARAMETERS "Module parameters"
#define D_MODULE_TYPE "Module type"
#define D_PULLUP_ENABLE "No Button/Switch pull-up"
#define D_GPIO "GPIO"
#define D_SERIAL_IN "Serial In"
#define D_SERIAL_OUT "Serial Out"

#define D_WIFI_PARAMETERS "Wifi parameters"
#define D_SCAN_FOR_WIFI_NETWORKS "Scan for wifi networks"
#define D_SCAN_DONE "Scan done"
#define D_NO_NETWORKS_FOUND "No networks found"
#define D_REFRESH_TO_SCAN_AGAIN "Refresh to scan again"
#define D_DUPLICATE_ACCESSPOINT "Duplicate AccessPoint"
#define D_SKIPPING_LOW_QUALITY "Skipping due to low quality"
#define D_RSSI "RSSI"
#define D_WEP "WEP"
#define D_WPA_PSK "WPA PSK"
#define D_WPA2_PSK "WPA2 PSK"
#define D_AP1_SSID "AP1 SSId"
#define D_AP1_PASSWORD "AP1 Password"
#define D_AP2_SSID "AP2 SSId"
#define D_AP2_PASSWORD "AP2 Password"

#define D_MQTT_PARAMETERS "MQTT parameters"
#define D_CLIENT "Client"
#define D_FULL_TOPIC "Full Topic"

#define D_LOGGING_PARAMETERS "Logging parameters"
#define D_SERIAL_LOG_LEVEL "Serial log level"
#define D_WEB_LOG_LEVEL "Web log level"
#define D_SYS_LOG_LEVEL "Syslog level"
#define D_MORE_DEBUG "More debug"
#define D_SYSLOG_HOST "Syslog host"
#define D_SYSLOG_PORT "Syslog port"
#define D_TELEMETRY_PERIOD "Telemetry period"

#define D_OTHER_PARAMETERS "Other parameters"
#define D_TEMPLATE "Template"
#define D_ACTIVATE "Activate"
#define D_WEB_ADMIN_PASSWORD "Web Admin Password"
#define D_MQTT_ENABLE "MQTT enable"
#define D_FRIENDLY_NAME "Friendly Name"
#define D_BELKIN_WEMO "Belkin WeMo"
#define D_HUE_BRIDGE "Hue Bridge"
#define D_SINGLE_DEVICE "single device"
#define D_MULTI_DEVICE "multi device"

#define D_CONFIGURE_TEMPLATE "Configure Template"
#define D_TEMPLATE_PARAMETERS "Template parameters"
#define D_TEMPLATE_NAME "Name"
#define D_BASE_TYPE "Based on"
#define D_TEMPLATE_FLAGS "Options"
#define D_ALLOW_ADC0 "ADC0 input"
#define D_ALLOW_ADC0_TEMP "ADC0 temperature"
#define D_ALLOW_PULLUP "User pull-up selection"

#define D_SAVE_CONFIGURATION "Save configuration"
#define D_CONFIGURATION_SAVED "Configuration saved"
#define D_CONFIGURATION_RESET "Configuration reset"

#define D_PROGRAM_VERSION "Program Version"
#define D_BUILD_DATE_AND_TIME "Build Date & Time"
#define D_CORE_AND_SDK_VERSION "Core/SDK Version"
#define D_FLASH_WRITE_COUNT "Flash write Count"
#define D_MAC_ADDRESS "MAC Address"
#define D_MQTT_HOST "MQTT Host"
#define D_MQTT_PORT "MQTT Port"
#define D_MQTT_CLIENT "MQTT Client"
#define D_MQTT_USER "MQTT User"
#define D_MQTT_TOPIC "MQTT Topic"
#define D_MQTT_GROUP_TOPIC "MQTT Group Topic"
#define D_MQTT_FULL_TOPIC "MQTT Full Topic"
#define D_MDNS_DISCOVERY "mDNS Discovery"
#define D_MDNS_ADVERTISE "mDNS Advertise"
#define D_ESP_CHIP_ID "ESP Chip Id"
#define D_FLASH_CHIP_ID "Flash Chip Id"
#define D_FLASH_CHIP_SIZE "Flash Size"
#define D_FREE_PROGRAM_SPACE "Free Program Space"

#define D_UPGRADE_BY_WEBSERVER "Upgrade by web server"
#define D_OTA_URL "OTA Url"
#define D_START_UPGRADE "Start upgrade"
#define D_UPGRADE_BY_FILE_UPLOAD "Upgrade by file upload"
#define D_UPLOAD_STARTED "Upload started"
#define D_UPGRADE_STARTED "Upgrade started"
#define D_UPLOAD_DONE "Upload done"
#define D_UPLOAD_ERR_1 "No file selected"
#define D_UPLOAD_ERR_2 "Not enough space"
#define D_UPLOAD_ERR_3 "Magic byte is not 0xE9"
#define D_UPLOAD_ERR_4 "Program flash size is larger than real flash size"
#define D_UPLOAD_ERR_5 "Upload buffer miscompare"
#define D_UPLOAD_ERR_6 "Upload failed. Enable logging 3"
#define D_UPLOAD_ERR_7 "Upload aborted"
#define D_UPLOAD_ERR_8 "File invalid"
#define D_UPLOAD_ERR_9 "File too large"
#define D_UPLOAD_ERR_10 "Failed to init RF chip"
#define D_UPLOAD_ERR_11 "Failed to erase RF chip"
#define D_UPLOAD_ERR_12 "Failed to write to RF chip"
#define D_UPLOAD_ERR_13 "Failed to decode RF firmware"
#define D_UPLOAD_ERROR_CODE "Upload error code"

#define D_ENTER_COMMAND "Enter command"
#define D_ENABLE_WEBLOG_FOR_RESPONSE "Enable weblog 2 if response expected"
#define D_NEED_USER_AND_PASSWORD "Need user=<username>&password=<password>"

/**
 * Pixels
 * */
#define D_NEOPIXEL_PALETTE "Palette "
#define D_NEOPIXEL_TRANSITION_ORDER "Transition order "
#define D_NEOPIXEL_BLENDTIME "Transition time "
#define D_NEOPIXEL_BLENDRATE "Transition rate "
#define D_NEOPIXEL_MODE "Mode "

/**
 * mqtt
 * */
#define D_FINGERPRINT "Verify TLS fingerprint..."
#define D_TLS_CONNECT_FAILED_TO "TLS Connect failed to"
#define D_RETRY_IN "Retry in"
#define D_VERIFIED "Verified using Fingerprint"
#define D_INSECURE "Insecure connection due to invalid Fingerprint"
#define D_CONNECT_FAILED_TO "Connect failed to"
#define D_TOPIC_TELE      "tele"
#define D_TOPIC_IFCHANGED "ifchanged"
#define D_TOPIC_SLASH "/"
#define D_TOPIC_STATUS "status"
#define D_TOPIC_STATUS_S      D_TOPIC_STATUS D_TOPIC_SLASH
#define D_TOPIC_S_TELE_S      D_TOPIC_SLASH D_TOPIC_TELE D_TOPIC_SLASH
#define D_TOPIC_S_IFCHANGED_S D_TOPIC_SLASH D_TOPIC_IFCHANGED D_TOPIC_SLASH
// #define D_TOPIC_PIXELS "/pixels"
// #define D_TOPIC_NEXTION "/nextion"
// #define D_TOPIC_CEILINGFAN "/ceilingfan"
// #define D_TOPIC_SYSTEM "/system"
// #define D_TOPIC_HEATING "/heating"
// #define D_TOPIC_RELAYS "/relays"
// #define D_TOPIC_RADIATORFAN "/radiatorfan"
// #define D_TOPIC_BLINDS "/blinds"

#define D_TOPIC_COMMAND "set" //to be "command"
#define D_TOPIC_STATUS "status" //to be "command"
#define D_TOPIC_RESPONSE "response"


/**
 * Nextion
 * */
#define D_NEXTION_TX "HMI TX --> "
#define D_NEXTION_RX "HMI RX <-- "
#define D_NEXTION_COMMAND "TYPE"


/**
 * Timers
 * */
#define D_CONFIGURE_TIMER "Configure Timer"
#define D_TIMER_PARAMETERS "Timer parameters"
#define D_TIMER_ENABLE "Enable Timers"
#define D_TIMER_ARM "Arm"
#define D_TIMER_TIME "Time"
#define D_TIMER_DAYS "Days"
#define D_TIMER_REPEAT "Repeat"
#define D_TIMER_OUTPUT "Output"
#define D_TIMER_ACTION "Action"


/**
 * Energy
 * */
#define D_ENERGY_TODAY "Energy Today"
#define D_ENERGY_YESTERDAY "Energy Yesterday"
#define D_ENERGY_TOTAL "Energy Total"


/**
 * db18s20
 * */
#define D_SENSOR_BUSY "Sensor busy"
#define D_SENSOR_CRC_ERROR "Sensor CRC error"
#define D_SENSORS_FOUND "Sensors found"


/**
 * dht
 * */
#define D_TIMEOUT_WAITING_FOR "Timeout waiting for"
#define D_START_SIGNAL_LOW "start signal low"
#define D_START_SIGNAL_HIGH "start signal high"
#define D_PULSE "pulse"
#define D_CHECKSUM_FAILURE "Checksum failure"


/**
 * mpu6050
 * */
#define D_AX_AXIS "Accel. X-Axis"
#define D_AY_AXIS "Accel. Y-Axis"
#define D_AZ_AXIS "Accel. Z-Axis"
#define D_GX_AXIS "Gyro X-Axis"
#define D_GY_AXIS "Gyro Y-Axis"
#define D_GZ_AXIS "Gyro Z-Axis"


/****
 * 
 * List of function, that will be shown in webui
 *  keep them as short as possible to be able to fit them in GUI drop down box 
 * */
#define D_GPIO_FUNCTION_NONE_CTR          "None"
#define D_GPIO_FUNCTION_USER_CTR          "User"
#define D_GPIO_FUNCTION_DOOR_POSITION_CTR "Door Position"
#define D_GPIO_FUNCTION_RGB_DATA1_CTR      "RGBData1"
#define D_GPIO_FUNCTION_RGB_DATA_CTR      "WS281x Data"
#define D_GPIO_FUNCTION_RGB_CLOCK_CTR     "WS281x Clock"
#define D_GPIO_FUNCTION_SWT1_CTR     "SWT1"
#define D_GPIO_FUNCTION_SWT2_CTR     "SWT2"
#define D_GPIO_FUNCTION_SWT1_INV_CTR     "SWT1 Inv"
#define D_GPIO_FUNCTION_SWT2_INV_CTR     "SWT2 Inv"
#define D_GPIO_FUNCTION_SWT3_INV_CTR     "SWT3 Inv"
#define D_GPIO_FUNCTION_SWT1_NP_CTR     "SWT1_NP"
#define D_GPIO_FUNCTION_SWT2_NP_CTR     "SWT2_NP"
#define D_GPIO_FUNCTION_REL1_CTR          "Relay 1"
#define D_GPIO_FUNCTION_REL2_CTR          "Relay 2"
#define D_GPIO_FUNCTION_REL3_CTR          "Relay 3"
#define D_GPIO_FUNCTION_REL4_CTR          "Relay 4"
#define D_GPIO_FUNCTION_REL1_INV_CTR      "Relay 1 Inv"
#define D_GPIO_FUNCTION_REL2_INV_CTR      "Relay 2 Inv"
#define D_GPIO_FUNCTION_REL3_INV_CTR      "Relay 3 Inv"
#define D_GPIO_FUNCTION_REL4_INV_CTR      "Relay 4 Inv"
#define D_GPIO_FUNCTION_FAN_PWM1_CTR      "Fan PWM1"
#define D_GPIO_FUNCTION_I2C_SCL_CTR       "I2C SCL"
#define D_GPIO_FUNCTION_I2C_SDA_CTR       "I2C SDA"

#define D_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR "LDR Basic A1"
#define D_GPIO_FUNCTION_LDR_BASIC_ANALOG2_CTR "LDR Basic A2"
#define D_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR "LDR Basic D1"
#define D_GPIO_FUNCTION_LDR_BASIC_DIGITAL2_CTR "LDR Basic D2"


#define D_GPIO_FUNCTION_ANALOG_INPUT0_CTR "Analog Input0"

#define D_GPIO_FUNCTION_OLED_RESET_CTR "OLED Reset"

#define D_GPIO_FUNCTION_PIR_1_CTR         "PIR 1"
#define D_GPIO_FUNCTION_PIR_2_CTR         "PIR 2"
#define D_GPIO_FUNCTION_PIR_3_CTR         "PIR 3"
#define D_GPIO_FUNCTION_PIR_1_NP_CTR         "PIR 1 NP"
#define D_GPIO_FUNCTION_PIR_2_NP_CTR         "PIR 2 NP"
#define D_GPIO_FUNCTION_PIR_3_NP_CTR         "PIR 3 NP"
#define D_GPIO_FUNCTION_PIR_1_INV_CTR     "PIR 1 Inv"
#define D_GPIO_FUNCTION_PIR_2_INV_CTR     "PIR 2 Inv"
#define D_GPIO_FUNCTION_PIR_3_INV_CTR     "PIR 3 Inv"
#define D_GPIO_FUNCTION_PIR_1_NP_INV_CTR     "PIR 1 NP Inv"
#define D_GPIO_FUNCTION_PIR_2_NP_INV_CTR     "PIR 2 NP Inv"
#define D_GPIO_FUNCTION_PIR_3_NP_INV_CTR     "PIR 3 NP Inv"
#define D_GPIO_FUNCTION_DHT11_CTR         "DHT11"
#define D_GPIO_FUNCTION_DHT11_1_CTR         "DHT11_1"
#define D_GPIO_FUNCTION_DHT11_2_CTR         "DHT11_2"
#define D_GPIO_FUNCTION_DHT22_CTR         "DHT22"
#define D_GPIO_FUNCTION_DHT22_1_CTR         "DHT22_1"
#define D_GPIO_FUNCTION_DHT22_2_CTR         "DHT22_2"
#define D_GPIO_FUNCTION_AM2301_CTR        "AM2301"
#define D_GPIO_FUNCTION_SI7021_CTR        "SI7021"
#define D_GPIO_FUNCTION_DS18X20_CTR       "DS18x20"
#define D_GPIO_FUNCTION_DS18X20_1_CTR       "DS18x20_1"
#define D_GPIO_FUNCTION_DS18X20_2_CTR       "DS18x20_2"
#define D_GPIO_FUNCTION_CHIME_RINGER_CTR  "Chime Ringer"
#define D_GPIO_FUNCTION_CHIME_INPUT_CTR   "Chime Input"
#define D_GPIO_FUNCTION_IRSEND_CTR        "IRsend"
#define D_GPIO_FUNCTION_SWITCH_CTR        "Switch"     // Suffix "1"
// #define D_GPIO_FUNCTION_SWITCH_1_CTR      D_GPIO_FUNCTION_SWITCH_CTR " 1"     // Suffix "1"
// #define D_GPIO_FUNCTION_SWITCH_2_CTR      D_GPIO_FUNCTION_SWITCH_CTR " 2"     // Suffix "1"
// #define D_GPIO_FUNCTION_SWITCH_3_CTR      D_GPIO_FUNCTION_SWITCH_CTR " 3"     // Suffix "1"
// #define D_GPIO_FUNCTION_SWITCH_4_CTR      D_GPIO_FUNCTION_SWITCH_CTR " 4"     // Suffix "1"
#define D_GPIO_FUNCTION_BUTTON_CTR        "Button"     // Suffix "1"
// #define D_GPIO_FUNCTION_BUTTON_1_CTR      D_GPIO_FUNCTION_BUTTON_CTR " 1"     // Suffix "1"
#define D_GPIO_FUNCTION_RELAY_CTR         "Relay"      // Suffix "1i"
// #define D_GPIO_FUNCTION_RELAY_1_CTR       D_GPIO_FUNCTION_RELAY_CTR " 1"      // Suffix "1i"
// #define D_GPIO_FUNCTION_RELAY_2_CTR       D_GPIO_FUNCTION_RELAY_CTR " 2"      // Suffix "1i"
// #define D_GPIO_FUNCTION_RELAY_3_CTR       D_GPIO_FUNCTION_RELAY_CTR " 3"      // Suffix "1i"
// #define D_GPIO_FUNCTION_RELAY_4_CTR       D_GPIO_FUNCTION_RELAY_CTR " 4"      // Suffix "1i"
#define D_GPIO_FUNCTION_LED_CTR           "Led"        // Suffix "1i"
// #define D_GPIO_FUNCTION_NETWORK_STATUS_LED_CTR "Network Status LED"
//REMOVE LINK LED
// #define D_GPIO_FUNCTION_LEDLNK_CTR          D_GPIO_FUNCTION_LED_CTR "_Link"
// #define D_GPIO_FUNCTION_LED1_CTR          D_GPIO_FUNCTION_LED_CTR "_Link" " Inv"
#define D_GPIO_FUNCTION_DOOR_OPEN_CTR       "Door Open"
#define D_GPIO_FUNCTION_DOOR_LOCK_CTR       "Door Lock"
#define D_GPIO_FUNCTION_SERIAL_DEBUG_TX_CTR "Serial Debug TX"
#define D_GPIO_FUNCTION_SERIAL_DEBUG_RX_CTR "Serial Debug RX"

#define D_GPIO_FUNCTION_HWSERIAL0_TX_CTR "HWSERIAL0_TX"
#define D_GPIO_FUNCTION_HWSERIAL0_RX_CTR "HWSERIAL0_RX"

#define D_GPIO_FUNCTION_GPS_SERIAL0_TX_CTR "GPS Serial0 TX"
#define D_GPIO_FUNCTION_GPS_SERIAL0_RX_CTR "GPS Serial0 RX"
#define D_GPIO_FUNCTION_GPS_SERIAL1_TX_CTR "GPS Serial1 TX"
#define D_GPIO_FUNCTION_GPS_SERIAL1_RX_CTR "GPS Serial1 RX"
#define D_GPIO_FUNCTION_GPS_SERIAL2_TX_CTR "GPS Serial2 TX"
#define D_GPIO_FUNCTION_GPS_SERIAL2_RX_CTR "GPS Serial2 RX"


#define D_GPIO_FUNCTION__FONA_POWER_KEY__CTR       "Fona Key"
#define D_GPIO_FUNCTION__FONA_POWER_STATUS__CTR    "Fona PS"
#define D_GPIO_FUNCTION__FONA_NETWORK_STATUS__CTR  "Fona NS"
#define D_GPIO_FUNCTION__FONA_RESET__CTR           "Fona Rst"
#define D_GPIO_FUNCTION__FONA_UART_TX__CTR         "Fona TX"
#define D_GPIO_FUNCTION__FONA_UART_RX__CTR         "Fona RX"
#define D_GPIO_FUNCTION__FONA_RING_INDICATOR__CTR  "Fona RI"




#define D_GPIO_FUNCTION_BUILTIN_LED1_CTR              "Builtin LED 1" // only enabled if available and not being used by another pin


#define D_GPIO_FUNCTION_LED1_CTR              D_GPIO_FUNCTION_LED_CTR "1"
#define D_GPIO_FUNCTION_LED2_CTR              D_GPIO_FUNCTION_LED_CTR "2"
#define D_GPIO_FUNCTION_LED3_CTR              D_GPIO_FUNCTION_LED_CTR "3"
#define D_GPIO_FUNCTION_LED4_CTR              D_GPIO_FUNCTION_LED_CTR "4"
#define D_GPIO_FUNCTION_LED5_CTR              D_GPIO_FUNCTION_LED_CTR "5"
#define D_GPIO_FUNCTION_LED1_INV_CTR           D_GPIO_FUNCTION_LED_CTR "1" " Inv"
#define D_GPIO_FUNCTION_LED2_INV_CTR           D_GPIO_FUNCTION_LED_CTR "2" " Inv"
#define D_GPIO_FUNCTION_LED3_INV_CTR           D_GPIO_FUNCTION_LED_CTR "3" " Inv"
#define D_GPIO_FUNCTION_LED4_INV_CTR           D_GPIO_FUNCTION_LED_CTR "4" " Inv"
#define D_GPIO_FUNCTION_LED5_INV_CTR           D_GPIO_FUNCTION_LED_CTR "5" " Inv"
#define D_GPIO_FUNCTION_PWM1_CTR           "PWM1"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM2_CTR           "PWM2"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM3_CTR           "PWM3"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM4_CTR           "PWM4"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM5_CTR           "PWM5"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM1_INV_CTR           "PWM1_INV"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM2_INV_CTR           "PWM2_INV"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM3_INV_CTR           "PWM3_INV"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM4_INV_CTR           "PWM4_INV"        // Suffix "1"
#define D_GPIO_FUNCTION_PWM5_INV_CTR           "PWM5_INV"        // Suffix "1"
#define D_GPIO_FUNCTION_TXD_CTR            "Serial Tx"
#define D_GPIO_FUNCTION_COUNTER_CTR       "Counter"    // Suffix "1"
#define D_GPIO_FUNCTION_IRRECV_CTR        "IRrecv"
#define D_SENSOR_MHZ_RX_CTR        "MHZ Rx"
#define D_SENSOR_MHZ_TX_CTR        "MHZ Tx"
#define D_GPIO_FUNCTION_PZEM004_RX_CTR    "PZEM004 Rx"
#define D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR    "PZEM0XX_MODBUS_ Rx"
#define D_GPIO_FUNCTION_PZEM017_RX_CTR    "PZEM017 Rx"
#define D_GPIO_FUNCTION_PZEM0XX_TX_CTR    "PZEM0XX Tx"
#define D_SENSOR_SPI_CS_CTR        "SPI CS"
#define D_SENSOR_SPI_DC_CTR        "SPI DC"
#define D_SENSOR_BACKLIGHT_CTR     "BkLight"
#define D_SENSOR_PMS5003_CTR       "PMS5003"
#define D_GPIO_FUNCTION_SDS0X1_RX_CTR      "SDS0X1 Rx"
#define D_GPIO_FUNCTION_SDS0X1_TX_CTR      "SDS0X1 Tx"
#define D_GPIO_FUNCTION_SR04_ECHO_CTR      "SR04 Ech"
#define D_GPIO_FUNCTION_SR04_TRIG_CTR      "SR04 Trig"
#define D_GPIO_FUNCTION__ROTARY_ENCODER_A__CTR      "REnc A"
#define D_GPIO_FUNCTION__ROTARY_ENCODER_B__CTR      "REnc B"
#define D_GPIO_FUNCTION_NEXTION_RX_CTR     "Nextion Rx"
#define D_GPIO_FUNCTION_NEXTION_TX_CTR     "Nextion Tx"
#define D_GPIO_FUNCTION_KEY1_CTR        "Key1"
#define D_GPIO_FUNCTION_KEY1_INV_CTR        "Key1 Inv"
#define D_GPIO_FUNCTION_KEY2_CTR        "Key2"
#define D_GPIO_FUNCTION_KEY2_INV_CTR        "Key2 Inv"

#define D_GPIO_FUNCTION_KEY3_CTR        "Key3"
#define D_GPIO_FUNCTION_KEY3_INV_CTR        "Key3 Inv"
#define D_GPIO_FUNCTION_KEY4_CTR        "KeY4"
#define D_GPIO_FUNCTION_KEY4_INV_CTR        "Key4 Inv"
#define D_GPIO_FUNCTION_KEY5_CTR        "Key5"
#define D_GPIO_FUNCTION_KEY5_INV_CTR        "Key5 Inv"
#define D_GPIO_FUNCTION_KEY6_CTR        "Key6"
#define D_GPIO_FUNCTION_KEY6_INV_CTR        "Key6 Inv"
#define D_GPIO_FUNCTION_KEY7_CTR        "Key7"
#define D_GPIO_FUNCTION_KEY7_INV_CTR        "Key7 Inv"
#define D_GPIO_FUNCTION_KEY8_CTR        "Key8"
#define D_GPIO_FUNCTION_KEY8_INV_CTR        "Key8 Inv"

#define D_GPIO_FUNCTION__RF_433MHZ_RX__CTR "RF 433 RX"
#define D_GPIO_FUNCTION__RF_433MHZ_TX__CTR "RF 433 TX"

#define D_GPIO_FUNCTION_KEY1_NP_CTR        "Key1 NP"

#define D_GPIO_FUNCTION_KEY1_PULLDOWN_CTR        "Key1 PD" //active when high. Only available on GPIO16/D0 of esp8266


#define D_SENSOR_SDM120_TX_CTR     "SDMx20 Tx"
#define D_SENSOR_SDM120_RX_CTR     "SDMx20 Rx"
#define D_SENSOR_SDM630_TX_CTR     "SDM630 Tx"
#define D_SENSOR_SDM630_RX_CTR     "SDM630 Rx"
#define D_SENSOR_TM1638_CLK_CTR    "TM16 CLK"
#define D_SENSOR_TM1638_DIO_CTR    "TM16 DIO"
#define D_SENSOR_TM1638_STB_CTR    "TM16 STB"
#define D_SENSOR_HX711_SCK_CTR     "HX711 SCK"
#define D_SENSOR_HX711_DAT_CTR     "HX711 DAT"
#define D_SENSOR_TX20_TX_CTR       "TX20"
#define D_SENSOR_RFSEND_CTR        "RFSend"
#define D_SENSOR_RFRECV_CTR        "RFrecv"
#define D_SENSOR_TUYA_TX_CTR       "Tuya Tx"
#define D_SENSOR_TUYA_RX_CTR       "Tuya Rx"
#define D_SENSOR_MGC3130_XFER_CTR  "MGC3130 Xfr"
#define D_SENSOR_MGC3130_RESET_CTR "MGC3130 Rst"
#define D_SENSOR_SSPI_MISO_CTR     "SSPI MISO"
#define D_SENSOR_SSPI_MOSI_CTR     "SSPI MOSI"
#define D_SENSOR_SSPI_SCLK_CTR     "SSPI SCLK"
#define D_SENSOR_SSPI_CS_CTR       "SSPI CS"
#define D_SENSOR_SSPI_DC_CTR       "SSPI DC"
#define D_SENSOR_RF_SENSOR_CTR     "RF Sensor"
#define D_SENSOR_AZ_RX_CTR         "AZ Rx"
#define D_SENSOR_AZ_TX_CTR         "AZ Tx"
#define D_SENSOR_MAX31855_CS_CTR   "MX31855 CS"
#define D_SENSOR_MAX31855_CLK_CTR  "MX31855 CLK"
#define D_SENSOR_MAX31855_DO_CTR   "MX31855 DO"
#define D_SENSOR_HLW_CF_CTR        "HLW8012 CF"
#define D_SENSOR_HJL_CF_CTR        "BL0937 CF"
#define D_SENSOR_MCP39F5_TX_CTR    "MCP39F5 Tx"
#define D_SENSOR_MCP39F5_RX_CTR    "MCP39F5 Rx"
#define D_SENSOR_MCP39F5_RST_CTR   "MCP39F5 Rst"
#define D_SENSOR_CSE7766_TX_CTR    "CSE7766 Tx"
#define D_SENSOR_CSE7766_RX_CTR    "CSE7766 Rx"
#define D_SENSOR_PN532_TX_CTR      "PN532 Tx"
#define D_SENSOR_PN532_RX_CTR      "PN532 Rx"
#define D_SENSOR_SM16716_CLK_CTR   "SM16716 CLK"
#define D_SENSOR_SM16716_DAT_CTR   "SM16716 DAT"
#define D_SENSOR_SM16716_POWER_CTR "SM16716 PWR"
#define D_SENSOR_MY92X1_DI_CTR     "MY92x1 DI"
#define D_SENSOR_MY92X1_DCKI_CTR   "MY92x1 DCKI"
#define D_SENSOR_ARIRFRCV_CTR      "ALux IrRcv"
#define D_SENSOR_TXD_CTR           "Serial Tx"
#define D_SENSOR_RXD_CTR           "Serial Rx"
#define D_SENSOR_ROTARY_CTR        "Rotary"
#define D_SENSOR_HRE_CLOCK_CTR     "HRE Clock"
#define D_SENSOR_HRE_DATA_CTR      "HRE Data"
#define D_SENSOR_ADE7953_IRQ_CTR   "ADE7953 IRQ"
#define D_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR     "HBRIDGE L9110 Input A"
#define D_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR     "HBRIDGE L9110 Input B"
#define D_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR     "HBRIDGE L9110 Output A"
#define D_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR     "HBRIDGE L9110 Output B"
#define D_GPIO_FUNCTION_ANALOG_POSITION_CTR "Analog Position"
#define D_GPIO_FUNCTION_FAN_IRSEND_CTR "Fan IRSend"
#define D_GPIO_FUNCTION_ADE7953_IRQ_CTR "ADE7953_IRQ"
#define D_GPIO_FUNCTION_ANALOG_CTR "Analog"


#define D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR "HWSerial1 RingBuffer Rx"
#define D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR "HWSerial1 RingBuffer Tx"

#define D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR "HWSerial2 RingBuffer Rx"
#define D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR "HWSerial2 RingBuffer Tx"

#define D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR  "SDCard HSPI CSO"
#define D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR  "SDCard HSPI CLK"
#define D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR "SDCard HSPI MOSI"
#define D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR "SDCard HSPI MISO"


#define D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR  "SDCard VSPI CSO"
#define D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR  "SDCard VSPI CLK"
#define D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR "SDCard VSPI MOSI"
#define D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR "SDCard VSPI MISO"



#define D_GPIO_FUNCTION_ADC1_CH1_CTR "ADC1_CH1"
#define D_GPIO_FUNCTION_ADC1_CH2_CTR "ADC1_CH2"
#define D_GPIO_FUNCTION_ADC1_CH3_CTR "ADC1_CH3"
#define D_GPIO_FUNCTION_ADC1_CH4_CTR "ADC1_CH4_GPIO32"
#define D_GPIO_FUNCTION_ADC1_CH5_CTR "ADC1_CH5"
#define D_GPIO_FUNCTION_ADC1_CH6_CTR "ADC1_CH6"
#define D_GPIO_FUNCTION_ADC1_CH7_CTR "ADC1_CH7_GPIO35"
#define D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR "ADC1_Ext_Int"

/**
 * Special cases 
 * */
#define D_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_CTR "RXON_SAMPLING_ENABLED"
#define D_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_CTR "CC1110_SYNC_PULSE"


// Special generic pins that can be used as special functions eg logger or debuggers
#define D_GPIO_FUNCTION_GENERIC_GPIO1_CTR "Generic GPIO1"

// Debug pins
#define D_GPIO_FUNCTION_DEBUG_PIN1_CTR "Debug Pin1"
#define D_GPIO_FUNCTION_DEBUG_PIN2_CTR "Debug Pin2"
#define D_GPIO_FUNCTION_DEBUG_PIN3_CTR "Debug Pin3"


#define D_GPIO_WEBCAM_DATA1_CTR     "Cam Data1"
#define D_GPIO_WEBCAM_DATA2_CTR     "Cam Data2"
#define D_GPIO_WEBCAM_DATA3_CTR     "Cam Data3"
#define D_GPIO_WEBCAM_DATA4_CTR     "Cam Data4"
#define D_GPIO_WEBCAM_DATA5_CTR     "Cam Data5"
#define D_GPIO_WEBCAM_DATA6_CTR     "Cam Data6"
#define D_GPIO_WEBCAM_DATA7_CTR     "Cam Data7"
#define D_GPIO_WEBCAM_DATA8_CTR     "Cam Data8"
#define D_GPIO_WEBCAM_XCLK_CTR      "Cam XCLK"
#define D_GPIO_WEBCAM_PCLK_CTR      "Cam PCLK"
#define D_GPIO_WEBCAM_HREF_CTR      "Cam HREF"
#define D_GPIO_WEBCAM_VSYNC_CTR     "Cam VSYNC"
#define D_GPIO_WEBCAM_SIOD_CTR      "Cam SIOD"
#define D_GPIO_WEBCAM_SIOC_CTR      "Cam SIOC"
#define D_GPIO_WEBCAM_PWDN_CTR      "Cam PWDN"

/**
 *  Units
 * */
#define D_UNIT_AMPERE "A"
#define D_UNIT_CENTIMETER "cm"
#define D_UNIT_HERTZ "Hz"
#define D_UNIT_HOUR "Hr"
#define D_UNIT_GALLONS "gal"
#define D_UNIT_GALLONS_PER_MIN "g/m"
#define D_UNIT_INCREMENTS "inc"
#define D_UNIT_KILOGRAM "kg"
#define D_UNIT_KILOMETER_PER_HOUR "km/h"  // or "km/h"
#define D_UNIT_KILOOHM "kOhm"
#define D_UNIT_KILOWATTHOUR "kWh"
#define D_UNIT_LUX "lx"
#define D_UNIT_MICROGRAM_PER_CUBIC_METER "ug/m3"
#define D_UNIT_MICROMETER "um"
#define D_UNIT_MICROSECOND "us"
#define D_UNIT_MILLIAMPERE "mA"
#define D_UNIT_MILLIMETER "mm"
#define D_UNIT_MILLIMETER_MERCURY "mmHg"
#define D_UNIT_MILLISECOND "ms"
#define D_UNIT_MINUTE "Min"
#define D_UNIT_PARTS_PER_BILLION "ppb"
#define D_UNIT_PARTS_PER_DECILITER "ppd"
#define D_UNIT_PARTS_PER_MILLION "ppm"
#define D_UNIT_PRESSURE "hPa"
#define D_UNIT_SECOND "sec"
#define D_UNIT_SECTORS "sectors"
#define D_UNIT_VA "VA"
#define D_UNIT_VAR "VAr"
#define D_UNIT_VOLT "V"
#define D_UNIT_WATT "W"
#define D_UNIT_WATTHOUR "Wh"
#define D_UNIT_WATT_METER_QUADRAT "W/m²"

#define D_CMND_RESET "CommandReset"

/**
 *  Log message prefix
 * */
#define D_LOG_APPLICATION "APP: "  // Application
#define D_LOG_BRIDGE "BRG: "       // Bridge
#define D_LOG_BUTTONS "BTN: "
#define D_LOG_CONFIG "CFG: "       // Settings
#define D_LOG_POINTERCONFIG "PCFG: "       // Settings
#define D_LOG_COMMAND "CMD: "      // Command
#define D_LOG_DEBUG "DBG: "        // Debug
#define D_LOG_DHT "DHT: "          // DHT sensor
#define D_LOG_DSB "DSB: "          // DS18xB20 sensor
#define D_LOG_GARAGE "GAR: "         // relays
#define D_LOG_HTTP "HTP: "         // HTTP webserver
#define D_LOG_I2C "I2C: "          // I2C
#define D_LOG_IRR "IRR: "          // Infra Red Received
#define D_LOG_LOG "LOG: "          // Logging
#define D_LOG_MODULE "MOD: "       // Module
#define D_LOG_MDNS "DNS: "         // mDNS
#define D_LOG_MQTT "MQT: "         // MQTT
#define D_LOG_PUBSUB "PSB: "         // PubSub
#define D_LOG_OTHER "OTH: "        // Other
#define D_LOG_RESULT "RSL: "       // Result
#define D_LOG_RFR "RFR: "          // RF Received
#define D_LOG_SERIAL "SER: "       // Serial
#define D_LOG_SHT1 "SHT: "         // SHT1x sensor
#define D_LOG_UPLOAD "UPL: "       // Upload
#define D_LOG_UPNP "UPP: "         // UPnP
#define D_LOG_WIFI "WIF: "         // Wifi
#define D_LOG_NEO "NEO: "         // Neopixels
#define D_LOG_LIGHT "LGT: "         // Light interface
#define D_LOG_DOORBELL "DRB: "         // Neopixels
#define D_LOG_RELAYS "RLY: "         // relays
#define D_LOG_TIME "TIM: "         // Time
#define D_LOG_UPTIME "UPT: "         // Uptime
#define D_LOG_DHT "DHT: "         // Uptime
#define D_LOG_BME "BME: "         // Uptime
#define D_LOG_PZEM "PZM: "         // Uptime
#define D_LOG_SETTINGS "SET: "         // Uptime
#define D_LOG_ULTRASONIC "ULT: "         // Uptime
#define D_LOG_DB18 "DB8: "         // Uptime
#define D_LOG_HEATINGPANEL "HPL: "         // Uptime
#define D_LOG_HEATING "HET: "         // Uptime
#define D_LOG_PIR "PIR: "         // PIR
#define D_LOG_CEILINGFAN "CFN: "         // Ceiling Fan
#define D_LOG_RESPONSE "RES: "
#define D_LOG_CLASSLIST "CLL: "
#define D_LOG_NEXTION "NEX: "
#define D_LOG_CHIME "CHM: "         // Uptime
#define D_LOG_BLINDS "BLD: " 
#define D_LOG_MEMORY "MEM: "       // 
#define D_LOG_ASYNC "ASC: "
#define D_LOG_TEST "TST: "
#define D_LOG_SDCARD "SDC: "
#define D_LOG_SWITCHES "SWH: "
#define D_LOG_RULES "RUL: "
#define D_LOG_SR04 "SR4: "
#define D_LOG_FONA "FON: "
#define D_LOG_BH1750 "BH1: "

//SDM220
#define D_PHASE_ANGLE     "Phase Angle"
#define D_IMPORT_ACTIVE   "Import Active"
#define D_EXPORT_ACTIVE   "Export Active"
#define D_IMPORT_REACTIVE "Import Reactive"
#define D_EXPORT_REACTIVE "Export Reactive"
#define D_TOTAL_REACTIVE  "Total Reactive"
#define D_UNIT_KWARH      "kVArh"
#define D_UNIT_ANGLE      "Deg"


/*********************************************************************************************\
 * All text used in Commands
\*********************************************************************************************/

#define D_JSON_ABORTED "Aborted"
#define D_JSON_ACTIVE "Active"
#define D_JSON_ACCURACY "Accuracy"
#define D_JSON_ADDRESS "Address"
#define D_JSON_ADDLOG "AddLog"
#define D_JSON_AIRQUALITY "AirQuality"
#define D_JSON_ANALOG_INPUT "Analog"
#define D_JSON_AP "AP"                   // Access Point
#define D_JSON_APMAC_ADDRESS "APMac"
#define D_JSON_APPENDED "Appended"
#define D_JSON_BAUDRATE "Baudrate"
#define D_JSON_BLINK "Blink"
#define D_JSON_BLOCKED_LOOP "Blocked Loop"
#define D_JSON_BOOTVERSION "Boot"
#define D_JSON_BOOTCOUNT "BootCount"
#define D_JSON_BOOTCOUNTERRORS "BootCountErrors"
#define D_JSON_BROKERHOSTNAME "BrokerHostname"
#define D_JSON_BSSID "BSSId"
#define D_JSON_BUILDDATETIME "BuildDateTime"
#define D_JSON_BUILDDATE "BuildDate"
#define D_JSON_BUILDTIME "BuildTime"
#define D_JSON_CHANNEL "Channel"
#define D_JSON_CO2 "CarbonDioxide"
#define D_JSON_COMMAND "Command"
#define D_JSON_CLEARLOG "ClearLog"
#define D_JSON_SETTINGS_HOLDER "SettingsHolder"
#define D_JSON_CONNECT_FAILED "Connect failed"
#define D_JSON_CONNECTCOUNT "ConnectCount"
#define D_JSON_CONNECTS "Connects"
#define D_JSON_COREVERSION "Core"
#define D_JSON_COUNT "Count"
#define D_JSON_COUNTER "Counter"
#define D_JSON_CHANNELCOUNT "ChannelCount"
// #define D_JSON_DATA "Data"
#define D_JSON_DISTANCE "Distance"

#define D_JSON_DISPLAY "Display"
#define D_JSON_DISPLAY_MODE "DisplayMode"



#define D_JSON_DNSSERVER "DNSServer"
#define D_JSON_DONE "Done"
#define D_JSON_DOWNTIME "Downtime"
#define D_JSON_DOWNSECS "DownSecs"
#define D_JSON_DRIVERS "Drivers"
#define D_JSON_ECO2 "eCO2"
#define D_JSON_EMPTY "Empty"
#define D_JSON_EVENT "Event"
#define D_JSON_MQTT_ENABLE_RESTART "MQTT Enable Restart"
#define D_JSON_ENDDST "EndDST"           // End Daylight Savings Time
#define D_JSON_ERASE "Erase"
#define D_JSON_EVERY "Every"
#define D_JSON_EXPORT_ACTIVE "ExportActivePower"
#define D_JSON_EXPORT_REACTIVE "ExportReactivePower"
#define D_JSON_FAILED "Failed"
#define D_JSON_FALLBACKTOPIC "FallbackTopic"
#define D_JSON_FEATURES "Features"
#define D_JSON_FLASHCHIPID "FlashChipId"
#define D_JSON_FLASHMODE "FlashMode"
#define D_JSON_FLASHSIZE "FlashSize"
#define D_JSON_FLOWRATE "FlowRate"
#define D_JSON_FREEMEMORY "FreeMemory"
#define D_JSON_FREESKETCHSPACE "FreeSketchSpace"
#define D_JSON_FREEHEAP     "FreeHeap"
#define D_JSON_FREQUENCY "Frequency"
#define D_JSON_FROM "from"
#define D_JSON_GAS "Gas"
#define D_JSON_GATEWAY "Gateway"
#define D_JSON_GROUP "Group"
#define D_JSON_GROUPTOPIC "GroupTopic"
#define D_JSON_HEAPSIZE "HeapSize"
#define D_JSON_HIGH "High"
#define D_JSON_HOST_NOT_FOUND "Host not found"
#define D_JSON_HSBCOLOR "HSBColor"
#define D_JSON_HUMIDITY "Humidity"
#define D_JSON_I2CSCAN_DEVICES_FOUND_AT "Device(s) found at"
#define D_JSON_I2CSCAN_UNKNOWN_ERROR_AT "Unknown error at"
#define D_JSON_I2CSCAN_NO_DEVICES_FOUND "No devices found"
#define D_JSON_ID "Id"
#define D_JSON_ILLUMINANCE "Illuminance"
#define D_JSON_IMPORT_ACTIVE "ImportActivePower"
#define D_JSON_IMPORT_REACTIVE "ImportReactivePower"
#define D_JSON_INFRARED "Infrared"
#define D_JSON_INSIDE "Inside"
#define D_JSON_INSTANT "Instant"
#define D_JSON_UNKNOWN "Unknown"
#define D_JSON_LIGHT "Light"
#define D_JSON_LIGHTPOWER "LightPower"
#define D_JSON_LINKCOUNT "LinkCount"
#define D_JSON_LEVEL "Level"
#define D_JSON_LOCAL_TIME "Local"
#define D_JSON_LOGLEVEL_SERIAL "LogLevel_Serial"
#define D_JSON_LOGLEVELS "LogLevels"
#define D_JSON_LOADAVERAGE "LoadAverage"
#define D_JSON_LOCATION "Location"
#define D_JSON_LOOPDELAY "LoopDelay"
#define D_JSON_LOOPSSEC "LoopsSec"
#define D_JSON_LOOPRATIO "LoopRatio"
#define D_JSON_LIGHT_LEVEL "Light Level"
#define D_JSON_LIGHT_LUMINANCE_LUX "Lux"
#define D_JSON_LOW "Low"
#define D_JSON_MAC "Mac"
#define D_JSON_MASK "Mask"
#define D_JSON_MDNS "MDNS"
#define D_JSON_MESSAGE "Message"
#define D_JSON_MINIMAL "minimal"
#define D_JSON_MODEL "Model"
#define D_JSON_MQTT "MQTT"
#define D_JSON_MQTT_COUNT "MqttCount"
#define D_JSON_MILLIS "millis"
#define D_JSON_NO "No"
#define D_JSON_NOISE "Noise"
#define D_JSON_NONE "None"
#define D_JSON_NETWORK "Network"
#define D_JSON_NEGATIVE "Negative"
#define D_JSON_NARROW "Narrow"
#define D_JSON_OR "or"
#define D_JSON_OTAURL "OTAURL"
#define D_JSON_OPEN "Open"
#define D_JSON_OPENED "Opened"
#define D_JSON_OUTSIDE "Outside"
#define D_JSON_PERIOD "Period"
#define D_JSON_PERCENTAGE "Percentage"
#define D_JSON_PERCENT "Percent"
#define D_JSON_PHASE_ANGLE "PhaseAngle"
#define D_JSON_POWERFACTOR "PowerFactor"
#define D_JSON_POWERUSAGE "Power"
#define D_JSON_POSITIVE "Positive"
#define D_JSON_ACTIVE_POWERUSAGE "ActivePower"
#define D_JSON_APPARENT_POWERUSAGE "ApparentPower"
#define D_JSON_REACTIVE_POWERUSAGE "ReactivePower"
#define D_JSON_PRESSURE "Pressure"
#define D_JSON_PRESSUREATSEALEVEL "SeaPressure"
#define D_JSON_PRESSURE_UNIT "PressureUnit"
#define D_JSON_PROBETEMPERATURE "ProbeTemperature"
#define D_JSON_PROGRAMFLASHSIZE "ProgramFlashSize"
#define D_JSON_PROGRAMSIZE "ProgramSize"
#define D_JSON_REFERENCETEMPERATURE "ReferenceTemperature"
#define D_JSON_RELATIVE "Relative"
#define D_JSON_RESET "Reset"
#define D_JSON_RESOLUTION "Resolution"
#define D_JSON_RESTARTING "Restarting"
#define D_JSON_RESTARTREASON "RestartReason"
#define D_JSON_RSSI "RSSI"
#define D_JSON_SAVEADDRESS "SaveAddress"
#define D_JSON_SAVECOUNT "SaveCount"
#define D_JSON_SAVED "Saved"
#define D_JSON_SAVESTATE "SaveState"
#define D_JSON_SDKVERSION "SDK"
#define D_JSON_SELECTED "selected"
#define D_JSON_SERIAL "Serial"
#define D_JSON_SLEEP "Sleep"
#define D_JSON_SLEEPMODE "SleepMode"
#define D_JSON_SENSORS "Sensors"
#define D_JSON_SENSORCOUNT "SensorCount"
#define D_JSON_SENTCOUNT "SentCount"
#define D_JSON_SENTPERMINUTE "SentPerMinute"
#define D_JSON_SERIALRECEIVED "SerialReceived"
#define D_JSON_SSID "SSID"
#define D_JSON_STARTDST "StartDST"       // Start Daylight Savings Time
#define D_JSON_STARTED "Started"
#define D_JSON_STARTUPUTC "StartupUTC"
#define D_JSON_SMOOTH "Smooth"

#define D_JSON_MOTION_STARTED "MotionStarted"
#define D_JSON_MOTION_ENDED "MotionEnded"

#define D_JSON_SPEEDOFSOUND "SpeedOfSound"

#define D_JSON_STATIC_IPADDRESS "StaticIPAddress"
#define D_JSON_SUBNETMASK "Subnetmask"
#define D_JSON_SUCCESSFUL "Successful"
#define D_JSON_SUNRISE "Sunrise"
#define D_JSON_SUNSET "Sunset"
#define D_JSON_SWITCH "Switch"
#define D_JSON_SYNC "Sync"
#define D_JSON_SYSTEM "System"
#define D_JSON_SYSTEM_RESTART "SystemRestart"
#define D_JSON_SUN_AZIMUTH "Sun Azimuth"
#define D_JSON_SUN_ELEVATION "Sun Elevation"
#define D_JSON_TEMPERATURE "Temperature"
#define D_JSON_TEMPERATURE_UNIT "TempUnit"
#define D_JSON_TEMPLATE_USED "TemplateUsed"
#define D_JSON_THRESHOLD "Threshold"
#define D_JSON_TEMPLATE_PARSE_SUCCESS "TemplateParseSucess"
#define D_JSON_TELNET "Telnet"
#define D_JSON_TELNET_PORT "TelnetPort"
#define D_JSON_TEMPLATE "Template"
#define D_JSON_TIMEZONE "TimeZone"
#define D_JSON_TODAY "Today"
#define D_JSON_TOTAL "Total"
#define D_JSON_TOTAL_USAGE "TotalUsage"
#define D_JSON_TOTAL_REACTIVE "TotalReactivePower"
#define D_JSON_TOTAL_START_TIME "TotalStartTime"
#define D_JSON_TVOC "TVOC"
#define D_JSON_UPTIME "Uptime"
#define D_JSON_UPSECONDS "UpSeconds"
#define D_JSON_UPPER "Upper"
#define D_JSON_UTC_TIME "UTC"
#define D_JSON_UV_INDEX "UvIndex"
#define D_JSON_UV_INDEX_TEXT "UvIndexText"
#define D_JSON_UV_LEVEL "UvLevel"
#define D_JSON_UV_POWER "UvPower"
#define D_JSON_VCC "Vcc"
#define D_JSON_VERSION "Version"
#define D_JSON_VERSIONNAME "VersionName"
#define D_JSON_VERSIONNUMBER "VersionNumber"
#define D_JSON_VERSIONNUMBER_MINIMUM "VersionNumberMininum"
#define D_JSON_VOLTAGE "Voltage"
#define D_JSON_WEIGHT "Weight"
#define D_JSON_WEB "Web"
#define D_JSON_WIFI "Wifi"
#define D_JSON_WIDE "Wide"
#define D_JSON_WRONG "Wrong"
#define D_JSON_WRONG_PARAMETERS "Wrong parameters"
#define D_JSON_WEBSERVER_ENABLED "Webserver Enabled"
#define D_JSON_WIFICONFIG_STATE "WiFiConfig State"
#define D_JSON_YESTERDAY "Yesterday"
#define D_JSON_ZERO_POINT_CALIBRATION "Zero Point Calibration"
#define D_JSON_ALTITUDE "altitude"
#define D_JSON_AGE "age"
#define D_JSON_ANY "Any"
#define D_JSON_ACTIVE_POWER "ActivePower"
#define D_JSON_CCT_PERCENTAGE "CCT_TempPercentage"
#define D_JSON_CCT_TEMP "CCT_Temp"

#define D_JSON_RGBCCT_LINKED "rgbcct_linked"

#define D_JSON_BRIGHTNESS "Brightness"
#define D_JSON_BRIGHTNESS_255 "Brightness_255"
#define D_JSON_BRIGHTNESS_PERCENTAGE "brightness_percentage"
#define D_JSON_CLIMATE "climate"
#define D_JSON_CLASSID "ClassID"
#define D_JSON_CLOSED "Closed"

#define D_JSON_CURRENT "Current"
#define D_JSON_CRASHDUMP "CrashDump"
#define D_JSON_COLOUR_PALETTE "ColourPalette"
#define D_JSON_DATA "Data"
#define D_JSON_DATETIME "DateTime"
#define D_JSON_DESIRED "Desired"
#define D_JSON_DEVICE "Device"
#define D_JSON_DEVICEID "DeviceID"
#define D_JSON_DEVICES_CONNECTED "DevicesConnected"

#define D_JSON_SCENE "Scene"
#define D_JSON_COLOUR "Colour"

#define D_JSON_SENSOR_TYPE "SensorType"



#define D_JSON_HVAC_DEVICE "HeatingDevice"

#define D_JSON_DEVIATION "Deviation"

#define D_JSON_DEVICENAME "DeviceName"
#define D_JSON_DEVICEFRIENDLYNAME "DeviceFriendlyName"

#define D_JSON_DOOR "Door"
#define D_JSON_RINGDOORBELL "DoorbellRing"
#define D_JSON_DURATION "Duration"
#define D_JSON_DURATION_RAW "DurationRaw"
#define D_JSON_ERROR "Error"
#define D_JSON_EFFECTS "Effects"
#define D_JSON_EFFECT "Effect"
#define D_JSON_FUNCTION "Function"

#define D_JSON_ENABLED "Enabled"

#define D_JSON_PIXELGROUPING "PixelGrouping"
#define D_JSON_MULTIPLIER "Multiplier"

#define D_JSON_BRIGHTNESS_MIN "BrightnessMin"
#define D_JSON_BRIGHTNESS_MAX "BrightnessMax"
#define D_JSON_BRIGHTNESS_RGB "BrightnessRGB"
#define D_JSON_BRIGHTNESS_RGB_255 "BrightnessRGB_255"
#define D_JSON_BRIGHTNESS_CCT  "BrightnessCCT"
#define D_JSON_BRIGHTNESS_CCT_255  "BrightnessCCT_255"
#define D_JSON_RANDOM_AMOUNT "RandomAmount"
#define D_JSON_RANDOMISE_BRIGHTNESS_MODE "RandomiseBrightnessMode"

#define D_JSON_RUNNING_ID "RunningID"

#define D_JSON_SEQUENTIAL "Sequential"
#define D_JSON_DIRECTION "Direction"

#define D_RGB_DATA_STREAM "RGBDataStream"

#define D_JSON_ALTERNATE "Alternate"
#define D_JSON_ALTERNATE_AMOUNT "AlternateAmount"
#define D_JSON_ALTERNATE_BRIGHTNESS_MIN "AlternateBrightnessMin"
#define D_JSON_ALTERNATE_BRIGHTNESS_MAX "AlternateBrightnessMax"

#define D_JSON_COLOUR_REFRESH_RATE "ColourRefreshRate"

#define D_JSON_RUNTIME_DURATION_SCALER_PERCENTAGE "run_time_duration_scaler_as_percentage"

#define D_JSON_FAILSAFE "failsafe"
#define D_JSON_FUNCTIONS "functions"
#define D_JSON_HUM "hum"
#define D_JSON_HUE "Hue"
#define D_JSON_INDEX "Index"
#define D_JSON_ISCHANGED "ischanged"
#define D_JSON_ISCHANGEDMETHOD "IsChangedMethod"
#define D_JSON_ISRUNNING "isrunning"
#define D_JSON_ISVALID "IsValid"
#define D_JSON_CAPTURE_UPSECONDS "CaptureUpSeconds"
#define D_JSON_IMPORTANCE "importance"
#define D_JSON_INORDER "InOrder"
#define D_JSON_LIMIT "Limit"
#define D_JSON_LASTREAD "LastRead"
#define D_JSON_LENGTH "length"
#define D_JSON_MAX "max"
#define D_JSON_MESSAGE "Message"
#define D_JSON_METHOD "method"
#define D_JSON_MINUTES "minutes"
#define D_JSON_MIN "min"
#define D_JSON_MODE "Mode"
#define D_JSON_MIXER "Mixer"
#define D_JSON_MOTION "Motion"
#define D_JSON_MODULENAME "ModuleName"
#define D_JSON_MODULEID "ModuleID"
#define D_JSON_MINUTES_ON "minutes_on"
#define D_JSON_NAME "Name"

#define D_JSON_MEMORY_INSUFFICIENT "Memory Insufficient"

#define D_JSON_SCENENAME "SceneName"
#define D_JSON_ANIMATIONMODE "AnimationMode"

#define D_JSON_MAPPED_MULTIPLIER_DATA "MappedMultiplierData"

#define D_JSON_NOTIFICATIONS "Notifications"

#define D_JSON_POWERNAME "PowerName"
#define D_JSON_RELAY "Relay"

#define D_JSON_ANIMATIONENABLE "AnimationEnable"
#define D_JSON_AGED_COLOURING "AgedColouring"

#define D_JSON_ON "On"
// #define D_JSON_ONTIME "ontime"
#define D_JSON_ONOFF "OnOff"
#define D_JSON_ONOFF_NAME "OnOffName"
#define D_JSON_OFF "Off"
// #define D_JSON_OFFTIME "offtime"
#define D_JSON_ORDER "Order"
#define D_JSON_OUTLIERS "Outliers"
#define D_JSON_PACKETS "Packets"
#define D_JSON_PIXELSGROUPED "pixels_grouped"

#define D_JSON_LAST "Last"
#define D_JSON_ONTIME "OnTime"
#define D_JSON_OFFTIME "OffTime"

#define D_JSON_PIXELS_UPDATE_NUMBER "PixelUpdateNum"
#define D_JSON_PIXELS_UPDATE_PERCENTAGE "PixelUpdatePerc"

#define D_JSON_POWER_FACTOR "PowerFactor"

#define D_JSON_PIXELNUM "pixelnum"
#define D_JSON_PULSER "pulser"
#define D_JSON_REFRESH_ALLPIXELS "refresh_allpixels"

#define D_JSON_RATE "Rate"
#define D_JSON_RATE_MS "RateMs"
#define D_JSON_RATE_SECS "RateSecs"


#define D_JSON_RECEIVEDCOUNT "ReceivedCount"
#define D_JSON_RATIO "Ratio"

#define D_JSON_RELAY_NAME "Relay Name"

#define D_JSON_ROOMHINT "RoomHint"

#define D_JSON_MOTION_NAME "Motion Name"

#define D_JSON_SAT "Sat"
#define D_JSON_SAT255 "Sat255"
#define D_JSON_SECONDS "seconds"
#define D_JSON_SET "Set"
#define D_JSON_SENS1 "sens1"
#define D_JSON_SENSORADDRESS "SensorAddress"
#define D_JSON_SECS "Secs"
#define D_JSON_SIZE "size"
#define D_JSON_SIGNIFICANTLY "significantly"
#define D_JSON_STEP "step"
#define D_JSON_STATUS "Status"

#define D_JSON_SENSOR_MEASURERATE "Sensor_MeasureRate"

#define D_JSON_SCENE "Scene"
#define D_JSON_SCENE_COLOUR "SceneColour"
#define D_JSON_SCENE_NAME   "SceneName"

#define D_JSON_SCHEDULE "Schedule"

#define D_JSON_SCHEDULED_OFF D_OFF
#define D_JSON_SCHEDULED_SET "Scheduled Set"
#define D_JSON_SCHEDULED_ON "Scheduled ON"
#define D_JSON_MANUAL_ON "Manual ON"

#define D_JSON_TIME "Time"
#define D_JSON_TIME_MS "TimeMs"
#define D_JSON_TIME_SECS "TimeSecs"
#define D_JSON_TIME_ON "TimeOn"
#define D_JSON_TIME_ON_SECS "TimeOnSecs"
#define D_JSON_TIME_OFF_THEN_ON_SECS "TimeOffThenOnSecs"
#define D_JSON_TIME_ON_MINUTES "TimeOnMins"
#define D_JSON_TIME_ON_MS "TimeOnMs"
#define D_JSON_TIME_DELAYED "TimeDelayed"
#define D_JSON_TIME_MAINTAINING "TimeMaintaining"
#define D_JSON_TIME_RUNNING "TimeRunning"
#define D_JSON_TIME_TO_HEAT "TimeToHeat"
#define D_JSON_TEMP "Temp"
#define D_JSON_TRANSITION "Transition"
#define D_JSON_TYPE "Type"
#define D_JSON_RELAY "Relay"

#define D_JSON_SUBTYPE "SubType"

#define D_JSON_MANUAL_SETPIXEL_TO_SCENE "ManualSetPixelToScene"

#define D_JSON_TIME_SCALER_AS_PERCENTAGE "TimeMultiplierAsPerc"

#define D_JSON_EXTERNAL_POWER_ONOFF "ExternalPowerOnOff"



#define D_JSON_RGB "RGB"
#define D_JSON_RBG "RBG"
#define D_JSON_GRB "GRB"

#define D_JSON_HSB "HSB"

#define D_JSON_RANDOM "Random"
#define D_JSON_VALUE "Value"


#define D_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR "GRB"

#define D_JSON_PALETTE_EDIT "PaletteEdit"
#define D_JSON_PALETTE_GENERATION "PaletteGeneration"

#define D_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR "RGB"

#define D_JSON_COLOUR_TYPE "ColourType"


#define D_JSON_HARDWARE_TYPE "HardwareType"
#define D_JSON_STRIP_SIZE "LightSize"
#define D_JSON_RGB_COLOUR_ORDER "ColourOrder"
#define D_JSON_RGB_CLOCK "RGBClock"
#define D_JSON_MANUAL_NUMBER "ManualNumber"
#define D_JSON_STRIP_REPEAT "strip_repeat"

#define D_JSON_WS2812       "WS2812"



#define D_RSLT_ENERGY "ENERGY"
#define D_RSLT_HASS_STATE "HASS_STATE"
#define D_RSLT_INFO "INFO"
#define D_RSLT_MARGINS "MARGINS"
#define D_RSLT_POWER "POWER"
#define D_RSLT_RESULT "RESULT"
#define D_RSLT_SENSOR "SENSOR"
#define D_RSLT_STATE "STATE"
#define D_RSLT_UPTIME "UPTIME"
#define D_RSLT_WARNING "WARNING"

// Commands sonoff.ino
#define D_JSON_BACKLOG "Backlog"
#define D_JSON_DELAY "Delay"
#define D_JSON_STATUS "Status"
#define D_JSON_STATE "State"
#define D_JSON_POWER "Power"
#define D_JSON_FANSPEED "FanSpeed"
#define D_JSON_FANSPEED_MANUAL "FanSpeed_Manual"
#define D_JSON_FANPWM "FanPWM"
#define D_JSON_POWERONSTATE "PowerOnState"
#define D_JSON_POWER_STATE "PowerState"
#define D_JSON_POWER_STATE_NAME "PowerStateName"
#define D_JSON_PULSETIME "PulseTime"
#define D_JSON_BLINKTIME "BlinkTime"
#define D_JSON_BLINKCOUNT "BlinkCount"
#define D_JSON_SENSOR "Sensor"
#define D_JSON_SENSOR_COUNT "SensorCount"
#define D_JSON_DRIVER "Driver"
#define D_JSON_SAVEDATA "SaveData"
#define D_JSON_SETOPTION "SetOption"
#define D_JSON_TEMPERATURE_RESOLUTION "TempRes"
#define D_JSON_HUMIDITY_RESOLUTION "HumRes"
#define D_JSON_PRESSURE_RESOLUTION "PressRes"
#define D_JSON_POWER_RESOLUTION "WattRes"
#define D_JSON_VOLTAGE_RESOLUTION "VoltRes"
#define D_JSON_FREQUENCY_RESOLUTION "FreqRes"
#define D_JSON_CURRENT_RESOLUTION "AmpRes"
#define D_JSON_ENERGY_RESOLUTION "EnergyRes"
#define D_JSON_WEIGHT_RESOLUTION "WeightRes"
#define D_JSON_MODULE "Module"
#define D_JSON_MODULES "Modules"
  #define D_JSON_NOT_SUPPORTED "Not supported"
#define D_JSON_GPIOS "GPIOs"
#define D_JSON_PWM "PWM"
#define D_JSON_PWMFREQUENCY "PWMFrequency"
#define D_JSON_PWMRANGE "PWMRange"
#define D_JSON_COUNTER "Counter"
#define D_JSON_COUNTERTYPE "CounterType"
#define D_JSON_COUNTERDEBOUNCE "CounterDebounce"
#define D_JSON_BUTTONDEBOUNCE "ButtonDebounce"
#define D_JSON_SWITCHDEBOUNCE "SwitchDebounce"
#define D_JSON_SLEEP "Sleep"
#define D_JSON_UPLOAD "Upload"
#define D_JSON_UPGRADE "Upgrade"
  #define D_JSON_ONE_OR_GT "1 or >%s to upgrade"
// #define D_JSON_OTAURL "OtaUrl"
#define D_JSON_SERIALLOG "SerialLog"
#define D_JSON_SYSLOG "SysLog"
#define D_JSON_LOGHOST "LogHost"
#define D_JSON_LOGPORT "LogPort"
#define D_JSON_LOWER "Lower"
#define D_JSON_IPADDRESS "IPAddress"
#define D_JSON_NTPSERVER "NtpServer"
// #define D_JSON_AP "Ap"
#define D_JSON_SSID "SSID"
#define D_JSON_SSID_NUMBERED "SSID_Number"
#define D_JSON_PASSWORD "Password"
#define D_JSON_HOSTNAME "Hostname"
#define D_JSON_WIFICONFIG "WifiConfig"
  #define WCFG_MAX_STRING_LENGTH 12
  #define D_WCFG_0_RESTART "Restart"
  #define D_WCFG_1_SMARTCONFIG "SmartConfig"
  #define D_WCFG_2_WIFIMANAGER "WifiManager"
  #define D_WCFG_3_WPSCONFIG "WPSConfig"
  #define D_WCFG_4_RETRY "Retry"
  #define D_WCFG_5_WAIT "Wait"
  #define D_WCFG_6_SERIAL "Serial"
  #define D_WCFG_7_WIFIMANAGER_RESET_ONLY "ManagerRst"
  
#define D_JSON_WIFIPOWER "WifiPower"

#define D_JSON_WIFI_AP "WifiAP"

#define D_JSON_ROOMHINT "RoomHint"

#define D_JSON_FRIENDLYNAME "FriendlyName"
#define D_JSON_SWITCHMODE "SwitchMode"
#define D_JSON_INTERLOCK "Interlock"
#define D_JSON_TELEPERIOD "TelePeriod"
#define D_JSON_RESTART "Restart"
  #define D_JSON_ONE_TO_RESTART "1 to restart"
#define D_JSON_RESET "Reset"
  #define D_JSON_RESET_AND_RESTARTING "Reset and Restarting"
  #define D_JSON_ONE_TO_RESET "1 to reset"
// #define D_JSON_TIMEZONE "Timezone"
#define D_JSON_TIMESTD "TimeStd"
#define D_JSON_TIMEDST "TimeDst"
// #define D_JSON_ALTITUDE "Altitude"
#define D_JSON_LEDPOWER "LedPower"
#define D_JSON_LEDSTATE "LedState"
#define D_JSON_LEDMASK "LedMask"
#define D_JSON_I2CSCAN "I2CScan"
#define D_JSON_SERIALSEND "SerialSend"
#define D_JSON_SERIALDELIMITER "SerialDelimiter"
#define D_JSON_BAUDRATE "Baudrate"
#define D_JSON_TEMPLATE "Template"
  #define D_JSON_GPIO "GPIO"
  #define D_JSON_GPIOC "GPIOC"
  #define D_JSON_FLAG "FLAG"
#define D_JSON_BASE "Base"

// Commands xdrv_01_mqtt.ino
#define D_JSON_MQTTHOST "MqttHost"
#define D_JSON_MQTTPORT "MqttPort"
#define D_JSON_MQTTRETRY "MqttRetry"
#define D_JSON_STATETEXT "StateText"
#define D_JSON_MQTTFINGERPRINT "MqttFingerprint"
#define D_JSON_MQTTCLIENT "MqttClient"
#define D_JSON_MQTTUSER "MqttUser"
#define D_JSON_MQTTPASSWORD "MqttPassword"
#define D_JSON_FULLTOPIC "FullTopic"
#define D_JSON_GROUPTOPIC "GroupTopic"
#define D_JSON_TOPIC "Topic"
#define D_JSON_BUTTONTOPIC "ButtonTopic"
#define D_JSON_SWITCHTOPIC "SwitchTopic"
#define D_JSON_BUTTONRETAIN "ButtonRetain"
#define D_JSON_SWITCHRETAIN "SwitchRetain"
#define D_JSON_POWERRETAIN "PowerRetain"
#define D_JSON_SENSORRETAIN "SensorRetain"
#define D_JSON_PUBLISH "Publish"

// Commands xdrv_02_webserver.ino
#define D_JSON_WEBSERVER "Webserver"
  #define D_JSON_WEBSERVER_MODE "WebServerMode"
  #define D_JSON_ACTIVE_FOR "Active for"
  #define D_JSON_ON_DEVICE "on"
  #define D_JSON_WITH_IP_ADDRESS "with IP address"
#define D_JSON_WEBPASSWORD "WebPassword"
#define D_JSON_WEBLOG "WebLog"
#define D_JSON_WEBREFRESH "WebRefresh"
#define D_JSON_WEBSEND "WebSend"
#define D_JSON_WEBCOLOR  "WebColor"
#define D_JSON_EMULATION "Emulation"

// Commands xdrv_03_energy.ino
#define D_JSON_POWERLOW "PowerLow"
#define D_JSON_POWERHIGH "PowerHigh"
#define D_JSON_POWERDELTA "PowerDelta"
#define D_JSON_VOLTAGELOW "VoltageLow"
#define D_JSON_VOLTAGEHIGH "VoltageHigh"
#define D_JSON_CURRENTLOW "CurrentLow"
#define D_JSON_CURRENTHIGH "CurrentHigh"
#define D_JSON_ENERGYRESET "EnergyReset"
#define D_JSON_POWERSET "PowerSet"
#define D_JSON_VOLTAGESET "VoltageSet"
#define D_JSON_CURRENTSET "CurrentSet"
#define D_JSON_FREQUENCYSET "FrequencySet"
#define D_JSON_MAXPOWER "MaxPower"
#define D_JSON_MAXPOWERHOLD "MaxPowerHold"
#define D_JSON_MAXPOWERWINDOW "MaxPowerWindow"
  #define D_JSON_MAXPOWERREACHED "MaxPowerReached"
  #define D_JSON_MAXPOWERREACHEDRETRY "MaxPowerReachedRetry"
#define D_JSON_SAFEPOWER "SafePower"
#define D_JSON_SAFEPOWERHOLD "SafePowerHold"
#define D_JSON_SAFEPOWERWINDOW "SafePowerWindow"
  #define D_JSON_POWERMONITOR "PowerMonitor"
#define D_JSON_MAXENERGY "MaxEnergy"
#define D_JSON_MAXENERGYSTART "MaxEnergyStart"
  #define D_JSON_ENERGYMONITOR "EnergyMonitor"
  #define D_JSON_ENERGY "Energy"
  #define D_JSON_ENERGYLAST "EnergyLast"
  #define D_JSON_MAXENERGYREACHED "MaxEnergyReached"

  #define D_JSON_KWH_STATS "kwh_stats"

// Commands xdrv_04_light.ino
#define D_JSON_CHANNEL "Channel"
#define D_JSON_COLOR "Color"
#define D_JSON_COLORTEMPERATURE "CT"
#define D_JSON_DIMMER "Dimmer"
#define D_JSON_HSBCOLOR "HSBColor"
#define D_JSON_LED "Led"
#define D_JSON_LEDTABLE "LedTable"
#define D_JSON_FADE "Fade"
#define D_JSON_PIXELS "Pixels"
#define D_JSON_RGBWWTABLE "RGBWWTable"
#define D_JSON_ROTATION "Rotation"
#define D_JSON_SCHEME "Scheme"
#define D_JSON_SPEED "Speed"
#define D_JSON_WAKEUP "Wakeup"
#define D_JSON_WAKEUPDURATION "WakeUpDuration"
#define D_JSON_WHITE "White"
#define D_JSON_WIDTH "Width"

// Commands xdrv_05_irremote.ino
#define D_JSON_IRSEND "IRSend"
  #define D_JSON_INVALID_JSON "Invalid JSON"
  #define D_JSON_INVALID_RAWDATA "Invalid RawData"
  #define D_JSON_NO_BUFFER_SPACE "No buffer space"
  #define D_JSON_PROTOCOL_NOT_SUPPORTED "Protocol not supported"
  #define D_JSON_IR_PROTOCOL "Protocol"
  #define D_JSON_IR_BITS "Bits"
  #define D_JSON_IR_DATA "Data"
  #define D_JSON_IR_RAWDATA "RawData"
#define D_JSON_IRHVAC "IRHVAC"
  #define D_JSON_IRHVAC_VENDOR "VENDOR"
  #define D_JSON_IRHVAC_POWER "POWER"
  #define D_JSON_IRHVAC_MODE "MODE"
  #define D_JSON_IRHVAC_FANSPEED "FANSPEED"
  #define D_JSON_IRHVAC_TEMP "TEMP"
#define D_JSON_IRRECEIVED "IrReceived"

// Commands xdrv_06_snfbridge.ino
#define D_JSON_RFCODE "RfCode"
#define D_JSON_RFHIGH "RfHigh"
#define D_JSON_RFHOST "RfHost"
#define D_JSON_RFKEY "RfKey"
  #define D_JSON_START_LEARNING "Start learning"
  #define D_JSON_SET_TO_DEFAULT "Set to default"
  #define D_JSON_DEFAULT_SENT "Default sent"
  #define D_JSON_LEARNED_SENT "Learned sent"
  #define D_JSON_LEARNING_ACTIVE "Learning active"
  #define D_JSON_LEARN_FAILED "Learn failed"
  #define D_JSON_LEARNED "Learned"
#define D_JSON_RFLOW "RfLow"
#define D_JSON_RFSYNC "RfSync"
  #define D_JSON_RFRECEIVED "RfReceived"
#define D_JSON_RFRAW "RfRaw"

// Commands xdrv_08_serial_bridge.ino
#define D_JSON_SSERIALSEND "SSerialSend"
#define D_JSON_SBAUDRATE "SBaudrate"
  #define D_JSON_SSERIALRECEIVED "SSerialReceived"

// Commands xdrv_09_timers.ino
#define D_JSON_TIMER "Timer"
  #define D_JSON_TIMER_ARM "Arm"
  #define D_JSON_TIMER_MODE "Mode"
  #define D_JSON_TIMER_TIME "Time"
  #define D_JSON_TIMER_WINDOW "Window"
  #define D_JSON_TIMER_DAYS "Days"
  #define D_JSON_TIMER_REPEAT "Repeat"
  #define D_JSON_TIMER_OUTPUT "Output"
  #define D_JSON_TIMER_ACTION "Action"
  #define D_JSON_TIMER_NO_DEVICE "No GPIO as output configured"
#define D_JSON_TIMERS "Timers"
#define D_JSON_LATITUDE "Latitude"
#define D_JSON_LONGITUDE "Longitude"

#define D_JSON_DESERIALIZATION_ERROR "DeserializationError"

  

/********************************************************************************************/

#define D_ASTERIX "********"
#define D_ASTERISK_PWD "****"

#ifndef MY_LANGUAGE
  //#include "//2_CoreSystem/11_Languages/mLanguage.h"
#else
  #define QUOTEME(x) QUOTEME_1(x)
  #define QUOTEME_1(x) #x
  #define INCLUDE_FILE(x) QUOTEME(language/x.h)
  #include INCLUDE_FILE(MY_LANGUAGE)
#endif


#endif  // _I18N_H_
