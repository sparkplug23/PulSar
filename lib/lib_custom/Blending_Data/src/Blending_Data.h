#ifndef DATA_SMOOTHING_H
#define DATA_SMOOTHING_H

/*-------------------------------------------------------------------------
mDataBlend provides blend timing support.
-------------------------------------------------------------------------*/

#pragma once

#include <Arduino.h>
#include "mNeoEase.h"

#define BLEND_DATA_MILLISECONDS        1    // ~65 seconds max duration, ms updates
#define BLEND_DATA_CENTISECONDS       10    // ~10.9 minutes max duration, centisecond updates
#define BLEND_DATA_DECISECONDS       100    // ~1.8 hours max duration, decisecond updates
#define BLEND_DATA_SECONDS          1000    // ~18.2 hours max duration, second updates
#define BLEND_DATA_DECASECONDS     10000    // ~7.5 days, 10 second updates

template <typename TYPE>
class LinearBlendVariable
{
public:
    LinearBlendVariable(uint16_t countBlends, uint16_t timeScale = BLEND_DATA_MILLISECONDS) :
        _blendLastTick(0),
        _activeBlends(false),
        _isRunning(true)
    {
        setTimeScale(timeScale);
        _blends = new BlendContext;
        memset(&_value,0,sizeof(VALUE));
    }


    ~LinearBlendVariable()
    {
        delete _blends;
    }


    bool IsBlending() const
    {
        return _activeBlends;
    }

    void StartBlend(uint16_t duration)
    {

        if (!_activeBlends)
        {
            _blendLastTick = millis();
        }

        StopBlend();

        // all blends must have at least non zero duration, otherwise they are considered stopped
        if (duration == 0)
        {
            duration = 1;
        }

        _blends->StartBlend(duration);

        _activeBlends = true;
    }
    
    void StopBlend()
    {

        if (IsBlendActive())
        {
            _blends->StopBlend();
        }   

    }

    void RestartBlend()
    {
        if (_blends._duration == 0)
        {
            return;
        }

        StartBlend(_blends._duration);
    }

    bool IsBlendActive() const
    {
        return (IsBlending() && _blends->_remaining != 0);
    }

    uint16_t BlendDuration()
    {
        return _blends->_duration;
    }

    void ChangeBlendDuration(uint16_t newDuration)
    {
        
        // calc the current blend progress 
        float progress = _blends->CurrentProgress();

        // keep progress in range just in case
        if (progress < 0.0f)
        {
            progress = 0.0f;
        }
        else if (progress > 1.0f)
        {
            progress = 1.0f;
        }

        // change the duration 
        _blends->_duration = newDuration;
            
        // _remaining time must also be reset after a duration change; use the progress to recalculate it
        _blends->_remaining = uint16_t(_blends->_duration * (1.0f - progress));

    }



    void UpdateVariableUsingProgress()
    {
        /**
         * @brief If target is already met, then stop blend
         **/
        // if(_value.current == _value.target)
        // {
        // //     // _blends->StopBlend();
        // //             // param.state = BlendState_Completed2;
        // //             // param.progress = 1.0f;

        //             Serial.printf("Stop Blending EARLY\n\r");
        // //             _activeBlends--; 
        // //             _blends->StopBlend();
        // //             StopBlend(0);

        //             _activeBlends--; 
        //     _blends[0].StopBlend();
        // //     return;
        // }

        _value.current = mapfloat(_blends->CurrentProgress(),  0.0f, 1.0f,  _value.start, _value.target);

        Serial.println(_value.current);  
    }

    void UpdateBlend()
    {
        if (_isRunning)
        {
            uint32_t currentTick = millis();
            uint32_t delta = currentTick - _blendLastTick;

            if (delta >= _timeScale)
            {

                delta /= _timeScale; // scale delta into blend time

                if (_blends->_remaining > delta)
                {

                    UpdateVariableUsingProgress();

                    _blends->_remaining -= delta;
                }
                else if (_blends->_remaining > 0)
                {

                    _activeBlends = false; 
                    _blends->StopBlend();

                    UpdateVariableUsingProgress();
                }

                _blendLastTick = currentTick;
            }
        }

    }

    bool IsPaused()
    {
        return (!_isRunning);
    }

    void Pause()
    {
        _isRunning = false;
    }

    void Resume()
    {
        _isRunning = true;
        _blendLastTick = millis();
    }

    uint16_t getTimeScale()
    {
        return _timeScale;
    }

    void setTimeScale(uint16_t timeScale)
    {
        _timeScale = (timeScale < 1) ? (1) : (timeScale > 32768) ? 32768 : timeScale;
    }


    /**
     * @brief Set Target value
     **/
    void SetTarget(TYPE v)
    {
        _value.target = v;
        _value.start = _value.current; // Use current value as start value
    }

    void SetTargetAndStartBlend(TYPE v, uint16_t duration)
    {
        _value.target = v;
        _value.start = _value.current; // Use current value as start value
        StartBlend(duration);
    }

    /**
     * @brief Get value
     **/
    TYPE GetTarget()
    {
        return _value.target;
    }

    /**
     * @brief Get value
     **/
    TYPE GetValue()
    {
        return _value.current;
    }

    /**
     * @brief Set the Current Value object
     * Used to update externally
     * 
     * @return TYPE 
     */
    void SetCurrentValue(TYPE v)
    {
        // StopBlend
        _value.current = v;
    }


    float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }


private:
    struct BlendContext
    {
        BlendContext() :
            _duration(0),
            _remaining(0)
        {}

        void StartBlend(uint16_t duration)
        {
            _duration = duration;
            _remaining = duration;
        }

        void StopBlend()
        {
            _remaining = 0;
        }

        /**
         * @brief Progress from 0 to 1
         **/
        float CurrentProgress()
        {
            return (float)(_duration - _remaining) / (float)_duration;
        }

        uint16_t _duration;
        uint16_t _remaining;
    };

    BlendContext* _blends;
    uint32_t _blendLastTick;
    bool _activeBlends;  // as a bool and not count, this is probably the same as "isRunning"
    uint16_t _timeScale;
    bool _isRunning;
  
    struct VALUE{  // remove struct
        TYPE start;
        TYPE current;
        TYPE target;
    }_value;


};


#endif

