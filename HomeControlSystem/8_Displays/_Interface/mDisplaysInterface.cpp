/*
  xdrv_13_display.ino - Display support for Tasmota

  Copyright (C) 2021  Theo Arends

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
#include "mDisplaysInterface.h"

#ifdef USE_MODULE_DISPLAYS_INTERFACE

const char* mDisplaysInterface::PM_MODULE_DISPLAYS_INTERFACE_CTR = D_MODULE_DISPLAYS_INTERFACE_CTR;
const char* mDisplaysInterface::PM_MODULE_DISPLAYS_INTERFACE_FRIENDLY_CTR = D_MODULE_DISPLAYS_INTERFACE_FRIENDLY_CTR;

int8_t mDisplaysInterface::Tasker(uint8_t function, JsonParserObject obj){
  
// return 0;

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
      EveryLoop();
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
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * OTHER SECTION * 
    *******************/
    case FUNC_SET_POWER:
      // DisplaySetPower();
    break;
    case FUNC_EVERY_SECOND:
      #ifdef USE_GRAPH
        DisplayCheckGraph();
      #endif
      #ifdef USE_DT_VARS
        get_dt_mqtt();
        draw_dt_vars();
      #endif // USE_DT_VARS
      DisplaySetPower();
    break;
    // case FUNC_AFTER_TELEPERIOD:
    //   #ifdef USE_DT_VARS
    //   DisplayDTVarsTeleperiod();
    //   #endif // USE_DT_VARS
    // break;
    // case FUNC_MQTT_SUBSCRIBE:
    //   DisplayMqttSubscribe();
    // break;
    // case FUNC_MQTT_DATA:
    //   result = DisplayMqttData();
    // break;
    // case FUNC_SHOW_SENSOR:
    //   DisplayLocalSensor();
    // break; 
  }
  
  return function_result;

} // END function


void mDisplaysInterface::Pre_Init(void)
{
  DisplayInitDriver();
  settings.fEnableSensor = true;
}


void mDisplaysInterface::Init(void)
{

}


void mDisplaysInterface::EveryLoop()
{

}


uint8_t mDisplaysInterface::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();

    JsonBuilderI->Add("model", pCONT_set->Settings.display.model);
    JsonBuilderI->Add("mode", pCONT_set->Settings.display.mode);
    JsonBuilderI->Add("refresh", pCONT_set->Settings.display.refresh);
    JsonBuilderI->Add("rows", pCONT_set->Settings.display.rows);
    JsonBuilderI->Add("cols0", pCONT_set->Settings.display.cols[0]);
    JsonBuilderI->Add("cols1", pCONT_set->Settings.display.cols[1]);
    JsonBuilderI->Add("address0", pCONT_set->Settings.display.address[0]);
    JsonBuilderI->Add("dimmer", pCONT_set->Settings.display.dimmer);
    JsonBuilderI->Add("size", pCONT_set->Settings.display.size);
    JsonBuilderI->Add("font", pCONT_set->Settings.display.font);
    JsonBuilderI->Add("rotate", pCONT_set->Settings.display.rotate);

    JsonBuilderI->Add("width", pCONT_set->Settings.display.width);
    JsonBuilderI->Add("height", pCONT_set->Settings.display.height);

  return JsonBuilderI->End();

}


uint8_t mDisplaysInterface::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_VOLTAGE, 0);
  return JsonBuilderI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mDisplaysInterface::MQTTHandler_Init(){

  struct handler<mDisplaysInterface>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mDisplaysInterface::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mDisplaysInterface::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mDisplaysInterface::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mDisplaysInterface::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mDisplaysInterface::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mDisplaysInterface::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, 
    EM_MODULE_DISPLAYS_INTERFACE_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id
  );

}

// /*********************************************************************************************/

// void DisplayInit(uint8_t mode)
// {
//   if (renderer)  {
//     renderer->DisplayInit(mode, pCONT_set->Settings.display.size, pCONT_set->Settings.display.rotate, pCONT_set->Settings.display.font);
//   }
//   else {
//     dsp_init = mode;
//     pCONT->Tasker_Interface(FUNC_DISPLAY_INIT);
//   }
// }

// void DisplayClear(void)
// {
//   pCONT->Tasker_Interface(FUNC_DISPLAY_CLEAR);
// }

void mDisplaysInterface::DisplayDrawStringAt(uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t flag)
{
  dsp_x = x;
  dsp_y = y;
  dsp_str = str;
  dsp_color = color;
  dsp_flag = flag;
  pCONT->Tasker_Interface(FUNC_DISPLAY_DRAW_STRING);
}

void mDisplaysInterface::DisplayOnOff(uint8_t on)
{
//   ExecuteCommandPower(disp_device, on, SRC_DISPLAY);
}

// /*-------------------------------------------------------------------------------------------*/

// // get asci float number
// uint8_t fatoiv(char *cp,float *res) {
//   uint8_t index=0;
//   *res=CharToFloat(cp);
//   while (*cp) {
//     if ((*cp>='0' && *cp<='9') || (*cp=='-') || (*cp=='.')) {
//       cp++;
//       index++;
//     } else {
//       break;
//     }
//   }
//   return index;
// }

