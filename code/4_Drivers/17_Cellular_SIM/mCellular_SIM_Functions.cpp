/*
  mCellular_SIM7000 - infra red support

  Copyright (C) 2022  Heiko Krupp, Lazar Obradovic, Theo Arends and Michael Doone

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

#include "mCellular_SIM.h"

#ifdef USE_MODULE_DRIVERS__CELLULAR_SIM7000

/**
 * @brief 
 * 
 * @return int8_t 
 */
int8_t mCellular_SIM7000::CommandGet_RSSI_dBm()
{

  uint8_t n = fona->getRSSI();
  int8_t r;
  
  if (n == 0) r = -115;
  if (n == 1) r = -111;
  if (n == 31) r = -52;
  if ((n >= 2) && (n <= 30))
  {
    r = map(n, 2, 30, -110, -54);
  }

  ALOG_INF( PSTR("Rssi = %d, dBm = %d"), n, r);

  return r;
}

/**
 * @brief Set power
 * 
 * @return true 
 * @return false 
 */
bool mCellular_SIM7000::CommandSet_Power()
{
  
  ALOG_INF( PSTR("CommandSet_Power PS:%d"), digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_STATUS__ID)) );

  if(digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_STATUS__ID)) == 1)
  {


  }





}


const char* mCellular_SIM7000::GetNetworkStatusNameByID(uint8_t id, char* buffer, uint16_t buflen){
  switch(id){
    default:
    case 0: memcpy_P(buffer, PM_NETWORK_STATUS__NOT_REGISTERED__CTR , sizeof(PM_NETWORK_STATUS__NOT_REGISTERED__CTR)); break;
    case 1: memcpy_P(buffer, PM_NETWORK_STATUS__REGISTERED_HOME__CTR , sizeof(PM_NETWORK_STATUS__REGISTERED_HOME__CTR)); break;
    case 2: memcpy_P(buffer, PM_NETWORK_STATUS__NOT_REGISTERED_SEARCHIN__CTR , sizeof(PM_NETWORK_STATUS__NOT_REGISTERED_SEARCHIN__CTR)); break;
    case 3: memcpy_P(buffer, PM_NETWORK_STATUS__DENIED__CTR , sizeof(PM_NETWORK_STATUS__DENIED__CTR)); break;
    case 4: memcpy_P(buffer, PM_NETWORK_STATUS__UNKNOWN__CTR , sizeof(PM_NETWORK_STATUS__UNKNOWN__CTR)); break;
    case 5: memcpy_P(buffer, PM_NETWORK_STATUS__REGISTERED_ROAMING__CTR , sizeof(PM_NETWORK_STATUS__REGISTERED_ROAMING__CTR)); break;
  }
  return buffer;
} 

#endif
