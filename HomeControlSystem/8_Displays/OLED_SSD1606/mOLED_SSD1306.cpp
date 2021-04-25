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

const char* mOLED_SSD1306::PM_MODULE_DISPLAYS_OLED_SSD1306_CTR = D_MODULE_DISPLAYS_OLED_SSD1306_CTR;
const char* mOLED_SSD1306::PM_MODULE_DISPLAYS_OLED_SSD1306_FRIENDLY_CTR = D_MODULE_DISPLAYS_OLED_SSD1306_FRIENDLY_CTR;

int8_t mOLED_SSD1306::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;

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
    case FUNC_DISPLAY_INIT_DRIVER:
      InitDriver();
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
    case FUNC_EVERY_SECOND:
      RefreshDisplay();
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


void mOLED_SSD1306::EveryLoop()
{

}


uint8_t mOLED_SSD1306::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);
  return JsonBuilderI->End();

}


uint8_t mOLED_SSD1306::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_VOLTAGE, 0);
  return JsonBuilderI->End();
    
}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mOLED_SSD1306::MQTTHandler_Init(){

  struct handler<mOLED_SSD1306>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mOLED_SSD1306::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mOLED_SSD1306::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mOLED_SSD1306::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mOLED_SSD1306::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mOLED_SSD1306::MQTTHandler_Set_TelePeriod(){

  mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mOLED_SSD1306::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, 
    EM_MODULE_DISPLAYS_OLED_SSD1306_ID, list_ptr, list_ids, sizeof(list_ptr)/sizeof(list_ptr[0]), mqtt_handler_id
  );

}

#endif


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
      pCONT_set->Settings.display.model = XDSP_02;
    }
    else if (pCONT_sup->I2cSetDevice(OLED_ADDRESS2))
    {
      pCONT_set->Settings.display.address[0] = OLED_ADDRESS2;
      pCONT_set->Settings.display.model = XDSP_02;
    }
  }

  AddLog(LOG_LEVEL_INFO, PSTR("DSP: SD1306 address[0] %d"),pCONT_set->Settings.display.address[0]);
  
  if(XDSP_02 == pCONT_set->Settings.display.model)
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

    oled1306 = new Adafruit_SSD1306(pCONT_set->Settings.display.width, pCONT_set->Settings.display.height, pCONT_sup->wire, pCONT_pins->Pin(GPIO_OLED_RESET_ID));
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

    AddLog(LOG_LEVEL_INFO, PSTR("DSP: SD1306"));
  }
}


void mOLED_SSD1306::Ssd1306PrintLog(void)
{
  pCONT_iDisp->disp_refresh--;
  if (!pCONT_iDisp->disp_refresh) {
    pCONT_iDisp->disp_refresh = pCONT_set->Settings.display.refresh;
    if (!pCONT_iDisp->disp_screen_buffer_cols) { pCONT_iDisp->DisplayAllocScreenBuffer(); }

    char* txt = pCONT_iDisp->DisplayLogBuffer('\370');
    if (txt != NULL) {
      uint8_t last_row = pCONT_set->Settings.display.rows -1;

      pCONT_iDisp->renderer->clearDisplay();
      pCONT_iDisp->renderer->setTextSize(pCONT_set->Settings.display.size);
      pCONT_iDisp->renderer->setCursor(0,0);
      for (byte i = 0; i < last_row; i++) {
        strlcpy(pCONT_iDisp->disp_screen_buffer[i], pCONT_iDisp->disp_screen_buffer[i +1], pCONT_iDisp->disp_screen_buffer_cols);
        pCONT_iDisp->renderer->println(pCONT_iDisp->disp_screen_buffer[i]);
      }
      strlcpy(pCONT_iDisp->disp_screen_buffer[last_row], txt, pCONT_iDisp->disp_screen_buffer_cols);
      pCONT_iDisp->DisplayFillScreen(last_row);

      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "[%s]"), pCONT_iDisp->disp_screen_buffer[last_row]);

      pCONT_iDisp->renderer->println(pCONT_iDisp->disp_screen_buffer[last_row]);
      pCONT_iDisp->renderer->Updateframe();
    }
  }
}


void mOLED_SSD1306::Ssd1306Time(void)
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


void mOLED_SSD1306::ShowModuleBuffer(void)
{
  char line[12];

  sprintf(module_buffer,"test");
  pCONT_iDisp->renderer->clearDisplay();
  pCONT_iDisp->renderer->setTextSize(pCONT_set->Settings.display.size);
  pCONT_iDisp->renderer->setTextFont(pCONT_set->Settings.display.font);
  pCONT_iDisp->renderer->setCursor(0, 0);
  // snprintf_P(line, sizeof(line), PSTR(" %02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"), pCONT_time->RtcTime.hour,  pCONT_time->RtcTime.minute,  pCONT_time->RtcTime.second);  // [ 12:34:56 ]
  pCONT_iDisp->renderer->println(module_buffer);
  pCONT_iDisp->renderer->println();
  
  // snprintf_P(line, sizeof(line), PSTR("%02d" D_MONTH_DAY_SEPARATOR "%02d" D_YEAR_MONTH_SEPARATOR "%04d"),  pCONT_time->RtcTime.Mday,  pCONT_time->RtcTime.month,  pCONT_time->RtcTime.year);   // [01-02-2018]
  // pCONT_iDisp->renderer->println(line);

  pCONT_iDisp->renderer->Updateframe();

}


void mOLED_SSD1306::RefreshDisplay(void)
{
  if (!pCONT_iDisp->renderer) return;
  
  switch (pCONT_set->Settings.display.mode) {
    case EM_DISPLAY_MODE_USER_TEXT_SERIALISED_ID: 
    case EM_DISPLAY_MODE_USER_TEXT_ADVANCED_JSON_ID:
      // Refresh not needed, drawn directly on execution
      break;
    case EM_DISPLAY_MODE_UTC_TIME_ID:
      Ssd1306Time();
      break;
    case EM_DISPLAY_MODE_LOCAL1_ID:
    case EM_DISPLAY_MODE_LOCAL2_ID:
    case EM_DISPLAY_MODE_MQTT1_ID:
    case EM_DISPLAY_MODE_MQTT2_ID:
      Ssd1306PrintLog();
      break;
    case EM_DISPLAY_MODE_BASIC_BUFFER_TEXT_DRAW:
      ShowModuleBuffer();
      break;
  }

}

