#include "2_CoreSystem/06_Support/mSupport.h"

/**
 * 
 * System level commands to sort through
 * 
 * */



// const char kTasmotaCommands[] PROGMEM = "|"  // No prefix
//   D_JSON_BACKLOG "|" D_JSON_DELAY "|" D_JSON_POWER "|" D_JSON_STATUS "|" D_JSON_STATE "|" D_JSON_SLEEP "|" D_JSON_UPGRADE "|" D_JSON_UPLOAD "|" D_JSON_OTAURL "|"
//   D_JSON_SERIALLOG "|" D_JSON_RESTART "|" D_JSON_POWERONSTATE "|" D_JSON_PULSETIME "|" D_JSON_BLINKTIME "|" D_JSON_BLINKCOUNT "|" D_JSON_SAVEDATA "|"
//   D_JSON_SO "|" D_JSON_SETOPTION "|" D_JSON_TEMPERATURE_RESOLUTION "|" D_JSON_HUMIDITY_RESOLUTION "|" D_JSON_PRESSURE_RESOLUTION "|" D_JSON_POWER_RESOLUTION "|"
//   D_JSON_VOLTAGE_RESOLUTION "|" D_JSON_FREQUENCY_RESOLUTION "|" D_JSON_CURRENT_RESOLUTION "|" D_JSON_ENERGY_RESOLUTION "|" D_JSON_WEIGHT_RESOLUTION "|"
//   D_JSON_MODULE "|" D_JSON_MODULES "|" D_JSON_GPIO "|" D_JSON_GPIOS "|" D_JSON_TEMPLATE "|" D_JSON_PWM "|" D_JSON_PWMFREQUENCY "|" D_JSON_PWMRANGE "|"
//   D_JSON_BUTTONDEBOUNCE "|" D_JSON_SWITCHDEBOUNCE "|" D_JSON_SYSLOG "|" D_JSON_LOGHOST "|" D_JSON_LOGPORT "|" D_JSON_SERIALSEND "|" D_JSON_BAUDRATE "|" D_JSON_SERIALCONFIG "|"
//   D_JSON_SERIALDELIMITER "|" D_JSON_IPADDRESS "|" D_JSON_NTPSERVER "|" D_JSON_AP "|" D_JSON_SSID "|" D_JSON_PASSWORD "|" D_JSON_HOSTNAME "|" D_JSON_WIFICONFIG "|"
//   D_JSON_DEVICENAME "|" D_JSON_FRIENDLYNAME "|" D_JSON_SWITCHMODE "|" D_JSON_INTERLOCK "|" D_JSON_TELEPERIOD "|" D_JSON_RESET "|" D_JSON_TIME "|" D_JSON_TIMEZONE "|" D_JSON_TIMESTD "|"
//   D_JSON_TIMEDST "|" D_JSON_ALTITUDE "|" D_JSON_LEDPOWER "|" D_JSON_LEDSTATE "|" D_JSON_LEDMASK "|" D_JSON_LEDPWM_ON "|" D_JSON_LEDPWM_OFF "|" D_JSON_LEDPWM_MODE "|"
//   D_JSON_WIFIPOWER "|" D_JSON_TEMPOFFSET "|" D_JSON_HUMOFFSET "|" D_JSON_SPEEDUNIT "|" D_JSON_GLOBAL_TEMP "|" D_JSON_GLOBAL_HUM "|" D_JSON_WIFI "|"
// #ifdef USE_I2C
//   D_JSON_I2CSCAN "|" D_JSON_I2CDRIVER "|"
// #endif
// #ifdef USE_DEVICE_GROUPS
//   D_JSON_DEVGROUP_NAME "|"
// #ifdef USE_DEVICE_GROUPS_SEND
//   D_JSON_DEVGROUP_SEND "|"
// #endif  // USE_DEVICE_GROUPS_SEND
//   D_JSON_DEVGROUP_SHARE "|" D_JSON_DEVGROUPSTATUS "|"
// #endif  // USE_DEVICE_GROUPS
//   D_JSON_SENSOR "|" D_JSON_DRIVER
// #ifdef ESP32
//    "|" D_JSON_TOUCH_CAL "|" D_JSON_TOUCH_THRES "|" D_JSON_TOUCH_NUM "|" D_JSON_CPU_FREQUENCY
// #endif //ESP32
//   ;

// void (* const TasmotaCommand[])(void) PROGMEM = {
//   &CmndBacklog, &CmndDelay, &CmndPower, &CmndStatus, &CmndState, &CmndSleep, &CmndUpgrade, &CmndUpgrade, &CmndOtaUrl,
//   &CmndSeriallog, &CmndRestart, &CmndPowerOnState, &CmndPulsetime, &CmndBlinktime, &CmndBlinkcount, &CmndSavedata,
//   &CmndSetoption, &CmndSetoption, &CmndTemperatureResolution, &CmndHumidityResolution, &CmndPressureResolution, &CmndPowerResolution,
//   &CmndVoltageResolution, &CmndFrequencyResolution, &CmndCurrentResolution, &CmndEnergyResolution, &CmndWeightResolution,
//   &CmndModule, &CmndModules, &CmndGpio, &CmndGpios, &CmndTemplate, &CmndPwm, &CmndPwmfrequency, &CmndPwmrange,
//   &CmndButtonDebounce, &CmndSwitchDebounce, &CmndSyslog, &CmndLoghost, &CmndLogport, &CmndSerialSend, &CmndBaudrate, &CmndSerialConfig,
//   &CmndSerialDelimiter, &CmndIpAddress, &CmndNtpServer, &CmndAp, &CmndSsid, &CmndPassword, &CmndHostname, &CmndWifiConfig,
//   &CmndDevicename, &CmndFriendlyname, &CmndSwitchMode, &CmndInterlock, &CmndTeleperiod, &CmndReset, &CmndTime, &CmndTimezone, &CmndTimeStd,
//   &CmndTimeDst, &CmndAltitude, &CmndLedPower, &CmndLedState, &CmndLedMask, &CmndLedPwmOn, &CmndLedPwmOff, &CmndLedPwmMode,
//   &CmndWifiPower, &CmndTempOffset, &CmndHumOffset, &CmndSpeedUnit, &CmndGlobalTemp, &CmndGlobalHum, &CmndWifi,
// #ifdef USE_I2C
//   &CmndI2cScan, CmndI2cDriver,
// #endif
// #ifdef USE_DEVICE_GROUPS
//   &CmndDevGroupName,
// #ifdef USE_DEVICE_GROUPS_SEND
//   &CmndDevGroupSend,
// #endif  // USE_DEVICE_GROUPS_SEND
//   &CmndDevGroupShare, &CmndDevGroupStatus,
// #endif  // USE_DEVICE_GROUPS
//   &CmndSensor, &CmndDriver
// #ifdef ESP32
//   ,&CmndTouchCal, &CmndTouchThres, &CmndTouchNum, &CmndCpuFrequency
// #endif //ESP32
//   };

// const char kWifiConfig[] PROGMEM =
//   D_WCFG_0_RESTART "||" D_WCFG_2_WIFIMANAGER "||" D_WCFG_4_RETRY "|" D_WCFG_5_WAIT "|" D_WCFG_6_SERIAL "|" D_WCFG_7_WIFIMANAGER_RESET_ONLY;

// /********************************************************************************************/

// void ResponseCmndNumber(int value)
// {
//   Response_P(S_JSON_COMMAND_NVALUE, XdrvMailbox.command, value);
// }

// void ResponseCmndFloat(float value, uint32_t decimals)
// {
//   char stemp1[TOPSZ];
//   dtostrfd(value, decimals, stemp1);
//   Response_P(S_JSON_COMMAND_XVALUE, XdrvMailbox.command, stemp1);  // Return float value without quotes
// }

// void ResponseCmndIdxNumber(int value)
// {
//   Response_P(S_JSON_COMMAND_INDEX_NVALUE, XdrvMailbox.command, XdrvMailbox.index, value);
// }

// void ResponseCmndChar_P(const char* value)
// {
//   Response_P(S_JSON_COMMAND_SVALUE, XdrvMailbox.command, value);
// }

// void ResponseCmndChar(const char* value)
// {
//   Response_P(S_JSON_COMMAND_SVALUE, XdrvMailbox.command, EscapeJSONString(value).c_str());
// }

// void ResponseCmndStateText(uint32_t value)
// {
//   ResponseCmndChar(GetStateText(value));
// }

// void ResponseCmndDone(void)
// {
//   ResponseCmndChar(D_JSON_DONE);
// }

// void ResponseCmndIdxChar(const char* value)
// {
//   Response_P(S_JSON_COMMAND_INDEX_SVALUE, XdrvMailbox.command, XdrvMailbox.index, EscapeJSONString(value).c_str());
// }

// void ResponseCmndAll(uint32_t text_index, uint32_t count)
// {
//   uint32_t real_index = text_index;
//   mqtt_data[0] = '\0';
//   for (uint32_t i = 0; i < count; i++) {
//     if ((SET_MQTT_GRP_TOPIC == text_index) && (1 == i)) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
//     ResponseAppend_P(PSTR("%c\"%s%d\":\"%s\""), (i) ? ',' : '{', XdrvMailbox.command, i +1, EscapeJSONString(SettingsText(real_index +i)).c_str());
//   }
//   ResponseJsonEnd();
// }

// /********************************************************************************************/

// void ExecuteCommand(const char *cmnd, uint32_t source)
// {
//   // cmnd: "status 0"  = stopic "status" and svalue " 0"
//   // cmnd: "var1 =1"   = stopic "var1" and svalue " =1"
//   // cmnd: "var1=1"    = stopic "var1" and svalue "=1"
// #ifdef USE_DEBUG_DRIVER
//   ShowFreeMem(PSTR("ExecuteCommand"));
// #endif
//   ShowSource(source);

//   const char *pos = cmnd;
//   while (*pos && isspace(*pos)) {
//     pos++;                         // Skip all spaces
//   }

//   const char *start = pos;
//   // Get a command. Commands can only use letters, digits and underscores
//   while (*pos && (isalpha(*pos) || isdigit(*pos) || '_' == *pos || '/' == *pos)) {
//     if ('/' == *pos) {            // Skip possible cmnd/tasmota/ preamble
//       start = pos + 1;
//     }
//     pos++;
//   }
//   if ('\0' == *start || pos <= start) {
//     return;                      // Did not find any command to execute
//   }

//   uint32_t size = pos - start;
//   char stopic[size + 2];         // with leader '/' and end '\0'
//   stopic[0] = '/';
//   memcpy(stopic+1, start, size);
//   stopic[size+1] = '\0';

//   char svalue[strlen(pos) +1];   // pos point to the start of parameters
//   strlcpy(svalue, pos, sizeof(svalue));
//   CommandHandler(stopic, svalue, strlen(svalue));
// }

// /********************************************************************************************/

// // topicBuf:                    /power1  dataBuf: toggle  = Console command
// // topicBuf:        cmnd/tasmota/power1  dataBuf: toggle  = Mqtt command using topic
// // topicBuf:       cmnd/tasmotas/power1  dataBuf: toggle  = Mqtt command using a group topic
// // topicBuf: cmnd/DVES_83BB10_fb/power1  dataBuf: toggle  = Mqtt command using fallback topic

// void CommandHandler(char* topicBuf, char* dataBuf, uint32_t data_len)
// {
// #ifdef USE_DEBUG_DRIVER
//   ShowFreeMem(PSTR("CommandHandler"));
// #endif

//   while (*dataBuf && isspace(*dataBuf)) {
//     dataBuf++;                           // Skip leading spaces in data
//     data_len--;
//   }

//   bool grpflg = false;
//   uint32_t real_index = SET_MQTT_GRP_TOPIC;
//   for (uint32_t i = 0; i < MAX_GROUP_TOPICS; i++) {
//     if (1 == i) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
//     char *group_topic = SettingsText(real_index +i);
//     if (*group_topic && strstr(topicBuf, group_topic) != nullptr) {
//       grpflg = true;
//       break;
//     }
//   }

//   char stemp1[TOPSZ];
//   GetFallbackTopic_P(stemp1, "");  // Full Fallback topic = cmnd/DVES_xxxxxxxx_fb/
//   fallback_topic_flag = (!strncmp(topicBuf, stemp1, strlen(stemp1)));

//   char *type = strrchr(topicBuf, '/');   // Last part of received topic is always the command (type)

//   uint32_t index = 1;
//   bool user_index = false;
//   if (type != nullptr) {
//     type++;
//     uint32_t i;
//     int nLen; // strlen(type)
//     char *s = type;
//     for (nLen = 0; *s; s++, nLen++) {
//       *s=toupper(*s);
//     }
//     i = nLen;
//     if (i > 0) { // may be 0
//       while (isdigit(type[i-1])) {
//         i--;
//       }
//     }
//     if (i < nLen) {
//       index = atoi(type + i);
//       user_index = true;
//     }
//     type[i] = '\0';
//   }

//   AddLog_P2(LOG_LEVEL_DEBUG, PSTR("CMD: " D_GROUP " %d, " D_INDEX " %d, " D_COMMAND " \"%s\", " D_DATA " \"%s\""), grpflg, index, type, dataBuf);

//   if (type != nullptr) {
//     Response_P(PSTR("{\"" D_JSON_COMMAND "\":\"" D_JSON_ERROR "\"}"));

//     if (Settings.ledstate &0x02) { blinks++; }

//     if (!strcmp(dataBuf,"?")) { data_len = 0; }

//     char *p;
//     int32_t payload = strtol(dataBuf, &p, 0);  // decimal, octal (0) or hex (0x)
//     if (p == dataBuf) { payload = -99; }
//     int temp_payload = GetStateNumber(dataBuf);
//     if (temp_payload > -1) { payload = temp_payload; }

//     DEBUG_CORE_LOG(PSTR("CMD: Payload %d"), payload);

// //    backlog_delay = millis() + (100 * MIN_BACKLOG_DELAY);
//     backlog_delay = millis() + Settings.setoption_255[P_BACKLOG_DELAY];

//     char command[CMDSZ] = { 0 };
//     XdrvMailbox.command = command;
//     XdrvMailbox.index = index;
//     XdrvMailbox.data_len = data_len;
//     XdrvMailbox.payload = payload;
//     XdrvMailbox.grpflg = grpflg;
//     XdrvMailbox.usridx = user_index;
//     XdrvMailbox.topic = type;
//     XdrvMailbox.data = dataBuf;

// #ifdef USE_SCRIPT_SUB_COMMAND
//   // allow overwrite tasmota cmds
//     if (!Script_SubCmd()) {
//       if (!DecodeCommand(kTasmotaCommands, TasmotaCommand)) {
//         if (!XdrvCall(FUNC_COMMAND)) {
//           if (!XsnsCall(FUNC_COMMAND)) {
//             type = nullptr;  // Unknown command
//           }
//         }
//       }
//     }
// #else //USE_SCRIPT_SUB_COMMAND
//     if (!DecodeCommand(kTasmotaCommands, TasmotaCommand)) {
//       if (!XdrvCall(FUNC_COMMAND)) {
//         if (!XsnsCall(FUNC_COMMAND)) {
//           type = nullptr;  // Unknown command
//         }
//       }
//     }
// #endif //USE_SCRIPT_SUB_COMMAND

//   }

//   if (type == nullptr) {
//     blinks = 201;
//     snprintf_P(stemp1, sizeof(stemp1), PSTR(D_JSON_COMMAND));
//     Response_P(PSTR("{\"" D_JSON_COMMAND "\":\"" D_JSON_UNKNOWN "\"}"));
//     type = (char*)stemp1;
//   }

//   if (mqtt_data[0] != '\0') {
//      MqttPublishPrefixTopic_P(RESULT_OR_STAT, type);
//      XdrvRulesProcess();
//   }
//   fallback_topic_flag = false;
// }

// /********************************************************************************************/

// void CmndBacklog(void)
// {
//   if (XdrvMailbox.data_len) {

// #ifdef SUPPORT_IF_STATEMENT
//     char *blcommand = strtok(XdrvMailbox.data, ";");
//     while ((blcommand != nullptr) && (backlog.size() < MAX_BACKLOG))
// #else
//     uint32_t bl_pointer = (!backlog_pointer) ? MAX_BACKLOG -1 : backlog_pointer;
//     bl_pointer--;
//     char *blcommand = strtok(XdrvMailbox.data, ";");
//     while ((blcommand != nullptr) && (backlog_index != bl_pointer))
// #endif
//     {
//       while(true) {
//         blcommand = Trim(blcommand);
//         if (!strncasecmp_P(blcommand, PSTR(D_JSON_BACKLOG), strlen(D_JSON_BACKLOG))) {
//           blcommand += strlen(D_JSON_BACKLOG);                                  // Skip unnecessary command Backlog
//         } else {
//           break;
//         }
//       }
//       if (*blcommand != '\0') {
// #ifdef SUPPORT_IF_STATEMENT
//         if (backlog.size() < MAX_BACKLOG) {
//           backlog.add(blcommand);
//         }
// #else
//         backlog[backlog_index] = String(blcommand);
//         backlog_index++;
//         if (backlog_index >= MAX_BACKLOG) backlog_index = 0;
// #endif
//       }
//       blcommand = strtok(nullptr, ";");
//     }
// //    ResponseCmndChar(D_JSON_APPENDED);
//     mqtt_data[0] = '\0';
//   } else {
//     bool blflag = BACKLOG_EMPTY;
// #ifdef SUPPORT_IF_STATEMENT
//     backlog.clear();
// #else
//     backlog_pointer = backlog_index;
// #endif
//     ResponseCmndChar(blflag ? D_JSON_EMPTY : D_JSON_ABORTED);
//   }
// }

