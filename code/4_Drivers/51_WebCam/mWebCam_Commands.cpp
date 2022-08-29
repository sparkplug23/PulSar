
#include "mWebCam.h"

#ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM


void mWebCam::parse_JSONCommand(JsonParserObject obj)
{


  AddLog(LOG_LEVEL_INFO, PSTR("mWebCam::parse_JSONCommand"));

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  JsonParserObject jobj = 0;
 
  uint8_t relay_id= 0,state=-1;    //assume index 0 if none given

  // if(jtok = obj[PM_JSON_POWERNAME]){
  //   if(jtok.isStr()){
  //     relay_id = GetRelayIDbyName(jtok.getStr());
  //   }else 
  //   if(jtok.isNum()){
  //     relay_id  = jtok.getInt();
  //   }
  // }

  // // Primary method since v0.86.14.21
  // if(jtok = obj[PM_JSON_POWER_STATE]){
  //   if(jtok.isStr()){
  //     state = pCONT_sup->GetStateNumber(jtok.getStr());
  //   }else 
  //   if(jtok.isNum()){
  //     state  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
  //   }

  //   /**
  //    * @brief If off, clear any timer decounters for relays
  //    * 
  //    */
  //   if(state == 0)
  //   {
  //     CommandSet_Timer_Decounter(0, relay_id);
  //   }

	// 	//state needs checked for flipped
	// 	// if(state == 2){

	// 	// }

  // }

  // // PHASE OUT by version 0.87
  // if(jtok = obj[PM_JSON_ONOFF]){
  //   if(jtok.isStr()){
  //     state = pCONT_sup->GetStateNumber(jtok.getStr());
  //   }else 
  //   if(jtok.isNum()){
  //     state  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
  //   }
  // }


  


  // if(jtok = obj[PM_JSON_RELAY].getObject()[PM_JSON_TIME_ON]){
  //   CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
  // }else
  // if(jtok = obj[PM_JSON_RELAY].getObject()[PM_JSON_TIME_ON_SECS]){
  //   CommandSet_Timer_Decounter(jtok.getInt(), relay_id);
  // }else
  // if(jtok = obj[PM_JSON_RELAY].getObject()[PM_JSON_TIME_ON_MINUTES]){
  //   CommandSet_Timer_Decounter(jtok.getInt()*60, relay_id);
  // }

  // if(jtok = obj[PM_JSON_RELAY].getObject()[PM_JSON_TIME_OFF_THEN_ON_SECS]){
  //   CommandSet_RelayAsRessetingDevice_TurnOffThenOnAgain(jtok.getInt(), relay_id);
  // }









  // if(IsWithinRange(state, 0,10) && IsWithinRange(relay_id, 0,settings.relays_connected)){
  //   CommandSet_Relay_Power(state,relay_id);
  // }


	// //move this so its passed as object, hence "EnabledTime" can be search for by (no number and hence use powername) or "EnabledTime#" #=1,2,3 ... how does tas pull the numbers out? must match key partially

	// // Search for match which uses relayindex
	// if(jtok = obj["RelayEnabled"]){
	// 	SubCommandSet_EnabledTime(jtok.getObject());
	// }

  // char rule_name[20] = {0};
  // for(uint8_t ii=0;ii<3;ii++){
  //   sprintf(rule_name, "RelayEnabled%d", ii);
      
	// 	if(jtok = obj[rule_name]){
	// 		AddLog(LOG_LEVEL_INFO, PSTR("MATCHED %s"),rule_name);
	// 		SubCommandSet_EnabledTime(jtok.getObject(), ii);
	// 	}

  // }  

  


  // if(jtok = obj["EnabledTime"]){
  //   time_short_t ontime;
  //   time_short_t offtime;
  //   uint8_t index = 0;
    
  //   if(jtok = obj["EnabledTime"].getObject()[PM_JSON_INDEX]){
  //     index = jtok.getInt();    
  //   }
    
  //   if(jtok = obj["EnabledTime"].getObject()[PM_JSON_ONTIME]){
  //     ontime = mTime::Parse_Time_TimeShortCtr_To_TimeShort(jtok.getStr());
  //     relay_status[relay_id].enabled_ranges[index].ontime = ontime;
  //   }
  //   if(jtok = obj["EnabledTime"].getObject()[PM_JSON_OFFTIME]){
  //     offtime = mTime::Parse_Time_TimeShortCtr_To_TimeShort(jtok.getStr());
  //     relay_status[relay_id].enabled_ranges[index].offtime = offtime;
  //   }
  //   if(jtok = obj["EnabledTime"].getObject()[PM_JSON_ENABLED]){
  //     relay_status[relay_id].enabled_ranges[index].enabled = jtok.getInt();
  //   }


  //   pCONT_time->PrintDateTime(ontime);
  //   pCONT_time->PrintDateTime(offtime);


  // }


  // mqtthandler_scheduled_teleperiod.flags.SendNow = true;

}


