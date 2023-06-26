/*
  File to be deleted, but hidding things to be redone here
*/
#include "mHVAC.h"

#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE



void mHVAC::FunctionHandler_FailSafe(void){
return;
  uint8_t fMessageToSend = false;
  char tmpctr[100] = {0}; 
  char buffer[100];
  //memset(tmpctr,0,sizeof(tmpctr));

  uint16_t time_minutes_on = 0;


  for(int device_id=0;device_id<4;device_id++){

    time_minutes_on = pCONT_mry->relay_status[device_id].time_seconds_on/60;

    if(
      (time_minutes_on>FAILSAFE_MINUTES_WARNING)&&
      (time_minutes_on<FAILSAFE_MINUTES_ERROR)
    ){
      // snprintf(tmpctr, sizeof(tmpctr), "Warning, %s heating has been on for a long time of %d minutes",GetDeviceNameLongbyIDCtr(device_id),heating_device_relays[device_id].time_minutes_on);
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_FAILSAFE D_WARNING "\"%s\""),tmpctr);
      fMessageToSend = true;
    }else 
    if(time_minutes_on>FAILSAFE_MINUTES_ERROR){
      // snprintf(tmpctr, sizeof(tmpctr), "Error, %s heating has been on for too long. Turning off now.",GetDeviceNameLongbyIDCtr(device_id));
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_FAILSAFE D_ERROR "\"%s\""),tmpctr);
      fMessageToSend = true;
    }

  //   // if(fMessageToSend){
  //   //   StaticJsonDocument<300> doc;
  //   //   JsonObject obj = doc.to<JsonObject>();
  //   //   obj[D_JSON_TYPE] = "voice+notification"; // broadcast (public/non bedroom speakers), notification(on phones) //SPECIAL, can contain both
  //   //   obj[D_JSON_MESSAGE] = tmpctr;
  //   //   data_buffer.payload.len = measureJson(obj)+1;
  //   //   serializeJson(doc,data_buffer.payload.ctr);
  //   //   pCONT->mqt->ppublish("status/alert",data_buffer.payload.ctr,false);
  //   // }
  }

} //end fucntion


#endif
