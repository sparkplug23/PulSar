#include "mLouvoliteHub.h"

#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB


void mLouvoliteHub::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[100];

  if(jtok = obj["Louvolite"].getObject()["Position"])
  {
    if(jtok.isStr()){
      if((tmp_id=CommandMode_GetID_by_Name(jtok.getStr()))>=0){
        CommandSet_BlindTransmit_ByID(tmp_id);
        data_buffer.isserviced++;
      }
    }else
    if(jtok.isNum()){
      CommandSet_BlindTransmit_ByID(jtok.getInt());
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
      CommandSet_BlindTransmit_Up_Step();
  }

	if(jtok = obj["Louv"].getObject()["Debug"])
	{

    // Optional set protocol (default is 1, will work for most outlets)
    transceive->setProtocol(36);
    // Optional set number of transmission repetitions.
    transceive->setRepeatTransmit(20);

    switch (jtok.getInt())
    {
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
    }

		ALOG_INF( PSTR("CodeID=%d"), jtok.getInt() );
	}

    
}


void mLouvoliteHub::CommandSet_BlindTransmit_ByID(int8_t id)
{
  switch (id)
  {
    case COMMAND__UP_STEP__ID:    CommandSet_BlindTransmit_Up_Step(); break;
    case COMMAND__DOWN_STEP__ID:  CommandSet_BlindTransmit_Down_Step(); break;
    case COMMAND__UP_FULL__ID:    CommandSet_BlindTransmit_Up_Full(); break;
    case COMMAND__DOWN_FULL__ID:  CommandSet_BlindTransmit_Down_Full(); break;
    case COMMAND__STOP__ID:       CommandSet_BlindTransmit_Stop(); break;
  }
  last_position_command = id;
}


void mLouvoliteHub::CommandSet_BlindTransmit_Up_Step()
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_UP_HEX, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Down_Step()
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_DOWN_HEX, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Up_Full()
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_UP_HEX, 64); //short down
  delay(300);
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_UP_HEX, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Down_Full()
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_DOWN_HEX, 64); //short down
  delay(300);
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_BUTTON_DOWN_HEX, 64); //short down
}


void mLouvoliteHub::CommandSet_BlindTransmit_Stop()
{
  transceive->setProtocol(36);
  transceive->setRepeatTransmit(10);
  transceive->send(D_LOUVOLITE_COMMAND__STOP__HEX, 64); //short down;
}



#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH
