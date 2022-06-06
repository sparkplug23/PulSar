#include "mLEDs.h"

#ifdef USE_MODULE_DRIVERS_LEDS

void mLEDs::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["LED"].getObject()["SetState"])
	{

		if(jtok.isNum())
		{
			// mySwitch->setReceiveProtocolMask(jtok.getUInt());
			mqtthandler_settings_teleperiod.flags.SendNow = true;
		}

		// JBI->Start();

		// pCONT->Tasker_Interface(FUNC_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID);

		// bool ready_to_send = JBI->End();

		// if(!ready_to_send)
		// {
		// 	// Nothing was found, create new message
		// 	JBI->Start();
		// 		JBI->Add("SensorScan", "No Devices Found");
		// 	ready_to_send = JBI->End();
		// }


		// if(ready_to_send)
		// {			
    	// AddLog(LOG_LEVEL_TEST, PSTR("RfMask = %d / %d"), jtok.getUInt(), mySwitch->GetReceiveProtolMask());
		// 	pCONT_mqtt->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
		// }

	}



int8_t led_id = -1;
uint16_t state_value = 0;

 if(jtok = obj["LEDName"]){
    // if(jtok.isStr()){
    //   relay_id = GetRelayIDbyName(jtok.getStr());
    // }else 
    // if(jtok.isNum()){
    //   relay_id  = jtok.getInt();
    // }
	led_id = jtok.getInt();
  }

  // Primary method since v0.86.14.21
  if(jtok = obj["LEDState"]){
    // if(jtok.isStr()){
    //   state = pCONT_sup->GetStateNumber(jtok.getStr());
    // }else 
    // if(jtok.isNum()){
      state_value  = jtok.getInt();//pCONT_sup->GetStateNumber(jtok.getInt());
    // }

    /**
     * @brief If off, clear any timer decounters for relays
     * 
     */
    // if(state == 0)
    // {
    //   CommandSet_Timer_Decounter(0, relay_id);
    // }

		//state needs checked for flipped
		// if(state == 2){

		// }
		
// SetState()

  }
    
}

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH
