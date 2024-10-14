/*
  xdsp_02_ssd1306.ino - Display Oled SSD1306 support for Tasmota

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
#include "mOLED_SSD1306.h"

#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306

int8_t mOLED_SSD1306::Tasker(uint8_t function, JsonParserObject obj)
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

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }
  if(!pCONT_iDisp->renderer) { return FUNCTION_RESULT_ERROR_POINTER_INVALID_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      EverySecond();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


void mOLED_SSD1306::parse_JSONCommand(JsonParserObject obj)
{

}


void mOLED_SSD1306::Pre_Init(void)
{
  if (pCONT_sup->I2cEnabled(XI2C_04))
  { 
    settings.fEnableSensor = true;
  }
}


void mOLED_SSD1306::Init(void)
{
  
}


void mOLED_SSD1306::EverySecond(void)
{
  /**
   * RefreshDisplay
   * */
  switch (pCONT_set->Settings.display.mode) {
    default:
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
    case EM_DISPLAY_MODE_UTC_TIME_ID:
      ShowUTCTime();
      break;
  }

}


void mOLED_SSD1306::InitDriver(void)
{
  if (!pCONT_set->i2c_enabled) 
  {
    return; 
  }

  if (!pCONT_set->Settings.display.model)
  {
    if (pCONT_sup->I2cSetDevice(OLED_ADDRESS1))
    {
      pCONT_set->Settings.display.address[0] = OLED_ADDRESS1;
      pCONT_set->Settings.display.model = D_GROUP_MODULE_DISPLAYS_OLED_SSD1306_ID;
    }
    else if (pCONT_sup->I2cSetDevice(OLED_ADDRESS2))
    {
      pCONT_set->Settings.display.address[0] = OLED_ADDRESS2;
      pCONT_set->Settings.display.model = D_GROUP_MODULE_DISPLAYS_OLED_SSD1306_ID;
    }
  }

  ALOG_INF(PSTR("DSP: SD1306 address[0] %d"),pCONT_set->Settings.display.address[0]);
  
  if(pCONT_set->Settings.display.model == D_GROUP_MODULE_DISPLAYS_OLED_SSD1306_ID)
  {
    pCONT_sup->I2cSetActiveFound(pCONT_set->Settings.display.address[0], "SSD1306");

    if((pCONT_set->Settings.display.width != 64) && (pCONT_set->Settings.display.width != 96) && (pCONT_set->Settings.display.width != 128))
    {
      pCONT_set->Settings.display.width = 128;
    }
    if((pCONT_set->Settings.display.height != 16) && (pCONT_set->Settings.display.height != 32) && (pCONT_set->Settings.display.height != 48) && (pCONT_set->Settings.display.height != 64))
    {
      pCONT_set->Settings.display.height = 64;
    }

    oled1306 = new Adafruit_SSD1306(pCONT_set->Settings.display.width, pCONT_set->Settings.display.height, pCONT_i2c->wire, pCONT_pins->Pin(GPIO_OLED_RESET_ID));
    oled1306->begin(SSD1306_SWITCHCAPVCC, pCONT_set->Settings.display.address[0], pCONT_pins->Pin(GPIO_OLED_RESET_ID) >= 0);
    pCONT_iDisp->renderer = oled1306;
    pCONT_iDisp->renderer->DisplayInit(pCONT_iDisp->DISPLAY_INIT_MODE, pCONT_set->Settings.display.size, pCONT_set->Settings.display.rotate, pCONT_set->Settings.display.font);
    pCONT_iDisp->renderer->setTextColor(1,0);

    #ifdef SHOW_SPLASH
    pCONT_iDisp->renderer->setTextFont(0);
    pCONT_iDisp->renderer->setTextSize(2);
    pCONT_iDisp->renderer->setCursor(20,20);
    pCONT_iDisp->renderer->println(F("SSD1306"));
    pCONT_iDisp->renderer->Updateframe();
    pCONT_iDisp->renderer->DisplayOnff(1);
    #endif

    ALOG_INF(PSTR("DSP: SD1306"));
  }
}


/**
 * @brief Will take from log_buffer and fill screen_buffer, shifting the rows up
 * */
