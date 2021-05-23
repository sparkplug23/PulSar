#include "mSDCard.h"

//  "Debug.cpp" are just extra functions used during debugging and development, but can be ignored in release versions

#ifdef USE_MODULE_DRIVERS_SDCARD

void mSDCard::SDCardSpeedDebug()
{

  // switch(write_test_count)
  debug_write_times.write_byte_count = 300;
  // { 
  //   default:
    // case 0: 
    SDCardBulkSpeedTest(debug_write_times.write_test_count,debug_write_times.write_byte_count); 
    // break;
  //   case 1: SDCardBulkSpeedTest(write_test_count,100); break;
  //   case 2: SDCardBulkSpeedTest(write_test_count,1000); break;
  //   case 3: SDCardBulkSpeedTest(write_test_count,10000); break;
  //   case 4: SDCardBulkSpeedTest(write_test_count,100000); break;
  // }

  mqtthandler_file_writer.flags.SendNow = true;

  // if(write_test_count++ > 1)
  // {
  //   write_test_count = 0;
  // }

}


void mSDCard::SDCardBulkSpeedTest(uint8_t test_number, uint32_t bytes_to_write)
{
  AddLog(LOG_LEVEL_TEST, PSTR("SDCardBulkSpeedTest %d %d Starting in 5 seconds"),test_number,bytes_to_write);
  // delay(5000);

  uint32_t time_start = millis();
  DEBUG_PIN1_SET(LOW);

  // Open file
  sprintf(writer_settings.file_name, "/%s%d.txt", "TestWriter",test_number);
  File file = SD.open(writer_settings.file_name, FILE_APPEND);
  if(!file){
    AddLog(LOG_LEVEL_TEST, PSTR("file \"%s\" did not open"),writer_settings.file_name);
  }
  AddLog(LOG_LEVEL_TEST, PSTR("file \"%s\" Opened!"),writer_settings.file_name);

  // write all bytes (numbers 0 to 9 single digits only)
  uint8_t bytes_for_card = 0;
  for(int i=0; i<bytes_to_write; i++)
  {
    DEBUG_PIN2_SET(LOW);
    file.write(bytes_for_card);
    DEBUG_PIN2_SET(HIGH);
    if(bytes_for_card++>9)
    {
      bytes_for_card = 0;
    }
  }

  //close file
  file.close();
  DEBUG_PIN1_SET(HIGH);

  uint32_t time_duration = millis() - time_start;
  debug_write_times.complete_write_duration = time_duration;
  AddLog(LOG_LEVEL_TEST, PSTR("file_name \"%s\" bytes: %d, time: %d ms"),writer_settings.file_name, bytes_to_write, time_duration);

}


#endif