// void mWebCam::SubCommandSet_EnabledTime(JsonParserObject jobj, uint8_t relay_id){

//   JsonParserToken jtok;
//   int16_t matched_id = 0;

		
//   // if(jtok = obj["EnabledTime"]){
//     time_short_t ontime;
//     time_short_t offtime;
//     uint8_t index = 0;
    
//     if(jtok = jobj[PM_JSON_INDEX]){
//       index = jtok.getInt();    
//     }
    
//     if(jtok = jobj[PM_JSON_ONTIME]){
//       ontime = mTime::Parse_Time_TimeShortCtr_To_TimeShort(jtok.getStr());
//       relay_status[relay_id].enabled_ranges[index].ontime = ontime;
//     }
//     if(jtok = jobj[PM_JSON_OFFTIME]){
//       offtime = mTime::Parse_Time_TimeShortCtr_To_TimeShort(jtok.getStr());
//       relay_status[relay_id].enabled_ranges[index].offtime = offtime;
//     }
//     if(jtok = jobj[PM_JSON_ENABLED]){
//       relay_status[relay_id].enabled_ranges[index].enabled = jtok.getInt();
//     }


//     pCONT_time->PrintDateTime(ontime);
//     pCONT_time->PrintDateTime(offtime);


//   // }


// }




/*********************************************************************************************
* Input & Output Interface ******************************************************************
*************************************************************************************************/


//   //flip args around so the single arg means state of relay 0
//   // Only apply changes when state is changed
// void mWebCam::CommandSet_Relay_Power(uint8_t state, uint8_t num){

//   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RELAYS D_FUNCTION_NAME_SVALUE " " D_JSON_COMMAND_NVALUE " " D_JSON_COMMAND_NVALUE)
//     ,"CommandSet_Relay_Power","num",num,"state",state);  

// 	// Check state if it needs to toggle result
//   ModifyStateNumberIfToggled(&state, CommandGet_Relay_Power(num));
  
//   //error patch  within vaible range
//   if(!((num>=0)&&(num<=settings.relays_connected))){
//     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_RELAYS D_FUNCTION_NAME_SVALUE " Command Invalid"),"CommandSet_Relay_Power");
//     return;
//   }

//   #ifdef ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

//     // AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_RELAYS "settings.flags.enabled_relays_allowed_time_window_checks=%d"), settings.flags.enabled_relays_allowed_time_window_checks);
//     // AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_RELAYS "IsRelayTimeWindowAllowed(num)=%d"), IsRelayTimeWindowAllowed(num));
    
//     /**
//      * @note Only block relay from turning on, as off period may occur outside window
//      * */
//     if(state==1)
//     {
//       if(settings.flags.enabled_relays_allowed_time_window_checks && pCONT_time->RtcTime.valid)
//       {  
//         if(!IsRelayTimeWindowAllowed(num))
//         {
//           state = 0; // forcing off
//           AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RELAYS "Relay %d is locked: Outside enabled time window"), num);
//           //   return;
//         }
//         else
//         {
//           AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_RELAYS "Relay %d is permitted: Inside enabled time window"), num);
//         }
//       }
//     }
//   #endif // ENABLE_DEVFEATURE_RELAY_DISABLING_SCHEDULE_CHECKS

//   if(CommandGet_Relay_Power(num)==state){
//     relay_status[num].ischanged = false;
//     return;
//   }else{
//     relay_status[num].ischanged = true;
//   }

//   // relay_status[num].onoff = state;
//   bitWrite(pCONT_set->power, num, state);

//   if(state){ 
//     relay_status[num].last.ontime = pCONT_time->RtcTime; //create future "operators" to handle these conversions
//     relay_status[num].time_seconds_on = 1;
//   }else{ 
//     relay_status[num].last.offtime = pCONT_time->RtcTime; 
//     relay_status[num].time_seconds_on = 0; // Off 
//   }

//   ExecuteCommandPower(num,state,SRC_MQTT);

// }

// uint8_t mWebCam::CommandGet_Relay_Power(uint8_t num){
//   return bitRead(pCONT_set->power, num);
// }



// /**********************************************************************************************
//  *********************************************************************************************
//   Parameter: time_seconds_on
//  *********************************************************************************************
//  ********************************************************************************************/

// // void mWebCam::CommandSet_Timer_Decounter(uint16_t time_secs, uint8_t relay_id){
// //   relay_status[relay_id].timer_decounter.seconds = time_secs;
// //   relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
// //   #ifdef ENABLE_LOG_LEVEL_COMMANDS
// //     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
// //   #endif
// // }

// uint32_t mWebCam::CommandGet_SecondsRelayHasBeenOn(uint8_t relay_id)
// {
//   // relay_status[relay_id].timer_decounter.seconds = time_secs;
//   // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
//   // #endif

//   return relay_status[relay_id].time_seconds_on;
// }




/**********************************************************************************************
 *********************************************************************************************
  Parameter: TimerDecounter
 *********************************************************************************************
 ********************************************************************************************/

// void mWebCam::CommandSet_Timer_Decounter(uint16_t time_secs, uint8_t relay_id){
//   relay_status[relay_id].timer_decounter.seconds = time_secs;
//   relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
//   #endif
// }

// uint16_t mWebCam::CommandGet_SecondsToRemainOn(uint8_t relay_id)
// {
//   // relay_status[relay_id].timer_decounter.seconds = time_secs;
//   // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
//   // #endif

//   return relay_status[relay_id].timer_decounter.seconds;
// }


/**********************************************************************************************
 *********************************************************************************************
  Parameter: New function, to briefly command a relay turns off, then to turn it on again (ie when used to reset devices)
 *********************************************************************************************
 ********************************************************************************************/

// void mWebCam::CommandSet_RelayAsRessetingDevice_TurnOffThenOnAgain(uint16_t time_secs, uint8_t relay_id)
// {
//   // relay_status[relay_id].timer_decounter.seconds = time_secs;
//   // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
//   // #endif

//   /**
//    * Variable that is "off" peroid
//    * Variable that will then turn it on again
//    * */

//   relay_status[relay_id].timer_off_then_on_decounter.seconds = time_secs;
//   relay_status[relay_id].timer_off_then_on_decounter.active = time_secs > 0 ? true : false;
//   #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_off_then_on_decounter.seconds);  
//   #endif



//   // return relay_status[relay_id].time_seconds_on;
// }


// uint16_t mWebCam::CommandGet_SecondsToRemainOff(uint8_t relay_id)
// {
//   // relay_status[relay_id].timer_decounter.seconds = time_secs;
//   // relay_status[relay_id].timer_decounter.active = time_secs > 0 ? true : false;
//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   //   AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_RELAYS "Set" D_JSON_TIME "Relay%d " "%d" D_UNIT_SECOND), relay_id, relay_status[relay_id].timer_decounter.seconds);  
//   // #endif

//   return relay_status[relay_id].timer_off_then_on_decounter.seconds;
// }







#endif // USE_MODULE_DRIVERS_CAMERA_WEBCAM











/***********
 * 
 * 
 * 
 * 
 
 {
  "PowerName": 0,
  "Relay":{"TimeOffThenOnSecs": 5, "TimeOnSecs":10}
}
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * ***/