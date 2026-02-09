/*
  mServerResetRelays.h 
  
  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef HEADER_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS_H
#define HEADER_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS_H
  
#include "1_TaskerManager/mTaskerManager.h"
#define D_UNIQUE_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS_ID   10010 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS

// later, can I make this cycling through all sensors and show them line by line, and/or, show energy line by line

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"


#ifdef USE_MODULE_NETWORK_WEBSERVER
#include "Web/Gen/generated_web.h"
#endif


class mServerResetRelays :
  public mTaskerInterface
{

  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mServerResetRelays(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);

    static constexpr const char* PM_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS_CTR = D_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS_ID; }    
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void EverySecond();
    void SubTask_UpdateOLED();

    struct RUNTIME{
      struct RELAY_CONTROLS{
        uint32_t time_last_changed = 0;
      };
      RELAY_CONTROLS rel[8];
    }rt;
    
    
    /******************************************************************************************************************
     * SECTION: Web
    *******************************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebPage_Root_AddHandlers();
    void Serve_Submodule_ServerResetRelays(AsyncWebServerRequest* request);
    void Serve_Submodule_ServerResetRelays_Post(AsyncWebServerRequest* request);
    void Serve_Submodule_ServerResetRelays_JS(AsyncWebServerRequest* request);
    void Serve_Submodule_ServerResetRelays_Page(AsyncWebServerRequest* request);
    uint8_t poll_rate_s = 10; // default idle rate
    #endif


    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    std::vector<struct handler<mServerResetRelays>*> mqtthandler_list;
    struct handler<mServerResetRelays> mqtthandler_settings;    struct handler<mServerResetRelays> mqtthandler_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif // USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED

#endif