void mOLED_SSD1306::ShowScrollingLog(void)
{
  
  // If no columns have been init, then first allocate memory
  if (!pCONT_iDisp->screen_buffer.cols) { pCONT_iDisp->ScreenBuffer_Alloc(); }

  // Get pointer to row
  char* txt = pCONT_iDisp->LogBuffer_GetRowPointer('\370');
  if (txt != NULL) {
    // Last row is row_size - 1 for indexing
    uint8_t last_row = pCONT_set->Settings.display.rows -1;

    // Start by clearing the display
    pCONT_iDisp->renderer->clearDisplay();
    pCONT_iDisp->renderer->setTextSize(pCONT_set->Settings.display.size);
    pCONT_iDisp->renderer->setCursor(0,0);

    // Shift the logs by moving the rows from next into current and display this
    for (byte i = 0; i < last_row; i++) {
      strlcpy(pCONT_iDisp->screen_buffer.ptr[i], pCONT_iDisp->screen_buffer.ptr[i +1], pCONT_iDisp->screen_buffer.cols);
      pCONT_iDisp->renderer->println(pCONT_iDisp->screen_buffer.ptr[i]);
    }
    // Add new row
    strlcpy(pCONT_iDisp->screen_buffer.ptr[last_row], txt, pCONT_iDisp->screen_buffer.cols);
    // This is only done here, as the copied rows will have it done at this line when first commited to the screen_buffer
    pCONT_iDisp->ScreenBuffer_SetUnusedRowCharsToSpaceChar(last_row); 

    ALOG_DBG(PSTR(D_LOG_DEBUG "[%s]"), pCONT_iDisp->screen_buffer.ptr[last_row]);

    // Print last row
    pCONT_iDisp->renderer->println(pCONT_iDisp->screen_buffer.ptr[last_row]);

    // Commit display refresh
    pCONT_iDisp->renderer->Updateframe();
  }

}

/**
 * @brief Unlike AppendingLog method, this will only commit all of log_buffer into screen_buffer and display
 * */
void mOLED_SSD1306::ShowStaticLog(void)
{
  
  // If no columns have been init, then first allocate memory
  if (!pCONT_iDisp->screen_buffer.cols) { pCONT_iDisp->ScreenBuffer_Alloc(); }

  // Start by clearing the display
  pCONT_iDisp->renderer->clearDisplay();
  pCONT_iDisp->renderer->setTextSize(pCONT_set->Settings.display.size);
  pCONT_iDisp->renderer->setCursor(0,0);

  // Copy log_buffer contents into screen_buffer
  for(int row_index=0; row_index<pCONT_set->Settings.display.rows; row_index++)
  {
    // Get log_buffer by row
    char* row_ptr = pCONT_iDisp->LogBuffer_GetRowPointerByRowIndex(row_index);
    // Move to screen_buffer
    strlcpy(pCONT_iDisp->screen_buffer.ptr[row_index], row_ptr, pCONT_iDisp->screen_buffer.cols);
    // Fill remaining spaces
    pCONT_iDisp->ScreenBuffer_SetUnusedRowCharsToSpaceChar(row_index); 
    // Write row to display
    pCONT_iDisp->renderer->println(pCONT_iDisp->screen_buffer.ptr[row_index]);
  }

  // Commit display refresh
  pCONT_iDisp->renderer->Updateframe();

}


void mOLED_SSD1306::ShowUTCTime(void)
{

  char line[12];
  pCONT_iDisp->renderer->clearDisplay();
  pCONT_iDisp->renderer->setTextSize(pCONT_set->Settings.display.size);
  pCONT_iDisp->renderer->setTextFont(pCONT_set->Settings.display.font);
  pCONT_iDisp->renderer->setCursor(0, 0);
  snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), pCONT_time->RtcTime.hour,  pCONT_time->RtcTime.minute,  pCONT_time->RtcTime.second);  // [ 12:34:56 ]
  pCONT_iDisp->renderer->println(line);
  pCONT_iDisp->renderer->println();
  snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"),  pCONT_time->RtcTime.Mday,  pCONT_time->RtcTime.month,  pCONT_time->RtcTime.year);   // [01-02-2018]
  pCONT_iDisp->renderer->println(line);
  pCONT_iDisp->renderer->Updateframe();

}



uint8_t mOLED_SSD1306::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

  #ifdef USE_MODULE_NETWORK_MQTT
void mOLED_SSD1306::MQTTHandler_Init(){

  struct handler<mOLED_SSD1306>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mOLED_SSD1306::ConstructJSON_Settings;
  
} 


// Can these be replaced by mqtt shared function that gets the mqtt from the module
void mOLED_SSD1306::MQTTHandler_RefreshAll(){

  mqtthandler_settings.flags.SendNow = true;

} 


void mOLED_SSD1306::MQTTHandler_Rate(){

  mqtthandler_settings.tRateSecs = pCONT_mqtt->dt.teleperiod_secs;

} //end "MQTTHandler_Rate"


void mOLED_SSD1306::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, 
    EM_MODULE_DISPLAYS_OLED_SSD1306_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id
  );

}

#endif // USE_MODULE_NETWORK_MQTT

#endif
