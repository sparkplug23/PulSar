#include "mWLEDEffects.h"

#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS 

const char* mWLEDEffects::PM_MODULE_LIGHTS_WLED_CTR = D_MODULE_LIGHTS_WLED_CTR;
const char* mWLEDEffects::PM_MODULE_LIGHTS_WLED_FRIENDLY_CTR = D_MODULE_LIGHTS_WLED_FRIENDLY_CTR;

int8_t mWLEDEffects::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_INIT){
    Init();
  }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
      SubTask_Animation();
    break;
    case FUNC_EVERY_SECOND:{
      char buffer[50];
      AddLog(LOG_LEVEL_TEST, PSTR("GetEffectsModeNamebyID=%s"), 
        GetEffectsModeNamebyID(mEffects->getMode(),buffer,sizeof(buffer))
      );
    }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
  } // end switch
    
} // END function


//enable custom per-LED mapping. This can allow for better effects on matrices or special displays
//#define WLED_CUSTOM_LED_MAPPING
#ifdef WLED_CUSTOM_LED_MAPPING
//this is just an example (30 LEDs). It will first set all even, then all uneven LEDs.
const uint16_t customMappingTable[] = {
  0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28,
  1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29};

//another example. Switches direction every 5 LEDs.
/*const uint16_t customMappingTable[] = {
  0, 1, 2, 3, 4, 9, 8, 7, 6, 5, 10, 11, 12, 13, 14,
  19, 18, 17, 16, 15, 20, 21, 22, 23, 24, 29, 28, 27, 26, 25};*/

const uint16_t customMappingSize = sizeof(customMappingTable)/sizeof(uint16_t); //30 in example
#endif



void mWLEDEffects::Init(void)
{

  pCONT_iLight->Init_NeoPixelAnimator(1, NEO_ANIMATION_TIMEBASE); // NeoPixel animation management object

  ledCount = 50;

  mEffects = new WS2812FX();

  // Class constructor of WS2812FX is defining the animations and the size at the start

  DEBUG_PRINT("esp8266 ");
  DEBUG_PRINTLN(ESP.getCoreVersion());
  
  int heapPreAlloc = ESP.getFreeHeap();
  DEBUG_PRINT("heap ");
  DEBUG_PRINTLN(ESP.getFreeHeap());
  // registerUsermods();

  mEffects->init(0, ledCount, 0);
  mEffects->setBrightness(0);

  DEBUG_PRINT("LEDs inited. heap usage ~");
  DEBUG_PRINTLN(heapPreAlloc - ESP.getFreeHeap());

  // Initialize NeoPixel Strip and button
  // mEffects->setShowCallback(handleOverlayDraw);

  colorUpdated(NOTIFIER_CALL_MODE_INIT);
  
  mEffects->service();
  
  //mEffects->setEffectConfig(effectCurrent, effectSpeed, effectIntensity, effectPalette);
  mEffects->setEffectConfig(FX_MODE_FADE, 100, 100, 11);

}


void mWLEDEffects::SubTask_Animation()
{
  
  handleTransitions();
  mEffects->service();

}


/*
 * Color conversion methods
 */
void mWLEDEffects::colorFromUint32(uint32_t in, bool secondary)
{
  if (secondary) {
    colSec[3] = in >> 24 & 0xFF;
    colSec[0] = in >> 16 & 0xFF;
    colSec[1] = in >> 8  & 0xFF;
    colSec[2] = in       & 0xFF;
  } else {
    col[3] = in >> 24 & 0xFF;
    col[0] = in >> 16 & 0xFF;
    col[1] = in >> 8  & 0xFF;
    col[2] = in       & 0xFF;
  }
}

//load a color without affecting the white channel
void mWLEDEffects::colorFromUint24(uint32_t in, bool secondary)
{
  if (secondary) {
    colSec[0] = in >> 16 & 0xFF;
    colSec[1] = in >> 8  & 0xFF;
    colSec[2] = in       & 0xFF;
  } else {
    col[0] = in >> 16 & 0xFF;
    col[1] = in >> 8  & 0xFF;
    col[2] = in       & 0xFF;
  }
}

//relatively change white brightness, minumum A=5
void mWLEDEffects::relativeChangeWhite(int8_t amount, byte lowerBoundary)
{
  int16_t new_val = (int16_t) col[3] + amount;
  if (new_val > 0xFF) new_val = 0xFF;
  else if (new_val < lowerBoundary) new_val = lowerBoundary;
  col[3] = new_val;
}

void mWLEDEffects::colorHStoRGB(uint16_t hue, byte sat, byte* rgb) //hue, sat to rgb
{
  float h = ((float)hue)/65535.0;
  float s = ((float)sat)/255.0;
  byte i = floor(h*6);
  float f = h * 6-i;
  float p = 255 * (1-s);
  float q = 255 * (1-f*s);
  float t = 255 * (1-(1-f)*s);
  switch (i%6) {
    case 0: rgb[0]=255,rgb[1]=t,rgb[2]=p;break;
    case 1: rgb[0]=q,rgb[1]=255,rgb[2]=p;break;
    case 2: rgb[0]=p,rgb[1]=255,rgb[2]=t;break;
    case 3: rgb[0]=p,rgb[1]=q,rgb[2]=255;break;
    case 4: rgb[0]=t,rgb[1]=p,rgb[2]=255;break;
    case 5: rgb[0]=255,rgb[1]=p,rgb[2]=q;
  }
  if (useRGBW && mEffects->rgbwMode == RGBW_MODE_LEGACY) colorRGBtoRGBW(col);
}

