#include "mNextion.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION


int8_t mNextion::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();  
      break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP:
    
      if(!update_in_progress)
        EveryLoop();

    break;
    case TASK_EVERY_SECOND:
      if(!update_in_progress)
      {
        EverySecond_ActivityCheck();
      }
    break;
    case TASK_RESTART_SET_DO_FINAL_CLEANUP:      
      nextionSendCmd("page message");
      nextionSetAttr("message.main.txt", "\"System Rebooting...\"");
      nextionSetAttr("message.main.bco", NEXTION_16BIT_COLOUR_GREEN_STR);
      display->flush();
    break;
    /************
     * Network SECTION * 
    *******************/
    case TASK_WEB_ADD_HANDLER:
      WebPage_AddHandlers();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
    
      if(!update_in_progress && !tkr_sup->arduino_ota_triggered)
        parse_JSONCommand(obj);

    break;
    case TASK_DISPLAY_CONNECTION_ESTABLISHED:

      // Restore page if non-zero
      if (settings.page)
      {
        char command_ctr[16];
        snprintf(command_ctr, sizeof(command_ctr), "page %u", (unsigned)settings.page);
        nextionSendCmd(command_ctr);
      }

    break;
    case TASK_DISPLAY_CONNECTION_FAILED:
    
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    case TASK_MQTT_CONNECTED:
      Show_ConnectionWorking();
    break;
    case TASK_MQTT_DISCONNECTED:
      Show_ConnectionNotWorking();
    break;
    #endif // USE_MODULE_NETWORK_MQTT
  }

  return TASKER_RESULT__SUCCESS_ID;

}


void mNextion::Pre_Init(void)
{

  module_state.mode = ModuleStatus::Initialising;

  // Plan to make Serial the primary code, then everything here will interface into it. 
  // For now, just using locally

  uint8_t serial_port = 1; // default
  uint8_t rx_pin = 0;
  uint8_t tx_pin = 0;
  if(tkr_pins->PinUsed(GPIO_NEXTION_RX) && tkr_pins->PinUsed(GPIO_NEXTION_TX))
  {
    serial_port = 1;
    rx_pin = tkr_pins->GetPin(GPIO_NEXTION_RX);
    tx_pin = tkr_pins->GetPin(GPIO_NEXTION_TX);
  }
  else if(tkr_pins->PinUsed(GPIO_NEXTION_RX2) && tkr_pins->PinUsed(GPIO_NEXTION_TX2))
  {
    serial_port = 2;
    rx_pin = tkr_pins->GetPin(GPIO_NEXTION_RX2);
    tx_pin = tkr_pins->GetPin(GPIO_NEXTION_TX2);
  }else{
    module_state.mode = ModuleStatus::NoGPIOConfigured;
    return;
  }

  display = new HardwareSerial(serial_port);
  
  ALOG_INF(PSTR(D_LOG_NEXTION "Using GPIO%d for Nextion RX"), rx_pin);

  pinMode( rx_pin, OUTPUT); // RX - try forcing these to GPIO to stop serial comms
  pinMode( tx_pin, OUTPUT); // TX - try forcing these to GPIO to stop serial comms

  /**
   * @brief ESP32 is hanging when starting Serial 1 or 2.
   * A work around is to start at a low baud, then switch to the desired baud to avoid hanging
   **/
  display->begin(
    9600,
    SERIAL_8N1,
    rx_pin, // RX
    tx_pin  // TX
  );
  display->updateBaudRate(ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT);

  module_state.mode = ModuleStatus::Initialising; 
  
}


void mNextion::Init()
{ 


  if (!nextionConnect())
  {
    if (lcdConnected)
    {
      ALOG_INF(PSTR("HMI: LCD responding but initialization wasn't completed. Continuing program load anyway."));
      module_state.devices++;
    }
    else
    {
      ALOG_INF(PSTR("HMI: LCD not responding, continuing program load"));
      module_state.devices++;
    }
  }else{
    module_state.devices++;
  }
  
  memset(nextionSuffix,0xFF,sizeof(nextionSuffix));

  #ifdef USE_FEATURE_NEXTION__FORCE_SERIAL_BAUDRATE_FROM_DEFAULT  
  CommandSet_Baud(USE_FEATURE_NEXTION__FORCE_SERIAL_BAUDRATE_FROM_DEFAULT);
  #endif 

  #ifdef NEXTION_INIT_PANEL_COMMAND_TEMPLATE
  Template_Load_Init_Display_Command();
  #endif // NEXTION_INIT_PANEL_COMMAND_TEMPLATE

  settings.page = NEXTION_DEFAULT_PAGE_NUMBER;
  Command_SetPage(settings.page);

  
  if(module_state.devices)
  {
    module_state.mode = ModuleStatus::Running;
  }


}


