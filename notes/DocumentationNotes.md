This file will be the outline of documentation to come later, with bullet points describing what each module can do, and plans for the future.

# Overview

The project has a layout with related code being grouped into classes, also reffered to as "modules", since each class performs a specialised task. The modules are further grouped by the type of task they perform (e.g., Networking, Sensors) and placed into their own folders [Figure 1].

![image](https://user-images.githubusercontent.com/35019746/118510354-cc5aa700-b728-11eb-8245-7344dad1cb4a.png)

These folders can be summarised as follows:
  * `0_ConfigUser` - Allows configuring what sections of the project should be included, configurable by the user.
  * `1_TaskerManager` - This is the primary entry point for all modules, together with the `loop()` and `setup()` from `HomeControlSystem.cpp`, the tasks, also known as "Functions" will be executed when needed. This is effectively a multitasking thread controller, as it allows each active module to share resources by limiting how long each function takes up in CPU time, including splitting functions into multiple split tasks where they would otherwise be "blocking" or waiting code. An example of this is requesting a sensor reading, then returning to the module to check for its result instead of waiting for the result. 
  * `2_CoreSystem` - This contains modules which are shared resources by all hardware, and are often all required for each project. 
  * `3_Network` - Includes all networking modules required for communicating with a host, this does not include any remote devices that would fall into a hardware addon (ie 433mhz radios).
  * `4_Drivers` - All modules which directly control an external device, connected via GPIO pins.
  * `5_Sensors` - All modules that receive data from external devices, connected via GPIO pins.
  * `6_Lights` - Control any connected devices that control lights, including addressable leds or pwm controlled hardware. 
  * `7_Energy` - All modules that receive power data from connected hardware, connceted via GPIO pins.
  * `8_Displays` - Special drivers for controlling connected displays.
  * `9_Controller` - Bespoke modules which use the other IO modules (eg drivers, sensors and displays) to perform as a unified system. Examples include HVAC controllers, which can use any sensor as input to control any driver as output.


# Module Details

## ConfigUser

## TaskerManager

## CoreSystem

## Network

## Drivers

## Sensors

This folder contains the code required to read all connected devices that report measurements from the real world. The first module in this folder, is the interface module, which contains shared resources that all sensors use and is responsible for enabling unified sensor readings, effectively acting as an application layer so any other module can access sensor readings by type, without requiring knowledge of the physical connected hardware. 

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
`Presence` | Occupancy
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
`Presence` (Occupancy)
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
