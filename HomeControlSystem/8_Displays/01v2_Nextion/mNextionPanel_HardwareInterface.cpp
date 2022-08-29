
#include "mNextionPanel.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION_V2

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


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void nextionReset()
// {
//   debugPrintln(F("HMI: Rebooting LCD"));
//   digitalWrite(nextionResetPin, LOW);
//   Serial1.print("rest");
//   Serial1.write(nextionSuffix, sizeof(nextionSuffix));
//   Serial1.flush();
//   delay(100);
//   digitalWrite(nextionResetPin, HIGH);

//   unsigned long lcdResetTimer = millis();
//   const unsigned long lcdResetTimeout = 5000;

//   lcdConnected = false;
//   while (!lcdConnected && (millis() < (lcdResetTimer + lcdResetTimeout)))
//   {
//     nextionHandleInput();
//   }
//   if (lcdConnected)
//   {
//     debugPrintln(F("HMI: Rebooting LCD completed"));
//     if (nextionActivePage)
//     {
//       nextionSendCmd("page " + String(nextionActivePage));
//     }
//   }
//   else
//   {
//     debugPrintln(F("ERROR: Rebooting LCD completed, but LCD is not responding."));
//   }
//   mqttClient.publish(mqttStatusTopic, "OFF", true, 0);
//   debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + String(F("' : 'OFF'")));
// }



// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void espReset()
// {
//   debugPrintln(F("RESET: HASPone reset"));
//   if (mqttClient.connected())
//   {
//     mqttClient.publish(mqttStateJSONTopic, String(F("{\"event_type\":\"hasp_device\",\"event\":\"offline\"}")));
//     debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F(" : {\"event_type\":\"hasp_device\",\"event\":\"offline\"}")));
//     mqttClient.publish(mqttStatusTopic, "OFF", true, 0);
//     mqttClient.disconnect();
//     debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + String(F("' : 'OFF'")));
//   }
//   debugPrintln(F("HMI: Rebooting LCD"));
//   digitalWrite(nextionResetPin, LOW);
//   Serial1.print("rest");
//   Serial1.write(nextionSuffix, sizeof(nextionSuffix));
//   Serial1.flush();
//   delay(500);
//   ESP.reset();
//   delay(5000);
// }


////////////////////////////////////////////////////////////////////////////////////////////////////
bool mNextionPanel::nextionConnect()
{


  // if ((millis() - nextionCheckTimer) >= nextionCheckInterval)
  // {
  //   static unsigned int nextionRetryCount = 0;
  //   if ((nextionModel.length() == 0) && (nextionRetryCount < (nextionRetryMax - 2)))
  //   { // Try issuing the "connect" command a few times
  //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion connect request"));
  //     nextionSendCmd("connect");
  //     nextionRetryCount++;
  //     nextionCheckTimer = millis();
  //   }
  //   else if ((nextionModel.length() == 0) && (nextionRetryCount < nextionRetryMax))
  //   { // If we still don't have model info, try to change nextion serial speed from 9600 to 115200
  //     nextionSetSpeed();
  //     nextionRetryCount++;
  //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion serial speed 115200 request"));
  //     nextionCheckTimer = millis();
  //   }
  //   else if ((lcdVersion < 1) && (nextionRetryCount <= nextionRetryMax))
  //   {
  //     if (nextionModel.length() == 0)
  //     { // one last hail mary, maybe the serial speed is set correctly now
  //       nextionSendCmd("connect");
  //     }
  //     //nextionSendCmd("get " + lcdVersionQuery.toString().c_str());
  //     lcdVersionQueryFlag = true;
  //     nextionRetryCount++;
  //     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "sending Nextion version query"));
  //     nextionCheckTimer = millis();
  //   }
  // }


  memset(nextionSuffix,0xFF,sizeof(nextionSuffix));

  const unsigned long nextionCheckTimeout = 2000; // Max time in msec for nextion connection check
  unsigned long nextionCheckTimer = millis();     // Timer for nextion connection checks

  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));

  nextionSendCmd("connect"); // 
  // S:comok 1,30601-0,NX3224T024_011R,163,61488,DE6064B7E70C6521,4194304ÿÿÿ // response from connect


  if (!lcdConnected)
  { // Check for some traffic from our LCD
    debugPrintln(F("HMI: Waiting for LCD connection"));
    while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && !lcdConnected)
    {
      nextionHandleInput();
    }
  }

  if (!lcdConnected)
  { // No response from the display using the configured speed, so scan all possible speeds
    nextionSetSpeed();

    nextionCheckTimer = millis(); // Reset our timer
    debugPrintln(F("HMI: Waiting again for LCD connection"));
    while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && !lcdConnected)
    {
      SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
      nextionHandleInput();
    }
    if (!lcdConnected)
    {
      debugPrintln(F("HMI: LCD connection timed out"));
      return false;
    }
  }

  // Query backlight status.  This should always succeed under simulation or non-HASPone HMI
  lcdBacklightQueryFlag = true;
  debugPrintln(F("HMI: Querying LCD backlight status"));
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  nextionSendCmd("get dim");
  while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && lcdBacklightQueryFlag)
  {
    nextionHandleInput();
  }
  if (lcdBacklightQueryFlag)
  { // Our flag is still set, meaning we never got a response.
    debugPrintln(F("HMI: LCD backlight query timed out"));
    lcdBacklightQueryFlag = false;
    return false;
  }

  // We are now communicating with the panel successfully.  Enable ACK checking for all future commands.
  nextionAckEnable = true;
  nextionSendCmd("bkcmd=3");

  // This check depends on the HMI having been designed with a version number in the object
  // defined in lcdVersionQuery.  It's OK if this fails, it just means the HMI project is
  // not utilizing the version capability that the HASPone project makes use of.
  lcdVersionQueryFlag = true;
  debugPrintln(F("HMI: Querying LCD firmware version number"));
  nextionSendCmd_String("get " + lcdVersionQuery);
  while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && lcdVersionQueryFlag)
  {
    nextionHandleInput();
  }
  if (lcdVersionQueryFlag)
  { // Our flag is still set, meaning we never got a response.  This should only happen if
    // there's a problem.  Non-HASPone projects should pass this check with lcdVersion = 0
    debugPrintln(F("HMI: LCD version query timed out"));
    lcdVersionQueryFlag = false;
    return false;
  }

  if (nextionModel.length() == 0)
  { // Check for LCD model via `connect`.  The Nextion simulator does not support this command,
    // so if we're running under that environment this process should timeout.
    debugPrintln(F("HMI: Querying LCD model information"));
    nextionSendCmd("connect");
    while (((millis() - nextionCheckTimer) <= nextionCheckTimeout) && (nextionModel.length() == 0))
    {
      nextionHandleInput();
    }
  }
  return true;


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
  // SERIAL_NEXTION_TX.begin(9600);
  // SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  // SERIAL_NEXTION_TX.print("bauds=38400");
  // SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  // SERIAL_NEXTION_TX.flush();
  // SERIAL_NEXTION_TX.begin(NEXTION_BAUD);

  
  // SERIAL_NEXTION_TX.begin(115200);
  // SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  // SERIAL_NEXTION_TX.print("bauds=38400");
  // SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  // SERIAL_NEXTION_TX.flush();
  // SERIAL_NEXTION_TX.begin(NEXTION_BAUD);

  #endif

}

// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void nextionSetSpeed()
// {
//   debugPrintln(String(F("HMI: No Nextion response, attempting to set serial speed to ")) + String(nextionBaud));
//   for (unsigned int nextionSpeedsIndex = 0; nextionSpeedsIndex < nextionSpeedsLength; nextionSpeedsIndex++)
//   {
//     debugPrintln(String(F("HMI: Sending bauds=")) + String(nextionBaud) + " @" + String(nextionSpeeds[nextionSpeedsIndex]) + " baud");
//     Serial1.flush();
//     Serial1.begin(nextionSpeeds[nextionSpeedsIndex]);
//     Serial1.write(nextionSuffix, sizeof(nextionSuffix));
//     Serial1.write(nextionSuffix, sizeof(nextionSuffix));
//     Serial1.write(nextionSuffix, sizeof(nextionSuffix));
//     Serial1.print("bauds=" + String(nextionBaud));
//     Serial1.write(nextionSuffix, sizeof(nextionSuffix));
//     Serial1.flush();
//   }
//   Serial1.begin(atoi(nextionBaud));
// }




// ////////////////////////////////////////////////////////////////////////////////////////////////////
// bool mNextionPanel::nextionHandleInput()
// { // Handle incoming serial data from the Nextion panel
//   // This will collect serial data from the panel and place it into the global buffer
//   // nextionReturnBuffer[nextionReturnIndex]
//   // Return: true if we've received a string of 3 consecutive 0xFF values
//   // Return: false otherwise
//   bool nextionCommandComplete = false;
//   static int nextionTermByteCnt = 0;   // counter for our 3 consecutive 0xFFs
//   static String hmiDebug = ""; // assemble a string for debug output

//   if (serial_available())
//   {
//     // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_NEXTION " if (Serial.available())"));
//     lcdConnected = true;
//     byte nextionCommandByte = serial_read();

//     hmiDebug += (" 0x" + String(nextionCommandByte, HEX));
//     // check to see if we have one of 3 consecutive 0xFF which indicates the end of a command
//     if (nextionCommandByte == 0xFF)
//     {
//       nextionTermByteCnt++;
//       if (nextionTermByteCnt >= 3)
//       { // We have received a complete command
//         nextionCommandComplete = true;
//         nextionTermByteCnt = 0; // reset counter
//       }
//     }
//     else
//     {
//       nextionTermByteCnt = 0; // reset counter if a non-term byte was encountered
//     }
//     nextionReturnBuffer[nextionReturnIndex] = nextionCommandByte;
//     nextionReturnIndex++;
//   }
//   if (nextionCommandComplete)
//   {
//   //  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " %s"),
//     //Serial.println(ConvertBytetoASCII(nextionReturnBuffer,nextionReturnIndex));
//     // for(int i=0;i<nextionReturnIndex;i++){
//     //   AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX "%d | %s"),i,String(nextionReturnBuffer[i], HEX));
//     // }
//     AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX " %s"),hmiDebug.c_str());
//     hmiDebug = "";

    
// DEBUG_LINE_HERE;
// Serial.println(hmiDebug);

//   }



//   return nextionCommandComplete;
// }



////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionHandleInput()
{ // Handle incoming serial data from the Nextion panel
  // This will collect serial data from the panel and place it into the global buffer
  // nextionReturnBuffer[nextionReturnIndex]
  unsigned long handlerTimeout = millis() + 100;
  bool nextionCommandComplete = false;
  static uint8_t nextionTermByteCnt = 0; // counter for our 3 consecutive 0xFFs

  while (SERIAL_NEXTION_RX.available() && !nextionCommandComplete && (millis() < handlerTimeout))
  {
    byte nextionCommandByte = SERIAL_NEXTION_RX.read();
    Serial.write(nextionCommandByte);
    if (nextionCommandByte == 0xFF)
    { // check to see if we have one of 3 consecutive 0xFF which indicates the end of a command
      nextionTermByteCnt++;
      if (nextionTermByteCnt >= 3)
      { // We have received a complete command
        lcdConnected = true;
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
    if (nextionCommandComplete)
    {
      nextionAckReceived = true;
      nextionProcessInput();
    }
    yield();
  }


  if (millis() > handlerTimeout)
  {
    debugPrintln(String(F("HMI ERROR: nextionHandleInput timeout")));
  }
}




////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionProcessInput()
{ // Process incoming serial commands from the Nextion panel
  // Command reference: https://www.itead.cc/wiki/Nextion_Instruction_Set#Format_of_Device_Return_Data
  // tl;dr, command byte, command data, 0xFF 0xFF 0xFF

  Serial.println("nextionProcessInput");
  DEBUG_LINE_HERE;
  
  char event_ctr[30];
  //https://www.itead.cc/wiki/Nextion_Instruction_Set

  // first instructions byte
  switch(nextionReturnBuffer[0]){
    case NEXTION_COMMAND_INVALID_INSTRUCTION:
      //AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_RX D_NEXTION_COMMAND D_NEXTION_COMMAND_INVALID_INSTRUCTION_CTR));   
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
  DEBUG_LINE_HERE;
  
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
  DEBUG_LINE_HERE;
  
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
  { 
  DEBUG_LINE_HERE;
  // Handle incoming "sendme" page number
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
  { 
  DEBUG_LINE_HERE;
  // Handle touch coordinate data
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
  { 
  DEBUG_LINE_HERE;
  // Handle get string return
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
  { 
  DEBUG_LINE_HERE;
  // Handle get int return
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
  DEBUG_LINE_HERE;
      //mqttClient.publish(mqttStateTopic, getString);
      Serial.println(getString);
      pCONT_mqtt->ppublish("status/nextion/event9",getString.c_str(),0);
    }
    // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
    else
    {
  DEBUG_LINE_HERE;
      String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
      //mqttClient.publish(mqttReturnTopic, getString);
      pCONT_mqtt->ppublish("status/nextion/event10",getString.c_str(),0);
      String mqttButtonJSONEvent = String(F("{\"event\":\"")) + mqttGetSubtopicJSON + String(F("\", \"value\":")) + getString + String(F("}"));
      //mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
      pCONT_mqtt->ppublish("status/nextion/event11",mqttButtonJSONEvent.c_str(),0);
      mqttGetSubtopic = "";
    }
  DEBUG_LINE_HERE;

  }
  else if (nextionReturnBuffer[0] == 0x63 && nextionReturnBuffer[1] == 0x6f && nextionReturnBuffer[2] == 0x6d && nextionReturnBuffer[3] == 0x6f && nextionReturnBuffer[4] == 0x6b)
  { 
  DEBUG_LINE_HERE;
  // Catch 'comok' response to 'connect' command: https://www.itead.cc/blog/nextion-hmi-upload-protocol

    ALOG_INF(PSTR("Catch 'comok' response to 'connect' command"));

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

DEBUG_LINE_HERE;

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


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void nextionProcessInput()
// { // Process complete incoming serial command from the Nextion panel
//   // Command reference: https://www.itead.cc/wiki/Nextion_Instruction_Set#Format_of_Device_Return_Data
//   // tl;dr: command byte, command data, 0xFF 0xFF 0xFF

//   if (nextionReturnBuffer[0] == 0x01)
//   { // 	Instruction Successful - quietly ignore this as it will be returned after every command issued,
//     //  and processing it + spitting out serial output is a huge drag on performance if serial debug is enabled.

//     // debugPrintln(String(F("HMI IN: [Instruction Successful] 0x")) + String(nextionReturnBuffer[0], HEX));
//     // if (mqttClient.connected())
//     // {
//     //   String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Instruction Successful\"}"));
//     //   mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//     //   debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     // }
//     nextionReturnIndex = 0; // Done handling the buffer, reset index back to 0
//     return;                 // skip the rest of the tests below and return immediately
//   }

//   debugPrintln(String(F("HMI IN: [")) + String(nextionReturnIndex) + String(F(" bytes]: ")) + printHex8(nextionReturnBuffer, nextionReturnIndex));

//   if (nextionReturnBuffer[0] == 0x00 && nextionReturnBuffer[1] == 0x00 && nextionReturnBuffer[2] == 0x00)
//   { // Nextion Startup
//     debugPrintln(String(F("HMI IN: [Nextion Startup] 0x00 0x00 0x00")));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x00 0x00 0x00\",\"return_code_description\":\"Nextion Startup\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x00)
//   { // Invalid Instruction
//     debugPrintln(String(F("HMI IN: [Invalid Instruction] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Instruction\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x02)
//   { // Invalid Component ID
//     debugPrintln(String(F("HMI IN: [Invalid Component ID] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Component ID\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x03)
//   { // Invalid Page ID
//     debugPrintln(String(F("HMI IN: [Invalid Page ID] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Page ID\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x04)
//   { // Invalid Picture ID
//     debugPrintln(String(F("HMI IN: [Invalid Picture ID] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Picture ID\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x05)
//   { // Invalid Font ID
//     debugPrintln(String(F("HMI IN: [Invalid Font ID	] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Font ID	\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x06)
//   { // Invalid File Operation
//     debugPrintln(String(F("HMI IN: [Invalid File Operation] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid File Operation\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x09)
//   { // Invalid CRC
//     debugPrintln(String(F("HMI IN: [Invalid CRC] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid CRC\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x11)
//   { // Invalid Baud rate Setting
//     debugPrintln(String(F("HMI IN: [Invalid Baud rate Setting] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Baud rate Setting\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x12)
//   { // Invalid Waveform ID or Channel #
//     debugPrintln(String(F("HMI IN: [Invalid Waveform ID or Channel #] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Waveform ID or Channel #\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1A)
//   { // Invalid Variable name or attribute
//     debugPrintln(String(F("HMI IN: [Invalid Variable name or attribute] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Variable name or attribute\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1B)
//   { // Invalid Variable Operation
//     debugPrintln(String(F("HMI IN: [Invalid Variable Operation] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Variable Operation\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1C)
//   { // Assignment failed to assign
//     debugPrintln(String(F("HMI IN: [Assignment failed to assign] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Assignment failed to assign\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1D)
//   { // EEPROM Operation failed
//     debugPrintln(String(F("HMI IN: [EEPROM Operation failed] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"EEPROM Operation failed\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1E)
//   { // Invalid Quantity of Parameters
//     debugPrintln(String(F("HMI IN: [Invalid Quantity of Parameters] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Invalid Quantity of Parameters\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x1F)
//   { // IO Operation failed
//     debugPrintln(String(F("HMI IN: [IO Operation failed] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"IO Operation failed\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x20)
//   { // Escape Character Invalid
//     debugPrintln(String(F("HMI IN: [Escape Character Invalid] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Escape Character Invalid\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x23)
//   { // Variable name too long
//     debugPrintln(String(F("HMI IN: [Variable name too long] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Variable name too long\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x24)
//   { // Serial Buffer Overflow
//     debugPrintln(String(F("HMI IN: [Serial Buffer Overflow] 0x")) + String(nextionReturnBuffer[0], HEX));
//     if (mqttClient.connected())
//     {
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextion_return_data\",\"return_code\":\"0x")) + String(nextionReturnBuffer[0], HEX) + String(F("\",\"return_code_description\":\"Serial Buffer Overflow\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }

//   else if (nextionReturnBuffer[0] == 0x65)
//   { // Handle incoming touch command
//     // 0x65+Page ID+Component ID+TouchEvent+End
//     // Return this data when the touch event created by the user is pressed.
//     // Definition of TouchEvent: Press Event 0x01, Release Event 0X00
//     // Example: 0x65 0x00 0x02 0x01 0xFF 0xFF 0xFF
//     // Meaning: Touch Event, Page 0, Object 2, Press
//     String nextionPage = String(nextionReturnBuffer[1]);
//     String nextionButtonID = String(nextionReturnBuffer[2]);
//     byte nextionButtonAction = nextionReturnBuffer[3];

//     if (nextionButtonAction == 0x01)
//     {
//       debugPrintln(String(F("HMI IN: [Button ON] 'p[")) + nextionPage + "].b[" + nextionButtonID + "]'");
//       if (mqttClient.connected())
//       {
//         // Only process touch events if screen backlight is on and configured to do so.
//         if (ignoreTouchWhenOff && !lcdBacklightOn)
//         {
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_press_disabled\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"ON\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         }
//         else
//         {
//           String mqttButtonTopic = mqttStateTopic + "/p[" + nextionPage + "].b[" + nextionButtonID + "]";
//           mqttClient.publish(mqttButtonTopic, "ON");
//           debugPrintln(String(F("MQTT OUT: '")) + mqttButtonTopic + "' : 'ON'");
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_press\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"ON\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         }
//       }
//       if (beepEnabled)
//       {
//         beepOnTime = 500;
//         beepOffTime = 100;
//         beepCounter = 1;
//       }
//       if (rebootOnp0b1 && (nextionPage == "0") && (nextionButtonID == "1"))
//       {
//         debugPrintln(String(F("HMI IN: p[0].b[1] pressed during HASPone configuration, rebooting.")));
//         espReset();
//       }
//     }
//     else if (nextionButtonAction == 0x00)
//     {
//       debugPrintln(String(F("HMI IN: [Button OFF] 'p[")) + nextionPage + "].b[" + nextionButtonID + "]'");
//       if (mqttClient.connected())
//       {
//         // Only process touch events if screen backlight is on and configured to do so.
//         if (ignoreTouchWhenOff && !lcdBacklightOn)
//         {
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_release_disabled\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"ON\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         }
//         else
//         {
//           String mqttButtonTopic = mqttStateTopic + "/p[" + nextionPage + "].b[" + nextionButtonID + "]";
//           mqttClient.publish(mqttButtonTopic, "OFF");
//           debugPrintln(String(F("MQTT OUT: '")) + mqttButtonTopic + "' : 'OFF'");
//           String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_release\",\"event\":\"p[")) + nextionPage + String(F("].b[")) + nextionButtonID + String(F("]\",\"value\":\"OFF\"}"));
//           mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//           debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//           // Now see if this object has a .val that might have been updated.  Works for sliders,
//           // two-state buttons, etc, returns 0 for normal buttons
//           mqttGetSubtopic = "/p[" + nextionPage + "].b[" + nextionButtonID + "].val";
//           // This right here is dicey.  We're done w/ this command so reset the index allowing this to be kinda-reentrant
//           // because the call to nextionGetAttr is going to call us back.
//           nextionReturnIndex = 0;
//           nextionGetAttr("p[" + nextionPage + "].b[" + nextionButtonID + "].val");
//         }
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x66)
//   { // Handle incoming "sendme" page number
//     // 0x66+PageNum+End
//     // Example: 0x66 0x02 0xFF 0xFF 0xFF
//     // Meaning: page 2
//     String nextionPage = String(nextionReturnBuffer[1]);
//     debugPrintln(String(F("HMI IN: [sendme Page] '")) + nextionPage + String(F("'")));
//     if ((nextionPage != "0") || nextionReportPage0)
//     { // If we have a new page AND ( (it's not "0") OR (we've set the flag to report 0 anyway) )

//       if (mqttClient.connected())
//       {
//         String mqttButtonJSONEvent = String(F("{\"event\":\"page\",\"value\":")) + nextionPage + String(F("}"));
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         String mqttPageTopic = mqttStateTopic + "/page";
//         debugPrintln(String(F("MQTT OUT: '")) + mqttPageTopic + String(F("' : '")) + nextionPage + String(F("'")));
//         mqttClient.publish(mqttPageTopic, nextionPage, false, 0);
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x67 || nextionReturnBuffer[0] == 0x68)
//   { // Handle touch coordinate data
//     // 0X67+Coordinate X High+Coordinate X Low+Coordinate Y High+Coordinate Y Low+TouchEvent+End
//     // Example: 0X67 0X00 0X7A 0X00 0X1E 0X01 0XFF 0XFF 0XFF
//     // Meaning: Coordinate (122,30), Touch Event: Press
//     // issue Nextion command "sendxy=1" to enable this output
//     // 0x68 is the same, but returned when the screen touch has awakened the screen from sleep
//     uint16_t xCoord = nextionReturnBuffer[1];
//     xCoord = xCoord * 256 + nextionReturnBuffer[2];
//     uint16_t yCoord = nextionReturnBuffer[3];
//     yCoord = yCoord * 256 + nextionReturnBuffer[4];
//     String xyCoord = String(xCoord) + String(',') + String(yCoord);
//     byte nextionTouchAction = nextionReturnBuffer[5];
//     if (nextionTouchAction == 0x01)
//     {
//       debugPrintln(String(F("HMI IN: [Touch ON] '")) + xyCoord + String(F("'")));
//       if (mqttClient.connected())
//       {
//         String mqttTouchTopic = mqttStateTopic + "/touchOn";
//         mqttClient.publish(mqttTouchTopic, xyCoord);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttTouchTopic + String(F("' : '")) + xyCoord + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_press\",\"event\":\"touchxy\",\"touch_event\":\"ON\",\"touchx\":\"")) + String(xCoord) + String(F("\",\"touchy\":\"")) + String(yCoord) + String(F("\",\"screen_state\":\""));
//         if (nextionReturnBuffer[0] == 0x67)
//         {
//           mqttButtonJSONEvent += "awake\"}";
//         }
//         else
//         {
//           mqttButtonJSONEvent += "asleep\"}";
//         }
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//       }
//     }
//     else if (nextionTouchAction == 0x00)
//     {
//       debugPrintln(String(F("HMI IN: [Touch OFF] '")) + xyCoord + String(F("'")));
//       if (mqttClient.connected())
//       {
//         String mqttTouchTopic = mqttStateTopic + "/touchOff";
//         mqttClient.publish(mqttTouchTopic, xyCoord);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttTouchTopic + String(F("' : '")) + xyCoord + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event_type\":\"button_short_press\",\"event\":\"touchxy\",\"touch_event\":\"OFF\",\"touchx\":\"")) + String(xCoord) + String(F("\",\"touchy\":\"")) + String(yCoord) + String(F("\",\"screen_state\":\""));
//         if (nextionReturnBuffer[0] == 0x67)
//         {
//           mqttButtonJSONEvent += "awake\"}";
//         }
//         else
//         {
//           mqttButtonJSONEvent += "asleep\"}";
//         }
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x70)
//   { // Handle get string return
//     // 0x70+ASCII string+End
//     // Example: 0x70 0x41 0x42 0x43 0x44 0x31 0x32 0x33 0x34 0xFF 0xFF 0xFF
//     // Meaning: String data, ABCD1234
//     String getString;
//     for (int i = 1; i < nextionReturnIndex - 3; i++)
//     { // convert the payload into a string
//       getString += (char)nextionReturnBuffer[i];
//     }
//     debugPrintln(String(F("HMI IN: [String Return] '")) + getString + String(F("'")));
//     if (mqttClient.connected())
//     {
//       if (mqttGetSubtopic == "")
//       { // If there's no outstanding request for a value, publish to mqttStateTopic
//         mqttClient.publish(mqttStateTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateTopic + String(F("' : '")) + getString + String(F("'")));
//       }
//       else
//       { // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
//         String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
//         mqttClient.publish(mqttReturnTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttReturnTopic + String(F("' : '")) + getString + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event\":\"")) + mqttGetSubtopic.substring(1) + String(F("\",\"value\":\"")) + getString + String(F("\"}"));
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//         mqttGetSubtopic = "";
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x71)
//   { // Handle get int return
//     // 0x71+byte1+byte2+byte3+byte4+End (4 byte little endian)
//     // Example: 0x71 0x7B 0x00 0x00 0x00 0xFF 0xFF 0xFF
//     // Meaning: Integer data, 123
//     long getInt = nextionReturnBuffer[4];
//     getInt = getInt * 256 + nextionReturnBuffer[3];
//     getInt = getInt * 256 + nextionReturnBuffer[2];
//     getInt = getInt * 256 + nextionReturnBuffer[1];
//     String getString = String(getInt);
//     debugPrintln(String(F("HMI IN: [Int Return] '")) + getString + String(F("'")));

//     if (lcdVersionQueryFlag)
//     {
//       lcdVersion = getInt;
//       lcdVersionQueryFlag = false;
//       debugPrintln(String(F("HMI IN: lcdVersion '")) + String(lcdVersion) + String(F("'")));
//     }
//     else if (lcdBacklightQueryFlag)
//     {
//       lcdBacklightDim = getInt;
//       lcdBacklightQueryFlag = false;
//       if (lcdBacklightDim > 0)
//       {
//         lcdBacklightOn = 1;
//       }
//       else
//       {
//         lcdBacklightOn = 0;
//       }
//       debugPrintln(String(F("HMI IN: lcdBacklightDim '")) + String(lcdBacklightDim) + String(F("'")));
//     }
//     else if (mqttGetSubtopic == "")
//     {
//       if (mqttClient.connected())
//       {
//         mqttClient.publish(mqttStateTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateTopic + String(F("' : '")) + getString + String(F("'")));
//       }
//     }
//     // Otherwise, publish the to saved mqttGetSubtopic and then reset mqttGetSubtopic
//     else
//     {
//       if (mqttClient.connected())
//       {
//         String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
//         mqttClient.publish(mqttReturnTopic, getString);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttReturnTopic + String(F("' : '")) + getString + String(F("'")));
//         String mqttButtonJSONEvent = String(F("{\"event\":\"")) + mqttGetSubtopic.substring(1) + String(F("\",\"value\":")) + getString + String(F("}"));
//         mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//       }
//       mqttGetSubtopic = "";
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x63 && nextionReturnBuffer[1] == 0x6f && nextionReturnBuffer[2] == 0x6d && nextionReturnBuffer[3] == 0x6f && nextionReturnBuffer[4] == 0x6b)
//   { // Catch 'comok' response to 'connect' command: https://www.itead.cc/blog/nextion-hmi-upload-protocol
//     String comokField;
//     uint8_t comokFieldCount = 0;
//     byte comokFieldSeperator = 0x2c; // ","

//     for (uint8_t i = 0; i <= nextionReturnIndex; i++)
//     { // cycle through each byte looking for our field seperator
//       if (nextionReturnBuffer[i] == comokFieldSeperator)
//       { // Found the end of a field, so do something with it.  Maybe.
//         if (comokFieldCount == 2)
//         {
//           nextionModel = comokField;
//           debugPrintln(String(F("HMI IN: nextionModel: ")) + nextionModel);
//         }
//         comokFieldCount++;
//         comokField = "";
//       }
//       else
//       {
//         comokField += String(char(nextionReturnBuffer[i]));
//       }
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x86)
//   { // Returned when Nextion enters sleep automatically. Using sleep=1 will not return an 0x86
//     // 0x86+End
//     if (mqttClient.connected())
//     {
//       lcdBacklightOn = 0;
//       mqttClient.publish(mqttLightStateTopic, "OFF", true, 1);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'OFF'")));
//       String mqttButtonJSONEvent = String(F("{\"event\":\"sleep\",\"value\":\"ON\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x87)
//   { // Returned when Nextion leaves sleep automatically. Using sleep=0 will not return an 0x87
//     // 0x87+End
//     if (mqttClient.connected())
//     {
//       lcdBacklightOn = 1;
//       mqttClient.publish(mqttLightStateTopic, "ON", true, 1);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'ON'")));
//       mqttClient.publish(mqttLightBrightStateTopic, String(lcdBacklightDim), true, 1);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttLightBrightStateTopic + String(F("' : ")) + String(lcdBacklightDim));
//       String mqttButtonJSONEvent = String(F("{\"event\":\"sleep\",\"value\":\"OFF\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else if (nextionReturnBuffer[0] == 0x88)
//   { // Returned when Nextion powers on
//     // 0x88+End
//     debugPrintln(F("HMI: Nextion panel connected."));
//   }
//   nextionReturnIndex = 0; // Done handling the buffer, reset index back to 0
// }

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
  // DEBUG_LINE_HERE_PAUSE;
    SERIAL_NEXTION_TX.print(hmiAttribute);
    SERIAL_NEXTION_TX.print("=");
  // DEBUG_LINE_HERE_PAUSE;
    // SERIAL_NEXTION_TX.print(utf8ascii((char*)hmiValue));
    SERIAL_NEXTION_TX.print(utf8ascii((String)hmiValue));
  // DEBUG_LINE_HERE_PAUSE;
    SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  #endif
  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_NEXTION D_NEXTION_TX "PHASEOUT USING, KEEP LEGACY, SET %s=%s"),hmiAttribute,hmiValue);
}


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void nextionSetAttr(const String &hmiAttribute, const String &hmiValue)
// { // Set the value of a Nextion component attribute
//   Serial1.print(hmiAttribute);
//   Serial1.print("=");
//   Serial1.print(hmiValue);
//   Serial1.write(nextionSuffix, sizeof(nextionSuffix));
//   Serial1.flush();
//   debugPrintln(String(F("HMI OUT: '")) + hmiAttribute + "=" + hmiValue + String(F("'")));
//   if (nextionAckEnable)
//   {
//     nextionAckReceived = false;
//     nextionAckTimer = millis();

