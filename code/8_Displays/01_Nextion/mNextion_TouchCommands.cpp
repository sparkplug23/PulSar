#include "mNextion.h"

#ifdef USE_MODULE_DISPLAYS_NEXTION


void mNextion::ProcessInput()
{
    if (!incoming_frame.complete || incoming_frame.len == 0) return;

  const uint8_t code = incoming_frame.data[0];

  // Always log the instruction code and name (legacy helper; expand it later for errors too)
  ALOG_INF(PSTR("Nextion RX: code=0x%02X (%s), len=%u"), code, GetInstructionName(code), (unsigned)incoming_frame.len);

  // ---- core events ----
  if (code == INSTRUCTION_SET_RETURN_CODE__TOUCH_EVENT) {
    ProcessInput__TouchEvent();
  }
  else if (code == INSTRUCTION_SET_RETURN_CODE__CURRENT_PAGE_NUMBER) {
    ProcessInput__CurrentPageNumber();
  }
  else if (code == INSTRUCTION_SET_RETURN_CODE__TOUCH_COORDINATE_AWAKE) {
    ProcessInput__TouchCoordinateData();
  }
  else if (code == INSTRUCTION_SET_RETURN_CODE__TOUCH_COORDINATE_SLEEP) {
    ProcessInput__TouchCoordinateData_Wake();
  }
  else if (code == INSTRUCTION_SET_RETURN_CODE__STRING_DATA_ENCLOSED) {
    ProcessInput__GetStringReturn();
  }
  else if (code == INSTRUCTION_SET_RETURN_CODE__NUMERIC_DATA_ENCLOSED) {
    ProcessInput__GetIntReturn();
  }
  else if (code == 0x63) {
    ProcessInput__ComokResponse();
  }

  // ---- error / status return codes (framework bucket, expand later) ----
  else if (
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_INSTRUCTION || 
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_COMPONENT_ID || 
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_PAGE_ID || 
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_PICTURE_ID || 
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_FONT_ID || 
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_FILE_OPERATION || 
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_CRC ||
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_BAUD_RATE || 
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_WAVEFORM_ID_OR_CHANNEL ||
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_VARIABLE_NAME_OR_ATTRIBUTE || 
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_VARIABLE_OPERATION || 
      code == INSTRUCTION_SET_RETURN_CODE__ASSIGNMENT_FAILED || 
      code == INSTRUCTION_SET_RETURN_CODE__EEPROM_OPERATION_FAILED || 
      code == INSTRUCTION_SET_RETURN_CODE__INVALID_PARAMETER_QUANTITY || 
      code == INSTRUCTION_SET_RETURN_CODE__IO_OPERATION_FAILED ||
      code == INSTRUCTION_SET_RETURN_CODE__ESCAPE_CHARACTER_INVALID || 
      code == INSTRUCTION_SET_RETURN_CODE__VARIABLE_NAME_TOO_LONG || 
      code == INSTRUCTION_SET_RETURN_CODE__SERIAL_BUFFER_OVERFLOW
    )
  {
    ProcessInput__ErrorReturnCode();
  }

  // ---- unknown ----
  else {
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_NEXTION "HMI IN: [Unhandled] code=0x%02X, len=%u"), code, (unsigned)incoming_frame.len);
  }

  // Reset for next frame
  incoming_frame.complete = false;
  incoming_frame.len = 0;

}



