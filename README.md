
UNDERDEVELOPMENT -- Not ready for public use until v1.0 is reached

# Home Automation Control System (HACS)

Firmware for _ESP8266_ and _ESP32* based devices_ with **web UI, OTA updates, Nextion displays and sensor support** for integration with smart home systems. Written for VSCode IDE with PlatformIO.
Project has been written to be highly modular, but also includes some bespoke firmware options for specific use cases.

Currently supports enabling functions on devices:
- Nextion displays (ESP32 only)
- WS28xx RGB(W)
- BME280 climate sensors
- DHT11/22 climate sensors
- Relay controls (up to 4)
- SAW radios (488 Mhz transceivers such as those found in low cost plugs and garage openers)
- Ultrasonic distance measuring (with optional temperature compensation)
- Motion sensing (PIR)
- Door sensing (magnetic reed sensor)
- Door bell (button + chime)

Specific hardware support/functions for (details below):
- Heating controller (2 zones radiators + 2 zone water heating)
- Oil Tank level sensing with temperature compensaiton
- Radiator fan controller for augmenting enclosed convection radiator with fans
- Security lights (motion + lighting control)

If you like **Home Automation Control System**, give it a star, or fork it, and contribute!

## Contribute
You can contribute to HACS by:
- providing Pull Requests (Features, Proof of Concepts, Language files or Fixes)
- testing new released features and report issues
- donating to acquire hardware for testing and implementing or out of gratitude
- contributing missing documentation for features and devices

[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/sparkplug2324)

## Credits

This project started as a bespoke controller for home heating, and slowly grew over the space of a year with features and devices being added to control lights and add sensors for around the home. A year into the development, I discovered Tasmota and all it has to offer and have since started adding features found within Tasmota that are useful in my home automation environment. This has helped jump start my projects scope, and in the near term will probably include many simularities with Tasmota, but over time I aim to diverge this project into its own identity as I have time to develop it further. Nevertheless, Tasmota has been and continues to be a great inspiration and I owe a great deal to their developer team ([Tasmota](https://github.com/arendst/Tasmota/releases/latest)).

### Libraries Used
Libraries used with HACS are:
- [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)
- [ArduinoJson](https://arduinojson.org/)
- [BME280]()
- [Bosch BME680](https://github.com/BoschSensortec/BME680_driver)
- [IRremoteEsp8266](https://github.com/markszabo/IRremoteESP8266)
- [NeoPixelBus](https://github.com/Makuna/NeoPixelBus)
- [OneWire](https://github.com/PaulStoffregen/OneWire)
- [PubSubClient](https://github.com/knolleary/pubsubclient) (Modified)
- [rc-switch](https://github.com/sui77/rc-switch)

>Modified libraries include changes that make them operationally different to the original version, these changes are required for better integration with this project.

## Features under development

### Short-term (actively working on)
<!-- - Adding eeprom settings storing
- WebUI controls and basic status feedback for all devices/tasks currently supported with mqtt -->

### Long-term (planned)
<!-- - Other sensors such as:
  - current sensing
  - presence detection
- Webcommands in mqtt format (topic as task, command in json)   -->

## License

This program is licensed under GPL-3.0
