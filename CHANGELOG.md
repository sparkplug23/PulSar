# Changelog
All notable changes to this project will be documented in this file.

Section Next, will always be "Development", which with copy and paste will become the version below it as Released (uploaded to github)

## [Unreleased] - Development

## [B0.83.80.109]
### Added
- Custom light version for first beta tester
- Ifdef to disable additional webpages (beyond root) and the related buttons for stability testing of root page
- compression of OTA using python script
- Ability to disable webserver, mqtt, ntp while still enabling system to run via defaults in progmem (eg outside tree lighting)

### Breaking Changed
- esp32 neopixel not working with wifi
- Disabled webserver 

### Changed
- refactored ``ini`` files to allow esp8266 and esp32 compiles
- ``INFO_PARSING`` is now reffered to as ``COMMANDS`` as values are set with `CommandSet_x` functions to consolate `AddLog`
- `AnimationProfile` function/enum list changed to `SetLightPowerState`
- Refactored commands in lightinterface and animator (to be renamed `mRGBAddressable`)
- Created more files (cpp) under lighting to split code segments out for easier reading
- Cleaned `LanguageDefaults`

### Fixed
- RTC code to use interrupts, eliminating time drift

### Removed
- none



## [Released - Uploaded to Github]

## [9.0.0.3] - 18-12-2020 - Template for subheadings
### Added
- template

### Breaking Changed
- template

### Changed
- template

### Fixed
- templete

### Removed
- template
