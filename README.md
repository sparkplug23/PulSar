# Home Automation Control System (HACS)

Firmware for _ESP8266_ and _ESP32_ based devices for integration with smart home systems. The project has been written to be highly modular, but also includes some bespoke firmware options for specific use cases. The modularity, is intended to allow easy integration of additional sensors or drivers as the project expands. 

## Credits

This project started as a bespoke controller for home heating, and slowly grew over the space of a year with features and devices being added to control lights and add sensors for around the home. A year into the development, I discovered Tasmota and all it has to offer and have since started adding features found within Tasmota that are useful in my home automation environment. This has helped jump start the projects scope, and in the near term will probably include many simularities with Tasmota, but over time I aim to diverge this project into its own identity as I have time to develop it further. Nevertheless, Tasmota has been and continues to be a great inspiration and I owe a great deal to their developer team ([Tasmota](https://github.com/arendst/Tasmota/releases/latest)).

### Libraries Used (included in project)
Libraries used with HACS are:
- [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)
- [BME280]()
- [IRremoteEsp8266](https://github.com/markszabo/IRremoteESP8266)
- [NeoPixelBus](https://github.com/Makuna/NeoPixelBus)<sup>1</sup>
- [OneWire](https://github.com/PaulStoffregen/OneWire)
- [rc-switch](https://github.com/sui77/rc-switch)

><sup>1</sup>Modified libraries include changes that make them operationally different to the original version, these changes are required for better integration with this project.

## Features already supported by the project

### Drivers

### Sensors

### Displays

## Features under development for future integration

## Notes
- HACS is pronounced as "Hex/Hacks".
- Written with VSCode IDE and PlatformIO.
- [x]<sup>1</sup> extra comment.

## License

This program is licensed under GPL-3.0


## How to use the project
Since this project is still under development, it requires extra steps for others to deploy it in their network. This will become easier and eventually seamless with further development time. Here, I will list the steps that are required to get the system running under different situations. 




