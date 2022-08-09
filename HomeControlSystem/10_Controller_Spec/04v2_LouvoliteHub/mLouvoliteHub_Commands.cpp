#include "mLouvoliteHub.h"

#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2


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
      PSTR(D_LOG_LIGHT D_JSON_COMMAND_SVALUE_K(D_JSON_COLOUR_PALETTE)), 
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
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].up, 64); //short down
  delay(300);
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].up, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Down_Full(int8_t device_id)
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].down, 64); //short down
  delay(300);
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].down, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Stop(int8_t device_id)
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(paired_codes[device_id].stop, 64); //short down;
}



#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH
