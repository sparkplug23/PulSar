/*
  mFona_Cellular - infra red support

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

#include "mFona_Cellular.h"

#ifdef USE_MODULE_DRIVERS_FONA_CELLULAR

/**
 * @brief 
 * 
 * @return int8_t 
 */
int8_t mFona_Cellular::CommandGet_RSSI_dBm()
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
bool mFona_Cellular::CommandSet_Power()
{
  
  ALOG_INF( PSTR("CommandSet_Power PS:%d"), digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_STATUS__ID)) );

  if(digitalRead(pCONT_pins->GetPin(GPIO_FUNCTION__FONA_POWER_STATUS__ID)) == 1)
  {


  }





}



#endif
