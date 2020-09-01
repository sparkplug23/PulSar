
#pragma once

// A meta-function that returns true if T is an integral type.
template <typename T>
struct is_char_type {
  static const bool value =
      std::is_same<T, signed char>::value || 
      std::is_same<T, unsigned char>::value || 
      std::is_same<T, char>::value;
};

template <typename T>
struct is_char_type<const T> : is_char_type<T> {};
