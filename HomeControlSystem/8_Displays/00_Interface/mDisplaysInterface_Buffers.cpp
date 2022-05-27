#include "mDisplaysInterface.h"

#ifdef USE_MODULE_DISPLAYS_INTERFACE

/********************************************************************************************
 * Screen Buffer
 * *******************************************************************************************/

/**
 * @brief Fills the remaining row buffer with 0x20 (Space ASCII) char, by calculating X row length and its maximum size, filling the rest. The last byte is null terminated
 * */
void mDisplaysInterface::ScreenBuffer_SetUnusedRowCharsToSpaceChar(uint32_t line)
{
  uint32_t unused_char_spaces = screen_buffer.cols - strlen(screen_buffer.ptr[line]);
  if (unused_char_spaces) {
    // Fill the remaining chars in line with 0x20
    memset(screen_buffer.ptr[line] + strlen(screen_buffer.ptr[line]), 0x20, unused_char_spaces);
    // Null terminate
    screen_buffer.ptr[line][screen_buffer.cols -1] = 0;
  }
}


void mDisplaysInterface::ScreenBuffer_Clear(void)
{
  if (screen_buffer.cols) {
    for (uint32_t i = 0; i < screen_buffer.rows; i++) {
      memset(screen_buffer.ptr[i], 0, screen_buffer.cols);
    }
  }
}

void mDisplaysInterface::ScreenBuffer_Free(void)
{
  if (screen_buffer.ptr != nullptr) {
    for (uint32_t i = 0; i < screen_buffer.rows; i++) {
      if (screen_buffer.ptr[i] != nullptr) { free(screen_buffer.ptr[i]); }
    }
    free(screen_buffer.ptr);
    screen_buffer.cols = 0;
    screen_buffer.rows = 0;
  }
}

void mDisplaysInterface::ScreenBuffer_Alloc(void)
{
  if (!screen_buffer.cols) {
    screen_buffer.rows = pCONT_set->Settings.display.rows;
    screen_buffer.ptr = (char**)malloc(sizeof(*screen_buffer.ptr) * screen_buffer.rows);
    if (screen_buffer.ptr != nullptr) {
      for (uint32_t i = 0; i < screen_buffer.rows; i++) {
        screen_buffer.ptr[i] = (char*)malloc(sizeof(*screen_buffer.ptr[i]) * (pCONT_set->Settings.display.cols[0] +1));
        if (screen_buffer.ptr[i] == nullptr) {
          ScreenBuffer_Free();
          break;
        }
      }
    }
    if (screen_buffer.ptr != nullptr) {
      screen_buffer.cols = pCONT_set->Settings.display.cols[0] +1;
      ScreenBuffer_Clear();
    }
  }
}

void mDisplaysInterface::ScreenBuffer_ReAlloc(void)
{
  ScreenBuffer_Free();
  ScreenBuffer_Alloc();
}

/*-------------------------------------------------------------------------------------------*/

#ifdef ENABLE_FEATURE_DISPLAY_LOG_BUFFER

void mDisplaysInterface::LogBuffer_Init(void)
{
    
  log_buffer.index = 0;
  log_buffer.ptr_index = 0;
  disp_refresh = pCONT_set->Settings.display.refresh;

  LogBuffer_ReAlloc();
 
}

void mDisplaysInterface::LogBuffer_Clear(void)
{
  // If columns were succesful
  if (log_buffer.cols) {
    // Using the row pointer, clear the row (column width) by how many there is
    for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
      memset(log_buffer.ptr[i], 0, log_buffer.cols); // note, cols also contains the terminating character
    }
  }
}

void mDisplaysInterface::LogBuffer_Free(void)
{
  // Only free if the pointer has already been set once
  if (log_buffer.ptr != nullptr) {
    // Clear each row, using the pointer to that row
    for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
      if (log_buffer.ptr[i] != nullptr) { free(log_buffer.ptr[i]); }
    }
    free(log_buffer.ptr);
    log_buffer.cols = 0;
  }
}

void mDisplaysInterface::LogBuffer_Alloc(void)
{
  if (!log_buffer.cols) {

    // Create a list of pointers to each row
    log_buffer.ptr = (char**)malloc(sizeof(*log_buffer.ptr) * DISPLAY_LOG_ROWS);
    
    // Check if malloc was succesful
    if (log_buffer.ptr != nullptr) {
      
      // Allocate each row pointer, to have enough space for the row + termination
      for (uint32_t i = 0; i < DISPLAY_LOG_ROWS; i++) {
        log_buffer.ptr[i] = (char*)malloc(sizeof(*log_buffer.ptr[i]) * (pCONT_set->Settings.display.cols[0] +1));
        // If not succesful, reset buffer
        if (log_buffer.ptr[i] == nullptr) {
          // Clear buffer again
          LogBuffer_Free();
          break;
        }
      }
    }
    // If all of the above worked, then get the number of coloumn available
    if (log_buffer.ptr != nullptr) {
      log_buffer.cols = pCONT_set->Settings.display.cols[0] +1;
      // Init the data as empty
      LogBuffer_Clear();
    }
  }
}

void mDisplaysInterface::LogBuffer_ReAlloc(void)
{
  LogBuffer_Free();
  LogBuffer_Alloc();
}

void mDisplaysInterface::LogBuffer_Add(char* txt)
{
  // Cols is just to check if things are initialised
  if (log_buffer.cols) {
    // Copy new text into current line
    strlcpy(log_buffer.ptr[log_buffer.index], txt, log_buffer.cols);  // This preserves the % sign where printf won't
    
    // Add that a new row exists, this will allow "LogBuffer_GetRowPointer" to know an update is required
    log_buffer.index++;
    if (DISPLAY_LOG_ROWS == log_buffer.index) { log_buffer.index = 0; }
  }
}

void mDisplaysInterface::LogBuffer_AddRow(char* txt, uint8_t row_index)
{
  // Cols is just to check if things are initialised
  if (log_buffer.cols) {
    // Copy new text into current line
    if (row_index < DISPLAY_LOG_ROWS)
    {
      strlcpy(log_buffer.ptr[row_index], txt, log_buffer.cols);  // This preserves the % sign where printf won't
    }
  }
}

char* mDisplaysInterface::LogBuffer_GetRowPointer(char temp_code)
{
  char* result = nullptr;
  if (log_buffer.cols) {
    // AddLog(LOG_LEVEL_DEBUG, PSTR("log_buffer.cols=%d"),log_buffer.cols);
    // If currently shown line, is not the latest line, we must update the screen 
    if (log_buffer.index != log_buffer.ptr_index) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("log_buffer.index=%d %d"),log_buffer.index,log_buffer.ptr_index);
      result = log_buffer.ptr[log_buffer.ptr_index];
    AddLog(LOG_LEVEL_DEBUG, PSTR("result=%s"),result);
      log_buffer.ptr_index++;

      // if reached maximum rows, wrap around
      if (DISPLAY_LOG_ROWS == log_buffer.ptr_index) { log_buffer.ptr_index = 0; }

      // Swaps '~' for degree symbol for temperature ( i dont need this on test method)
      char *pch = strchr(result, '~');  // = 0x7E (~) Replace degrees character (276 octal)
      if (pch != nullptr) { result[pch - result] = temp_code; }
    }
  }
  return result;
}

char* mDisplaysInterface::LogBuffer_GetRowPointerByRowIndex(uint8_t row_index)
{
  if(log_buffer.cols)
  {
    if(row_index < DISPLAY_LOG_ROWS)
    {
      return log_buffer.ptr[row_index];
    }
  }
  return nullptr; // invalid row
}

#endif // ENABLE_FEATURE_DISPLAY_LOG_BUFFER

#endif // USE_MODULE_DISPLAYS_INTERFACE
