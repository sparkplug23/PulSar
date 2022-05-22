




// #ifdef USE_UFILESYS
// void Display_Text_From_File(const char *file) {
//   File fp;
//   if (!ufsp) return;
//   fp = ufsp->open(file, FS_FILE_READ);
//   if (fp > 0) {
//     char *savptr = XdrvMailbox.data;
//     char linebuff[128];
//     while (fp.available()) {
//       uint16_t index = 0;
//       while (fp.available()) {
//         uint8_t buf[1];
//         fp.read(buf,1);
//         if (buf[0]=='\n' || buf[0]=='\r') {
//           break;
//         } else {
//           linebuff[index] = buf[0];
//           index++;
//           if (index >= sizeof(linebuff) - 1) {
//             break;
//           }
//         }
//       }
//       linebuff[index] = 0;
//       char *cp = linebuff;
//       while (*cp==' ') cp++;
//       if (*cp == ';') continue;
//       //AddLog(LOG_LEVEL_INFO, PSTR("displaytext %s"), cp);
//       // execute display text here
//       XdrvMailbox.data = cp;
//       XdrvMailbox.data_len = 0;
//       DisplayText();
//     }
//     XdrvMailbox.data = savptr;
//     fp.close();
//   }
// }
// #endif // USE_UFILESYS


// #ifdef USE_DT_VARS

// #ifndef MAX_DT_VARS
// #define MAX_DT_VARS 8
// #endif // MAX_DT_VARS

// #define MAX_DVTSIZE 24

// typedef struct {
//   uint16_t xp;
//   uint16_t yp;
//   uint8_t txtbcol;
//   uint8_t txtfcol;
//   int8_t txtsiz;
//   int8_t txtlen;
//   int8_t dp;
//   int8_t font;
//   int8_t time;
//   int8_t timer;
//   char unit[6];
//   char *jstrbuf;
//   char rstr[32];
// } DT_VARS;

// DT_VARS *dt_vars[MAX_DT_VARS];

// void define_dt_var(uint32_t num, uint32_t xp, uint32_t yp,  uint32_t txtbcol,  uint32_t txtfcol, int32_t font, int32_t txtsiz, int32_t txtlen, int32_t time, int32_t dp, char *jstr, char *unit) {
//   if (num >= MAX_DT_VARS) return;

//   if (dt_vars[num]) {
//     if (dt_vars[num]->jstrbuf) free(dt_vars[num]->jstrbuf);
//     free(dt_vars[num]);
//   }
//   //dt [dv0:100:100:0:3:2:1:10:2:WLAN#ID:uV:]

//   DT_VARS *dtp = (DT_VARS*)malloc(sizeof(DT_VARS));
//   if (!dtp) return;

//   dt_vars[num] = dtp;

//   dtp->xp = xp;
//   dtp->yp = yp;
//   dtp->txtbcol = txtbcol;
//   dtp->txtfcol = txtfcol;
//   dtp->font = font;
//   dtp->txtsiz = txtsiz;
//   dtp->time = time;
//   if (txtlen > MAX_DVTSIZE) {txtlen = MAX_DVTSIZE;}
//   dtp->txtlen = txtlen;
//   dtp->dp = dp;
//   uint8_t jlen = strlen(jstr);
//   dtp->jstrbuf = (char*)calloc(jlen + 2,1);
//   if (!dtp->jstrbuf) {
//     free (dtp);
//     return;
//   }
//   dtp->rstr[0] = 0;
//   strcpy(dtp->unit, unit);
//   strcpy(dtp->jstrbuf, jstr);
//   if (!time) time = 1;
//   dtp->timer = time;
// }

// void draw_dt_vars(void) {
//   if (!renderer) return;

//   for (uint32_t cnt = 0; cnt < MAX_DT_VARS; cnt++) {
//     DT_VARS *dtp = dt_vars[cnt];
//     if (dtp) {
//       if (dtp->jstrbuf) {
//         // draw
//         dtp->timer--;
//         if (!dtp->timer) {
//           dtp->timer = dtp->time;
//           char vstr[MAX_DVTSIZE + 7];
//           memset(vstr, ' ', sizeof(vstr));
//           strcpy(vstr, dtp->rstr);
//           strcat(vstr, " ");
//           strcat(vstr, dtp->unit);
//           uint16_t slen = strlen(vstr);
//           vstr[slen] = ' ';

//           if (!dtp->txtlen) {
//             vstr[slen] = 0;
//           } else {
//             vstr[abs(int(dtp->txtlen))] = 0;
//           }
//           if (dtp->txtlen < 0) {
//             // right align
//             alignright(vstr);
//           }

//           if (dtp->txtsiz > 0) {
//             renderer->setDrawMode(0);
//           } else {
//             renderer->setDrawMode(2);
//           }
//           renderer->setTextColor(GetColorFromIndex(dtp->txtfcol),GetColorFromIndex(dtp->txtbcol));
//           renderer->setTextFont(dtp->font);
//           renderer->setTextSize(abs(dtp->txtsiz));

//           if (dtp->jstrbuf[0]=='[') {
//             uint16_t s_disp_xpos = disp_xpos;
//             uint16_t s_disp_ypos = disp_ypos;
//             uint16_t s_bg_color = bg_color;
//             uint16_t s_fg_color = fg_color;
//             disp_xpos = dtp->xp;
//             disp_ypos = dtp->yp;
//             bg_color = GetColorFromIndex(dtp->txtbcol);
//             fg_color = GetColorFromIndex(dtp->txtfcol);
//             char *savmbd = XdrvMailbox.data;
//             XdrvMailbox.data = dtp->jstrbuf;
//             DisplayText();
//             XdrvMailbox.data = savmbd;
//             disp_xpos = s_disp_xpos;
//             disp_ypos = s_disp_ypos;
//             bg_color = s_bg_color;
//             fg_color = s_fg_color;
//           } else {
//             renderer->DrawStringAt(dtp->xp, dtp->yp, vstr, GetColorFromIndex(dtp->txtfcol), 0);
//           }

//           // restore display vars
//           renderer->setTextColor(fg_color, bg_color);
//           renderer->setDrawMode(auto_draw>>1);
//         }
//       }
//     }
//   }
// }

// #define DTV_JSON_SIZE 1024

// void DisplayDTVarsTeleperiod(void) {
//   ResponseClear();
//   MqttShowState();
//   uint32_t jlen = strlen(pCONT_set->runtime_var.mqtt_data);

//   if (jlen < DTV_JSON_SIZE) {
//     char *json = (char*)malloc(jlen + 2);
//     if (json) {
//       strlcpy(json, pCONT_set->runtime_var.mqtt_data, jlen + 1);
//       get_dt_vars(json);
//       free(json);
//     }
//   }
// }

// void get_dt_mqtt(void) {
//   static uint8_t xsns_index = 0;

//   ResponseClear();
//   uint16_t script_tele_period_save = pCONT_set->runtime_var.tele_period;
//   pCONT_set->runtime_var.tele_period = 2;
//   XsnsNextCall(FUNC_JSON_APPEND, xsns_index);
//   pCONT_set->runtime_var.tele_period = script_tele_period_save;
//   if (strlen(pCONT_set->runtime_var.mqtt_data)) {
//     pCONT_set->runtime_var.mqtt_data[0] = '{';
//     snprintf_P(pCONT_set->runtime_var.mqtt_data, sizeof(pCONT_set->runtime_var.mqtt_data), PSTR("%s}"), pCONT_set->runtime_var.mqtt_data);
//   }
//   get_dt_vars(pCONT_set->runtime_var.mqtt_data);
// }

// void get_dt_vars(char *json) {
//   if (strlen(json)) {
//     JsonParser parser(json);
//     JsonParserObject obj = parser.getRootObject();

//     for (uint32_t cnt = 0; cnt < MAX_DT_VARS; cnt++) {
//       if (dt_vars[cnt]) {
//         if (dt_vars[cnt]->jstrbuf && dt_vars[cnt]->jstrbuf[0]!='[') {
//           char sbuf[32];
//           uint32_t res = JsonParsePath(&obj, dt_vars[cnt]->jstrbuf, '#', NULL, sbuf, sizeof(sbuf));
//           if (res) {
//             if (dt_vars[cnt]->dp < 0) {
//               // use string
//               strcpy(dt_vars[cnt]->rstr, sbuf);
//             } else {
//               // convert back and forth
//               dtostrfd(CharToFloat(sbuf), dt_vars[cnt]->dp, dt_vars[cnt]->rstr);
//             }
//           }
//         }
//       }
//     }
//   }
// }

// void free_dt_vars(void) {
//   for (uint32_t cnt = 0; cnt < MAX_DT_VARS; cnt++) {
//     if (dt_vars[cnt]) {
//       if (dt_vars[cnt]->jstrbuf) free(dt_vars[cnt]->jstrbuf);
//       free(dt_vars[cnt]);
//       dt_vars[cnt] = 0;
//     }
//   }
// }

// #endif // USE_DT_VARS




// /*********************************************************************************************\
//  * Sensors
// \*********************************************************************************************/

// enum SensorQuantity {
//   JSON_TEMPERATURE,
//   JSON_HUMIDITY, JSON_LIGHT, JSON_NOISE, JSON_AIRQUALITY,
//   JSON_PRESSURE, JSON_PRESSUREATSEALEVEL,
//   JSON_ILLUMINANCE,
//   JSON_GAS,
//   JSON_YESTERDAY, JSON_TOTAL, JSON_TODAY,
//   JSON_PERIOD,
//   JSON_POWERFACTOR, JSON_COUNTER, JSON_ANALOG_INPUT, JSON_UV_LEVEL,
//   JSON_CURRENT,
//   JSON_VOLTAGE,
//   JSON_POWERUSAGE,
//   JSON_CO2,
//   JSON_FREQUENCY };
// const char kSensorQuantity[] PROGMEM =
//   D_JSON_TEMPERATURE "|"                                                        // degrees
//   D_JSON_HUMIDITY "|" D_JSON_LIGHT "|" D_JSON_NOISE "|" D_JSON_AIRQUALITY "|"   // percentage
//   D_JSON_PRESSURE "|" D_JSON_PRESSUREATSEALEVEL "|"                             // hPa
//   D_JSON_ILLUMINANCE "|"                                                        // lx
//   D_JSON_GAS "|"                                                                // kOhm
//   D_JSON_YESTERDAY "|" D_JSON_TOTAL "|" D_JSON_TODAY "|"                        // kWh
//   D_JSON_PERIOD "|"                                                             // Wh
//   D_JSON_POWERFACTOR "|" D_JSON_COUNTER "|" D_JSON_ANALOG_INPUT "|" D_JSON_UV_LEVEL "|"                 // No unit
//   D_JSON_CURRENT "|"                                                            // Ampere
//   D_JSON_VOLTAGE "|"                                                            // Volt
//   D_JSON_POWERUSAGE "|"                                                         // Watt
//   D_JSON_CO2 "|"                                                                // ppm
//   D_JSON_FREQUENCY ;                                                            // Hz

// void DisplayJsonValue(const char* topic, const char* device, const char* mkey, const char* value)
// {
//   char quantity[TOPSZ];
//   char buffer[pCONT_set->Settings.display.cols[0] +1];
//   char spaces[pCONT_set->Settings.display.cols[0]];
//   char source[pCONT_set->Settings.display.cols[0] - pCONT_set->Settings.display.cols[1]];
//   char svalue[pCONT_set->Settings.display.cols[1] +1];

// #ifdef USE_DEBUG_DRIVER
//   ShowFreeMem(PSTR("DisplayJsonValue"));
// #endif

//   memset(spaces, 0x20, sizeof(spaces));
//   spaces[sizeof(spaces) -1] = '\0';
//   snprintf_P(source, sizeof(source), PSTR("%s%s%s%s"), topic, (strlen(topic))?"/":"", mkey, spaces);  // pow1/Voltage or Voltage if topic is empty (local sensor)

//   int quantity_code = GetCommandCode(quantity, sizeof(quantity), mkey, kSensorQuantity);
//   if ((-1 == quantity_code) || !strcmp_P(mkey, S_RSLT_POWER)) {  // Ok: Power, Not ok: POWER
//     return;
//   }
//   if (JSON_TEMPERATURE == quantity_code) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s~%s"), value, disp_temp);
//   }
//   else if ((quantity_code >= JSON_HUMIDITY) && (quantity_code <= JSON_AIRQUALITY)) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s%%"), value);
//   }
//   else if ((quantity_code >= JSON_PRESSURE) && (quantity_code <= JSON_PRESSUREATSEALEVEL)) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s%s"), value, disp_pres);
//   }
//   else if (JSON_ILLUMINANCE == quantity_code) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_LUX), value);
//   }
//   else if (JSON_GAS == quantity_code) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_KILOOHM), value);
//   }
//   else if ((quantity_code >= JSON_YESTERDAY) && (quantity_code <= JSON_TODAY)) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_KILOWATTHOUR), value);
//   }
//   else if (JSON_PERIOD == quantity_code) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_WATTHOUR), value);
//   }
//   else if ((quantity_code >= JSON_POWERFACTOR) && (quantity_code <= JSON_UV_LEVEL)) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s"), value);
//   }
//   else if (JSON_CURRENT == quantity_code) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_AMPERE), value);
//   }
//   else if (JSON_VOLTAGE == quantity_code) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_VOLT), value);
//   }
//   else if (JSON_POWERUSAGE == quantity_code) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_WATT), value);
//   }
//   else if (JSON_CO2 == quantity_code) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_PARTS_PER_MILLION), value);
//   }
//   else if (JSON_FREQUENCY == quantity_code) {
//     snprintf_P(svalue, sizeof(svalue), PSTR("%s" D_UNIT_HERTZ), value);
//   }
//   snprintf_P(buffer, sizeof(buffer), PSTR("%s %s"), source, svalue);

// //  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "mkey [%s], source [%s], value [%s], quantity_code %d, log_buffer [%s]"), mkey, source, value, quantity_code, buffer);

//   LogBuffer_Add(buffer);
// }

// void DisplayAnalyzeJson(char *topic, char *json)
// {
// // //tele/pow2/STATE    {"Time":"2017-09-20T11:53:03", "Uptime":10, "Vcc":3.123, "POWER":"ON", "Wifi":{"AP":2, "SSId":"indebuurt2", "RSSI":68, "APMac":"00:22:6B:FE:8E:20"}}
// // //tele/pow2/ENERGY   {"Time":"2017-09-20T11:53:03", "Total":6.522, "Yesterday":0.150, "Today":0.073, "Period":0.5, "Power":12.1, "Factor":0.56, "Voltage":210.1, "Current":0.102}

// // tele/pow1/SENSOR = {"Time":"2018-01-02T17:13:17","ENERGY":{"Total":13.091,"Yesterday":0.060,"Today":0.046,"Period":0.2,"Power":9.8,"Factor":0.49,"Voltage":206.8,"Current":0.096}}
// // tele/dual/STATE    {"Time":"2017-09-20T11:53:03","Uptime":25,"Vcc":3.178,"POWER1":"OFF","POWER2":"OFF","Wifi":{"AP":2,"SSId":"indebuurt2","RSSI":100,"APMac":"00:22:6B:FE:8E:20"}}
// // tele/sc/SENSOR     {"Time":"2017-09-20T11:53:09","Temperature":24.0,"Humidity":16.0,"Light":30,"Noise":20,"AirQuality":100,"TempUnit":"C"}
// // tele/rf1/SENSOR    {"Time":"2017-09-20T11:53:23","BH1750":{"Illuminance":57}}
// // tele/wemos5/SENSOR {"Time":"2017-09-20T11:53:53","SHT1X":{"Temperature":20.1,"Humidity":58.9},"HTU21":{"Temperature":20.7,"Humidity":58.5},"BMP280":{"Temperature":21.6,"Pressure":1020.3},"TempUnit":"C"}
// // tele/th1/SENSOR    {"Time":"2017-09-20T11:54:48","DS18B20":{"Temperature":49.7},"TempUnit":"C"}

//   String jsonStr = json;  // Move from stack to heap to fix watchdogs (20180626)
//   JsonParser parser((char*)jsonStr.c_str());
//   JsonParserObject root = parser.getRootObject();
//   if (root) {   // did JSON parsing went ok?

//     const char *unit = root.getStr(PSTR(D_JSON_TEMPERATURE_UNIT), nullptr);   // nullptr if not found
//     if (unit) {
//       snprintf_P(disp_temp, sizeof(disp_temp), PSTR("%s"), unit);  // C or F
//     }
//     unit = root.getStr(PSTR(D_JSON_PRESSURE_UNIT), nullptr);   // nullptr if not found
//     if (unit) {
//       snprintf_P(disp_pres, sizeof(disp_pres), PSTR("%s"), unit);  // hPa or mmHg
//     }
//     for (auto key1 : root) {
//       JsonParserToken value1 = key1.getValue();
//       if (value1.isObject()) {
//         JsonParserObject Object2 = value1.getObject();
//         for (auto key2 : Object2) {
//           JsonParserToken value2 = key2.getValue();
//           if (value2.isObject()) {
//             JsonParserObject Object3 = value2.getObject();
//             for (auto key3 : Object3) {
//               const char* value3 = key3.getValue().getStr(nullptr);
//               if (value3 != nullptr) {  // "DHT11":{"Temperature":null,"Humidity":null} - ignore null as it will raise exception 28
//                 DisplayJsonValue(topic, key1.getStr(), key3.getStr(), value3);  // Sensor 56%
//               }
//             }
//           } else {
//             const char* value = value2.getStr(nullptr);
//             if (value != nullptr) {
//               DisplayJsonValue(topic, key1.getStr(), key2.getStr(), value);  // Sensor  56%
//             }
//           }
//         }
//       } else {
//         const char* value = value1.getStr(nullptr);
//         if (value != nullptr) {
//           DisplayJsonValue(topic, key1.getStr(), key1.getStr(), value);  // Topic  56%
//         }
//       }
//     }
//   }
// }

// void DisplayMqttSubscribe(void)
// {
// /* Subscribe to tele messages only
//  * Supports the following FullTopic formats
//  * - %prefix%/%topic%
//  * - home/%prefix%/%topic%
//  * - home/level2/%prefix%/%topic% etc.
//  */
//   if (pCONT_set->Settings.display.model && (pCONT_set->Settings.display.mode &0x04)) {

//     char stopic[TOPSZ];
//     char ntopic[TOPSZ];

//     ntopic[0] = '\0';
//     strlcpy(stopic, SettingsText(SET_MQTT_FULLTOPIC), sizeof(stopic));
//     char *tp = strtok(stopic, "/");
//     while (tp != nullptr) {
//       if (!strcmp_P(tp, MQTT_TOKEN_PREFIX)) {
//         break;
//       }
//       strncat_P(ntopic, PSTR("+/"), sizeof(ntopic) - strlen(ntopic) -1);           // Add single-level wildcards
//       tp = strtok(nullptr, "/");
//     }
//     strncat(ntopic, SettingsText(SET_MQTTPREFIX3), sizeof(ntopic) - strlen(ntopic) -1);  // Subscribe to tele messages
//     strncat_P(ntopic, PSTR("/#"), sizeof(ntopic) - strlen(ntopic) -1);             // Add multi-level wildcard
//     MqttSubscribe(ntopic);
//     disp_subscribed = true;
//   } else {
//     disp_subscribed = false;
//   }
// }

// bool DisplayMqttData(void)
// {
//   if (disp_subscribed) {
//     char stopic[TOPSZ];

//     snprintf_P(stopic, sizeof(stopic) , PSTR("%s/"), SettingsText(SET_MQTTPREFIX3));  // tele/
//     char *tp = strstr(XdrvMailbox.topic, stopic);
//     if (tp) {                                                // tele/tasmota/SENSOR
//       if (pCONT_set->Settings.display.mode &0x04) {
//         tp = tp + strlen(stopic);                              // tasmota/SENSOR
//         char *topic = strtok(tp, "/");                         // tasmota
//         DisplayAnalyzeJson(topic, XdrvMailbox.data);
//       }
//       return true;
//     }
//   }
//   return false;
// }

// void DisplayLocalSensor(void)
// {
//   if ((pCONT_set->Settings.display.mode &0x02) && (0 == pCONT_set->runtime_var.tele_period)) {
//     char no_topic[1] = { 0 };
// //    DisplayAnalyzeJson(pCONT_set->runtime_var.mqtt_topic, pCONT_set->runtime_var.mqtt_data);  // Add local topic
//     DisplayAnalyzeJson(no_topic, pCONT_set->runtime_var.mqtt_data);    // Discard any topic
//   }
// }

// #endif  // USE_DISPLAY_MODES1TO5



// /*********************************************************************************************\
//  * Optional drivers
// \*********************************************************************************************/

// #ifdef USE_TOUCH_BUTTONS
// // very limited path size, so, add .jpg
// void draw_picture(char *path, uint32_t xp, uint32_t yp, uint32_t xs, uint32_t ys, uint32_t ocol, bool inverted) {
// char ppath[16];
//   strcpy(ppath, path);
//   uint8_t plen = strlen(path) -1;
//   if (ppath[plen]=='1') {
//     // index mode
//     if (inverted) {
//       ppath[plen] = '2';
//     }
//     inverted = false;
//   }
//   if (ocol == 9) {
//     strcat(ppath, ".rgb");
//   } else {
//     strcat(ppath, ".jpg");
//   }
//   Draw_RGB_Bitmap(ppath, xp, yp, inverted);
// }
// #endif  // USE_TOUCH_BUTTONS


// #ifdef ESP32
// #ifdef JPEG_PICTS
// #include "img_converters.h"
// #include "esp_jpg_decode.h"
// bool jpg2rgb888(const uint8_t *src, size_t src_len, uint8_t * out, jpg_scale_t scale);
// char get_jpeg_size(unsigned char* data, unsigned int data_size, unsigned short *width, unsigned short *height);
// #endif // JPEG_PICTS
// #endif // ESP32

// #ifdef USE_UFILESYS
// extern FS *ufsp;
// #define XBUFF_LEN 128
// void Draw_RGB_Bitmap(char *file,uint16_t xp, uint16_t yp, bool inverted ) {
//   if (!renderer) return;
//   File fp;
//   char *ending = strrchr(file,'.');
//   if (!ending) return;
//   ending++;
//   char estr[8];
//   memset(estr,0,sizeof(estr));
//   for (uint32_t cnt=0; cnt<strlen(ending); cnt++) {
//     estr[cnt]=tolower(ending[cnt]);
//   }

//   if (!strcmp(estr,"rgb")) {
//     // special rgb format
//     fp=ufsp->open(file,FS_FILE_READ);
//     if (!fp) return;
//     uint16_t xsize;
//     fp.read((uint8_t*)&xsize,2);
//     uint16_t ysize;
//     fp.read((uint8_t*)&ysize,2);
// #if 1
//     renderer->setAddrWindow(xp,yp,xp+xsize,yp+ysize);
//     uint16_t rgb[xsize];
//     for (int16_t j=0; j<ysize; j++) {
//     //  for(int16_t i=0; i<xsize; i+=XBUFF_LEN) {
//         fp.read((uint8_t*)rgb,xsize*2);
//         renderer->pushColors(rgb,xsize,true);
//     //  }
//       OsWatchLoop();
//     }
//     renderer->setAddrWindow(0,0,0,0);
// #else
//     for(int16_t j=0; j<ysize; j++) {
//       for(int16_t i=0; i<xsize; i++ ) {
//         uint16_t rgb;
//         uint8_t res=fp.read((uint8_t*)&rgb,2);
//         if (!res) break;
//         renderer->writePixel(xp+i,yp,rgb);
//       }
//       delay(0);
//       OsWatchLoop();
//       yp++;
//     }
// #endif
//     fp.close();
//   } else if (!strcmp(estr,"jpg")) {
//     // jpeg files on ESP32 with more memory
// #ifdef ESP32
// #ifdef JPEG_PICTS
//     fp=ufsp->open(file,FS_FILE_READ);
//     if (!fp) return;
//     uint32_t size = fp.size();
//     uint8_t *mem = (uint8_t *)special_malloc(size+4);
//     if (mem) {
//       uint8_t res=fp.read(mem, size);
//       if (res) {
//         uint16_t xsize;
//         uint16_t ysize;
//         if (mem[0]==0xff && mem[1]==0xd8) {
//           get_jpeg_size(mem, size, &xsize, &ysize);
//           //Serial.printf(" x,y,fs %d - %d - %d\n",xsize, ysize, size );
//           if (xsize && ysize) {
//             uint8_t *out_buf = (uint8_t *)special_malloc((xsize*ysize*3)+4);
//             if (out_buf) {
//               uint16_t *pixb = (uint16_t *)special_malloc((xsize*2)+4);
//               if (pixb) {
//                 uint8_t *ob=out_buf;
//                 if (jpg2rgb888(mem, size, out_buf, (jpg_scale_t)JPG_SCALE_NONE)) {
//                   renderer->setAddrWindow(xp,yp,xp+xsize,yp+ysize);
//                   for(int32_t j=0; j<ysize; j++) {
//                     if (inverted==false) {
//                       rgb888_to_565(ob, pixb, xsize);
//                     } else {
//                       rgb888_to_565i(ob, pixb, xsize);
//                     }
//                     ob+=xsize*3;
//                     renderer->pushColors(pixb, xsize, true);
//                     OsWatchLoop();
//                   }
//                   renderer->setAddrWindow(0,0,0,0);
//                 }
//                 free(out_buf);
//                 free(pixb);
//               } else {
//                 free(out_buf);
//               }
//             }
//           }
//         }
//         free(mem);
//       }
//       fp.close();
//     }
// #endif // JPEG_PICTS
// #endif // ESP32
//   }
// }
// #endif // USE_UFILESYS

// /*********************************************************************************************\
//  * AWatch
// \*********************************************************************************************/

// #ifdef USE_AWATCH
// #define MINUTE_REDUCT 4

// #ifndef pi
// #define pi 3.14159265359
// #endif

// // draw analog watch, just for fun
// void DrawAClock(uint16_t rad) {
//     if (!renderer) return;
//     float frad=rad;
//     uint16_t hred=frad/3.0;
//     renderer->fillCircle(disp_xpos, disp_ypos, rad, bg_color);
//     renderer->drawCircle(disp_xpos, disp_ypos, rad, fg_color);
//     renderer->fillCircle(disp_xpos, disp_ypos, 4, fg_color);
//     for (uint8_t count=0; count<60; count+=5) {
//       float p1=((float)count*(pi/30)-(pi/2));
//       uint8_t len;
//       if ((count%15)==0) {
//         len=4;
//       } else {
//         len=2;
//       }
//       renderer->writeLine(disp_xpos+((float)(rad-len)*cosf(p1)), disp_ypos+((float)(rad-len)*sinf(p1)), disp_xpos+(frad*cosf(p1)), disp_ypos+(frad*sinf(p1)), fg_color);
//     }

//     // hour
//     float hour=((float)RtcTime.hour*60.0+(float)RtcTime.minute)/60.0;
//     float temp=(hour*(pi/6.0)-(pi/2.0));
//     renderer->writeLine(disp_xpos, disp_ypos,disp_xpos+(frad-hred)*cosf(temp),disp_ypos+(frad-hred)*sinf(temp), fg_color);

//     // minute
//     temp=((float)RtcTime.minute*(pi/30.0)-(pi/2.0));
//     renderer->writeLine(disp_xpos, disp_ypos,disp_xpos+(frad-MINUTE_REDUCT)*cosf(temp),disp_ypos+(frad-MINUTE_REDUCT)*sinf(temp), fg_color);
// }
// #endif // USE_AWATCH

// /*********************************************************************************************\
//  * Graphics
// \*********************************************************************************************/


// #ifdef USE_GRAPH


// #define TICKLEN 4
// void ClrGraph(uint16_t num) {
//   struct GRAPH *gp=graph[num];

//   uint16_t xticks=gp->xticks;
//   uint16_t yticks=gp->yticks;
//   uint16_t count;

//   // clr inside, but only 1.graph if overlapped
//   if (gp->flags.overlay) return;

//   renderer->fillRect(gp->xp+1,gp->yp+1,gp->xs-2,gp->ys-2,bg_color);

//   if (xticks) {
//     float cxp=gp->xp,xd=(float)gp->xs/(float)xticks;
//     for (count=0; count<xticks; count++) {
//       renderer->writeFastVLine(cxp,gp->yp+gp->ys-TICKLEN,TICKLEN,fg_color);
//       cxp+=xd;
//     }
//   }
//   if (yticks) {
//     if (gp->ymin<0 && gp->ymax>0) {
//       // draw zero seperator
//       float cxp=0;
//       float czp=gp->yp+(gp->ymax/gp->range);
//       while (cxp<gp->xs) {
//         renderer->writeFastHLine(gp->xp+cxp,czp,2,fg_color);
//         cxp+=6.0;
//       }
//       // align ticks to zero line
//       float cyp=0,yd=gp->ys/yticks;
//       for (count=0; count<yticks; count++) {
//         if ((czp-cyp)>gp->yp) {
//           renderer->writeFastHLine(gp->xp,czp-cyp,TICKLEN,fg_color);
//           renderer->writeFastHLine(gp->xp+gp->xs-TICKLEN,czp-cyp,TICKLEN,fg_color);
//         }
//         if ((czp+cyp)<(gp->yp+gp->ys)) {
//           renderer->writeFastHLine(gp->xp,czp+cyp,TICKLEN,fg_color);
//           renderer->writeFastHLine(gp->xp+gp->xs-TICKLEN,czp+cyp,TICKLEN,fg_color);
//         }
//         cyp+=yd;
//       }
//     } else {
//       float cyp=gp->yp,yd=gp->ys/yticks;
//       for (count=0; count<yticks; count++) {
//         renderer->writeFastHLine(gp->xp,cyp,TICKLEN,fg_color);
//         renderer->writeFastHLine(gp->xp+gp->xs-TICKLEN,cyp,TICKLEN,fg_color);
//         cyp+=yd;
//       }
//     }
//   }
// }

// // define a graph
// void DefineGraph(uint16_t num,uint16_t xp,uint16_t yp,int16_t xs,uint16_t ys,int16_t dec,float ymin, float ymax,uint8_t icol) {
//   if (!renderer) return;
//   uint8_t rflg=0;
//   if (xs<0) {
//     rflg=1;
//     xs=abs(xs);
//   }
//   struct GRAPH *gp;
//   uint16_t count;
//   uint16_t index=num%NUM_GRAPHS;
//   if (!graph[index]) {
//     gp=(struct GRAPH*)calloc(sizeof(struct GRAPH),1);
//     if (!gp) return;
//     graph[index]=gp;
//   } else {
//     gp=graph[index];
//     if (rflg) {
//       RedrawGraph(index,1);
//       return;
//     }
//   }

//   // 6 bits per axis
//   gp->xticks=(num>>4)&0x3f;
//   gp->yticks=(num>>10)&0x3f;
//   gp->xp=xp;
//   gp->yp=yp;
//   gp->xs=xs;
//   gp->ys=ys;
//   if (!dec) dec=1;
//   gp->decimation=dec;
//   if (dec>0) {
//     // is minutes per sweep prepare timing parameters in ms
//     gp->x_time=((float)dec*60000.0)/(float)xs;
//     gp->last_ms=millis()+gp->x_time;
//   }
//   gp->ymin=ymin;
//   gp->ymax=ymax;
//   gp->range=(ymax-ymin)/ys;
//   gp->xcnt=0;
//   gp->dcnt=0;
//   gp->summ=0;
//   if (gp->values) free(gp->values);
//   gp->values=(uint8_t*) calloc(1,xs+2);
//   if (!gp->values) {
//     free(gp);
//     graph[index]=0;
//     return;
//   }
//   // start from zero
//   gp->values[0]=0;

//   gp->last_ms_redrawn=millis();

//   if (!icol) icol=1;
//   gp->color_index=icol;
//   gp->flags.overlay=0;
//   gp->flags.draw=1;

//   // check if previous graph has same coordinates
//   if (index>0) {
//     for (uint8_t count=0; count<index; count++) {
//       if (graph[count]) {
//         // a previous graph is defined, compare
//         // assume the same if corner is identical
//         struct GRAPH *gp1=graph[count];
//         if ((gp->xp==gp1->xp) && (gp->yp==gp1->yp)) {
//           gp->flags.overlay=1;
//           break;
//         }
//       }
//     }
//   }

//   // draw rectangle
//   renderer->drawRect(xp,yp,xs,ys,fg_color);
//   // clr inside
//   ClrGraph(index);

// }

// // check if to advance GRAPH
// void DisplayCheckGraph() {
//   int16_t count;
//   struct GRAPH *gp;
//   for (count=0;count<NUM_GRAPHS;count++) {
//     gp=graph[count];
//     if (gp) {
//       if (gp->decimation>0) {
//         // if time over add value
//         while (millis()>gp->last_ms) {
//           gp->last_ms+=gp->x_time;
//           uint8_t val;
//           if (gp->dcnt) {
//             val=gp->summ/gp->dcnt;
//             gp->dcnt=0;
//             gp->summ=0;
//             gp->last_val=val;
//           } else {
//             val=gp->last_val;
//           }
//           AddGraph(count,val);
//         }
//       }
//     }
//   }
// }


// #if (defined(USE_SCRIPT_FATFS) && defined(USE_SCRIPT)) || defined(USE_UFILESYS)
// #ifdef ESP32
// #include <SD.h>
// #endif

// void Save_graph(uint8_t num, char *path) {
//   if (!renderer) return;
//   uint16_t index=num%NUM_GRAPHS;
//   struct GRAPH *gp=graph[index];
//   if (!gp) return;
//   File fp;
//   ufsp->remove(path);
//   fp=ufsp->open(path,FS_FILE_WRITE);
//   if (!fp) return;
//   char str[32];
//   sprintf_P(str,PSTR("%d\t%d\t%d\t"),gp->xcnt,gp->xs,gp->ys);
//   fp.print(str);
//   dtostrfd(gp->ymin,2,str);
//   fp.print(str);
//   fp.print("\t");
//   dtostrfd(gp->ymax,2,str);
//   fp.print(str);
//   fp.print("\t");
//   for (uint32_t count=0;count<gp->xs;count++) {
//     dtostrfd(gp->values[count],0,str);
//     fp.print(str);
//     fp.print("\t");
//   }
//   fp.print("\n");
//   fp.close();
// }

// void Restore_graph(uint8_t num, char *path) {
//   if (!renderer) return;
//   uint16_t index=num%NUM_GRAPHS;
//   struct GRAPH *gp=graph[index];
//   if (!gp) return;
//   File fp;
//   fp=ufsp->open(path,FS_FILE_READ);
//   if (!fp) return;
//   char vbuff[32];
//   char *cp=vbuff;
//   uint8_t buf[2];
//   uint8_t findex=0;

//   for (uint32_t count=0;count<=gp->xs+4;count++) {
//     cp=vbuff;
//     findex=0;
//     while (fp.available()) {
//       fp.read(buf,1);
//       if (buf[0]=='\t' || buf[0]==',' || buf[0]=='\n' || buf[0]=='\r') {
//         break;
//       } else {
//         *cp++=buf[0];
//         findex++;
//         if (findex>=sizeof(vbuff)-1) break;
//       }
//     }
//     *cp=0;
//     if (count<=4) {
//       if (count==0) gp->xcnt=atoi(vbuff);
//     } else {
//       gp->values[count-5]=atoi(vbuff);
//     }
//   }
//   fp.close();
//   RedrawGraph(num,1);
// }
// #endif // USE_SCRIPT_FATFS

// void RedrawGraph(uint8_t num, uint8_t flags) {
//   uint16_t index=num%NUM_GRAPHS;
//   struct GRAPH *gp=graph[index];
//   if (!gp) return;
//   if (!flags) {
//     gp->flags.draw=0;
//     return;
//   }
//   if (!renderer) return;

//   gp->flags.draw=1;
//   uint16_t linecol=fg_color;

//   if (color_type==COLOR_COLOR) {
//     linecol = GetColorFromIndex(gp->color_index);
//   }

//   if (!gp->flags.overlay) {
//     // draw rectangle
//     renderer->drawRect(gp->xp,gp->yp,gp->xs,gp->ys,fg_color);
//     // clr inside
//     ClrGraph(index);
//   }

//   for (uint16_t count=0;count<gp->xs-1;count++) {
//     renderer->writeLine(gp->xp+count,gp->yp+gp->ys-gp->values[count]-1,gp->xp+count+1,gp->yp+gp->ys-gp->values[count+1]-1,linecol);
//   }
// }

// // add next value to graph
// void AddGraph(uint8_t num,uint8_t val) {
//   struct GRAPH *gp=graph[num];
//   if (!renderer) return;

//   uint16_t linecol=fg_color;
//   if (color_type==COLOR_COLOR) {
//     linecol = GetColorFromIndex(gp->color_index);
//   }
//   gp->xcnt++;
//   if (gp->xcnt>gp->xs) {
//     gp->xcnt=gp->xs;
//     int16_t count;
//     // shift values
//     for (count=0;count<gp->xs-1;count++) {
//       gp->values[count]=gp->values[count+1];
//     }
//     gp->values[gp->xcnt-1]=val;

//     if (!gp->flags.draw) return;

//     // only redraw every second or longer
//     if (millis()-gp->last_ms_redrawn>1000) {
//       gp->last_ms_redrawn=millis();
//       // clr area and redraw graph
//       if (!gp->flags.overlay) {
//         // draw rectangle
//         renderer->drawRect(gp->xp,gp->yp,gp->xs,gp->ys,fg_color);
//         // clr inner and draw ticks
//         ClrGraph(num);
//       }

//       for (count=0;count<gp->xs-1;count++) {
//         renderer->writeLine(gp->xp+count,gp->yp+gp->ys-gp->values[count]-1,gp->xp+count+1,gp->yp+gp->ys-gp->values[count+1]-1,linecol);
//       }
//     }
//   } else {
//     // add value and draw a single line
//     gp->values[gp->xcnt]=val;
//     if (!gp->flags.draw) return;
//     renderer->writeLine(gp->xp+gp->xcnt-1,gp->yp+gp->ys-gp->values[gp->xcnt-1]-1,gp->xp+gp->xcnt,gp->yp+gp->ys-gp->values[gp->xcnt]-1,linecol);
//   }
// }

// // add next value
// void AddValue(uint8_t num,float fval) {
//   // not yet defined ???
//   num=num%NUM_GRAPHS;
//   struct GRAPH *gp=graph[num];
//   if (!gp) return;

//   if (fval>gp->ymax) fval=gp->ymax;
//   if (fval<gp->ymin) fval=gp->ymin;

//   int16_t val;
//   val=(fval-gp->ymin)/gp->range;

//   if (val>gp->ys-1) val=gp->ys-1;
//   if (val<0) val=0;

//   // summ values
//   gp->summ+=val;
//   gp->dcnt++;

//   // decimation option
//   if (gp->decimation<0) {
//     if (gp->dcnt>=-gp->decimation) {
//       gp->dcnt=0;
//       // calc average
//       val=gp->summ/-gp->decimation;
//       gp->summ=0;
//       // add to graph
//       AddGraph(num,val);
//     }
//   }
// }
// #endif // USE_GRAPH

// /*********************************************************************************************\
//  * Touch panel control
// \*********************************************************************************************/

// #if defined(USE_FT5206) || defined(USE_XPT2046)
// bool FT5206_found = false;
// bool XPT2046_found = false;

// int16_t touch_xp;
// int16_t touch_yp;
// bool touched;

// #ifdef USE_M5STACK_CORE2
// uint8_t tbstate[3];
// #endif // USE_M5STACK_CORE2

// #ifdef USE_FT5206
// #include <FT5206.h>
// // touch panel controller
// #undef FT5206_address
// #define FT5206_address 0x38

// FT5206_Class *FT5206_touchp;


// bool FT5206_Touch_Init(TwoWire &i2c) {
//   FT5206_found = false;
//   FT5206_touchp = new FT5206_Class();
//   if (FT5206_touchp->begin(i2c, FT5206_address)) {
//     I2cSetActiveFound(FT5206_address, "FT5206");
//     FT5206_found = true;
//   }
//   return FT5206_found;
// }

// bool FT5206_touched() {
//   return FT5206_touchp->touched();
// }
// int16_t FT5206_x() {
//   TP_Point pLoc = FT5206_touchp->getPoint(0);
//   return pLoc.x;
// }
// int16_t FT5206_y() {
//   TP_Point pLoc = FT5206_touchp->getPoint(0);
//   return pLoc.y;
// }
// #endif  // USE_FT5206

// #ifdef USE_XPT2046
// #include <XPT2046_Touchscreen.h>
// XPT2046_Touchscreen *XPT2046_touchp;


// bool XPT2046_Touch_Init(uint16_t CS) {
//   XPT2046_touchp = new XPT2046_Touchscreen(CS);
//   XPT2046_found = XPT2046_touchp->begin();
//   if (XPT2046_found) {
// 	   AddLog(LOG_LEVEL_INFO, PSTR("TS: XPT2046"));
//   }
//   return XPT2046_found;
// }
// bool XPT2046_touched() {
//   return XPT2046_touchp->touched();
// }
// int16_t XPT2046_x() {
//   TS_Point pLoc = XPT2046_touchp->getPoint();
//   return pLoc.x;
// }
// int16_t XPT2046_y() {
//   TS_Point pLoc = XPT2046_touchp->getPoint();
//   return pLoc.y;
// }
// #endif  // USE_XPT2046

// uint32_t Touch_Status(uint32_t sel) {
//   if (FT5206_found || XPT2046_found) {
//     switch (sel) {
//       case 0:
//         return  touched;
//       case 1:
//         return touch_xp;
//       case 2:
//         return touch_yp;
//     }
//     return 0;
//   } else {
//     return 0;
//   }
// }

// void Touch_Check(void(*rotconvert)(int16_t *x, int16_t *y)) {

// #ifdef USE_FT5206
//   if (FT5206_found) {
//     touch_xp = FT5206_x();
//     touch_yp = FT5206_y();
//     touched = FT5206_touched();
//   }
// #endif // USE_FT5206

// #ifdef USE_XPT2046
//   if (XPT2046_found) {
//     touch_xp = XPT2046_x();
//     touch_yp = XPT2046_y();
//     touched = XPT2046_touched();
//   }
// #endif // USE_XPT2046

//   if (touched) {