//     while ((!nextionAckReceived) || (millis() - nextionAckTimer > nextionAckTimeout))
//     {
//       nextionHandleInput();
//     }
//     if (!nextionAckReceived)
//     {
//       debugPrintln(String(F("HMI ERROR: Nextion Ack timeout")));
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextionError\",\"value\":\"Nextion Ack timeout\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else
//   {
//     nextionHandleInput();
//   }
// }


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


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void nextionGetAttr(const String &hmiAttribute)
// { // Get the value of a Nextion component attribute
//   // This will only send the command to the panel requesting the attribute, the actual
//   // return of that value will be handled by nextionProcessInput and placed into mqttGetSubtopic
//   Serial1.print("get ");
//   Serial1.print(hmiAttribute);
//   Serial1.write(nextionSuffix, sizeof(nextionSuffix));
//   Serial1.flush();
//   debugPrintln(String(F("HMI OUT: 'get ")) + hmiAttribute + String(F("'")));
//   if (nextionAckEnable)
//   {
//     nextionAckReceived = false;
//     nextionAckTimer = millis();

//     while ((!nextionAckReceived) || (millis() - nextionAckTimer > nextionAckTimeout))
//     {
//       nextionHandleInput();
//     }
//     if (!nextionAckReceived)
//     {
//       debugPrintln(String(F("HMI ERROR: Nextion Ack timeout")));
//       String mqttButtonJSONEvent = String(F("{\"event\":\"nextionError\",\"value\":\"Nextion Ack timeout\"}"));
//       mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
//       debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     }
//   }
//   else
//   {
//     nextionHandleInput();
//   }
// }


