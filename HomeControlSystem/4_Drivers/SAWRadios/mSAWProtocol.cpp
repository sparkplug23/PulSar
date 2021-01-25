#include "4_Drivers/SAWRadios/mSAWProtocol.h"


#include "2_CoreSystem/mBaseConfig.h"
#ifdef USE_SAW_TRANSCEIVER

void mSAWProtocol::DecodePayload(uint64_t decimal, uint16_t bitlength, uint16_t protocol){//}), RTC_DS3231 rtc) {

   decode.bitlength = bitlength;
   decode.decimel = decimal;
   decode.protocol = protocol;

   if(bitlength == 24){ // Known basic codes

     memset(decode.namectr,0,sizeof(decode.namectr));

     Serial.println(decode.bitlength);
     Serial.println((uint32_t)decode.decimel);
     Serial.println(decode.protocol);

     // returns 0 if not known
     if(const char* tmp = GetNameFromCodeCtr((uint32_t)decimal)){ Serial.println("[SAW] !GetNameFromCodeCtr");
       decode.fWaiting = true;
       memcpy(decode.namectr,tmp,strlen(tmp));
     }else{
       memcpy(decode.namectr,"Unknown",sizeof("Unknown")-1);
     }

     //if none is found
     if(!decode.fWaiting){ decode.fWaiting = true; //send anyway
        //force it
     }

  } // END of 24 bitlength

} // END function


uint32_t mSAWProtocol::GetCodeFromName(const char* name){
  if(strstr(name,"SOCKET_ID01_ON")){
    return SAW_SOCKET_ID01_NUM_ON;
  }else
  if(strstr(name,"SOCKET_ID01_OFF")){
    return SAW_SOCKET_ID01_NUM_OFF;
  }else
  if(strstr(name,"DOORBELL_BACK1")){
    return SAWCODE_BACKDOORBELL_1;
  }else
  if(strstr(name,"DOORBELL_BACK2")){
    return SAWCODE_BACKDOORBELL_2;
  }
  return 0;// no code found
}


const char* mSAWProtocol::GetNameFromCodeCtr(uint32_t code){
  return
      (code == SAWCODE_ALARMSENSOR_1 ?  "ALARMSENSOR_1\0" :
      (code == SAWCODE_ALARMSENSOR_2 ?  "ALARMSENSOR_2\0" :
      (code == SAWCODE_ALARMSENSOR_3 ?  "ALARMSENSOR_3\0" :
      (code == SAWCODE_BACKDOORBELL_1 ? "DOORBELL_BACK1\0" :
      (code == SAWCODE_BACKDOORBELL_2 ? "DOORBELL_BACK2\0" :
      #ifdef USE_PLUG_DECODES
      (code == SAW_SOCKET_ID01_NUM_ON ? "SOCKET_ID01_ON\0" :

      "\0")))))); // 0 means no match
      #else
      "\0"))))); // 0 means no match
      #endif
}

#endif
