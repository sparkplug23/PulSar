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

#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2


  //  { 20,  16, { 14, 30 }, 1, { 510,  30 }, { 14,  30 }, { 30, 14 }, false,  230 }  // 36 (Louvolite) with premable

  // #define USE_MODULE_CONTROLLER__LOUVOLITE_HUB // my room only
  
int8_t mLouvoliteHub::Tasker(uint8_t function, JsonParserObject obj){

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function)
  {    
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_100_MSECOND:
      SubTask_SlowOpen_Mode();
    break;
    case TASK_EVERY_SECOND:
      // Perhaps adding method for telling the blind to open in steps, for "slower" opening. ie, opens bottom 10% in steps, then opens fully.
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); //make a TASK_MQTT_INIT and group mqtt togather
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate(); // Load teleperiod setting into local handlers
    break; 
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
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

  // if(start_slow_open == true)
  // {
  //   uint32_t elapsed_time = millis() - tSaved_millis_mode_slow_open_start;


  //   // if(elapsed_time < 1000)
  //   // {


  //   // }else
  //   if(elapsed_time >20000)
  //   {


  //   }

    
  //   if(mTime::TimeReached(&tSaved_millis_mode_slow_open_increment, 2000))
  //   {
  //     CommandSet_BlindTransmit_Up_Step();
  //     ALOG_INF(PSTR("Up Step %d, %d"),tSaved_millis_mode_slow_open_increment,stop_delay_ms);
  //   }

  //   /**
  //    * @brief Stop limit reached
  //    **/
  //   if(mTime::TimeReached(&tSaved_millis_mode_slow_open_last, 1000))
  //   {
  //     CommandSet_BlindTransmit_Stop();
  //     ALOG_INF(PSTR("Stopping %d,%d"), tSaved_millis_mode_slow_open_last, stop_delay_ms);
  //     // stop_delay_ms = 1000;
  //     // tSaved_millis_mode_slow_open_increment = millis();
  //   }

  //   if(elapsed_time >20000)
  //   {
  //     start_slow_open = false;
  //     ALOG_INF(PSTR("start_slow_open = false;"));
  //   }
    

  // }


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
    transceive->setRepeatTransmit(20);
    settings.fEnableSensor = true;
  }

  /**
   * @brief Init codes
   **/
  paired_codes[EM_DEVICE__BEDROOM_ID].up   = D_LOUVOLITE_COMMAND_UP__BEDROOM__HEX;
  paired_codes[EM_DEVICE__BEDROOM_ID].down = D_LOUVOLITE_COMMAND_DOWN__BEDROOM__HEX;
  paired_codes[EM_DEVICE__BEDROOM_ID].stop = D_LOUVOLITE_COMMAND_STOP__BEDROOM__HEX;
  paired_codes[EM_DEVICE__MASTERBEDROOM_ID].up   = D_LOUVOLITE_COMMAND_UP__MASTERBEDROOM__HEX;
  paired_codes[EM_DEVICE__MASTERBEDROOM_ID].down = D_LOUVOLITE_COMMAND_DOWN__MASTERBEDROOM__HEX;
  paired_codes[EM_DEVICE__MASTERBEDROOM_ID].stop = D_LOUVOLITE_COMMAND_STOP__MASTERBEDROOM__HEX;

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

  int8_t device_id = EM_DEVICE__BEDROOM_ID;

  if(jtok = obj["Louvolite"].getObject()["DeviceName"])
  {
    if(jtok.isStr()){
      if(strcasecmp(jtok.getStr(),"MasterBedroom")==0){ device_id = EM_DEVICE__MASTERBEDROOM_ID; }
      else if(strcasecmp(jtok.getStr(),"Bedroom")==0){ device_id = EM_DEVICE__BEDROOM_ID; }
      data_buffer.isserviced++;
    }

    ALOG_INF(PSTR("device_id=%d"), device_id);
    
  }




  if(jtok = obj["Louvolite"].getObject()["Position"])
  {
    if(jtok.isStr()){
      if((tmp_id=CommandMode_GetID_by_Name(jtok.getStr()))>=0){
        CommandSet_BlindTransmit_ByID(tmp_id, device_id);
        data_buffer.isserviced++;
      }
    }else
    if(jtok.isNum()){
      CommandSet_BlindTransmit_ByID(jtok.getInt(), device_id);
      data_buffer.isserviced++;
    }
    ALOG_COM( 
      PSTR(D_LOG_LIGHT D_COMMAND_SVALUE_K(D_COLOUR_PALETTE)), 
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
      CommandSet_BlindTransmit_Up_Step(device_id);
  }

	if(jtok = obj["Louv"].getObject()["Debug"])
	{

    // Optional set protocol (default is 1, will work for most outlets)
    transceive->setProtocol(36);
    // Optional set number of transmission repetitions.
    transceive->setRepeatTransmit(20);

    switch (jtok.getInt())
    {
      /**
       * @brief My Bedroom Commands
       * 
       */
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

      /**
       * @brief Masterbedroom
       * 
       */
    //case 10: transceive->send("1010110010100000111100101111011100111111000000000000101111010011"); break;  // My Up to compare
      case 10: transceive->send("1010110010100001011001011100001100111111000000000000101100010011"); break;  // Master Bedroom Up
      case 11: transceive->send("1010110010100001011001011100001100111111000000000010001100101011"); break;  // Master Bedroom Stop
      case 12: transceive->send("1010110010100001011001011100001100111111000000000100001101001011"); break;  // Master Bedroom Down


    }

		ALOG_INF( PSTR("CodeID=%d"), jtok.getInt() );
	}

    
}


void mLouvoliteHub::CommandSet_BlindTransmit_ByID(int8_t id, int8_t device_id)
{
  switch (id)
  {
    case COMMAND__UP_STEP__ID:    CommandSet_BlindTransmit_Up_Step(device_id); break;
    case COMMAND__DOWN_STEP__ID:  CommandSet_BlindTransmit_Down_Step(device_id); break;
    case COMMAND__UP_FULL__ID:    CommandSet_BlindTransmit_Up_Full(device_id); break;
    case COMMAND__DOWN_FULL__ID:  CommandSet_BlindTransmit_Down_Full(device_id); break;
    case COMMAND__STOP__ID:       CommandSet_BlindTransmit_Stop(device_id); break;
  }
  last_position_command = id;
}


void mLouvoliteHub::CommandSet_BlindTransmit_Up_Step(int8_t device_id)
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].up, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Down_Step(int8_t device_id)
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].down, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Up_Full(int8_t device_id)
{
  #ifdef ENABLE_DEVFEATURE_BLINDS_OPEN_COMMANDS_REPEATED_AND_DUPLICATED
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(20);
  
  // for(int i=0;i<3;i++)
  // {
  //   transceive->send(paired_codes[device_id].up, 64); //short down
  //   if(i<2){ delay(400); } // dont repeat on last loop
  // }
  
  // transceive->setProtocol(36);
  // transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].up, 64); //short down
  delay(300);
  // transceive->setProtocol(36);
  // transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].up, 64); //short down

  #else
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].up, 64); //short down
  delay(300);
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].up, 64); //short down
  #endif // ENABLE_DEVFEATURE_BLINDS_OPEN_COMMANDS_REPEATED_AND_DUPLICATED
}


void mLouvoliteHub::CommandSet_BlindTransmit_Down_Full(int8_t device_id)
{
  #ifdef ENABLE_DEVFEATURE_BLINDS_OPEN_COMMANDS_REPEATED_AND_DUPLICATED
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(20);

  // for(int i=0;i<3;i++)
  // {
  //   transceive->send(paired_codes[device_id].down, 64); //short down
  //   if(i<2){ delay(400); } // dont repeat on last loop // Need to change these to non blocking commands later!!
  // }

  
  // transceive->setProtocol(36);
  // transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].down, 64); //short down
  delay(300);
  // transceive->setProtocol(36);
  // transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].down, 64); //short down

  #else
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].down, 64); //short down
  delay(300);
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].down, 64); //short down


  #endif // ENABLE_DEVFEATURE_BLINDS_OPEN_COMMANDS_REPEATED_AND_DUPLICATED
}


void mLouvoliteHub::CommandSet_BlindTransmit_Stop(int8_t device_id)
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].stop, 64); //short down;
}



/******************************************************************************************************************
 * mLouvoliteHub_ConstructJSON.cpp
*******************************************************************************************************************/


uint8_t mLouvoliteHub::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_COUNT, settings.fEnableSensor);
    JBI->Add("RfMask", transceive->GetReceiveProtolMask());
  return JBI->End();

}

uint8_t mLouvoliteHub::ConstructJSON_State(uint8_t json_level, bool json_appending){

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

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mLouvoliteHub::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
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
void mLouvoliteHub::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mLouvoliteHub::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mLouvoliteHub::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH