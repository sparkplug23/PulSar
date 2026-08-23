/*
  xdsp_02_ssd1306.ino - Display Oled SH1106 support for Tasmota

  Copyright (C) 2021  Theo Arends and Adafruit

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
#include "mOLED_SH1106.h"

#ifdef USE_MODULE_DISPLAYS_OLED_SH1106

int8_t mOLED_SH1106::Tasker(uint8_t function, JsonParserObject obj)
{
  
  int8_t function_result = 0;

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
    case TASK_DISPLAY_INIT_DRIVER:
      InitDriver();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }
  if(!tkr_iDisp->renderer) { return TASKER_RESULT__ERROR_POINTER_INVALID_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    case TASK_DISPLAY_REFRESH_SHOW_ID:
      RefreshDisplay();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
     /************
     * TELEMETRY SECTION * 
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_tele->Telemetry_Rate(telemetry_list);
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY__SENDER_MQTT:
      tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_SERIAL
    case TASK_SERIAL_TELEMETRY:
      tkr_serial->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_TELEMETRY:
      tkr_web->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
  }
  
  return function_result;

} // END function


void mOLED_SH1106::Pre_Init(void)
{
  if (tkr_i2c->I2cEnabled(XI2C_04))
  { 
    module_state.mode = ModuleStatus::Running;
  }
}


void mOLED_SH1106::Init(void)
{
  
}


void mOLED_SH1106::EverySecond(void)
{
  /**
   * RefreshDisplay
   * */
  RefreshDisplay();
}

// void mOLED_SH1106::EveryLoop()
// {
//   if(mTime::TimeReached(&tSaved))
// RefreshDisplay();

// }



void mOLED_SH1106::RefreshDisplay()
{

  switch (tkr_iDisp->display.mode) {
    default:
    case EM_DISPLAY_MODE_UTC_TIME_ID:
      ShowUTCTime();
    break;
    case EM_DISPLAY_MODE_LOG_SCROLLING_ID:
      ShowScrollingLog();
    break;
    case EM_DISPLAY_MODE_LOG_STATIC_ID:
      ShowStaticLog();
    break;
    case EM_DISPLAY_MODE_USER_TEXT_SERIALISED_ID: 
    case EM_DISPLAY_MODE_USER_TEXT_ADVANCED_ID:
      // Refresh not needed, drawn directly on execution
    break;
  }


}


void mOLED_SH1106::InitDriver(void)
{
  if (!tkr_set->runtime.i2c_enabled) 
  {
    return; 
  }

  if (!tkr_iDisp->display.model)
  {
    if (tkr_i2c->I2cSetDevice(OLED_ADDRESS1))
    {
      tkr_iDisp->display.address[0] = OLED_ADDRESS1;
      tkr_iDisp->display.model = D_GROUP_MODULE_DISPLAYS_OLED_SH1106_ID;
    }
    else if (tkr_i2c->I2cSetDevice(OLED_ADDRESS2))
    {
      tkr_iDisp->display.address[0] = OLED_ADDRESS2;
      tkr_iDisp->display.model = D_GROUP_MODULE_DISPLAYS_OLED_SH1106_ID;
    }
  }else{
    ALOG_INF(PSTR(D_LOG_SH1106 "Model already set %d"), tkr_iDisp->display.model);
  }

  ALOG_INF(PSTR("DSP: SH1106 address[0] %d"),tkr_iDisp->display.address[0]);
  ALOG_INF(PSTR("DSP: model %d"),tkr_iDisp->display.model);
  
  if(tkr_iDisp->display.model == D_GROUP_MODULE_DISPLAYS_OLED_SH1106_ID)
  {
    tkr_i2c->I2cSetActiveFound(tkr_iDisp->display.address[0], "SH1106");

    if(
      (tkr_iDisp->display.width != 64) && 
      (tkr_iDisp->display.width != 96) && 
      (tkr_iDisp->display.width != 128))
    {
      tkr_iDisp->display.width = 128;
    }
    if(
      (tkr_iDisp->display.height != 16) && 
      (tkr_iDisp->display.height != 32) && 
      (tkr_iDisp->display.height != 48) && 
      (tkr_iDisp->display.height != 64)
    ){
      tkr_iDisp->display.height = 64;
    }

    oled1106 = new Adafruit_SH1106(tkr_iDisp->display.width, tkr_iDisp->display.height, tkr_i2c->wire);
    oled1106->begin(SH1106_SWITCHCAPVCC, tkr_iDisp->display.address[0], tkr_pins->Pin(GPIO_OLED_RESET) >= 0);
    tkr_iDisp->renderer = oled1106;
    tkr_iDisp->renderer->DisplayInit(tkr_iDisp->DISPLAY_INIT_MODE, tkr_iDisp->display.size, tkr_iDisp->display.rotate, tkr_iDisp->display.font);
    tkr_iDisp->renderer->setTextColor(1,0);

    #ifdef SHOW_SPLASH
    tkr_iDisp->renderer->setTextFont(0);
    tkr_iDisp->renderer->setTextSize(2);
    tkr_iDisp->renderer->setCursor(20,20);
    tkr_iDisp->renderer->println(F("SH1106a"));
    tkr_iDisp->renderer->Updateframe();
    tkr_iDisp->renderer->DisplayOnff(true);
    #endif



    ALOG_INF(PSTR("DSP: SH1106"));
  }

    tkr_iDisp->display.invert = 0;
    #ifdef ENABLE_DEVFEATURE_DISPLAY__INVERT
    tkr_iDisp->display.invert = 1;
    #endif

    // tkr_iDisp->renderer->invertDisplay(tkr_iDisp->display.invert);
}


