
#include "mNextionPanel.h"

/**
 * @brief 
 * commands held in esp 
 * 
 * auto timeout for setting the display to sleep (off or else dimmer brightness level)
 * 
 * What is the difference between openHASP and HASPone~
HASPone uses a d1-mini ESP8266 connected to a Nextion/TJC smart display via the serial port. HASPone is the go-to firmware for using a Nextion/TJC screen in your Home Automation setup.

openHASP does not support Nextion/TJC displays because it needs to be able to drive the display directly. It was created specifically to eliminate the proprietary hardware and take control of the screen.

So openHASP is a hard fork of the original HASwitchPlate project to run on open hardware.

So I will need to create a nextion class (HASPone) and then another for direct drive stuff (openHASP)

 * 
 */

#ifdef USE_MODULE_DISPLAYS_NEXTION

const char* mNextionPanel::PM_MODULE_DISPLAYS_NEXTION_CTR = D_MODULE_DISPLAYS_NEXTION_CTR;
const char* mNextionPanel::PM_MODULE_DISPLAYS_NEXTION_FRIENDLY_CTR = D_MODULE_DISPLAYS_NEXTION_FRIENDLY_CTR;

#warning "Old code to be deleted, build with version 2"

int8_t mNextionPanel::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();  
      break;
    case FUNC_INIT:
      Init();
      break;
  }

  // if(!settings.flags.EnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID;}

  switch(function){
    /************
     * SETTINGS SECTION * 
    *******************/
    // case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
    //   Settings_Load();
    // break;
    // case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
    //   Settings_Save();
    // break;
    // case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
    // case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
    //   Settings_Default();
    // break;
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
      EveryLoop();
    break;
    case FUNC_EVERY_SECOND:
      // EverySecond_SendScreenInfo();
      // EverySecond_FlashScreen();

      EverySecond_ActivityCheck();

    break;
    case FUNC_EVERY_MINUTE:
      // EverySecond_SendScreenInfo();
    break;
    case FUNC_EVERY_HOUR:
      Command_SetPage(settings.page);   //temp fix
    break;
    /************
     * Network SECTION * 
    *******************/
    case FUNC_WIFI_CONNECTED:
      // wifiConnected();
      // Show_ConnectionWorking();
    break;
    case FUNC_WIFI_DISCONNECTED:
      // wifiDisconnected();
      // Show_ConnectionNotWorking();
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
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      mqttConnected();
      Show_ConnectionWorking();
    break;
    case FUNC_MQTT_DISCONNECTED:
      mqttDisconnected();
      Show_ConnectionNotWorking();
    break;
    #endif
  }

  /************
   * WEBPAGE SECTION * 
  *******************/
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  //return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

}


/**
 * @brief Using a template for "working" and another for "not working"
 * hardcoding for now
 **/
void mNextionPanel::Show_ConnectionWorking()
{

// char* command = "{}";

#ifdef ENABLE_DEVFEATURE_NEXTION_DISPLAY

  nextionSendCmd("page 2");
  nextionSetAttr("p[2].b[1].txt", "\"Computer connected!\"");
  delay(1000);
  nextionSendCmd("page 1");
  nextionSetAttr("p[1].b[1].txt", "\"Computer connected!\"");

#endif

}


void mNextionPanel::Show_ConnectionNotWorking()
{

#ifdef ENABLE_DEVFEATURE_NEXTION_DISPLAY

  // nextionSendCmd("page 2");
  // nextionSetAttr("p[2].b[1].txt", "\"Show_ConnectionNotWorking\"");

#endif



  
}



void mNextionPanel::Pre_Init(void){
 
  // if (mcl->mset->pin[GPIO_SR04_TRIG] < 99) {  // not set when 255
  //   pin_rx = mcl->mset->pin[GPIO_SR04_TRIG];
  // }

  // if (mcl->mset->pin[GPIO_SR04_ECHO] < 99) {  // not set when 255
  //   pin_tx = mcl->mset->pin[GPIO_SR04_ECHO];
  // }

  // if((pin_rx>=0)&&(pin_tx>=0)){
  //   status_enabled = true;
  // }else{
  //   AddLog(LOG_LEVEL_ERROR,PSTR(D_LOG_ULTRASONIC "Pin Invalid %d %d"),pin_tx,pin_rx);
  //   status_enabled = false;
  // }

  #ifdef USE_HARDWARE_SERIAL_TEMP
    SERIAL_NEXTION_RX.begin(NEXTION_BAUD);
  #endif


  // Check if pins are hardware serial
  // if(true){
    // swSer = new SoftwareSerial(14,12);//D5,D6);//R-T, false, 256);
    // swSer->begin(NEXTION_BAUD);
  // }
  
  
}


