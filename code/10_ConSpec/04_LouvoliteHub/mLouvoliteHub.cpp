/**
  mLouvoliteHub.ino - RF transceiver using RcSwitch library

  Copyright (C) 2022    Michael Doone

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

#include "mLouvoliteHub.h"

#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB

const char* mLouvoliteHub::PM_MODULE_CONTROLLER_LOUVOLITE_HUB_CTR = D_MODULE_CONTROLLER_LOUVOLITE_HUB_CTR;
const char* mLouvoliteHub::PM_MODULE_CONTROLLER_LOUVOLITE_HUB_FRIENDLY_CTR = D_MODULE_CONTROLLER_LOUVOLITE_HUB_FRIENDLY_CTR;


int8_t mLouvoliteHub::Tasker(uint8_t function, JsonParserObject obj){

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

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function)
  {    
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_100_MSECOND:
      SubTask_SlowOpen_Mode();
    break;
    case FUNC_EVERY_SECOND:
      // Perhaps adding method for telling the blind to open in steps, for "slower" opening. ie, opens bottom 10% in steps, then opens fully.
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
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    break; 
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif  
  }

}//end


/**
 * @brief When commanded, the millis from start will be recorded, and each stepped movement will be relative to that start
 * 
 */
void mLouvoliteHub::SubTask_SlowOpen_Mode()
{

  if(start_slow_open == true)
  {
    uint32_t elapsed_time = millis() - tSaved_millis_mode_slow_open_start;


    // if(elapsed_time < 1000)
    // {


    // }else
    if(elapsed_time >20000)
    {


    }

    
    if(mTime::TimeReached(&tSaved_millis_mode_slow_open_increment, 2000))
    {
      CommandSet_BlindTransmit_Up_Step();
      ALOG_INF(PSTR("Up Step %d, %d"),tSaved_millis_mode_slow_open_increment,stop_delay_ms);
    }

    /**
     * @brief Stop limit reached
     **/
    if(mTime::TimeReached(&tSaved_millis_mode_slow_open_last, 1000))
    {
      CommandSet_BlindTransmit_Stop();
      ALOG_INF(PSTR("Stopping %d,%d"), tSaved_millis_mode_slow_open_last, stop_delay_ms);
      // stop_delay_ms = 1000;
      // tSaved_millis_mode_slow_open_increment = millis();
    }

    if(elapsed_time >20000)
    {
      start_slow_open = false;
      ALOG_INF(PSTR("start_slow_open = false;"));
    }
    

  }


}

void mLouvoliteHub::Pre_Init(void)
{

}


void mLouvoliteHub::Init(void)
{
  
  if (pCONT_pins->PinUsed(GPIO_RF_433MHZ_TX_ID))
  {
    transceive = new RCSwitch();
    transceive->enableTransmit(pCONT_pins->GetPin(GPIO_RF_433MHZ_TX_ID));
    transceive->setRepeatTransmit(10);
    settings.fEnableSensor = true;
  }

}


int16_t mLouvoliteHub::CommandMode_GetID_by_Name(const char* c)
{
  if(*c=='\0'){    return -1; }
  if(strcasecmp_P(c,PM_LOUVOLITE_COMMAND__STOP__CTR)==0){ return COMMAND__STOP__ID; }
  if(strcasecmp_P(c,PM_LOUVOLITE_COMMAND__UP_STEP__CTR)==0){ return COMMAND__UP_STEP__ID; }
  if(strcasecmp_P(c,PM_LOUVOLITE_COMMAND__DOWN_STEP__CTR)==0){ return COMMAND__DOWN_STEP__ID; }
  if(strcasecmp_P(c,PM_LOUVOLITE_COMMAND__UP_FULL__CTR)==0){ return COMMAND__UP_FULL__ID; }
  if(strcasecmp_P(c,PM_LOUVOLITE_COMMAND__DOWN_FULL__CTR)==0){ return COMMAND__DOWN_FULL__ID; }
  return -1;
}


const char* mLouvoliteHub::CommandMode_GetName_by_ID(uint8_t id, char* buffer, uint8_t buflen)
{
  switch(id){
    default:
    case COMMAND__STOP__ID:      memcpy_P(buffer, PM_LOUVOLITE_COMMAND__STOP__CTR, sizeof(PM_LOUVOLITE_COMMAND__STOP__CTR)); break;
    case COMMAND__UP_STEP__ID:   memcpy_P(buffer, PM_LOUVOLITE_COMMAND__UP_STEP__CTR, sizeof(PM_LOUVOLITE_COMMAND__UP_STEP__CTR)); break; 
    case COMMAND__DOWN_STEP__ID: memcpy_P(buffer, PM_LOUVOLITE_COMMAND__DOWN_STEP__CTR, sizeof(PM_LOUVOLITE_COMMAND__DOWN_STEP__CTR)); break; 
    case COMMAND__UP_FULL__ID:   memcpy_P(buffer, PM_LOUVOLITE_COMMAND__UP_FULL__CTR, sizeof(PM_LOUVOLITE_COMMAND__UP_FULL__CTR)); break; 
    case COMMAND__DOWN_FULL__ID: memcpy_P(buffer, PM_LOUVOLITE_COMMAND__DOWN_FULL__CTR, sizeof(PM_LOUVOLITE_COMMAND__DOWN_FULL__CTR)); break; 
  }
  return buffer;
}

/******************************************************************************************************************
 * mLouvoliteHub_Commands.cpp
*******************************************************************************************************************/

void mLouvoliteHub::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[100];

  int8_t device_id = -1;

  if(jtok = obj["Louvolite"].getObject()["DeviceName"])
  {
    if(jtok.isStr()){
      if(strcasecmp(jtok.isStr(),"MasterBedroom")==0){ device_id = 0; }
      else if(strcasecmp(jtok.isStr(),"Bedroom")==0){ device_id = 1; }
      data_buffer.isserviced++;
    }
  }




  if(jtok = obj["Louvolite"].getObject()["Position"])
  {
    if(jtok.isStr()){
      if((tmp_id=CommandMode_GetID_by_Name(jtok.getStr()))>=0){
        CommandSet_BlindTransmit_ByID(tmp_id);
        data_buffer.isserviced++;
      }
    }else
    if(jtok.isNum()){
      CommandSet_BlindTransmit_ByID(jtok.getInt());
      data_buffer.isserviced++;
    }
    ALOG_COM( 
      PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), 
      CommandMode_GetName_by_ID(jtok.getInt(), buffer, sizeof(buffer))
    );
  }

	if(jtok = obj["Louv"].getObject()["Test"])
	{
    start_slow_open = true;
    tSaved_millis_mode_slow_open_start = millis();
    tSaved_millis_mode_slow_open_last = millis();
    tSaved_millis_mode_slow_open_increment = millis();
    stop_delay_ms = 500;
      CommandSet_BlindTransmit_Up_Step();
  }

	if(jtok = obj["Louv"].getObject()["Debug"])
	{

    // Optional set protocol (default is 1, will work for most outlets)
    transceive->setProtocol(36);
    // Optional set number of transmission repetitions.
    transceive->setRepeatTransmit(20);

    switch (jtok.getInt())
    {
      case 1: transceive->send("1010110010100000111100101111011100111111000000000000101111010011"); break;  // Up Short
      case 2: transceive->send("1010110010100000111100101111011100111111000000000010010011101100"); break;  // Up Long
      case 3: transceive->send("1010110010100000111100101111011100111111000000000010001111101011"); break;  // Stop short
      case 4: transceive->send("1010110010100000111100101111011100111111000000000100001100001011"); break;  // Down Short
      case 5: transceive->send("1010110010100000111100101111011100111111000000001100001110001011"); break;  // Down Long
      case 6:
      // transceive->send("1010110010100000111100101111011100111111000000000000101111010011"); // Up Short
      // delay(200);
      // transceive->send("1010110010100000111100101111011100111111000000001100001110001011"); // Up Long
      // break;
      case 7:
      //   transceive->send("1010110010100000111100101111011100111111000000001100001110001011");
      // break;
      case 8:
      //   transceive->send("1010110010100000111100101111011100111111000000000100001100001011");
      // break;
      case 9: 
        transceive->setProtocol(36);
        transceive->setRepeatTransmit(10);
       transceive->send("1010110010100000111100101111011100111111000000000100001100001011"); //short down
       delay(300);
        transceive->setProtocol(36);
        transceive->setRepeatTransmit(10);
       transceive->send("1010110010100000111100101111011100111111000000000100001100001011"); //short down
      //  delay(300);
      //   transceive->setProtocol(36);
      //   transceive->setRepeatTransmit(15);
      //  transceive->send("1010110010100000111100101111011100111111000000001100001110001011"); //long down
      break;
    }

		ALOG_INF( PSTR("CodeID=%d"), jtok.getInt() );
	}

    
}


void mLouvoliteHub::CommandSet_BlindTransmit_ByID(int8_t id)
{
  switch (id)
  {
    case COMMAND__UP_STEP__ID:    CommandSet_BlindTransmit_Up_Step(); break;
    case COMMAND__DOWN_STEP__ID:  CommandSet_BlindTransmit_Down_Step(); break;
    case COMMAND__UP_FULL__ID:    CommandSet_BlindTransmit_Up_Full(); break;
    case COMMAND__DOWN_FULL__ID:  CommandSet_BlindTransmit_Down_Full(); break;
    case COMMAND__STOP__ID:       CommandSet_BlindTransmit_Stop(); break;
  }
  last_position_command = id;
}


void mLouvoliteHub::CommandSet_BlindTransmit_Up_Step()
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_UP_HEX, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Down_Step()
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_DOWN_HEX, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Up_Full()
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_UP_HEX, 64); //short down
  delay(300);
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_UP_HEX, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Down_Full()
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_DOWN_HEX, 64); //short down
  delay(300);
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_DOWN_HEX, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Stop()
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_COMMAND__STOP__HEX, 64); //short down;
}


/******************************************************************************************************************
 * mLouvoliteHub_ConstructJSON.cpp
*******************************************************************************************************************/


uint8_t mLouvoliteHub::ConstructJSON_Settings(uint8_t json_level, bool json_object_start_end_required){

  JBI->Start();
    JBI->Add(D_JSON_COUNT, settings.fEnableSensor);
    JBI->Add("RfMask", transceive->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mLouvoliteHub::ConstructJSON_State(uint8_t json_level, bool json_object_start_end_required){

  char buffer[40];

  JBI->Start();  
    JBI->Add("LastCommand", last_position_command);
    JBI->Add("LastCommandName", CommandMode_GetName_by_ID(last_position_command, buffer, sizeof(buffer)));
  return JBI->End();

}



/******************************************************************************************************************
 * mLouvoliteHub_MQTT.cpp
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mLouvoliteHub::MQTTHandler_Init()
{

  struct handler<mLouvoliteHub>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mLouvoliteHub::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mLouvoliteHub::ConstructJSON_State;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mLouvoliteHub::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mLouvoliteHub::MQTTHandler_Set_TelePeriod()
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
void mLouvoliteHub::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH