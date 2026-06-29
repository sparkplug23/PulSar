/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _DoubleEMAFilterLib_h
#define _DoubleEMAFilterLib_h

#if defined(ARDUINO) && ARDUINO >= 100
  #include "arduino.h"
#else
  #include "WProgram.h"
#endif

#include <math.h>

template <typename T>
class DoubleEMAFilter
{
public:
  DoubleEMAFilter<T>(const double alpha1, const double alpha2);

  T AddValue(const T value);
  T GetBandPass();
  T GetBandStop();

  void SetAlpha(const double alpha1, const double alpha2);
  void Reset();
  bool IsReady();

private:
  double ClampAlpha(const double alpha);

private:
  double _alphaLow  = 0.0;
  double _alphaHigh = 0.0;

  T _lowPassFilterLow  = static_cast<T>(0);
  T _lowPassFilterHigh = static_cast<T>(0);
  T _bandPassFilter    = static_cast<T>(0);
  T _bandStopFilter    = static_cast<T>(0);

  bool _hasValue = false;
};


template<typename T>
double DoubleEMAFilter<T>::ClampAlpha(const double alpha)
{
  if (!isfinite(alpha))
  {
    return 0.0;
  }

  if (alpha < 0.0)
  {
    return 0.0;
  }

  if (alpha > 1.0)
  {
    return 1.0;
  }

  return alpha;
}


template<typename T>
void DoubleEMAFilter<T>::SetAlpha(const double alpha1, const double alpha2)
{
  const double a1 = ClampAlpha(alpha1);
  const double a2 = ClampAlpha(alpha2);

  _alphaLow  = (a1 <  a2) ? a1 : a2;
  _alphaHigh = (a1 >= a2) ? a1 : a2;
}


template<typename T>
DoubleEMAFilter<T>::DoubleEMAFilter(const double alpha1, const double alpha2)
{
  SetAlpha(alpha1, alpha2);
  Reset();
}


template<typename T>
void DoubleEMAFilter<T>::Reset()
{
  _lowPassFilterLow  = static_cast<T>(0);
  _lowPassFilterHigh = static_cast<T>(0);
  _bandPassFilter    = static_cast<T>(0);
  _bandStopFilter    = static_cast<T>(0);

  _hasValue = false;
}


template<typename T>
bool DoubleEMAFilter<T>::IsReady()
{
  return _hasValue;
}


template<typename T>
T DoubleEMAFilter<T>::AddValue(const T value)
{
  if (!isfinite((double)value))
  {
    return _bandStopFilter;
  }

  if (!_hasValue)
  {
    _lowPassFilterLow  = value;
    _lowPassFilterHigh = value;
    _bandPassFilter    = static_cast<T>(0);
    _bandStopFilter    = value;

    _hasValue = true;

    return _bandStopFilter;
  }

  const double value_d = (double)value;

  const double next_low =
    (_alphaLow * value_d) +
    ((1.0 - _alphaLow) * (double)_lowPassFilterLow);

  const double next_high =
    (_alphaHigh * value_d) +
    ((1.0 - _alphaHigh) * (double)_lowPassFilterHigh);

  if ((!isfinite(next_low)) || (!isfinite(next_high)))
  {
    Reset();
    return _bandStopFilter;
  }

  _lowPassFilterLow  = static_cast<T>(next_low);
  _lowPassFilterHigh = static_cast<T>(next_high);

  _bandPassFilter = static_cast<T>(_lowPassFilterHigh - _lowPassFilterLow);
  _bandStopFilter = static_cast<T>(value - _bandPassFilter);

  if (!isfinite((double)_bandPassFilter) || !isfinite((double)_bandStopFilter))
  {
    Reset();
    return _bandStopFilter;
  }

  return _bandStopFilter;
}


template<typename T>
T DoubleEMAFilter<T>::GetBandPass()
{
  return _bandPassFilter;
}


template<typename T>
T DoubleEMAFilter<T>::GetBandStop()
{
  return _bandStopFilter;
}

#endif