// void CmndDelay(void)
// {
//   if ((XdrvMailbox.payload >= (MIN_BACKLOG_DELAY / 100)) && (XdrvMailbox.payload <= 3600)) {
//     backlog_delay = millis() + (100 * XdrvMailbox.payload);
//   }
//   uint32_t bl_delay = 0;
//   long bl_delta = TimePassedSince(backlog_delay);
//   if (bl_delta < 0) { bl_delay = (bl_delta *-1) / 100; }
//   ResponseCmndNumber(bl_delay);
// }

// void CmndPower(void)
// {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= devices_present)) {
//     if ((XdrvMailbox.payload < POWER_OFF) || (XdrvMailbox.payload > POWER_BLINK_STOP)) {
//       XdrvMailbox.payload = POWER_SHOW_STATE;
//     }
// //      Settings.flag.device_index_enable = XdrvMailbox.usridx;  // SetOption26 - Switch between POWER or POWER1
//     ExecuteCommandPower(XdrvMailbox.index, XdrvMailbox.payload, SRC_IGNORE);
//     mqtt_data[0] = '\0';
//   }
//   else if (0 == XdrvMailbox.index) {
//     if ((XdrvMailbox.payload < POWER_OFF) || (XdrvMailbox.payload > POWER_TOGGLE)) {
//       XdrvMailbox.payload = POWER_SHOW_STATE;
//     }
//     SetAllPower(XdrvMailbox.payload, SRC_IGNORE);
//     mqtt_data[0] = '\0';
//   }
// }

// void CmndStatus(void)
// {
//   uint32_t payload = ((XdrvMailbox.payload < 0) || (XdrvMailbox.payload > MAX_STATUS)) ? 99 : XdrvMailbox.payload;

//   uint32_t option = STAT;
//   char stemp[200];
//   char stemp2[TOPSZ];

//   // Workaround MQTT - TCP/IP stack queueing when SUB_PREFIX = PUB_PREFIX
//   // Commented on 20200118 as it seems to be no longer needed
// //  if (!strcmp(SettingsText(SET_MQTTPREFIX1), SettingsText(SET_MQTTPREFIX2)) && (!payload)) { option++; }  // TELE

//   if ((!Settings.flag.mqtt_enabled) && (6 == payload)) { payload = 99; }  // SetOption3 - Enable MQTT
//   if (!energy_flg && (9 == payload)) { payload = 99; }
//   if (!CrashFlag() && (12 == payload)) { payload = 99; }

//   if ((0 == payload) || (99 == payload)) {
//     uint32_t maxfn = (devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : (!devices_present) ? 1 : devices_present;
// #ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
//     if (IsModuleIfan()) { maxfn = 1; }
// #endif  // USE_MODULE_CONTROLLER_SONOFF_IFAN
//     stemp[0] = '\0';
//     for (uint32_t i = 0; i < maxfn; i++) {
//       snprintf_P(stemp, sizeof(stemp), PSTR("%s%s\"%s\"" ), stemp, (i > 0 ? "," : ""), EscapeJSONString(SettingsText(SET_FRIENDLYNAME1 +i)).c_str());
//     }
//     stemp2[0] = '\0';
//     for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
//       snprintf_P(stemp2, sizeof(stemp2), PSTR("%s%s%d" ), stemp2, (i > 0 ? "," : ""), Settings.switchmode[i]);
//     }
//     Response_P(PSTR("{\"" D_JSON_STATUS "\":{\"" D_JSON_MODULE "\":%d,\"" D_JSON_DEVICENAME "\":\"%s\",\"" D_JSON_FRIENDLYNAME "\":[%s],\"" D_JSON_TOPIC "\":\"%s\",\""
//                           D_JSON_BUTTONTOPIC "\":\"%s\",\"" D_JSON_POWER "\":%d,\"" D_JSON_POWERONSTATE "\":%d,\"" D_JSON_LEDSTATE "\":%d,\""
//                           D_JSON_LEDMASK "\":\"%04X\",\"" D_JSON_SAVEDATA "\":%d,\"" D_JSON_SAVESTATE "\":%d,\"" D_JSON_SWITCHTOPIC "\":\"%s\",\""
//                           D_JSON_SWITCHMODE "\":[%s],\"" D_JSON_BUTTONRETAIN "\":%d,\"" D_JSON_SWITCHRETAIN "\":%d,\"" D_JSON_SENSORRETAIN "\":%d,\"" D_JSON_POWERRETAIN "\":%d}}"),
//                           ModuleNr(), EscapeJSONString(SettingsText(SET_DEVICENAME)).c_str(), stemp, mqtt_topic,
//                           SettingsText(SET_MQTT_BUTTON_TOPIC), power, Settings.poweronstate, Settings.ledstate,
//                           Settings.ledmask, Settings.save_data,
//                           Settings.flag.save_state,           // SetOption0 - Save power state and use after restart
//                           SettingsText(SET_MQTT_SWITCH_TOPIC),
//                           stemp2,
//                           Settings.flag.mqtt_button_retain,   // CMND_BUTTONRETAIN
//                           Settings.flag.mqtt_switch_retain,   // CMND_SWITCHRETAIN
//                           Settings.flag.mqtt_sensor_retain,   // CMND_SENSORRETAIN
//                           Settings.flag.mqtt_power_retain);   // CMND_POWERRETAIN
//     MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS));
//   }

//   if ((0 == payload) || (1 == payload)) {
//     Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS1_PARAMETER "\":{\"" D_JSON_BAUDRATE "\":%d,\"" D_JSON_SERIALCONFIG "\":\"%s\",\"" D_JSON_GROUPTOPIC "\":\"%s\",\"" D_JSON_OTAURL "\":\"%s\",\""
//                           D_JSON_RESTARTREASON "\":\"%s\",\"" D_JSON_UPTIME "\":\"%s\",\"" D_JSON_STARTUPUTC "\":\"%s\",\"" D_JSON_SLEEP "\":%d,\""
//                           D_JSON_SETTINGS_HOLDER "\":%d,\"" D_JSON_BOOTCOUNT "\":%d,\"BCResetTime\":\"%s\",\"" D_JSON_SAVECOUNT "\":%d"
// #ifdef ESP8266
//                           ",\"" D_JSON_SAVEADDRESS "\":\"%X\""
// #endif
//                           "}}"),
//                           Settings.baudrate * 300, GetSerialConfig().c_str(), SettingsText(SET_MQTT_GRP_TOPIC), SettingsText(SET_OTAURL),
//                           GetResetReason().c_str(), GetUptime().c_str(), GetDateAndTimeCtr(DT_RESTART).c_str(), Settings.sleep,
//                           Settings.cfg_holder, Settings.bootcount, GetDateAndTimeCtr(DT_BOOTCOUNT).c_str(), Settings.save_flag
// #ifdef ESP8266
//                           , GetSettingsAddress()
// #endif
//                           );
//     MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "1"));
//   }

//   if ((0 == payload) || (2 == payload)) {
//     Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS2_FIRMWARE "\":{\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_BUILDDATETIME "\":\"%s\""
// #ifdef ESP8266
//                           ",\"" D_JSON_BOOTVERSION "\":%d"
// #endif
//                           ",\"" D_JSON_COREVERSION "\":\"" ARDUINO_CORE_RELEASE "\",\"" D_JSON_SDKVERSION "\":\"%s\","
//                           "\"CpuFrequency\":%d,\"Hardware\":\"%s\""
//                           "%s}}"),
//                           my_version, my_image, GetBuildDateAndTime().c_str()
// #ifdef ESP8266
//                           , ESP.getBootVersion()
// #endif
//                           , ESP.getSdkVersion(),
//                           ESP.getCpuFreqMHz(), GetDeviceHardware().c_str(),
//                           GetStatistics().c_str());
//     MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "2"));
//   }

//   if ((0 == payload) || (3 == payload)) {
//     Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS3_LOGGING "\":{\"" D_JSON_SERIALLOG "\":%d,\"" D_JSON_WEBLOG "\":%d,\"" D_JSON_MQTTLOG "\":%d,\"" D_JSON_SYSLOG "\":%d,\""
//                           D_JSON_LOGHOST "\":\"%s\",\"" D_JSON_LOGPORT "\":%d,\"" D_JSON_SSID "\":[\"%s\",\"%s\"],\"" D_JSON_TELEPERIOD "\":%d,\""
//                           D_JSON_RESOLUTION "\":\"%08X\",\"" D_JSON_SETOPTION "\":[\"%08X\",\"%s\",\"%08X\",\"%08X\"]}}"),
//                           Settings.seriallog_level, Settings.weblog_level, Settings.mqttlog_level, Settings.syslog_level,
//                           SettingsText(SET_SYSLOG_HOST), Settings.syslog_port, EscapeJSONString(SettingsText(SET_STASSID1)).c_str(), EscapeJSONString(SettingsText(SET_STASSID2)).c_str(), Settings.tele_period,
//                           Settings.flag2.data, Settings.flag.data, ToHex_P((unsigned char*)Settings.param, PARAM8_SIZE, stemp2, sizeof(stemp2)),
//                           Settings.flag3.data, Settings.flag4.data);
//     MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "3"));
//   }

//   if ((0 == payload) || (4 == payload)) {
//     Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS4_MEMORY "\":{\"" D_JSON_PROGRAMSIZE "\":%d,\"" D_JSON_FREEMEMORY "\":%d,\"" D_JSON_HEAPSIZE "\":%d,\""
// #ifdef ESP32
//                           D_JSON_PSRMAXMEMORY "\":%d,\"" D_JSON_PSRFREEMEMORY "\":%d,"
// #endif
//                           D_JSON_PROGRAMFLASHSIZE "\":%d,\"" D_JSON_FLASHSIZE "\":%d"
// #ifdef ESP8266
//                           ",\"" D_JSON_FLASHCHIPID "\":\"%06X\""
// #endif
//                           ",\"FlashFrequency\":%d,\"" D_JSON_FLASHMODE "\":%d,\""
//                           D_JSON_FEATURES "\":[\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\",\"%08X\"]"),
//                           ESP_getSketchSize()/1024, ESP.getFreeSketchSpace()/1024, ESP_getFreeHeap()/1024,
// #ifdef ESP32
//                           ESP.getPsramSize()/1024, ESP.getFreePsram()/1024,
// #endif
//                           ESP.getFlashChipSize()/1024, ESP.getFlashChipRealSize()/1024
// #ifdef ESP8266
//                           , ESP.getFlashChipId()
// #endif
//                           , ESP.getFlashChipSpeed()/1000000, ESP.getFlashChipMode(),
//                           LANGUAGE_LCID, feature_drv1, feature_drv2, feature_sns1, feature_sns2, feature5, feature6);
//     XsnsDriverState();
//     ResponseAppend_P(PSTR(",\"Sensors\":"));
//     XsnsSensorState();
//     ResponseJsonEndEnd();
//     MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "4"));
//   }

//   if ((0 == payload) || (5 == payload)) {
//     Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS5_NETWORK "\":{\"" D_JSON_HOSTNAME "\":\"%s\",\"" D_JSON_IPADDRESS "\":\"%s\",\"" D_JSON_GATEWAY "\":\"%s\",\""
//                           D_JSON_SUBNETMASK "\":\"%s\",\"" D_JSON_DNSSERVER "\":\"%s\",\"" D_JSON_MAC "\":\"%s\",\""
//                           D_JSON_WEBSERVER "\":%d,\"" D_JSON_WIFICONFIG "\":%d,\"" D_JSON_WIFIPOWER "\":%s}}"),
//                           NetworkHostname(), NetworkAddress().toString().c_str(), IPAddress(Settings.ip_address[1]).toString().c_str(),
//                           IPAddress(Settings.ip_address[2]).toString().c_str(), IPAddress(Settings.ip_address[3]).toString().c_str(), NetworkMacAddress().c_str(),
//                           Settings.webserver, Settings.sta_config, WifiGetOutputPower().c_str());
//     MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "5"));
//   }

//   if (((0 == payload) || (6 == payload)) && Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
//     Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS6_MQTT "\":{\"" D_JSON_MQTTHOST "\":\"%s\",\"" D_JSON_MQTTPORT "\":%d,\"" D_JSON_MQTTCLIENT D_JSON_MASK "\":\"%s\",\""
//                           D_JSON_MQTTCLIENT "\":\"%s\",\"" D_JSON_MQTTUSER "\":\"%s\",\"" D_JSON_MQTT_COUNT "\":%d,\"MAX_PACKET_SIZE\":%d,\"KEEPALIVE\":%d}}"),
//                           SettingsText(SET_MQTT_HOST), Settings.mqtt_port, EscapeJSONString(SettingsText(SET_MQTT_CLIENT)).c_str(),
//                           mqtt_client, EscapeJSONString(SettingsText(SET_MQTT_USER)).c_str(), MqttConnectCount(), MQTT_MAX_PACKET_SIZE, MQTT_KEEPALIVE);
//     MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "6"));
//   }

//   if ((0 == payload) || (7 == payload)) {
//     if (99 == Settings.timezone) {
//       snprintf_P(stemp, sizeof(stemp), PSTR("%d" ), Settings.timezone);
//     } else {
//       snprintf_P(stemp, sizeof(stemp), PSTR("\"%s\"" ), GetTimeZone().c_str());
//     }
// #if defined(USE_TIMERS) && defined(USE_SUNRISE)
//     Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS7_TIME "\":{\"" D_JSON_UTC_TIME "\":\"%s\",\"" D_JSON_LOCAL_TIME "\":\"%s\",\"" D_JSON_STARTDST "\":\"%s\",\""
//                           D_JSON_ENDDST "\":\"%s\",\"" D_JSON_TIMEZONE "\":%s,\"" D_JSON_SUNRISE "\":\"%s\",\"" D_JSON_SUNSET "\":\"%s\"}}"),
//                           GetDateAndTimeCtr(DT_UTC).c_str(), GetDateAndTimeCtr(DT_LOCALNOTZ).c_str(), GetDateAndTimeCtr(DT_DST).c_str(),
//                           GetDateAndTimeCtr(DT_STD).c_str(), stemp, GetSunTimeAtHorizon(0).c_str(), GetSunTimeAtHorizon(1).c_str());
// #else
//     Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS7_TIME "\":{\"" D_JSON_UTC_TIME "\":\"%s\",\"" D_JSON_LOCAL_TIME "\":\"%s\",\"" D_JSON_STARTDST "\":\"%s\",\""
//                           D_JSON_ENDDST "\":\"%s\",\"" D_JSON_TIMEZONE "\":%s}}"),
//                           GetDateAndTimeCtr(DT_UTC).c_str(), GetDateAndTimeCtr(DT_LOCALNOTZ).c_str(), GetDateAndTimeCtr(DT_DST).c_str(),
//                           GetDateAndTimeCtr(DT_STD).c_str(), stemp);
// #endif  // USE_TIMERS and USE_SUNRISE
//     MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "7"));
//   }

// #if defined(USE_ENERGY_SENSOR) && defined(USE_ENERGY_MARGIN_DETECTION)
//   if (energy_flg) {
//     if ((0 == payload) || (9 == payload)) {
//       Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS9_MARGIN "\":{\"" D_JSON_POWERDELTA "\":%d,\"" D_JSON_POWERLOW "\":%d,\"" D_JSON_POWERHIGH "\":%d,\""
//                             D_JSON_VOLTAGELOW "\":%d,\"" D_JSON_VOLTAGEHIGH "\":%d,\"" D_JSON_CURRENTLOW "\":%d,\"" D_JSON_CURRENTHIGH "\":%d}}"),
//                             Settings.energy_power_delta, Settings.energy_min_power, Settings.energy_max_power,
//                             Settings.energy_min_voltage, Settings.energy_max_voltage, Settings.energy_min_current, Settings.energy_max_current);
//       MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "9"));
//     }
//   }
// #endif  // USE_ENERGY_MARGIN_DETECTION

//   if ((0 == payload) || (8 == payload) || (10 == payload)) {
//     Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS10_SENSOR "\":"));
//     MqttShowSensor();
//     ResponseJsonEnd();
//     if (8 == payload) {
//       MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "8"));
//     } else {
//       MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "10"));
//     }
//   }

//   if ((0 == payload) || (11 == payload)) {
//     Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS11_STATUS "\":"));
//     MqttShowState();
//     ResponseJsonEnd();
//     MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "11"));
//   }

//   if (CrashFlag()) {
//     if ((0 == payload) || (12 == payload)) {
//       Response_P(PSTR("{\"" D_JSON_STATUS D_STATUS12_STATUS "\":"));
//       CrashDump();
//       ResponseJsonEnd();
//       MqttPublishPrefixTopic_P(option, PSTR(D_JSON_STATUS "12"));
//     }
//   }

// #ifdef USE_SCRIPT_STATUS
//   if (bitRead(Settings.rule_enabled, 0)) Run_Scripter(">U",2,mqtt_data);
// #endif

//   if (payload) {
//     XdrvRulesProcess();  // Allow rule processing on single Status command only
//   }

//   mqtt_data[0] = '\0';
// }

// void CmndState(void)
// {
//   mqtt_data[0] = '\0';
//   MqttShowState();
//   if (Settings.flag3.hass_tele_on_power) {  // SetOption59 - Send tele/%topic%/STATE in addition to stat/%topic%/RESULT
//     MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_STATE), MQTT_TELE_RETAIN);
//   }
// #ifdef USE_HOME_ASSISTANT
//   if (Settings.flag.hass_discovery) {       // SetOption19 - Control Home Assistantautomatic discovery (See SetOption59)
//     HAssPublishStatus();
//   }
// #endif  // USE_HOME_ASSISTANT
// }

// void CmndTempOffset(void)
// {
//   if (XdrvMailbox.data_len > 0) {
//     int value = (int)(CharToFloat(XdrvMailbox.data) * 10);
//     if ((value > -127) && (value < 127)) {
//       Settings.temp_comp = value;
//     }
//   }
//   ResponseCmndFloat((float)(Settings.temp_comp) / 10, 1);
// }

