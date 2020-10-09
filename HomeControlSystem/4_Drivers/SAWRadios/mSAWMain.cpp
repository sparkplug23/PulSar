#include "4_Drivers/SAWRadios/mSAWMain.h"


#include "0_ConfigUser/mUserConfig.h"
#ifdef USE_SAW_TRANSCEIVER
//Updates to progress library to version 3
// mSAWTransceive = SawRadios must only contain basic "core" functions --
        //Split in transmit basic codes
        // and ability to receive
// mSAWCodes = Create another class, prob within this file (e.g mSawController) to hold "tasks" --
      // contains, and searches for known codes
// mSAWProtocol = Uses my own 8 bytes packet with CRC bytes available on the 8th bit

mSAWMain::mSAWMain(){}

void mSAWMain::parse_JSONCommand(void){


  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/rcswitch")>=0){
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_TOPIC_PIXELS));
      pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
  }else{
    return 0; // not meant for here
  }



  // if(mSupport::memsearch(data_buffer2.topic.ctr,data_buffer2.topic.len,"/transmit",sizeof("/transmit")-1)>=0){Serial.println("\tSSF::transmit");
  //   parsesub_Transmit();
  // }
  // else{
  //   //mso_mrgbani->MessagePrintln("\tSSF::INVALID");
  // }

}

void mSAWMain::parsesub_Transmit(void){

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(data_buffer2.payload.ctr);//json);

  uint32_t code = 0;

  if(root["raw"]){ Serial.println("root[\"raw\"]");//send raw code

  }else
  if(const char* name = root["name"]){ //send known code by its name
    Serial.println("root[\"name\"]");
    if(code=msp_msm->GetCodeFromName(root["name"].as<const char*>())){
      Serial.print("mst_msm->send(code,24);>> ");
        Serial.println(code);
       mst_msm->send(code,24);
    }
  }else{
    Serial.println("unknown root[\"??\"]");
  }

}



// Listen for manual doorbell presses from radios
void mSAWMain::Tasker(uint8_t function){

  switch(function){
    case FUNC_INIT:

    break;
    case FUNC_LOOP: 

    break;
    case FUNC_MQTT_SENDER:
      SubTasker_MQTTSender();
    break;
  }

  // Decode packets
  if(mst_msm->available()){Serial.println("mst_msm->available()");
    msp_msm->DecodePayload(mst_msm->getReceivedValue(),mst_msm->getReceivedBitlength(),mst_msm->getReceivedProtocol());
    mst_msm->disableReceive();
    tSavedDisabledTime = millis();
    mst_msm->resetAvailable();
  }
  //  else{
  // //   Serial.println("NONE");
  //    mst_msm->enableReceive(D6);
  //  }

  if(!mst_msm->getReceiveEnableState()){ // Reset the SAW Receive after timout// is SAW receive NOT enabled
    if((abs(millis() - tSavedDisabledTime) > 100)){ // Only reenable after 100ms to stop too many listens
      tSavedDisabledTime = millis();
      mst_msm->enableReceive(D6);
    }
  }

  //mqtt subtasker
  //{devicename_ctr,code,bitlength,protocol,decoded:unknown/backdoorbell}
  SubTasker_MQTTSender();

}


// NEW METHOD -- first senders then on internals
void mSAWMain::SubTasker_MQTTSender(){

  // if((abs(millis()-tSavedSendData)>=120000)||(fForceSendAllMQQTData)||(pCONT->mqt->fSendAllData)){tSavedSendData=millis();
  //   //MQQTRGBAnimationStatus(); // overview
  // }

  if(msp_msm->decode.fWaiting){ msp_msm->decode.fWaiting = false;
    MQTTSendDecoded();
  }

  if(fForceSendAllMQQTData){fForceSendAllMQQTData=false;}

}


void mSAWMain::MQTTSendDecoded(){
  MQQTDataBuilder_Decoded();
   if(data_buffer2.payload.len){
     pCONT->mqt->ppublish("status/sawradios/received",data_buffer2.payload.ctr,false);
   }
}

// Update struct that shows overview, always sends
void mSAWMain::MQQTDataBuilder_Decoded(void){

  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  // struct DECODEDATA{
  //   uint64_t decimel;
  //   uint16_t bitlength;
  //   uint16_t protocol;
  //   char namectr[10];
  //   char statectr[2];
  //   uint8_t fWaiting;
  // }decode;

  root["name"] = msp_msm->decode.namectr;
  root["time"] = pCONT->mt->mtime.hhmmss_ctr;

  JsonObject& decodedobj = root.createNestedObject("raw");
  decodedobj["code"] = (uint32_t)msp_msm->decode.decimel;
  decodedobj["bitlength"] = msp_msm->decode.bitlength;
  decodedobj["protocol"] = msp_msm->decode.protocol;

  //root["statectr"] = msp_msm->decode.statectr;

  // JsonObject& blendstyleobj = root.createNestedObject("blendstyle");
  //
  //   blendstyleobj["type"] = UpdateBlendStyleCtr();
  //   blendstyleobj["time"] = round(animation.blendstyle.time_ms/1000);
  //   blendstyleobj["rate"] = animation.blendstyle.rate_sec;
  //   blendstyleobj["amount"] = animation.amount;

  data_buffer2.payload.len = root.measureLength()+1;
  root.printTo((char*)data_buffer2.payload.ctr, data_buffer2.payload.len);

}

#endif
