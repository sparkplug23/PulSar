/**
  @brief mUserConfig_Example.h - This file will override "mBaseConfig.h" and will only be regenerated if the file does not exist

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
**/
#ifndef MUSERCONFIG_EXAMPLE_HEADER
#define MUSERCONFIG_EXAMPLE_HEADER

#include "stdint.h"
#include "2_CoreSystem/mSystemConfig.h"
#include "0_ConfigUser/mFirmwareCustom_Secret.h"

/*********************************************************************************************\
 * This file will override defines in "mBaseConfig.h", and is only regenerated at compile time if the file does not exist
 * This allows the user to have their own build configuration that wont be changed with future git downloads.
\*********************************************************************************************/

// Wifi

#undef  STA_SSID1
#define STA_SSID1            ""                // [Ssid1] Wifi SSID
#undef  STA_PASS1
#define STA_PASS1            ""                // [Password1] Wifi password

#undef  STA_SSID2
#define STA_SSID2            ""                // [Ssid2] Optional alternate AP Wifi SSID
#undef  STA_PASS2
#define STA_PASS2            ""                // [Password2] Optional alternate AP Wifi password

#undef  STA_SSID3
#define STA_SSID3            ""                // [Ssid2] Optional alternate AP Wifi SSID
#undef  STA_PASS3
#define STA_PASS3            ""                // [Password2] Optional alternate AP Wifi password


// MQTT
#undef D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED //TO BE REPLACED WITH FOLLOWING METHOD
#define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED 192,168,1,65
#undef MQTT_HOST
#define MQTT_HOST  "192.168.1.65" //Parsing the IP String will be the new method


// Belfast
#undef LATITUDE
#define LATITUDE               54.6         // [Latitude] Your location to be used with sunrise and sunset
#undef LONGTITUDE
#define LONGITUDE              5.9          // [Longitude] Your location to be used with sunrise and sunset


#endif // MUSERCONFIG_EXAMPLE_HEADER


