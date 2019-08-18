#pragma once


template<typename T>
T RotateEnumLeft(T& value, T end) {
  if (static_cast<unsigned int>(value) == 0) {
    value = static_cast<T>(static_cast<unsigned int>(end) - 1);
  } else {
    value = static_cast<T>(static_cast<unsigned int>(value) - 1);
  }
  return value;
}


template<typename T>
T RotateEnumRight(T& value, T end) {
  value = static_cast<T>(static_cast<unsigned int>(value) + 1);
  if (value == end) {
    value = static_cast<T>(0);
  }
  return value;
}