////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionSendCmd(const char* c_str)
{ // Send a raw command to the Nextion panel
  serial_print(utf8ascii(c_str));
  serial_print_suffix();
  AddLog(settings.dynamic_log_level,PSTR(D_LOG_NEXTION D_NEXTION_TX " %s"),c_str);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void mNextionPanel::nextionSendCmd_String(const String &nextionCmd)
{ // Send a raw command to the Nextion panel
  SERIAL_NEXTION_TX.print(nextionCmd);
  SERIAL_NEXTION_TX.write(nextionSuffix, sizeof(nextionSuffix));
  SERIAL_NEXTION_TX.flush();
  debugPrintln(String(F("HMI OUT: ")) + nextionCmd);

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
      debugPrintln(String(F("HMI ERROR: Nextion Ack timeout")));
      String mqttButtonJSONEvent = String(F("{\"event\":\"nextionError\",\"value\":\"Nextion Ack timeout\"}"));
      // mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent);
      debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
    }
  }
  else
  {
    nextionHandleInput();
  }
}





void mNextionPanel::nextionSendCmd_ContainingFormattedText(const char* c_str)
{ // Send a raw command to the Nextion panel

//move format checks here
  char conversion_buffer[100] = {0};

  snprintf(conversion_buffer, sizeof(conversion_buffer), "%s", c_str);

  // AddLog(LOG_LEVEL_INFO, PSTR("before %s"),conversion_buffer);

  uint8_t command_length = strlen(conversion_buffer);  // use this later to check if "matched index + key + 3 data hue bytes" = "length" for vailidity check

  uint16_t converted_565_number = 0;
  uint16_t hue_input = 0;


  char* pos_start_of_token = nullptr;
  char* pos_start_of_response_field = nullptr;

  if((pos_start_of_token=strstr(conversion_buffer, "co=h"))!=nullptr) //pco and bco, returns pointer to substring
  {              
    // AddLog(LOG_LEVEL_INFO, PSTR("MATCHED %s"),pos_start_of_token);
    pos_start_of_response_field = pos_start_of_token+3; // Only 3, since "h" is not part of the output command

    hue_input = pCONT_sup->TextToInt(pos_start_of_response_field+1); //skipping h to numbers only

    float r,g,b;
    
    HueToRgb(hue_input, &r,&g,&b);

    uint8_t red = (int)r;
    uint8_t green = (int)g;
    uint8_t blue = (int)b;

    uint16_t Rgb565 = (((red & 0xf8)<<8) + ((green & 0xfc)<<3)+(blue>>3));


    // AddLog(LOG_LEVEL_INFO, PSTR("Rgb565 = %d"), Rgb565);


    sprintf(pos_start_of_response_field, "%d\0", Rgb565);

  }
  else
  if((pos_start_of_token=strstr(conversion_buffer, "co=#"))!=nullptr) //pco and bco, returns pointer to substring, "#" (capital) for byte packed rgb
  {               
    // AddLog(LOG_LEVEL_INFO, PSTR("MATCHED %s"),pos_start_of_token);
    pos_start_of_response_field = pos_start_of_token+3; // Only 3, since "h" is not part of the output command

    // hue_input = pCONT_sup->TextToInt(pos_start_of_response_field+1); //skipping h to numbers only

    
    // AddLog(LOG_LEVEL_INFO, PSTR("pos_start_of_response_field = %s"), pos_start_of_response_field);

    uint32_t colour32bit = 0;
    if(pos_start_of_response_field[0]=='#'){ colour32bit = (long) strtol( &pos_start_of_response_field[1], NULL, 16);
    }else{ colour32bit = (long) strtol( &pos_start_of_response_field[0], NULL, 16); }

    // RgbColor rgb;
    // rgb.R = colour32bit >> 16; //RGB
    // rgb.G = colour32bit >> 8 & 0xFF; //RGB
    // rgb.B = colour32bit & 0xFF; //RGB


    // float r,g,b;
    
    // HueToRgb(hue_input, &r,&g,&b);

    uint8_t red   = colour32bit >> 16; //RGB
    uint8_t green = colour32bit >> 8 & 0xFF; //RGB
    uint8_t blue  = colour32bit & 0xFF; //RGB

    uint16_t Rgb565 = (((red & 0xf8)<<8) + ((green & 0xfc)<<3)+(blue>>3));

// 
    // AddLog(LOG_LEVEL_INFO, PSTR("Rgb565 = %d"), Rgb565);


    sprintf(pos_start_of_response_field, "%d\0", Rgb565);

  }
  
  
  
  
  else{
    // AddLog(LOG_LEVEL_INFO, PSTR("NO match"));

  }

  nextionSendCmd(conversion_buffer);
}



