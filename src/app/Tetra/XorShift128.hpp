#pragma once

#include <cstdint>

#include "../DbgPrintf.hpp"


class random_xorshift128 {
public:
  using result_type = std::uint_fast32_t;

  static constexpr result_type DefaultX = 123456789;
  static constexpr result_type DefaultY = 362436069;
  static constexpr result_type DefaultZ = 521288629;

private:
  result_type w;
  result_type x;
  result_type y;
  result_type z;

public:
  static constexpr result_type min() {
    return 0;
  }

  static constexpr result_type max() {
    return 0xFFFFFFFF;
  }

  random_xorshift128(const random_xorshift128&) = delete;

  void operator=(const random_xorshift128&) = delete;

  constexpr random_xorshift128(result_type w, result_type x = DefaultX, result_type y = DefaultY, result_type z = DefaultZ) :
    w(w),
    x(x),
    y(y),
    z(z)
  {
    DbgPrintf("random_xorshift128: w = %08x, x = %08x, y = %08x, z = %08x\n", w, x, y, z);
  }

  constexpr double entropy() const noexcept {
    // https://en.cppreference.com/w/cpp/numeric/random/random_device/entropy
    // A deterministic random number generator (e.g. a pseudo-random engine) has entropy zero.
    return 0.;
  }

  constexpr result_type operator()() {
    const result_type t = (x ^ (x << 11)) & 0xFFFFFFFF;
    x = y;
    y = z;
    z = w;
    return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
  }
};
