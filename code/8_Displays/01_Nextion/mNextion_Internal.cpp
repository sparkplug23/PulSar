#include "mNextion.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION


void mNextion::Command_SplashPage(char* pagename, uint8_t time_on_page)
{
  // saved current page to return to later
  splash_page_saved_page = settings.page;
  // Change page by name
  Command_SetPage(pagename);
  // start timer to return to saved page
  splash_page_seconds = time_on_page;

}

  
/******************************************************************************************************************
 * SECTION: Commands
*******************************************************************************************************************/

void mNextion::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken  jtok = 0; 
  JsonParserObject jobj = 0;
  int8_t tmp_id = 0;

  if(jtok = obj["Debug"].getObject()["Nextion"])
  {
    settings.dynamic_log_level = jtok.getInt();
    ALOG_COM(PSTR(D_LOG_NEXTION D_COMMAND_NVALUE_K("Debug")), settings.dynamic_log_level);
  }

  
  if(jtok = obj["Display"].getObject()["SetPageIfChanged"])
  {
    // if(jtok.isStr())
    // {
    //   Command_SetPageIfChanged(jtok.getInt());
    // }
    // else
    // if(jtok.isInt())
    // {
      Command_SetPageIfChanged(jtok.getInt());
    // }
  }


  if(jtok = obj["commands"]){
      JsonParserArray array = jtok;
      for(auto val : array) {
          ALOG_INF(PSTR("F::%s %s"),__FUNCTION__,val.getStr());
          nextionSendCmd(val.getStr());
          data_buffer.isserviced++;
      }
  }


  if(jtok = obj["Nextion"])
  {      
    SubParse_DisplaySet_JSON(jtok.getObject());
    settings.timeout_check.timeout_period = 10; //make fucntion to reset it
  }

  
  if(jtok = obj["Display"].getObject()["Println"]){
      // JsonParserArray array = jtok;
      // for(auto val : array) {
      //     // ALOG_INF(PSTR("F::%s %s"),__FUNCTION__,val.getStr());
      //     nextionSendCmd(val.getStr());
      // }
      display->println(jtok.getStr());
  }


  if(jtok = obj["Display"].getObject()[PM_BAUDRATE])
  {
    CommandSet_Baud(jtok.getInt());
    ALOG_COM(PSTR(D_LOG_NEXTION D_COMMAND_NVALUE_K(D_BAUDRATE)), jtok.getInt());
  }



  if(jtok = obj["Nextion"].getObject()["GetBaud"])
  {
    ALOG_COM(PSTR("display->baudRate=%d"), display->baudRate());
  }

  /***
   * 
   * Create new functions below that assume they are under "Nextion" json, but just parse that one bit out here for each the once
   * 
   * 
  */
  JsonParserObject obj1 = 0;
  JsonParserToken jtok_nextion = 0;
  JsonParserToken jtok_parse = 0;

  if(jtok_nextion = obj["Nextion"])
  {
    JsonParserObject json_obj = jtok_nextion.getObject();

    if(jtok_parse = json_obj["SetBrightness"])
    {
      Command_SetBrightness255(jtok_parse.getInt());
    }

  }

}


void mNextion::SubParse_DisplaySet_JSON(JsonParserObject obj)
{

  ALOG_DBG(PSTR("mNextion::SubParse_DisplaySet_JSON"));

  JsonParserToken jtok_items = 0;
  JsonParserToken jtok_items2 = 0;
  JsonParserToken jtok = 0;
  JsonParserToken jtok_item_attribute = 0;

  JsonParserObject obj1 = 0;

  
  char page_name[100] = {0};
  char item_name[100] = {0};
  char item_value[100] = {0};
  char command_ctr[100] = {0};

  /**
   * @brief Construct a new if object
   * If no page name is sent, then send commands without page name
   * 
   */

  if(jtok_items = obj["Page"])
  {
    sprintf(page_name,"%s",jtok_items.getStr());
    Command_SetPageIfChanged(6); //tmp fix
    ALOG_WRN(PSTR("Page is forced as 6 by default, need to fix this"));
  }


  if(obj1 = obj["Items"])
  {

    if(!strlen(page_name))
    {
      ALOG_DBG(PSTR("No page name sent, sending commands without page name"));
    }else{
      sniprintf(page_name, sizeof(page_name), "%s.", page_name); // add "." to page name to make code below easier
    }
    
    for(auto key : obj1) 
    {
      
      const char* key_string = key.getStr();
      ALOG_DBG(PSTR("Key = \"%s\""), key_string);
      sprintf(item_name,"%s",key_string);

      if(jtok = obj1[key_string])
      {

        if(jtok_item_attribute = jtok.getObject()["txt"])
        {
          ALOG_DBG(PSTR("txt = \"%s\""), jtok_item_attribute.getStr());
          sprintf(command_ctr,"%s%s.txt=\"%s\"", page_name, item_name, jtok_item_attribute.getStr());
          nextionSendCmd(command_ctr); 
        }

        if(jtok_item_attribute = jtok.getObject()["val"])
        {
          ALOG_DBG(PSTR("val = %d"), jtok_item_attribute.getInt());
          sprintf(command_ctr,"%s%s.val=%d", page_name, item_name, jtok_item_attribute.getInt());
          nextionSendCmd(command_ctr);  
        }
        
        if(jtok_item_attribute = jtok.getObject()["valf"])
        {
          char convf[TBUFFER_SIZE_FLOAT];
          mSupport::float2CString(jtok_item_attribute.getFloat(), 1, convf);
          ALOG_DBG(PSTR("val = \"%s\""), convf);          
          sprintf(command_ctr,"%s%s.val=%s", page_name, item_name, convf); //another conversion will be needed for float here later
          nextionSendCmd(command_ctr); 
        }

        if(jtok_item_attribute = jtok.getObject()["pco"])
        {
          ALOG_DBG(PSTR("pco = %d"), jtok_item_attribute.getInt());          
          sprintf(command_ctr,"%s%s.pco=%d", page_name, item_name, jtok_item_attribute.getInt());
          nextionSendCmd(command_ctr); 
        }

        if(jtok_item_attribute = jtok.getObject()["pco8"]) //8 bit standard R,G,B, needs conversion to 16 bit
        {
          if(jtok_item_attribute.isStr())
          {
            ALOG_DBG(PSTR("pco8 = %s"), jtok_item_attribute.getStr());   
            char colour_string[10] = {0};
            snprintf(colour_string, sizeof(colour_string), "%s", jtok_item_attribute.getStr());
            if(colour_string[0] == '#') // hex format of RGB in RGB888
            {
              uint32_t colour32bit = 0;
              colour32bit = (long) strtol( &colour_string[1], NULL, 16);
              sprintf(command_ctr,"%s%s.pco=%d", page_name, item_name, RGB888_to_RGB565(colour32bit));
              nextionSendCmd(command_ctr); 
            }
          }
        }

        if(jtok_item_attribute = jtok.getObject()["bco"])
        {
          ALOG_DBG(PSTR("bco = %d"), jtok_item_attribute.getInt());          
          sprintf(command_ctr,"%s%s.bco=%d", page_name, item_name, jtok_item_attribute.getInt());
          nextionSendCmd(command_ctr); 
        }
        
        if(jtok_item_attribute = jtok.getObject()["bco8"]) //8 bit standard R,G,B, needs conversion to 16 bit
        {
          if(jtok_item_attribute.isStr())
          {
            ALOG_DBG(PSTR("bco8 = %s"), jtok_item_attribute.getStr());   
            char colour_string[10] = {0};
            snprintf(colour_string, sizeof(colour_string), "%s", jtok_item_attribute.getStr());
            if(colour_string[0] == '#') // hex format of RGB in RGB888
            {
              uint32_t colour32bit = 0;
              colour32bit = (long) strtol( &colour_string[1], NULL, 16);
              sprintf(command_ctr,"%s%s.bco=%d", page_name, item_name, RGB888_to_RGB565(colour32bit));
              nextionSendCmd(command_ctr); 
            }
          }
        }
        
        if(jtok_item_attribute = jtok.getObject()["pic"])
        {
          ALOG_DBG(PSTR("pic = %d"), jtok_item_attribute.getInt());          
          sprintf(command_ctr,"%s%s.pic=%d", page_name, item_name, jtok_item_attribute.getInt());
          nextionSendCmd(command_ctr); 
        }
        
        data_buffer.isserviced++;      
        
      }

    }

  }

}


