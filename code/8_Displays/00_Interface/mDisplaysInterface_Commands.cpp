#include "mDisplaysInterface.h"

#ifdef USE_MODULE_DISPLAYS_INTERFACE
// {
//   "DisplayStaticLog":["Open","file123456","V 12:34:56","2 kB"]
// }
void mDisplaysInterface::parse_JSONCommand(JsonParserObject obj){

  char buffer[50];
  JsonParserToken jtok = 0; 
  JsonParserToken jtok_level = 0; 
  JsonParserObject obj_sub = 0;
  int8_t tmp_id = 0;

  /**
   * @brief New method
   * How to check for a json level that is repeated 
   */
  if(obj_sub = obj[PM_JSON_DISPLAY])
  {

    if(jtok = obj_sub[PM_JSON_MODEL])
    {
      pCONT_set->Settings.display.model = jtok.getInt();
      ALOG_COM( PM_JSON_COMMAND_PM_SVALUE_SVALUE_NVALUE, PM_JSON_DISPLAY, PM_JSON_MODEL, pCONT_set->Settings.display.model );
    }

    if(jtok = obj_sub[PM_JSON_MODE])
    {
      pCONT_set->Settings.display.mode = jtok.getInt();
      ALOG_COM( PM_JSON_COMMAND_PM_SVALUE_SVALUE_NVALUE, PM_JSON_DISPLAY, PM_JSON_MODE, pCONT_set->Settings.display.mode );
    }

    // if(jtok = obj["DisplayRefresh"]){
    //   pCONT_set->Settings.display.refresh = jtok.getInt();
    // }
    // if(jtok = obj["DisplayRows"]){
    //   pCONT_set->Settings.display.rows = jtok.getInt();
    // }
    // if(jtok = obj["DisplayCols"])
    // {
    //   JsonParserArray arr_pos = jtok;
    //   if(arr_pos.size() == 2)
    //   {
    //     pCONT_set->Settings.display.cols[0] = arr_pos[0].getInt();
    //     pCONT_set->Settings.display.cols[1] = arr_pos[1].getInt();
    //   }
    // }
    // if(jtok = obj["DisplayDimmer"]){
    //   pCONT_set->Settings.display.dimmer = jtok.getInt();
    // }
    // if(jtok = obj["DisplaySize"]){
    //   pCONT_set->Settings.display.size = jtok.getInt();
    // }
    // if(jtok = obj["DisplayFont"]){
    //   pCONT_set->Settings.display.font = jtok.getInt();
    // }
    // if(jtok = obj["DisplayRotate"]){
    //   pCONT_set->Settings.display.rotate = jtok.getInt();
    // }


    if(jtok = obj["DisplayText"]){
      CmndDisplayText(jtok.getStr());
    //   if(jtok.isStr()){
    //     if((tmp_id=mPaletteI->GetPaletteIDbyName(jtok.getStr()))>=0){
    //       CommandSet_PaletteID(tmp_id);
    //       data_buffer.isserviced++;
    //     }
    //   }else
    //   if(jtok.isNum()){
    //     CommandSet_PaletteID(jtok.getInt());
    //     data_buffer.isserviced++;
    //   }
    //   #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "DisplayText"));//D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
    //   #endif // ENABLE_LOG_LEVEL_DEBUG
    }

    /**
     * @brief Display:AddLog
     * */
    if(jtok = obj_sub[PM_JSON_ADDLOG])
    {
      if(jtok.isStr()){
        CommandSet_DisplayAddLog(jtok.getStr());
      }
      ALOG_COM(PM_JSON_COMMAND_PM_SVALUE_SVALUE_SVALUE, PM_JSON_DISPLAY, PM_JSON_ADDLOG, jtok.getStr());
    }

    /**
     * @brief Display:ClearLog
     * */
    if(jtok = obj_sub[PM_JSON_CLEARLOG])
    {
      if(jtok.isInt()){
        CommandSet_DisplayClearLog(jtok.getInt());
      }
      ALOG_COM(PM_JSON_COMMAND_PM_SVALUE_SVALUE_NVALUE, PM_JSON_DISPLAY, PM_JSON_CLEARLOG, jtok.getInt());
    }

    if(jtok = obj_sub[PM_JSON_BRIGHTNESS])
    {
      // if(jtok.isInt()){
      //   CommandSet_DisplayClearLog(jtok.getInt());
      // }

// I need to introduce how tasmota has a received/mailbox so I can store the brightness to then call sub displays to set them by hardware

      ALOG_COM(PM_JSON_COMMAND_PM_SVALUE_SVALUE_NVALUE, PM_JSON_DISPLAY, PM_JSON_CLEARLOG, jtok.getInt());
    }


    
//     if(!obj["brightness"].isNull()){ 
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION D_PARSING_MATCHED "brightness"));    
//       uint8_t brightness = obj["brightness"];
//       //nextionSetAttr("dim", String(brightness));
//       //sprintf(command_ctr,"dims=%d",brightness);
//       nextionSendCmd("dims=dim");
//     }else



  }






  /**
   * @brief DisplayAddLog
   * */
  if(jtok = obj["DisplayAddStaticLog"]){
    // if(jtok.isStr()){
    //   CommandSet_DisplayAddLog(jtok.getStr());
    // }

    pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
    LogBuffer_Add((char*)jtok.getStr());

    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "DisplayAddLog %s"),jtok.getStr());//D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
    #endif
  }



  /**
   * @brief DisplayStaticLog with direct index
   * */
  if(jtok = obj["DisplayStaticLog"].getObject()["Row"]){
    uint8_t row_number = jtok.getInt();
    if(jtok = obj["DisplayStaticLog"].getObject()["Text"]){
      pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
      LogBuffer_AddRow((char*)jtok.getStr(), row_number);
    }
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "DisplayAddLog %s"),jtok.getStr());
    #endif
  }

  /**
   * @brief DisplayStaticLog with arrays per line
   * */
  if(jtok = obj["DisplayStaticLog"])
  {
    if(jtok.isArray())
    {
      pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
      JsonParserArray array = jtok;
      uint8_t index = 0;
      LogBuffer_Clear();
      for(auto& object:array)
      {
        LogBuffer_AddRow((char*)object.getStr(), index++);
      }
    }
    #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "DisplayAddLog %s"),jtok.getStr());
    #endif
  }




  if(jtok = obj["ClearDisplay"]){

    // create time out options where the screen can auto clear

    pCONT_iDisp->renderer->clearDisplay();

      // CmndDisplayText(jtok.getStr());


  }

  if(jtok = obj["DrawText"]){
    CommandSet_DisplayText_Advanced_JSON(obj);

    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "DrawText" ));//D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
    // #endif // ENABLE_LOG_LEVEL_DEBUG


  }


  if(jtok = obj[PM_JSON_DISPLAY_MODE]){
    SetDisplayMode(jtok.getInt());
  }


  // mqtthandler_debug_teleperiod.flags.SendNow = true;

}


// /*********************************************************************************************\
//  * Commands
// \*********************************************************************************************/


/**
 * @brief CommandSet_DisplayAddLog
 * */
void mDisplaysInterface::CommandSet_DisplayAddLog(const char* c)
{  
  SetDisplayMode(EM_DISPLAY_MODE_LOCAL1_ID);
  LogBuffer_Add((char*)c);

  // Change to flag method later, so this function will not be called for every command
  pCONT->Tasker_Interface(FUNC_DISPLAY_REFRESH_SHOW_ID);

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "DisplayAddLog %s"),c);//D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
  #endif
}

/**
 * @brief CommandSet_DisplayClearLog
 * */
void mDisplaysInterface::CommandSet_DisplayClearLog(bool d)
{  
  LogBuffer_Clear(); 
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "DisplayClearLog"));//D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
  #endif
}


/**
 * @brief CommandSet_DisplayAddLog
 * */
// void mDisplaysInterface::CommandSet_DisplayAddLog(const char* c)
// {  
//   SetDisplayMode(EM_DISPLAY_MODE_LOG_STATIC_ID);
//   LogBuffer_Add((char*)c);
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "DisplayAddLog %s"),c);//D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), GetPaletteNameByID(animation.palette.id, buffer, sizeof(buffer)));
//   #endif
// }





