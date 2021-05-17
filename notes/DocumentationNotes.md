This file will be the outline of documentation to come later, with bullet points describing what each module can do, and plans for the future.

# Overview

The project has a layout with related code being grouped into classes, also reffered to as "modules", since each class performs a specialised task. The modules are further grouped by the type of task they perform (e.g., Networking, Sensors) and placed into their own folders [Figure 1].

![image](https://user-images.githubusercontent.com/35019746/118504784-bc8c9400-b723-11eb-8e76-422709fb901c.png)

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


# Description of what each folder contains

## ConfigUser

## TaskerManager

## CoreSystem

## Network

## Drivers

## Sensors

## Lights

## Energy

## Displays

## Controller