// get asci number until delimiter and return asci number lenght and value
uint8_t mDisplaysInterface::atoiv(char *cp, int16_t *res)
{
  uint8_t index = 0;
  *res = atoi(cp);
  while (*cp) {
    if ((*cp>='0' && *cp<='9') || (*cp=='-')) {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

// get asci number until delimiter and return asci number lenght and value
uint8_t mDisplaysInterface::atoiV(char *cp, uint16_t *res)
{
  uint8_t index = 0;
  *res = atoi(cp);
  while (*cp) {
    if (*cp>='0' && *cp<='9') {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

// right align string
void mDisplaysInterface::alignright(char *string) {
  uint16_t slen=strlen(string);
  uint16_t len=slen;
  while (len) {
    // count spaces to the right
    if (string[len-1]!=' ') {
      break;
    }
    len--;
  }
  uint16_t diff=slen-len;
  if (diff>0) {
    // move string
    memmove(&string[diff],string,len);
    memset(string,' ',diff);
  }
}

char *mDisplaysInterface::get_string(char *buff,uint8_t len,char *cp) {
uint8_t index=0;
  while (*cp!=':') {
    buff[index]=*cp++;
    index++;
    if (index>=len) break;
  }
  buff[index]=0;
  cp++;
  return cp;
}

#define ESCAPE_CHAR '~'

// decode text escapes, 1 hexbyte assumed
uint32_t mDisplaysInterface::decode_te(char *line) {
  uint32_t skip = 0;
  char sbuf[3],*cp;
  while (*line) {
    if (*line==ESCAPE_CHAR) {
      cp=line+1;
      if (*cp!=0 && *cp==ESCAPE_CHAR) {
        // escape escape, discard one
        memmove(cp,cp+1,strlen(cp));
        skip++;
      } else {
        // escape HH
        if (strlen(cp)<2) {
          // illegal lenght, ignore
          return skip;
        }
        // take 2 hex chars
        sbuf[0]=*(cp);
        sbuf[1]=*(cp+1);
        sbuf[2]=0;
        *line=strtol(sbuf,0,16);
        // must shift string 2 bytes shift zero also
        memmove(cp,cp+2,strlen(cp)-1);
        skip += 2;
      }
    }
    line++;
  }
  return skip;
}

// /*-------------------------------------------------------------------------------------------*/

#define DISPLAY_BUFFER_COLS    128          // Max number of characters in linebuf

// uint16_t GetColorFromIndex(uint32_t index) {
//   if (index >= MAX_INDEXCOLORS) index = 0;

//   if (index < PREDEF_INDEXCOLORS) {
//     return renderer->GetColorFromIndex(index);
//   } else {
//     return index_colors[index - PREDEF_INDEXCOLORS];
//   }
// }

void mDisplaysInterface::DisplayText(const char* buffer)
{
  uint8_t lpos;
  uint8_t escape = 0;
  uint8_t var;
  int16_t lin = 0;
  int16_t col = 0;
  int16_t fill = 0;
  int16_t temp;
  int16_t temp1;
  float ftemp;

  char linebuf[DISPLAY_BUFFER_COLS];
  char *dp = linebuf;
  char *cp = (char*)buffer;//XdrvMailbox.data;

  memset(linebuf, ' ', sizeof(linebuf));
  linebuf[sizeof(linebuf)-1] = 0;
  *dp = 0;

  while (*cp) {
    if (!escape) {
      // check for escape
      // if (*cp == '[') {
      //   escape = 1;
      //   cp++;
      //   // if string in buffer print it
      //   if ((uint32_t)dp - (uint32_t)linebuf) {
      //     if (!fill) { *dp = 0; }
      //     if (col > 0 && lin > 0) {
      //       // use col and lin
      //       if (!renderer) DisplayDrawStringAt(col, lin, linebuf, fg_color, 1);
      //       else renderer->DrawStringAt(col, lin, linebuf, fg_color, 1);
      //     } else {
      //       // use disp_xpos, disp_ypos
      //       if (!renderer) DisplayDrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
      //       else renderer->DrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
      //     }
      //     memset(linebuf, ' ', sizeof(linebuf));
      //     linebuf[sizeof(linebuf)-1] = 0;
      //     dp = linebuf;
      //   }
      // } else {
      //   // copy chars
      //   if (dp < (linebuf + DISPLAY_BUFFER_COLS)) { *dp++ = *cp++; }
      // }
    } else {
      // check escapes
      if (*cp == ']') {
        escape = 0;
        cp++;
      } else {
        // analyze escapes
        switch (*cp++) {
          case 'z':
            // clear display
            if (!renderer) DisplayClear();
            else renderer->fillScreen(bg_color);
            disp_xpos = 0;
            disp_ypos = 0;
            col = 0;
            lin = 0;
            break;
          case 'i':
            // init display with partial update
            DisplayInit(DISPLAY_INIT_PARTIAL);
            break;
          case 'I':
            // init display with full refresh
            DisplayInit(DISPLAY_INIT_FULL);
            break;
          case 'o':
            DisplayOnOff(0);
            break;
          case 'O':
            DisplayOnOff(1);
            break;
          case 'x':
            // set disp_xpos
            var = atoiv(cp, &disp_xpos);
            cp += var;
            break;
          case 'y':
            // set disp_ypos
            var = atoiv(cp, &disp_ypos);
            cp += var;
            break;
          case 'l':
            // text line lxx
            var = atoiv(cp, &lin);
            cp += var;
            //display.setCursor(display.getCursorX(),(lin-1)*font_y*txtsize);
            break;
          case 'c':
            // text column cxx
            var = atoiv(cp, &col);
            cp += var;
            //display.setCursor((col-1)*font_x*txtsize,display.getCursorY());
            break;
          // case 'C':
          //   // text color cxx
          //   if (*cp=='i') {
          //     // color index 0-18
          //     cp++;
          //     var = atoiv(cp, &temp);
          //     if (renderer) ftemp = GetColorFromIndex(temp);
          //   } else {
          //     // float because it must handle unsigned 16 bit
          //     var = fatoiv(cp,&ftemp);
          //   }
          //   fg_color=ftemp;
          //   cp += var;
          //   if (renderer) renderer->setTextColor(fg_color,bg_color);
          //   break;
          // case 'B':
          //   // bg color Bxx
          //   if (*cp=='i') {
          //     // color index 0-18
          //     cp++;
          //     var = atoiv(cp, &temp);
          //     if (renderer) ftemp = GetColorFromIndex(temp);
          //   } else {
          //     var = fatoiv(cp,&ftemp);
          //   }
          //   bg_color=ftemp;
          //   cp += var;
          //   if (renderer) renderer->setTextColor(fg_color,bg_color);
          //   break;
          case 'p':
            // pad field with spaces fxx
            var = atoiv(cp, &fill);
            cp += var;
            linebuf[fill] = 0;
            break;
// #ifdef USE_UFILESYS
//           case 'P':
//             { char *ep=strchr(cp,':');
//              if (ep) {
//                *ep=0;
//                ep++;
//                Draw_RGB_Bitmap(cp,disp_xpos,disp_ypos, false);
//                cp=ep;
//              }
//             }
//             break;
// #ifdef USE_MULTI_DISPLAY
//           case 'S':
//             {
//               var = atoiv(cp, &temp);
//               cp += var;
//               if (temp < 1 || temp > 3) {
//                 temp = 1;
//               }
//               temp--;
//               if (*cp == ':') {
//                 cp++;
//                 if (displays[temp].display) {
//                   Set_display(cur_display);
//                   Get_display(temp);
//                 }
//               } else {
//                 char *ep=strchr(cp,':');
//                 if (ep) {
//                   *ep=0;
//                   ep++;
//                   File fp;
//                   if (ffsp) {
//                     AddLog(LOG_LEVEL_INFO, PSTR("DSP: File: %s"),cp);
//                     fp = ffsp->open(cp, "r");
//                     if (fp > 0) {
//                       uint32_t size = fp.size();
//                       char *fdesc = (char *)calloc(size + 4, 1);
//                       if (fdesc) {
//                         fp.read((uint8_t*)fdesc, size);
//                         fp.close();
//                         Get_display(temp);
//                         renderer = Init_uDisplay(fdesc, -1);
//                         Set_display(temp);
//                         AddLog(LOG_LEVEL_INFO, PSTR("DSP: File descriptor loaded %x"),renderer);
//                       }
//                     }
//                   }
//                 }
//                 cp = ep;
//               }
//             }
//             break;
// #endif // USE_MULTI_DISPLAY
// #endif // USE_UFILESYS
          case 'h':
            // hor line to
            var = atoiv(cp, &temp);
            cp += var;
            if (temp < 0) {
              if (renderer) renderer->writeFastHLine(disp_xpos + temp, disp_ypos, -temp, fg_color);
              //else DisplayDrawHLine(disp_xpos + temp, disp_ypos, -temp, fg_color);
            } else {
              if (renderer) renderer->writeFastHLine(disp_xpos, disp_ypos, temp, fg_color);
              //else DisplayDrawHLine(disp_xpos, disp_ypos, temp, fg_color);
            }
            disp_xpos += temp;
            break;
          case 'v':
            // vert line to
            var = atoiv(cp, &temp);
            cp += var;
            if (temp < 0) {
              if (renderer) renderer->writeFastVLine(disp_xpos, disp_ypos + temp, -temp, fg_color);
              //else DisplayDrawVLine(disp_xpos, disp_ypos + temp, -temp, fg_color);
            } else {
              if (renderer) renderer->writeFastVLine(disp_xpos, disp_ypos, temp, fg_color);
              //else DisplayDrawVLine(disp_xpos, disp_ypos, temp, fg_color);
            }
            disp_ypos += temp;
            break;
          case 'L':
            // any line to
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            if (renderer) renderer->writeLine(disp_xpos, disp_ypos, temp, temp1, fg_color);
            //else DisplayDrawLine(disp_xpos, disp_ypos, temp, temp1, fg_color);
            disp_xpos += temp;
            disp_ypos += temp1;
            break;
          case 'k':
            // circle
            var = atoiv(cp, &temp);
            cp += var;
            if (renderer) renderer->drawCircle(disp_xpos, disp_ypos, temp, fg_color);
            //else DisplayDrawCircle(disp_xpos, disp_ypos, temp, fg_color);
            break;
          case 'K':
            // filled circle
            var = atoiv(cp, &temp);
            cp += var;
            if (renderer) renderer->fillCircle(disp_xpos, disp_ypos, temp, fg_color);
            //else DisplayDrawFilledCircle(disp_xpos, disp_ypos, temp, fg_color);
            break;
          case 'r':
            // rectangle
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            if (renderer) renderer->drawRect(disp_xpos, disp_ypos, temp, temp1, fg_color);
            //else DisplayDrawRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            break;
          case 'R':
            // filled rectangle
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            if (renderer) renderer->fillRect(disp_xpos, disp_ypos, temp, temp1, fg_color);
            //else DisplayDrawFilledRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            break;
          case 'u':
            // rounded rectangle
            { int16_t rad;
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            cp++;
            var = atoiv(cp, &rad);
            cp += var;
            if (renderer) renderer->drawRoundRect(disp_xpos, disp_ypos, temp, temp1, rad, fg_color);
              //else DisplayDrawFilledRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            }
            break;
          case 'U':
            // rounded rectangle
            { int16_t rad;
            var = atoiv(cp, &temp);
            cp += var;
            cp++;
            var = atoiv(cp, &temp1);
            cp += var;
            cp++;
            var = atoiv(cp, &rad);
            cp += var;
            if (renderer) renderer->fillRoundRect(disp_xpos, disp_ypos, temp, temp1, rad, fg_color);
                  //else DisplayDrawFilledRectangle(disp_xpos, disp_ypos, temp, temp1, fg_color);
            }
            break;

          // case 't':
          //   if (*cp=='S') {
          //     cp++;
          //     if (dp < (linebuf + DISPLAY_BUFFER_COLS) -8) {
          //       snprintf_P(dp, 9, PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute, RtcTime.second);
          //       dp += 8;
          //     }
          //   } else {
          //     if (dp < (linebuf + DISPLAY_BUFFER_COLS) -5) {
          //       snprintf_P(dp, 6, PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d"), RtcTime.hour, RtcTime.minute);
          //       dp += 5;
          //     }
          //   }
          //   break;
          // case 'T': {
          //   uint8_t param1 = RtcTime.day_of_month;
          //   uint8_t param2 = RtcTime.month;
          //   if (*cp=='U') {
          //     cp++;
          //     param1 = RtcTime.month;
          //     param2 = RtcTime.day_of_month;
          //   }
          //   if (dp < (linebuf + DISPLAY_BUFFER_COLS) -8) {
          //     snprintf_P(dp, 9, PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%02d"), param1, param2, RtcTime.year%2000);
          //     dp += 8;
          //   }
          //   break; }
//           case 'd':
//             if (*cp == 'c') {
//               cp++;
//               // define index colo
//               var = atoiv(cp, &temp);
//               cp += var;
//               cp++;
//               var = fatoiv(cp, &ftemp);
//               cp += var;
//               if (temp >= MAX_INDEXCOLORS) temp = PREDEF_INDEXCOLORS;
//               if (temp < PREDEF_INDEXCOLORS) temp = PREDEF_INDEXCOLORS;
//               index_colors[temp - PREDEF_INDEXCOLORS] = ftemp;
//               break;
//             }
// #ifdef USE_DT_VARS
//           if (*cp == 'v') {
//             cp++;
//             { int16_t num, gxp, gyp, textbcol, textfcol, font, textsize, txlen, dp, time;
//               var=atoiv(cp,&num);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gxp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gyp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&textbcol);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&textfcol);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&font);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&textsize);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&txlen);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&dp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&time);
//               cp+=var;
//               cp++;
//               // text itself
//               char bbuff[32];
//               cp = get_string(bbuff, sizeof(bbuff), cp);
//               char unit[4];
//               cp = get_string(unit, sizeof(unit), cp);
//               define_dt_var(num, gxp, gyp, textbcol, textfcol, font, textsize, txlen, time, dp, bbuff, unit);
//             }
//           }
// #endif // USE_DT_VARS
//             // force draw grafics buffer
//             if (renderer) renderer->Updateframe();
//             //else DisplayDrawFrame();
//             break;
          case 'D':
            // set auto draw mode
            auto_draw=*cp&3;
            if (renderer) renderer->setDrawMode(auto_draw>>1);
            cp += 1;
            break;
          case 's':
            // size sx
            if (renderer) renderer->setTextSize(*cp&7);
            //else DisplaySetSize(*cp&3);
            cp += 1;
            break;
          case 'f':
            // font sx
            { uint8_t font = *cp&7;
              if (renderer) renderer->setTextFont(font);
              //else DisplaySetFont(font);
              if (font) {
                // for backward compatibility set size to 1 on non GFX fonts
                if (renderer) renderer->setTextSize(1);
                //else DisplaySetSize(1);
              }
              cp += 1;
            }
            break;
// #ifdef USE_UFILESYS
// #ifdef USE_RAMFONT
// extern FS *ffsp;
//           case 'F':
//             { char *ep = strchr(cp,':');
//               if (ep) {
//                 static uint8_t *ram_font;
//                 char fname[32];
//                 *ep = 0;
//                 ep++;
//                 if (*cp != '/') {
//                   fname[0] = '/';
//                   fname[1] = 0;
//                 } else {
//                   fname[0] = 0;
//                 }
//                 strlcat(fname, cp, sizeof(fname));
//                 if (!strstr(cp, ".fnt")) {
//                   strlcat(fname, ".fnt", sizeof(fname));
//                 }
//                 if (ffsp) {
//                   File fp;
//                   fp = ffsp->open(fname, "r");
//                   if (fp > 0) {
//                     uint32_t size = fp.size();
//                     if (ram_font) free (ram_font);
//                     ram_font = (uint8_t*)special_malloc(size + 4);
//                     fp.read((uint8_t*)ram_font, size);
//                     fp.close();
//                     if (renderer) renderer->SetRamfont(ram_font);
//                     //Serial.printf("Font loaded: %s\n",fname );
//                   }
//                 }
//                 cp = ep;
//               }
//             }
//             break;
// #endif // USE_RAMFONT
// #endif // USE_UFILESYS
          case 'a':
            // rotation angle
            if (renderer) renderer->setRotation(*cp&3);
            //else DisplaySetRotation(*cp&3);
            cp+=1;
            break;

// #ifdef USE_GRAPH
//           case 'G':
//             // define graph
//             if (*cp=='d') {
//               cp++;
//               var=atoiv(cp,&temp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&temp1);
//               cp+=var;
//               RedrawGraph(temp,temp1);
//               break;
//             }
// #if (defined(USE_SCRIPT_FATFS) && defined(USE_SCRIPT)) || defined(USE_UFILESYS)
//             if (*cp=='s') {
//               cp++;
//               var=atoiv(cp,&temp);
//               cp+=var;
//               cp++;
//               // path
//               char bbuff[128];
//               cp=get_string(bbuff,sizeof(bbuff),cp);
//               Save_graph(temp,bbuff);
//               break;
//             }
//             if (*cp=='r') {
//               cp++;
//               var=atoiv(cp,&temp);
//               cp+=var;
//               cp++;
//               // path
//               char bbuff[128];
//               cp=get_string(bbuff,sizeof(bbuff),cp);
//               Restore_graph(temp,bbuff);
//               break;
//             }
// #endif // USE_SCRIPT_FATFS
//             { int16_t num,gxp,gyp,gxs,gys,dec,icol;
//               float ymin,ymax;
//               var=atoiv(cp,&num);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gxp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gyp);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gxs);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&gys);
//               cp+=var;
//               cp++;
//               var=atoiv(cp,&dec);
//               cp+=var;
//               cp++;
//               var=fatoiv(cp,&ymin);
//               cp+=var;
//               cp++;
//               var=fatoiv(cp,&ymax);
//               cp+=var;
//               if (color_type==COLOR_COLOR) {
//                 // color graph requires channel color
//                 cp++;
//                 var=atoiv(cp,&icol);
//                 cp+=var;
//               } else {
//                 icol=0;
//               }
//               DefineGraph(num,gxp,gyp,gxs,gys,dec,ymin,ymax,icol);
//             }
//             break;
//           case 'g':
//               { float temp;
//                 int16_t num;
//                 var=atoiv(cp,&num);
//                 cp+=var;
//                 cp++;
//                 var=fatoiv(cp,&temp);
//                 cp+=var;
//                 AddValue(num,temp);
//               }
//             break;
// #endif // USE_GRAPH

// #ifdef USE_AWATCH
//           case 'w':
//               var = atoiv(cp, &temp);
//               cp += var;
//               DrawAClock(temp);
//               break;
// #endif // USE_AWATCH

// #ifdef USE_TOUCH_BUTTONS
//           case 'b':
//           { int16_t num, gxp, gyp, gxs, gys, outline, fill, textcolor, textsize; uint8_t dflg = 1, sbt = 0;
//             if (*cp == 'e' || *cp == 'd') {
//               // enable disable
//               uint8_t dis = 0;
//               if (*cp == 'd') dis = 1;
//               cp++;
//               var = atoiv(cp, &num);
//               num = num % MAX_TOUCH_BUTTONS;
//               cp += var;
//               if (buttons[num]) {
//                 buttons[num]->vpower.disable = dis;
//                 if (!dis) {
//                   if (buttons[num]->vpower.is_virtual) buttons[num]->xdrawButton(buttons[num]->vpower.on_off);
//                   else buttons[num]->xdrawButton(bitRead(pCONT_set->runtime_var.power,num));
//                 }
//               }
//               break;
//             }
//             if (*cp == '-') {
//               cp++;
//               dflg = 0;
//             }
//             if (*cp == 's') {
//               cp++;
//               sbt = 1;
//             }
//             var=atoiv(cp,&num);
//             cp+=var;
//             uint8_t bflags=num>>8;
//             num=num%MAX_TOUCH_BUTTONS;
//             if (*cp == 's') {
//               cp++;
//               var=atoiv(cp,&gxp);
//               if (buttons[num]) {
//                 // set slider or button
//                 if (buttons[num]->vpower.slider) {
//                   buttons[num]->UpdateSlider(-gxp, -gxp);
//                 } else {
//                   buttons[num]->vpower.on_off = gxp;
//                   buttons[num]->xdrawButton(buttons[num]->vpower.on_off);
//                 }
//               }
//               break;
//             }
//             cp++;
//             var=atoiv(cp,&gxp);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&gyp);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&gxs);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&gys);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&outline);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&fill);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&textcolor);
//             cp+=var;
//             cp++;
//             var=atoiv(cp,&textsize);
//             cp+=var;
//             cp++;
//             // text itself
//             char bbuff[32];
//             if (!sbt) {
//               // text itself
//               cp = get_string(bbuff, sizeof(bbuff), cp);
//             }
//             if (buttons[num]) {
//               delete buttons[num];
//             }
//             if (renderer) {
//               buttons[num]= new VButton();
//               if (buttons[num]) {
//                 if (!sbt) {
//                   buttons[num]->vpower.slider = 0;
//                   buttons[num]->initButtonUL(renderer, gxp, gyp, gxs, gys, GetColorFromIndex(outline),\
//                     GetColorFromIndex(fill), GetColorFromIndex(textcolor), bbuff, textsize);
//                   if (!bflags) {
//                     // power button
//                     if (dflg) buttons[num]->xdrawButton(bitRead(pCONT_set->runtime_var.power, num));
//                     buttons[num]->vpower.is_virtual = 0;
//                   } else {
//                     // virtual button
//                     buttons[num]->vpower.is_virtual = 1;
//                     if (bflags==2) {
//                       // push
//                       buttons[num]->vpower.is_pushbutton = 1;
//                     } else {
//                       // toggle
//                       buttons[num]->vpower.is_pushbutton = 0;
//                     }
//                     if (dflg) buttons[num]->xdrawButton(buttons[num]->vpower.on_off);
//                     buttons[num]->vpower.disable = !dflg;
//                   }
//                 } else {
//                   // slider
//                   buttons[num]->vpower.slider = 1;
//                   buttons[num]->SliderInit(renderer, gxp, gyp, gxs, gys, outline, GetColorFromIndex(fill),\
//                     GetColorFromIndex(textcolor), GetColorFromIndex(textsize));
//                 }
//               }
//             }
//           }
//           break;
// #endif // USE_TOUCH_BUTTONS
          default:
            // unknown escape
            // Response_P(PSTR("Unknown Escape"));
            goto exit;
            break;
        }
      }
    }
  }
  exit:
  // now draw buffer
    dp -= decode_te(linebuf);
    if ((uint32_t)dp - (uint32_t)linebuf) {
      if (!fill) {
        *dp = 0;
      } else {
        linebuf[abs(int(fill))] = 0;
      }
      if (fill<0) {
        // right align
        alignright(linebuf);
      }
      if (col > 0 && lin > 0) {
        // use col and lin
        if (!renderer) DisplayDrawStringAt(col, lin, linebuf, fg_color, 1);
        else renderer->DrawStringAt(col, lin, linebuf, fg_color, 1);
      } else {
        // use disp_xpos, disp_ypos
        if (!renderer) DisplayDrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
        else renderer->DrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
      }
    }
    // draw buffer
    if (auto_draw&1) {
      if (renderer) renderer->Updateframe();
      //else DisplayDrawFrame();
    }
} //end function

// /*********************************************************************************************/

// #ifdef USE_DISPLAY_MODES1TO5

void mDisplaysInterface::DisplayClearScreenBuffer(void)
{
  if (disp_screen_buffer_cols) {
    for (uint32_t i = 0; i < disp_screen_buffer_rows; i++) {
      memset(disp_screen_buffer[i], 0, disp_screen_buffer_cols);
    }
  }
}

void mDisplaysInterface::DisplayFreeScreenBuffer(void)
{
  if (disp_screen_buffer != nullptr) {
    for (uint32_t i = 0; i < disp_screen_buffer_rows; i++) {
      if (disp_screen_buffer[i] != nullptr) { free(disp_screen_buffer[i]); }
    }
    free(disp_screen_buffer);
    disp_screen_buffer_cols = 0;
    disp_screen_buffer_rows = 0;
  }
}

void mDisplaysInterface::DisplayAllocScreenBuffer(void)
{
  if (!disp_screen_buffer_cols) {
    disp_screen_buffer_rows = pCONT_set->Settings.display.rows;
    disp_screen_buffer = (char**)malloc(sizeof(*disp_screen_buffer) * disp_screen_buffer_rows);
    if (disp_screen_buffer != nullptr) {
      for (uint32_t i = 0; i < disp_screen_buffer_rows; i++) {
        disp_screen_buffer[i] = (char*)malloc(sizeof(*disp_screen_buffer[i]) * (pCONT_set->Settings.display.cols[0] +1));
        if (disp_screen_buffer[i] == nullptr) {
          DisplayFreeScreenBuffer();
          break;
        }
      }
    }
    if (disp_screen_buffer != nullptr) {
      disp_screen_buffer_cols = pCONT_set->Settings.display.cols[0] +1;
      DisplayClearScreenBuffer();
    }
  }
}

void mDisplaysInterface::DisplayReAllocScreenBuffer(void)
{
  DisplayFreeScreenBuffer();
  DisplayAllocScreenBuffer();
}

void mDisplaysInterface::DisplayFillScreen(uint32_t line)
{
  uint32_t len = disp_screen_buffer_cols - strlen(disp_screen_buffer[line]);
  if (len) {
    memset(disp_screen_buffer[line] + strlen(disp_screen_buffer[line]), 0x20, len);
    disp_screen_buffer[line][disp_screen_buffer_cols -1] = 0;
  }
}

/*-------------------------------------------------------------------------------------------*/

void mDisplaysInterface::DisplayClearLogBuffer(void)
{
  if (disp_log_buffer_cols) {
    for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
      memset(disp_log_buffer[i], 0, disp_log_buffer_cols);
    }
  }
}

void mDisplaysInterface::DisplayFreeLogBuffer(void)
{
  if (disp_log_buffer != nullptr) {
    for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
      if (disp_log_buffer[i] != nullptr) { free(disp_log_buffer[i]); }
    }
    free(disp_log_buffer);
    disp_log_buffer_cols = 0;
  }
}

void mDisplaysInterface::DisplayAllocLogBuffer(void)
{
  if (!disp_log_buffer_cols) {
    disp_log_buffer = (char**)malloc(sizeof(*disp_log_buffer) * DISPLAY_LOG_ROWS);
    if (disp_log_buffer != nullptr) {
      for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
        disp_log_buffer[i] = (char*)malloc(sizeof(*disp_log_buffer[i]) * (pCONT_set->Settings.display.cols[0] +1));
        if (disp_log_buffer[i] == nullptr) {
          DisplayFreeLogBuffer();
          break;
        }
      }
    }
    if (disp_log_buffer != nullptr) {
      disp_log_buffer_cols = pCONT_set->Settings.display.cols[0] +1;
      DisplayClearLogBuffer();
    }
  }
}

void mDisplaysInterface::DisplayReAllocLogBuffer(void)
{
  DisplayFreeLogBuffer();
  DisplayAllocLogBuffer();
}

void mDisplaysInterface::DisplayLogBufferAdd(char* txt)
{
  if (disp_log_buffer_cols) {
    strlcpy(disp_log_buffer[disp_log_buffer_idx], txt, disp_log_buffer_cols);  // This preserves the % sign where printf won't
    disp_log_buffer_idx++;
    if (DISPLAY_LOG_ROWS == disp_log_buffer_idx) { disp_log_buffer_idx = 0; }
  }
}

char* mDisplaysInterface::DisplayLogBuffer(char temp_code)
{
  char* result = nullptr;
  if (disp_log_buffer_cols) {
    if (disp_log_buffer_idx != disp_log_buffer_ptr) {
      result = disp_log_buffer[disp_log_buffer_ptr];
      disp_log_buffer_ptr++;
      if (DISPLAY_LOG_ROWS == disp_log_buffer_ptr) { disp_log_buffer_ptr = 0; }

      char *pch = strchr(result, '~');  // = 0x7E (~) Replace degrees character (276 octal)
      if (pch != nullptr) { result[pch - result] = temp_code; }
    }
  }
  return result;
}

void mDisplaysInterface::DisplayLogBufferInit(void)
{
  
  // if (pCONT_set->Settings.display.mode) {
    
    disp_log_buffer_idx = 0;
    disp_log_buffer_ptr = 0;
    disp_refresh = pCONT_set->Settings.display.refresh;

    // snprintf_P(disp_temp, sizeof(disp_temp), PSTR("%c"), TempUnit());
    // snprintf_P(disp_pres, sizeof(disp_pres), PressureUnit().c_str());

    DisplayReAllocLogBuffer();

    char buffer[100];
    snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_VERSIONNAME "\n %s"), pCONT_set->firmware_version.current.name_ctr);//pCONT_set->runtime_var.version, pCONT_set->runtime_var.image_name);
    DisplayLogBufferAdd(buffer);
    snprintf_P(buffer, sizeof(buffer), PSTR("Display mode %d"), pCONT_set->Settings.display.mode);
    DisplayLogBufferAdd(buffer);

    // snprintf_P(buffer, sizeof(buffer), PSTR(D_CMND_HOSTNAME " %s"), NetworkHostname());
    // DisplayLogBufferAdd(buffer);
    // snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_MAC " %s"), NetworkMacAddress().c_str());
    // DisplayLogBufferAdd(buffer);
    // ext_snprintf_P(buffer, sizeof(buffer), PSTR("IP %_I"), (uint32_t)NetworkAddress());
    // DisplayLogBufferAdd(buffer);
    // if (!pCONT_set->runtime_var.global_state.wifi_down) {
    //   snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_SSID " %s"), SettingsText(SET_STASSID1 + Settings.sta_active));
    //   DisplayLogBufferAdd(buffer);
    //   snprintf_P(buffer, sizeof(buffer), PSTR(D_JSON_RSSI " %d%%"), WifiGetRssiAsQuality(WiFi.RSSI()));
    //   DisplayLogBufferAdd(buffer);
    // }
  // }
  
}


/*********************************************************************************************\
 * Public
\*********************************************************************************************/

void mDisplaysInterface::DisplayInitDriver(void)
{
  pCONT->Tasker_Interface(FUNC_DISPLAY_INIT_DRIVER);

#ifdef USE_MULTI_DISPLAY
  Set_display(0);
#endif // USE_MULTI_DISPLAY

  if (renderer) {
    renderer->setTextFont(pCONT_set->Settings.display.font);
    renderer->setTextSize(pCONT_set->Settings.display.size);
    // force opaque mode
    renderer->setDrawMode(0);

    for (uint32_t cnt = 0; cnt < (MAX_INDEXCOLORS - PREDEF_INDEXCOLORS); cnt++) {
      index_colors[cnt] = 0;
    }
  }

  #ifdef USE_DT_VARS
    free_dt_vars();
  #endif

  #ifdef USE_UFILESYS
    Display_Text_From_File("/display.ini");
  #endif

  #ifdef USE_GRAPH
    for (uint8_t count = 0; count < NUM_GRAPHS; count++) { graph[count] = 0; }
  #endif

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Display model %d"), pCONT_set->Settings.display.model);

  pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_UTC_TIME_ID;

  if (pCONT_set->Settings.display.model) {
    pCONT_set->devices_present++;
    if (!pCONT_pins->PinUsed(GPIO_BACKLIGHT_ID)) {
      if (pCONT_set->runtime_var.light_type && (4 == pCONT_set->Settings.display.model)) {
        pCONT_set->devices_present--;  // Assume PWM channel is used for backlight
      }
    }
    disp_device = pCONT_set->devices_present;

    #ifndef USE_DISPLAY_MODES1TO5
      pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_USER_TEXT_SERIALISED_ID;
    #else
      DisplayLogBufferInit();
    #endif  // USE_DISPLAY_MODES1TO5
    }

    // pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_LOCAL1_ID;
    
}

void mDisplaysInterface::DisplaySetPower(void)
{
  disp_power = 1;//bitRead(XdrvMailbox.index, disp_device -1);

//AddLog(LOG_LEVEL_DEBUG, PSTR("DSP: Power %d"), disp_power);

  if (pCONT_set->Settings.display.model) {
    if (!renderer) {
      pCONT->Tasker_Interface(FUNC_DISPLAY_POWER);
    } else {
      renderer->DisplayOnff(disp_power);
    }
  }
  
}


// void DisplayReInitDriver(void) {
//   pCONT->Tasker_Interface(FUNC_DISPLAY_INIT_DRIVER);
// #ifdef USE_MULTI_DISPLAY
//   Set_display(0);
// #endif // USE_MULTI_DISPLAY
//   ResponseCmndDone();
// }



/**
 * @brief Indirect function layer to allow debugging
 * @note command will contain checks for viability
 * */
void mDisplaysInterface::SetCursor(uint8_t x, uint8_t y)
{
  AddLog(LOG_LEVEL_TEST, PSTR("SetCursor(%d,%d)"),x,y);
  pCONT_iDisp->renderer->setCursor(x,y);
}

/**
 * @brief Indirect function layer to allow debugging
 * @note command will contain checks for viability
 * */
void mDisplaysInterface::SetTextSize(uint8_t font_size)
{
  AddLog(LOG_LEVEL_TEST, PSTR("SetFontSize(%d)"),font_size);
  pCONT_iDisp->renderer->setTextSize(font_size);
}

/**
 * @brief Indirect function layer to allow debugging
 * @note command will contain checks for viability
 * */
void mDisplaysInterface::SetDisplayMode(uint8_t mode)
{
  AddLog(LOG_LEVEL_TEST, PSTR("SetDisplayMode(%d)"),mode);
  pCONT_set->Settings.display.mode = mode;
}



#endif
