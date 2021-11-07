
#include "2_CoreSystem/DeviceNameList/mDeviceNameList.h"


/* Null, because instance will be initialized on demand. */
DeviceNameList* DeviceNameList::instance = nullptr;

DeviceNameList* DeviceNameList::GetInstance(){
  if (instance == nullptr){
    instance = new DeviceNameList();
    // PRINT_FLUSHED("instance = new JsonBuilder()");
  }
  return instance;
}



//returns length
// Other variations needed:
//  - Overwrite existing devicename
//  - Remove by name OR by ID
int8_t DeviceNameList::AddDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id){

  char* buffer = name_buffer.ptr;
  uint16_t buffer_length = strlen(buffer);

  #ifdef ENABLE_LOG_LEVEL_INFO
  //AddLog(LOG_LEVEL_INFO,PSTR("mSettings::AddDeviceName len=%d"),buffer_length);
  #endif// ENABLE_LOG_LEVEL_INFO

  buffer_length = buffer_length;// > DEVICENAMEBUFFER_NAME_BUFFER_LENGTH ? DEVICENAMEBUFFER_NAME_BUFFER_LENGTH : buffer_length;
  
  #ifdef USE_LOG
  //AddLog(LOG_LEVEL_INFO,PSTR("name_bufferB=%s"), buffer);
  #endif


  // Check indexing
  uint8_t index = 0;
  if(buffer_length){ //if anything in buffer
    index = CountCharInCtr(buffer,'|'); // first | indicates index from 0
  }
  
  // Check not above "id" limits
  if(index >= (number_buffer.length-1)){// (DEVICENAMEBUFFER_NAME_INDEX_LENGTH-1)){
    return 0; // too many devices in list
  }

  uint16_t new_buffer_length = buffer_length+strlen(name_ctr);

    #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("AddDeviceName(%s,%d,%d) %d index=%d"), 
  //   name_ctr, 
  //   class_id, 
  //   device_id, 
  //   buffer_length, 
  //   index
  // );
    #endif// ENABLE_LOG_LEVEL_INFO
  //AddLog(LOG_LEVEL_INFO,PSTR("name_bufferB=%s"), buffer);

  // Write name to next slot
  if(new_buffer_length<name_buffer.length){
    buffer_length+=sprintf(buffer+buffer_length, "%s|", name_ctr); 
    #ifdef ENABLE_LOG_LEVEL_INFO
    // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("AddDeviceName ADDED + \"%s\""),name_ctr); 
    #endif // ENABLE_LOG_LEVEL_INFO
  }

  // Add index to next slot
  // number_buffer.unique_group_ids[index] = class_id;
  // number_buffer.index_ids[index] = device_id;

  number_buffer.unique_group_ids[index] = class_id;
  number_buffer.index_ids[index] = device_id;

  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("name_bufferE=%s"), buffer);

    #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog_Array(LOG_LEVEL_DEBUG_MORE,PSTR("class_id"),number_buffer.unique_group_ids,(int16_t)DEVICENAMEBUFFER_NAME_INDEX_LENGTH);
  // AddLog_Array(LOG_LEVEL_DEBUG_MORE,PSTR("device_id"),number_buffer.index_ids,(int8_t)DEVICENAMEBUFFER_NAME_INDEX_LENGTH);
    #endif// ENABLE_LOG_LEVEL_INFO

}

int8_t DeviceNameList::RemoveDeviceName(const char* name_ctr, int16_t class_id, int8_t device_id){
  // Find name, shift results to remove found name
  // Not needed until I want to remove values
}


// const char* DeviceNameList::GetDeviceNameWithUniqueNumber(int16_t module_id, int8_t device_id, char* buffer, uint16_t buffer_size){

// }

const char* DeviceNameList::GetDeviceNameWithEnumNumber(int16_t module_id, int8_t device_id, char* buffer, uint16_t buffer_size){

  //convert enum number to unqiue
  module_id = pCONT->GetModuleUniqueIDbyVectorIndex(module_id);

  int8_t found_index = -1;
  // Check if class & id match
  for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++){
    
    // if((number_buffer.unique_group_ids[i]==module_id)&&(number_buffer.index_ids[i]==device_id)){
    
    if((
      
      
      number_buffer.unique_group_ids[i]==module_id)&&(number_buffer.index_ids[i]==device_id)){
      found_index = i;
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("DeviceNameList::GetDeviceNameWithEnumNumber found_index %d"),i);
    #endif // ENABLE_LOG_LEVEL_INFO
      break;
    }
  }
  //future, if none found, have a list of the prefered defaults, relay%d, sensor%d etc

  if(found_index == -1){
    memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("F::%s >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
    #endif // ENABLE_LOG_LEVEL_INFO
    return buffer;
  }

  char* name_buffer2 = name_buffer.ptr;

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber len=%d"),strlen(buffer));
    #endif // ENABLE_LOG_LEVEL_INFO
  // gets first index from the array, where we start at the position the desired name is the next name
  pCONT_sup->GetTextIndexed(buffer, buffer_size, found_index, name_buffer2);
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber=%s"),buffer);
    #endif // ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("GetDeviceNameWithEnumNumber &name_buffer[index]=%s"),&name_buffer[index]);

  if(buffer == nullptr){
    memcpy(buffer,PM_SEARCH_NOMATCH,sizeof(PM_SEARCH_NOMATCH));
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_ERROR, PSTR("F::%s ERROR >> %s"),__FUNCTION__,PM_SEARCH_NOMATCH);
    #endif // ENABLE_LOG_LEVEL_INFO
  }

  return buffer;
  
}

// search for name, return its index position using pointers
// search for name within devicebuffer, return its class/device id's
// int8_t mSettings::GetDeviceIDbyName(int8_t* class_id, int8_t* device_id, char* name_tofind){

