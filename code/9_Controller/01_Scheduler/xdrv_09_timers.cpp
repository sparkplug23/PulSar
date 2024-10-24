
// #ifdef ESP8266
// #ifdef USE_TIMERS
// /*********************************************************************************************\
//  * Timers
//  *
//  * Arm a timer using one or all of the following JSON values:
//  * {"Arm":1,"Mode":0,"Time":"09:23","Window":0,"Days":"--TW--S","Repeat":1,"Output":1,"Action":1}
//  *
//  * Arm     0 = Off, 1 = On
//  * Mode    0 = Schedule, 1 = Sunrise, 2 = Sunset
//  * Time    hours:minutes
//  * Window  minutes (0..15)
//  * Days    7 day character mask starting with Sunday (SMTWTFS). 0 or - = Off, any other value = On
//  * Repeat  0 = Execute once, 1 = Execute again
//  * Output  1..16
//  * Action  0 = Off, 1 = On, 2 = Toggle, 3 = Blink or Rule if USE_RULES enabled
//  *
// \*********************************************************************************************/

// #define XDRV_09             9

// enum TimerCommands { CMND_TIMER, CMND_TIMERS
// #ifdef USE_SUNRISE
// , CMND_LATITUDE, CMND_LONGITUDE
// #endif
//  };
// const char kTimerCommands[] PROGMEM = D_TIMER "|" D_TIMERS
// #ifdef USE_SUNRISE
// "|" D_LATITUDE "|" D_LONGITUDE
// #endif
// ;

// uint16_t timer_last_minute = 60;
// int8_t timer_window[MAX_TIMERS] = { 0 };

// // #ifdef USE_SUNRISE
// // /*********************************************************************************************\
// //  * Sunrise and sunset (+13k code)
// //  *
// //  * https://forum.arduino.cc/index.php?topic=218280.0
// //  * Source: C-Programm von http://lexikon.astronomie.info/zeitgleichung/neu.html
// //  *         Rewrite for Arduino by 'jurs' for German Arduino forum
// // \*********************************************************************************************/
// //
// // const double pi2 = TWO_PI;
// // const double pi = PI;
// // const double RAD = DEG_TO_RAD;
// //
// // double JulianischesDatum(void)
// // {
// //   // Gregorianischer Kalender
// //   int Gregor;
// //   int Jahr = RtcTime.year;
// //   int Monat = RtcTime.month;
// //   int Tag = RtcTime.day_of_month;
// //
// //   if (Monat <= 2) {
// //     Monat += 12;
// //     Jahr -= 1;
// //   }
// //   Gregor = (Jahr / 400) - (Jahr / 100) + (Jahr / 4);  // Gregorianischer Kalender
// //   return 2400000.5 + 365.0*Jahr - 679004.0 + Gregor + (int)(30.6001 * (Monat +1)) + Tag + 0.5;
// // }
// //
// // double InPi(double x)
// // {
// //   int n = (int)(x / pi2);
// //   x = x - n*pi2;
// //   if (x < 0) x += pi2;
// //   return x;
// // }
// //
// // double eps(double T)
// // {
// //   // Neigung der Erdachse
// //   return RAD * (23.43929111 + (-46.8150*T - 0.00059*T*T + 0.001813*T*T*T)/3600.0);
// // }
// //
// // double BerechneZeitgleichung(double *DK,double T)
// // {
// //   double RA_Mittel = 18.71506921 + 2400.0513369*T +(2.5862e-5 - 1.72e-9*T)*T*T;
// //   double M = InPi(pi2 * (0.993133 + 99.997361*T));
// //   double L = InPi(pi2 * (0.7859453 + M/pi2 + (6893.0*sin(M)+72.0*sin(2.0*M)+6191.2*T) / 1296.0e3));
// //   double e = eps(T);
// //   double RA = atan(tan(L)*cos(e));
// //   if (RA < 0.0) RA += pi;
// //   if (L > pi) RA += pi;
// //   RA = 24.0*RA/pi2;
// //   *DK = asin(sin(e)*sin(L));
// //   // Damit 0<=RA_Mittel<24
// //   RA_Mittel = 24.0 * InPi(pi2*RA_Mittel/24.0)/pi2;
// //   double dRA = RA_Mittel - RA;
// //   if (dRA < -12.0) dRA += 24.0;
// //   if (dRA > 12.0) dRA -= 24.0;
// //   dRA = dRA * 1.0027379;
// //   return dRA;
// // }
// //
// // void DuskTillDawn(uint8_t *hour_up,uint8_t *minute_up, uint8_t *hour_down, uint8_t *minute_down)
// // {
// //   double JD2000 = 2451545.0;
// //   double JD = JulianischesDatum();
// //   double T = (JD - JD2000) / 36525.0;
// //   double DK;
// //   /*
// //   h (D) = -0.8333 normaler SA & SU-Gang
// //   h (D) = -6.0 civile Dämmerung
// //   h (D) = -12.0 nautische Dämmerung
// //   h (D) = -18.0 astronomische Dämmerung
// //   */
// // //  double h = -50/60.0*RAD;
// //   double h = SUNRISE_DAWN_ANGLE *RAD;
// //   double B = (((double)Settings.latitude)/1000000) * RAD; // geographische Breite
// //   double GeographischeLaenge = ((double)Settings.longitude)/1000000;
// // //  double Zeitzone = 0; //Weltzeit
// // //  double Zeitzone = 1; //Winterzeit
// // //  double Zeitzone = 2.0;   //Sommerzeit
// //   double Zeitzone = ((double)time_timezone) / 60;
// //   double Zeitgleichung = BerechneZeitgleichung(&DK, T);
// //   double Zeitdifferenz = 12.0*acos((sin(h) - sin(B)*sin(DK)) / (cos(B)*cos(DK)))/pi; // time difference
// //   double AufgangOrtszeit = 12.0 - Zeitdifferenz - Zeitgleichung;
// //   double UntergangOrtszeit = 12.0 + Zeitdifferenz - Zeitgleichung; //Downfall local time
// //   double AufgangWeltzeit = AufgangOrtszeit - GeographischeLaenge / 15.0;
// //   double UntergangWeltzeit = UntergangOrtszeit - GeographischeLaenge / 15.0;
// //   double Aufgang = AufgangWeltzeit + Zeitzone;         // In Stunden
// //   if (Aufgang < 0.0) {
// //     Aufgang += 24.0;
// //   } else {
// //     if (Aufgang >= 24.0) Aufgang -= 24.0;
// //   }
// //   double Untergang = UntergangWeltzeit + Zeitzone;
// //   if (Untergang < 0.0) {
// //     Untergang += 24.0;
// //   } else {
// //     if (Untergang >= 24.0) Untergang -= 24.0;
// //   }
// //   int AufgangMinuten = (int)(60.0*(Aufgang - (int)Aufgang)+0.5);
// //   int AufgangStunden = (int)Aufgang;
// //   if (AufgangMinuten >= 60.0) {
// //     AufgangMinuten -= 60.0;
// //     AufgangStunden++;
// //   } else {
// //     if (AufgangMinuten < 0.0) {
// //       AufgangMinuten += 60.0;
// //       AufgangStunden--;
// //       if (AufgangStunden < 0.0) AufgangStunden += 24.0;
// //     }
// //   }
// //   int UntergangMinuten = (int)(60.0*(Untergang - (int)Untergang)+0.5);
// //   int UntergangStunden = (int)Untergang;
// //   if (UntergangMinuten >= 60.0) {
// //     UntergangMinuten -= 60.0;
// //     UntergangStunden++;
// //   } else {
// //     if (UntergangMinuten<0) {
// //       UntergangMinuten += 60.0;
// //       UntergangStunden--;
// //       if (UntergangStunden < 0.0) UntergangStunden += 24.0;
// //     }
// //   }
// //   *hour_up = AufgangStunden;
// //   *minute_up = AufgangMinuten;
// //   *hour_down = UntergangStunden;
// //   *minute_down = UntergangMinuten;
// // }
// //
// // void ApplyTimerOffsets(Timer *duskdawn)
// // {
// //   uint8_t hour[2];
// //   uint8_t minute[2];
// //   Timer stored = (Timer)*duskdawn;
// //
// //   // replace hours, minutes by sunrise
// //   DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
// //   uint8_t mode = (duskdawn->mode -1) &1;
// //   duskdawn->time = (hour[mode] *60) + minute[mode];
// //
// //   // apply offsets, check for over- and underflows
// //   uint16_t timeBuffer;
// //   if ((uint16_t)stored.time > 719) {
// //     // negative offset, time after 12:00
// //     timeBuffer = (uint16_t)stored.time - 720;
// //     // check for underflow
// //     if (timeBuffer > (uint16_t)duskdawn->time) {
// //       timeBuffer = 1440 - (timeBuffer - (uint16_t)duskdawn->time);
// //       duskdawn->days = duskdawn->days >> 1;
// //       duskdawn->days |= (stored.days << 6);
// //     } else {
// //       timeBuffer = (uint16_t)duskdawn->time - timeBuffer;
// //     }
// //   } else {
// //     // positive offset
// //     timeBuffer = (uint16_t)duskdawn->time + (uint16_t)stored.time;
// //     // check for overflow
// //     if (timeBuffer > 1440) {
// //       timeBuffer -= 1440;
// //       duskdawn->days = duskdawn->days << 1;
// //       duskdawn->days |= (stored.days >> 6);
// //     }
// //   }
// //   duskdawn->time = timeBuffer;
// // }
// //
// // String GetSunTimeAtHorizon(uint8_t dawn)
// // {
// //   char stime[6];
// //
// //   uint8_t hour[2];
// //   uint8_t minute[2];
// //
// //   DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
// //   dawn &= 1;
// //   snprintf_P(stime, sizeof(stime), PSTR("%02d:%02d"), hour[dawn], minute[dawn]);
// //   return String(stime);
// // }
// //
// // uint16_t SunMinutes(uint8_t dawn)
// // {
// //   uint8_t hour[2];
// //   uint8_t minute[2];
// //
// //   DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
// //   dawn &= 1;
// //   return (hour[dawn] *60) + minute[dawn];
// // }
// //
// // #endif  // USE_SUNRISE