void mWLEDEffects::colorCTtoRGB(uint16_t mired, byte* rgb) //white spectrum to rgb
{
  //this is only an approximation using WS2812B with gamma correction enabled
  if (mired > 475) {
    rgb[0]=255;rgb[1]=199;rgb[2]=92;//500
  } else if (mired > 425) {
    rgb[0]=255;rgb[1]=213;rgb[2]=118;//450
  } else if (mired > 375) {
    rgb[0]=255;rgb[1]=216;rgb[2]=118;//400
  } else if (mired > 325) {
    rgb[0]=255;rgb[1]=234;rgb[2]=140;//350
  } else if (mired > 275) {
    rgb[0]=255;rgb[1]=243;rgb[2]=160;//300
  } else if (mired > 225) {
    rgb[0]=250;rgb[1]=255;rgb[2]=188;//250
  } else if (mired > 175) {
    rgb[0]=247;rgb[1]=255;rgb[2]=215;//200
  } else {
    rgb[0]=237;rgb[1]=255;rgb[2]=239;//150
  }
  if (useRGBW && mEffects->rgbwMode == RGBW_MODE_LEGACY) colorRGBtoRGBW(col);
}


void mWLEDEffects::colorFromDecOrHexString(byte* rgb, char* in)
{
  if (in[0] == 0) return;
  char first = in[0];
  uint32_t c = 0;
  
  if (first == '#' || first == 'h' || first == 'H') //is HEX encoded
  {
    c = strtoul(in +1, NULL, 16);
  } else
  {
    c = strtoul(in, NULL, 10);
  }

  rgb[3] = (c >> 24) & 0xFF;
  rgb[0] = (c >> 16) & 0xFF;
  rgb[1] = (c >>  8) & 0xFF;
  rgb[2] =  c        & 0xFF;
}

float mWLEDEffects::minf2(float v, float w)
{
  if (w > v) return v;
  return w;
}

float mWLEDEffects::maxf2(float v, float w)
{
  if (w > v) return w;
  return v;
}

void mWLEDEffects::colorRGBtoRGBW(byte* rgb) //rgb to rgbw (http://codewelt.com/rgbw). (RGBW_MODE_LEGACY)
{
  float low = minf2(rgb[0],minf2(rgb[1],rgb[2]));
  float high = maxf2(rgb[0],maxf2(rgb[1],rgb[2]));
  if (high < 0.1f) return;
  float sat = 100.0f * ((high - low) / high);;   // maximum saturation is 100  (corrected from 255)
  rgb[3] = (byte)((255.0f - sat) / 255.0f * (rgb[0] + rgb[1] + rgb[2]) / 3);
}


/*
 * LED methods
 */
void mWLEDEffects::setValuesFromMainSeg()
{
  WS2812FX::Segment& seg = mEffects->getSegment(mEffects->getMainSegmentId());
  colorFromUint32(seg.colors[0]);
  colorFromUint32(seg.colors[1], true);
  effectCurrent = seg.mode;
  effectSpeed = seg.speed;
  effectIntensity = seg.intensity;
  effectPalette = seg.palette;
}


void mWLEDEffects::resetTimebase()
{
  mEffects->timebase = 0 - millis();
}


void mWLEDEffects::toggleOnOff()
{
  if (bri == 0)
  {
    bri = briLast;
  } else
  {
    briLast = bri;
    bri = 0;
  }
}


void mWLEDEffects::setAllLeds()
{
  if (!realtimeMode || !arlsForceMaxBri)
  {
    double d = briT*briMultiplier;
    int val = d/100;
    if (val > 255) val = 255;
    mEffects->setBrightness(val);
  }
  if (useRGBW && mEffects->rgbwMode == RGBW_MODE_LEGACY)
  {
    colorRGBtoRGBW(colT);
    colorRGBtoRGBW(colSecT);
  }
  mEffects->setColor(0, colT[0], colT[1], colT[2], colT[3]);
  mEffects->setColor(1, colSecT[0], colSecT[1], colSecT[2], colSecT[3]);
}


void mWLEDEffects::setLedsStandard(bool justColors)
{
  for (byte i=0; i<4; i++)
  {
    colOld[i] = col[i];
    colT[i] = col[i];
    colSecOld[i] = colSec[i];
    colSecT[i] = colSec[i];
  }
  if (justColors) return;
  briOld = bri;
  briT = bri;
  setAllLeds();
}


bool mWLEDEffects::colorChanged()
{
  for (byte i=0; i<4; i++)
  {
    if (col[i] != colIT[i]) return true;
    if (colSec[i] != colSecIT[i]) return true;
  }
  if (bri != briIT) return true;
  return false;
}


