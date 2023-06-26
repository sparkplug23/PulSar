/******************************************************************
  DHT Temperature & Humidity Sensor library for Arduino & ESP32.

  Features:
  - Support for DHT11 and DHT22/AM2302/RHT03
  - Auto detect sensor model
  - Very low memory footprint
  - Very small code

  https://github.com/beegee-tokyo/arduino-DHTesp

  Written by Mark Ruys, mark@paracas.nl.
  Updated to work with ESP32 by Bernd Giesecke, bernd@giesecke.tk

  GNU General Public License, check LICENSE for more information.
  All text above must be included in any redistribution.

  Datasheets:
  - http://www.micro4you.com/files/sensor/DHT11.pdf
  - http://www.adafruit.com/datasheets/DHT22.pdf
  - http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/Weather/RHT03.pdf
  - http://meteobox.tk/files/AM2302.pdf

  Changelog:
    2013-06-10: Initial version
    2013-06-12: Refactored code
    2013-07-01: Add a resetTimer method
    2017-12-12: Added task switch disable
                Added computeHeatIndex function from Adafruit DNT library
    2017-12-14: Added computeDewPoint function from idDHTLib Library
                Added getComfortRatio function from libDHT Library
    2017-12-15: Added computePerception function
    2018-01-02: Added example for multiple sensors usage.
    2018-01-03: Added function getTempAndHumidity which returns temperature and humidity in one call.
    2018-01-03: Added retry in case the reading from the sensor fails with a timeout.
    2018-01-08: Added ESP8266 (and probably AVR) compatibility.
	2018-03-11: Updated DHT example    
    2018-06-19: Updated DHT example to distinguish between ESP8266 examples and ESP32 examples    
    2018-07-06: Fixed bug in ESP32 example    
******************************************************************/

// #include "DataSmoothing.h"

