#include "mWebCamera.h"

#ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM_V4

void mWebCamera::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["RfMask"])
	{

		if(jtok.isNum())
		{
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


	}



    
}

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH
