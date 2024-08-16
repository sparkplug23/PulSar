// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

// #include <ArduinoJson/Configuration.hpp>
#include "is_same2.hpp"

namespace ARDUINOJSON_NAMESPACE2 {
// A meta-function that returns true if T is an integral type.
template <typename T>
struct is_integral2 {
  static const bool value =
      is_same2<T, signed char>::value || is_same2<T, unsigned char>::value ||
      is_same2<T, signed short>::value || is_same2<T, unsigned short>::value ||
      is_same2<T, signed int>::value || is_same2<T, unsigned int>::value ||
      is_same2<T, signed long>::value || is_same2<T, unsigned long>::value ||
#if ARDUINOJSON_HAS_LONG_LONG
      is_same2<T, signed long long>::value ||
      is_same2<T, unsigned long long>::value ||
#endif
#if ARDUINOJSON_HAS_INT64
      is_same2<T, signed __int64>::value ||
      is_same2<T, unsigned __int64>::value ||
#endif
      is_same2<T, char>::value || is_same2<T, bool>::value;
};

template <typename T>
struct is_integral2<const T> : is_integral2<T> {};

}