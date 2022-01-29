
#include "mNextionPanel.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION

// Functions which do the majority of the work, for now placed in here to make it easier to build up how the thing works
// Likely this will be moved back in to panel


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionReset()
{
  AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI: Rebooting LCD");
  //digitalWrite(nextionResetPin, LOW);
  
  #ifdef USE_NEXTION_SOFTWARE_SERIAL
    swSer->print("rest");
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->flush();
  #else
    SERIAL_NEXTION_TX.print("rest");
    SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
    SERIAL_NEXTION_TX.flush();
  #endif

  delay(100);
  //digitalWrite(nextionResetPin, HIGH);

  unsigned long lcdResetTimer = millis();
  const unsigned long lcdResetTimeout = 5000;

  lcdConnected = false;
  while (!lcdConnected && (millis() < (lcdResetTimer + lcdResetTimeout)))
  {
    Serial.println("while-nextionReset");
    nextionHandleInput();
  }
  if (lcdConnected)
  {
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI: Rebooting LCD completed");
    if (settings.page)
    {


char command_ctr[15];

sprintf(command_ctr,"page %d\0",settings.page);

      nextionSendCmd(command_ctr);
    }
  }
  else
  {
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"ERROR: Rebooting LCD completed, but LCD is not responding.");
  }
  //mqttClient.publish(mqttStatusTopic, WILLMESSAGE_ONDISCONNECT_CTR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionConnect()
{
  if ((millis() - nextionCheckTimer) >= nextionCheckInterval)
  {
    static unsigned int nextionRetryCount = 0;
    if ((nextionModel.length() == 0) && (nextionRetryCount < (nextionRetryMax - 2)))
    { // Try issuing the "connect" command a few times
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion connect request"));
      nextionSendCmd("connect");
      nextionRetryCount++;
      nextionCheckTimer = millis();
    }
    else if ((nextionModel.length() == 0) && (nextionRetryCount < nextionRetryMax))
    { // If we still don't have model info, try to change nextion serial speed from 9600 to 115200
      nextionSetSpeed();
      nextionRetryCount++;
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion serial speed 115200 request"));
      nextionCheckTimer = millis();
    }
    else if ((lcdVersion < 1) && (nextionRetryCount <= nextionRetryMax))
    {
      if (nextionModel.length() == 0)
      { // one last hail mary, maybe the serial speed is set correctly now
        nextionSendCmd("connect");
      }
      //nextionSendCmd("get " + lcdVersionQuery.toString().c_str());
      lcdVersionQueryFlag = true;
      nextionRetryCount++;
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion version query"));
      nextionCheckTimer = millis();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionSetSpeed()
{
  
  ////#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL
    Serial.println(F("HMI: No Nextion response, attempting 9600bps connection\n\n\n\n\n\n\n"));
    swSer->begin(9600);
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->print("bauds=38400");
    //swSer->print("bauds=9600");
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
    swSer->flush();
    swSer->begin(38400);
  #else
  
  ////AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI: No Nextion response, attempting 9600bps connection"));
  SERIAL_NEXTION_TX.begin(9600);
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.print("bauds=38400");
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.flush();
  SERIAL_NEXTION_TX.begin(NEXTION_BAUD);

  
  SERIAL_NEXTION_TX.begin(115200);
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.print("bauds=38400");
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.flush();
  SERIAL_NEXTION_TX.begin(NEXTION_BAUD);

  #endif

}


////////////////////////////////////////////////////////////////////////////////////////////////////
bool mNextionPanel::nextionHandleInput()
{ // Handle incoming serial data from the Nextion panel
  // This will collect serial data from the panel and place it into the global buffer
  // nextionReturnBuffer[nextionReturnIndex]
  // Return: true if we've received a string of 3 consecutive 0xFF values
  // Return: false otherwise
  bool nextionCommandComplete = false;
  static int nextionTermByteCnt = 0;   // counter for our 3 consecutive 0xFFs
  static String hmiDebug = ""; // assemble a string for debug output

  if (serial_available())
  {
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION " if (Serial.available())"));
    lcdConnected = true;
    byte nextionCommandByte = serial_read();

    hmiDebug += (" 0x" + String(nextionCommandByte, HEX));
    // check to see if we have one of 3 consecutive 0xFF which indicates the end of a command
    if (nextionCommandByte == 0xFF)
    {
      nextionTermByteCnt++;
      if (nextionTermByteCnt >= 3)
      { // We have received a complete command
        nextionCommandComplete = true;
        nextionTermByteCnt = 0; // reset counter
      }
    }
    else
    {
      nextionTermByteCnt = 0; // reset counter if a non-term byte was encountered
    }
    nextionReturnBuffer[nextionReturnIndex] = nextionCommandByte;
    nextionReturnIndex++;
  }
  if (nextionCommandComplete)
  {
  //  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " %s"),
    //Serial.println(ConvertBytetoASCII(nextionReturnBuffer,nextionReturnIndex));
    // for(int i=0;i<nextionReturnIndex;i++){
    //   AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "%d | %s"),i,String(nextionReturnBuffer[i], HEX));
    // }
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " %s"),hmiDebug.c_str());
    hmiDebug = "";

    
DEBUG_LINE_HERE;
Serial.println(hmiDebug);

  }



  return nextionCommandComplete;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionProcessInput()
{ // Process incoming serial commands from the Nextion panel
  // Command reference: https://www.itead.cc/wiki/Nextion_Instruction_Set#Format_of_Device_Return_Data
  // tl;dr, command byte, command data, 0xFF 0xFF 0xFF

  //Serial.println("nextionProcessInput");
  
  char event_ctr[30];
  //https://www.itead.cc/wiki/Nextion_Instruction_Set

  // first instructions byte
  switch(nextionReturnBuffer[0]){
    case NEXTION_COMMAND_INVALID_INSTRUCTION:
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX D_NEXTION_COMMAND D_NEXTION_COMMAND_INVALID_INSTRUCTION_CTR));   
    break;

  }
  

  if (nextionReturnBuffer[0] == 0x65)
  { // Handle incoming touch command
    // 0x65+Page ID+Component ID+TouchEvent+End
    // Return this data when the touch event created by the user is pressed.
    // Definition of TouchEvent: Press Event 0x01, Release Event 0X00
    // Example: 0x65 0x00 0x02 0x01 0xFF 0xFF 0xFF
    // Meaning: Touch Event, Page 0, Object 2, Press
    String nextionPage = String(nextionReturnBuffer[1]);
    String nextionButtonID = String(nextionReturnBuffer[2]);
    byte nextionButtonAction = nextionReturnBuffer[3];

    screen_press.page = nextionReturnBuffer[1];
    screen_press.event = nextionReturnBuffer[2];

    if (nextionButtonAction == 0x01) // ON=PRESSED
    {
      screen_press.tSavedButtonONEvent = millis();
      screen_press.fEnableImmediateButtonTime = true; 

      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "\"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,D_JSON_ON);
              
      memset(event_ctr,0,sizeof(event_ctr));
      sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);

      JsonBuilderI->Start();
        JsonBuilderI->Add("event", event_ctr);
        JsonBuilderI->Add("value", D_JSON_ON);
      JsonBuilderI->End();

      pCONT_mqtt->ppublish("status/nextion/event",JsonBuilderI->GetBufferPtr(),0);
      pCONT_mqtt->ppublish("status/nextion/event/start",JsonBuilderI->GetBufferPtr(),0);

    }
    if (nextionButtonAction == 0x00) // OFF - LET_GO
    {
      screen_press.tSavedButtonOFFEvent = millis();
      screen_press.fEnableImmediateButtonTime = false; //start timer
      screen_press.tSavedButtonONDurationEvent = screen_press.tSavedButtonOFFEvent - screen_press.tSavedButtonONEvent;
      screen_press.duration = screen_press.tSavedButtonONDurationEvent;
      
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "\"p[%d].b[%d]\"=%s"),screen_press.page,screen_press.event,D_JSON_OFF);
              
      memset(event_ctr,0,sizeof(event_ctr));
      sprintf(event_ctr,"p[%d].b[%d]",screen_press.page,screen_press.event);
      
      JsonBuilderI->Start();
        JsonBuilderI->Add("event", event_ctr);
        JsonBuilderI->Add("value", D_JSON_OFF);
        JsonBuilderI->Add("duration", screen_press.tSavedButtonONDurationEvent);
      JsonBuilderI->End();

      if(!fEnableIgnoreNextOffEvent){
        AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "fEnableIgnoreNextOffEvent = NOT set"));
        pCONT_mqtt->ppublish("status/nextion/event",JsonBuilderI->GetBufferPtr(),0);
        pCONT_mqtt->ppublish("status/nextion/event/end",JsonBuilderI->GetBufferPtr(),0);
        MQTTSend_PressEvent();
      }else{
        fEnableIgnoreNextOffEvent = false;// reset to listen to next event
        AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "fEnableIgnoreNextOffEvent = reset"));
      }


      // Now see if this object has a .val that might have been updated.  Works for sliders,
      // two-state buttons, etc, throws a 0x1A error for normal buttons which we'll catch and ignore
      // mqttGetSubtopic = "/p[" + nextionPage + "].b[" + nextionButtonID + "].val";
      // mqttGetSubtopicJSON = "p[" + nextionPage + "].b[" + nextionButtonID + "].val";
      // nextionGetAttr("p[" + nextionPage + "].b[" + nextionButtonID + "].val");
    }
  }
  else if (nextionReturnBuffer[0] == 0x66)
  { // Handle incoming "sendme" page number
    // 0x66+PageNum+End
    // Example: 0x66 0x02 0xFF 0xFF 0xFF
    // Meaning: page 2
    String nextionPage = String(nextionReturnBuffer[1]);
    
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "[sendme Page] \"%s\""),nextionPage.c_str());

    if ((settings.page != nextionPage.toInt()) && ((nextionPage != "0") || nextionReportPage0))
    { // If we have a new page AND ( (it's not "0") OR (we've set the flag to report 0 anyway) )
      settings.page = nextionPage.toInt();
      String mqttPageTopic = mqttStateTopic + "/page";      
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: mqttPageTopic=\"%s\" nextionPage=\"%s\""),mqttPageTopic.c_str(),nextionPage.c_str());
      //mqttClient.publish(mqttPageTopic, nextionPage);
      pCONT_mqtt->ppublish("status/nextion/event4",nextionPage.c_str(),0);
    }
  }
  else if (nextionReturnBuffer[0] == 0x67)
  { // Handle touch coordinate data
    // 0X67+Coordinate X High+Coordinate X Low+Coordinate Y High+Coordinate Y Low+TouchEvent+End
    // Example: 0X67 0X00 0X7A 0X00 0X1E 0X01 0XFF 0XFF 0XFF
    // Meaning: Coordinate (122,30), Touch Event: Press
    // issue Nextion command "sendxy=1" to enable this output
    uint16_t xCoord = nextionReturnBuffer[1];
    xCoord = xCoord * 256 + nextionReturnBuffer[2];
    uint16_t yCoord = nextionReturnBuffer[3];
    yCoord = yCoord * 256 + nextionReturnBuffer[4];
    String xyCoord = String(xCoord) + ',' + String(yCoord);
    byte nextionTouchAction = nextionReturnBuffer[5];
    if (nextionTouchAction == 0x01)
    {  
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [Touch ON] '%s'"),xyCoord.c_str());
      String mqttTouchTopic = mqttStateTopic + "/touchOn";
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' '%s'"),mqttTouchTopic.c_str(),xyCoord.c_str());
      pCONT_mqtt->ppublish("status/nextion/xyCoord",xyCoord.c_str(),0);
    }
    else if (nextionTouchAction == 0x00)
    {
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [Touch OFF] '%s'"),xyCoord.c_str());
      String mqttTouchTopic = mqttStateTopic + "/touchOff";
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' '%s'"),mqttTouchTopic.c_str(),xyCoord.c_str());
      pCONT_mqtt->ppublish("status/nextion/event6",xyCoord.c_str(),0);
    }
  }
  else if (nextionReturnBuffer[0] == 0x70)
  { // Handle get string return
    // 0x70+ASCII string+End
    // Example: 0x70 0x41 0x42 0x43 0x44 0x31 0x32 0x33 0x34 0xFF 0xFF 0xFF
    // Meaning: String data, ABCD1234
    String getString;
    for (int i = 1; i < nextionReturnIndex - 3; i++)
    { // convert the payload into a string
      getString += (char)nextionReturnBuffer[i];
    }
    
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "HMI IN: [String Return] '%s'"),getString.c_str());
  
    if (mqttGetSubtopic == "")
    { // If there's no outstanding request for a value, publish to mqttStateTopic
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' : '%s']"),mqttStateTopic.c_str(),getString.c_str());
      pCONT_mqtt->ppublish("status/nextion/getString",getString.c_str(),0);
    }
    else
    { // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
      String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;      
      AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION "MQTT OUT: '%s' : '%s']"),mqttReturnTopic.c_str(),getString.c_str());
      pCONT_mqtt->ppublish("status/nextion/getString",getString.c_str(),0);
      mqttGetSubtopic = "";
    }
  }
  else if (nextionReturnBuffer[0] == 0x71)
  { // Handle get int return
    // 0x71+byte1+byte2+byte3+byte4+End (4 byte little endian)
    // Example: 0x71 0x7B 0x00 0x00 0x00 0xFF 0xFF 0xFF
    // Meaning: Integer data, 123
    unsigned long getInt = nextionReturnBuffer[4];
    getInt = getInt * 256 + nextionReturnBuffer[3];
    getInt = getInt * 256 + nextionReturnBuffer[2];
    getInt = getInt * 256 + nextionReturnBuffer[1];
    String getString = String(getInt);
    //AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: [Int Return] '")) + getString + "'");

    if (lcdVersionQueryFlag)
    {
      lcdVersion = getInt;
      lcdVersionQueryFlag = false;
      ////AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: lcdVersion '")) + String(lcdVersion) + "'");
    }
    else if (mqttGetSubtopic == "")
    {
      //mqttClient.publish(mqttStateTopic, getString);
      pCONT_mqtt->ppublish("status/nextion/event9",getString.c_str(),0);
    }
    // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
    else
    {
      String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
      //mqttClient.publish(mqttReturnTopic, getString);
      pCONT_mqtt->ppublish("status/nextion/event10",getString.c_str(),0);
      String mqttButtonJSONEvent = String(F("{\"event\":\"")) + mqttGetSubtopicJSON + String(F("\", \"value\":")) + getString + String(F("}"));
      //mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
      pCONT_mqtt->ppublish("status/nextion/event11",mqttButtonJSONEvent.c_str(),0);
      mqttGetSubtopic = "";
    }
  }
  else if (nextionReturnBuffer[0] == 0x63 && nextionReturnBuffer[1] == 0x6f && nextionReturnBuffer[2] == 0x6d && nextionReturnBuffer[3] == 0x6f && nextionReturnBuffer[4] == 0x6b)
  { // Catch 'comok' response to 'connect' command: https://www.itead.cc/blog/nextion-hmi-upload-protocol
    String comokField;
    uint8_t comokFieldCount = 0;
    byte comokFieldSeperator = 0x2c; // ","

    for (uint8_t i = 0; i <= nextionReturnIndex; i++)
    { // cycle through each byte looking for our field seperator
      if (nextionReturnBuffer[i] == comokFieldSeperator)
      { // Found the end of a field, so do something with it.  Maybe.
        if (comokFieldCount == 2)
        {
          nextionModel = comokField;
          ////AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION "%s"),"HMI IN: nextionModel: ")) + nextionModel);
        }
        comokFieldCount++;
        comokField = "";
      }
      else
      {
        comokField += String(char(nextionReturnBuffer[i]));
      }
    }
  }

  // else if (nextionReturnBuffer[0] == 0x1A)
  // { // Catch 0x1A error, possibly from .val query against things that might not support that request
  //   // 0x1A+End
  //   // ERROR: Variable name invalid
  //   // We'll be triggering this a lot due to requesting .val on every component that sends us a Touch Off
  //   // Just reset mqttGetSubtopic and move on with life.
  //   mqttGetSubtopic = "";
  // }
  nextionReturnIndex = 0; // Done handling the buffer, reset index back to 0
}



void mNextionPanel::SetAttribute_Txt(uint8_t page, uint8_t element_id, const char* ctr){
  char command_ctr[100];
  sprintf(command_ctr,"p[%d].b[%d].txt=\"%s\"",page,element_id,ctr);
  serial_print_suffixed(command_ctr);  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}

void mNextionPanel::SetAttribute_BackgroundColour(uint8_t page, uint8_t element_id, uint32_t colour){
  char command_ctr[30];
  sprintf(command_ctr,"p[%d].b[%d].bco=%d",page,element_id,colour);
  serial_print_suffixed(command_ctr);
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}

void mNextionPanel::SetAttribute_FontColour(uint8_t page, uint8_t element_id, uint32_t colour){
  char command_ctr[30];
  sprintf(command_ctr,"p[%d].b[%d].pco=%d",page,element_id,colour);
  serial_print_suffixed(command_ctr);
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "nSetTxtAttr %s"),command_ctr);
}




void mNextionPanel::nextionSetAttr(const char* hmiAttribute, const char* hmiValue)
{ // Set the value of a Nextion component attribute
  //#define USE_NEXTION_SOFTWARE_SERIAL
  #ifdef USE_NEXTION_SOFTWARE_SERIAL
    swSer->print(hmiAttribute);
    swSer->print("=");
    swSer->print(utf8ascii2((char*)hmiValue));
    swSer->write(nextionSuffix, sizeof(nextionSuffix));
  #else
    SERIAL_NEXTION_TX.print(hmiAttribute);
    SERIAL_NEXTION_TX.print("=");
    SERIAL_NEXTION_TX.print(utf8ascii2((char*)hmiValue));
    SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  #endif
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "PHASEOUT USING, KEEP LEGACY, SET %s=%s"),hmiAttribute,hmiValue);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionGetAttr(const char* c_str)//String hmiAttribute)
{ // Get the value of a Nextion component attribute
  // This will only send the command to the panel requesting the attribute, the actual
  // return of that value will be handled by nextionProcessInput and placed into mqttGetSubtopic

  char hmiattribute_ctr[100]; 
  memset(hmiattribute_ctr,0,sizeof(hmiattribute_ctr));
  sprintf(hmiattribute_ctr,"get %s",c_str);
  serial_print_suffixed(hmiattribute_ctr);
  
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX " GET 'get %s'"),hmiattribute_ctr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionSendCmd(const char* c_str)
{ // Send a raw command to the Nextion panel
  serial_print(utf8ascii(c_str));
  serial_print_suffix();
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX " %s"),c_str);
}



/**********************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 * Useful Commands ******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 **********************************************************************************************************************************************************************************/
 

void mNextionPanel::Command_SetPage(uint8_t page){
  settings.page = page;
  char ctr[10];
  sprintf(ctr,"page %d",page);
  nextionSendCmd(ctr);
}

void mNextionPanel::Command_SetBrightness(uint8_t brightness_percentage){
  char ctr[10];
  sprintf(ctr,"dims=%d",brightness_percentage); //dims range 0-100
  nextionSendCmd(ctr);
  settings.brightness_percentage = brightness_percentage;
}

void mNextionPanel::Command_SetBrightness255(uint8_t brightness){
  settings.brightness_percentage = map(brightness,0,255,0,100);
  Command_SetBrightness(settings.brightness_percentage);
}

void mNextionPanel::Command_ToggleBrightness(){  
  Command_SetBrightness(settings.brightness_percentage>0?0:settings.brightness_percentage);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::espWifiSetup()
{ // Connect to WiFi
  nextionSendCmd("page 0");
  nextionSetAttr("p[0].b[1].txt", "\"WiFi Connecting\"");
}


// UTF8-Decoder: convert UTF8-String to extended ASCII http://playground.arduino.cc/main/Utf8ascii
// Workaround for issue here: https://github.com/home-assistant/home-assistant/issues/9528
// Nextion claims that "Unicode and UTF will not be among the supported encodings", so this should
// be safe to run against all attribute values coming in.
byte mNextionPanel::utf8ascii(byte ascii)
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

String mNextionPanel::utf8ascii(String s)
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

void mNextionPanel::utf8ascii(char *s)
{ // In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
  uint16_t k = 0;
  char c;
  for (uint16_t i = 0; i < strlen(s); i++)
  {
    c = utf8ascii(s[i]);
    if (c != 0)
      s[k++] = c;
  }
  s[k] = 0;
}

char* mNextionPanel::utf8ascii2(char *s)
{ // In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
  uint16_t k = 0;
  char c;
  for (uint16_t i = 0; i < strlen(s); i++)
  {
    c = utf8ascii(s[i]);
    if (c != 0)
      s[k++] = c;
  }
  s[k] = 0;
  return s;
}




/**********************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 * TIMED FUNCTIONS ******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 **********************************************************************************************************************************************************************************/


void mNextionPanel::EverySecond_FlashScreen(){

  if(flash_message.cShowSeconds==0){
    // Return screen to previous
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION D_JSON_COMMAND_NVALUE),"settings.page_saved",settings.page_saved);
    Command_SetPage(settings.page_saved);
    flash_message.cShowSeconds = -1;
  }else
  if(flash_message.cShowSeconds>0){
    flash_message.cShowSeconds--;
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION D_JSON_COMMAND_NVALUE),"flash_message.cShowSeconds",flash_message.cShowSeconds);
  }

} //end F

// https://seithan.com/Easy-Nextion-Library/Use-Nextion-General-View/
void mNextionPanel::EverySecond_SendScreenInfo(){

  char rtc_ctr[40]; memset(rtc_ctr,'\0',sizeof(rtc_ctr));
  sprintf(rtc_ctr, "%02d:%02d:%02d\n\r",
  pCONT_time->RtcTime.hour,pCONT_time->RtcTime.minute,pCONT_time->RtcTime.second);

  SetAttribute_Txt(settings.page,1,rtc_ctr);
  
  // char health_ctr[40]; memset(health_ctr,'\0',sizeof(health_ctr));
  // sprintf(health_ctr, "%c%c%c %d",
  //   WiFi.status() == WL_CONNECTED ? 'N' : 'n',
  //   pCONT_mqtt->pubsub->connected() ? 'M' : 'm',
  //   fOpenHABDataStreamActive ? 'O' : 'o',
  //   0
  //   // pCONT_wif->WifiGetRssiAsQuality(WiFi.RSSI())
  // );

  // Serial.println(health_ctr);

  // SetAttribute_Txt(settings.page,5,health_ctr);

}


#endif