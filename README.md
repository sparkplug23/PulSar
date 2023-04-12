# PulSar

Firmware for _ESP8266_ and _ESP32_ based devices for integration with smart home systems. The project has been written to be highly modular, but also includes some bespoke firmware options for specific use cases. The modularity is intended to allow easy integration of additional sensors or drivers as the project grows. 

*The current release of this project into the public domain is to allow easy sharing with early adopters, who are helping to test and debug this project for future official public release.*

Note: The documentation on this project is currently sparse and not maintained as significant changes are ongoing to the firmware. Credit to the developers who have inspired and contributed to this code will be added when documentation is added.

## Credits

This project started as a bespoke controller for home heating, and slowly expanded with features and devices being added to control lights and add sensors for around the home. A year into the development, I discovered *Tasmota* and all it has to offer and have since started adding features found within Tasmota that are useful in my home automation environment. This has helped rapidly expand the scale of my project, but with continued development I aim to make this project unique as my skills as a programmer improves. Nevertheless, Tasmota has been and continues to be a great inspiration and I owe a great deal to their developer team ([Tasmota](https://github.com/arendst/Tasmota/releases/latest)). Additionally, the project *WLED*, has been extrememly useful learning resource and I equally am extremely grateful for the use of their software ([WLED](https://github.com/Aircoookie/WLED)). 

### Libraries Used (included in project)
Libraries used are:
- [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)
- [BME280](https://github.com/adafruit/Adafruit_BME280_Library)
- [DallasTemperature]()
- [DHTEsp]()
- [ESP Async WebServer]()
- [ESPAsyncTCP]()
- [IRremoteEsp8266](https://github.com/markszabo/IRremoteESP8266)
- [JSMN](https://github.com/zserge/jsmn)
- [NeoPixelBus](https://github.com/Makuna/NeoPixelBus)<sup>1</sup>
- [OneWire](https://github.com/PaulStoffregen/OneWire)
- [PubSubClient]()
- [TasmotaSerial]()
- [TasmotaModbus]()
- [rc-switch](https://github.com/sui77/rc-switch)

><sup>1</sup>Modified libraries include changes that make them operationally different to the original version, these changes are required for better integration with this project.

## Inspiration
Projects which gave me invaluable ideas of code that aided development:
- [Tasmota](https://github.com/arendst/Tasmota/releases/latest)
- [WLED](https://github.com/Aircoookie/WLED)

<!-- ## Features already supported by the project

### Drivers
These modules are used to control elements, currently these include:
- Buzzer
- Camera OV2640*
- FileSystem
- GPS
- InfraRed Transceiver
- LEDs
- Motors (HBridge)
- Pulse Width Modulation (PWM)
- RCSwitch ie SAW 433MHz Radios (Surface Acoustic Wave typically used in wireless doorbells) 
- Relays
- SD Card (grouped into filesystem?)
- Shelly Dimmer 2

### Sensors
The modules are used to measure aspects of the surrounding environment, currently these include:
- ADS1115ADC (Analog-to-digital converter)
- Analog Inputs
- APDS9960 Proximity
- BH1750 Lux Meter
- BME
- Buttons
- DB18x20 (ie DB18S20, DB18?20)
- DHT (ie DHT11 and DHT22)
- Door movements (note: phasing out to merge with basic IO reading and reporting via interface as motion)
- Doorbell (pn. Move to controller?)
- Moisture (eg, Resistive and Capacitive)
- PIR/Doppler motion sensors
- Pulse Counter
- Rotatory
- Switches
- TSL2561 Light
- Ultrasonic SR04 (movement and depth perception)

### Lights
- Addressable (ie. WS2811, WS2811, SK6812 with RGB and RGBW)
- PWM lights (1-5 channels and up to full RgbcctColour support)

### Energy
- ADE7953 
- INA219 Current sensor
- PZEM004Tv3

### Displays
- Nextion touch panels (limited support at this time)
- OLED SSD1606

### Controllers (Using the above modules as an all in one system)
- Blinds (ie. motors, analog and light sensing)
- Exercise Bike (ie. pulse counter)
- Fans PWM
- Sonoff iFan03
- HVAC (capable of using an ambient climate sensor as input per each controlled zone, with timers, temperature programs included scheduling)
- IR Projector
- TankVolume (ie. using the ultrasonic sensor to estimate volume) [pn. change to just tank, so water can also be measured]
- Radiator Fan (ie. use temperature sensor for enclosed radiators to activate ducted fans for improved room heating)
- Scheduler (TBD. A method of holding tasks to be run at a later or exact time)
- SD Card Logger
- Treadmill ie Sensor readings mapped into estimated outputs (eg. Using linear voltage to mph conversion for treadmill distances) -->

<!-- ## Features under development for future integration
- TBA -->

## Notes
- This project name is a placeholder, and will change later in development.
- Written with VSCode IDE with PlatformIO.
<!-- - [x]<sup>1</sup> extra comment. -->

## License

This program is licensed under GPL-3.0

<!-- # How to use the project
Since this project is still under development, it requires extra steps for others to deploy it in their network. This will become easier and eventually seamless with further development time. Here, I will list the steps that are required to get the system running under different environments. 

## Configure

1) The folder ```0_ConfigUser``` holds the files to configure the project, with a file named `mUserConfigSecret.h` containing each users configuration parameters. This is not shared via github, but is generated during the first compile (via template file located in the tools folder). Subsequent compiles will not change the `mUserConfigSecret.h` file.

2) Inside the file `mUserConfigSecret.h`, the user can add their unique values for their automation system. These include the SSID and password for up to three network connections, and if enabled, the mqtt broker IP address.

3) There are a number of example configurations for popular devices provided. Some noteworthy parameters are:
  * ```DEVICENAME_CTR``` is the unique name of that device, and mqtt will use this for the topic.
  * ```MODULE_TEMPLATE``` provides the device configuration, including its name, hardware, and the required pin functionality and is formatted using JSON commands. Some of the json keys (eg. `GPIOC`) can be omited, but `MODULE_TEMPLATE` is required for compiling.
  * (*optional*) ```LIGHTING_TEMPLATE``` is also formatted as json, however, this is optional configuration for the lighting modules during boot. This accepts any command which could also be executed via mqtt, webui or serial.
  * (*optional*) ```FUNCTION_TEMPLATE``` will also run any command, including the configuration of sensors and drivers during boot time.
  * Additionally, by defining any module as enabled including its required pin function via `GPIOC`, all sensors and drivers are available. It must be noted, this is highly developmental firmware and some modules may either have bugs, or not work at all.

## Uploading firmware

4) The file named ```platformio.ini``` has a variable named `default_envs`. Each (on a new line) device in the format `esp_8266_<devicename>` will be compiled. At least one must be provided, but multiple devices can be built and flashed at once.

```cpp
default_envs = 
    esp_8266_customuser_example_serial
    ; esp_8266_customuser_example_ota
```

5) The file named ```platformio_env8266.ini``` must contain the environment named in the previous step (ie `esp_8266_<devicename>`). For serial flashing, the minimal environment should have these as a minimum.

```cpp
[env:esp_8266_rgbcustomuser01]
build_flags = ${common.build_flags} 
                -D DEVICE_RGBCUSTOM_USER_01
                -w
upload_port = COM3
```

Similarly, for OTA flashing, the `upload_port` must be changed to the IP and `upload_protocol` must be set to `espota`.

```cpp
[env:esp_8266_rgbcustomuser01]
build_flags = ${common.build_flags} 
                -D DEVICE_RGBCUSTOM_USER_01
                -w
upload_protocol = espota
upload_port = 192.168.1.79
``` -->


# How to use the project (Design methodology)

<!-- 1) On first compile, it will likely fail, let me know. -->
2) The `mFirmwareCustom_Secret.h` and `mUserConfig_Secret.h` files should be generated on second compile.
3) `mFirmwareCustom_Secret.h` contains the hardware layout. The device/mqtt name, extra GPIO (more than its template) and any JSON commands to be run during startup.
4) `mUserConfig_Secret.h` contains the code layout. Defining Wifi and MQTT are neccesary right now. Additional changes to default settings found in `mBaseConfig.h` can also be added here.
5) `platformio_override` is the main ini file, it can compile both esp8266 and esp32 builds (under `default_envs`)
6) If more than one `default_envs` is uncommented, you can flash multiple devices. To do this, you must define the `DEVICENAME_x` in the `platformio_override` for the respective build, and not have it defined elsewhere. If another `DEVICE_x` meant for a different device is defined elsewhere, the compiler will complain, so you can't accidently flash code meant for another device by leaving a `DEVICE_x` set elsewhere. 
7) I will be removing `platformio_override.ini` from git in the future, replacing it with `platformio_override_sample`. Since new files are generated by running the `ini` file, I can not generate the `platformio_override.ini` by compiling, instead you must rename the `_sample` file manually. For now, expect PULLING FROM GIT TO ERASE THIS FILE. 