void mNextion::ProcessInput__TouchEvent()
{
// 0x65+Page ID+Component ID+TouchEvent+End
// Return this data when the touch event created by the user is pressed.
// Definition of TouchEvent: Press Event 0x01, Release Event 0X00
// Example: 0x65 0x00 0x02 0x01 0xFF 0xFF 0xFF
// Meaning: Touch Event, Page 0, Object 2, Press

  if (incoming_frame.len < 4) {
    ALOG_WRN(PSTR(D_LOG_NEXTION "TouchEvent frame too short (%u)"),
             incoming_frame.len);
    return;
  }

  const uint8_t page   = incoming_frame.data[1];
  const uint8_t comp   = incoming_frame.data[2];
  const uint8_t action = incoming_frame.data[3];

  screen_press.page  = page;
  screen_press.event = comp;

  if (action == 0x01) { // PRESSED
    screen_press.tSavedButtonONEvent = millis();
    screen_press.fEnableImmediateButtonTime = true;

    AddLog(LOG_LEVEL_INFO,
      PSTR(D_LOG_NEXTION D_NEXTION_RX "\"p[%d].b[%d]\"=%s"),
      page, comp, D_ON);

    JBI->Start();
      JBI->Add("Page", page);
      JBI->Add("ID", comp);
      JBI->Addf("event", "p[%d].b[%d]", page, comp);
      JBI->Add("value", D_ON);
    JBI->End();

    tkr_mqtt->Publish("status/nextion/event",       JBI->GetBufferPtr(), 0);
    tkr_mqtt->Publish("status/nextion/event/start", JBI->GetBufferPtr(), 0);
  }
  else if (action == 0x00) { // RELEASE
    screen_press.tSavedButtonOFFEvent = millis();
    screen_press.fEnableImmediateButtonTime = false;

    screen_press.duration =
      screen_press.tSavedButtonOFFEvent - screen_press.tSavedButtonONEvent;

    AddLog(LOG_LEVEL_INFO,
      PSTR(D_LOG_NEXTION D_NEXTION_RX "\"p[%d].b[%d]\"=%s"),
      page, comp, D_OFF);

    JBI->Start();
      JBI->Add("Page", page);
      JBI->Add("ID", comp);
      JBI->Addf("event", "p[%d].b[%d]", page, comp);
      JBI->Add("value", D_OFF);
      JBI->Add("duration", screen_press.duration);
    JBI->End();

    if (!fEnableIgnoreNextOffEvent) {
      tkr_mqtt->Publish("status/nextion/event",     JBI->GetBufferPtr(), 0);
      tkr_mqtt->Publish("status/nextion/event/end", JBI->GetBufferPtr(), 0);
      MQTTSend_PressEvent();
    } else {
      fEnableIgnoreNextOffEvent = false;
    }
  }
}


void mNextion::ProcessInput__CurrentPageNumber()
{
    
  // Handle incoming "sendme" page number
    // 0x66+PageNum+End
    // Example: 0x66 0x02 0xFF 0xFF 0xFF
    // Meaning: page 2

  // 0x66 + PageNum + 0xFF 0xFF 0xFF
  if (incoming_frame.len < 2) {
    ALOG_WRN(PSTR(D_LOG_NEXTION "CurrentPage frame too short (%u)"), incoming_frame.len);
    return;
  }

  const uint8_t page = incoming_frame.data[1];

  // Keep same logging style, but avoid String churn unless you need it elsewhere
  AddLog(LOG_LEVEL_INFO,
         PSTR(D_LOG_NEXTION D_NEXTION_RX "[sendme Page] \"%u\""),
         (unsigned)page);

  // If you later re-enable this, do it with numeric page:
  //
  // if ((settings.page != page) && ((page != 0) || nextionReportPage0)) {
  //   settings.page = page;
  //   tkr_mqtt->Publish("status/nextion/event4", String(page).c_str(), 0); // or publish numeric
  // }
}


void mNextion::ProcessInput__GetStringReturn()
{
  // 0x70 + ASCII string...
  if (incoming_frame.len < 2) {
    ALOG_WRN(PSTR(D_LOG_NEXTION "GetString frame too short (%u)"), incoming_frame.len);
    return;
  }

  String getString;
  getString.reserve(incoming_frame.len > 1 ? (incoming_frame.len - 1) : 0);

  // Bytes [1..len-1] are ASCII payload (assuming len excludes 0xFF terminators)
  for (uint16_t i = 1; i < incoming_frame.len; i++) {
    getString += char(incoming_frame.data[i]);
  }

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "HMI IN: [String Return] '%s'"), getString.c_str());

  // if (mqttGetSubtopic == "") {
  //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "MQTT OUT: '%s' : '%s']"),
  //          mqttStateTopic.c_str(), getString.c_str());
  //   tkr_mqtt->Publish("status/nextion/getString", getString.c_str(), 0);
  // } else {
  //   String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
  //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "MQTT OUT: '%s' : '%s']"),
  //          mqttReturnTopic.c_str(), getString.c_str());
  //   tkr_mqtt->Publish("status/nextion/getString", getString.c_str(), 0);
  //   mqttGetSubtopic = "";
  // }
}



void mNextion::ProcessInput__GetIntReturn()
{
  // 0x71 + byte1 + byte2 + byte3 + byte4 (little endian)
  if (incoming_frame.len < 5) {
    ALOG_WRN(PSTR(D_LOG_NEXTION "GetInt frame too short (%u)"), incoming_frame.len);
    return;
  }

  const uint32_t getInt =
      (uint32_t(incoming_frame.data[1])      ) |
      (uint32_t(incoming_frame.data[2]) <<  8) |
      (uint32_t(incoming_frame.data[3]) << 16) |
      (uint32_t(incoming_frame.data[4]) << 24);

  String getString = String(getInt);

  if (lcdVersionQueryFlag) {
    lcdVersion = getInt;
    lcdVersionQueryFlag = false;
    return;
  }

  // if (mqttGetSubtopic == "") {
  //   Serial.println(getString);
  //   tkr_mqtt->Publish("status/nextion/event9", getString.c_str(), 0);
  // } else {
  //   String mqttReturnTopic = mqttStateTopic + mqttGetSubtopic;
  //   tkr_mqtt->Publish("status/nextion/event10", getString.c_str(), 0);

  //   String mqttButtonJSONEvent =
  //       String(F("{\"event\":\"")) + mqttGetSubtopicJSON +
  //       String(F("\", \"value\":")) + getString +
  //       String(F("}"));

  //   tkr_mqtt->Publish("status/nextion/event11", mqttButtonJSONEvent.c_str(), 0);

  //   mqttGetSubtopic = "";
  // }
}






// =================================================================================================
// Error/Status return code framework (includes all the ones you listed)
// =================================================================================================
void mNextion::ProcessInput__ErrorReturnCode()
{
  if (!incoming_frame.complete || incoming_frame.len == 0) return;

  const uint8_t code = incoming_frame.data[0];

  // Framework mapping (expand later as needed)
  const char* desc = "Unhandled Error Code";
  switch (code) {
    case 0x04: desc = "Invalid Picture ID"; break;
    case 0x05: desc = "Invalid Font ID"; break;
    case 0x06: desc = "Invalid File Operation"; break;
    case 0x09: desc = "Invalid CRC"; break;

    case 0x11: desc = "Invalid Baud rate Setting"; break;
    case 0x12: desc = "Invalid Waveform ID or Channel #"; break;

    case 0x1A: desc = "Invalid Variable name or attribute"; break;
    case 0x1B: desc = "Invalid Variable Operation"; break;
    case 0x1C: desc = "Assignment failed to assign"; break;
    case 0x1D: desc = "EEPROM Operation failed"; break;
    case 0x1E: desc = "Invalid Quantity of Parameters"; break;
    case 0x1F: desc = "IO Operation failed"; break;

    case 0x20: desc = "Escape Character Invalid"; break;
    case 0x23: desc = "Variable name too long"; break;
    case 0x24: desc = "Serial Buffer Overflow"; break;
    default: break;
  }

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "HMI IN: [ReturnCode] code=0x%02X (%s), len=%u"),
         code, desc, (unsigned)incoming_frame.len);

  // Framework hook: always build JSON, but DO NOT publish yet (you can flip later)
  // Keep it consistent with your existing JBI usage.
  //
  // Example output:
  //   {"event":"nextion_return_data","return_code":"0x1A","return_code_description":"Invalid Variable name or attribute"}
  //
  // Note: avoid sprintf into small buffers; keep it simple.

  if (JBI) {
    char code_hex[6]; // "0x" + 2 hex + null
    snprintf(code_hex, sizeof(code_hex), "0x%02X", code);

    JBI->Start();
      JBI->Add("event", "nextion_return_data");
      JBI->Add("return_code", code_hex);
      JBI->Add("return_code_description", desc);
      JBI->Add("len", (uint32_t)incoming_frame.len);
    JBI->End();

    // Intentionally disabled during migration to prevent spam:
    // tkr_mqtt->Publish("status/nextion/return_code", JBI->GetBufferPtr(), 0);
  }

  // Special-case behaviour you previously relied on:
  // 0x1A often occurs when querying .val on objects that don't support it.
  // Keep the old side-effect, but only for that code.
  // if (code == 0x1A) {
  //   mqttGetSubtopic = "";
  // }
}


