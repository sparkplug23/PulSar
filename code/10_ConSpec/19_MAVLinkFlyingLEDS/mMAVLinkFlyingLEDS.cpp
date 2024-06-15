/**

  Copyright (C) 2022    Michael Doone

  @note Relys on all other modules (no timers), and acts in place of rules

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "mMAVLinkFlyingLEDS.h"

#ifdef USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS

const char* mMavlinkFlyingLEDS::PM_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS_CTR = D_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS_CTR;
const char* mMavlinkFlyingLEDS::PM_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS_FRIENDLY_CTR = D_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS_FRIENDLY_CTR;


int8_t mMavlinkFlyingLEDS::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function)
  {    
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:
      EverySecond();
    break;
    case FUNC_LOOP:
      EveryLoop();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORKS_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break; 
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif  
  }

}//end



void mMavlinkFlyingLEDS::Pre_Init(void)
{

}


void mMavlinkFlyingLEDS::Init(void)
{

  module_state.mode = ModuleStatus::Running;

}


void mMavlinkFlyingLEDS::EverySecond()
{
  
  // SubTask_UpdateOLED();

  // uint16_t throttle_pwm = tkr_mavlink->pkt.rc_channels.data.chan3_raw;

  // uint16_t hue = map(throttle_pwm, 1000, 2000, 0, 360);

  // ALOG_INF(PSTR("Throttle: %d, Hue: %d"), throttle_pwm, hue);

  // HsbColor color = HsbColor(hue/360.0f, 1, 1);

  // pCONT_lAni->SEGMENT_I(0).rgbcctcolors[0] = color;
  // pCONT_lAni->SEGMENT_I(0).palette_id = 0;
  // pCONT_lAni->SEGMENT_I(0).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;

}

void mMavlinkFlyingLEDS::Effect_LandingLights()
{


  /**
   * @brief Segment0 [Wings]
   **/
  pCONT_lAni->SEGMENT_I(0).rgbcctcolors[0] = RgbcctColor(255,255,255,255,255); //white
  pCONT_lAni->SEGMENT_I(0).palette_id = 0;
  pCONT_lAni->SEGMENT_I(0).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  pCONT_lAni->SEGMENT_I(0).cycle_time__rate_ms = FRAMETIME_MS;
  pCONT_lAni->SEGMENT_I(0).speed = 255;
  pCONT_lAni->SEGMENT_I(0).setBrightnessRGB(0);
  pCONT_lAni->SEGMENT_I(0).setBrightnessCCT(255);
  /**
   * @brief Segment1 [Wings]
   **/
  pCONT_lAni->SEGMENT_I(1).rgbcctcolors[0] = RgbcctColor(255,255,255,255,255); //white
  pCONT_lAni->SEGMENT_I(1).palette_id = 0;
  pCONT_lAni->SEGMENT_I(1).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  pCONT_lAni->SEGMENT_I(1).cycle_time__rate_ms = FRAMETIME_MS;
  pCONT_lAni->SEGMENT_I(1).speed = 255;
  pCONT_lAni->SEGMENT_I(1).setBrightnessRGB(255);
  pCONT_lAni->SEGMENT_I(1).setBrightnessCCT(255);
  /**
   * @brief Segment1 [Hatches] - when landing, make the hatches be chasing (for power reserve) but match rudder solid for altitude
   **/
  // For testing of full range, lets use "seconds" as the ticker
  HsbColor color_hatch_chase = HsbColor(0, 1, 1);
  #ifdef ENABLE_DEVFEATURE_MAVLINK__USE_TIME_FOR_RANGE_TESTING
  hue = map(pCONT_time->RtcTime.second, 0,59, 0,359);
  ALOG_INF(PSTR("Hue: %d"), hue);
  color_hatch_chase = HsbColor(hue/360.0f, 1, 1);
  #else
    #ifdef ENABLE_DEVFEATURE_MAVLINK__USE_MANUAL_FOR_RANGE_TESTING

    float altitude = test_float;

    if(altitude > 50)
    { 
      color_hatch_chase = HsbColor(240/360.0f, 1, 1); 
    }else{
      altitude = constrain(altitude, 0, 50);
      float hue = mSupport::mapfloat(altitude, 0, 50, 0.0f, 1.0f);
      color_hatch_chase = HsbColor(hue, 1, 1);
    }

    #else
      uint16_t pwm_led_mode = 0;
      mMAVLink_Decoder::PacketData* message = tkr_mavlink->findPacketData(MAVLINK_MSG_ID_RC_CHANNELS);
      if (message != nullptr && message->data != nullptr) {
          mavlink_rc_channels_t* rc_channels_data = static_cast<mavlink_rc_channels_t*>(message->data);
          pwm_led_mode = rc_channels_data->chan15_raw;
      }
    #endif
  #endif
  pCONT_lAni->SEGMENT_I(2).rgbcctcolors[0] = color_hatch_chase;
  pCONT_lAni->SEGMENT_I(2).rgbcctcolors[1] = RgbcctColor(0,0,0,0,0); //black
  pCONT_lAni->SEGMENT_I(2).palette_id = mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
  pCONT_lAni->SEGMENT_I(2).effect_id = 71;//mAnimatorLight::EFFECTS_FUNCTION__CHASE_THEATER__ID;
  pCONT_lAni->SEGMENT_I(2).cycle_time__rate_ms = FRAMETIME_MS;
  pCONT_lAni->SEGMENT_I(2).speed = 255;
  pCONT_lAni->SEGMENT_I(2).cycle_time__rate_ms = 25;
  pCONT_lAni->SEGMENT_I(2).reverse = true;
  pCONT_lAni->SEGMENT_I(2).setBrightnessRGB(50);
  pCONT_lAni->SEGMENT_I(2).setBrightnessCCT(0);
  /**
   * @brief Segment1 [Rudder] - height is colour
   **/
  HsbColor color_rudder = color_hatch_chase;
  pCONT_lAni->SEGMENT_I(3).rgbcctcolors[0] = color_rudder;
  pCONT_lAni->SEGMENT_I(3).palette_id = mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
  pCONT_lAni->SEGMENT_I(3).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  pCONT_lAni->SEGMENT_I(3).cycle_time__rate_ms = FRAMETIME_MS;
  pCONT_lAni->SEGMENT_I(3).speed = 255;
  pCONT_lAni->SEGMENT_I(3).cycle_time__rate_ms = 25;
  pCONT_lAni->SEGMENT_I(3).setBrightnessRGB(50);
  pCONT_lAni->SEGMENT_I(3).setBrightnessCCT(0);
  /**
   * @brief Segment4 [Nose]
   **/
  pCONT_lAni->SEGMENT_I(4).rgbcctcolors[0] = RgbcctColor(255,255,255,255,255); //white
  pCONT_lAni->SEGMENT_I(4).palette_id = 0;
  pCONT_lAni->SEGMENT_I(4).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  pCONT_lAni->SEGMENT_I(4).cycle_time__rate_ms = FRAMETIME_MS;
  pCONT_lAni->SEGMENT_I(4).speed = 255;
  pCONT_lAni->SEGMENT_I(4).setBrightnessRGB(255);
  pCONT_lAni->SEGMENT_I(4).setBrightnessCCT(255);



}

void mMavlinkFlyingLEDS::Effect_TakeoffLights()
{

  
  /**
   * @brief Segment0 [Wings]
   **/
  pCONT_lAni->SEGMENT_I(0).rgbcctcolors[0] = RgbcctColor(255,255,255,255,255); //white
  pCONT_lAni->SEGMENT_I(0).palette_id = 0;
  pCONT_lAni->SEGMENT_I(0).effect_id = mAnimatorLight::EFFECTS_FUNCTION__COLOR_WIPE_RANDOM__ID;// 71;//mAnimatorLight::EFFECTS_FUNCTION__CHASE_THEATER__ID;
  pCONT_lAni->SEGMENT_I(0).cycle_time__rate_ms = FRAMETIME_MS;
  // pCONT_lAni->SEGMENT_I(0).mirror = true; // so it comes from centre out
  // pCONT_lAni->SEGMENT_I(0).reverse = true;
  pCONT_lAni->SEGMENT_I(0).speed = 127;
  pCONT_lAni->SEGMENT_I(0).setBrightnessRGB(255);
  pCONT_lAni->SEGMENT_I(0).setBrightnessCCT(0);
  /**
   * @brief Segment1 [Wings]
   **/
  pCONT_lAni->SEGMENT_I(1).rgbcctcolors[0] = RgbcctColor(255,255,255,255,255); //white
  pCONT_lAni->SEGMENT_I(1).palette_id = 0;
  pCONT_lAni->SEGMENT_I(1).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  pCONT_lAni->SEGMENT_I(1).cycle_time__rate_ms = FRAMETIME_MS;
  pCONT_lAni->SEGMENT_I(1).speed = 255;
  pCONT_lAni->SEGMENT_I(1).setBrightnessRGB(255);
  pCONT_lAni->SEGMENT_I(1).setBrightnessCCT(255);
  /**
   * @brief Segment1 [Hatches] - when landing, make the hatches be chasing (for power reserve) but match rudder solid for altitude
   **/
  // For testing of full range, lets use "seconds" as the ticker
  HsbColor color_hatch_chase = HsbColor(0, 1, 1);
  #ifdef ENABLE_DEVFEATURE_MAVLINK__USE_TIME_FOR_RANGE_TESTING
  hue = map(pCONT_time->RtcTime.second, 0,59, 0,359);
  ALOG_INF(PSTR("Hue: %d"), hue);
  color_hatch_chase = HsbColor(hue/360.0f, 1, 1);
  #else
    #ifdef ENABLE_DEVFEATURE_MAVLINK__USE_MANUAL_FOR_RANGE_TESTING

    float altitude = test_float;

    if(altitude > 50)
    { 
      color_hatch_chase = HsbColor(240/360.0f, 1, 1); 
    }else{
      altitude = constrain(altitude, 0, 50);
      float hue = mSupport::mapfloat(altitude, 0, 50, 0.0f, 1.0f);
      color_hatch_chase = HsbColor(hue, 1, 1);
    }

    #else
      uint16_t pwm_led_mode = 0;
      mMAVLink_Decoder::PacketData* message = tkr_mavlink->findPacketData(MAVLINK_MSG_ID_RC_CHANNELS);
      if (message != nullptr && message->data != nullptr) {
          mavlink_rc_channels_t* rc_channels_data = static_cast<mavlink_rc_channels_t*>(message->data);
          pwm_led_mode = rc_channels_data->chan15_raw;
      }
    #endif
  #endif
  pCONT_lAni->SEGMENT_I(2).rgbcctcolors[0] = color_hatch_chase;
  pCONT_lAni->SEGMENT_I(2).rgbcctcolors[1] = RgbcctColor(0,0,0,0,0); //black
  pCONT_lAni->SEGMENT_I(2).palette_id = mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
  pCONT_lAni->SEGMENT_I(2).effect_id = 71;//mAnimatorLight::EFFECTS_FUNCTION__CHASE_THEATER__ID;
  pCONT_lAni->SEGMENT_I(2).cycle_time__rate_ms = FRAMETIME_MS;
  pCONT_lAni->SEGMENT_I(2).speed = 255;
  pCONT_lAni->SEGMENT_I(2).cycle_time__rate_ms = 25;
  pCONT_lAni->SEGMENT_I(2).reverse = true;
  pCONT_lAni->SEGMENT_I(2).setBrightnessRGB(50);
  pCONT_lAni->SEGMENT_I(2).setBrightnessCCT(0);
  /**
   * @brief Segment1 [Rudder] - height is colour
   **/
  HsbColor color_rudder = color_hatch_chase;
  pCONT_lAni->SEGMENT_I(3).rgbcctcolors[0] = color_rudder;
  pCONT_lAni->SEGMENT_I(3).palette_id = mPalette::PALETTELIST_SEGMENT__RGBCCT_COLOUR_01__ID;
  pCONT_lAni->SEGMENT_I(3).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  pCONT_lAni->SEGMENT_I(3).cycle_time__rate_ms = FRAMETIME_MS;
  pCONT_lAni->SEGMENT_I(3).speed = 255;
  pCONT_lAni->SEGMENT_I(3).cycle_time__rate_ms = 25;
  pCONT_lAni->SEGMENT_I(3).setBrightnessRGB(50);
  pCONT_lAni->SEGMENT_I(3).setBrightnessCCT(0);
  /**
   * @brief Segment4 [Nose]
   **/
  pCONT_lAni->SEGMENT_I(4).rgbcctcolors[0] = RgbcctColor(255,255,255,255,255); //white
  pCONT_lAni->SEGMENT_I(4).palette_id = 0;
  pCONT_lAni->SEGMENT_I(4).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
  pCONT_lAni->SEGMENT_I(4).cycle_time__rate_ms = FRAMETIME_MS;
  pCONT_lAni->SEGMENT_I(4).speed = 255;
  pCONT_lAni->SEGMENT_I(4).setBrightnessRGB(255);
  pCONT_lAni->SEGMENT_I(4).setBrightnessCCT(255);


}


void mMavlinkFlyingLEDS::EveryLoop()
{
  uint16_t pwm_led_mode = 0;
  mMAVLink_Decoder::PacketData* packet_rc_channels = tkr_mavlink->findPacketData(MAVLINK_MSG_ID_RC_CHANNELS);
  if (packet_rc_channels != nullptr && packet_rc_channels->data != nullptr) {
      mavlink_rc_channels_t* rc_channels_data = static_cast<mavlink_rc_channels_t*>(packet_rc_channels->data);
      pwm_led_mode = rc_channels_data->chan15_raw;
  }

  enum SWITCH_POSITION__MODE{
    Bottom__Landing = 0,
    MidBottom = 1,
    Centre__AltitudeLighting = 2,
  };
  
  uint8_t mode = 0; // should be failsafe as "landing" lighting mode
  if     (pwm_led_mode <= 1000){ mode = 0; }                            // Bottom
  else if((pwm_led_mode > 1000) && (pwm_led_mode <= 1400)){ mode = 1; } // Mid-Bottom
  else if((pwm_led_mode > 1400) && (pwm_led_mode <= 1600)){ mode = Centre__AltitudeLighting; } // Centre
  else if((pwm_led_mode > 1600) && (pwm_led_mode <= 2000)){ mode = 3; } // Mid-Top
  else if(pwm_led_mode > 2000){ mode = 4; }                             // Top

  #ifdef ENABLE_DEBUG_FEATURE__MAVLINK_FLYING_LEDS__FORCED_TESTBED_MODE
  mode = ENABLE_DEBUG_FEATURE__MAVLINK_FLYING_LEDS__FORCED_TESTBED_MODE;
  #endif

  if(effect_manual == true)
  {
    switch(effect_mode)
    {
      default:
      case 0:
        effect_manual = false;
      break;
      case 1: // Landing Lights
        Effect_LandingLights();
      break;
      case 2: // Takeoff Lights
        Effect_TakeoffLights();
      break;
    }
  }


  switch(mode)
  {
    default:
    case 0: // Bottom == Landing
    {
      uint16_t hue = map(pwm_led_mode, 1000, 2000, 0, 360);
      HsbColor color = HsbColor(hue/360.0f, 1, 1);

      pCONT_lAni->SEGMENT_I(0).rgbcctcolors[0] = color;
      pCONT_lAni->SEGMENT_I(0).rgbcctcolors[0].W1 = 255;
      pCONT_lAni->SEGMENT_I(0).palette_id = 0;
      pCONT_lAni->SEGMENT_I(0).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
      pCONT_lAni->SEGMENT_I(0).cycle_time__rate_ms = FRAMETIME_MS;
      pCONT_lAni->SEGMENT_I(0).speed = 255;

    }break;
    case Centre__AltitudeLighting:




    break;
    case 3:

      //test, pitch is colour

      // float pitch = 0;

      // mMAVLink_Decoder::PacketData* packet_attitude = tkr_mavlink->findPacketData(MAVLINK_MSG_ID_ATTITUDE);
      // if (packet_attitude != nullptr && packet_attitude->data != nullptr) {
      //   mavlink_attitude_t* attitude_data = static_cast<mavlink_attitude_t*>(packet_attitude->data);
      //   pitch = attitude_data->pitch;
      // }


      // uint16_t hue = mSupport::mapfloat(pitch, -1.5, 1.5, 0, 360);
      // HsbColor color = HsbColor(hue/360.0f, 1, 1);

      // pCONT_lAni->SEGMENT_I(0).rgbcctcolors[0] = color;
      // pCONT_lAni->SEGMENT_I(0).palette_id = 0;
      // pCONT_lAni->SEGMENT_I(0).effect_id = mAnimatorLight::EFFECTS_FUNCTION__SOLID_COLOUR__ID;
      // pCONT_lAni->SEGMENT_I(0).cycle_time__rate_ms = FRAMETIME_MS;
      // pCONT_lAni->SEGMENT_I(0).speed = 255;





    break;

  }

  // ALOG_INF(PSTR("Throttle: %d, Hue: %d, Mode %d"), pwm_led_mode, hue, mode);

}

/******************************************************************************************************************
 * mTempSensorOLEDBath_Commands.cpp
*******************************************************************************************************************/


void mMavlinkFlyingLEDS::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[100];

  if(jtok = obj["MAVEffect"])
  {
    
    effect_manual = true;
    effect_mode = jtok.getInt();

    switch(effect_mode)
    {
      default:
      case 0:
        effect_manual = false;
      break;
      case 1: // Landing Lights
        Effect_LandingLights();
      break;
      case 2: // Takeoff Lights
        Effect_TakeoffLights();
      break;
    }

    ALOG_COM(PSTR("MAVEffect: %d"), effect_mode);
    
  }

  if(jtok = obj["MAVTestF"])
  {
    test_float = jtok.getFloat();
    // ALOG_COM(PSTR("MAVTestF: %f"), test_float);
  }

    
}


/******************************************************************************************************************
 * mTempSensorOLEDBath_ConstructJSON.cpp
*******************************************************************************************************************/



uint8_t mMavlinkFlyingLEDS::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
  return JBI->End();

}

uint8_t mMavlinkFlyingLEDS::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();  
  
  return JBI->End();

}


/******************************************************************************************************************
 * mTempSensorOLEDBath_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mMavlinkFlyingLEDS::MQTTHandler_Init()
{

  struct handler<mMavlinkFlyingLEDS>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mMavlinkFlyingLEDS::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mMavlinkFlyingLEDS::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mMavlinkFlyingLEDS::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mMavlinkFlyingLEDS::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mMavlinkFlyingLEDS::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, TaskerID::CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS__ID, handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS