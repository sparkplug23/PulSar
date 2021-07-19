#include "mSDCard.h"

// Low level functions for writing/reading from the SD Card

#ifdef USE_MODULE_DRIVERS_SDCARD

  File stored_file;


void mSDCard::listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

	File root = fs.open(dirname);
	if(!root){
		Serial.println("Failed to open directory");
		return;
	}
	if(!root.isDirectory()){
		Serial.println("Not a directory");
		return;
	}

	File file = root.openNextFile();
	while(file){
		if(file.isDirectory()){
			Serial.print("  DIR : ");
			Serial.println(file.name());
			if(levels){
				listDir(fs, file.name(), levels -1);
			}
		} else {
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("  SIZE: ");
			Serial.println(file.size());
		}
		file = root.openNextFile();
	}

}


void mSDCard::createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void mSDCard::removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void mSDCard::readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void mSDCard::writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void mSDCard::appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
}

void mSDCard::appendFile_open_and_close(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void mSDCard::write_append_array(fs::FS &fs, const char * path, uint8_t* buffer, uint16_t buflen)
{
    // Serial.printf("Appending to file: %s\n", path);

    // I dont want this, because it keeps opening and closing!!!
    File file = fs.open(path, FILE_APPEND);
    if(!file){
        // Serial.println("Failed to open file for appending");
        return;
    }

    for(uint16_t i=0; i<buflen; i++)
    {
        file.write(buffer[i]);
    }

    // if(file.print(message)){
    //     Serial.println("Message appended");
    // } else {
    //     Serial.println("Append failed");
    // }
}

void mSDCard::SubTask_Append_To_Open_File(char* buffer, uint16_t buflen)
{
   // DEBUG_PIN1_SET(0);

  switch(writer_settings.status)
  {
    case FILE_STATUS_OPENING_ID:

// add option to use GPS time instead of NTP time, where it updates every second inside gps

			sprintf(writer_settings.file_name, "/%s_%s_%02d%02d%02d_%03d.txt", // Unique name each time it is opened
				"APPEND", 
				DEVICENAME_FOR_SDCARD_FRIENDLY_CTR, 
				pCONT_time->RtcTime.hour, pCONT_time->RtcTime.minute, pCONT_time->RtcTime.second, 
				random(1,1000)
			);

      stored_file = SD.open(writer_settings.file_name, FILE_APPEND);
      if(!stored_file){
        AddLog(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "stored_file \"%s\" did not open"),writer_settings.file_name);
      }else{
	      AddLog(LOG_LEVEL_TEST, PSTR("stored_file \"%s\" Opened!"),writer_settings.file_name);
      	writer_settings.status = FILE_STATUS_OPENED_ID;
				sdcard_status.bytes_written_to_file = 0;
			}


    // break;
    case FILE_STATUS_OPENED_ID:

      for(int i=0; i<buflen; i++)
      {
        stored_file.write(buffer[i]);
      }
	    sdcard_status.bytes_written_to_file += buflen;

    break;
    case FILE_STATUS_CLOSING_ID:
        
      AddLog(LOG_LEVEL_TEST, PSTR("stored_file \"%s\" Closed!"),writer_settings.file_name);

      stored_file.close();

      writer_settings.status = FILE_STATUS_CLOSED_ID;

    break;

    //no break
    // write into

    //close file
  }

  //  DEBUG_PIN1_SET(1);

}

/***
 * New ringbuffer for stream out to sdcard, every 10ms if there is new sdcard data to write, it will be taken from ringbuffer
 * 
 * */
void mSDCard::Stream_AddToBuffer(char* buffer, uint16_t buflen)
{






}







void mSDCard::renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void mSDCard::deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

void mSDCard::testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}



#endif
