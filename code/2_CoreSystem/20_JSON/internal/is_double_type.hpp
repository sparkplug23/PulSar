
#pragma once

// A meta-function that returns true if T is an integral type.
template <typename T>
struct is_double_type {
  static const bool value =
      std::is_same<T, double>::value;
};

template <typename T>
struct is_double_type<const T> : is_double_type<T> {};
