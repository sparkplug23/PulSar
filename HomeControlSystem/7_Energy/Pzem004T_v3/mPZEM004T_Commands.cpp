#include "mPZEM004T.h"

#ifdef USE_MODULE_ENERGY_PZEM004T_V3


void mEnergyPZEM004T::parse_JSONCommand(JsonParserObject obj)
{

  AddLog(LOG_LEVEL_TEST, PSTR( DEBUG_INSERT_PAGE_BREAK "mEnergyInterface::parse_JSONCommand"));
  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  
  if(jtok = obj[D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR].getObject()["SearchForDevices"])
  {

// This should become its own command, which can internally be used to search for connected devices during init.

      uint8_t found_address = 0;
      uint8_t modbus_buffer[30] = {0}; 

      for(uint8_t address_search=0;address_search<30;address_search++)
      {

        modbus->Send(address_search, 0x04, 0, 10);      
        found_address = 0;
        timeout = millis();
        while(abs(millis()-timeout)<200)
        {

          // ESP.wdtFeed();

          if(modbus->ReceiveReady())
          { 
            uint8_t error = modbus->ReceiveBuffer(modbus_buffer, 10);
            found_address = modbus_buffer[2]; // addres byte
            AddLog(LOG_LEVEL_TEST, PSTR("MODBUS Address =%d %d FOUND"),found_address,address_search);
            break; // out of the while
          }
        }
        
        AddLog(LOG_LEVEL_TEST, PSTR("MODBUS Address =%d %d"),found_address,address_search);

      }



  }
   
}


#endif // USE_MODULE_SENSORS_INTERFACE
