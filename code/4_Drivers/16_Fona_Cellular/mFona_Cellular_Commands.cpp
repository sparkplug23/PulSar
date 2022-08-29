#include "mFona_Cellular.h"

#ifdef USE_MODULE_DRIVERS_FONA_CELLULAR

void mFona_Cellular::parse_JSONCommand(JsonParserObject obj)
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

 if(jtok = obj["Fona"].getObject()["CommandList"])
 {
	// char buffer[10];
	const char* buf = jtok.getStr();
	Command(buf[0]);
  }


 if(jtok = obj["Fona"].getObject()["SendTextMessage"])
 {
	// char buffer[10];
	char* message = (char*)jtok.getStr();
	// Command(buf[0]);


	
	if (!fona->sendSMS(D_FLEXI_O2_PHONE_NUMBER_CTR, message)) {
		Serial.println(F("Failed"));
	} else {
		Serial.println(F("Sent!"));
	}


  }

 if(jtok = obj["Fona"].getObject()["CallPhone"])
 {
// 	// char buffer[10];
// 	char* message = (char*)jtok.getStr();
// 	// Command(buf[0]);


	
// 	if (!fona->sendSMS(D_FLEXI_O2_PHONE_NUMBER_CTR, message)) {
// 		Serial.println(F("Failed"));
// 	} else {
// 		Serial.println(F("Sent!"));
// 	}

	// call a phone!
        // char number[30];
        // flushSerial();
        Serial.print(F("Call #"));
        // readline(number, 30);
        // Serial.println();
        Serial.print(F("Calling ")); Serial.println(D_FLEXI_O2_PHONE_NUMBER_CTR);
        if (!fona->callPhone(D_FLEXI_O2_PHONE_NUMBER_CTR)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("Sent!"));
        }


  }


 if(jtok = obj["Fona"].getObject()["PowerCycle"])
 {
	digitalWrite(GPIO_FUNCTION__FONA_POWER_KEY__ID, LOW);
	delay(1000);
	digitalWrite(GPIO_FUNCTION__FONA_POWER_KEY__ID, HIGH);

	Serial.println(F("Power Cycle"));

	// ALOG_INF
	


  }


    
}

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH
