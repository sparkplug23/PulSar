/*
  mProgramTimer.cpp - Home Heating SubClass - Wrapper for basic program timers

  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef mPROGRAMTEMP_H
#define mPROGRAMTEMP_H

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_HVAC

enum TEMPERATURE_MODE_IDS{
  TEMPERATURE_MODE_STANDBY_ID=0,
  TEMPERATURE_MODE_HEATING_ID,
  TEMPERATURE_MODE_COOLING_ID,
  TEMPERATURE_MODE_LENGTH_ID  
};

/**
 * Subclass that contains a heating method for 1 zone
 * Lets first get measured temperature into this
 * */
class ProgramTemperature{
  private:

    uint8_t device_name = 0;
    /**
     * @brief value used to count down the seconds to stay on
     * */
    int16_t time_on_seconds_decounter = 0;
    /**
     * @brief value used to store the initial desired time on
     * */
    int16_t time_on_seconds_set = 0;
		/**
		 * @brief Refers to state changes, ie started, ended, thus does relays need setting
		 * */
    bool    ischanged = false;
    /**
     * mode - heating, cooling, standby, maintaining
     * */
    uint8_t mode = 0;


    struct TEMP{
      float current=0;
      float error=0;
      float desired=0;
    }temperature;


  public:

    ProgramTemperature(){};

    void SetDesiredTemperature(float temp)
    {
      temperature.desired = temp;
    }

    void SetCurrentTemperature(float temp)
    {
      temperature.current = temp;
      temperature.error = temperature.current-temperature.desired;
    }

    float GetCurrentTemperature(void)
    {
      return temperature.current;
    }

    float GetDesiredTemperature(void)
    {
      return temperature.desired;
    }

    float GetErrorTemperature(void)
    {
      return temperature.current-temperature.desired;
    }

    void SetTimer_Minutes(uint16_t minutes)
    {
			time_on_seconds_decounter = minutes * 60;
    }

    uint16_t GetTimer_Minutes()
    {
			return time_on_seconds_decounter>=0 ? time_on_seconds_decounter/60 : 0;
    }

    uint16_t GetTimer_Seconds()
    {
			return time_on_seconds_decounter>=0 ? time_on_seconds_decounter : 0;
    }
    
    void SetMode(uint8_t value)
    {
      mode = value;
    }

    uint8_t GetMode(void)
    {
      return mode;
    }

    void StartProgram(uint16_t minutes_on)
    {
      // mode = _mode;
      SetTimer_Minutes(minutes_on);
    }

    bool IsRunning()
    {
      return (time_on_seconds_decounter >= 0) ? true : false;
    }

    bool IsChanged()
    {
      return ischanged;
    }

    bool IsChangedThenReset()
    {	
			bool state = ischanged;
			if(ischanged)
				ischanged = false;
			return state;
    }
    
    void EverySecond(void);


};



#endif

#endif