/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _SingleEMAFilterLib_h
#define _SingleEMAFilterLib_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

template <typename T>
class SingleEMAFilter
{
public:
  SingleEMAFilter<T>(const double alpha);

  T AddValue(const T value);
  T GetLowPass();
  T GetHighPass();

  void SetAlpha(const double alpha);
  void Reset();
  bool IsReady();

private:
  double _alpha = 0.0;

  T _lowPassFilter = static_cast<T>(0);
  T _highPassFilter = static_cast<T>(0);

  bool _hasValue = false;
};
#endif


template<typename T>
SingleEMAFilter<T>::SingleEMAFilter(const double alpha)
{
  SetAlpha(alpha);
  Reset();
}


template<typename T>
void SingleEMAFilter<T>::SetAlpha(const double alpha)
{
  if (alpha < 0.0)
  {
    _alpha = 0.0;
  }
  else if (alpha > 1.0)
  {
    _alpha = 1.0;
  }
  else
  {
    _alpha = alpha;
  }
}


template<typename T>
void SingleEMAFilter<T>::Reset()
{
  _lowPassFilter = static_cast<T>(0);
  _highPassFilter = static_cast<T>(0);
  _hasValue = false;
}


template<typename T>
bool SingleEMAFilter<T>::IsReady()
{
  return _hasValue;
}


template<typename T>
T SingleEMAFilter<T>::AddValue(const T value)
{
  if (!_hasValue)
  {
    _lowPassFilter = value;
    _highPassFilter = static_cast<T>(0);
    _hasValue = true;
    return _lowPassFilter;
  }

  _lowPassFilter = static_cast<T>((_alpha * value) + ((1.0 - _alpha) * _lowPassFilter));
  _highPassFilter = static_cast<T>(value - _lowPassFilter);

  return _lowPassFilter;
}


template<typename T>
T SingleEMAFilter<T>::GetLowPass()
{
  return _lowPassFilter;
}


template<typename T>
T SingleEMAFilter<T>::GetHighPass()
{
  return _highPassFilter;
}