// /*******************************************************************************************/

// void TimerSetRandomWindow(uint8_t index)
// {
//   timer_window[index] = 0;
//   if (Settings.timer[index].window) {
//     timer_window[index] = (random(0, (Settings.timer[index].window << 1) +1)) - Settings.timer[index].window;  // -15 .. 15
//   }
// }

// void TimerSetRandomWindows(void)
// {
//   for (uint8_t i = 0; i < MAX_TIMERS; i++) { TimerSetRandomWindow(i); }
// }

// void TimerEverySecond(void)
// {
//   //Serial.println("TimerEverySecond1");
//   if (RtcTime.valid) {
//     //Serial.println("RtcTime.valid");
//     if (!RtcTime.hour && !RtcTime.minute && !RtcTime.second) { TimerSetRandomWindows(); }  // Midnight
//     if (Settings.flag_network.timers_enable && (uptime > 60) && (RtcTime.minute != timer_last_minute)) {  // Execute from one minute after restart every minute only once

//       timer_last_minute = RtcTime.minute;
//       int16_t time = (RtcTime.hour *60) + RtcTime.minute;
//       uint8_t days = 1 << (RtcTime.day_of_week -1);

//         Serial.print("timer_last_minute");
//           Serial.println(timer_last_minute);

//       for (uint8_t i = 0; i < MAX_TIMERS; i++) {
// //        if (Settings.timer[i].device >= devices_present) Settings.timer[i].data = 0;  // Reset timer due to change in devices present
//         Timer xtimer = Settings.timer[i];
//         uint16_t set_time = xtimer.time;
// // #ifdef USE_SUNRISE
// //         if ((1 == xtimer.mode) || (2 == xtimer.mode)) {  // Sunrise or Sunset
// //           ApplyTimerOffsets(&xtimer);
// //           set_time = xtimer.time;
// //         }
// // #endif
//         if (xtimer.arm) {
//           Serial.println("xtimer.arm");
//           set_time += timer_window[i];                // Add random time offset
//           if (set_time < 0) { set_time = 0; }         // Stay today;
//           if (set_time > 1439) { set_time = 1439; } //minutes in day-1
//           Serial.print("(time - set_time");Serial.println(time - set_time);
//           if (time == set_time) { Serial.println("(time == set_time");
//             if (xtimer.days & days) {
//               Settings.timer[i].arm = xtimer.repeat;
// // #ifdef USE_RULES
// //               if (3 == xtimer.power) {  // Blink becomes Rule disregarding device and allowing use of Backlog commands
// //                 Response_P(PSTR("{\"Clock\":{\"Timer\":%d}}"), i +1);
// //                 XdrvRulesProcess();
// //               } else
// // #endif  // USE_RULES
//                 if (devices_present) { ExecuteCommandPower(xtimer.device +1, xtimer.power, SRC_TIMER); }
//             }
//           }
//         }else{
//           Serial.println("TIMER NOT ARMED");
//         }//arm
//       }//timer loop
//     }else{
//       // Serial.print("Settings.flag_network.timers_enable");Serial.println(Settings.flag_network.timers_enable);
//       //   Serial.print("uptime");Serial.println(uptime);
//       //     Serial.print("RtcTime.minute");Serial.println(RtcTime.minute);
//       //       Serial.print("timer_last_minute");Serial.println(timer_last_minute);
//     }//check time
//   }else{
//     Serial.println("RTC invalid");
//   }
// }

// void PrepShowTimer(uint8_t index)
// {
//   char days[8] = { 0 };
//   char sign[2] = { 0 };
//   char soutput[80];

//   Timer xtimer = Settings.timer[index -1];

//   for (uint8_t i = 0; i < 7; i++) {
//     uint8_t mask = 1 << i;
//     snprintf(days, sizeof(days), "%s%d", days, ((xtimer.days & mask) > 0));
//   }

//   soutput[0] = '\0';
//   if (devices_present) {
//     snprintf_P(soutput, sizeof(soutput), PSTR(",\"" D_TIMER_OUTPUT "\":%d"), xtimer.device +1);
//   }
// #ifdef USE_SUNRISE
//   int16_t hour = xtimer.time / 60;
//   if ((1 == xtimer.mode) || (2 == xtimer.mode)) {  // Sunrise or Sunset
//     if (hour > 11) {
//       hour -= 12;
//       sign[0] = '-';
//     }
//   }
//   ResponseAppend_P(PSTR("\"" D_TIMER "%d\":{\"" D_TIMER_ARM "\":%d,\"" D_TIMER_MODE "\":%d,\"" D_TIMER_TIME "\":\"%s%02d:%02d\",\"" D_TIMER_WINDOW "\":%d,\"" D_TIMER_DAYS "\":\"%s\",\"" D_TIMER_REPEAT "\":%d%s,\"" D_TIMER_ACTION "\":%d}"),
//     index, xtimer.arm, xtimer.mode, sign, hour, xtimer.time % 60, xtimer.window, days, xtimer.repeat, soutput, xtimer.power);
// #else
//   ResponseAppend_P(PSTR("\"" D_TIMER "%d\":{\"" D_TIMER_ARM "\":%d,\"" D_TIMER_TIME "\":\"%02d:%02d\",\"" D_TIMER_WINDOW "\":%d,\"" D_TIMER_DAYS "\":\"%s\",\"" D_TIMER_REPEAT "\":%d%s,\"" D_TIMER_ACTION "\":%d}"),
//     index, xtimer.arm, xtimer.time / 60, xtimer.time % 60, xtimer.window, days, xtimer.repeat, soutput, xtimer.power);
// #endif  // USE_SUNRISE
// }

// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/

// bool TimerCommand(void)
// {
//   char command[CMDSZ];
//   char dataBufUc[XdrvMailbox.data_len];
//   bool serviced = true;
//   uint8_t index = XdrvMailbox.index;

//   UpperCase(dataBufUc, XdrvMailbox.data);
//   int command_code = GetCommandCode(command, sizeof(command), XdrvMailbox.topic, kTimerCommands);
//   if (-1 == command_code) {
//     serviced = false;  // Unknown command
//   }
//   else if ((CMND_TIMER == command_code) && (index > 0) && (index <= MAX_TIMERS)) {
//     uint8_t error = 0;
//     if (XdrvMailbox.data_len) {
//       if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= MAX_TIMERS)) {
//         if (XdrvMailbox.payload == 0) {
//           Settings.timer[index -1].data = 0;  // Clear timer
//         } else {
//           Settings.timer[index -1].data = Settings.timer[XdrvMailbox.payload -1].data;  // Copy timer
//         }
//       } else {
// #ifndef USE_RULES
//         if (devices_present) {
// #endif
//           StaticJsonBuffer<256> jsonBuffer;
//           JsonObject& root = jsonBuffer.parseObject(dataBufUc);
//           if (!root.success()) {
//             Response_P(PSTR("{\"" D_TIMER "%d\":\"" D_INVALID_JSON "\"}"), index); // JSON decode failed
//             error = 1;
//           }
//           else {
//             char parm_uc[10];
//             index--;
//             if (root[UpperCase_P(parm_uc, PSTR(D_TIMER_ARM))].success()) {
//               Settings.timer[index].arm = (root[parm_uc] != 0);
//             }
// #ifdef USE_SUNRISE
//             if (root[UpperCase_P(parm_uc, PSTR(D_TIMER_MODE))].success()) {
//               Settings.timer[index].mode = (uint8_t)root[parm_uc] & 0x03;
//             }
// #endif
//             if (root[UpperCase_P(parm_uc, PSTR(D_TIMER_TIME))].success()) {
//               uint16_t itime = 0;
//               int8_t value = 0;
//               uint8_t sign = 0;
//               char time_str[10];

//               strlcpy(time_str, root[parm_uc], sizeof(time_str));
//               const char *substr = strtok(time_str, ":");
//               if (substr != nullptr) {
//                 if (strchr(substr, '-')) {
//                   sign = 1;
//                   substr++;
//                 }
//                 value = atoi(substr);
//                 if (sign) { value += 12; }  // Allow entering timer offset from -11:59 to -00:01 converted to 12:01 to 23:59
//                 if (value > 23) { value = 23; }
//                 itime = value * 60;
//                 substr = strtok(nullptr, ":");
//                 if (substr != nullptr) {
//                   value = atoi(substr);
//                   if (value < 0) { value = 0; }
//                   if (value > 59) { value = 59; }
//                   itime += value;
//                 }
//               }
//               Settings.timer[index].time = itime;
//             }
//             if (root[UpperCase_P(parm_uc, PSTR(D_TIMER_WINDOW))].success()) {
//               Settings.timer[index].window = (uint8_t)root[parm_uc] & 0x0F;
//               TimerSetRandomWindow(index);
//             }
//             if (root[UpperCase_P(parm_uc, PSTR(D_TIMER_DAYS))].success()) {
//               // SMTWTFS = 1234567 = 0011001 = 00TW00S = --TW--S
//               Settings.timer[index].days = 0;
//               const char *tday = root[parm_uc];
//               uint8_t i = 0;
//               char ch = *tday++;
//               while ((ch != '\0') && (i < 7)) {
//                 if (ch == '-') { ch = '0'; }
//                 uint8_t mask = 1 << i++;
//                 Settings.timer[index].days |= (ch == '0') ? 0 : mask;
//                 ch = *tday++;
//               }
//             }
//             if (root[UpperCase_P(parm_uc, PSTR(D_TIMER_REPEAT))].success()) {
//               Settings.timer[index].repeat = (root[parm_uc] != 0);
//             }
//             if (root[UpperCase_P(parm_uc, PSTR(D_TIMER_OUTPUT))].success()) {
//               uint8_t device = ((uint8_t)root[parm_uc] -1) & 0x0F;
//               Settings.timer[index].device = (device < devices_present) ? device : 0;
//             }
//             if (root[UpperCase_P(parm_uc, PSTR(D_TIMER_ACTION))].success()) {
//               uint8_t action = (uint8_t)root[parm_uc] & 0x03;
//               Settings.timer[index].power = (devices_present) ? action : 3;  // If no devices than only allow rules
//             }