// void CmndHumOffset(void)
// {
//   if (XdrvMailbox.data_len > 0) {
//     int value = (int)(CharToFloat(XdrvMailbox.data) * 10);
//     if ((value > -101) && (value < 101)) {
//       Settings.hum_comp = value;
//     }
//   }
//   ResponseCmndFloat((float)(Settings.hum_comp) / 10, 1);
// }

// void CmndGlobalTemp(void)
// {
//   if (XdrvMailbox.data_len > 0) {
//     float temperature = CharToFloat(XdrvMailbox.data);
//     if (!isnan(temperature) && Settings.flag.temperature_conversion) {    // SetOption8 - Switch between Celsius or Fahrenheit
//       temperature = (temperature - 32) / 1.8;                             // Celsius
//     }
//     if ((temperature >= -50.0f) && (temperature <= 100.0f)) {
//       ConvertTemp(temperature);
//       global_update = 1;  // Keep global values just entered valid
//     }
//   }
//   ResponseCmndFloat(global_temperature_celsius, 1);
// }

// void CmndGlobalHum(void)
// {
//   if (XdrvMailbox.data_len > 0) {
//     float humidity = CharToFloat(XdrvMailbox.data);
//     if ((humidity >= 0.0) && (humidity <= 100.0)) {
//       ConvertHumidity(humidity);
//       global_update = 1;  // Keep global values just entered valid
//     }
//   }
//   ResponseCmndFloat(global_humidity, 1);
// }

// void CmndSleep(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 251)) {
//     Settings.sleep = XdrvMailbox.payload;
//     ssleep = XdrvMailbox.payload;
//     WiFiSetSleepMode();
//   }
//   Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, XdrvMailbox.command, Settings.sleep, ssleep);

// }

// void CmndUpgrade(void)
// {
//   // Check if the payload is numerically 1, and had no trailing chars.
//   //   e.g. "1foo" or "1.2.3" could fool us.
//   // Check if the version we have been asked to upgrade to is higher than our current version.
//   //   We also need at least 3 chars to make a valid version number string.
//   if (((1 == XdrvMailbox.data_len) && (1 == XdrvMailbox.payload)) || ((XdrvMailbox.data_len >= 3) && NewerVersion(XdrvMailbox.data))) {
//     ota_state_flag = 3;
//     char stemp1[TOPSZ];
//     Response_P(PSTR("{\"%s\":\"" D_JSON_VERSION " %s " D_JSON_FROM " %s\"}"), XdrvMailbox.command, my_version, GetOtaUrl(stemp1, sizeof(stemp1)));
//   } else {
//     Response_P(PSTR("{\"%s\":\"" D_JSON_ONE_OR_GT "\"}"), XdrvMailbox.command, my_version);
//   }
// }

// void CmndOtaUrl(void)
// {
//   if (XdrvMailbox.data_len > 0) {
//     SettingsUpdateText(SET_OTAURL, (SC_DEFAULT == Shortcut()) ? PSTR(OTA_URL) : XdrvMailbox.data);
//   }
//   ResponseCmndChar(SettingsText(SET_OTAURL));
// }

// void CmndSeriallog(void)
// {
//   if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {
//     Settings.flag.mqtt_serial = 0;       // CMND_SERIALSEND and CMND_SERIALLOG
//     SetSeriallog(XdrvMailbox.payload);
//   }
//   Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, XdrvMailbox.command, Settings.seriallog_level, seriallog_level);
// }

void mSupport::CmndRestart(void)
{




  // switch (XdrvMailbox.payload) {
  // case 1:
  //   restart_flag = 2;
  //   ResponseCmndChar(D_JSON_RESTARTING);
  //   break;
  // case -1:
  //   CmndCrash();    // force a crash
  //   break;
  // case -2:
  //   CmndWDT();
  //   break;
  // case -3:
  //   CmndBlockedLoop();
  //   break;
  // case 99:
  //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_RESTARTING));
  //   EspRestart();
  //   break;
  // default:
  //   ResponseCmndChar_P(PSTR(D_JSON_ONE_TO_RESTART));
  // }
}

// void CmndPowerOnState(void)
// {
// #ifdef ESP8266
//   if (my_module_type != MOTOR)
// #endif  // ESP8266
//   {
//     /* 0 = Keep relays off after power on
//       * 1 = Turn relays on after power on, if PulseTime set wait for PulseTime seconds, and turn relays off
//       * 2 = Toggle relays after power on
//       * 3 = Set relays to last saved state after power on
//       * 4 = Turn relays on and disable any relay control (used for Sonoff Pow to always measure power)
//       * 5 = Keep relays off after power on, if PulseTime set wait for PulseTime seconds, and turn relays on
//       */
//     if ((XdrvMailbox.payload >= POWER_ALL_OFF) && (XdrvMailbox.payload <= POWER_ALL_OFF_PULSETIME_ON)) {
//       Settings.poweronstate = XdrvMailbox.payload;
//       if (POWER_ALL_ALWAYS_ON == Settings.poweronstate) {
//         for (uint32_t i = 1; i <= devices_present; i++) {
//           ExecuteCommandPower(i, POWER_ON, SRC_IGNORE);
//         }
//       }
//     }
//     ResponseCmndNumber(Settings.poweronstate);
//   }
// }

// void CmndPulsetime(void)
// {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_PULSETIMERS)) {
//     uint32_t items = 1;
//     if (!XdrvMailbox.usridx && !XdrvMailbox.data_len) {
//       items = MAX_PULSETIMERS;
//     } else {
//       if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 65536)) {
//         Settings.pulse_timer[XdrvMailbox.index -1] = XdrvMailbox.payload;  // 0 - 65535
//         SetPulseTimer(XdrvMailbox.index -1, XdrvMailbox.payload);
//       }
//     }
//     mqtt_data[0] = '\0';
//     for (uint32_t i = 0; i < items; i++) {
//       uint32_t index = (1 == items) ? XdrvMailbox.index : i +1;
//       ResponseAppend_P(PSTR("%c\"%s%d\":{\"" D_JSON_SET "\":%d,\"" D_JSON_REMAINING "\":%d}"),
//         (i) ? ',' : '{',
//         XdrvMailbox.command, index,
//         Settings.pulse_timer[index -1], GetPulseTimer(index -1));
//     }
//     ResponseJsonEnd();
//   }
// }

// void CmndBlinktime(void)
// {
//   if ((XdrvMailbox.payload > 1) && (XdrvMailbox.payload <= 3600)) {
//     Settings.blinktime = XdrvMailbox.payload;
//     if (blink_timer > 0) { blink_timer = millis() + (100 * XdrvMailbox.payload); }
//   }
//   ResponseCmndNumber(Settings.blinktime);
// }

// void CmndBlinkcount(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 65536)) {
//     Settings.blinkcount = XdrvMailbox.payload;  // 0 - 65535
//     if (blink_counter) { blink_counter = Settings.blinkcount *2; }
//   }
//   ResponseCmndNumber(Settings.blinkcount);
// }

// void CmndSavedata(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3600)) {
//     Settings.save_data = XdrvMailbox.payload;
//     save_data_counter = Settings.save_data;
//   }
//   SettingsSaveAll();
//   char stemp1[TOPSZ];
//   if (Settings.save_data > 1) {
//     snprintf_P(stemp1, sizeof(stemp1), PSTR(D_JSON_EVERY " %d " D_UNIT_SECOND), Settings.save_data);
//   }
//   ResponseCmndChar((Settings.save_data > 1) ? stemp1 : GetStateText(Settings.save_data));
// }

// void CmndSetoption(void)
// {
//   snprintf_P(XdrvMailbox.command, CMDSZ, PSTR(D_JSON_SETOPTION));  // Rename result shortcut command SO to SetOption

//   if (XdrvMailbox.index < 114) {
//     uint32_t ptype;
//     uint32_t pindex;
//     if (XdrvMailbox.index <= 31) {         // SetOption0 .. 31 = Settings.flag
//       ptype = 2;
//       pindex = XdrvMailbox.index;          // 0 .. 31
//     }
//     else if (XdrvMailbox.index <= 49) {    // SetOption32 .. 49 = Settings.param
//       ptype = 1;
//       pindex = XdrvMailbox.index -32;      // 0 .. 17 (= PARAM8_SIZE -1)
//     }
//     else if (XdrvMailbox.index <= 81) {    // SetOption50 .. 81 = Settings.flag3
//       ptype = 3;
//       pindex = XdrvMailbox.index -50;      // 0 .. 31
//     }
//     else {                                 // SetOption82 .. 113 = Settings.flag4
//       ptype = 4;
//       pindex = XdrvMailbox.index -82;      // 0 .. 31
//     }

//     if (XdrvMailbox.payload >= 0) {
//       if (1 == ptype) {                    // SetOption32 .. 49
//         uint32_t param_low = 0;
//         uint32_t param_high = 255;
//         switch (pindex) {
//           case P_HOLD_TIME:
//           case P_MAX_POWER_RETRY:
//             param_low = 1;
//             param_high = 250;
//             break;
//         }
//         if ((XdrvMailbox.payload >= param_low) && (XdrvMailbox.payload <= param_high)) {
//           Settings.setoption_255[pindex] = XdrvMailbox.payload;
// #ifdef USE_LIGHT
//           if (P_RGB_REMAP == pindex) {
//             LightUpdateColorMapping();
//             restart_flag = 2;              // SetOption37 needs a reboot in most cases
//           }
// #endif
// #if (defined(USE_IR_REMOTE) && defined(USE_IR_RECEIVE)) || defined(USE_IR_REMOTE_FULL)
//           if (P_IR_UNKNOW_THRESHOLD == pindex) {
//             IrReceiveUpdateThreshold();
//           }
// #endif
//         } else {
//           ptype = 99;                      // Command Error
//         }
//       } else {
//         if (XdrvMailbox.payload <= 1) {
//           if (2 == ptype) {                // SetOption0 .. 31
//             switch (pindex) {
//               case 5:                      // mqtt_power_retain (CMND_POWERRETAIN)
//               case 6:                      // mqtt_button_retain (CMND_BUTTONRETAIN)
//               case 7:                      // mqtt_switch_retain (CMND_SWITCHRETAIN)
//               case 9:                      // mqtt_sensor_retain (CMND_SENSORRETAIN)
//               case 14:                     // interlock (CMND_INTERLOCK)
//               case 22:                     // mqtt_serial (SerialSend and SerialLog)
//               case 23:                     // mqtt_serial_raw (SerialSend)
//               case 25:                     // knx_enabled (Web config)
//               case 27:                     // knx_enable_enhancement (Web config)
//                 ptype = 99;                // Command Error
//                 break;                     // Ignore command SetOption
//               case 3:                      // mqtt
//               case 15:                     // pwm_control
//                 restart_flag = 2;
//               default:
//                 bitWrite(Settings.flag.data, pindex, XdrvMailbox.payload);
//             }
//             if (12 == pindex) {            // stop_flash_rotate
//               stop_flash_rotate = XdrvMailbox.payload;
//               SettingsSave(2);
//             }
//   #ifdef USE_HOME_ASSISTANT
//             if ((19 == pindex) || (30 == pindex)) {
//               HAssDiscover();              // Delayed execution to provide enough resources during hass_discovery or hass_light
//             }
//   #endif  // USE_HOME_ASSISTANT
//           }
//           else if (3 == ptype) {           // SetOption50 .. 81
//             bitWrite(Settings.flag3.data, pindex, XdrvMailbox.payload);
//             switch (pindex) {
//               case 5:                      // SetOption55
//                 if (0 == XdrvMailbox.payload) {
//                   restart_flag = 2;        // Disable mDNS needs restart
//                 }
//                 break;
//               case 10:                     // SetOption60 enable or disable traditional sleep
//                 WiFiSetSleepMode();        // Update WiFi sleep mode accordingly
//                 break;
//               case 18:                     // SetOption68 for multi-channel PWM, requires a reboot
//               case 25:                     // SetOption75 grouptopic change
//                 restart_flag = 2;
//                 break;
//             }
//           }
//           else if (4 == ptype) {           // SetOption82 .. 113
//             bitWrite(Settings.flag4.data, pindex, XdrvMailbox.payload);
//             switch (pindex) {
//               case 3:                      // SetOption85 - Enable Device Groups
//               case 6:                      // SetOption88 - PWM Dimmer Buttons control remote devices
//               case 15:                     // SetOption97 - Set Baud rate for TuyaMCU serial communication (0 = 9600 or 1 = 115200)
//                 restart_flag = 2;
//                 break;
//             }
//           }
//         } else {
//           ptype = 99;                      // Command Error
//         }
//       }
//     }

//     if (ptype < 99) {
//       if (1 == ptype) {
//         ResponseCmndIdxNumber(Settings.setoption_255[pindex]);
//       } else {
//         uint32_t flag = Settings.flag.data;
//         if (3 == ptype) {
//           flag = Settings.flag3.data;
//         }
//         else if (4 == ptype) {
//           flag = Settings.flag4.data;
//         }
//         ResponseCmndIdxChar(GetStateText(bitRead(flag, pindex)));
//       }
//     }
//   }
// }

// void CmndTemperatureResolution(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
//     Settings.flag2.temperature_resolution = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.flag2.temperature_resolution);
// }

// void CmndHumidityResolution(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
//     Settings.flag2.humidity_resolution = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.flag2.humidity_resolution);
// }

// void CmndPressureResolution(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
//     Settings.flag2.pressure_resolution = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.flag2.pressure_resolution);
// }

// void CmndPowerResolution(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
//     Settings.flag2.wattage_resolution = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.flag2.wattage_resolution);
// }

// void CmndVoltageResolution(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
//     Settings.flag2.voltage_resolution = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.flag2.voltage_resolution);
// }

// void CmndFrequencyResolution(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
//     Settings.flag2.frequency_resolution = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.flag2.frequency_resolution);
// }

// void CmndCurrentResolution(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
//     Settings.flag2.current_resolution = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.flag2.current_resolution);
// }

// void CmndEnergyResolution(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 5)) {
//     Settings.flag2.energy_resolution = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.flag2.energy_resolution);
// }

// void CmndWeightResolution(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
//     Settings.flag2.weight_resolution = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.flag2.weight_resolution);
// }

// void CmndSpeedUnit(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 6)) {
//     Settings.flag2.speed_conversion = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.flag2.speed_conversion);
// }

// void CmndModule(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= MAXMODULE)) {
//     bool present = false;
//     if (0 == XdrvMailbox.payload) {
//       XdrvMailbox.payload = USER_MODULE;
//       present = true;
//     } else {
//       XdrvMailbox.payload--;
//       present = ValidTemplateModule(XdrvMailbox.payload);
//     }
//     if (present) {
//       if (XdrvMailbox.index == 2) {
//         Settings.fallback_module = XdrvMailbox.payload;
//       } else {
//         Settings.last_module = Settings.module;
//         Settings.module = XdrvMailbox.payload;
//         SetModuleType();
//         if (Settings.last_module != XdrvMailbox.payload) {
//           for (uint32_t i = 0; i < ARRAY_SIZE(Settings.my_gp.io); i++) {
//             Settings.my_gp.io[i] = GPIO_NONE;
//           }
//         }
//         restart_flag = 2;
//       }
//     }
//   }
//   uint8_t module_real = Settings.module;
//   uint8_t module_number = ModuleNr();
//   if (XdrvMailbox.index == 2) {
//     module_real = Settings.fallback_module;
//     module_number = (USER_MODULE == Settings.fallback_module) ? 0 : Settings.fallback_module +1;
//     strcat(XdrvMailbox.command, "2");
//   }
//   Response_P(S_JSON_COMMAND_NVALUE_SVALUE, XdrvMailbox.command, module_number, AnyModuleName(module_real).c_str());
// }

// void CmndModules(void)
// {
//   uint32_t midx = USER_MODULE;
//   uint32_t lines = 1;
//   bool jsflg = false;
//   for (uint32_t i = 0; i <= sizeof(kModuleNiceList_IDS); i++) {
//     if (i > 0) { midx = pgm_read_byte(kModuleNiceList_IDS + i -1); }
//     if (!jsflg) {
//       Response_P(PSTR("{\"" D_JSON_MODULES "%d\":{"), lines);
//     } else {
//       ResponseAppend_P(PSTR(","));
//     }
//     jsflg = true;
//     uint32_t j = i ? midx +1 : 0;
//     if ((ResponseAppend_P(PSTR("\"%d\":\"%s\""), j, AnyModuleName(midx).c_str()) > (LOGSZ - TOPSZ)) || (i == sizeof(kModuleNiceList_IDS))) {
//       ResponseJsonEndEnd();
//       MqttPublishPrefixTopic_P(RESULT_OR_STAT, XdrvMailbox.command);
//       jsflg = false;
//       lines++;
//     }
//   }
//   mqtt_data[0] = '\0';
// }

// void CmndGpio(void)
// {
//   if (XdrvMailbox.index < ARRAY_SIZE(Settings.my_gp.io)) {
//     myio cmodule;
//     TemplateGPIOs(&cmodule);
//     if (ValidGPIO(XdrvMailbox.index, cmodule.io[XdrvMailbox.index]) && (XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < AGPIO(GPIO_SENSOR_END))) {
//       bool present = false;
//       for (uint32_t i = 0; i < ARRAY_SIZE(kGpioNiceList); i++) {
// #ifdef ESP8266
//         uint32_t midx = pgm_read_byte(kGpioNiceList + i);
//         if (midx == XdrvMailbox.payload) {
//           present = true;
//           break;
//         }
// #else  // ESP32
//         uint32_t midx = pgm_read_word(kGpioNiceList + i);
//         if ((XdrvMailbox.payload >= (midx & 0xFFE0)) && (XdrvMailbox.payload < midx)) {
//           present = true;
//           break;
//         }
// #endif  // ESP8266 - ESP32
//       }
//       if (present) {
//         for (uint32_t i = 0; i < ARRAY_SIZE(Settings.my_gp.io); i++) {
//           if (ValidGPIO(i, cmodule.io[i]) && (Settings.my_gp.io[i] == XdrvMailbox.payload)) {
//             Settings.my_gp.io[i] = GPIO_NONE;
//           }
//         }
//         Settings.my_gp.io[XdrvMailbox.index] = XdrvMailbox.payload;
//         restart_flag = 2;
//       }
//     }
//     Response_P(PSTR("{"));
//     bool jsflg = false;
//     for (uint32_t i = 0; i < ARRAY_SIZE(Settings.my_gp.io); i++) {
//       if (ValidGPIO(i, cmodule.io[i]) || ((AGPIO(GPIO_USER) == XdrvMailbox.payload) && !FlashPin(i))) {
//         if (jsflg) { ResponseAppend_P(PSTR(",")); }
//         jsflg = true;
//         uint32_t sensor_type = Settings.my_gp.io[i];
//         if (!ValidGPIO(i, cmodule.io[i])) {
//           sensor_type = cmodule.io[i];
//           if (AGPIO(GPIO_USER) == sensor_type) {  // A user GPIO equals a not connected (=GPIO_NONE) GPIO here
//             sensor_type = GPIO_NONE;
//           }
//         }
//         char sindex[4] = { 0 };
//         uint32_t sensor_name_idx = BGPIO(sensor_type);
// #ifdef ESP32
//         uint32_t nice_list_search = sensor_type & 0xFFE0;
//         for (uint32_t j = 0; j < ARRAY_SIZE(kGpioNiceList); j++) {
//           uint32_t nls_idx = pgm_read_word(kGpioNiceList + j);
//           if (((nls_idx & 0xFFE0) == nice_list_search) && ((nls_idx & 0x001F) > 0)) {
//             snprintf_P(sindex, sizeof(sindex), PSTR("%d"), (sensor_type & 0x001F) +1);
//             break;
//           }
//         }
// #endif  // ESP32
//         const char *sensor_names = kSensorNames;
//         if (sensor_name_idx > GPIO_FIX_START) {
//           sensor_name_idx = sensor_name_idx - GPIO_FIX_START -1;
//           sensor_names = kSensorNamesFixed;
//         }
//         char stemp1[TOPSZ];
//         ResponseAppend_P(PSTR("\"" D_JSON_GPIO "%d\":{\"%d\":\"%s%s\"}"),
//           i, sensor_type, GetTextIndexed(stemp1, sizeof(stemp1), sensor_name_idx, sensor_names), sindex);
//       }
//     }
//     if (jsflg) {
//       ResponseJsonEnd();
//     } else {
//       ResponseCmndChar(D_JSON_NOT_SUPPORTED);
//     }
//   }
// }

// void CmndGpios(void)
// {
//   myio cmodule;
//   TemplateGPIOs(&cmodule);
//   uint32_t lines = 1;
//   bool jsflg = false;
//   for (uint32_t i = 0; i < ARRAY_SIZE(kGpioNiceList); i++) {
// #ifdef ESP8266
//     uint32_t midx = pgm_read_byte(kGpioNiceList + i);
//     uint32_t ridx = midx;
// #else  // ESP32
//     uint32_t ridx = pgm_read_word(kGpioNiceList + i) & 0xFFE0;
//     uint32_t midx = BGPIO(ridx);
// #endif  // ESP8266 - ESP32
//     if ((XdrvMailbox.payload != 255) && GetUsedInModule(midx, cmodule.io)) { continue; }
//     if (!jsflg) {
//       Response_P(PSTR("{\"" D_JSON_GPIOS "%d\":{"), lines);
//     } else {
//       ResponseAppend_P(PSTR(","));
//     }
//     jsflg = true;
//     char stemp1[TOPSZ];
//     if ((ResponseAppend_P(PSTR("\"%d\":\"%s\""), ridx, GetTextIndexed(stemp1, sizeof(stemp1), midx, kSensorNames)) > (LOGSZ - TOPSZ)) || (i == ARRAY_SIZE(kGpioNiceList) -1)) {
//       ResponseJsonEndEnd();
//       MqttPublishPrefixTopic_P(RESULT_OR_STAT, XdrvMailbox.command);
//       jsflg = false;
//       lines++;
//     }
//   }
//   mqtt_data[0] = '\0';
// }

// void CmndTemplate(void)
// {
//   // {"NAME":"Generic","GPIO":[17,254,29,254,7,254,254,254,138,254,139,254,254],"FLAG":1,"BASE":255}
//   bool error = false;

//   if (strstr(XdrvMailbox.data, "{") == nullptr) {  // If no JSON it must be parameter
//     if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= MAXMODULE)) {
//       XdrvMailbox.payload--;
//       if (ValidTemplateModule(XdrvMailbox.payload)) {
//         ModuleDefault(XdrvMailbox.payload);     // Copy template module
//         if (USER_MODULE == Settings.module) { restart_flag = 2; }
//       }
//     }
//     else if (0 == XdrvMailbox.payload) {        // Copy current template to user template
//       if (Settings.module != USER_MODULE) {
//         ModuleDefault(Settings.module);
//       }
//     }
//     else if (255 == XdrvMailbox.payload) {      // Copy current module with user configured GPIO
//       if (Settings.module != USER_MODULE) {
//         ModuleDefault(Settings.module);
//       }
//       SettingsUpdateText(SET_TEMPLATE_NAME, "Merged");
//       uint32_t j = 0;
//       for (uint32_t i = 0; i < ARRAY_SIZE(Settings.user_template.gp.io); i++) {
//         if (6 == i) { j = 9; }
//         if (8 == i) { j = 12; }
//         if (my_module.io[j] > GPIO_NONE) {
//           Settings.user_template.gp.io[i] = my_module.io[j];
//         }
//         j++;
//       }
//     }
//   }
//   else {
//     if (JsonTemplate(XdrvMailbox.data)) {    // Free 336 bytes StaticJsonBuffer stack space by moving code to function
//       if (USER_MODULE == Settings.module) { restart_flag = 2; }
//     } else {
//       ResponseCmndChar_P(PSTR(D_JSON_INVALID_JSON));
//       error = true;
//     }
//   }
//   if (!error) { TemplateJson(); }
// }

// void CmndPwm(void)
// {
//   if (pwm_present && (XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_PWMS)) {
//     if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= Settings.pwm_range) && PinUsed(GPIO_PWM1, XdrvMailbox.index -1)) {
//       Settings.pwm_value[XdrvMailbox.index -1] = XdrvMailbox.payload;
//       analogWrite(Pin(GPIO_PWM1, XdrvMailbox.index -1), bitRead(pwm_inverted, XdrvMailbox.index -1) ? Settings.pwm_range - XdrvMailbox.payload : XdrvMailbox.payload);
//     }
//     Response_P(PSTR("{"));
//     MqttShowPWMState();  // Render the PWM status to MQTT
//     ResponseJsonEnd();
//   }
// }

// void CmndPwmfrequency(void)
// {
//   if ((1 == XdrvMailbox.payload) || ((XdrvMailbox.payload >= PWM_MIN) && (XdrvMailbox.payload <= PWM_MAX))) {
//     Settings.pwm_frequency = (1 == XdrvMailbox.payload) ? PWM_FREQ : XdrvMailbox.payload;
// #ifdef ESP8266
//     analogWriteFreq(Settings.pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)
// #else
//     analogWriteFreqRange(0,Settings.pwm_frequency,Settings.pwm_range);
// #endif
//   }
//   ResponseCmndNumber(Settings.pwm_frequency);
// }

// void CmndPwmrange(void)
// {
//   if ((1 == XdrvMailbox.payload) || ((XdrvMailbox.payload > 254) && (XdrvMailbox.payload < 1024))) {
//     Settings.pwm_range = (1 == XdrvMailbox.payload) ? PWM_RANGE : XdrvMailbox.payload;
//     for (uint32_t i = 0; i < MAX_PWMS; i++) {
//       if (Settings.pwm_value[i] > Settings.pwm_range) {
//         Settings.pwm_value[i] = Settings.pwm_range;
//       }
//     }
// #ifdef ESP8266
//     analogWriteRange(Settings.pwm_range);      // Default is 1023 (Arduino.h)
// #else
//     analogWriteFreqRange(0,Settings.pwm_frequency,Settings.pwm_range);
// #endif
//   }
//   ResponseCmndNumber(Settings.pwm_range);
// }

// void CmndButtonDebounce(void)
// {
//   if ((XdrvMailbox.payload > 39) && (XdrvMailbox.payload < 1001)) {
//     Settings.button_debounce = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.button_debounce);
// }

// void CmndSwitchDebounce(void)
// {
//   if ((XdrvMailbox.payload > 39) && (XdrvMailbox.payload < 1010)) {
//     Settings.switch_debounce = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.switch_debounce);
// }

// void CmndBaudrate(void)
// {
//   if (XdrvMailbox.payload >= 300) {
//     XdrvMailbox.payload /= 300;  // Make it a valid baudrate
//     uint32_t baudrate = (XdrvMailbox.payload & 0xFFFF) * 300;
//     SetSerialBaudrate(baudrate);
//   }
//   ResponseCmndNumber(Settings.baudrate * 300);
// }

// void CmndSerialConfig(void)
// {
//   // See TasmotaSerialConfig for possible options
//   // SerialConfig 0..23 where 3 equals 8N1
//   // SerialConfig 8N1

//   if (XdrvMailbox.data_len > 0) {
//     if (XdrvMailbox.data_len < 3) {                    // Use 0..23 as serial config option
//       if ((XdrvMailbox.payload >= TS_SERIAL_5N1) && (XdrvMailbox.payload <= TS_SERIAL_8O2)) {
//         SetSerialConfig(XdrvMailbox.payload);
//       }
//     }
//     else if ((XdrvMailbox.payload >= 5) && (XdrvMailbox.payload <= 8)) {
//       uint8_t serial_config = XdrvMailbox.payload -5;  // Data bits 5, 6, 7 or 8, No parity and 1 stop bit

//       bool valid = true;
//       char parity = (XdrvMailbox.data[1] & 0xdf);
//       if ('E' == parity) {
//         serial_config += 0x08;                         // Even parity
//       }
//       else if ('O' == parity) {
//         serial_config += 0x10;                         // Odd parity
//       }
//       else if ('N' != parity) {
//         valid = false;
//       }

//       if ('2' == XdrvMailbox.data[2]) {
//         serial_config += 0x04;                         // Stop bits 2
//       }
//       else if ('1' != XdrvMailbox.data[2]) {
//         valid = false;
//       }

//       if (valid) {
//         SetSerialConfig(serial_config);
//       }
//     }
//   }
//   ResponseCmndChar(GetSerialConfig().c_str());
// }

// void CmndSerialSend(void)
// {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 5)) {
//     SetSeriallog(LOG_LEVEL_NONE);
//     Settings.flag.mqtt_serial = 1;                                  // CMND_SERIALSEND and CMND_SERIALLOG
//     Settings.flag.mqtt_serial_raw = (XdrvMailbox.index > 3) ? 1 : 0;  // CMND_SERIALSEND3
//     if (XdrvMailbox.data_len > 0) {
//       if (1 == XdrvMailbox.index) {
//         DEBUG_PRINTF("%s\n", XdrvMailbox.data);                    // "Hello Tiger\n"
//       }
//       else if (2 == XdrvMailbox.index || 4 == XdrvMailbox.index) {
//         for (uint32_t i = 0; i < XdrvMailbox.data_len; i++) {
//           Serial.write(XdrvMailbox.data[i]);                        // "Hello Tiger" or "A0"
//         }
//       }
//       else if (3 == XdrvMailbox.index) {
//         uint32_t dat_len = XdrvMailbox.data_len;
//         DEBUG_PRINTF("%s", Unescape(XdrvMailbox.data, &dat_len));  // "Hello\f"
//       }
//       else if (5 == XdrvMailbox.index) {
//         SerialSendRaw(RemoveSpace(XdrvMailbox.data));               // "AA004566" as hex values
//       }
//       ResponseCmndDone();
//     }
//   }
// }

// void CmndSerialDelimiter(void)
// {
//   if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.payload < 256)) {
//     if (XdrvMailbox.payload > 0) {
//       Settings.serial_delimiter = XdrvMailbox.payload;
//     } else {
//       uint32_t dat_len = XdrvMailbox.data_len;
//       Unescape(XdrvMailbox.data, &dat_len);
//       Settings.serial_delimiter = XdrvMailbox.data[0];
//     }
//   }
//   ResponseCmndNumber(Settings.serial_delimiter);
// }

// void CmndSyslog(void)
// {
//   if ((XdrvMailbox.payload >= LOG_LEVEL_NONE) && (XdrvMailbox.payload <= LOG_LEVEL_DEBUG_MORE)) {
//     SetSyslog(XdrvMailbox.payload);
//   }
//   Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, XdrvMailbox.command, Settings.syslog_level, syslog_level);
// }

// void CmndLoghost(void)
// {
//   if (XdrvMailbox.data_len > 0) {
//     SettingsUpdateText(SET_SYSLOG_HOST, (SC_DEFAULT == Shortcut()) ? SYS_LOG_HOST : XdrvMailbox.data);
//   }
//   ResponseCmndChar(SettingsText(SET_SYSLOG_HOST));
// }

// void CmndLogport(void)
// {
//   if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 65536)) {
//     Settings.syslog_port = (1 == XdrvMailbox.payload) ? SYS_LOG_PORT : XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.syslog_port);
// }

// void CmndIpAddress(void)
// {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 4)) {
//     uint32_t address;
//     if (ParseIp(&address, XdrvMailbox.data)) {
//       Settings.ip_address[XdrvMailbox.index -1] = address;
// //        restart_flag = 2;
//     }
//     char stemp1[TOPSZ];
//     snprintf_P(stemp1, sizeof(stemp1), PSTR(" (%s)"), WiFi.localIP().toString().c_str());
//     Response_P(S_JSON_COMMAND_INDEX_SVALUE_SVALUE, XdrvMailbox.command, XdrvMailbox.index, IPAddress(Settings.ip_address[XdrvMailbox.index -1]).toString().c_str(), (1 == XdrvMailbox.index) ? stemp1:"");
//   }
// }

// void CmndNtpServer(void)
// {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_NTP_SERVERS)) {
//     if (!XdrvMailbox.usridx) {
//       ResponseCmndAll(SET_NTPSERVER1, MAX_NTP_SERVERS);
//     } else {
//       uint32_t ntp_server = SET_NTPSERVER1 + XdrvMailbox.index -1;
//       if (XdrvMailbox.data_len > 0) {
//         SettingsUpdateText(ntp_server,
//           (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? (1 == XdrvMailbox.index) ? PSTR(NTP_SERVER1) : (2 == XdrvMailbox.index) ? PSTR(NTP_SERVER2) : PSTR(NTP_SERVER3) : XdrvMailbox.data);
//         SettingsUpdateText(ntp_server, ReplaceCommaWithDot(SettingsText(ntp_server)));
//   //        restart_flag = 2;  // Issue #3890
//         ntp_force_sync = true;
//       }
//       ResponseCmndIdxChar(SettingsText(ntp_server));
//     }
//   }
// }

// void CmndAp(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
//     switch (XdrvMailbox.payload) {
//     case 0:  // Toggle
//       Settings.sta_active ^= 1;
//       break;
//     case 1:  // AP1
//     case 2:  // AP2
//       Settings.sta_active = XdrvMailbox.payload -1;
//     }
//     Settings.wifi_channel = 0;  // Disable stored AP
//     restart_flag = 2;
//   }
//   Response_P(S_JSON_COMMAND_NVALUE_SVALUE, XdrvMailbox.command, Settings.sta_active +1, EscapeJSONString(SettingsText(SET_STASSID1 + Settings.sta_active)).c_str());
// }

// void CmndSsid(void)
// {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_SSIDS)) {
//     if (!XdrvMailbox.usridx) {
//       ResponseCmndAll(SET_STASSID1, MAX_SSIDS);
//     } else {
//       if (XdrvMailbox.data_len > 0) {
//         SettingsUpdateText(SET_STASSID1 + XdrvMailbox.index -1,
//                 (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? (1 == XdrvMailbox.index) ? STA_SSID1 : STA_SSID2 : XdrvMailbox.data);
//         Settings.sta_active = XdrvMailbox.index -1;
//         restart_flag = 2;
//       }
//       ResponseCmndIdxChar(SettingsText(SET_STASSID1 + XdrvMailbox.index -1));
//     }
//   }
// }

// void CmndPassword(void)
// {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
//     if ((XdrvMailbox.data_len > 4) || (SC_CLEAR == Shortcut()) || (SC_DEFAULT == Shortcut())) {
//       SettingsUpdateText(SET_STAPWD1 + XdrvMailbox.index -1,
//               (SC_CLEAR == Shortcut()) ? "" : (SC_DEFAULT == Shortcut()) ? (1 == XdrvMailbox.index) ? STA_PASS1 : STA_PASS2 : XdrvMailbox.data);
//       Settings.sta_active = XdrvMailbox.index -1;
//       restart_flag = 2;
//       ResponseCmndIdxChar(SettingsText(SET_STAPWD1 + XdrvMailbox.index -1));
//     } else {
//       Response_P(S_JSON_COMMAND_INDEX_ASTERISK, XdrvMailbox.command, XdrvMailbox.index);
//     }
//   }
// }

// void CmndHostname(void)
// {
//   if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
//     SettingsUpdateText(SET_HOSTNAME, (SC_DEFAULT == Shortcut()) ? WIFI_HOSTNAME : XdrvMailbox.data);
//     if (strstr(SettingsText(SET_HOSTNAME), "%") != nullptr) {
//       SettingsUpdateText(SET_HOSTNAME, WIFI_HOSTNAME);
//     }
//     restart_flag = 2;
//   }
//   ResponseCmndChar(SettingsText(SET_HOSTNAME));
// }

// void CmndWifiConfig(void)
// {
//   if ((XdrvMailbox.payload >= WIFI_RESTART) && (XdrvMailbox.payload < MAX_WIFI_OPTION)) {
//     if ((EX_WIFI_SMARTCONFIG == XdrvMailbox.payload) || (EX_WIFI_WPSCONFIG == XdrvMailbox.payload)) {
//       XdrvMailbox.payload = WIFI_MANAGER;
//     }
//     Settings.sta_config = XdrvMailbox.payload;
//     wifi_state_flag = Settings.sta_config;
//     if (WifiState() > WIFI_RESTART) {
//       restart_flag = 2;
//     }
//   }
//   char stemp1[TOPSZ];
//   Response_P(S_JSON_COMMAND_NVALUE_SVALUE, XdrvMailbox.command, Settings.sta_config, GetTextIndexed(stemp1, sizeof(stemp1), Settings.sta_config, kWifiConfig));
// }

// void CmndWifi(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     Settings.flag4.network_wifi = XdrvMailbox.payload;
//     restart_flag = 2;
//   }
//   ResponseCmndStateText(Settings.flag4.network_wifi);
// }

// void CmndDevicename(void)
// {
//   if (!XdrvMailbox.grpflg && (XdrvMailbox.data_len > 0)) {
//     SettingsUpdateText(SET_DEVICENAME, ('"' == XdrvMailbox.data[0]) ? "" : (SC_DEFAULT == Shortcut()) ? SettingsText(SET_FRIENDLYNAME1) : XdrvMailbox.data);
//   }
//   ResponseCmndChar(SettingsText(SET_DEVICENAME));
// }

// void CmndFriendlyname(void)
// {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_FRIENDLYNAMES)) {
//     if (!XdrvMailbox.usridx && !XdrvMailbox.data_len) {
//       ResponseCmndAll(SET_FRIENDLYNAME1, MAX_FRIENDLYNAMES);
//     } else {
//       if (XdrvMailbox.data_len > 0) {
//         char stemp1[TOPSZ];
//         if (1 == XdrvMailbox.index) {
//           snprintf_P(stemp1, sizeof(stemp1), PSTR(FRIENDLY_NAME));
//         } else {
//           snprintf_P(stemp1, sizeof(stemp1), PSTR(FRIENDLY_NAME "%d"), XdrvMailbox.index);
//         }
//         SettingsUpdateText(SET_FRIENDLYNAME1 + XdrvMailbox.index -1, ('"' == XdrvMailbox.data[0]) ? "" : (SC_DEFAULT == Shortcut()) ? stemp1 : XdrvMailbox.data);
//       }
//       ResponseCmndIdxChar(SettingsText(SET_FRIENDLYNAME1 + XdrvMailbox.index -1));
//     }
//   }
// }

// void CmndSwitchMode(void)
// {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_SWITCHES)) {
//     if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < MAX_SWITCH_OPTION)) {
//       Settings.switchmode[XdrvMailbox.index -1] = XdrvMailbox.payload;
//     }
//     ResponseCmndIdxNumber(Settings.switchmode[XdrvMailbox.index-1]);
//   }
// }

// void CmndInterlock(void)
// {
//   // Interlock 0 - Off, Interlock 1 - On, Interlock 1,2 3,4 5,6,7
//   uint32_t max_relays = devices_present;
//   if (Settings.light_settings.type) { max_relays--; }
//   if (max_relays > sizeof(Settings.interlock[0]) * 8) { max_relays = sizeof(Settings.interlock[0]) * 8; }
//   if (max_relays > 1) {                                         // Only interlock with more than 1 relay
//     if (XdrvMailbox.data_len > 0) {
//       if (strstr(XdrvMailbox.data, ",") != nullptr) {                    // Interlock entry
//         for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) { Settings.interlock[i] = 0; }  // Reset current interlocks
//         char *group;
//         char *q;
//         uint32_t group_index = 0;
//         power_t relay_mask = 0;
//         for (group = strtok_r(XdrvMailbox.data, " ", &q); group && group_index < MAX_INTERLOCKS; group = strtok_r(nullptr, " ", &q)) {
//           char *str;
//           char *p;
//           for (str = strtok_r(group, ",", &p); str; str = strtok_r(nullptr, ",", &p)) {
//             int pbit = atoi(str);
//             if ((pbit > 0) && (pbit <= max_relays)) {           // Only valid relays
//               pbit--;
//               if (!bitRead(relay_mask, pbit)) {                 // Only relay once
//                 bitSet(relay_mask, pbit);
//                 bitSet(Settings.interlock[group_index], pbit);
//               }
//             }
//           }
//           group_index++;
//         }
//         for (uint32_t i = 0; i < group_index; i++) {
//           uint32_t minimal_bits = 0;
//           for (uint32_t j = 0; j < max_relays; j++) {
//             if (bitRead(Settings.interlock[i], j)) { minimal_bits++; }
//           }
//           if (minimal_bits < 2) { Settings.interlock[i] = 0; }  // Discard single relay as interlock
//         }
//       } else {
//         Settings.flag.interlock = XdrvMailbox.payload &1;       // CMND_INTERLOCK - Enable/disable interlock
//         if (Settings.flag.interlock) {
//           SetDevicePower(power, SRC_IGNORE);                    // Remove multiple relays if set
//         }
//       }
// #ifdef USE_SHUTTER
//       if (Settings.flag3.shutter_mode) {  // SetOption80 - Enable shutter support
//         ShutterInit(); // to update shutter mode
//       }
// #endif  // USE_SHUTTER
//     }
//     Response_P(PSTR("{\"" D_JSON_INTERLOCK "\":\"%s\",\"" D_JSON_GROUPS "\":\""), GetStateText(Settings.flag.interlock));
//     uint32_t anygroup = 0;
//     for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
//       if (Settings.interlock[i]) {
//         anygroup++;
//         ResponseAppend_P(PSTR("%s"), (anygroup > 1) ? " " : "");
//         uint32_t anybit = 0;
//         power_t mask = 1;
//         for (uint32_t j = 0; j < max_relays; j++) {
//           if (Settings.interlock[i] & mask) {
//             anybit++;
//             ResponseAppend_P(PSTR("%s%d"), (anybit > 1) ? "," : "", j +1);
//           }
//           mask <<= 1;
//         }
//       }
//     }
//     if (!anygroup) {
//       for (uint32_t j = 1; j <= max_relays; j++) {
//         ResponseAppend_P(PSTR("%s%d"), (j > 1) ? "," : "", j);
//       }
//     }
//     ResponseAppend_P(PSTR("\"}"));
//   } else {
//     // never ever reset interlock mode inadvertently if we forced it upon compilation
//     Settings.flag.interlock = APP_INTERLOCK_MODE;               // CMND_INTERLOCK - Enable/disable interlock
//     ResponseCmndStateText(Settings.flag.interlock);
//   }
// }

// void CmndTeleperiod(void)
// {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 3601)) {
//     Settings.tele_period = (1 == XdrvMailbox.payload) ? TELE_PERIOD : XdrvMailbox.payload;
//     if ((Settings.tele_period > 0) && (Settings.tele_period < 10)) Settings.tele_period = 10;   // Do not allow periods < 10 seconds
// //    tele_period = Settings.tele_period;
//   }
//   tele_period = Settings.tele_period;        // Show teleperiod data also on empty command
//   ResponseCmndNumber(Settings.tele_period);
// }

// void CmndReset(void)
// {
//   switch (XdrvMailbox.payload) {
//   case 1:
//     restart_flag = 211;
//     ResponseCmndChar(PSTR(D_JSON_RESET_AND_RESTARTING));
//     break;
//   case 2 ... 6:
//     restart_flag = 210 + XdrvMailbox.payload;
//     Response_P(PSTR("{\"" D_JSON_RESET "\":\"" D_JSON_ERASE ", " D_JSON_RESET_AND_RESTARTING "\"}"));
//     break;
//   case 99:
//     Settings.bootcount = 0;
//     Settings.bootcount_reset_time = 0;
//     ResponseCmndDone();
//     break;
//   default:
//     ResponseCmndChar(PSTR(D_JSON_ONE_TO_RESET));
//   }
// }

// void CmndTime(void)
// {
// // payload 0 = (re-)enable NTP
// // payload 1 = Time format {"Time":"2019-09-04T14:31:29"}
// // payload 2 = Time format {"Time":"2019-09-04T14:31:29","Epoch":1567600289}
// // payload 3 = Time format {"Time":1567600289}
// // payload 4 = Time format {"Time":"2019-09-04T14:31:29.123"}
// // payload 1451602800 - disable NTP and set time to epoch

//   uint32_t format = Settings.flag2.time_format;
//   if (XdrvMailbox.data_len > 0) {
//     if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload < 5)) {
//       Settings.flag2.time_format = XdrvMailbox.payload -1;
//       format = Settings.flag2.time_format;
//     } else {
//       format = 1;  // {"Time":"2019-09-04T14:31:29","Epoch":1567600289}
//       RtcSetTime(XdrvMailbox.payload);
//     }
//   }
//   mqtt_data[0] = '\0';
//   ResponseAppendTimeFormat(format);
//   ResponseJsonEnd();
// }

// void CmndTimezone(void)
// {
//   if ((XdrvMailbox.data_len > 0) && (XdrvMailbox.payload >= -13)) {
//     Settings.timezone = XdrvMailbox.payload;
//     Settings.timezone_minutes = 0;
//     if (XdrvMailbox.payload < 15) {
//       char *p = strtok (XdrvMailbox.data, ":");
//       if (p) {
//         p = strtok (nullptr, ":");
//         if (p) {
//           Settings.timezone_minutes = strtol(p, nullptr, 10);
//           if (Settings.timezone_minutes > 59) { Settings.timezone_minutes = 59; }
//         }
//       }
//     } else {
//       Settings.timezone = 99;
//     }
//     ntp_force_sync = true;
//   }
//   if (99 == Settings.timezone) {
//     ResponseCmndNumber(Settings.timezone);
//   } else {
//     char stemp1[TOPSZ];
//     snprintf_P(stemp1, sizeof(stemp1), PSTR("%+03d:%02d"), Settings.timezone, Settings.timezone_minutes);
//     ResponseCmndChar(stemp1);
//   }
// }

// void CmndTimeStdDst(uint32_t ts)
// {
//   // TimeStd 0/1, 0/1/2/3/4, 1..12, 1..7, 0..23, +/-780
//   if (XdrvMailbox.data_len > 0) {
//     if (strstr(XdrvMailbox.data, ",") != nullptr) {   // Process parameter entry
//       uint32_t tpos = 0;                      // Parameter index
//       int value = 0;
//       char *p = XdrvMailbox.data;                           // Parameters like "1, 2,3 , 4 ,5, -120" or ",,,,,+240"
//       char *q = p;                           // Value entered flag
//       while (p && (tpos < 7)) {
//         if (p > q) {                         // Any value entered
//           if (1 == tpos) { Settings.tflag[ts].hemis = value &1; }
//           if (2 == tpos) { Settings.tflag[ts].week = (value < 0) ? 0 : (value > 4) ? 4 : value; }
//           if (3 == tpos) { Settings.tflag[ts].month = (value < 1) ? 1 : (value > 12) ? 12 : value; }
//           if (4 == tpos) { Settings.tflag[ts].dow = (value < 1) ? 1 : (value > 7) ? 7 : value; }
//           if (5 == tpos) { Settings.tflag[ts].hour = (value < 0) ? 0 : (value > 23) ? 23 : value; }
//           if (6 == tpos) { Settings.toffset[ts] = (value < -900) ? -900 : (value > 900) ? 900 : value; }
//         }
//         p = Trim(p);                        // Skip spaces
//         if (tpos && (*p == ',')) { p++; }    // Skip separator
//         p = Trim(p);                        // Skip spaces
//         q = p;                               // Reset any value entered flag
//         value = strtol(p, &p, 10);
//         tpos++;                              // Next parameter
//       }
//       ntp_force_sync = true;
//     } else {
//       if (0 == XdrvMailbox.payload) {
//         if (0 == ts) {
//           SettingsResetStd();
//         } else {
//           SettingsResetDst();
//         }
//       }
//       ntp_force_sync = true;
//     }
//   }
//   Response_P(PSTR("{\"%s\":{\"Hemisphere\":%d,\"Week\":%d,\"Month\":%d,\"Day\":%d,\"Hour\":%d,\"Offset\":%d}}"),
//     XdrvMailbox.command, Settings.tflag[ts].hemis, Settings.tflag[ts].week, Settings.tflag[ts].month, Settings.tflag[ts].dow, Settings.tflag[ts].hour, Settings.toffset[ts]);
// }

// void CmndTimeStd(void)
// {
//   CmndTimeStdDst(0);
// }

// void CmndTimeDst(void)
// {
//   CmndTimeStdDst(1);
// }

// void CmndAltitude(void)
// {
//   if ((XdrvMailbox.data_len > 0) && ((XdrvMailbox.payload >= -30000) && (XdrvMailbox.payload <= 30000))) {
//     Settings.altitude = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(Settings.altitude);
// }

