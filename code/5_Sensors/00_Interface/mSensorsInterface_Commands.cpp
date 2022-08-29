#include "mSensorsInterface.h"

#ifdef USE_MODULE_SENSORS_INTERFACE

void mSensorsInterface::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["MQTT"].getObject()["ScanSensors"])
	{
		JBI->Start();

		pCONT->Tasker_Interface(FUNC_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID);

		bool ready_to_send = JBI->End();

		if(!ready_to_send)
		{
			// Nothing was found, create new message
			JBI->Start();
				JBI->Add("SensorScan", "No Devices Found");
			ready_to_send = JBI->End();
		}


		if(ready_to_send)
		{			
    	AddLog(LOG_LEVEL_TEST, PSTR("ScanSensors=\"%s\""), JBI->GetBufferPtr());
			pCONT_mqtt->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
		}

	}



    
}

#endif // USE_MODULE_SENSORS_INTERFACE