// std::string& replace(std::string& s, const std::string& from, const std::string& to)
// {
//     if(!from.empty())
//         for(size_t pos = 0; (pos = s.find(from, pos)) != std::string::npos; pos += to.size())
//             s.replace(pos, from.size(), to);
//     return s;
// }


void mNextion::CommandSet_Baud(uint32_t baud)
{
    // strPayload.toCharArray(nextionBaud, 7); // set nextionBaud to value provided in payload
    nextionAckEnable = false;

    char command_ctr[30];
    
    sprintf(command_ctr,"bauds=%d", baud);

    nextionSendCmd(command_ctr); // send baud rate to nextion

    nextionAckEnable = true;


    display->flush();
    display->end();
    display->begin(baud); // Serial2 - LCD TX, no RX

    ALOG_INF(PSTR("Set Baud Rate = %d"), baud);

}
 

void mNextion::SetAttribute_Txt(uint8_t page, uint8_t element_id, const char* ctr){
  char command_ctr[100];
  sprintf(command_ctr,"p[%d].b[%d].txt=\"%s\"",page,element_id,ctr);
  serial_print_suffixed(command_ctr);  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}

void mNextion::SetAttribute_BackgroundColour(uint8_t page, uint8_t element_id, uint32_t colour){
  char command_ctr[30];
  sprintf(command_ctr,"p[%d].b[%d].bco=%d",page,element_id,colour);
  serial_print_suffixed(command_ctr);
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}

void mNextion::SetAttribute_FontColour(uint8_t page, uint8_t element_id, uint32_t colour){
  char command_ctr[30];
  sprintf(command_ctr,"p[%d].b[%d].pco=%d",page,element_id,colour);
  serial_print_suffixed(command_ctr);
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}




void mNextion::nextionSetAttr(const char* hmiAttribute, const char* hmiValue)
{ // Set the value of a Nextion component attribute
  display->print(hmiAttribute);
  display->print("=");
  // display->print(utf8ascii((char*)hmiValue));
  display->print(utf8ascii((String)hmiValue));
  display->write(nextionSuffix, sizeof(nextionSuffix));
  
  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "PHASEOUT USING, KEEP LEGACY, SET %s=%s"),hmiAttribute,hmiValue);
}

void mNextion::nextionSetAttr(const char* hmiAttribute, uint32_t value)
{ // Set the value of a Nextion component attribute

  display->print(hmiAttribute);
  display->print("=");
  display->print(value);
  display->write(nextionSuffix, sizeof(nextionSuffix));
  
  // AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "PHASEOUT USING, KEEP LEGACY, SET %s=%s"),hmiAttribute,hmiValue);
}

void mNextion::nextionGetAttr(const char* c_str)
{ // Get the value of a Nextion component attribute
  // This will only send the command to the panel requesting the attribute, the actual
  // return of that value will be handled by nextionProcessInput and placed into mqttGetSubtopic

  char hmiattribute_ctr[100]; 
  memset(hmiattribute_ctr,0,sizeof(hmiattribute_ctr));
  sprintf(hmiattribute_ctr,"get %s",c_str);
  serial_print_suffixed(hmiattribute_ctr);
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX " GET 'get %s'"),hmiattribute_ctr);
}


void mNextion::nextionSendCmd(const char* c_str)
{ // Send a raw command to the Nextion panel
  serial_print(utf8ascii(c_str));
  serial_print_suffix();
  AddLog(settings.dynamic_log_level,PSTR(D_LOG_NEXTION D_NEXTION_TX " %s"),c_str);
}


void mNextion::nextionSendCmd_String(const String &nextionCmd)
{ // Send a raw command to the Nextion panel
  display->print(nextionCmd);
  display->write(nextionSuffix, sizeof(nextionSuffix));
  display->flush();
  ALOG_INF(PSTR(D_LOG_NEXTION "HMI OUT: %s"), nextionCmd);

  if (nextionAckEnable)
  {
    nextionAckReceived = false;
    nextionAckTimer = millis();

    while ((!nextionAckReceived) && (millis() - nextionAckTimer < nextionAckTimeout))
    {
      nextionHandleInput();
    }
    if (!nextionAckReceived)
    {
      ALOG_INF(PSTR(D_LOG_NEXTION "HMI ERROR: Nextion Ack timeout"));
      String mqttButtonJSONEvent = String(F("{\"event\":\"nextionError\",\"value\":\"Nextion Ack timeout\"}"));
      // mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
      // ALOG_INF(PSTR(D_LOG_NEXTION "MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
    }
  }
  else
  {
    nextionHandleInput();
  }
}


/******************************************************************************************************************
 * SECTION: HardwareInterface
*******************************************************************************************************************/


bool mNextion::nextionConnect()
{
  memset(nextionSuffix, 0xFF, sizeof(nextionSuffix));

  const uint32_t nextionCheckTimeout = 2000;

  // Ensure we don't carry stale state
  lcdConnected = false;
  nextionAckEnable = false;

  // 1) Try at current speed
  uint32_t t0 = millis();
  ALOG_INF(PSTR("HMI: Waiting for LCD connection"));
  nextionSendCmd("connect");
  while ((millis() - t0) <= nextionCheckTimeout && !lcdConnected) {
    nextionHandleInput();
  }

  // 2) If still not connected, scan speeds and retry
  if (!lcdConnected)
  {
    nextionSetSpeed();

    t0 = millis();
    ALOG_INF(PSTR("HMI: Waiting again for LCD connection"));
    nextionSendCmd("connect");
    while ((millis() - t0) <= nextionCheckTimeout && !lcdConnected) {
      nextionHandleInput();
    }

    if (!lcdConnected) {
      ALOG_INF(PSTR("HMI: LCD connection timed out"));
      return false;
    }
  }

  // 3) Backlight query
  lcdBacklightQueryFlag = true;
  ALOG_INF(PSTR("HMI: Querying LCD backlight status"));
  nextionSendCmd("get dim");

  t0 = millis();
  while ((millis() - t0) <= nextionCheckTimeout && lcdBacklightQueryFlag) {
    nextionHandleInput();
  }
  if (lcdBacklightQueryFlag) {
    ALOG_INF(PSTR("HMI: LCD backlight query timed out"));
    lcdBacklightQueryFlag = false;
    return false;
  }

  // Enable ACK checking from now on
  nextionAckEnable = true;
  nextionSendCmd("bkcmd=3");

  // 4) Version query
  lcdVersionQueryFlag = true;
  ALOG_INF(PSTR("HMI: Querying LCD firmware version number"));
  nextionSendCmd_String("get " + lcdVersionQuery);

  t0 = millis();
  while ((millis() - t0) <= nextionCheckTimeout && lcdVersionQueryFlag) {
    nextionHandleInput();
  }
  if (lcdVersionQueryFlag) {
    ALOG_INF(PSTR("HMI: LCD version query timed out"));
    lcdVersionQueryFlag = false;
    return false;
  }

  // 5) Model query (connect response fills nextionModel)
  if (nextionModel.length() == 0) {
    ALOG_INF(PSTR("HMI: Querying LCD model information"));
    nextionSendCmd("connect");

    t0 = millis();
    while ((millis() - t0) <= nextionCheckTimeout && (nextionModel.length() == 0)) {
      nextionHandleInput();
    }
    // optional: do not hard-fail if simulator
  }

  return true;
}