/*********************************************************************************************/
#ifdef USE_DISPLAY_MODES1TO5

/**
 * @brief Will take from log_buffer and fill screen_buffer, shifting the rows up
 * */
void mOLED_SH1106::ShowScrollingLog(void)
{
  
  tkr_iDisp->disp_refresh--;
  if (!tkr_iDisp->disp_refresh) {
    tkr_iDisp->disp_refresh = tkr_iDisp->display.refresh;

    // If no columns have been init, then first allocate memory
    if (!tkr_iDisp->screen_buffer.cols) { tkr_iDisp->ScreenBuffer_Alloc(); }

    // Get pointer to row
    char* txt = tkr_iDisp->LogBuffer_GetRowPointer('\370');
    if (txt != NULL) {
      // Last row is row_size - 1 for indexing
      uint8_t last_row = tkr_iDisp->display.rows -1;

      // Start by clearing the display
      tkr_iDisp->renderer->clearDisplay();
      tkr_iDisp->renderer->setTextSize(tkr_iDisp->display.size);
      tkr_iDisp->renderer->setCursor(0,0);

      // Shift the logs by moving the rows from next into current and display this
      for (byte i = 0; i < last_row; i++) {
        strlcpy(tkr_iDisp->screen_buffer.ptr[i], tkr_iDisp->screen_buffer.ptr[i +1], tkr_iDisp->screen_buffer.cols);
        tkr_iDisp->renderer->println(tkr_iDisp->screen_buffer.ptr[i]);
      }
      // Add new row
      strlcpy(tkr_iDisp->screen_buffer.ptr[last_row], txt, tkr_iDisp->screen_buffer.cols);
      // This is only done here, as the copied rows will have it done at this line when first commited to the screen_buffer
      tkr_iDisp->ScreenBuffer_SetUnusedRowCharsToSpaceChar(last_row); 

      ALOG_DBG(PSTR(D_LOG_DEBUG "[%s]"), tkr_iDisp->screen_buffer.ptr[last_row]);

      // Print last row
      tkr_iDisp->renderer->println(tkr_iDisp->screen_buffer.ptr[last_row]);

      // Commit display refresh
      tkr_iDisp->renderer->Updateframe();
    }
  }
}


/**
 * @brief Unlike AppendingLog method, this will only commit all of log_buffer into screen_buffer and display
 * */
void mOLED_SH1106::ShowStaticLog(void)
{
  
  // If no columns have been init, then first allocate memory
  if (!tkr_iDisp->screen_buffer.cols) { tkr_iDisp->ScreenBuffer_Alloc(); }

  // Start by clearing the display
  tkr_iDisp->renderer->clearDisplay();
  tkr_iDisp->renderer->setTextSize(tkr_iDisp->display.size);
  tkr_iDisp->renderer->setCursor(0,0);

  // Copy log_buffer contents into screen_buffer
  for(int row_index=0; row_index<tkr_iDisp->display.rows; row_index++)
  {
    // Get log_buffer by row
    char* row_ptr = tkr_iDisp->LogBuffer_GetRowPointerByRowIndex(row_index);
    // Move to screen_buffer
    strlcpy(tkr_iDisp->screen_buffer.ptr[row_index], row_ptr, tkr_iDisp->screen_buffer.cols);
    // Fill remaining spaces
    tkr_iDisp->ScreenBuffer_SetUnusedRowCharsToSpaceChar(row_index); 
    // Write row to display
    tkr_iDisp->renderer->println(tkr_iDisp->screen_buffer.ptr[row_index]);
  }

  // Commit display refresh
  tkr_iDisp->renderer->Updateframe();

}


void mOLED_SH1106::ShowUTCTime(void)
{

  char line[12];
  tkr_iDisp->renderer->clearDisplay();
  tkr_iDisp->renderer->setTextSize(tkr_iDisp->display.size);
  tkr_iDisp->renderer->setTextFont(tkr_iDisp->display.font);
  tkr_iDisp->renderer->setCursor(0, 0);
  // snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), tkr_time->RtcTime.hour,  tkr_time->RtcTime.minute,  tkr_time->RtcTime.second);  // [ 12:34:56 ]
  tkr_iDisp->renderer->println( tkr_time->GetTime().c_str() );
  char buffer[40];
  tkr_time->GetUptime(buffer,sizeof(buffer));
  tkr_iDisp->renderer->println(buffer);
  // snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"),  tkr_time->RtcTime.Mday,  tkr_time->RtcTime.month,  tkr_time->RtcTime.year);   // [01-02-2018]
  // tkr_iDisp->renderer->println(line);
  tkr_iDisp->renderer->Updateframe();

}


#endif  // USE_DISPLAY_MODES1TO5






/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mOLED_SH1106::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
    
}

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

  
uint8_t mOLED_SH1106::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
  
  return JBI->End();

}


uint8_t mOLED_SH1106::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();

    // JBI->Object_Start(D_RFRECEIVED);
    
    JBI->Object_End();
  
  return JBI->End();

}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mOLED_SH1106::Telemetry_Init()
{

  struct telemetry_handler<mOLED_SH1106>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mOLED_SH1106::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_IFCHANGED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mOLED_SH1106::ConstructJSON_State;
  telemetry_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT






#endif // USE_MODULE_DISPLAYS_OLED_SH1106

