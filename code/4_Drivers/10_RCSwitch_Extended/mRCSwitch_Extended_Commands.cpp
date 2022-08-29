#include "mRCSwitch_Extended.h"

#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED

void mRCSwitch::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["RfMask"])
	{

		if(jtok.isNum())
		{
			mySwitch->setReceiveProtocolMask(jtok.getUInt());
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
    	AddLog(LOG_LEVEL_TEST, PSTR("RfMask = %d / %d"), jtok.getUInt(), mySwitch->GetReceiveProtolMask());
		// 	pCONT_mqtt->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
		// }

	}



    
}

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH
