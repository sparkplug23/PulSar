
# Reason for making major revision increase (ie major milestones)

## FIRMWARE_VERSION_MINOR

### 0.100.0.0 : Measurement System functional
- Marking the measurement system on esp32 as functional


### 0.108.0.0 : File Names Changed
- Sub folder for sensors/drivers have numbers added, and `class_id` now uses `uint16_t` format with multiples of 1000 for major group (core/sensor/driver/controller) and 0-999 for sub folders for each module/class within that group
- Cleaning up of home automation, moving permanent devices into its own ini file `whitehall` that uses both esp32 and esp8266 together
- Considering, but not yet implemented, a branch for just my `whitehall` automation that can slowly take it final (released) devices much later.