void mNextion::nextionReset()
{
  AddLog(LOG_LEVEL_DEV_TEST, PSTR(D_LOG_NEXTION "%s"), "HMI: Rebooting LCD");

  if (!display) {
    AddLog(LOG_LEVEL_DEV_TEST, PSTR(D_LOG_NEXTION "%s"), "HMI: Rebooting LCD aborted (display is null)");
    return;
  }

  const uint32_t lcdResetTimeout = 5000;
  const uint32_t tStart = millis();

  // Reset state so we *must* see fresh traffic
  lcdConnected = false;
  nextionAckReceived = false;

  // Best-effort clear of any partial RX
  incoming_frame.reset();

  // Send initial reset
  nextionSendCmd("rest"); // ensures 0xFF 0xFF 0xFF is appended by your command helper
  display->flush();
  
  // Wait for any response from panel; keep sending rest periodically until we see traffic or timeout
  while (!lcdConnected && (millis() - tStart) <= lcdResetTimeout)
  {
    // Pump RX (this is what will set lcdConnected once any complete frame arrives)
    nextionHandleInput();

    // Re-issue reset every ~250ms while waiting (prevents a lost command from stalling recovery)
    static uint32_t tLastKick = 0;
    if ((millis() - tLastKick) >= 250) {
      tLastKick = millis();
      nextionSendCmd("rest");
      display->flush();
    }

    delay(10);
    yield();
  }

  if (lcdConnected){
    AddLog(LOG_LEVEL_DEV_TEST, PSTR(D_LOG_NEXTION "%s"), "HMI: Rebooting LCD completed");
    tkr->Tasker_Interface(TASK_DISPLAY_CONNECTION_ESTABLISHED);
  }else{
    AddLog(LOG_LEVEL_DEV_TEST, PSTR(D_LOG_NEXTION "%s"), "ERROR: Rebooting LCD timed out, LCD not responding");
    tkr->Tasker_Interface(TASK_DISPLAY_CONNECTION_FAILED);
  }
  
}



void mNextion::nextionSetSpeed()
{

  #ifdef USE_NEXTION_SOFTWARE_SERIAL
    Serial.println(F("HMI: No Nextion response, attempting 9600bps connection\n\n\n\n\n\n\n"));
    swSer->begin(9600);
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->print("bauds=38400");
    //swSer->print("bauds=9600");
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->flush();
    swSer->begin(38400);
  #endif

}


void mNextion::nextionHandleInput()
{
  // Read incoming Nextion bytes into incoming_frame until 0xFF 0xFF 0xFF terminator.
  // When complete, call ProcessInput() directly (legacy nextionReturnBuffer/index removed).

  if (!display) { return; }

  const uint32_t TIMEOUT_MS = 100;
  const uint32_t tStop = millis() + TIMEOUT_MS;

  static uint8_t termFFCount = 0;

  auto reset_rx_state = [&]() {
    incoming_frame.reset();
    termFFCount = 0;
  };

  // Start fresh for this call (do not carry partials unless you explicitly want that)
  incoming_frame.complete = false;

  while (display->available() && (int32_t)(millis() - tStop) < 0)
  {
    const uint8_t b = (uint8_t)display->read();

    // Bounds check
    if (incoming_frame.len >= NEXTION_RX_MAX)
    {
      ALOG_ERR(PSTR(D_LOG_NEXTION "HMI RX overflow (dropping frame): frame.len=%u max=%u"),
               (unsigned)incoming_frame.len, (unsigned)NEXTION_RX_MAX);
      reset_rx_state();
      return;
    }

    incoming_frame.data[incoming_frame.len++] = b;

    // Serial.write(b); // Optional: raw echo for debug (disable for production)

    // Terminator detection: 0xFF 0xFF 0xFF
    if (b == 0xFF) {
      if (++termFFCount >= 3) {
        incoming_frame.complete = true;
        termFFCount = 0;
        break;
      }
    } else {
      termFFCount = 0;
    }

    yield();
  }

  if (incoming_frame.complete)
  {
    lcdConnected = true;
    nextionAckReceived = true;

    // New path: decode/dispatch using incoming_frame
    ProcessInput();

    // Drop frame after processing
    incoming_frame.reset();
    return;
  }

  // Timeout: drop partial frame so it doesn't contaminate next call
  if ((int32_t)(millis() - tStop) >= 0)
  {
    if (incoming_frame.len > 0) {
      ALOG_WRN(PSTR(D_LOG_NEXTION "HMI RX timeout (dropping partial): frame.len=%u"),
               (unsigned)incoming_frame.len);
    }
    reset_rx_state();
  }
}



