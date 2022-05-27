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


#ifdef ENABLE_DISPLAY_MODE_USER_TEXT_SERIALISED

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

  // {"DisplayText":"[Michael]"}

  char linebuf[DISPLAY_BUFFER_COLS];
  char *dp = linebuf;
  char *cp = (char*)buffer;

  AddLog(LOG_LEVEL_TEST, PSTR("DisplayText %s"),cp);

  memset(linebuf, ' ', sizeof(linebuf));
  linebuf[sizeof(linebuf)-1] = 0;
  *dp = 0;

  while (*cp) {
    if (!escape) {
      // check for escape
      if (*cp == '[') {
        escape = 1;
        cp++;
        // if string in buffer print it
        if ((uint32_t)dp - (uint32_t)linebuf) {
          if (!fill) { *dp = 0; }
          if (col > 0 && lin > 0) {
            // use col and lin
            if (!renderer) DisplayDrawStringAt(col, lin, linebuf, fg_color, 1);
            else renderer->DrawStringAt(col, lin, linebuf, fg_color, 1);
          } else {
            // use disp_xpos, disp_ypos
            if (!renderer) DisplayDrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
            else renderer->DrawStringAt(disp_xpos, disp_ypos, linebuf, fg_color, 0);
          }
          memset(linebuf, ' ', sizeof(linebuf));
          linebuf[sizeof(linebuf)-1] = 0;
          dp = linebuf;
        }
      } else {
        // copy chars
        if (dp < (linebuf + DISPLAY_BUFFER_COLS)) { *dp++ = *cp++; }
      }
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

#endif // ENABLE_DISPLAY_MODE_USER_TEXT_SERIALISED


#endif