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

#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE

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
     * @brief This will be the cutoff time
     * */
		/**
		 * @brief Refers to state changes, ie started, ended, thus does relays need setting
		 * */
    bool    ischanged = false;
		/**
		 * @brief Refers to state changes, ie started, ended, thus does relays need setting
		 * */
    bool    isrunning_heating = false;
    bool    isrunning_cooling = false;
    bool    mode_heating_enabled = true;
    bool    mode_cooling_enabled = false;
    /**
     * mode - heating, cooling, standby, maintaining
     * */
    uint8_t mode = 0;
    /**
     * Updated externally on sensor change
     * */
    struct TEMP{
      float current=0;
      float error=0;
      float desired=0;
    }temperature;    
    /**
     * @brief Internal/private variables use seconds, while external commands use minutes unless specified
     * */
    struct TIMERRUNNING{
      int16_t on=-1;
      int16_t limit=5*60;
    }time_running;
    struct TIMEMAINTAINING{
      int16_t on=-1;
      int16_t limit=5*60;
    }time_maintaining;
    struct TIMETOHEAT{
      int16_t minutes=-1;
    }time_to_heat;

    void CheckRunningProgram_Heating_Profile1();
    void CheckRunningProgram_Cooling();

  public:

    ProgramTemperature(){};

    /**
     * @brief This will both set the desired temperature, and trigger the program to aim for it.
     *        If cooling is enabled, desired temperatures below measured will trigger cooling
     * */
    void StartDesiredTemperature(float temp)
    {
      temperature.desired = temp;
      // Begin running by setting to 0 seconds (ie greater than -1)
      time_running.on = 1;
      // time_maintaining.on = 0;
    }

    /**
     * Temps
     * */
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

    /**
     * Timers
     * */
    void SetTimer_Running_Minutes(uint16_t minutes)
    {
			time_running.on = minutes * 60;
    }

    uint16_t GetTimer_Running_Minutes()
    {
			return time_running.on>=0 ? time_running.on/60 : 0;
    }

    uint16_t GetTimer_Running_Seconds()
    {
			return time_running.on>=0 ? time_running.on : 0;
    }

    void SetTimer_Running_Limit_Minutes(uint16_t minutes)
    {
			time_running.limit = minutes * 60;
    }

    uint16_t GetTimer_Running_Limit_Minutes()
    {
			return time_running.limit>=0 ? time_running.limit/60 : 0;
    }

    uint16_t GetTimer_Running_Limit_Seconds()
    {
			return time_running.limit>=0 ? time_running.limit : 0;
    }

    

    void SetTimer_Maintaining_Minutes(uint16_t minutes)
    {
			time_maintaining.on = minutes * 60;
    }

    uint16_t GetTimer_Maintaining_Minutes()
    {
			return time_maintaining.on>=0 ? time_maintaining.on/60 : 0;
    }

    uint16_t GetTimer_Maintaining_Seconds()
    {
			return time_maintaining.on>=0 ? time_maintaining.on : 0;
    }

    void SetTimer_Maintaining_Limit_Minutes(uint16_t minutes)
    {
			time_maintaining.limit = minutes * 60;
    }

    uint16_t GetTimer_Maintaining_Limit_Minutes()
    {
			return time_maintaining.limit>=0 ? time_maintaining.limit/60 : 0;
    }

    uint16_t GetTimer_Maintaining_Limit_Seconds()
    {
			return time_maintaining.limit>=0 ? time_maintaining.limit : 0;
    }

    /**
     * Modes
     * */
    
    void SetMode(uint8_t value)
    {
      mode = value;
    }

    uint8_t GetMode(void)
    {
      return mode;
    }

    // void StartProgram(uint16_t minutes_on)
    // {
    //   // mode = _mode;
    //   SetTimer_Minutes(minutes_on);
    // }
    
    /**
     * @brief This will tell any connected relays etc they should be on
     * @return 8bit packed value for [heat|fan|cool]
     * */
    uint8_t OutputDesiredState()
    {
      return isrunning_heating || isrunning_cooling;
    }

    bool IsRunning()
    {
      return isrunning_heating || isrunning_cooling;
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

    void CheckRunningProgram();


};



#endif

#endif