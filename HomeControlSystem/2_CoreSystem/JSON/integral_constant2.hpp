

#pragma once

// #include <ArduinoJson/Namespace.hpp>
#include "2_CoreSystem/JSON/namespace.hpp"

//namespace ARDUINOJSON_NAMESPACE2 {

template <typename T, T v>
struct integral_constant2 {
  static const T value = v;
};

typedef integral_constant2<bool, true> true_type2;
typedef integral_constant2<bool, false> false_type2;

//}  // namespace ARDUINOJSON_NAMESPACE
