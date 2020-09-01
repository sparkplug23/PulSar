#ifndef _MCEILINGFAN_H
#define _MCEILINGFAN_H 0.3

#include "1_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_CUSTOM_CEILING_FAN

#include "2_CoreSystem/InterfaceController/mInterfaceController.h"


#include <ArduinoJson.h>

#include <string.h>
#include <strings.h>

#define D_TASKNAME_CEILINGFAN "ceilingfan" //Used as part of mqtt command

DEFINE_PROGMEM_CTR(kListFanControls) "Light|Off|Low|Medium|High";

class mCeilingFan {

  private:
  public:
    mCeilingFan(){};
    int8_t Tasker(uint8_t function);

    #define CEILINGFAN_TOGGLE  0xA55595
    #define CEILINGFAN_DIMM    0xA55955
    #define CEILINGFAN_SPEED0  0xA55655
    #define CEILINGFAN_SPEED1  0xA55557
    #define CEILINGFAN_SPEED2  0xA55565
    #define CEILINGFAN_SPEED3  0xA5655B
    #define ONE_BIT_TIME 1252 //+- 30 std
    #define ZERO_BIT_TIME 439 //+- 30 std

    int8_t parse_JSONCommand();
    //void AddToJsonObject_AddHardware(JsonObject root);
    void AddToHardwareMessage();

    
void WebCommand_Parse(void);


void WebAppend_Root_Draw_PageTable();
void WebAppend_Root_Status_Table();

};

#endif

#endif