// void CmndDisplay(void) {
//   Response_P(PSTR("{\"" D_PRFX_DISPLAY "\":{\"" D_CMND_DISP_MODEL "\":%d,\"" D_CMND_DISP_TYPE "\":%d,\"" D_CMND_DISP_WIDTH "\":%d,\"" D_CMND_DISP_HEIGHT "\":%d,\""
//     D_CMND_DISP_MODE "\":%d,\"" D_CMND_DISP_DIMMER "\":%d,\"" D_CMND_DISP_SIZE "\":%d,\"" D_CMND_DISP_FONT "\":%d,\""
//     D_CMND_DISP_ROTATE "\":%d,\"" D_CMND_DISP_INVERT "\":%d,\"" D_CMND_DISP_REFRESH "\":%d,\"" D_CMND_DISP_COLS "\":[%d,%d],\"" D_CMND_DISP_ROWS "\":%d}}"),
//     pCONT_set->Settings.display.model, pCONT_set->Settings.display.options.type, pCONT_set->Settings.display.width, pCONT_set->Settings.display.height,
//     pCONT_set->Settings.display.mode, changeUIntScale(pCONT_set->Settings.display.dimmer, 0, 15, 0, 100), pCONT_set->Settings.display.size, pCONT_set->Settings.display.font,
//     pCONT_set->Settings.display.rotate, pCONT_set->Settings.display.options.invert, pCONT_set->Settings.display.refresh, pCONT_set->Settings.display.cols[0], pCONT_set->Settings.display.cols[1], pCONT_set->Settings.display.rows);
// }

// void CmndDisplayModel(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < DISPLAY_MAX_DRIVERS)) {
//     uint32_t last_display_model = pCONT_set->Settings.display.model;
//     pCONT_set->Settings.display.model = XdrvMailbox.payload;
//     if (pCONT->Tasker_Interface(FUNC_DISPLAY_MODEL)) {
//       pCONT_set->runtime_var.restart_flag = 2;  // Restart to re-init interface and add/Remove MQTT subscribe
//     } else {
//       pCONT_set->Settings.display.model = last_display_model;
//     }
//   }
//   ResponseCmndNumber(pCONT_set->Settings.display.model);
// }

// void CmndDisplayType(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 7)) {
//     pCONT_set->Settings.display.options.type = XdrvMailbox.payload;
//     pCONT_set->runtime_var.restart_flag = 2;
//   }
//   ResponseCmndNumber(pCONT_set->Settings.display.options.type);
// }

// void CmndDisplayWidth(void) {
//   if (XdrvMailbox.payload > 0) {
//     if (XdrvMailbox.payload != pCONT_set->Settings.display.width) {
//       pCONT_set->Settings.display.width = XdrvMailbox.payload;
//       pCONT_set->runtime_var.restart_flag = 2;  // Restart to re-init width
//     }
//   }
//   ResponseCmndNumber(pCONT_set->Settings.display.width);
// }

// void CmndDisplayHeight(void) {
//   if (XdrvMailbox.payload > 0) {
//     if (XdrvMailbox.payload != pCONT_set->Settings.display.height) {
//       pCONT_set->Settings.display.height = XdrvMailbox.payload;
//       pCONT_set->runtime_var.restart_flag = 2;  // Restart to re-init height
//     }
//   }
//   ResponseCmndNumber(pCONT_set->Settings.display.height);
// }

