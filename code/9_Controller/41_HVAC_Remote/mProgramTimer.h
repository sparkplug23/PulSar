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
#ifndef mPROGRAMTIMER_H
#define mPROGRAMTIMER_H

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE

/**
 * Subclass that contains a heating method for 1 zone
 * Lets first get measured temperature into this
 * */
class ProgramTimer
{
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

  public:

    ProgramTimer(){};

    void Init()
    {

    }

    void StartTimer_Minutes(uint16_t minutes)
    {
			time_on_seconds_set = minutes*60;
			time_on_seconds_decounter = time_on_seconds_set;
			ischanged = true;
    }

    uint16_t GetTimer_Minutes()
    {
			return time_on_seconds_decounter>=0 ? time_on_seconds_decounter/60 : 0;
    }

    uint16_t GetTimer_Seconds()
    {
			return time_on_seconds_decounter>=0 ? time_on_seconds_decounter : 0;
    }

		bool IsRunning()
		{
			return (time_on_seconds_decounter >= 0) ? true : false;
		}
    
    /**
     * @brief This will tell any connected relays etc they should be on
     * @return 8bit packed value for [heat|fan|cool]
     * */
    uint8_t OutputDesiredState()
    {
      return IsRunning();
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