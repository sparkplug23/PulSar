
#pragma once

#include "integral_constant2.hpp"


//namespace ARDUINOJSON_NAMESPACE2 {

// A meta-function that returns true if types T and U are the same.
template <typename T, typename U>
struct is_same2 : false_type2 {};

template <typename T>
struct is_same2<T, T> : true_type2 {};

//}

