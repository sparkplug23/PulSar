
# Reason for making major revision increase (ie major milestones)

## FIRMWARE_VERSION_MINOR



### 0.112.x.x : Moving back for December 2022
 - 112 is working with lighting branch only
 - All christmas lights should be built on this 112 version


### 0.110.x.x : Moving to MQTT:Crackle
 - Moving LongTerm devices over to new mqtt/OH starting with consumer unit
 - Also new "openhab_whitehall" database



### 0.109.x.x : Push to making palettes work for all effects
- Large rewrite of animations, and how colours are generated
- removing TransitionOrder, will become part of animations


### 0.108.0.0 : File Names Changed
- Sub folder for sensors/drivers have numbers added, and `class_id` now uses `uint16_t` format with multiples of 1000 for major group (core/sensor/driver/controller) and 0-999 for sub folders for each module/class within that group
- Cleaning up of home automation, moving permanent devices into its own ini file `whitehall` that uses both esp32 and esp8266 together
- Considering, but not yet implemented, a branch for just my `whitehall` automation that can slowly take it final (released) devices much later.



### 0.100.0.0 : Measurement System functional
- Marking the measurement system on esp32 as functional