# Preliminary Documentation

This file will be the outline of documentation to come later, with bullet points describing what each module can do, and plans for the future.

# Overview

The project has a layout with related code being grouped into classes, also reffered to as "modules", since each class performs a specialised task. The modules are further grouped by the type of task they perform (e.g., Networking, Sensors) and placed into their own folders [Figure 1].

![image](https://user-images.githubusercontent.com/35019746/118510354-cc5aa700-b728-11eb-8245-7344dad1cb4a.png)

These folders can be summarised as follows:
  * `0_ConfigUser` - Allows configuration of the desired functioanility of the project, configurable by the user.
  * `1_TaskerManager` - This is the primary entry point for all modules, together with the `loop()` and `setup()` from `HomeControlSystem.cpp`, the tasks, also known as "Functions" will be executed when needed. This is effectively a multitasking thread controller, as it allows each active module to share resources by limiting how long each function takes up in CPU time, including splitting functions into multiple split tasks where they would otherwise be "blocking" or waiting code. An example of this is requesting a sensor reading, then returning to the module to check for its result instead of waiting for the result. 
  * `2_CoreSystem` - This contains modules which are shared resources by all hardware, and are often all required for each project. 
  * `2b_Internal_TaskerSystems` - Shared resources, but are not need for most builds (e.g. SolarLunar positions)
  * `2c_Internal_IsolatedNoTaskerSystems` - Shared resources that can be used by other classes, but contains no "Tasker" calls.
  * `3_Network` - Includes all networking modules required for communicating with a host, this does not include any remote devices that would fall into a hardware addon (ie 433mhz radios).
  * `4_Drivers` - All modules which directly control an external device, connected via GPIO pins.
  * `5_Sensors` - All modules that receive data from external devices, connected via GPIO pins.
  * `6_Lights` - Control any connected devices that control lights, including addressable leds or pwm controlled hardware. 
  * `7_Energy` - All modules that receive energy data from connected hardware, connected via GPIO pins.
  * `8_Displays` - Special drivers for controlling connected displays.
  * `9_Controller` - Bespoke modules which use the other IO modules (eg drivers, sensors and displays) to perform as a unified system. Examples include HVAC controllers, which can use any sensor as input to control any driver as output. (Note: For simple reactive controls, rules may be used instead.)

# Module Details

## ConfigUser

## TaskerManager

## CoreSystem

## Network

## Drivers

## Sensors

This folder contains the code required to read all connected devices that report measurements from the real world. The first module in this folder, is the interface module, which contains shared resources that all sensors use and is responsible for enabling unified sensor readings. This module acts as an application layer so any other module can access sensor readings by type, without requiring knowledge of the physical connected hardware. 

The complete list of sensors are:

ModuleName | Measures | Range/Accuracy | Pin Interface | GPIO Name (Enum) | Supply Voltage
| :--- | :--- | :--- | :---  | :---  | :--- 
`Interface` | Unifying resources and readings | |
`ADS1115ADC` | Voltage | | I2C |
`Analog` | Voltage | 0..1023 | Analog | |
`APDS9960` | Proximity |  |
`BH1750` | Light | | I2C
`BME` | Temperature <BR> Humidity <BR> Pressure | -40…85°C (±1.0°C) <BR> 0..100 (±3%) <BR> 300...1100 hPa (±0.25%) | I2C <BR> SPI | `"I2C SCL"`, `"I2C SDA"` <BR> (GPIO_I2C_SCL_ID, GPIO_I2C_SDA_ID) | 3.3V
`Buttons` | MomentaryChanges || Digital
`Counters` | Counter <BR> Frequency
`DB18x20` | Temperature
`DHT` | Temperature <BR> Humidity || | | 3-5V
`Door` | Open/Closed
`Moisture` | Moisture
`Motion` | Motion <BR> PIR <BR> Doppler
`MOTION` | Occupancy
`PulseCounter` | Counter^
`RemoteSensor` | Any
`Rotaty` | Rotational Counter?
`SeeSawSoil` | Moisture
`SR04` | Distance
`Switches` | StateChanges
`TSL2516` | Lux
`Ultrasonic` | Distance <BR> same as SR04?

 ### InterfaceSensor
 Unifying resources and readings
 
 ### ADS1115ADC (Voltage)
 ### Analog (Voltage)
 ### APDS9960 (Proximity)
 ### BH1750 (Light)
 ### BME (Temperature, Humidity, Pressure)
 https://www.bosch-sensortec.com/products/environmental-sensors/humidity-sensors-bme280/

`Buttons` (MomentaryChanges)
`Counters` (Counter, Frequency)
`DB18x20` (Temperature)
`DHT` (Temperature, Humidity)
`Door` (Open/Closed)
`Moisture` (Moisture)
`Motion` (Motion, PIR, Doppler)
`MOTION` (Occupancy)
`PulseCounter` (Counter^)
`RemoteSensor` (Any)
`Rotaty` (Rotational Counter?)
`SeeSawSoil` (Moisture)
`SR04` (Distance)
`Switches` (StateChanges)
`TSL2516` (Lux)
`Ultrasonic` (Distance, same as SR04?)

## Lights

## Energy

## Displays

## Controller


# Development Plans

## Release 1.0
Current code stages planned to complete before releasing code as first 1.0 stage.
- File System
- Settings saving to enable users without building to configure device with basic WebUI
- Multipin support of lighting
- Basic webpage for uploading recovery firmware to device that uses, either; hardcoded SSID/PW to connect to expected fallback network, or, creation of its own network that a computer can connect directly to update new software.
- All code must be either at release level and tested, or suppressed by `#define` to gaurentee code is stable

## Release 2.0
Longer term planned features.
- Ethernet support with POE on ESP32
- Full webpage support 
- ESP32 Camera working as new "Camera Sensor Node" that will use the camera, PIR and host of other sensors to save images on the filesystem and push images via Email and other methods. (e.g., Can motion triggering a 10 second video sent to the phone?)
- GSM support (e.g., Enabling data logging with GPS and 9-axis gyro to the local file system. GSM/LTE will enable the ability to connect remotely and pull the data as needed with wifi uploads when available)

# Branch Descriptions

# development_multipin (short term)
For merging WLED multiple methods and light types across multiple output gpio

# development_home (long term)
To become the primary home (whitehall) active branch, so any additions from other branches must be tested before merging into this

# development_blackbox (phased out)
Temporary branch to develop LTE enabled mqtt device,to be renamed "development_cellular" 

# development_cellular (long term)
Branch to include anything LTE related, including making network/mqtt work via both LTE and WiFi (including some Ethernet testing)

# development_filesystem (long term)
Testing filesystem, including internal memory and external SD cards. Ideally enabling data logging to SDs (json format) and later a webui to download those files with possible python script to convert into kml files for opening in google earth

# development_webui (long term)
Making webpages work fully on ESP32 and ESP8266. This will require significant rewrite and testing.