// void CmndLedPower(void) {
//   // If GPIO_LEDLINK (used for network status) then allow up to 4 GPIO_LEDx control using led_power
//   // If no GPIO_LEDLINK then allow legacy single led GPIO_LED1 control using Settings.ledstate
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_LEDS)) {
//     if (!PinUsed(GPIO_LEDLNK)) { XdrvMailbox.index = 1; }
//     if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
//       Settings.ledstate &= 8;                // Disable power control
//       uint32_t mask = 1 << (XdrvMailbox.index -1);        // Led to control
//       switch (XdrvMailbox.payload) {
//       case 0: // Off
//         led_power &= (0xFF ^ mask);
//         Settings.ledstate = 0;
//         break;
//       case 1: // On
//         led_power |= mask;
//         Settings.ledstate = 8;
//         break;
//       case 2: // Toggle
//         led_power ^= mask;
//         Settings.ledstate ^= 8;
//         break;
//       }
//       blinks = 0;
//       if (!PinUsed(GPIO_LEDLNK)) {
//         SetLedPower(Settings.ledstate &8);
//       } else {
//         SetLedPowerIdx(XdrvMailbox.index -1, (led_power & mask));
//       }
//     }
//     bool state = bitRead(led_power, XdrvMailbox.index -1);
//     if (!PinUsed(GPIO_LEDLNK)) {
//       state = bitRead(Settings.ledstate, 3);
//     }
//     ResponseCmndIdxChar(GetStateText(state));
//   }
// }

// void CmndLedState(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < MAX_LED_OPTION)) {
//     Settings.ledstate = XdrvMailbox.payload;
//     if (!Settings.ledstate) {
//       SetLedPowerAll(0);
//       SetLedLink(0);
//     }
//   }
//   ResponseCmndNumber(Settings.ledstate);
// }

// void CmndLedMask(void) {
//   if (XdrvMailbox.data_len > 0) {
//     Settings.ledmask = XdrvMailbox.payload;
//   }
//   char stemp1[TOPSZ];
//   snprintf_P(stemp1, sizeof(stemp1), PSTR("%d (0x%04X)"), Settings.ledmask, Settings.ledmask);
//   ResponseCmndChar(stemp1);
// }

// void CmndLedPwmOff(void) {
//   if (XdrvMailbox.data_len > 0) {
//     if (XdrvMailbox.payload < 0) {
//       Settings.ledpwm_off = 0;
//     }
//     else if (XdrvMailbox.payload > 255) {
//       Settings.ledpwm_off = 255;
//     } else {
//       Settings.ledpwm_off = XdrvMailbox.payload;
//     }
//     UpdateLedPowerAll();
//   }
//   ResponseCmndNumber(Settings.ledpwm_off);
// }

// void CmndLedPwmOn(void) {
//   if (XdrvMailbox.data_len > 0) {
//     if (XdrvMailbox.payload < 0) {
//       Settings.ledpwm_on = 0;
//     }
//     else if (XdrvMailbox.payload > 255) {
//       Settings.ledpwm_on = 255;
//     } else {
//       Settings.ledpwm_on = XdrvMailbox.payload;
//     }
//     UpdateLedPowerAll();
//   }
//   ResponseCmndNumber(Settings.ledpwm_on);
// }

// void CmndLedPwmMode(void) {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= MAX_LEDS)) {
//     if (!PinUsed(GPIO_LEDLNK)) { XdrvMailbox.index = 1; }
//     if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 2)) {
//       uint32_t mask = 1 << (XdrvMailbox.index -1);        // Led to configure
//       switch (XdrvMailbox.payload) {
//       case 0: // digital
//         Settings.ledpwm_mask &= (0xFF ^ mask);
//         break;
//       case 1: // pwm
//         Settings.ledpwm_mask |= mask;
//         break;
//       case 2: // toggle
//         Settings.ledpwm_mask ^= mask;
//         break;
//       }
//       UpdateLedPowerAll();
//     }
//     bool state = bitRead(Settings.ledpwm_mask, XdrvMailbox.index -1);
//     ResponseCmndIdxChar(GetStateText(state));
//   }
// }

// void CmndWifiPower(void)
// {
//   if (XdrvMailbox.data_len > 0) {
//     Settings.wifi_output_power = (uint8_t)(CharToFloat(XdrvMailbox.data) * 10);
//     if (Settings.wifi_output_power > 205) {
//       Settings.wifi_output_power = 205;
//     }
//     WifiSetOutputPower();
//   }
//   ResponseCmndChar(WifiGetOutputPower().c_str());
// }

// #ifdef USE_I2C
// void CmndI2cScan(void)
// {
//   if (i2c_enabled) {
//     I2cScan(mqtt_data, sizeof(mqtt_data));
//   }
// }






/**
 Commands have form: "<name><index> <value>" e.g."button2 1" or "slider3 123"

*/
void mSupport::MqttDataHandler(char* topic, uint8_t* data, unsigned int data_len)
{

//   if (data_len > MQTT_MAX_PACKET_SIZE) { return; }  // Do not allow more data than would be feasable within stack space

//   char *str;

//   // if (!strcmp(pCONT_set->Settings.mqtt_prefix[0],pCONT_set->Settings.mqtt_prefix[1])) {
//   //   str = strstr(topic,pCONT_set->Settings.mqtt_prefix[0]);
//   //   if ((str == topic) && pCONT_set->mqtt_cmnd_publish) {
//   //     // if (pCONT_set->mqtt_cmnd_publish > 3) {
//   //     //   mqtt_cmnd_publish -= 3;
//   //     // } else {
//   //     //   mqtt_cmnd_publish = 0;
//   //     // }
//   //    // return;
//   //   }
//   // }

//   char topicBuf[TOPSZ];
//   char dataBuf[data_len+1];
//   char command [CMDSZ];
//   char stemp1[TOPSZ];
//   char *p;
//   char *type = nullptr;
//   uint8_t lines = 1;
//   bool jsflg = false;
//   bool grpflg = false;
//   bool user_append_index = false;
//   uint32_t i = 0;
//   uint32_t index;
//   uint32_t address;

//   #ifdef USE_DEBUG_DRIVER
//     ShowFreeMem(PSTR("MqttDataHandler"));
//   #endif

// /******************************************************************************************************
//  *******Extract <name><index> as topicBuf, <value> as dataBuf using space delimeter ***********************************************************************************************
//  ******************************************************************************************************/
 
//   // "string numbervalue" - split into topic and data buffers
//   strlcpy(topicBuf, topic, sizeof(topicBuf));
//   for (i = 0; i < data_len; i++) {
//     if (!isspace(data[i])) { break; } 
//   }
//   data_len -= i;
//   memcpy(dataBuf, data +i, sizeof(dataBuf));
//   dataBuf[sizeof(dataBuf)-1] = 0;

//   if (topicBuf[0] != '/') { ShowSource(SRC_MQTT); }

//   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_RECEIVED_TOPIC " %s, " D_DATA_SIZE " %d, " D_DATA " %s"), topicBuf, data_len, dataBuf);
 
//   // if (XdrvMqttData(topicBuf, sizeof(topicBuf), dataBuf, sizeof(dataBuf))) { return; }

//   // grpflg = (strstr(topicBuf, Settings.mqtt_grptopic) != nullptr);

//   // GetFallbackTopic_P(stemp1, CMND, "");  // Full Fallback topic = cmnd/DVES_xxxxxxxx_fb/
//   // fallback_topic_flag = (!strncmp(topicBuf, stemp1, strlen(stemp1)));

//   type = strrchr(topicBuf, '/');  // Last part of received topic is always the command (type)

// /******************************************************************************************************
//  *******Extract <index> from topicBuf with isdigit ***********************************************************************************************
//  ******************************************************************************************************/
 
//   index = 1;
//   if (type != nullptr) {
//     type++;
//     // Convert everything to same case
//     for (i = 0; i < strlen(type); i++) {
//       //type[i] = toupper(type[i]);
//     }
//     // From end of array, move backwards checking for numbers (ie stops when it hits " " space)
//     while (isdigit(type[i-1])) {
//       i--;
//     }
//     // Move from start of number to end of array to get number
//     if (i < strlen(type)) {
//       index = atoi(type +i);
//       user_append_index = true;
//     }
//     type[i] = '\0';
//   }

//   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_RESULT D_GROUP " %d, " D_INDEX " %d, " 
//     D_COMMAND " %s, " D_DATA " %s"),
//    grpflg, index, type, dataBuf);

  // if (type != nullptr) {
  //   Response_P(PSTR("{\"" D_JSON_COMMAND "\":\"" D_JSON_ERROR "\"}"));
  //   //if (Settings.ledstate &0x02) { blinks++; }

  //   if (!strcmp(dataBuf,"?")) { data_len = 0; }
  //   int16_t payload = -99;               // No payload
  //   uint16_t payload16 = 0;
  //   long payload32 = strtol(dataBuf, &p, 0);  // decimal, octal (0) or hex (0x)
  //   if (p != dataBuf) {
  //     payload = (int16_t) payload32;     // -32766 - 32767
  //     payload16 = (uint16_t) payload32;  // 0 - 65535
  //   } else {
  //     payload32 = 0;
  //   }
  //   backlog_delay = millis() + (100 * MIN_BACKLOG_DELAY);

  //   int temp_payload = GetStateNumber(dataBuf);
  //   if (temp_payload > -1) { payload = temp_payload; }

//    AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_RESULT "Payload %d, Payload16 %d, payload32 %u"), payload, payload16, payload32);

// New direct commands under devlopment

  //if(strstr(type,"mrgbanimator_pixels_scene_hue")){
  // if(strstr(type, D_WEBARG_SCENE_HUE)){
  //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RESULT D_PARSING_MATCHED D_WEBARG_SCENE_HUE));
  
  //   Serial.println("if(strstr(type,\"mrgbanimator_pixels_scene_hue\")){");
  //   // Fill mqtt with command, then set data waiting    
  //   memset(&data_buffer.payload,0,sizeof(data_buffer.payload));
  //   sprintf(data_buffer.topic.ctr,"%s/set/pixels/scene",DEVICENAME_CTR);
  //   sprintf(data_buffer.payload.ctr,"{\"name\":\"COLOURSCENE\",\"hue\":%s}",dataBuf);
  //   data_buffer.topic.len = strlen(data_buffer.topic.ctr);
  //   data_buffer.payload.len = strlen(data_buffer.payload.ctr);
  //   data_buffer.flags.waiting = true;



  // }



//     int command_code = GetCommandCode(command, sizeof(command), type, kTasmotaCommands);
//     if (-1 == command_code) {
// //      XdrvMailbox.valid = 1;
//       XdrvMailbox.index = index;
//       XdrvMailbox.data_len = data_len;
//       XdrvMailbox.payload16 = payload16;
//       XdrvMailbox.payload = payload;
//       XdrvMailbox.grpflg = grpflg;
//       XdrvMailbox.topic = type;
//       XdrvMailbox.data = dataBuf;
//       if (!XdrvCall(FUNC_COMMAND)) {
//         if (!XsnsCall(FUNC_COMMAND)) {
//           type = nullptr;  // Unknown command
//         }
//       }
//     }
//     else if (CMND_BACKLOG == command_code) {
//       if (data_len) {
//         uint8_t bl_pointer = (!backlog_pointer) ? MAX_BACKLOG -1 : backlog_pointer;
//         bl_pointer--;
//         char *blcommand = strtok(dataBuf, ";");
//         while ((blcommand != nullptr) && (backlog_index != bl_pointer)) {
//           while(true) {
//             blcommand = Trim(blcommand);
//             if (!strncasecmp_P(blcommand, PSTR(D_JSON_BACKLOG), strlen(D_JSON_BACKLOG))) {
//               blcommand += strlen(D_JSON_BACKLOG);                                  // Skip unnecessary command Backlog
//             } else {
//               break;
//             }
//           }
//           if (*blcommand != '\0') {
//             backlog[backlog_index] = String(blcommand);
//             backlog_index++;
//             if (backlog_index >= MAX_BACKLOG) backlog_index = 0;
//           }
//           blcommand = strtok(nullptr, ";");
//         }
// //        Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_APPENDED);
//         data_buffer.payload.ctr[0] = '\0';
//       } else {
//         uint8_t blflag = (backlog_pointer == backlog_index);
//         backlog_pointer = backlog_index;
//         Response_P(S_JSON_COMMAND_SVALUE, command, blflag ? D_JSON_EMPTY : D_JSON_ABORTED);
//       }
//     }
//     else if (CMND_DELAY == command_code) {
//       if ((payload >= MIN_BACKLOG_DELAY) && (payload <= 3600)) {
//         backlog_delay = millis() + (100 * payload);
//       }
//       uint16_t bl_delay = 0;
//       long bl_delta = TimePassedSince(backlog_delay);
//       if (bl_delta < 0) { bl_delay = (bl_delta *-1) / 100; }
//       Response_P(S_JSON_COMMAND_NVALUE, command, bl_delay);
//     }
//     else if ((CMND_POWER == command_code) && (index > 0) && (index <= devices_present)) {
//       if ((payload < 0) || (payload > 4)) { payload = 9; }
// //      Settings.flag_system.device_index_enable = user_append_index;
//       ExecuteCommandPower(index, payload, SRC_IGNORE);
//       fallback_topic_flag = false;
//       return;
//     }
//     else if ((CMND_FANSPEED == command_code) && (SONOFF_IFAN02 == my_module_type)) {
//       if (data_len > 0) {
//         if ('-' == dataBuf[0]) {
//           payload = (int16_t)GetFanspeed() -1;
//           if (payload < 0) { payload = MAX_FAN_SPEED -1; }
//         }
//         else if ('+' == dataBuf[0]) {
//           payload = GetFanspeed() +1;
//           if (payload > MAX_FAN_SPEED -1) { payload = 0; }
//         }
//       }
//       if ((payload >= 0) && (payload < MAX_FAN_SPEED) && (payload != GetFanspeed())) {
//         SetFanspeed(payload);
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, GetFanspeed());
//     }
//     else if (CMND_STATUS == command_code) {
//       if ((payload < 0) || (payload > MAX_STATUS)) payload = 99;
//       PublishStatus(payload);
//       fallback_topic_flag = false;
//       return;
//     }
//     else if (CMND_STATE == command_code) {
//       data_buffer.payload.ctr[0] = '\0';
//       MqttShowState();
//       if (Settings.flag_network.hass_tele_on_power) {
//         MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_STATE), MQTT_TELE_RETAIN);
//       }
// #ifdef USE_HOME_ASSISTANT
//       if (Settings.flag_system.hass_discovery) {
//         HAssPublishStatus();
//       }
// #endif  // USE_HOME_ASSISTANT
//     }
//     else if (CMND_SLEEP == command_code) {
//       if ((payload >= 0) && (payload < 251)) {
//         Settings.sleep = payload;
//         sleep = payload;
//         WiFiSetSleepMode();
//       }
//       Response_P(S_JSON_COMMAND_NVALUE_UNIT_NVALUE_UNIT, command, sleep, (Settings.flag_system.value_units) ? " " D_UNIT_MILLISECOND : "", Settings.sleep, (Settings.flag_system.value_units) ? " " D_UNIT_MILLISECOND : "");
//     }
//     else if ((CMND_UPGRADE == command_code) || (CMND_UPLOAD == command_code)) {
//       // Check if the payload is numerically 1, and had no trailing chars.
//       //   e.g. "1foo" or "1.2.3" could fool us.
//       // Check if the version we have been asked to upgrade to is higher than our current version.
//       //   We also need at least 3 chars to make a valid version number string.
//       if (((1 == data_len) && (1 == payload)) || ((data_len >= 3) && NewerVersion(dataBuf))) {
//         ota_state_flag = 3;
//         Response_P(PSTR("{\"%s\":\"" D_JSON_VERSION " %s " D_JSON_FROM " %s\"}"), command, my_version, GetOtaUrl(stemp1, sizeof(stemp1)));
//       } else {
//         Response_P(PSTR("{\"%s\":\"" D_JSON_ONE_OR_GT "\"}"), command, my_version);
//       }
//     }
//     else if (CMND_OTAURL == command_code) {
//       if ((data_len > 0) && (data_len < sizeof(Settings.ota_url))) {
//         strlcpy(Settings.ota_url, (SC_DEFAULT == Shortcut(dataBuf)) ? OTA_URL : dataBuf, sizeof(Settings.ota_url));
//       }
//       Response_P(S_JSON_COMMAND_SVALUE, command, Settings.ota_url);
//     }
//     else if (CMND_SERIALLOG == command_code) {
//       if ((payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_ALL)) {
//         Settings.flag_system.mqtt_serial = 0;
//         SetSeriallog(payload);
//       }
//       Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, command, Settings.seriallog_level, seriallog_level);
//     }
//     else if (CMND_RESTART == command_code) {
//       switch (payload) {
//       case 1:
//         restart_flag = 2;
//         Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_RESTARTING);
//         break;
//       case 99:
//         AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_RESTARTING));
//         EspRestart();
//         break;
//       default:
//         Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_ONE_TO_RESTART);
//       }
//     }
//     else if ((CMND_POWERONSTATE == command_code) && (my_module_type != MOTOR)) {
//       /* 0 = Keep relays off after power on
//        * 1 = Turn relays on after power on, if PulseTime set wait for PulseTime seconds, and turn relays off
//        * 2 = Toggle relays after power on
//        * 3 = Set relays to last saved state after power on
//        * 4 = Turn relays on and disable any relay control (used for Sonoff Pow to always measure power)
//        * 5 = Keep relays off after power on, if PulseTime set wait for PulseTime seconds, and turn relays on
//        */
//       if ((payload >= POWER_ALL_OFF) && (payload <= POWER_ALL_OFF_PULSETIME_ON)) {
//         Settings.poweronstate = payload;
//         if (POWER_ALL_ALWAYS_ON == Settings.poweronstate) {
//           for (uint32_t i = 1; i <= devices_present; i++) {
//             ExecuteCommandPower(i, POWER_ON, SRC_IGNORE);
//           }
//         }
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.poweronstate);
//     }
//     else if ((CMND_PULSETIME == command_code) && (index > 0) && (index <= MAX_PULSETIMERS)) {
//       if (data_len > 0) {
//         Settings.pulse_timer[index -1] = payload16;  // 0 - 65535
//         SetPulseTimer(index -1, payload16);
//       }
//       Response_P(S_JSON_COMMAND_INDEX_NVALUE_ACTIVE_NVALUE, command, index, Settings.pulse_timer[index -1], GetPulseTimer(index -1));
//     }
//     else if (CMND_BLINKTIME == command_code) {
//       if ((payload > 1) && (payload <= 3600)) {
//         Settings.blinktime = payload;
//         if (blink_timer > 0) { blink_timer = millis() + (100 * payload); }
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.blinktime);
//     }
//     else if (CMND_BLINKCOUNT == command_code) {
//       if (data_len > 0) {
//         Settings.blinkcount = payload16;  // 0 - 65535
//         if (blink_counter) blink_counter = Settings.blinkcount *2;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.blinkcount);
//     }
//     else if (CMND_SAVEDATA == command_code) {
//       if ((payload >= 0) && (payload <= 3600)) {
//         Settings.save_data = payload;
//         save_data_counter = Settings.save_data;
//       }
//       SettingsSaveAll();
//       if (Settings.save_data > 1) {
//         snprintf_P(stemp1, sizeof(stemp1), PSTR(D_JSON_EVERY " %d " D_UNIT_SECOND), Settings.save_data);
//       }
//       Response_P(S_JSON_COMMAND_SVALUE, command, (Settings.save_data > 1) ? stemp1 : GetStateText(Settings.save_data));
//     }
//     else if ((CMND_SENSOR == command_code) || (CMND_DRIVER == command_code)) {
//       XdrvMailbox.index = index;
//       XdrvMailbox.data_len = data_len;
//       XdrvMailbox.payload16 = payload16;
//       XdrvMailbox.payload = payload;
//       XdrvMailbox.grpflg = grpflg;
//       XdrvMailbox.topic = command;
//       XdrvMailbox.data = dataBuf;
//       if (CMND_SENSOR == command_code) {
//         XsnsCall(FUNC_COMMAND_SENSOR);
//       } else {
//         XdrvCall(FUNC_COMMAND_DRIVER);
//       }
//     }
//     else if ((CMND_SETOPTION == command_code) && (index < 82)) {
//       uint8_t ptype;
//       uint8_t pindex;
//       if (index <= 31) {         // SetOption0 .. 31 = Settings.flag
//         ptype = 0;
//         pindex = index;          // 0 .. 31
//       }
//       else if (index <= 49) {    // SetOption32 .. 49 = Settings.param
//         ptype = 2;
//         pindex = index -32;      // 0 .. 17 (= PARAM8_SIZE -1)
//       }
//       else {                     // SetOption50 .. 81 = Settings.flag3
//         ptype = 1;
//         pindex = index -50;      // 0 .. 31
//       }
//       if (payload >= 0) {
//         if (0 == ptype) {        // SetOption0 .. 31
//           if (payload <= 1) {
//             switch (pindex) {
//               case 5:            // mqtt_power_retain (CMND_POWERRETAIN)
//               case 6:            // mqtt_button_retain (CMND_BUTTONRETAIN)
//               case 7:            // mqtt_switch_retain (CMND_SWITCHRETAIN)
//               case 9:            // mqtt_sensor_retain (CMND_SENSORRETAIN)
//               case 14:           // interlock (CMND_INTERLOCK)
//               case 22:           // mqtt_serial (SerialSend and SerialLog)
//               case 23:           // mqtt_serial_raw (SerialSend)
//               case 25:           // knx_enabled (Web config)
//               case 27:           // knx_enable_enhancement (Web config)
//                 ptype = 99;      // Command Error
//                 break;           // Ignore command SetOption
//               case 3:            // mqtt
//               case 15:           // pwm_control
//                 restart_flag = 2;
//               default:
//                 bitWrite(Settings.flag_system.data, pindex, payload);
//             }
//             if (12 == pindex) {  // stop_flash_rotate
//               stop_flash_rotate = payload;
//               SettingsSave(2);
//             }
// #ifdef USE_HOME_ASSISTANT
//             if ((19 == pindex) || (30 == pindex)) {
//               HAssDiscover();    // Delayed execution to provide enough resources during hass_discovery or hass_light
//             }
// #endif  // USE_HOME_ASSISTANT
//           }
//         }
//         else if (1 == ptype) {   // SetOption50 .. 81
//           if (payload <= 1) {
//             bitWrite(Settings.flag_network.data, pindex, payload);
//             if (5 == pindex) {   // SetOption55
//               if (0 == payload) {
//                 restart_flag = 2;  // Disable mDNS needs restart
//               }
//             }
//             if (10 == pindex) {  // SetOption60 enable or disable traditional sleep
//               WiFiSetSleepMode();  // Update WiFi sleep mode accordingly
//             }
//           }
//         }
//         else {                   // SetOption32 .. 49
//           uint8_t param_low = 0;
//           uint8_t param_high = 255;
//           switch (pindex) {
//             case P_HOLD_TIME:
//             case P_MAX_POWER_RETRY:
//               param_low = 1;
//               param_high = 250;
//               break;
//           }
//           if ((payload >= param_low) && (payload <= param_high)) {
//             Settings.setoption_255[pindex] = payload;
//             switch (pindex) {
// #ifdef USE_LIGHT
//              case P_RGB_REMAP:
//                 LightUpdateColorMapping();
//                 break;
// #endif
// #if defined(USE_IR_REMOTE) && defined(USE_IR_RECEIVE)
//               case P_IR_UNKNOW_THRESHOLD:
//                 IrReceiveUpdateThreshold();
//                 break;
// #endif
//             }
//           }
//         }
//       }
//       if (ptype < 99) {
//         if (2 == ptype) snprintf_P(stemp1, sizeof(stemp1), PSTR("%d"), Settings.setoption_255[pindex]);
//         Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, (2 == ptype) ? stemp1 : (1 == ptype) ? GetStateText(bitRead(Settings.flag_network.data, pindex)) : GetStateText(bitRead(Settings.flag_system.data, pindex)));
//       }
//     }
//     else if (CMND_TEMPERATURE_RESOLUTION == command_code) {
//       if ((payload >= 0) && (payload <= 3)) {
//         Settings.flag_power.temperature_resolution = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag_power.temperature_resolution);
//     }
//     else if (CMND_HUMIDITY_RESOLUTION == command_code) {
//       if ((payload >= 0) && (payload <= 3)) {
//         Settings.flag_power.humidity_resolution = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag_power.humidity_resolution);
//     }
//     else if (CMND_PRESSURE_RESOLUTION == command_code) {
//       if ((payload >= 0) && (payload <= 3)) {
//         Settings.flag_power.pressure_resolution = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag_power.pressure_resolution);
//     }
//     else if (CMND_POWER_RESOLUTION == command_code) {
//       if ((payload >= 0) && (payload <= 3)) {
//         Settings.flag_power.wattage_resolution = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag_power.wattage_resolution);
//     }
//     else if (CMND_VOLTAGE_RESOLUTION == command_code) {
//       if ((payload >= 0) && (payload <= 3)) {
//         Settings.flag_power.voltage_resolution = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag_power.voltage_resolution);
//     }
//     else if (CMND_FREQUENCY_RESOLUTION == command_code) {
//       if ((payload >= 0) && (payload <= 3)) {
//         Settings.flag_power.frequency_resolution = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag_power.frequency_resolution);
//     }
//     else if (CMND_CURRENT_RESOLUTION == command_code) {
//       if ((payload >= 0) && (payload <= 3)) {
//         Settings.flag_power.current_resolution = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag_power.current_resolution);
//     }
//     else if (CMND_ENERGY_RESOLUTION == command_code) {
//       if ((payload >= 0) && (payload <= 5)) {
//         Settings.flag_power.energy_resolution = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag_power.energy_resolution);
//     }
//     else if (CMND_WEIGHT_RESOLUTION == command_code) {
//       if ((payload >= 0) && (payload <= 3)) {
//         Settings.flag_power.weight_resolution = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.flag_power.weight_resolution);
//     }
//     else if (CMND_MODULE == command_code) {
//       if ((payload >= 0) && (payload <= MAXMODULE)) {
//         bool present = false;
//         if (0 == payload) {
//           payload = USER_MODULE;
//           present = true;
//         } else {
//           payload--;
//           present = ValidTemplateModule(payload);
//         }
//         if (present) {
//           Settings.last_module = Settings.module;
//           Settings.module = payload;
//           SetModuleType();
//           if (Settings.last_module != payload) {
//             for (uint32_t i = 0; i < sizeof(Settings.module_pins); i++) {
//               Settings.module_pins.io[i] = GPIO_NONE;
//             }
//           }
//           restart_flag = 2;
//         }
//       }
//       Response_P(S_JSON_COMMAND_NVALUE_SVALUE, command, ModuleNr(), ModuleName().c_str());
//     }
//     else if (CMND_MODULES == command_code) {
//       uint8_t midx = USER_MODULE;
//       for (uint32_t i = 0; i <= sizeof(kModuleNiceList_IDS); i++) {
//         if (i > 0) { midx = pgm_read_byte(kModuleNiceList_IDS + i -1); }
//         if (!jsflg) {
//           Response_P(PSTR("{\"" D_JSON_MODULES "%d\":["), lines);
//         } else {
//           ResponseAppend_P(PSTR(","));
//         }
//         jsflg = true;
//         uint8_t j = i ? midx +1 : 0;
//         if ((ResponseAppend_P(PSTR("\"%d (%s)\""), j, AnyModuleName(midx).c_str()) > (LOGSZ - TOPSZ)) || (i == sizeof(kModuleNiceList_IDS))) {
//           ResponseAppend_P(PSTR("]}"));
//           MqttPublishPrefixTopic_P(RESULT_OR_STAT, type);
//           jsflg = false;
//           lines++;
//         }
//       }
//       data_buffer.payload.ctr[0] = '\0';
//     }
// #ifndef USE_ADC_VCC
//     else if (CMND_ADC == command_code) {
//       if (ValidAdc() && (payload >= 0) && (payload < ADC0_END)) {
//         Settings.my_adc0 = payload;
//         restart_flag = 2;
//       }
//       Response_P(PSTR("{\"" D_JSON_ADC "0\":\"%d (%s)\"}"), Settings.my_adc0, GetTextIndexed_P(stemp1, sizeof(stemp1), Settings.my_adc0, kAdc0Names));
//     }
//     else if (CMND_ADCS == command_code) {
//       Response_P(PSTR("{\"" D_JSON_ADCS "\":["));
//       for (uint32_t i = 0; i < ADC0_END; i++) {
//         if (jsflg) {
//           ResponseAppend_P(PSTR(","));
//         }
//         jsflg = true;
//         ResponseAppend_P(PSTR("\"%d (%s)\""), i, GetTextIndexed_P(stemp1, sizeof(stemp1), i, kAdc0Names));
//       }
//       ResponseAppend_P(PSTR("]}"));
//     }
// #endif  // USE_ADC_VCC
//     else if ((CMND_GPIO == command_code) && (index < sizeof(Settings.module_pins))) {
//       myio cmodule;
//       TemplateGPIOs(&cmodule);
//       if (ValidGPIO(index, cmodule.io[index]) && (payload >= 0) && (payload < GPIO_SENSOR_END)) {
//         bool present = false;
//         for (uint32_t i = 0; i < sizeof(kGpioNiceList); i++) {
//           uint8_t midx = pgm_read_byte(kGpioNiceList + i);
//           if (midx == payload) { present = true; }
//         }
//         if (present) {
//           for (uint32_t i = 0; i < sizeof(Settings.module_pins); i++) {
//             if (ValidGPIO(i, cmodule.io[i]) && (Settings.module_pins.io[i] == payload)) {
//               Settings.module_pins.io[i] = GPIO_NONE;
//             }
//           }
//           Settings.module_pins.io[index] = payload;
//           restart_flag = 2;
//         }
//       }
//       Response_P(PSTR("{"));
//       for (uint32_t i = 0; i < sizeof(Settings.module_pins); i++) {
//         if (ValidGPIO(i, cmodule.io[i])) {
//           if (jsflg) { ResponseAppend_P(PSTR(",")); }
//           jsflg = true;
//           ResponseAppend_P(PSTR("\"" D_JSON_GPIO "%d\":\"%d (%s)\""), i, Settings.module_pins.io[i], GetTextIndexed_P(stemp1, sizeof(stemp1), Settings.module_pins.io[i], kSensorNames));
//         }
//       }
//       if (jsflg) {
//         ResponseJsonEnd();
//       } else {
//         Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_NOT_SUPPORTED);
//       }
//     }
//     else if (CMND_GPIOS == command_code) {
//       myio cmodule;
//       TemplateGPIOs(&cmodule);
//       uint8_t midx;
//       for (uint32_t i = 0; i < sizeof(kGpioNiceList); i++) {
//         midx = pgm_read_byte(kGpioNiceList + i);
//         if (!GetUsedInModule(midx, cmodule.io)) {
//           if (!jsflg) {
//             Response_P(PSTR("{\"" D_JSON_GPIOS "%d\":["), lines);
//           } else {
//             ResponseAppend_P(PSTR(","));
//           }
//           jsflg = true;
//           if ((ResponseAppend_P(PSTR("\"%d (%s)\""), midx, GetTextIndexed_P(stemp1, sizeof(stemp1), midx, kSensorNames)) > (LOGSZ - TOPSZ)) || (i == sizeof(kGpioNiceList) -1)) {
//             ResponseAppend_P(PSTR("]}"));
//             MqttPublishPrefixTopic_P(RESULT_OR_STAT, type);
//             jsflg = false;
//             lines++;
//           }
//         }
//       }
//       data_buffer.payload.ctr[0] = '\0';
//     }
//     else if (CMND_TEMPLATE == command_code) {
//       // {"NAME":"Generic","GPIO":[17,254,29,254,7,254,254,254,138,254,139,254,254],"FLAG":1,"BASE":255}
//       bool error = false;