// =================================================================================================
// Minimal decoders for the remaining items you pasted (0x66,0x67,0x68,0x70,0x71,0x63/"comok")
// These are direct translations to incoming_frame, no behavioural changes beyond removing "not enabled".
// =================================================================================================


void mNextion::ProcessInput__TouchCoordinateData()
{
  // 0x67 + XH + XL + YH + YL + TouchEvent + 0xFF 0xFF 0xFF
  if (incoming_frame.len < 1 + 5 + 3) return;

  uint16_t xCoord = (uint16_t)incoming_frame.data[1] * 256u + (uint16_t)incoming_frame.data[2];
  uint16_t yCoord = (uint16_t)incoming_frame.data[3] * 256u + (uint16_t)incoming_frame.data[4];
  const uint8_t touch = incoming_frame.data[5]; // 0x01 press, 0x00 release

  String xyCoord = String(xCoord) + ',' + String(yCoord);

  if (touch == 0x01) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "HMI IN: [Touch ON] '%s'"), xyCoord.c_str());
    // String mqttTouchTopic = mqttStateTopic + "/touchOn";
    // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "MQTT OUT: '%s' '%s'"), mqttTouchTopic.c_str(), xyCoord.c_str());
    tkr_mqtt->Publish("status/nextion/xyCoord", xyCoord.c_str(), 0);
  } else if (touch == 0x00) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "HMI IN: [Touch OFF] '%s'"), xyCoord.c_str());
    // String mqttTouchTopic = mqttStateTopic + "/touchOff";
    // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "MQTT OUT: '%s' '%s'"), mqttTouchTopic.c_str(), xyCoord.c_str());
    tkr_mqtt->Publish("status/nextion/event6", xyCoord.c_str(), 0);
  }
}

void mNextion::ProcessInput__TouchCoordinateData_Wake()
{
  // 0x68 has historically been "same format as 0x67 but when waking from sleep"
  // Decode the same way for now; you decide behaviour later.
  if (incoming_frame.len < 1 + 5 + 3) return;

  uint16_t xCoord = (uint16_t)incoming_frame.data[1] * 256u + (uint16_t)incoming_frame.data[2];
  uint16_t yCoord = (uint16_t)incoming_frame.data[3] * 256u + (uint16_t)incoming_frame.data[4];
  const uint8_t touch = incoming_frame.data[5];

  String xyCoord = String(xCoord) + ',' + String(yCoord);

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEXTION "HMI IN: [Touch XY Wake] code=0x68, '%s', ev=0x%02X"),
         xyCoord.c_str(), touch);

  // No further action yet (framework stub)
}


void mNextion::ProcessInput__ComokResponse()
{
  // Expected: 0x63 'o' 'm' 'o' 'k' ... 0xFF 0xFF 0xFF
  if (!incoming_frame.complete) return;
  if (incoming_frame.len < (1 + 4 + 3)) return; // 0x63 + "omok"(4) + 3xFF

  if (incoming_frame.data[0] != 0x63 ||
      incoming_frame.data[1] != 0x6f || // 'o'
      incoming_frame.data[2] != 0x6d || // 'm'
      incoming_frame.data[3] != 0x6f || // 'o'
      incoming_frame.data[4] != 0x6b) { // 'k'
    return; // not comok
  }

  ALOG_INF(PSTR("Catch 'comok' response to 'connect' command (len=%u)"),
           (unsigned)incoming_frame.len);

  String comokField;
  uint8_t comokFieldCount = 0;
  const uint8_t sep = 0x2c; // ','

  // Exclude terminators
  const uint16_t end = incoming_frame.len - 3;

  for (uint16_t i = 0; i < end; i++) {
    const uint8_t b = incoming_frame.data[i];

    if (b == sep) {
      if (comokFieldCount == 2) {
        strncpy(nextionModel, comokField.c_str(), sizeof(nextionModel) - 1);
        nextionModel[sizeof(nextionModel) - 1] = '\0'; // Ensure null termination
      }
      comokFieldCount++;
      comokField = "";
    } else {
      comokField += (char)b;
    }
  }
}





#endif