// void CmndDisplayMode(void) {
// #ifdef USE_DISPLAY_MODES1TO5
// /*     Matrix / 7-segment   LCD / Oled                           TFT
//  * 1 = Text up and time     Time
//  * 2 = Date                 Local sensors                        Local sensors
//  * 3 = Day                  Local sensors and time               Local sensors and time
//  * 4 = Mqtt left and time   Mqtt (incl local) sensors            Mqtt (incl local) sensors
//  * 5 = Mqtt up and time     Mqtt (incl local) sensors and time   Mqtt (incl local) sensors and time
// */
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 5)) {
//     uint32_t last_display_mode = pCONT_set->Settings.display.mode;
//     pCONT_set->Settings.display.mode = XdrvMailbox.payload;

//     if (disp_subscribed != (pCONT_set->Settings.display.mode &0x04)) {
//       pCONT_set->runtime_var.restart_flag = 2;  // Restart to Add/Remove MQTT subscribe
//     } else {
//       if (last_display_mode && !pCONT_set->Settings.display.mode) {  // Switch to mode 0
//         DisplayInit(DISPLAY_INIT_MODE);
//         if (renderer) renderer->fillScreen(bg_color);
//         else DisplayClear();
//       } else {
//         LogBuffer_Init();
//         DisplayInit(DISPLAY_INIT_MODE);
//       }
//     }
//   }
// #endif  // USE_DISPLAY_MODES1TO5
//   ResponseCmndNumber(pCONT_set->Settings.display.mode);
// }

// void CmndDisplayDimmer(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 100)) {
//     pCONT_set->Settings.display.dimmer = changeUIntScale(XdrvMailbox.payload, 0, 100, 0, 15);  // Correction for Domoticz (0 - 15)
//     if (pCONT_set->Settings.display.dimmer && !(disp_power)) {
//       ExecuteCommandPower(disp_device, POWER_ON, SRC_DISPLAY);
//     }
//     else if (!pCONT_set->Settings.display.dimmer && disp_power) {
//       ExecuteCommandPower(disp_device, POWER_OFF, SRC_DISPLAY);
//     }
//     if (renderer) {
//       renderer->dim(pCONT_set->Settings.display.dimmer);
//     } else {
//       pCONT->Tasker_Interface(FUNC_DISPLAY_DIM);
//     }
//   }
//   ResponseCmndNumber(changeUIntScale(pCONT_set->Settings.display.dimmer, 0, 15, 0, 100));
// }

// void CmndDisplaySize(void) {
//   if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= 4)) {
//     pCONT_set->Settings.display.size = XdrvMailbox.payload;
//     if (renderer) renderer->setTextSize(pCONT_set->Settings.display.size);
//     //else DisplaySetSize(pCONT_set->Settings.display.size);
//   }
//   ResponseCmndNumber(pCONT_set->Settings.display.size);
// }

// void CmndDisplayFont(void) {
//   if ((XdrvMailbox.payload >=0) && (XdrvMailbox.payload <= 4)) {
//     pCONT_set->Settings.display.font = XdrvMailbox.payload;
//     if (renderer) renderer->setTextFont(pCONT_set->Settings.display.font);
//     //else DisplaySetFont(pCONT_set->Settings.display.font);
//   }
//   ResponseCmndNumber(pCONT_set->Settings.display.font);
// }

// void CmndDisplayRotate(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload < 4)) {
//     if ((pCONT_set->Settings.display.rotate) != XdrvMailbox.payload) {
// /*
//       // Needs font info regarding height and width
//       if ((pCONT_set->Settings.display.rotate &1) != (XdrvMailbox.payload &1)) {
//         uint8_t temp_rows = pCONT_set->Settings.display.rows;
//         pCONT_set->Settings.display.rows = pCONT_set->Settings.display.cols[0];
//         pCONT_set->Settings.display.cols[0] = temp_rows;
// #ifdef USE_DISPLAY_MODES1TO5
//         ScreenBuffer_ReAlloc();
// #endif  // USE_DISPLAY_MODES1TO5
//       }
// */
//       pCONT_set->Settings.display.rotate = XdrvMailbox.payload;
//       DisplayInit(DISPLAY_INIT_MODE);
// #ifdef USE_DISPLAY_MODES1TO5
//       LogBuffer_Init();
// #endif  // USE_DISPLAY_MODES1TO5
//     }
//   }
//   ResponseCmndNumber(pCONT_set->Settings.display.rotate);
// }

// void CmndDisplayInvert(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 1)) {
//     pCONT_set->Settings.display.options.invert = XdrvMailbox.payload;
//     if (renderer) renderer->invertDisplay(pCONT_set->Settings.display.options.invert);
//   }
//   ResponseCmndNumber(pCONT_set->Settings.display.options.invert);
// }

// void CmndDisplayRefresh(void) {
//   if ((XdrvMailbox.payload >= 1) && (XdrvMailbox.payload <= 7)) {
//     pCONT_set->Settings.display.refresh = XdrvMailbox.payload;
//   }
//   ResponseCmndNumber(pCONT_set->Settings.display.refresh);
// }

// void CmndDisplayColumns(void) {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 2)) {
//     if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_MAX_COLS)) {
//       pCONT_set->Settings.display.cols[XdrvMailbox.index -1] = XdrvMailbox.payload;
// #ifdef USE_DISPLAY_MODES1TO5
//       if (1 == XdrvMailbox.index) {
//         LogBuffer_Init();
//         ScreenBuffer_ReAlloc();
//       }
// #endif  // USE_DISPLAY_MODES1TO5
//     }
//     ResponseCmndIdxNumber(pCONT_set->Settings.display.cols[XdrvMailbox.index -1]);
//   }
// }

// void CmndDisplayRows(void) {
//   if ((XdrvMailbox.payload > 0) && (XdrvMailbox.payload <= DISPLAY_MAX_ROWS)) {
//     pCONT_set->Settings.display.rows = XdrvMailbox.payload;
// #ifdef USE_DISPLAY_MODES1TO5
//     LogBuffer_Init();
//     ScreenBuffer_ReAlloc();
// #endif  // USE_DISPLAY_MODES1TO5
//   }
//   ResponseCmndNumber(pCONT_set->Settings.display.rows);
// }

// void CmndDisplayAddress(void) {
//   if ((XdrvMailbox.index > 0) && (XdrvMailbox.index <= 8)) {
//     if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 255)) {
//       pCONT_set->Settings.display.address[XdrvMailbox.index -1] = XdrvMailbox.payload;
//     }
//     ResponseCmndIdxNumber(pCONT_set->Settings.display.address[XdrvMailbox.index -1]);
//   }
// }

// void CmndDisplayBlinkrate(void) {
//   if ((XdrvMailbox.payload >= 0) && (XdrvMailbox.payload <= 3)) {
//     if (!renderer) {
//       pCONT->Tasker_Interface(FUNC_DISPLAY_BLINKRATE);
//     }
//   }
//   ResponseCmndNumber(XdrvMailbox.payload);
// }

// #ifdef USE_UFILESYS
// void CmndDisplayBatch(void) {
//   if (XdrvMailbox.data_len > 0) {
//     if (!pCONT_set->Settings.display.mode) {
//       Display_Text_From_File(XdrvMailbox.data);
//     }
//     ResponseCmndChar(XdrvMailbox.data);
//   }
// }
// #endif

void mDisplaysInterface::CmndDisplayText(const char* buffer) {
  // if (disp_device && XdrvMailbox.data_len > 0) {

// #ifndef USE_DISPLAY_MODES1TO5
//     DisplayText();
// #else
//     if(pCONT_set->Settings.display.model == 15) {
//       pCONT->Tasker_Interface(FUNC_DISPLAY_SEVENSEG_TEXT);
//     } else if (!pCONT_set->Settings.display.mode) {
#ifdef ENABLE_DISPLAY_MODE_USER_TEXT_SERIALISED
      DisplayText(buffer);
#endif // ENABLE_DISPLAY_MODE_USER_TEXT_SERIALISED
    // } else {
    //   LogBuffer_Add(XdrvMailbox.data);
    // }
// #endif  // USE_DISPLAY_MODES1TO5


    // ResponseCmndChar(XdrvMailbox.data);
  // }
}


/**
 * @brief Parsing of direct to display method version 2, using json objects to control effects
 * @param jobj The JsonParserObject from the command
 * */
/**
 * 
{
  "ClearDisplay":1,
  "DrawText":
  [
    {
      "FontSize":1,
      "Position":[0,0],
      "Text":"TextBlock0"
    },
    {
      "FontSize":1,
      "Position":[0,40],
      "Text":"TextBlock1"
    }
  ]
}
 * 
 * */
void mDisplaysInterface::CommandSet_DisplayText_Advanced_JSON(JsonParserObject jobj) {

  JsonParserArray array = jobj["DrawText"];
  JsonParserToken jtok = 0;

  for(auto& object:array)
  {
    JsonParserObject obj1 = object;

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "obj1=%d" ), obj1.size());
    
    /**
     * @note Change fontsize
     * */
    if(jtok = obj1["TextSize"])
    {
      SetTextSize(jtok.getInt());
    }

    /**
     * @note Position of XY coordinate is always given as array
     * */
    if(jtok = obj1["Position"])
    {
      JsonParserArray arr_pos = obj1["Position"];
      if(arr_pos.size() == 2)
      {
        SetCursor(arr_pos[0].getInt(), arr_pos[1].getInt());
      }
    }

    /**
     * @note This must be last, as previous formatting needs to be applied first
     * */
    if(jtok = obj1["Text"])
    {
      //Draw function
      pCONT_iDisp->renderer->println(jtok.getStr());
    }

    pCONT_iDisp->renderer->Updateframe();

  }



// }else{
//  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_LIGHT "!o2.isArray(" ));

// }






}



// /*********************************************************************************************\
//  * Currently 7-segement specific - should have been handled by (extended) DisplayText command
// \*********************************************************************************************/

// void CmndDisplayClear(void) {
//   if (!renderer)
//     pCONT->Tasker_Interface(FUNC_DISPLAY_CLEAR);
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayNumber(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(FUNC_DISPLAY_NUMBER);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayFloat(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(FUNC_DISPLAY_FLOAT);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayNumberNC(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(FUNC_DISPLAY_NUMBERNC);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayFloatNC(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(FUNC_DISPLAY_FLOATNC);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayRaw(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(FUNC_DISPLAY_RAW);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayLevel(void) {
//   bool result = false;
//   if (!renderer) {
//     result = pCONT->Tasker_Interface(FUNC_DISPLAY_LEVEL);
//   }
//   if(result) ResponseCmndNumber(XdrvMailbox.payload);
// }

// void CmndDisplaySevensegText(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(FUNC_DISPLAY_SEVENSEG_TEXT);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayTextNC(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(FUNC_DISPLAY_SEVENSEG_TEXTNC);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplaySevensegTextNC(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(FUNC_DISPLAY_SEVENSEG_TEXTNC);
//   }
//   ResponseCmndChar(XdrvMailbox.data);
// }

// void CmndDisplayScrollDelay(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(FUNC_DISPLAY_SCROLLDELAY);
//   }
//   ResponseCmndNumber(XdrvMailbox.payload);
// }

// void CmndDisplayClock(void) {
//   if (!renderer) {
//     pCONT->Tasker_Interface(FUNC_DISPLAY_CLOCK);
//   }
//   ResponseCmndNumber(XdrvMailbox.payload);
// }

// void CmndDisplayScrollText(void) {
//   bool result = false;
//   if (!renderer) {
//     result = pCONT->Tasker_Interface(FUNC_DISPLAY_SCROLLTEXT);
//   }
//   if(result) ResponseCmndChar(XdrvMailbox.data);
// }


#endif // USE_MODULE_LIGHTS_INTERFACE