//   // pCONT_sup->GetText


// uint8_t GetDevice_


//   return *device_id;
// }
// int16_t mSettings::GetDeviceIDbyName(const char* name_tofind, int8_t device_id, int8_t class_id){
// //   return GetDeviceIDbyName(name_tofind, Settings.device_name_buffer.name_buffer, &device_id, &class_id);
// // }



// if class_id is nullptr (default value), then ignore matching class first and return first instance of device name
// if class_id is passed, then limit matching results to include that class
int16_t DeviceNameList::GetDeviceIDbyName(const char* name_tofind, int8_t device_id, int8_t class_id){
//   return GetDeviceIDbyName(name_tofind, name_buffer.ptr, &device_id, &class_id);
// }


// // if class_id is nullptr (default value), then ignore matching class first and return first instance of device name
// // if class_id is passed, then limit matching results to include that class
// int16_t mSettings::GetDeviceIDbyName(const char* name_tofind, const char* haystack, int8_t* device_id, int8_t* class_id)
// {
  const char* haystack = name_buffer.ptr;
  const char* read = haystack;
  int16_t position = -1;

  char name_tofind_with_delimeter[50];
  snprintf(name_tofind_with_delimeter,sizeof(name_tofind_with_delimeter),"%s|",name_tofind);


    // AddLog(LOG_LEVEL_INFO,PSTR("Name_tofind_with_delimeter = %s"),name_tofind_with_delimeter);

  // Search for substring
  char *p_start_of_found = strstr(haystack,name_tofind_with_delimeter);

  if(p_start_of_found == NULL){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("p_start_of_found == NOT FOUND"));
    #endif// ENABLE_LOG_LEVEL_INFO
    return -1;
  }

  bool limit_result_to_class_ids = true;
  // if((class_id == nullptr)||
  if(class_id == -1){
    limit_result_to_class_ids = false;
  }
    #ifdef ENABLE_LOG_LEVEL_INFO
  
  AddLog(LOG_LEVEL_DEBUG_MORE,PSTR("limit_result_to_class_ids = %s"),limit_result_to_class_ids?"YES":"NO");

    #endif// ENABLE_LOG_LEVEL_INFO

  uint8_t delimeter_count = 0;
  uint8_t delimeter_within_class_count = 0;
  uint16_t haystack_index = 0;
  uint16_t haystack_length = strlen(haystack);
  //search for delimeters between found index and start ie indexed list places
  while((read != p_start_of_found) && (haystack_index++ < haystack_length)){
    // Count delimeters
    if(*read == '|'){
      if(limit_result_to_class_ids){
        if(number_buffer.unique_group_ids[delimeter_count] == class_id){
          delimeter_within_class_count++;
          // #ifdef ENABLE_LOG_LEVEL_INFO
          // // AddLog(LOG_LEVEL_TEST,PSTR("\n\r%s\n\r found wclass_count\n\r %s\n\r %d %d\n\r\n\r"),haystack,read,delimeter_within_class_count,number_buffer.unique_group_ids[delimeter_count]);
          // AddLog(LOG_LEVEL_TEST,
          //   PSTR(
          //     "Searching \"%s\"\n\r"
          //     "Found \"%s\" @ index %d\n\r"
          //     "del count/index pos %d\n\r"
          //   ),         
          //   name_tofind,
          //   p_start_of_found,
          //   p_start_of_found - read, // pointer distance
          //   delimeter_within_class_count
          // );
          // #endif // ENABLE_LOG_LEVEL_INFO
        }
      }else{
        delimeter_within_class_count++;        
      }
      delimeter_count++;
    #ifdef ENABLE_LOG_LEVEL_INFO
      // AddLog(LOG_LEVEL_INFO,PSTR("%s found %s %d"),haystack,read,delimeter_count);
    #endif // ENABLE_LOG_LEVEL_INFO
    }
    read++; //move pointer along
  }

  #ifdef ENABLE_LOG_LEVEL_INFO
          // AddLog(LOG_LEVEL_TEST,PSTR("\n\r%s\n\r found wclass_count\n\r %s\n\r %d %d\n\r\n\r"),haystack,read,delimeter_within_class_count,number_buffer.unique_group_ids[delimeter_count]);
          AddLog(LOG_LEVEL_DEBUG_MORE,
            PSTR(
              "\n\rSearching \"%s\"\n\r"
              "Found \"%s\" @ index %d\n\r"
              "delimeter_count %d\n\r"
              "delimeter_within_class_count %d\n\r"
            ),         
            name_tofind,
            p_start_of_found,
            p_start_of_found - haystack, // pointer distance
            delimeter_count,
            delimeter_within_class_count
          );
          #endif // ENABLE_LOG_LEVEL_INFO

  return delimeter_within_class_count;
}




int16_t DeviceNameList::GetIndexOfNthCharPosition(const char* tosearch, char tofind, uint8_t occurance_count){
  uint16_t count = 0;
  for(int16_t i=0;i<strlen(tosearch);i++){
    if(tosearch[i]==tofind) count++;
    if(count == occurance_count) return i;
  }
  return -1;
}

// If no class_id provided, return total driver count, not just within that class
int8_t DeviceNameList::GetDeviceNameCount(int16_t class_id){
  int8_t count = 0;
  for(int i=0;i<DEVICENAMEBUFFER_NAME_INDEX_LENGTH;i++){
    if(number_buffer.unique_group_ids[i]==class_id){
      count++;
      break;
    }
  }
  return count;
}

uint16_t DeviceNameList::CountCharInCtr(const char* tosearch, char tofind)
{
  uint16_t count = 0;
  for(uint16_t i=0;i<strlen(tosearch);i++){
    if(tosearch[i]==tofind){ count++; }
  }
  return count;
}




// #endif