/**********************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 * Useful Commands ******************************************************************************************************************************************************************************
 ******************************************************************************************************************************************************************************* 
 **********************************************************************************************************************************************************************************/
 

/**
 * @brief This function going forward should only send the command if the page is not already set, so will need a way to poll this
 * 
 * @param page 
 */
void mNextionPanel::Command_SetPage(uint8_t page){
  settings.page = page;
  char ctr[10];
  sprintf(ctr,"page %d",page);
  nextionSendCmd(ctr);
}

void mNextionPanel::Command_SetPageIfChanged(uint8_t page){
  if(settings.page == page) {return;}
  Command_SetPage(page);
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

// void mNextionPanel::utf8ascii(char *s)
// { // In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
//   uint16_t k = 0;
//   char c;
//   for (uint16_t i = 0; i < strlen(s); i++)
//   {
//     c = utf8ascii(s[i]);
//     if (c != 0)
//       s[k++] = c;
//   }
//   s[k] = 0;
// }

char* mNextionPanel::utf8ascii_Char(char *s)
{ // In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)

AddLog(LOG_LEVEL_ERROR, PSTR("mNextionPanel::utf8ascii2(char *s) crashing function"));

  // uint16_t k = 0;
  // char c;
  // for (uint16_t i = 0; i < strlen(s); i++)
  // {
  //   c = utf8ascii(s[i]);
  //   if (c != 0)
  //     s[k++] = c;
  // }
  // s[k] = 0;
  // return s;
  // return nullptr;
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


// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void mqttConnect()
// { // MQTT connection and subscriptions

//   static bool mqttFirstConnect = true; // For the first connection, we want to send an OFF/ON state to
//                                        // trigger any automations, but skip that if we reconnect while
//                                        // still running the sketch
//   rebootOnp0b1 = true;
//   static uint8_t mqttReconnectCount = 0;
//   unsigned long mqttConnectTimer = 0;
//   const unsigned long mqttConnectTimeout = 5000;

//   // Check to see if we have a broker configured and notify the user if not
//   if (strcmp(mqttServer, "") == 0)
//   {
//     nextionSendCmd("page 0");
//     nextionSetAttr("p[0].b[1].font", "6");
//     nextionSetAttr("p[0].b[1].txt", "\"WiFi Connected!\\r " + String(WiFi.SSID()) + "\\rIP: " + WiFi.localIP().toString() + "\\r\\rConfigure MQTT:\\rhttp://" + WiFi.localIP().toString() + "\"");
//     while (strcmp(mqttServer, "") == 0)
//     { // Handle other stuff while we're waiting for MQTT to be configured
//       yield();
//       nextionHandleInput();     // Nextion serial communications loop
//       ArduinoOTA.handle();      // Arduino OTA loop
//       webServer.handleClient(); // webServer loop
//       telnetHandleClient();     // telnet client loop
//       motionHandle();           // motion sensor loop
//       beepHandle();             // beep feedback loop
//     }
//   }

//   if (mqttTlsEnabled)
//   { // Create MQTT service object with TLS connection
//     mqttClient.begin(mqttServer, atoi(mqttPort), mqttClientSecure);
//     if (strcmp(mqttFingerprint, "") == 0)
//     {
//       debugPrintln(String(F("MQTT: Configuring MQTT TLS connection without fingerprint validation.")));
//       mqttClientSecure.setInsecure();
//     }
//     else
//     {
//       debugPrintln(String(F("MQTT: Configuring MQTT TLS connection with fingerprint validation.")));
//       mqttClientSecure.allowSelfSignedCerts();
//       mqttClientSecure.setFingerprint(mqttFingerprint);
//     }
//     mqttClientSecure.setBufferSizes(512, 512);
//   }
//   else
//   { // Create MQTT service object without TLS connection
//     debugPrintln(String(F("MQTT: Configuring MQTT connection without TLS.")));
//     mqttClient.begin(mqttServer, atoi(mqttPort), wifiClient);
//   }

//   mqttClient.onMessage(mqttProcessInput); // Setup MQTT callback function

//   // MQTT topic string definitions
//   mqttStateTopic = "hasp/" + String(haspNode) + "/state";
//   mqttStateJSONTopic = "hasp/" + String(haspNode) + "/state/json";
//   mqttCommandTopic = "hasp/" + String(haspNode) + "/command";
//   mqttGroupCommandTopic = "hasp/" + String(groupName) + "/command";
//   mqttStatusTopic = "hasp/" + String(haspNode) + "/status";
//   mqttSensorTopic = "hasp/" + String(haspNode) + "/sensor";
//   mqttLightCommandTopic = "hasp/" + String(haspNode) + "/light/switch";
//   mqttLightStateTopic = "hasp/" + String(haspNode) + "/light/state";
//   mqttLightBrightCommandTopic = "hasp/" + String(haspNode) + "/brightness/set";
//   mqttLightBrightStateTopic = "hasp/" + String(haspNode) + "/brightness/state";
//   mqttMotionStateTopic = "hasp/" + String(haspNode) + "/motion/state";

//   const String mqttCommandSubscription = mqttCommandTopic + "/#";
//   const String mqttGroupCommandSubscription = mqttGroupCommandTopic + "/#";
//   const String mqttLightSubscription = mqttLightCommandTopic + "/#";
//   const String mqttLightBrightSubscription = mqttLightBrightCommandTopic + "/#";

//   // Generate an MQTT client ID as haspNode + our MAC address
//   mqttClientId = String(haspNode) + "-" + String(espMac[0], HEX) + String(espMac[1], HEX) + String(espMac[2], HEX) + String(espMac[3], HEX) + String(espMac[4], HEX) + String(espMac[5], HEX);
//   nextionSendCmd("page 0");
//   nextionSetAttr("p[0].b[1].font", "6");
//   nextionSetAttr("p[0].b[1].txt", "\"WiFi Connected!\\r " + String(WiFi.SSID()) + "\\rIP: " + WiFi.localIP().toString() + "\\r\\rMQTT Connecting:\\r " + String(mqttServer) + "\"");
//   if (mqttTlsEnabled)
//   {
//     debugPrintln(String(F("MQTT: Attempting connection to broker ")) + String(mqttServer) + String(F(" on port ")) + String(mqttPort) + String(F(" with TLS enabled as clientID ")) + mqttClientId);
//   }
//   else
//   {
//     debugPrintln(String(F("MQTT: Attempting connection to broker ")) + String(mqttServer) + String(F(" on port ")) + String(mqttPort) + String(F(" with TLS disabled as clientID ")) + mqttClientId);
//   }

//   // Set keepAlive, cleanSession, timeout
//   mqttClient.setOptions(30, true, mqttConnectTimeout);

//   // declare LWT
//   mqttClient.setWill(mqttStatusTopic.c_str(), "OFF", true, 1);

//   while (!mqttClient.connected())
//   { // Loop until we're connected to MQTT
//     mqttConnectTimer = millis();
//     mqttClient.connect(mqttClientId.c_str(), mqttUser, mqttPassword, false);

//     if (mqttClient.connected())
//     { // Attempt to connect to broker, setting last will and testament
//       // Update panel with MQTT status
//       nextionSetAttr("p[0].b[1].txt", "\"WiFi Connected!\\r " + String(WiFi.SSID()) + "\\rIP: " + WiFi.localIP().toString() + "\\r\\rMQTT Connected:\\r " + String(mqttServer) + "\"");
//       debugPrintln(F("MQTT: connected"));

//       // Reset our diagnostic booleans
//       mqttPingCheck = true;
//       mqttPortCheck = true;

//       // Subscribe to our incoming topics
//       if (mqttClient.subscribe(mqttCommandSubscription))
//       {
//         debugPrintln(String(F("MQTT: subscribed to ")) + mqttCommandSubscription);
//       }
//       if (mqttClient.subscribe(mqttGroupCommandSubscription))
//       {
//         debugPrintln(String(F("MQTT: subscribed to ")) + mqttGroupCommandSubscription);
//       }
//       if (mqttClient.subscribe(mqttLightSubscription))
//       {
//         debugPrintln(String(F("MQTT: subscribed to ")) + mqttLightSubscription);
//       }
//       if (mqttClient.subscribe(mqttLightBrightSubscription))
//       {
//         debugPrintln(String(F("MQTT: subscribed to ")) + mqttLightBrightSubscription);
//       }

//       // Publish discovery configuration
//       mqttDiscovery();

//       // Publish backlight status
//       if (lcdBacklightOn)
//       {
//         debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'ON'")));
//         mqttClient.publish(mqttLightStateTopic, "ON", true, 1);
//       }
//       else
//       {
//         debugPrintln(String(F("MQTT OUT: '")) + mqttLightStateTopic + String(F("' : 'OFF'")));
//         mqttClient.publish(mqttLightStateTopic, "OFF", true, 1);
//       }
//       debugPrintln(String(F("MQTT OUT: '")) + mqttLightBrightStateTopic + String(F("' : ")) + String(lcdBacklightDim));
//       mqttClient.publish(mqttLightBrightStateTopic, String(lcdBacklightDim), true, 1);

//       if (mqttFirstConnect)
//       { // Force any subscribed clients to toggle OFF/ON when we first connect to
//         // make sure we get a full panel refresh at power on.  Sending OFF,
//         // "ON" will be sent by the mqttStatusTopic subscription action below.
//         mqttFirstConnect = false;
//         debugPrintln(String(F("MQTT OUT: '")) + mqttStatusTopic + "' : 'OFF'");
//         mqttClient.publish(mqttStatusTopic, "OFF", true, 0);
//       }

//       if (mqttClient.subscribe(mqttStatusTopic))
//       {
//         debugPrintln(String(F("MQTT: subscribed to ")) + mqttStatusTopic);
//       }
//       mqttClient.loop();
//     }
//     else
//     { // Retry until we give up and restart after connectTimeout seconds
//       mqttReconnectCount++;
//       if (mqttReconnectCount * mqttConnectTimeout * 6 > (connectTimeout * 1000))
//       {
//         debugPrintln(String(F("MQTT: connection attempt ")) + String(mqttReconnectCount) + String(F(" failed with rc: ")) + String(mqttClient.returnCode()) + String(F(" and error: ")) + String(mqttClient.lastError()) + String(F(". Restarting device.")));
//         espReset();
//       }
//       yield();
//       webServer.handleClient();

//       String mqttCheckResult = "Ping: FAILED";
//       String mqttCheckResultNextion = "MQTT Check...";

//       debugPrintln(String(F("MQTT: connection attempt ")) + String(mqttReconnectCount) + String(F(" failed with rc ")) + String(mqttClient.returnCode()) + String(F(" and error: ")) + String(mqttClient.lastError()));
//       nextionSetAttr("p[0].b[1].txt", String(F("\"WiFi Connected!\\r ")) + String(WiFi.SSID()) + String(F("\\rIP: ")) + WiFi.localIP().toString() + String(F("\\r\\rMQTT Failed:\\r ")) + String(mqttServer) + String(F("\\rRC: ")) + String(mqttClient.returnCode()) + String(F("   Error: ")) + String(mqttClient.lastError()) + String(F("\\r")) + mqttCheckResultNextion + String(F("\"")));

//       mqttPingCheck = Ping.ping(mqttServer, 4);
//       yield();
//       webServer.handleClient();
//       mqttPortCheck = wifiClient.connect(mqttServer, atoi(mqttPort));
//       yield();
//       webServer.handleClient();

//       mqttCheckResultNextion = "Ping: ";
//       if (mqttPingCheck)
//       {
//         mqttCheckResult = "Ping: SUCCESS";
//         mqttCheckResultNextion = "Ping: ";
//       }
//       if (mqttPortCheck)
//       {
//         mqttCheckResult += " Port: SUCCESS";
//         mqttCheckResultNextion += " Port: ";
//       }
//       else
//       {
//         mqttCheckResult += " Port: FAILED";
//         mqttCheckResultNextion += " Port: ";
//       }
//       debugPrintln(String(F("MQTT: connection checks: ")) + mqttCheckResult + String(F(". Trying again in 30 seconds.")));
//       nextionSetAttr("p[0].b[1].txt", String(F("\"WiFi Connected!\\r ")) + String(WiFi.SSID()) + String(F("\\rIP: ")) + WiFi.localIP().toString() + String(F("\\r\\rMQTT Failed:\\r ")) + String(mqttServer) + String(F("\\rRC: ")) + String(mqttClient.returnCode()) + String(F("   Error: ")) + String(mqttClient.lastError()) + String(F("\\r")) + mqttCheckResultNextion + String(F("\"")));

//       while (millis() < (mqttConnectTimer + (mqttConnectTimeout * 6)))
//       {
//         yield();
//         nextionHandleInput();     // Nextion serial communications loop
//         ArduinoOTA.handle();      // Arduino OTA loop
//         webServer.handleClient(); // webServer loop
//         telnetHandleClient();     // telnet client loop
//         motionHandle();           // motion sensor loop
//         beepHandle();             // beep feedback loop
//       }
//     }
//   }
//   rebootOnp0b1 = false;
//   if (nextionActivePage < 0)
//   { // We never picked up a message giving us a page number, so we'll just go to the default page
//     debugPrintln(String(F("DEBUG: NextionActivePage not received from MQTT, setting to 0")));
//     String mqttButtonJSONEvent = String(F("{\"event\":\"page\",\"value\":0}"));
//     debugPrintln(String(F("MQTT OUT: '")) + mqttStateJSONTopic + String(F("' : '")) + mqttButtonJSONEvent + String(F("'")));
//     mqttClient.publish(mqttStateJSONTopic, mqttButtonJSONEvent, false, 0);
//     String mqttPageTopic = mqttStateTopic + "/page";
//     debugPrintln(String(F("MQTT OUT: '")) + mqttPageTopic + String(F("' : '0'")));
//     mqttClient.publish(mqttPageTopic, "0", false, 0);
//   }
// }

#endif



// ////////////////////////////////////////////////////////////////////////////////
// String printHex8(byte *data, uint8_t length)
// { // returns input bytes as printable hex values in the format 0x01 0x23 0xFF

//   String hex8String;
//   for (int i = 0; i < length; i++)
//   {
//     hex8String += "0x";
//     if (data[i] < 0x10)
//     {
//       hex8String += "0";
//     }
//     hex8String += String(data[i], HEX);
//     if (i != (length - 1))
//     {
//       hex8String += " ";
//     }
//   }
//   // hex8String.toUpperCase();
//   return hex8String;
// }
// // 