/******************************************************************************************************************
 * SECTION: Helper Functions
*******************************************************************************************************************/


/**
 * @brief This function going forward should only send the command if the page is not already set, so will need a way to poll this
 * 
 * @param page 
 */
void mNextion::Command_SetPage(uint8_t page){
  settings.page = page;
  char ctr[10];
  sprintf(ctr,"page %d",page);
  nextionSendCmd(ctr);
}
void mNextion::Command_SetPage(char* pagename)
{
  nextionSendCmd(pagename);
}

void mNextion::Command_SetPageIfChanged(uint8_t page){
  if(settings.page == page) {return;}
  Command_SetPage(page);
}


void mNextion::Command_SetBrightness(uint8_t brightness_percentage){
  char ctr[10];
  sprintf(ctr,"dims=%d",brightness_percentage); //dims range 0-100
  nextionSendCmd(ctr);
  settings.brightness_percentage = brightness_percentage;
}

void mNextion::Command_SetBrightness255(uint8_t brightness){
  settings.brightness_percentage = map(brightness,0,255,0,100);
  Command_SetBrightness(settings.brightness_percentage);
}

void mNextion::Command_ToggleBrightness(){  
  Command_SetBrightness(settings.brightness_percentage>0?0:settings.brightness_percentage);
}


uint16_t mNextion::RGB888_to_RGB565(uint8_t r, uint8_t g, uint8_t b){
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

uint16_t mNextion::RGB888_to_RGB565(uint32_t rgb_888) // W,R,G,B
{
  uint8_t r = (rgb_888 >> 16) & 0xFF;
  uint8_t g = (rgb_888 >> 8) & 0xFF;
  uint8_t b = rgb_888 & 0xFF;
  uint16_t rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  // ALOG_INF(PSTR("%08X r,g,b = %d,%d,%d --> %d"),rgb_888, r,g,b, rgb565);
  return rgb565;
}


// UTF8-Decoder: convert UTF8-String to extended ASCII http://playground.arduino.cc/main/Utf8ascii
// Workaround for issue here: https://github.com/home-assistant/home-assistant/issues/9528
// Nextion claims that "Unicode and UTF will not be among the supported encodings", so this should
// be safe to run against all attribute values coming in.
byte mNextion::utf8ascii(byte ascii)
{ 

  static byte c1; // Last character buffer
  // Convert a single Character from UTF8 to Extended ASCII. Return "0" if a byte has to be ignored.
  if (ascii < 128)
  { // Standard ASCII-set 0..0x7F handling
    c1 = 0;
    return (ascii);
  }
  // get previous input
  byte last = c1; // get last char
  c1 = ascii;     // remember actual character
  switch (last)
  { // conversion depending on first UTF8-character
  case 0xC2:
    return (ascii);
    break;
  case 0xC3:
    return (ascii | 0xC0);
    break;
  case 0x82:
    if (ascii == 0xAC)
      return (0x80); // special case Euro-symbol
  }
  return (0); // otherwise: return zero, if character has to be ignored
}

String mNextion::utf8ascii(String s)
{ // convert String object from UTF8 String to Extended ASCII
  String r = "";
  char c;
  for (uint16_t i = 0; i < s.length(); i++)
  {
    c = utf8ascii(s.charAt(i));
    if (c != 0)
      r += c;
  }
  return r;
}


void mNextion::EverySecond_FlashScreen(){

  if(flash_message.cShowSeconds==0){
    // Return screen to previous
    AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION D_COMMAND_NVALUE),"settings.page_saved",settings.page_saved);
    Command_SetPage(settings.page_saved);
    flash_message.cShowSeconds = -1;
  }else
  if(flash_message.cShowSeconds>0){
    flash_message.cShowSeconds--;
    AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_NEXTION D_COMMAND_NVALUE),"flash_message.cShowSeconds",flash_message.cShowSeconds);
  }

} //end F


void mNextion::EverySecond_SendScreenInfo(){

  char rtc_ctr[40]; memset(rtc_ctr,'\0',sizeof(rtc_ctr));
  sprintf(rtc_ctr, "%02d:%02d:%02d\n\r",
  tkr_time->RtcTime.hour,tkr_time->RtcTime.minute,tkr_time->RtcTime.second);

  SetAttribute_Txt(settings.page,1,rtc_ctr);
  
  // char health_ctr[40]; memset(health_ctr,'\0',sizeof(health_ctr));
  // sprintf(health_ctr, "%c%c%c %d",
  //   WiFi.status() == WL_CONNECTED ? 'N' : 'n',
  //   tkr_mqtt->pubsub->connected() ? 'M' : 'm',
  //   fOpenHABDataStreamActive ? 'O' : 'o',
  //   0
  //   // tkr_wifi->WifiGetRssiAsQuality(WiFi.RSSI())
  // );

  // Serial.println(health_ctr);

  // SetAttribute_Txt(settings.page,5,health_ctr);

}





const char*  mNextion::GetObjectName_FromID(uint8_t id, char* objname, uint8_t objname_size)
{
  

  uint16_t progmem_size = sizeof(DISPLAY_TEMPLATE);
  char buffer[progmem_size];
  memcpy_P(buffer,DISPLAY_TEMPLATE,sizeof(DISPLAY_TEMPLATE));

  JsonParser parser(buffer);
  JsonParserObject rootObj = parser.getRootObject();   
  if (!rootObj) 
  {
    ALOG_ERR(PSTR("DeserializationError with \"%s\""), buffer);
    return PM_SEARCH_NOMATCH;
  } 
  else
  {
    // ALOG_DBG(PSTR("Deserialization Success with \"%s\""), buffer);
  }
  
  JsonParserToken jtok = 0;
  JsonParserObject obj1 = 0;
  char item_name[100] = {0};

  // ALOG_INF(PSTR("ObjectNameID size %d"), rootObj["ObjectNameID"].size());

  if(jtok = rootObj["ObjectNameID"])
  {

    uint8_t jsonpair_count = jtok.size();

    for(int pair_index = 0; pair_index < jsonpair_count; pair_index++){

      jtok.nextOne(); // Skip over "ObjectNameID" to first key
      const char* key = jtok.getStr();
      // ALOG_INF(PSTR("ObjectNameID key %s"), key);
      jtok.nextOne(); // Skip to value
      const char* value = jtok.getStr();
      // ALOG_INF(PSTR("ObjectNameID value %s"), value);

      if(jtok.getInt() == id)
      {
        snprintf(objname, objname_size, "%s", key);
        ALOG_INF(PSTR("ObjectNameID objname %s"), objname);
        return objname;
      }

    }
  }

  return PM_SEARCH_NOMATCH;
}





/******************************************************************************************************************
 * SECTION: WebServer
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_WEBSERVER

void mNextion::WebPage_AddHandlers()
{
  // =====================================================
  // Static Nextion pages (served via handleStaticContent)
  // =====================================================

  tkr_web->server->on("/nextion/main", HTTP_GET, [](AsyncWebServerRequest* request){
    tkr_web->handleStaticContent(
      request, "", 200, FPSTR(CONTENT_TYPE_HTML),
      WEB_8_Displays_01_Nextion_nextion_root_htm,
      WEB_8_Displays_01_Nextion_nextion_root_htm_length
    );
  });

  tkr_web->server->on("/nextion/firmware", HTTP_GET, [](AsyncWebServerRequest* request){
    tkr_web->handleStaticContent(
      request, "", 200, FPSTR(CONTENT_TYPE_HTML),
      WEB_8_Displays_01_Nextion_firmware_htm,
      WEB_8_Displays_01_Nextion_firmware_htm_length
    );
  });

  tkr_web->server->on("/nextion/command", HTTP_GET, [](AsyncWebServerRequest* request){
    tkr_web->handleStaticContent(
      request, "", 200, FPSTR(CONTENT_TYPE_HTML),
      WEB_8_Displays_01_Nextion_nextion_command_htm,
      WEB_8_Displays_01_Nextion_nextion_command_htm_length
    );
  });

  tkr_web->server->on("/nextion/lcdOtaSuccess", HTTP_GET, [](AsyncWebServerRequest* request){
    tkr_web->handleStaticContent(
      request, "", 200, FPSTR(CONTENT_TYPE_HTML),
      WEB_8_Displays_01_Nextion_lcd_ota_success_htm,
      WEB_8_Displays_01_Nextion_lcd_ota_success_htm_length
    );
  });

  tkr_web->server->on("/nextion/lcdOtaFailure", HTTP_GET, [](AsyncWebServerRequest* request){
    tkr_web->handleStaticContent(
      request, "", 200, FPSTR(CONTENT_TYPE_HTML),
      WEB_8_Displays_01_Nextion_lcd_ota_failure_htm,
      WEB_8_Displays_01_Nextion_lcd_ota_failure_htm_length
    );
  });

  // =====================================================
  // Dynamic Nextion endpoints
  // =====================================================

  // TFT filesize (used by JS before upload)
  tkr_web->server->on("/nextion/tftFileSize", HTTP_GET,
    [this](AsyncWebServerRequest* request){
      this->webHandleTftFileSize(request);
    }
  );

  // TFT upload (multipart)
  tkr_web->server->on(
    "/nextion/lcdupload",
    HTTP_POST,
    [](AsyncWebServerRequest* request){ /* completion handled in final */ },
    [this](AsyncWebServerRequest* request,
           String filename,
           size_t index,
           uint8_t* data,
           size_t len,
           bool final)
    {
      this->webHandleLcdUpload(request, filename, index, data, len, final);
    }
  );

  // TFT download via URL
  tkr_web->server->on("/nextion/lcddownload", HTTP_GET,
    [this](AsyncWebServerRequest* request){
      this->webHandleLcdDownload(request);
    }
  );

  // Raw Nextion command receiver (GET + POST)
  tkr_web->server->on("/nextion/cmd", HTTP_GET,
    [this](AsyncWebServerRequest* request){
      this->webHandleNextionCmd(request);
    }
  );

  tkr_web->server->on("/nextion/cmd", HTTP_POST,
    [this](AsyncWebServerRequest* request){
      this->webHandleNextionCmd(request);
    }
  );

  // Reboot Nextion panel ONLY (not ESP)
  tkr_web->server->on("/nextion/reboot_panel", HTTP_GET,
    [this](AsyncWebServerRequest* request){
      this->webHandleNextionRebootPanel(request);
    }
  );
}




void mNextion::WebHandle_Nextion_Command(AsyncWebServerRequest* request)
{
  String cmd;

  // POST form field
  if (request->hasParam("cmd", true)) {
    cmd = request->getParam("cmd", true)->value();
  }
  // GET query param
  else if (request->hasParam("cmd")) {
    cmd = request->getParam("cmd")->value();
  }

  cmd.trim();
  if (!cmd.length()) {
    request->send(400, "text/plain", "Missing cmd");
    return;
  }

  // Optional safety: cap length to avoid insane payloads
  if (cmd.length() > 256) {
    request->send(413, "text/plain", "cmd too long");
    return;
  }

  this->nextionSendCmd(cmd.c_str());
  request->send(200, "text/plain", "OK");
}


void mNextion::webHandleNextionCmd(AsyncWebServerRequest* request)
{
  String cmd;

  // Accept GET: /nextion/cmd?cmd=page%200
  if (request->hasParam(F("cmd")))
  {
    cmd = request->getParam(F("cmd"))->value();
  }
  // Accept POST form field "cmd"
  else if (request->hasParam(F("cmd"), true))
  {
    cmd = request->getParam(F("cmd"), true)->value();
  }

  cmd.trim();

  if (cmd.length())
  {
    this->nextionSendCmd(cmd.c_str());
  }

  // Keep it simple: go back to command page
  request->redirect(F("/nextion/command"));
}

void mNextion::webHandleNextionRebootPanel(AsyncWebServerRequest* request)
{
  this->nextionSendCmd("rest");
  request->redirect(F("/nextion/command"));
}




#endif // USE_MODULE_NETWORK_WEBSERVER







#endif  // USE_MODULE_DISPLAYS_NEXTION