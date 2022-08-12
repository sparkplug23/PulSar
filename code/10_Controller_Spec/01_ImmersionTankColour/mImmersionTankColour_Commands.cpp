#include "mImmersionTankColour.h"

#ifdef USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR


void mImmersionTankColour::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  if(jtok = obj["ColourTimeOn"])
  {

    data.time_on->Start(jtok.getInt());

    data.mode = MODE_SHOW_COLOURS_GRADIENT_COLOUR_TEMP_ON_ID;

    ALOG_COM( PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K("ColourTimeOn")), data.time_on->Value() );

  }


}

/*

{
  "ColourPalette": 15,
  "PaletteEdit": {
    "ColourPalette": 15,
    "Data": [
      6,5,0,0,0,
  0,0,0,255,
  50,51,0,205,
  100,102,0,154,
  150,153,0,103,
  200,204,0,52,
  255,255,0,1
    ]
  },
  "AnimationMode": "Effects",
  "Effects": {
    "Function": "Static"
  },
  "Rate":1,
  "BrightnessRGB":100,
  "ColourTimeOn":600
}


{"TimeOn":521,"Mode":0,"encoded_gradient_temp_array":[
  
  6,5,0,0,0,
  
  0,255,0,0,
  51,0,255,0,
  102,0,0,255,
  153,255,0,255,
  204,255,255,0,
  255,0,255,255,
  
  
  3,127,252,253,57,156,228,22,112,255,153,219,249,129,132,19,28,188,52,249,189,96,161,120,88,16,95,91,186,103,186,20,151,191,42,79,88,81,156,10,212,229,130,94,37,93,35,189,132,103,252,165,18,193,84,80,133,210,3,221,176,17,129,194,241,197,206,203,213,9,249,187,211,46,83,82,36,232,185,109,243,229,255,140,248,100,19,119,173,118,175,143,230,224,202,236,33,244,226,103,219,240,24,147,198,203,69,33,62,218,129,250,232,251,144,67,111,183,92,89,251,243,151,122,116,101,223,51,59,189,141,174,188,170,237,18,97,211,67,189,239,214,178,9,169,185,111,169,73,66,4,107,103,254,179,38,182,69,78,0,129,254,125,127,126,97,197,225,166,51,176]}




*/

#endif // USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
