/**
 * @file mDisplaysInterface.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "mDisplaysInterface.h"

#ifdef USE_MODULE_DISPLAYS_INTERFACE

const char* mDisplaysInterface::PM_MODULE_DISPLAYS_INTERFACE_CTR = D_MODULE_DISPLAYS_INTERFACE_CTR;
const char* mDisplaysInterface::PM_MODULE_DISPLAYS_INTERFACE_FRIENDLY_CTR = D_MODULE_DISPLAYS_INTERFACE_FRIENDLY_CTR;

int8_t mDisplaysInterface::Tasker(uint8_t function, JsonParserObject obj){
  
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
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
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
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * OTHER SECTION * 
    *******************/
    case FUNC_SET_POWER:
      // DisplaySetPower();
    break;
    case FUNC_EVERY_SECOND:
      #ifdef USE_GRAPH
        DisplayCheckGraph();
      #endif
      #ifdef USE_DT_VARS
        get_dt_mqtt();
        draw_dt_vars();
      #endif // USE_DT_VARS
      SetPower();
    break;
  }
  
  return function_result;

} // END function


void mDisplaysInterface::Pre_Init(void)
{
  InitDriver();
  settings.fEnableSensor = true;
}


void mDisplaysInterface::Init(void)
{

}


void mDisplaysInterface::EveryLoop()
{

}


void mDisplaysInterface::Init(uint8_t mode)
{
  if (renderer)  {
    renderer->DisplayInit(mode, pCONT_set->Settings.display.size, pCONT_set->Settings.display.rotate, pCONT_set->Settings.display.font);
  }
  else {
    dsp_init = mode;
    pCONT->Tasker_Interface(FUNC_DISPLAY_INIT);
  }
}

void mDisplaysInterface::Clear(void)
{
  pCONT->Tasker_Interface(FUNC_DISPLAY_CLEAR);
}

void mDisplaysInterface::DrawStringAt(uint16_t x, uint16_t y, char *str, uint16_t color, uint8_t flag)
{
  dsp_x = x;
  dsp_y = y;
  dsp_str = str;
  dsp_color = color;
  dsp_flag = flag;
  pCONT->Tasker_Interface(FUNC_DISPLAY_DRAW_STRING);
}

void mDisplaysInterface::DisplayOnOff(uint8_t on)
{
//   ExecuteCommandPower(disp_device, on, SRC_DISPLAY);
}

/*-------------------------------------------------------------------------------------------*/

// get asci float number
uint8_t mDisplaysInterface::fatoiv(char *cp,float *res) {
  uint8_t index=0;
  *res= pCONT_sup->CharToFloat(cp);
  while (*cp) {
    if ((*cp>='0' && *cp<='9') || (*cp=='-') || (*cp=='.')) {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

// get asci number until delimiter and return asci number lenght and value
uint8_t mDisplaysInterface::atoiv(char *cp, int16_t *res)
{
  uint8_t index = 0;
  *res = atoi(cp);
  while (*cp) {
    if ((*cp>='0' && *cp<='9') || (*cp=='-')) {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

// get asci number until delimiter and return asci number lenght and value
uint8_t mDisplaysInterface::atoiV(char *cp, uint16_t *res)
{
  uint8_t index = 0;
  *res = atoi(cp);
  while (*cp) {
    if (*cp>='0' && *cp<='9') {
      cp++;
      index++;
    } else {
      break;
    }
  }
  return index;
}

// right align string
void mDisplaysInterface::alignright(char *string) {
  uint16_t slen=strlen(string);
  uint16_t len=slen;
  while (len) {
    // count spaces to the right
    if (string[len-1]!=' ') {
      break;
    }
    len--;
  }
  uint16_t diff=slen-len;
  if (diff>0) {
    // move string
    memmove(&string[diff],string,len);
    memset(string,' ',diff);
  }
}

char *mDisplaysInterface::get_string(char *buff,uint8_t len,char *cp) {
uint8_t index=0;
  while (*cp!=':') {
    buff[index]=*cp++;
    index++;
    if (index>=len) break;
  }
  buff[index]=0;
  cp++;
  return cp;
}

#define ESCAPE_CHAR '~'

// decode text escapes, 1 hexbyte assumed
uint32_t mDisplaysInterface::decode_te(char *line) {
  uint32_t skip = 0;
  char sbuf[3],*cp;
  while (*line) {
    if (*line==ESCAPE_CHAR) {
      cp=line+1;
      if (*cp!=0 && *cp==ESCAPE_CHAR) {
        // escape escape, discard one
        memmove(cp,cp+1,strlen(cp));
        skip++;
      } else {
        // escape HH
        if (strlen(cp)<2) {
          // illegal lenght, ignore
          return skip;
        }
        // take 2 hex chars
        sbuf[0]=*(cp);
        sbuf[1]=*(cp+1);
        sbuf[2]=0;
        *line=strtol(sbuf,0,16);
        // must shift string 2 bytes shift zero also
        memmove(cp,cp+2,strlen(cp)-1);
        skip += 2;
      }
    }
    line++;
  }
  return skip;
}

// /*-------------------------------------------------------------------------------------------*/


// uint16_t GetColorFromIndex(uint32_t index) {
//   if (index >= MAX_INDEXCOLORS) index = 0;

//   if (index < PREDEF_INDEXCOLORS) {
//     return renderer->GetColorFromIndex(index);
//   } else {
//     return index_colors[index - PREDEF_INDEXCOLORS];
//   }
// }

/*********************************************************************************************\
 * Public
\*********************************************************************************************/

void mDisplaysInterface::InitDriver(void)
{
  pCONT->Tasker_Interface(FUNC_DISPLAY_INIT_DRIVER);

  #ifdef USE_MULTI_DISPLAY
    Set_display(0);
  #endif // USE_MULTI_DISPLAY


  if (renderer) {
    
    renderer->setTextFont(pCONT_set->Settings.display.font);
    renderer->setTextSize(pCONT_set->Settings.display.size);
    
    // force opaque mode
    renderer->setDrawMode(0);

    for (uint32_t cnt = 0; cnt < (MAX_INDEXCOLORS - PREDEF_INDEXCOLORS); cnt++) {
      index_colors[cnt] = 0;
    }
  }
  
  #ifdef USE_DT_VARS
    free_dt_vars();
  #endif

  #ifdef USE_UFILESYS
    Display_Text_From_File("/display.ini");
  #endif

  #ifdef USE_GRAPH
    for (uint8_t count = 0; count < NUM_GRAPHS; count++) { graph[count] = 0; }
  #endif

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DEBUG "Display model %d"), pCONT_set->Settings.display.model);

  pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_UTC_TIME_ID;

  if (pCONT_set->Settings.display.model) {
    pCONT_set->devices_present++;
    if (!pCONT_pins->PinUsed(GPIO_BACKLIGHT_ID)) {
      if (pCONT_set->runtime_var.light_type && (4 == pCONT_set->Settings.display.model)) {
        pCONT_set->devices_present--;  // Assume PWM channel is used for backlight
      }
    }
    disp_device = pCONT_set->devices_present;


    #ifdef ENABLE_FEATURE_DISPLAY_LOG_BUFFER
      LogBuffer_Init();
    #endif

  
  }

  // pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_BASIC_BUFFER_TEXT_DRAW;
    
}


void mDisplaysInterface::SetPower(void)
{
  disp_power = 1;//bitRead(XdrvMailbox.index, disp_device -1);

  //AddLog(LOG_LEVEL_DEBUG, PSTR("DSP: Power %d"), disp_power);

  if (pCONT_set->Settings.display.model) {
    if (!renderer) {
      pCONT->Tasker_Interface(FUNC_DISPLAY_POWER);
    } else {
      renderer->DisplayOnff(disp_power);
    }
  }
  
}


// void DisplayReInitDriver(void) {
//   pCONT->Tasker_Interface(FUNC_DISPLAY_INIT_DRIVER);
// #ifdef USE_MULTI_DISPLAY
//   Set_display(0);
// #endif // USE_MULTI_DISPLAY
//   ResponseCmndDone();
// }



/**
 * @brief Indirect function layer to allow debugging
 * @note command will contain checks for viability
 * */
void mDisplaysInterface::SetCursor(uint8_t x, uint8_t y)
{
  AddLog(LOG_LEVEL_TEST, PSTR("SetCursor(%d,%d)"),x,y);
  pCONT_iDisp->renderer->setCursor(x,y);
}

/**
 * @brief Indirect function layer to allow debugging
 * @note command will contain checks for viability
 * */
void mDisplaysInterface::SetTextSize(uint8_t font_size)
{
  AddLog(LOG_LEVEL_TEST, PSTR("SetFontSize(%d)"),font_size);
  pCONT_iDisp->renderer->setTextSize(font_size);
}

/**
 * @brief Indirect function layer to allow debugging
 * @note command will contain checks for viability
 * */
void mDisplaysInterface::SetDisplayMode(uint8_t mode)
{
  AddLog(LOG_LEVEL_TEST, PSTR("SetDisplayMode(%d)"),mode);
  pCONT_set->Settings.display.mode = mode;
}


#endif
