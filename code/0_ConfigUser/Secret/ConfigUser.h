#pragma once

// ============================================================================
// 0_ConfigUser/ConfigUser.h
//
// Single include point for all user/firmware configuration.
// Include this from code instead of including individual Secret headers.
// ============================================================================



#include "00_Developing/Firmware__ActiveDevelopment.h"
#include "00_Developing/Firmware__Testbeds.h"
#include "00_Developing/Firmware__Validation.h"

#include "01_DevelopingByTypes/FirmwareGroup_BoardBuilds.h"
#include "01_DevelopingByTypes/FirmwareGroup_Cameras.h"
#include "01_DevelopingByTypes/FirmwareGroup_CellularDatalinks.h"
#include "01_DevelopingByTypes/FirmwareGroup_HVAC.h"
#include "01_DevelopingByTypes/FirmwareGroup_LightingEffects.h"
#include "01_DevelopingByTypes/FirmwareGroup_MotionDetectors.h"
#include "01_DevelopingByTypes/FirmwareGroup_PZEM.h"

#include "02_Whitehall/Firmware__Home__Secret.h"
#include "02_Whitehall/Firmware__Temporary__Secret.h"

#include "03_Meadows/FirmwareConfig_Deployed.h"

#include "04_RemoteLocations/05_mFirmwareCustom_Secret__Colorado_2024.h"
#include "04_RemoteLocations/05_mFirmwareCustom_Secret__Colorado_2025.h"
#include "04_RemoteLocations/05_mFirmwareCustom_Secret__NITC.h"

#include "05_Seasonal/Firmware__Christmas_2023__Secret.h"
#include "05_Seasonal/Firmware__Christmas_2024__Secret.h"
#include "05_Seasonal/Firmware__Christmas_2025__Secret.h"

#include "06_TemplateBuilds_From_INI/TemplateBase__Drivers.h"
#include "06_TemplateBuilds_From_INI/TemplateBase__Lighting.h"
#include "06_TemplateBuilds_From_INI/TemplateBase__Sensors.h"
#include "06_TemplateBuilds_From_INI/TemplateBase__HardwareSpecific.h"