//       if (strstr(dataBuf, "{") == nullptr) {  // If no JSON it must be parameter
//         if ((payload > 0) && (payload <= MAXMODULE)) {
//           payload--;
//           if (ValidTemplateModule(payload)) {
//             ModuleDefault(payload);     // Copy template module
//             if (USER_MODULE == Settings.module) { restart_flag = 2; }
//           }
//         }
//         else if (0 == payload) {        // Copy current template to user template
//           if (Settings.module != USER_MODULE) {
//             ModuleDefault(Settings.module);
//           }
//         }
//         else if (255 == payload) {      // Copy current module with user configured GPIO
//           if (Settings.module != USER_MODULE) {
//             ModuleDefault(Settings.module);
//           }
//           snprintf_P(Settings.user_template.name, sizeof(Settings.user_template.name), PSTR("Merged"));
//           uint8_t j = 0;
//           for (uint32_t i = 0; i < sizeof(mycfgio); i++) {
//             if (6 == i) { j = 9; }
//             if (8 == i) { j = 12; }
//             if (my_module.io[j] > GPIO_NONE) {
//               Settings.user_template2.hardware.gp.io[i] = my_module.io[j];
//             }
//             j++;
//           }
//         }
//       }
//       else {
//         if (JsonTemplate(dataBuf)) {    // Free 336 bytes StaticJsonBuffer stack space by moving code to function
//           if (USER_MODULE == Settings.module) { restart_flag = 2; }
//         } else {
//           Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_INVALID_JSON);
//           error = true;
//         }
//       }
//       if (!error) { TemplateJson(); }
//     }
//     else if ((CMND_PWM == command_code) && pwm_present && (index > 0) && (index <= MAX_PWMS)) {
//       if ((payload >= 0) && (payload <= Settings.pwm_range) && (pin[GPIO_PWM1 + index -1] < 99)) {
//         Settings.pwm_value[index -1] = payload;
//         analogWrite(pin[GPIO_PWM1 + index -1], bitRead(pwm_inverted, index -1) ? Settings.pwm_range - payload : payload);
//       }
//       Response_P(PSTR("{"));
//       MqttShowPWMState();  // Render the PWM status to MQTT
//       ResponseJsonEnd();
//     }
//     else if (CMND_PWMFREQUENCY == command_code) {
//       if ((1 == payload) || ((payload >= PWM_MIN) && (payload <= PWM_MAX))) {
//         Settings.pwm_frequency = (1 == payload) ? PWM_FREQ : payload;
//         analogWriteFreq(Settings.pwm_frequency);   // Default is 1000 (core_esp8266_wiring_pwm.c)
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.pwm_frequency);
//     }
//     else if (CMND_PWMRANGE == command_code) {
//       if ((1 == payload) || ((payload > 254) && (payload < 1024))) {
//         Settings.pwm_range = (1 == payload) ? PWM_RANGE : payload;
//         for (uint32_t i = 0; i < MAX_PWMS; i++) {
//           if (Settings.pwm_value[i] > Settings.pwm_range) {
//             Settings.pwm_value[i] = Settings.pwm_range;
//           }
//         }
//         analogWriteRange(Settings.pwm_range);      // Default is 1023 (Arduino.h)
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.pwm_range);
//     }
//     else if ((CMND_COUNTER == command_code) && (index > 0) && (index <= MAX_COUNTERS)) {
//       if ((data_len > 0) && (pin[GPIO_CNTR1 + index -1] < 99)) {
//         if ((dataBuf[0] == '-') || (dataBuf[0] == '+')) {
//           RtcSettings.pulse_counter[index -1] += payload32;
//           Settings.pulse_counter[index -1] += payload32;
//         } else {
//           RtcSettings.pulse_counter[index -1] = payload32;
//           Settings.pulse_counter[index -1] = payload32;
//         }
//       }
//       Response_P(S_JSON_COMMAND_INDEX_LVALUE, command, index, RtcSettings.pulse_counter[index -1]);
//     }
//     else if ((CMND_COUNTERTYPE == command_code) && (index > 0) && (index <= MAX_COUNTERS)) {
//       if ((payload >= 0) && (payload <= 1) && (pin[GPIO_CNTR1 + index -1] < 99)) {
//         bitWrite(Settings.pulse_counter_type, index -1, payload &1);
//         RtcSettings.pulse_counter[index -1] = 0;
//         Settings.pulse_counter[index -1] = 0;
//       }
//       Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, index, bitRead(Settings.pulse_counter_type, index -1));
//     }
//     else if (CMND_COUNTERDEBOUNCE == command_code) {
//       if ((data_len > 0) && (payload16 < 32001)) {
//         Settings.pulse_counter_debounce = payload16;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.pulse_counter_debounce);
//     }
//     else if (CMND_BUTTONDEBOUNCE == command_code) {
//       if ((payload > 39) && (payload < 1001)) {
//         Settings.button_debounce = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.button_debounce);
//     }
//     else if (CMND_SWITCHDEBOUNCE == command_code) {
//       if ((payload > 39) && (payload < 1001)) {
//         Settings.switch_debounce = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.switch_debounce);
//     }
//     else if (CMND_BAUDRATE == command_code) {
//       if (payload32 > 1200) {
//         payload32 /= 1200;  // Make it a valid baudrate
//         baudrate = (1 == payload) ? APP_BAUDRATE : payload32 * 1200;
//         SetSerialBaudrate(baudrate);
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.baudrate * 1200);
//     }
//     else if ((CMND_SERIALSEND == command_code) && (index > 0) && (index <= 5)) {
//       SetSeriallog(LOG_LEVEL_NONE);
//       Settings.flag_system.mqtt_serial = 1;
//       Settings.flag_system.mqtt_serial_raw = (index > 3) ? 1 : 0;
//       if (data_len > 0) {
//         if (1 == index) {
//           DEBUG_PRINTF("%s\n", dataBuf);                    // "Hello Tiger\n"
//         }
//         else if (2 == index || 4 == index) {
//           for (uint32_t i = 0; i < data_len; i++) {
//             Serial.write(dataBuf[i]);                        // "Hello Tiger" or "A0"
//           }
//         }
//         else if (3 == index) {
//           uint16_t dat_len = data_len;
//           DEBUG_PRINTF("%s", Unescape(dataBuf, &dat_len));  // "Hello\f"
//         }
//         else if (5 == index) {
//           SerialSendRaw(RemoveSpace(dataBuf));               // "AA004566" as hex values
//         }
//         Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
//       }
//     }
//     else if (CMND_SERIALDELIMITER == command_code) {
//       if ((data_len > 0) && (payload < 256)) {
//         if (payload > 0) {
//           Settings.serial_delimiter = payload;
//         } else {
//           uint16_t dat_len = data_len;
//           Unescape(dataBuf, &dat_len);
//           Settings.serial_delimiter = dataBuf[0];
//         }
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.serial_delimiter);
//     }
//     else if (CMND_SYSLOG == command_code) {
//       if ((payload >= LOG_LEVEL_NONE) && (payload <= LOG_LEVEL_ALL)) {
//         SetSyslog(payload);
//       }
//       Response_P(S_JSON_COMMAND_NVALUE_ACTIVE_NVALUE, command, Settings.syslog_level, syslog_level);
//     }
//     else if (CMND_LOGHOST == command_code) {
//       if ((data_len > 0) && (data_len < sizeof(Settings.syslog_host))) {
//         strlcpy(Settings.syslog_host, (SC_DEFAULT == Shortcut(dataBuf)) ? SYS_LOG_HOST : dataBuf, sizeof(Settings.syslog_host));
//       }
//       Response_P(S_JSON_COMMAND_SVALUE, command, Settings.syslog_host);
//     }
//     else if (CMND_LOGPORT == command_code) {
//       if (payload16 > 0) {
//         Settings.syslog_port = (1 == payload16) ? SYS_LOG_PORT : payload16;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.syslog_port);
//     }
//     else if ((CMND_IPADDRESS == command_code) && (index > 0) && (index <= 4)) {
//       if (ParseIp(&address, dataBuf)) {
//         Settings.ip_address[index -1] = address;
// //        restart_flag = 2;
//       }
//       snprintf_P(stemp1, sizeof(stemp1), PSTR(" (%s)"), WiFi.localIP().toString().c_str());
//       Response_P(S_JSON_COMMAND_INDEX_SVALUE_SVALUE, command, index, IPAddress(Settings.ip_address[index -1]).toString().c_str(), (1 == index) ? stemp1:"");
//     }
//     else if ((CMND_NTPSERVER == command_code) && (index > 0) && (index <= 3)) {
//       if ((data_len > 0) && (data_len < sizeof(Settings.ntp_server[0]))) {
//         strlcpy(Settings.ntp_server[index -1], (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? (1==index)?NTP_SERVER1:(2==index)?NTP_SERVER2:NTP_SERVER3 : dataBuf, sizeof(Settings.ntp_server[0]));
//         for (i = 0; i < strlen(Settings.ntp_server[index -1]); i++) {
//           if (Settings.ntp_server[index -1][i] == ',') Settings.ntp_server[index -1][i] = '.';
//         }
// //        restart_flag = 2;  // Issue #3890
//         ntp_force_sync = true;
//       }
//       Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.ntp_server[index -1]);
//     }
//     else if (CMND_AP == command_code) {
//       if ((payload >= 0) && (payload <= 2)) {
//         switch (payload) {
//         case 0:  // Toggle
//           Settings.sta_active ^= 1;
//           break;
//         case 1:  // AP1
//         case 2:  // AP2
//           Settings.sta_active = payload -1;
//         }
//         restart_flag = 2;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE_SVALUE, command, Settings.sta_active +1, Settings.sta_ssid[Settings.sta_active]);
//     }
//     else if ((CMND_SSID == command_code) && (index > 0) && (index <= 2)) {
//       if ((data_len > 0) && (data_len < sizeof(Settings.sta_ssid[0]))) {
//         strlcpy(Settings.sta_ssid[index -1], (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? (1 == index) ? STA_SSID1 : STA_SSID2 : dataBuf, sizeof(Settings.sta_ssid[0]));
//         Settings.sta_active = index -1;
//         restart_flag = 2;
//       }
//       Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.sta_ssid[index -1]);
//     }
//     else if ((CMND_PASSWORD == command_code) && (index > 0) && (index <= 2)) {
//       if ((data_len > 4 || SC_CLEAR == Shortcut(dataBuf) || SC_DEFAULT == Shortcut(dataBuf)) && (data_len < sizeof(Settings.sta_pwd[0]))) {
//         strlcpy(Settings.sta_pwd[index -1], (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? (1 == index) ? STA_PASS1 : STA_PASS2 : dataBuf, sizeof(Settings.sta_pwd[0]));
//         Settings.sta_active = index -1;
//         restart_flag = 2;
//         Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.sta_pwd[index -1]);
//       } else {
//         Response_P(S_JSON_COMMAND_INDEX_ASTERISK, command, index);
//       }
//     }
//     else if (CMND_HOSTNAME == command_code) {
//       if (!grpflg && (data_len > 0) && (data_len < sizeof(Settings.hostname))) {
//         strlcpy(Settings.hostname, (SC_DEFAULT == Shortcut(dataBuf)) ? WIFI_HOSTNAME : dataBuf, sizeof(Settings.hostname));
//         if (strstr(Settings.hostname, "%") != nullptr) {
//           strlcpy(Settings.hostname, WIFI_HOSTNAME, sizeof(Settings.hostname));
//         }
//         restart_flag = 2;
//       }
//       Response_P(S_JSON_COMMAND_SVALUE, command, Settings.hostname);
//     }
//     else if (CMND_WIFICONFIG == command_code) {
//       if ((payload >= WIFI_RESTART) && (payload < MAX_WIFI_OPTION)) {
//         Settings.sta_config = payload;
//         wifi_state_flag = Settings.sta_config;
//         snprintf_P(stemp1, sizeof(stemp1), kWifiConfig[Settings.sta_config]);
//         Response_P(PSTR("{\"" D_JSON_WIFICONFIG "\":\"%s " D_JSON_SELECTED "\"}"), stemp1);
//         if (WifiState() > WIFI_RESTART) {
// //          ResponseAppend_P(PSTR(" after restart"));
//           restart_flag = 2;
//         }
//       } else {
//         snprintf_P(stemp1, sizeof(stemp1), kWifiConfig[Settings.sta_config]);
//         Response_P(S_JSON_COMMAND_NVALUE_SVALUE, command, Settings.sta_config, stemp1);
//       }
//     }
//     else if ((CMND_FRIENDLYNAME == command_code) && (index > 0) && (index <= MAX_FRIENDLYNAMES)) {
//       if ((data_len > 0) && (data_len < sizeof(Settings.system_name.friendly[0]))) {
//         if (1 == index) {
//           snprintf_P(stemp1, sizeof(stemp1), PSTR(FRIENDLY_NAME));
//         } else {
//           snprintf_P(stemp1, sizeof(stemp1), PSTR(FRIENDLY_NAME "%d"), index);
//         }
//         strlcpy(Settings.system_name.friendly[index -1], (SC_DEFAULT == Shortcut(dataBuf)) ? stemp1 : dataBuf, sizeof(Settings.system_name.friendly[index -1]));
//       }
//       Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.system_name.friendly[index -1]);
//     }
//     else if ((CMND_SWITCHMODE == command_code) && (index > 0) && (index <= MAX_SWITCHES)) {
//       if ((payload >= 0) && (payload < MAX_SWITCH_OPTION)) {
//         Settings.switchmode[index -1] = payload;
//       }
//       Response_P(S_JSON_COMMAND_INDEX_NVALUE, command, index, Settings.switchmode[index-1]);
//     }
//     else if (CMND_INTERLOCK == command_code) {                      // Interlock 0 - Off, Interlock 1 - On, Interlock 1,2 3,4 5,6,7
//       uint8_t max_relays = devices_present;
//       if (Settings.light_settings.type) { max_relays--; }
//       if (max_relays > sizeof(Settings.interlock[0]) * 8) { max_relays = sizeof(Settings.interlock[0]) * 8; }
//       if (max_relays > 1) {                                         // Only interlock with more than 1 relay
//         if (data_len > 0) {
//           if (strstr(dataBuf, ",") != nullptr) {                    // Interlock entry
//             for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) { Settings.interlock[i] = 0; }  // Reset current interlocks
//             char *group;
//             char *q;
//             uint8_t group_index = 0;
//             power_t relay_mask = 0;
//             for (group = strtok_r(dataBuf, " ", &q); group && group_index < MAX_INTERLOCKS; group = strtok_r(nullptr, " ", &q)) {
//               char *str;
//               for (str = strtok_r(group, ",", &p); str; str = strtok_r(nullptr, ",", &p)) {
//                 int pbit = atoi(str);
//                 if ((pbit > 0) && (pbit <= max_relays)) {           // Only valid relays
//                   pbit--;
//                   if (!bitRead(relay_mask, pbit)) {                 // Only relay once
//                     bitSet(relay_mask, pbit);
//                     bitSet(Settings.interlock[group_index], pbit);
//                   }
//                 }
//               }
//               group_index++;
//             }
//             for (uint32_t i = 0; i < group_index; i++) {
//               uint8_t minimal_bits = 0;
//               for (uint32_t j = 0; j < max_relays; j++) {
//                 if (bitRead(Settings.interlock[i], j)) { minimal_bits++; }
//               }
//               if (minimal_bits < 2) { Settings.interlock[i] = 0; }  // Discard single relay as interlock
//             }
//           } else {
//             Settings.flag_system.interlock = payload &1;                   // Enable/disable interlock
//             if (Settings.flag_system.interlock) {
//               SetDevicePower(power, SRC_IGNORE);                    // Remove multiple relays if set
//             }
//           }
//         }
//         Response_P(PSTR("{\"" D_JSON_INTERLOCK "\":\"%s\",\"" D_JSON_GROUPS "\":\""), GetStateText(Settings.flag_system.interlock));
//         uint8_t anygroup = 0;
//         for (uint32_t i = 0; i < MAX_INTERLOCKS; i++) {
//           if (Settings.interlock[i]) {
//             anygroup++;
//             ResponseAppend_P(PSTR("%s"), (anygroup > 1) ? " " : "");
//             uint8_t anybit = 0;
//             power_t mask = 1;
//             for (uint32_t j = 0; j < max_relays; j++) {
//               if (Settings.interlock[i] & mask) {
//                 anybit++;
//                 ResponseAppend_P(PSTR("%s%d"), (anybit > 1) ? "," : "", j +1);
//               }
//               mask <<= 1;
//             }
//           }
//         }
//         if (!anygroup) {
//           for (uint32_t j = 1; j <= max_relays; j++) {
//             ResponseAppend_P(PSTR("%s%d"), (j > 1) ? "," : "", j);
//           }
//         }
//         ResponseAppend_P(PSTR("\"}"));
//       } else {
//         Settings.flag_system.interlock = 0;
//         Response_P(S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag_system.interlock));
//       }
//     }
//     else if (CMND_TELEPERIOD == command_code) {
//       if ((payload >= 0) && (payload < 3601)) {
//         Settings.tele_period = (1 == payload) ? TELE_PERIOD : payload;
//         if ((Settings.tele_period > 0) && (Settings.tele_period < 10)) Settings.tele_period = 10;   // Do not allow periods < 10 seconds
//         tele_period = Settings.tele_period;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE_UNIT, command, Settings.tele_period, (Settings.flag_system.value_units) ? " " D_UNIT_SECOND : "");
//     }
//     else if (CMND_RESET == command_code) {
//       switch (payload) {
//       case 1:
//         restart_flag = 211;
//         Response_P(S_JSON_COMMAND_SVALUE, command , D_JSON_RESET_AND_RESTARTING);
//         break;
//       case 2 ... 6:
//         restart_flag = 210 + payload;
//         Response_P(PSTR("{\"" D_JSON_RESET "\":\"" D_JSON_ERASE ", " D_JSON_RESET_AND_RESTARTING "\"}"));
//         break;
//       default:
//         Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_ONE_TO_RESET);
//       }
//     }
//     else if (CMND_TIMEZONE == command_code) {
//       if ((data_len > 0) && (payload >= -13)) {
//         Settings.timezone = payload;
//         Settings.timezone_minutes = 0;
//         if (payload < 15) {
//           p = strtok (dataBuf, ":");
//           if (p) {
//             p = strtok (nullptr, ":");
//             if (p) {
//               Settings.timezone_minutes = strtol(p, nullptr, 10);
//               if (Settings.timezone_minutes > 59) { Settings.timezone_minutes = 59; }
//             }
//           }
//         } else {
//           Settings.timezone = 99;
//         }
//         ntp_force_sync = true;
//       }
//       if (99 == Settings.timezone) {
//         Response_P(S_JSON_COMMAND_NVALUE, command, Settings.timezone);
//       } else {
//         snprintf_P(stemp1, sizeof(stemp1), PSTR("%+03d:%02d"), Settings.timezone, Settings.timezone_minutes);
//         Response_P(S_JSON_COMMAND_SVALUE, command, stemp1);
//       }
//     }
//     else if ((CMND_TIMESTD == command_code) || (CMND_TIMEDST == command_code)) {
//       // TimeStd 0/1, 0/1/2/3/4, 1..12, 1..7, 0..23, +/-780
//       uint8_t ts = 0;
//       if (CMND_TIMEDST == command_code) { ts = 1; }
//       if (data_len > 0) {
//         if (strstr(dataBuf, ",") != nullptr) {   // Process parameter entry
//           uint8_t tpos = 0;                      // Parameter index
//           int value = 0;
//           p = dataBuf;                           // Parameters like "1, 2,3 , 4 ,5, -120" or ",,,,,+240"
//           char *q = p;                           // Value entered flag
//           while (p && (tpos < 7)) {
//             if (p > q) {                         // Any value entered
//               if (1 == tpos) { Settings.tflag[ts].hemis = value &1; }
//               if (2 == tpos) { Settings.tflag[ts].week = (value < 0) ? 0 : (value > 4) ? 4 : value; }
//               if (3 == tpos) { Settings.tflag[ts].month = (value < 1) ? 1 : (value > 12) ? 12 : value; }
//               if (4 == tpos) { Settings.tflag[ts].dow = (value < 1) ? 1 : (value > 7) ? 7 : value; }
//               if (5 == tpos) { Settings.tflag[ts].hour = (value < 0) ? 0 : (value > 23) ? 23 : value; }
//               if (6 == tpos) { Settings.toffset[ts] = (value < -900) ? -900 : (value > 900) ? 900 : value; }
//             }
//             p = Trim(p);                        // Skip spaces
//             if (tpos && (*p == ',')) { p++; }    // Skip separator
//             p = Trim(p);                        // Skip spaces
//             q = p;                               // Reset any value entered flag
//             value = strtol(p, &p, 10);
//             tpos++;                              // Next parameter
//           }
//           ntp_force_sync = true;
//         } else {
//           if (0 == payload) {
//             if (0 == ts) {
//               SettingsResetStd();
//             } else {
//               SettingsResetDst();
//             }
//           }
//           ntp_force_sync = true;
//         }
//       }
//       Response_P(PSTR("{\"%s\":{\"Hemisphere\":%d,\"Week\":%d,\"Month\":%d,\"Day\":%d,\"Hour\":%d,\"Offset\":%d}}"),
//         command, Settings.tflag[ts].hemis, Settings.tflag[ts].week, Settings.tflag[ts].month, Settings.tflag[ts].dow, Settings.tflag[ts].hour, Settings.toffset[ts]);
//     }
//     else if (CMND_ALTITUDE == command_code) {
//       if ((data_len > 0) && ((payload >= -30000) && (payload <= 30000))) {
//         Settings.altitude = payload;
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.altitude);
//     }
//     else if ((CMND_LEDPOWER == command_code) && (index > 0) && (index <= MAX_LEDS)) {
// /*
//       if ((payload >= 0) && (payload <= 2)) {
//         Settings.ledstate &= 8;
//         switch (payload) {
//         case 0: // Off
//         case 1: // On
//           Settings.ledstate = payload << 3;
//           break;
//         case 2: // Toggle
//           Settings.ledstate ^= 8;
//           break;
//         }
//         blinks = 0;
//         SetLedPowerIdx(index -1, Settings.ledstate &8);
//       }
//       Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, GetStateText(bitRead(Settings.ledstate, 3)));
// */
// /*
//       if (99 == pin[GPIO_LEDLNK]) {
//         if ((payload >= 0) && (payload <= 2)) {
//           Settings.ledstate &= 8;
//           switch (payload) {
//           case 0: // Off
//           case 1: // On
//             Settings.ledstate = payload << 3;
//             break;
//           case 2: // Toggle
//             Settings.ledstate ^= 8;
//             break;
//           }
//           blinks = 0;
//           SetLedPower(Settings.ledstate &8);
//         }
//         Response_P(S_JSON_COMMAND_SVALUE, command, GetStateText(bitRead(Settings.ledstate, 3)));
//       } else {
//         if ((payload >= 0) && (payload <= 2)) {
//           Settings.ledstate &= 8;                // Disable power control
//           uint8_t mask = 1 << (index -1);        // Led to control
//           switch (payload) {
//           case 0: // Off
//             led_power &= (0xFF ^ mask);
//           case 1: // On
//             led_power |= mask;
//             break;
//           case 2: // Toggle
//             led_power ^= mask;
//             break;
//           }
//           blinks = 0;
//           SetLedPowerIdx(index -1, (led_power & mask));
//         }
//         Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, GetStateText(bitRead(led_power, index -1)));
//       }
// */
//       if (99 == pin[GPIO_LEDLNK]) { index = 1; }
//       if ((payload >= 0) && (payload <= 2)) {
//         Settings.ledstate &= 8;                // Disable power control
//         uint8_t mask = 1 << (index -1);        // Led to control
//         switch (payload) {
//         case 0: // Off
//           led_power &= (0xFF ^ mask);
//           Settings.ledstate = 0;
//           break;
//         case 1: // On
//           led_power |= mask;
//           Settings.ledstate = 8;
//           break;
//         case 2: // Toggle
//           led_power ^= mask;
//           Settings.ledstate ^= 8;
//           break;
//         }
//         blinks = 0;
//         if (99 == pin[GPIO_LEDLNK]) {
//           SetLedPower(Settings.ledstate &8);
//         } else {
//           SetLedPowerIdx(index -1, (led_power & mask));
//         }
//       }
//       uint8_t state = bitRead(led_power, index -1);
//       if (99 == pin[GPIO_LEDLNK]) {
//         state = bitRead(Settings.ledstate, 3);
//       }
//       Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, GetStateText(state));
//     }
//     else if (CMND_LEDSTATE == command_code) {
//       if ((payload >= 0) && (payload < MAX_LED_OPTION)) {
//         Settings.ledstate = payload;
//         if (!Settings.ledstate) {
//           SetLedPowerAll(0);
//           SetLedLink(0);
//         }
//       }
//       Response_P(S_JSON_COMMAND_NVALUE, command, Settings.ledstate);
//     }
//     else if (CMND_LEDMASK == command_code) {
//       if (data_len > 0) {
//         Settings.ledmask = payload16;
//       }
//       snprintf_P(stemp1, sizeof(stemp1), PSTR("%d (0x%04X)"), Settings.ledmask, Settings.ledmask);
//       Response_P(S_JSON_COMMAND_SVALUE, command, stemp1);
//     }
// #ifdef USE_I2C
//     else if ((CMND_I2CSCAN == command_code) && i2c_enabled) {
//       I2cScan(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr));
//     }
// #endif  // USE_I2C
//     else type = nullptr;  // Unknown command
//  } // end commands parse
  // if (type == nullptr) {
  //  // blinks = 201;
  //   snprintf_P(topicBuf, sizeof(topicBuf), PSTR(D_JSON_COMMAND));
  //   Response_P(PSTR("{\"" D_JSON_COMMAND "\":\"" D_JSON_UNKNOWN "\"}"));
  //   type = (char*)topicBuf;
  // }
  //if (data_buffer.payload.ctr[0] != '\0') {
    // MqttPublishPrefixTopic_P(RESULT_OR_STAT, type);
#ifdef USE_SCRIPT
     XdrvRulesProcess();
#endif
  //}
  //fallback_topic_flag = false;
}