void mNextion::EverySecond_ActivityCheck()
{

  ALOG_DBG(PSTR("fOpenHABDataStreamActive_last_secs = %d"), settings.timeout_check.timeout_period );

  if(settings.timeout_check.timeout_period==1)
  {
    ALOG_DBM(PSTR("No data"));
    // Show_ConnectionNotWorking();
    settings.page = -1; // unset to force update when available
  }
  else
  if(settings.timeout_check.timeout_period > 1)
  {
    settings.timeout_check.timeout_period--;
  }
  else
  {
    settings.timeout_check.timeout_period = 0; //stop
  }
    

}



void mNextion::EveryLoop()
{

  nextionHandleInput();     // Nextion serial communications loop
    
  // Check if long press threshold reached
  if(screen_press.fEnableImmediateButtonTime){
    if(mTime::TimeReachedNonReset(&screen_press.tSavedButtonONEvent,LONG_PRESS_DURATION)){
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "LONG_PRESS_DURATION reached"));
      screen_press.fEnableImmediateButtonTime=false;
      fEnableIgnoreNextOffEvent = true;
      MQTTSend_LongPressEvent();
      MQTTSend_PressEvent();
    }
  }

}



/******************************************************************************************************************
 * SECTION: MQTT
*******************************************************************************************************************/

void mNextion::MQTTSend_PressEvent(){

  if(!mTime::TimeReached(&tSaved_MQTTSend_PressEvent,200)){
    // Debounce and only send once per event (ie ignore release trigger following immediate trigger)
    return;
  }

  char buffer[50];
  
  screen_press.page  = incoming_frame.data[1];
  screen_press.event = incoming_frame.data[2];
  uint32_t tSavedTimeSincePressOn = millis() - screen_press.tSavedButtonONEvent;

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " \"p[%d].b[%d]\"=%s elapsed=%d of %dthreshold"), screen_press.page, screen_press.event, (tSavedTimeSincePressOn<LONG_PRESS_DURATION) ? "SHORT_PRESS" : "LONG_PRESS", tSavedTimeSincePressOn, LONG_PRESS_DURATION);
  
  JBI->Start();  
    JBI->Add("Page", screen_press.page);
    JBI->Add("ID", screen_press.event);
    JBI->Addf("Event", "p[%d].b[%d]",screen_press.page,screen_press.event);    
    JBI->Add("PressType", (tSavedTimeSincePressOn<LONG_PRESS_DURATION) ? "SHORT_PRESS" : "LONG_PRESS");
    JBI->Add("Duration", tSavedTimeSincePressOn);
    JBI->Add("ObjectName", GetObjectName_FromID(screen_press.event, buffer, sizeof(buffer)));
  JBI->End();

  tSaved_MQTTSend_PressEvent = millis();

  tkr_mqtt->Publish("status/nextion/event/press",JBI->GetBufferPtr(),false);

}


void mNextion::MQTTSend_LongPressEvent(){

  screen_press.page = incoming_frame.data[1];
  screen_press.event = incoming_frame.data[2];

  char buffer[50];

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " \"p[%d].b[%d]\"=%s"), screen_press.page, screen_press.event, "LONG_PRESS");

  JBI->Start();
    JBI->Add("Page", screen_press.page);
    JBI->Add("ID", screen_press.event);
    JBI->Addf("Event", "p[%d].b[%d]\0",screen_press.page,screen_press.event);
    JBI->Add("PressType", "LONG_PRESS");
    JBI->Add("DurationThreshold", LONG_PRESS_DURATION);
    JBI->Add("ObjectName", GetObjectName_FromID(screen_press.event, buffer, sizeof(buffer)));
  JBI->End();

  tkr_mqtt->Publish("status/nextion/event",JBI->GetBufferPtr(),0);
  tkr_mqtt->Publish("status/nextion/event/start",JBI->GetBufferPtr(),0);

}



/******************************************************************************************************************
 *SECTION: ConstructJson
*******************************************************************************************************************/


uint8_t mNextion::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{

  JBI->Start();
    JBI->Add("test", 1);
    JBI->Add("lcdConnected", lcdConnected);
  return JBI->End();
}


uint8_t mNextion::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{
  
  JBI->Start();
    JBI->Add("test", 1);
  return JBI->End();

}


/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mNextion::MQTTHandler_Init()
{

  struct handler<mNextion>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mNextion::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mNextion::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mNextion::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT


#endif