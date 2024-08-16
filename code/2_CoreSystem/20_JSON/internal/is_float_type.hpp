
#pragma once

// A meta-function that returns true if T is an integral type.
template <typename T>
struct is_float_type {
  static const bool value =
      std::is_same<T, float>::value;
};

template <typename T>
struct is_float_type<const T> : is_float_type<T> {};
