#pragma once

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

  template<typename T>
  inline void Copy(std::uintptr_t destAddress, const T& src) {
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using SV = typename T::value_type;
    constexpr std::size_t SVSize = sizeof(SV);

    static_assert(SVSize % sizeof(u16) == 0);

    if constexpr (SVSize < sizeof(u32) && SVSize * src.size() % sizeof(u32) == 0) {
      Copy(reinterpret_cast<volatile u32*>(destAddress), reinterpret_cast<const u32*>(src.data()), src.size() * SVSize / sizeof(u32));
      return;
    }

    Copy(reinterpret_cast<volatile SV*>(destAddress), src.data(), src.size());
  }
}


template<std::uint_fast8_t CharacterBaseBlock, typename T>
inline void BGCopyTile(const T& src) {
  CopyVRAMInternal::Copy(gba::memory::VRAM_BGTILE<CharacterBaseBlock>, src);
}


template<std::uint_fast8_t ScreenBaseBlock, typename T>
inline void BGCopyMap(const T& src) {
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


template<std::uint_fast8_t BeginIndex = 0, typename T>
inline void BGCopyPalette(const T& src) {
  CopyVRAMInternal::Copy(gba::memory::PALETTE_BG + BeginIndex * 32, src);
}


template<bool Narrow = false, typename T>
inline void OBJCopyTile(const T& src) {
  CopyVRAMInternal::Copy(Narrow ? gba::memory::VRAM_OBJTILE16 : gba::memory::VRAM_OBJTILE32, src);
}


template<std::uint_fast8_t BeginIndex = 0, typename T>
inline void OBJCopyPalette(const T& src) {
  CopyVRAMInternal::Copy(gba::memory::PALETTE_OBJ + BeginIndex * 32, src);
}