// #ifdef USE_TOUCH_BUTTONS
// #ifdef USE_M5STACK_CORE2
//     // handle  3 built in touch buttons
//     uint16_t xcenter = 80;
// #define TDELTA 30
// #define TYPOS 275
//     for (uint32_t tbut = 0; tbut < 3; tbut++) {
//       if (touch_xp > (xcenter - TDELTA) && touch_xp < (xcenter + TDELTA) && touch_yp > (TYPOS - TDELTA) && touch_yp < (TYPOS + TDELTA)) {
//         // hit a button
//         if (!(tbstate[tbut] & 1)) {
//           // pressed
//           tbstate[tbut] |= 1;
//           //AddLog(LOG_LEVEL_INFO, PSTR("tbut: %d pressed"), tbut);
//           Touch_MQTT(tbut, "BIB", tbstate[tbut] & 1);
//         }
//       }
//       xcenter += 100;
//     }
// #endif  // USE_M5STACK_CORE2
// #endif // USE_TOUCH_BUTTONS

//     rotconvert(&touch_xp, &touch_yp);

// #ifdef USE_TOUCH_BUTTONS
//     CheckTouchButtons(touched, touch_xp, touch_yp);
// #endif // USE_TOUCH_BUTTONS

//   } else {
// #ifdef USE_M5STACK_CORE2
//     for (uint32_t tbut = 0; tbut < 3; tbut++) {
//       if (tbstate[tbut] & 1) {
//         // released
//         tbstate[tbut] &= 0xfe;
//         Touch_MQTT(tbut, "BIB", tbstate[tbut] & 1);
//         //AddLog(LOG_LEVEL_INFO, PSTR("tbut: %d released"), tbut);
//       }
//     }
// #endif  // USE_M5STACK_CORE2

// #ifdef USE_TOUCH_BUTTONS
//     CheckTouchButtons(touched, touch_xp, touch_yp);
// #endif // USE_TOUCH_BUTTONS

//   }
// }
// #endif

// #ifdef USE_TOUCH_BUTTONS
// void Touch_MQTT(uint8_t index, const char *cp, uint32_t val) {
// #ifdef USE_FT5206
//   if (FT5206_found) ResponseTime_P(PSTR(",\"FT5206\":{\"%s%d\":\"%d\"}}"), cp, index+1, val);
// #endif
// #ifdef USE_XPT2046
//   if (XPT2046_found) ResponseTime_P(PSTR(",\"XPT2046\":{\"%s%d\":\"%d\"}}"), cp, index+1, val);
// #endif  // USE_XPT2046
//   MqttPublishTeleSensor();
// }

// void Touch_RDW_BUTT(uint32_t count, uint32_t pwr) {
//   buttons[count]->xdrawButton(pwr);
//   if (pwr) buttons[count]->vpower.on_off = 1;
//   else buttons[count]->vpower.on_off = 0;
// }



// void CheckTouchButtons(bool touched, int16_t touch_x, int16_t touch_y) {
//   uint16_t temp;
//   uint8_t rbutt=0;
//   uint8_t vbutt=0;

//   if (!renderer) return;
//     if (touched) {
//       // ALOG_DBM( PSTR("touch after convert %d - %d"), pLoc.x, pLoc.y);
//       // now must compare with defined buttons
//       for (uint8_t count = 0; count < MAX_TOUCH_BUTTONS; count++) {
//         if (buttons[count]) {
//           if (!buttons[count]->vpower.slider) {
//             if (!buttons[count]->vpower.disable) {
//               if (buttons[count]->contains(touch_x, touch_y)) {
//                 // did hit
//                 buttons[count]->press(true);
//                 if (buttons[count]->justPressed()) {
//                   if (!buttons[count]->vpower.is_virtual) {
//                     uint8_t pwr=bitRead(pCONT_set->runtime_var.power, rbutt);
//                     if (!SendKey(KEY_BUTTON, rbutt+1, POWER_TOGGLE)) {
//                       ExecuteCommandPower(rbutt+1, POWER_TOGGLE, SRC_BUTTON);
//                       Touch_RDW_BUTT(count, !pwr);
//                     }
//                   } else {
//                     // virtual button
//                     const char *cp;
//                     if (!buttons[count]->vpower.is_pushbutton) {
//                       // toggle button
//                       buttons[count]->vpower.on_off ^= 1;
//                       cp="TBT";
//                     } else {
//                       // push button
//                       buttons[count]->vpower.on_off = 1;
//                       cp="PBT";
//                     }
//                     buttons[count]->xdrawButton(buttons[count]->vpower.on_off);
//                     Touch_MQTT(count, cp, buttons[count]->vpower.on_off);
//                   }
//                 }
//               }
//               if (!buttons[count]->vpower.is_virtual) {
//                 rbutt++;
//               } else {
//                 vbutt++;
//               }
//             }
//           } else {
//             // slider
//             if (buttons[count]->didhit(touch_x, touch_y)) {
//               uint16_t value = buttons[count]->UpdateSlider(touch_x, touch_y);
//               Touch_MQTT(count, "SLD", value);
//             }
//           }
//         }
//       }

//   } else {
//     // no hit
//     for (uint8_t count = 0; count < MAX_TOUCH_BUTTONS; count++) {
//       if (buttons[count]) {
//         if (!buttons[count]->vpower.slider) {
//           buttons[count]->press(false);
//           if (buttons[count]->justReleased()) {
//             if (buttons[count]->vpower.is_virtual) {
//               if (buttons[count]->vpower.is_pushbutton) {
//                 // push button
//                 buttons[count]->vpower.on_off = 0;
//                 Touch_MQTT(count,"PBT", buttons[count]->vpower.on_off);
//                 buttons[count]->xdrawButton(buttons[count]->vpower.on_off);
//               }
//             }
//           }
//           if (!buttons[count]->vpower.is_virtual) {
//             // check if power button stage changed
//             uint8_t pwr = bitRead(pCONT_set->runtime_var.power, rbutt);
//             uint8_t vpwr = buttons[count]->vpower.on_off;
//             if (pwr != vpwr) {
//               Touch_RDW_BUTT(count, pwr);
//             }
//             rbutt++;
//           }
//         }
//       }
//     }
//     touch_xp = 0;
//     touch_yp = 0;
//   }
// }
// #endif // USE_TOUCH_BUTTONS