/**
 * @brief 
 * Create a init command for the nextion panels, that can be stored in progmem
 * 
 */



void mNextionPanel::Init()
{ 

/**
 * @brief 
 * Create a init command for the nextion panels, that can be stored in progmem
 * 
 */
  // char buffer_init[] = {
  //   ""


  // };
  
  // init variables
  memset(nextionSuffix,0xFF,sizeof(nextionSuffix));
  memcpy(lcdVersionQuery,"p[0].b[2].val",sizeof("p[0].b[2].val"));

// nextionSendCmd("page 4");

  // needs non blocking
  while(!lcdConnected && (millis() < 5000)){ // Wait up to 5 seconds for serial input from LCD
    nextionHandleInput();
  }

  // Serial.println("mNextionPanel::nextionHandleInput done");
  // Serial.flush();
  // delay(5000);
  if(lcdConnected){
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI: LCD responding, continuing program load"));
    nextionSendCmd("connect");
  }
  else{
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI: LCD not responding, continuing program load"));
  }

  // Configure some items
  //nextionSetAttr("p[0].b[3].txt", "\"http://" + WiFi.localIP().toString() + "\""); // QR code show wifi connect


  memset(flash_message.message,0,sizeof(flash_message.message));
  sprintf(flash_message.message,"%s","Awaitng a message to show");

  char page_default_command[10];
  sprintf(page_default_command,"page %d",NEXTION_DEFAULT_PAGE_NUMBER);


  #ifdef NEXTION_INIT_PANEL_COMMAND_TEMPLATE
    Template_Load_Init_Display_Command();
  #endif // NEXTION_INIT_PANEL_COMMAND_TEMPLATE

  /**
   * @brief 
   * Boot message
   * 
   */
  #ifdef TEMPLATE_DEFINED_BOOT_MESSAGE
  // Command_SetPage(settings.page);




  #endif // TEMPLATE_DEFINED_BOOT_MESSAGE



  /**
   * @brief 
   * Runtime page
   * 
   */

  settings.page = NEXTION_DEFAULT_PAGE_NUMBER;

  settings.flags.EnableModule = true;

  //nextionSendCmd(page_default_command); //set page 1 as default 
  //   //nextionSendCmd("page 1"); //set page 1 as default 

  Command_SetPage(settings.page);


}


void mNextionPanel::HueToRgb(uint16_t hue, float* r, float* g, float* b)
{
    float h = hue/360.0f;
    float s = 1.0f;
    float v = 1.0f;

    if (s == 0.0f)
    {
        *r = *g = *b = v; // achromatic or black
    }
    else
    {
        if (h < 0.0f)
        {
            h += 1.0f;
        }
        else if (h >= 1.0f)
        {
            h -= 1.0f;
        }
        h *= 6.0f;
        int i = (int)h;
        float f = h - i;
        float q = v * (1.0f - s * f);
        float p = v * (1.0f - s);
        float t = v * (1.0f - s * (1.0f - f));
        switch (i)
        {
        case 0:
            *r = v;
            *g = t;
            *b = p;
            break;
        case 1:
            *r = q;
            *g = v;
            *b = p;
            break;
        case 2:
            *r = p;
            *g = v;
            *b = t;
            break;
        case 3:
            *r = p;
            *g = q;
            *b = v;
            break;
        case 4:
            *r = t;
            *g = p;
            *b = v;
            break;
        default:
            *r = v;
            *g = p;
            *b = q;
            break;
        }
    }

    *r *= 255;
    *g *= 255;
    *b *= 255;

}



#ifdef NEXTION_INIT_PANEL_COMMAND_TEMPLATE

void mNextionPanel::Template_Load_Init_Display_Command(){

  #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
  AddLog(LOG_LEVEL_DEBUG, PSTR("mInterfaceLight::Template_Load()"));
  #endif

  #ifdef USE_LIGHTING_TEMPLATE
  // load from progmem into local
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,LIGHTING_TEMPLATE,sizeof(LIGHTING_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);

  // AddLog(LOG_LEVEL_TEST, PSTR("mInterfaceLight::Template_Load SettingsMerge\n\r\n\r\n\r\n\r"
  //     //Test data
  //     "%d:%d:%d:%d"
  //   ),
  //     //Testdata
  //      pCONT_set->Settings.animation_settings.xmas_controller_params[0]
  //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[1]
  //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[2]
  //     ,pCONT_set->Settings.animation_settings.xmas_controller_params[3]
  // );

  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_DEBUG, PSTR("LIGHTING_TEMPLATE Load"));// " READ = \"%s\""), data_buffer.payload.ctr);
  ALOG_DBM( PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), data_buffer.payload.ctr);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

  pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
  #endif // USE_LIGHTING_TEMPLATE

}

#endif // NEXTION_INIT_PANEL_COMMAND_TEMPLATE


void mNextionPanel::EverySecond_ActivityCheck()
{

  //  if(fOpenHABDataStreamActive_last_secs){ //only if positive
  //     if(fOpenHABDataStreamActive_last_secs++>OPENHAB_DATA_STREAM_ACTIVITY_TIMEOUT_SECS){
  //       fOpenHABDataStreamActive_last_secs = -1;
  //       fOpenHABDataStreamActive = false;
  //     }
  //   }
    

}



void mNextionPanel::EveryLoop()
{

  if (nextionHandleInput()){ // Process user input from HMI
    nextionProcessInput();
  }

  if ((lcdVersion < 1) && (millis() <= (nextionRetryMax * nextionCheckInterval)))
  { // Attempt to connect to LCD panel to collect model and version info during startup
    nextionConnect();
  }
    
  if ((millis() > (nextionRetryMax * nextionCheckInterval)) && !startupCompleteFlag)
  { // We still don't have LCD info so go ahead and run the rest of the checks once at startup anyway
    updateCheck();
    startupCompleteFlag = true;
  }

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



/***
 * Add subscribes for new connection to mqtt
 * Show update on panel
 * */
void mNextionPanel::mqttConnected()
{ // MQTT connection and subscriptions

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_LOG "mNextionPanel::mqttConnected"));

  // Show connection success
  //nextionSendCmd("page 0");

  // char display_ctr[30];memset(display_ctr,0,sizeof(display_ctr));
  // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connected\\r%s",WiFi.localIP().toString(),"192.168.1.65");

  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected %s"),display_ctr);    

  //nextionSetAttr("p[0].b[1].txt", String(display_ctr));

  // hide QR code
  // nextionSendCmd("vis 3,0");

}


void mNextionPanel::mqttDisconnected(){
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected"));
  
  // char display_ctr[120];memset(display_ctr,0,sizeof(display_ctr));
  // sprintf(display_ctr,"\"WiFi Connected\\r%s\\rMQTT Connect to%s\\rFAILED rc=%s\"",WiFi.localIP().toString(),"192.168.1.65",pCONT_mqtt->pubsub->stateCtr());

  // QR code show wifi connect
  // nextionSetAttr("p[0].b[3].txt", "\"http://" + WiFi.localIP().toString() + "\"");
  // // show item
  // nextionSendCmd("vis 3,1");
  
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "mNextionPanel::mqttDisconnected %s"),display_ctr);    

  //nextionSetAttr("p[0].b[1].txt", String(display_ctr));

}


void mNextionPanel::MQTTSend_PressEvent(){

  if(!mTime::TimeReached(&tSaved_MQTTSend_PressEvent,200)){
    // Debounce and only send once per event (ie ignore release trigger following immediate trigger)
    return;
  }
  
  screen_press.page = nextionReturnBuffer[1];
  screen_press.event = nextionReturnBuffer[2];
  uint32_t tSavedTimeSincePressOn = abs(millis() - screen_press.tSavedButtonONEvent);

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " \"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,"LONG_PRESS");
  
  JsonBuilderI->Start();

    char event_ctr[20]; memset(event_ctr,0,sizeof(event_ctr));
    sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);
    JsonBuilderI->Add("event", event_ctr);
    
    JsonBuilderI->Add("value", (tSavedTimeSincePressOn<LONG_PRESS_DURATION) ? "SHORT_PRESS" : "LONG_PRESS");
    JsonBuilderI->Add("duration", tSavedTimeSincePressOn);
  JsonBuilderI->End();


  tSaved_MQTTSend_PressEvent = millis();

  pCONT_mqtt->ppublish("status/nextion/event/press",JsonBuilderI->GetBufferPtr(),false);

}

void mNextionPanel::MQTTSend_LongPressEvent(){

  screen_press.page = nextionReturnBuffer[1];
  screen_press.event = nextionReturnBuffer[2];

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " \"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,"LONG_PRESS");

  char event_ctr[20];
  memset(event_ctr,0,sizeof(event_ctr));
  sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);

  JsonBuilderI->Start();
    JsonBuilderI->Add("event", event_ctr);
    JsonBuilderI->Add("value", "LONG_PRESS");
    JsonBuilderI->Add("duration_threshold", LONG_PRESS_DURATION);
  JsonBuilderI->End();

  pCONT_mqtt->ppublish("status/nextion/event",JsonBuilderI->GetBufferPtr(),0);
  pCONT_mqtt->ppublish("status/nextion/event/start",JsonBuilderI->GetBufferPtr(),0);

}


/*********************************************************************************************************************************************
******** Data Builders (JSON + Pretty) **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

uint8_t mNextionPanel::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add("test", 1);
    JBI->Add("lcdConnected", lcdConnected);
  JsonBuilderI->End();

}

uint8_t mNextionPanel::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();
    JsonBuilderI->Add("test", 1);
  JsonBuilderI->End();

}


/**
 * @brief 
 * 
 * 
 
 {
  "commands": [
    "page 5",
    "p[5].b[7].txt=\"Heating is working\"",
    "p[5].b[7].pco=2047",
    "p[5].b[7].bco=0",
    "p[5].b[11].txt=\"12.3\"",
    "p[5].b[11].pco=65500",
    "p[5].b[11].bco=0"
  ]
}

{
  "commands": [
    "page 1",
    "p[9].b[1].txt=\"hello\"",
    "p[9].b[1].bco=200"
  ]
}

{
  "commands": [
    "page 1",
    "p0.pic=1",
    "p[1].b[1].txt=\"Heating is working\"",
    "p[1].b[1].pco=2047",
    "p[1].b[1].bco=0",
    "p[1].b[7].txt=\"12.3\"",
    "p[1].b[7].pco=65500",
    "p[1].b[7].bco=0",
    "p[1].b[8].txt=\"12.3\"",
    "p[1].b[8].pco=65500",
    "p[1].b[8].bco=0",
    "p[1].b[9].txt=\"12.3\"",
    "p[1].b[9].pco=65500",
    "p[1].b[9].bco=0",
    "p[1].b[10].txt=\"12.3\"",
    "p[1].b[10].pco=65500",
    "p[1].b[10].bco=0",
    "p[1].b[11].txt=\"12.3\"",
    "p[1].b[11].pco=65500",
    "p[1].b[11].bco=0",
    "p[1].b[12].txt=\"12.3\"",
    "p[1].b[12].pco=65500",
    "p[1].b[12].bco=0",
    "p[1].b[13].txt=\"12.3\"",
    "p[1].b[13].pco=65500",
    "p[1].b[13].bco=0",
    "p[1].b[14].txt=\"12.3\"",
    "p[1].b[14].pco=65500",
    "p[1].b[14].bco=0",
    "p[1].b[2].txt=\"12.3\"",
    "p[1].b[13].pco=65500",
    "p[1].b[13].bco=0"
  ]
}

developing conversion of colours

{
  "commands": [
    "page 1",
    "p[1].b[1].txt=\"Heating is working\"",
    "p[1].b[1].pco=h120",
    "p[1].b[1].bco=0"
  ]
}




 * 
 */
#endif