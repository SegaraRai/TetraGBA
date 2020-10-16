#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include <gba.hpp>


namespace CopyVRAMInternal {
  template<typename T>
  inline void Copy(volatile T* dest, const T* src, std::size_t count) {
    for (std::size_t i = 0; i < count; i++) {
      dest[i] = src[i];
    }
  }

  template<typename T, std::size_t S>
  inline void Copy(std::uintptr_t destAddress, const std::array<T, S>& src) {
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;

    constexpr std::size_t TSize = sizeof(T);

    static_assert(TSize % sizeof(u16) == 0);

    if constexpr (TSize < sizeof(u32) && TSize * S % sizeof(u32) == 0) {
      Copy(reinterpret_cast<volatile u32*>(destAddress), reinterpret_cast<const u32*>(src.data()), S * TSize / sizeof(u32));
      return;
    }

    Copy(reinterpret_cast<volatile T*>(destAddress), src.data(), S);
  }
}


template<std::uint_fast8_t CharacterBaseBlock, typename T, std::size_t S>
inline void BGCopyTile(const std::array<T, S>& src) {
  CopyVRAMInternal::Copy(gba::memory::VRAM_BGTILE<CharacterBaseBlock>, src);
}


template<std::uint_fast8_t ScreenBaseBlock, typename T, std::size_t S>
inline void BGCopyMap(const std::array<T, S>& src) {
  CopyVRAMInternal::Copy(gba::memory::VRAM_BGMAP<ScreenBaseBlock>, src);
}


template<std::uint_fast8_t ScreenBaseBlock>
inline void BGClearMap() {
  using Block = std::uint32_t;
  const auto map = gba::template_memory::VRAM_BGMAP<ScreenBaseBlock, Block>;
  for (std::size_t i = 0; i < 0x800 / sizeof(Block); i++) {
    map[i] = 0;
  }
}


template<std::uint_fast8_t BeginIndex = 0, typename T, std::size_t S>
inline void BGCopyPalette(const std::array<T, S>& src) {
  CopyVRAMInternal::Copy(gba::memory::PALETTE_BG + BeginIndex * 32, src);
}


template<bool Narrow = false, typename T, std::size_t S>
inline void OBJCopyTile(const std::array<T, S>& src) {
  CopyVRAMInternal::Copy(Narrow ? gba::memory::VRAM_OBJTILE16 : gba::memory::VRAM_OBJTILE32, src);
}


template<std::uint_fast8_t BeginIndex = 0, typename T, std::size_t S>
inline void OBJCopyPalette(const std::array<T, S>& src) {
  CopyVRAMInternal::Copy(gba::memory::PALETTE_OBJ + BeginIndex * 32, src);
}
