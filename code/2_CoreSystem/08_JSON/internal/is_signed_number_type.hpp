
#pragma once

// A meta-function that returns true if T is an integral type.
template <typename T>
struct is_signed_number_type {
  static const bool value =
      std::is_same<T, signed short>::value || 
      std::is_same<T, signed int>::value || 
      std::is_same<T, signed long>::value || 
      std::is_same<T, int8_t>::value  || 
      std::is_same<T, int16_t>::value || 
      std::is_same<T, int32_t>::value || 
      std::is_same<T, int64_t>::value ;
};

template <typename T>
struct is_signed_number_type<const T> : is_signed_number_type<T> {};