//             index++;
//           }
// #ifndef USE_RULES
//         } else {
//           Response_P(PSTR("{\"" D_TIMER "%d\":\"" D_TIMER_NO_DEVICE "\"}"), index);  // No outputs defined so nothing to control
//           error = 1;
//         }
// #endif
//       }
//     }
//     if (!error) {
//       Response_P(PSTR("{"));
//       PrepShowTimer(index);
//       ResponseAppend_P(PSTR("}"));
//     }
//   }
//   else if (CMND_TIMERS == command_code) {
//     if (XdrvMailbox.data_len) {
//       if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//         Settings.flag_network.timers_enable = XdrvMailbox.payload;
//       }
//       if (XdrvMailbox.payload == 2) {
//         Settings.flag_network.timers_enable = !Settings.flag_network.timers_enable;
//       }
//     }

//     Response_P(S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag_network.timers_enable));
//     MqttPublishPrefixTopic_P(RESULT_OR_STAT, command);

//     uint8_t jsflg = 0;
//     uint8_t lines = 1;
//     for (uint8_t i = 0; i < MAX_TIMERS; i++) {
//       if (!jsflg) {
//         Response_P(PSTR("{\"" D_TIMERS "%d\":{"), lines++);
//       } else {
//         ResponseAppend_P(PSTR(","));
//       }
//       jsflg++;
//       PrepShowTimer(i +1);
//       if (jsflg > 3) {
//         ResponseAppend_P(PSTR("}}"));
//         MqttPublishPrefixTopic_P(RESULT_OR_STAT, PSTR(D_TIMERS));
//         jsflg = 0;
//       }
//     }
//     data_buffer.payload.ctr[0] = '\0';
//   }
// #ifdef USE_SUNRISE
//   else if (CMND_LONGITUDE == command_code) {
//     if (XdrvMailbox.data_len) {
//       Settings.longitude = (int)(CharToDouble(XdrvMailbox.data) *1000000);
//     }
//     char lbuff[33];
//     dtostrfd(((double)Settings.longitude) /1000000, 6, lbuff);
//     Response_P(S_JSON_COMMAND_SVALUE, command, lbuff);
//   }
//   else if (CMND_LATITUDE == command_code) {
//     if (XdrvMailbox.data_len) {
//       Settings.latitude = (int)(CharToDouble(XdrvMailbox.data) *1000000);
//     }
//     char lbuff[33];
//     dtostrfd(((double)Settings.latitude) /1000000, 6, lbuff);
//     Response_P(S_JSON_COMMAND_SVALUE, command, lbuff);
//   }
// #endif
//   else serviced = false;  // Unknown command

//   return serviced;
// }

// /*********************************************************************************************\
//  * Presentation
// \*********************************************************************************************/

// #ifdef USE_MODULE_NETWORK_WEBSERVER
// #ifdef USE_TIMERS_WEB

// #define WEB_HANDLE_TIMER "tm"

// const char S_CONFIGURE_TIMER[] PROGMEM = D_CONFIGURE_TIMER;

// const char HTTP_BTN_MENU_TIMER[] PROGMEM =
//   "<p><form action='" WEB_HANDLE_TIMER "' method='get'><button>" D_CONFIGURE_TIMER "</button></form></p>";

// const char HTTP_TIMER_SCRIPT1[] PROGMEM =
//   "var pt=[],ct=99;"
//   "function qs(s){"                                               // Alias to save code space
//     "return document.querySelector(s);"
//   "}"
//   "function ce(i,q){"                                             // Create select option
//     "var o=document.createElement('option');"
//     "o.textContent=i;"
//     "q.appendChild(o);"
//   "}";
// #ifdef USE_SUNRISE
// const char HTTP_TIMER_SCRIPT2[] PROGMEM =
//   "function gt(){"                                                // Set hours and minutes according to mode
//     "var m,p,q;"
//     "m=qs('input[name=\"rd\"]:checked').value;"                   // Get mode
//     "p=pt[ct]&0x7FF;"                                             // Get time
//     "if(m==0){"                                                   // Time is set
//       "so(0);"                                                    // Hide offset span and allow Hour 00..23
//       "q=Math.floor(p/60);if(q<10){q='0'+q;}qs('#ho').value=q;"   // Set hours
//       "q=p%%60;if(q<10){q='0'+q;}qs('#mi').value=q;"               // Set minutes
//     "}"
//     "if((m==1)||(m==2)){"                                         // Sunrise or sunset is set
//       "so(1);"                                                    // Show offset span and allow Hour 00..11
//       "q=Math.floor(p/60);"                                       // Parse hours
//       "if(q>=12){q-=12;qs('#dr').selectedIndex=1;}"               // Negative offset
//         "else{qs('#dr').selectedIndex=0;}"
//       "if(q<10){q='0'+q;}qs('#ho').value=q;"                      // Set offset hours
//       "q=p%%60;if(q<10){q='0'+q;}qs('#mi').value=q;"               // Set offset minutes
//     "}"
//   "}"
//   "function so(b){"                                               // Hide or show offset items
//     "o=qs('#ho');"
//     "e=o.childElementCount;"
//     "if(b==1){"
//       "qs('#dr').disabled='';"
//       "if(e>12){for(i=12;i<=23;i++){o.removeChild(o.lastElementChild);}}"  // Create offset hours select options
//     "}else{"
//       "qs('#dr').disabled='disabled';"
//       "if(e<23){for(i=12;i<=23;i++){ce(i,o);}}"                   // Create hours select options
//     "}"
//   "}";
// #endif
// const char HTTP_TIMER_SCRIPT3[] PROGMEM =
//   "function st(){"                                                // Save parameters to hidden area
//     "var i,l,m,n,p,s;"
//     "m=0;s=0;"
//     "n=1<<31;if(eb('a0').checked){s|=n;}"                         // Get arm
//     "n=1<<15;if(eb('r0').checked){s|=n;}"                         // Get repeat
//     "for(i=0;i<7;i++){n=1<<(16+i);if(eb('w'+i).checked){s|=n;}}"  // Get weekdays
// #ifdef USE_SUNRISE
//     "m=qs('input[name=\"rd\"]:checked').value;"                   // Check mode
//     "s|=(qs('input[name=\"rd\"]:checked').value<<29);"            // Get mode
// #endif
//     "if(%d>0){"
//       "i=qs('#d1').selectedIndex;if(i>=0){s|=(i<<23);}"           // Get output
//       "s|=(qs('#p1').selectedIndex<<27);"                         // Get action
//     "}else{"
//       "s|=3<<27;"                                                 // Get action (rule)
//     "}"
//     "l=((qs('#ho').selectedIndex*60)+qs('#mi').selectedIndex)&0x7FF;"
//     "if(m==0){s|=l;}"                                             // Get time
// #ifdef USE_SUNRISE
//     "if((m==1)||(m==2)){"
//       "if(qs('#dr').selectedIndex>0){l+=720;}"                    // If negative offset, add 12h to given offset time
//       "s|=l&0x7FF;"                                               // Save offset instead of time
//     "}"
// #endif
//     "s|=((qs('#mw').selectedIndex)&0x0F)<<11;"                    // Get window minutes
//     "pt[ct]=s;"
//     "eb('t0').value=pt.join();"                                   // Save parameters from array to hidden area
//   "}";
// const char HTTP_TIMER_SCRIPT4[] PROGMEM =
//   "function ot(t,e){"                                             // Select tab and update elements
//     "var i,n,o,p,q,s;"
//     "if(ct<99){st();}"                                            // Save changes
//     "ct=t;"
//     "o=document.getElementsByClassName('tl');"                    // Restore style to all tabs/buttons
//     "for(i=0;i<o.length;i++){o[i].style.cssText=\"background:#%06x;color:#%06x;font-weight:normal;\"}"  // COLOR_TIMER_TAB_BACKGROUND, COLOR_TIMER_TAB_TEXT
//     "e.style.cssText=\"background:#%06x;color:#%06x;font-weight:bold;\";"  // COLOR_FORM, COLOR_TEXT, Change style to tab/button used to open content
//     "s=pt[ct];"                                                   // Get parameters from array
// #ifdef USE_SUNRISE
//     "p=(s>>29)&3;eb('b'+p).checked=1;"                            // Set mode
//     "gt();"                                                       // Set hours and minutes according to mode
// #else
//     "p=s&0x7FF;"                                                  // Get time
//     "q=Math.floor(p/60);if(q<10){q='0'+q;}qs('#ho').value=q;"     // Set hours
//     "q=p%%60;if(q<10){q='0'+q;}qs('#mi').value=q;"                // Set minutes
// #endif
//     "q=(s>>11)&0xF;if(q<10){q='0'+q;}qs('#mw').value=q;"          // Set window minutes
//     "for(i=0;i<7;i++){p=(s>>(16+i))&1;eb('w'+i).checked=p;}"      // Set weekdays
//     "if(%d>0){"
//       "p=(s>>23)&0xF;qs('#d1').value=p+1;"                        // Set output
//       "p=(s>>27)&3;qs('#p1').selectedIndex=p;"                    // Set action
//     "}"
//     "p=(s>>15)&1;eb('r0').checked=p;"                             // Set repeat
//     "p=(s>>31)&1;eb('a0').checked=p;"                             // Set arm
//   "}";
// const char HTTP_TIMER_SCRIPT5[] PROGMEM =
//   "function it(){"                                                // Initialize elements and select first tab
//     "var b,i,o,s;"
//     "pt=eb('t0').value.split(',').map(Number);"                   // Get parameters from hidden area to array
//     "s='';"
//     "for(i=0;i<%d;i++){"
//       "b='';"
//       "if(0==i){b=\" id='dP'\";}"
//       "s+=\"<button type='buttonh' class='tl' onclick='ot(\"+i+\",this)'\"+b+\">\"+(i+1)+\"</button>\""
//     "}"
//     "eb('bt').innerHTML=s;"                                       // Create tabs
//     "if(%d>0){"                                                   // Create Output and Action drop down boxes
//       "eb('oa').innerHTML=\"<b>" D_TIMER_OUTPUT "</b>&nbsp;<span><select style='width:60px;' id='d1' name='d1'></select></span>&emsp;<b>" D_TIMER_ACTION "</b>&nbsp;<select style='width:99px;' id='p1' name='p1'></select>\";"
//       "o=qs('#p1');ce('" D_OFF "',o);ce('" D_ON "',o);ce('" D_TOGGLE "',o);"  // Create offset direction select options
// #ifdef USE_RULES
//       "ce('" D_RULE "',o);"
// #else
//       "ce('" D_BLINK "',o);"
// #endif
//     "}else{"
//       "eb('oa').innerHTML=\"<b>" D_TIMER_ACTION "</b> " D_RULE "\";"  // No outputs but rule is allowed
//     "}";
// const char HTTP_TIMER_SCRIPT6[] PROGMEM =
// #ifdef USE_SUNRISE
//     "o=qs('#dr');ce('+',o);ce('-',o);"                            // Create offset direction select options
// #endif
//     "o=qs('#ho');for(i=0;i<=23;i++){ce((i<10)?('0'+i):i,o);}"     // Create hours select options
//     "o=qs('#mi');for(i=0;i<=59;i++){ce((i<10)?('0'+i):i,o);}"     // Create minutes select options
//     "o=qs('#mw');for(i=0;i<=15;i++){ce((i<10)?('0'+i):i,o);}"     // Create window minutes select options
//     "o=qs('#d1');for(i=0;i<%d;i++){ce(i+1,o);}"                   // Create outputs
//     "var a='" D_DAY3LIST "';"
//     "s='';for(i=0;i<7;i++){s+=\"<input id='w\"+i+\"' name='w\"+i+\"' type='checkbox'><b>\"+a.substring(i*3,(i*3)+3)+\"</b> \"}"
//     "eb('ds').innerHTML=s;"                                       // Create weekdays
//     "eb('dP').click();"                                           // Get the element with id='dP' and click on it
//   "}"
//   "window.onload=it;";
// const char HTTP_TIMER_STYLE[] PROGMEM =
//   ".tl{float:left;border-radius:0;border:1px solid #%06x;padding:1px;width:6.25%%;}";  // COLOR_FORM, Border color needs to be the same as Fieldset background color from HTTP_HEAD_STYLE1 (transparent won't work)
// const char HTTP_FORM_TIMER1[] PROGMEM =
//   "<fieldset style='min-width:470px;text-align:center;'>"
//   "<legend style='text-align:left;'><b>&nbsp;" D_TIMER_PARAMETERS "&nbsp;</b></legend>"
//   "<form method='post' action='" WEB_HANDLE_TIMER "' onsubmit='return st();'>"
//   "<br/><input id='e0' name='e0' type='checkbox'%s><b>" D_TIMER_ENABLE "</b><br/><br/><hr/>"
//   "<input id='t0' name='t0' value='";
// const char HTTP_FORM_TIMER2[] PROGMEM =
//   "' hidden><div id='bt' name='bt'></div><br/><br/><br/>"
//   "<div id='oa' name='oa'></div><br/>"
//   "<div>"
//   "<input id='a0' name='a0' type='checkbox'><b>" D_TIMER_ARM "</b>&emsp;"
//   "<input id='r0' name='r0' type='checkbox'><b>" D_TIMER_REPEAT "</b>"
//   "</div><br/>"
//   "<div>";
// #ifdef USE_SUNRISE
// const char HTTP_FORM_TIMER3[] PROGMEM =
//   "<fieldset style='width:%dpx;margin:auto;text-align:left;border:0;'>"
//   "<input id='b0' name='rd' type='radio' value='0' onclick='gt();'><b>" D_TIMER_TIME "</b><br/>"
//   "<input id='b1' name='rd' type='radio' value='1' onclick='gt();'><b>" D_SUNRISE "</b> (%s)<br/>"
//   "<input id='b2' name='rd' type='radio' value='2' onclick='gt();'><b>" D_SUNSET "</b> (%s)<br/>"
//   "</fieldset>"
//   "<p></p>"
//   "<span><select style='width:46px;' id='dr' name='dr'></select></span>"
//   "&nbsp;";
// #else
// const char HTTP_FORM_TIMER3[] PROGMEM =
//   "<b>" D_TIMER_TIME "</b>&nbsp;";
// #endif  // USE_SUNRISE
// const char HTTP_FORM_TIMER4[] PROGMEM =
//   "<span><select style='width:60px;' id='ho' name='ho'></select></span>"
//   "&nbsp;" D_HOUR_MINUTE_SEPARATOR "&nbsp;"
//   "<span><select style='width:60px;' id='mi' name='mi'></select></span>"
//   "&emsp;<b>+/-</b>&nbsp;"
//   "<span><select style='width:60px;' id='mw' name='mw'></select></span>"
//   "</div><br/>"
//   "<div id='ds' name='ds'></div>";

// void HandleTimerConfiguration(void)
// {
//   if (!HttpCheckPriviledgedAccess()) { return; }

//   ALOG_DBG(S_LOG_HTTP, S_CONFIGURE_TIMER);

//   if (WebServer->hasParam("save")) {
//     TimerSaveSettings();
//     HandleConfiguration();
//     return;
//   }

//   WSStartAppend_P(S_CONFIGURE_TIMER);
//   WSBufferAppend_P(HTTP_TIMER_SCRIPT1);
// #ifdef USE_SUNRISE
//   WSBufferAppend_P(HTTP_TIMER_SCRIPT2);
// #endif  // USE_SUNRISE
//   WSBufferAppend_P(HTTP_TIMER_SCRIPT3, devices_present);
//   WSBufferAppend_P(HTTP_TIMER_SCRIPT4, WebColor(COL_TIMER_TAB_BACKGROUND), WebColor(COL_TIMER_TAB_TEXT), WebColor(COL_FORM), WebColor(COL_TEXT), devices_present);
//   WSBufferAppend_P(HTTP_TIMER_SCRIPT5, MAX_TIMERS, devices_present);
//   WSBufferAppend_P(HTTP_TIMER_SCRIPT6, devices_present);
//   WSContentSendStyle_P(HTTP_TIMER_STYLE, WebColor(COL_FORM));
//   WSBufferAppend_P(HTTP_FORM_TIMER1, (Settings.flag_network.timers_enable) ? " checked" : "");
//   for (uint8_t i = 0; i < MAX_TIMERS; i++) {
//     WSBufferAppend_P(PSTR("%s%u"), (i > 0) ? "," : "", Settings.timer[i].data);
//   }
//   WSBufferAppend_P(HTTP_FORM_TIMER2);
// #ifdef USE_SUNRISE
//   WSBufferAppend_P(HTTP_FORM_TIMER3, 100 + (strlen(D_SUNSET) *12), GetSunTimeAtHorizon(0).c_str(), GetSunTimeAtHorizon(1).c_str());
// #else
//   WSBufferAppend_P(HTTP_FORM_TIMER3);
// #endif  // USE_SUNRISE
//   WSBufferAppend_P(HTTP_FORM_TIMER4);
//   WSBufferAppend_P(HTTP_FORM_END);
//   WSContentSpaceButton(BUTTON_CONFIGURATION);
//   WSContentStop();
// }

// void TimerSaveSettings(void)
// {
//   char tmp[MAX_TIMERS *12];  // Need space for MAX_TIMERS x 10 digit numbers separated by a comma
//   Timer timer;

//   Settings.flag_network.timers_enable = WebServer->hasParam("e0");
//   WebGetArg(request,"t0", tmp, sizeof(tmp));
//   char *p = tmp;
//   snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_MQTT D_TIMERS " %d"), Settings.flag_network.timers_enable);
//   for (uint8_t i = 0; i < MAX_TIMERS; i++) {
//     timer.data = strtol(p, &p, 10);
//     p++;  // Skip comma
//     if (timer.time < 1440) {
//       bool flag = (timer.window != Settings.timer[i].window);
//       Settings.timer[i].data = timer.data;
//       if (flag) TimerSetRandomWindow(i);
//     }
//     snprintf_P(log_data, sizeof(log_data), PSTR("%s,0x%08X"), log_data, Settings.timer[i].data);
//   }
//   AddLogAddLog(LOG_LEVEL_DEBUG);
// }
// #endif  // USE_TIMERS_WEB
// #endif  // USE_MODULE_NETWORK_WEBSERVER

// /*********************************************************************************************\
//  * Interface
// \*********************************************************************************************/

// bool Xdrv09(uint8_t function)
// {
//     //Serial.println("Xdrv09 - START");
//   bool result = false;

//   switch (function) {
//     case TASK_PRE_INIT:
//       TimerSetRandomWindows();
//       break;
// #ifdef USE_MODULE_NETWORK_WEBSERVER
// #ifdef USE_TIMERS_WEB
//     case TASK_WEB_ADD_BUTTON:
// #ifdef USE_RULES
//       WSBufferAppend_P(HTTP_BTN_MENU_TIMER);
// #else
//       if (devices_present) { WSBufferAppend_P(HTTP_BTN_MENU_TIMER); }
// #endif  // USE_RULES
//       break;
//     case TASK_WEB_ADD_HANDLER:
//       WebServer->on("/" WEB_HANDLE_TIMER, HandleTimerConfiguration);
//       break;
// #endif  // USE_TIMERS_WEB
// #endif  // USE_MODULE_NETWORK_WEBSERVER
//     case TASK_EVERY_SECOND://Serial.println("Xdrv09 - TASK_EVERY_SECOND");
//       TimerEverySecond();
//       break;
//     case TASK_COMMAND:
//       result = TimerCommand();
//       break;
//   }
//     DEBUGLLN("Xdrv09 - END");

//   return result;
// }

// #endif  // USE_TIMERS
// #endif