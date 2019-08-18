#pragma once

#include <array>
#include <cstdint>

#include <gba.hpp>


template<std::uint_fast8_t ScreenBaseBlock, unsigned int BGWidth, unsigned int DestinationX, unsigned int DestinationY, unsigned int SourceWidth, typename T>
inline void BGCopyMap(const T& src) {
  static_assert(SourceWidth <= BGWidth);

  constexpr auto ArraySize = std::tuple_size<T>::value;
  static_assert(ArraySize % SourceWidth == 0);

  constexpr auto SourceHeight = ArraySize / SourceWidth;

  auto ptrSource = src.data();
  auto ptrDestination = gba::pointer_memory::VRAM_BGMAP<ScreenBaseBlock> + (DestinationY * BGWidth + DestinationX);
  for (unsigned int y = 0; y < SourceHeight; y++) {
    for (unsigned int x = 0; x < SourceWidth; x++) {
      *ptrDestination++ = *ptrSource++;
    }
    ptrDestination += BGWidth - SourceWidth;
  }
}


template<std::uint_fast8_t ScreenBaseBlock, unsigned int BGWidth, unsigned int X, unsigned int Y, unsigned int Width, unsigned int Height>
inline void BGClearMap() {
  static_assert(Width <= BGWidth);

  auto ptr = gba::pointer_memory::VRAM_BGMAP<ScreenBaseBlock> + (Y * BGWidth + X);
  for (unsigned int y = 0; y < Height; y++) {
    for (unsigned int x = 0; x < Width; x++) {
      *ptr++ = 0;
    }
    ptr += BGWidth - Width;
  }
}