void mWLEDEffects::colorUpdated(int callMode)
{
  //call for notifier -> 0: init 1: direct change 2: button 3: notification 4: nightlight 5: other (No notification)
  //                     6: fx changed 7: hue 8: preset cycle 9: blynk 10: alexa
  if (callMode != NOTIFIER_CALL_MODE_INIT && 
      callMode != NOTIFIER_CALL_MODE_DIRECT_CHANGE && 
      callMode != NOTIFIER_CALL_MODE_NO_NOTIFY) mEffects->applyToAllSelected = true; //if not from JSON api, which directly sets segments
  
  bool fxChanged = mEffects->setEffectConfig(effectCurrent, effectSpeed, effectIntensity, effectPalette);
  bool colChanged = colorChanged();

 // Serial.println("HERE");

  if (fxChanged || colChanged)
  {
    if (realtimeTimeout == UINT32_MAX) realtimeTimeout = 0;
    if (isPreset) {isPreset = false;}
        else {currentPreset = -1;}
        
    //notify(callMode);
    
    //set flag to update blynk and mqtt
    //if (callMode != NOTIFIER_CALL_MODE_PRESET_CYCLE) interfaceUpdateCallMode = callMode;
  } else {
    // if (nightlightActive && !nightlightActiveOld && 
    //     callMode != NOTIFIER_CALL_MODE_NOTIFICATION && 
    //     callMode != NOTIFIER_CALL_MODE_NO_NOTIFY)
    // {
    //   //notify(NOTIFIER_CALL_MODE_NIGHTLIGHT); 
    //   //interfaceUpdateCallMode = NOTIFIER_CALL_MODE_NIGHTLIGHT;
    // }
  }
  
  if (!colChanged) return; //following code is for e.g. initiating transitions
  
  // if (callMode != NOTIFIER_CALL_MODE_NO_NOTIFY && nightlightActive && (nightlightMode == NL_MODE_FADE || nightlightMode == NL_MODE_COLORFADE))
  // {
  //   briNlT = bri;
  //   nightlightDelayMs -= (millis() - nightlightStartTime);
  //   nightlightStartTime = millis();
  // }
  for (byte i=0; i<4; i++)
  {
    colIT[i] = col[i];
    colSecIT[i] = colSec[i];
  }
  if (briT == 0)
  {
    setLedsStandard(true);                                            //do not color transition if starting from off
    if (callMode != NOTIFIER_CALL_MODE_NOTIFICATION) resetTimebase(); //effect start from beginning
  }

  briIT = bri;
  if (bri > 0) briLast = bri;

  //deactivate nightlight if target brightness is reached
  // if (bri == nightlightTargetBri && callMode != NOTIFIER_CALL_MODE_NO_NOTIFY) nightlightActive = false;
  
  if (fadeTransition)
  {
    //set correct delay if not using notification delay
    if (callMode != NOTIFIER_CALL_MODE_NOTIFICATION && !jsonTransitionOnce) transitionDelayTemp = transitionDelay;
    jsonTransitionOnce = false;
    if (transitionDelayTemp == 0) {setLedsStandard(); mEffects->trigger(); return;}
    
    if (transitionActive)
    {
      for (byte i=0; i<4; i++)
      {
        colOld[i] = colT[i];
        colSecOld[i] = colSecT[i];
      }
      briOld = briT;
      tperLast = 0;
    }
    mEffects->setTransitionMode(true);
    transitionActive = true;
    transitionStartTime = millis();
  } else
  {
    setLedsStandard();
    mEffects->trigger();
  }
}



void mWLEDEffects::handleTransitions()
{
  
  if (transitionActive && transitionDelayTemp > 0)
  {
    float tper = (millis() - transitionStartTime)/(float)transitionDelayTemp;
    if (tper >= 1.0)
    {
      mEffects->setTransitionMode(false);
      transitionActive = false;
      tperLast = 0;
      setLedsStandard();
      return;
    }
    if (tper - tperLast < 0.004) return;
    tperLast = tper;
    for (byte i=0; i<4; i++)
    {
      colT[i] = colOld[i]+((col[i] - colOld[i])*tper);
      colSecT[i] = colSecOld[i]+((colSec[i] - colSecOld[i])*tper);
    }
    briT    = briOld   +((bri    - briOld   )*tper);
    
    setAllLeds();
  }
}


// Temporary method that tracks indexing, to be replaced with single dlist later
int8_t mWLEDEffects::GetEffectsModeIDbyName(const char* c){
  if(c=='\0') return -1;
  if(     strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)==0){ return LT_PWM5; }
  else if(strcmp_P(c,PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR)==0){       return LT_ADDRESSABLE; }
  return -1;
}
const char* mWLEDEffects::GetEffectsModeNamebyID(uint8_t id, char* buffer, uint8_t buflen){
  pCONT_sup->GetTextIndexed_P(buffer, buflen, id, PM_FX_MODES_NAMES_CTR);
  return buffer;
}





#endif // USE_DRIVER
