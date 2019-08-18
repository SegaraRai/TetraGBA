#pragma once

#include <array>
#include <cstdint>


template<typename T, typename... Args>
constexpr std::array<std::uint16_t, 256> MergePalette(T head1, T head2, Args... args) {
  return MergePalette(MergePalette(head1, head2), args...);
}


constexpr std::array<std::uint16_t, 256> MergePalette(const std::array<std::uint16_t, 256>& a) {
  return a;
}


constexpr std::array<std::uint16_t, 256> MergePalette(const std::array<std::uint16_t, 256>& a, const std::array<std::uint16_t, 256>& b) {
  std::array<std::uint16_t, 256> palette = a;

  for (unsigned int paletteIndex = 0; paletteIndex < 16; paletteIndex++) {
    bool existsInA = false;
    bool existsInB = false;
    for (unsigned int colorIndex = 0; colorIndex < 16; colorIndex++) {
      if (a[paletteIndex * 16 + colorIndex]) {
        existsInA = true;
      }
      if (b[paletteIndex * 16 + colorIndex]) {
        existsInB = true;
      }
    }
    if (existsInA && existsInB) {
      assert(false);
    }
    if (!existsInA && existsInB) {
      for (unsigned int colorIndex = 0; colorIndex < 16; colorIndex++) {
        palette[paletteIndex * 16 + colorIndex] = b[paletteIndex * 16 + colorIndex];
      }
    }
  }

  return palette;
}
