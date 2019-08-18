#ifndef _gba_memory_hpp_
#define _gba_memory_hpp_

#include <cstdint>

#include "type.hpp"


namespace gba::memory {
  // General Internal Memory
  constexpr std::uintptr_t BIOS        = 0x00000000;    // BIOS - System ROM (16 KBytes)
  constexpr std::uintptr_t WRAM_BOARD  = 0x02000000;    // WRAM - On-board Work RAM (256 KBytes) 2 Wait
  constexpr std::uintptr_t WRAM_CHIP   = 0x03000000;    // WRAM - On-chip Work RAM (32 KBytes)
  constexpr std::uintptr_t IO          = 0x04000000;    // I/O Registers

  // Internal Display Memory
  constexpr std::uintptr_t PALETTE_BG  = 0x05000000;    // BG Palette RAM (512 Bytes)
  constexpr std::uintptr_t PALETTE_OBJ = 0x05000200;    // OBJ Palette RAM (512 Bytes)
  constexpr std::uintptr_t VRAM        = 0x06000000;    // VRAM - Video RAM (96 KBytes)
  constexpr std::uintptr_t OAM         = 0x07000000;    // OAM - OBJ Attributes (1 Kbyte)

  // VRAM
  inline namespace vram {
    // BG Mode 0, 1, 2 : Tile address (cf. BGCNT)
    template<unsigned int CharacterBaseBlock>
    constexpr std::uintptr_t VRAM_BGTILE    = 0x06000000 + CharacterBaseBlock * 0x4000;

    // BG Mode 0, 1, 2 : Map address (cf. BGCNT)
    template<unsigned int ScreenBaseBlock>
    constexpr std::uintptr_t VRAM_BGMAP     = 0x06000000 + ScreenBaseBlock * 0x800;

    constexpr std::uintptr_t VRAM_FRAME0    = 0x06000000;    // BG Mode 3, 4, 5: 80 KBytes (Mode 3) / 40 KBytes (Mode 4, 5) Frame 0 buffer (only 75K actually used in Mode 3, only 37.5K used in Mode 4)
    constexpr std::uintptr_t VRAM_FRAME1    = 0x0600A000;    // BG Mode 4, 5: 40 KBytes Frame 1 buffer (only 37.5K used in Mode 4)

    constexpr std::uintptr_t VRAM_OBJTILE32 = 0x06010000;    // BG Mode 0, 1, 2: 32 KBytes OBJ Tiles
    constexpr std::uintptr_t VRAM_OBJTILE16 = 0x06014000;    // BG Mode 3, 4, 5: 16 KBytes OBJ Tiles
  }

  // External Memory (Game Pak)
  // TODO
} // namespace gba::memory


namespace gba::template_memory {
  // General Internal Memory
  template<typename T = std::uint16_t>                                                   // BIOS - System ROM (16 KBytes)
  static const auto BIOS = reinterpret_cast<volatile T*>(memory::BIOS);
  template<typename T = std::uint16_t>                                                   // WRAM - On-board Work RAM (256 KBytes) 2 Wait
  static const auto WRAM_BOARD = reinterpret_cast<volatile T*>(memory::WRAM_BOARD);
  template<typename T = std::uint16_t>                                                   // WRAM - On-chip Work RAM (32 KBytes)
  static const auto WRAM_CHIP = reinterpret_cast<volatile T*>(memory::WRAM_CHIP);
  template<typename T = std::uint16_t>                                                   // I/O Registers
  static const auto IO = reinterpret_cast<volatile T*>(memory::IO);

  // Internal Display Memory
  template<typename T = std::uint16_t>                                                   // BG Palette RAM (512 Bytes)
  static const auto PALETTE_BG = reinterpret_cast<volatile T*>(memory::PALETTE_BG);
  template<typename T = std::uint16_t>                                                   // OBJ Palette RAM (512 Bytes)
  static const auto PALETTE_OBJ = reinterpret_cast<volatile T*>(memory::PALETTE_OBJ);
  template<typename T = std::uint16_t>                                                   // VRAM - Video RAM (96 KBytes)
  static const auto VRAM = reinterpret_cast<volatile T*>(memory::VRAM);
  template<typename T = OBJAttr>                                                         // OAM - OBJ Attributes (1 Kbyte)
  static const auto OAM = reinterpret_cast<volatile T*>(memory::OAM);

  // VRAM
  inline namespace vram {
    template<unsigned int CharacterBaseBlock, typename T = std::uint16_t>                // BG Mode 0, 1, 2 : Tile address (cf. BGCNT)
    static const auto VRAM_BGTILE = reinterpret_cast<volatile T*>(memory::VRAM_BGTILE<CharacterBaseBlock>);

    template<unsigned int ScreenBaseBlock, typename T = std::uint16_t>                   // BG Mode 0, 1, 2 : Map address (cf. BGCNT)
    static const auto VRAM_BGMAP = reinterpret_cast<volatile T*>(memory::VRAM_BGMAP<ScreenBaseBlock>);

    template<typename T = std::uint16_t>                                                 // BG Mode 3, 4, 5: 80 KBytes (Mode 3) / 40 KBytes (Mode 4, 5) Frame 0 buffer (only 75K actually used in Mode 3, only 37.5K used in Mode 4)
    static const auto VRAM_FRAME0 = reinterpret_cast<volatile T*>(memory::VRAM_FRAME0);
    template<typename T = std::uint16_t>                                                 // BG Mode 4, 5: 40 KBytes Frame 1 buffer (only 37.5K used in Mode 4)
    static const auto VRAM_FRAME1 = reinterpret_cast<volatile T*>(memory::VRAM_FRAME1);

    template<typename T = std::uint16_t>                                                 // BG Mode 0, 1, 2: 32 KBytes OBJ Tiles
    static const auto VRAM_OBJTILE32 = reinterpret_cast<volatile T*>(memory::VRAM_OBJTILE32);
    template<typename T = std::uint16_t>                                                 // BG Mode 3, 4, 5: 16 KBytes OBJ Tiles
    static const auto VRAM_OBJTILE16 = reinterpret_cast<volatile T*>(memory::VRAM_OBJTILE16);
  }

  // External Memory (Game Pak)
  // TODO
} // namespace gba::template_memory


namespace gba::pointer_memory {
  // General Internal Memory
  static const auto& BIOS         = template_memory::BIOS<>;                  // BIOS - System ROM (16 KBytes)
  static const auto& WRAM_BOARD   = template_memory::WRAM_BOARD<>;            // WRAM - On-board Work RAM (256 KBytes) 2 Wait
  static const auto& WRAM_CHIP    = template_memory::WRAM_CHIP<>;             // WRAM - On-chip Work RAM (32 KBytes)
  static const auto& IO           = template_memory::IO<>;                    // I/O Registers

  // Internal Display Memory
  static const auto& PALETTE_BG   = template_memory::PALETTE_BG<>;            // BG Palette RAM (512 Bytes)
  static const auto& PALETTE_OBJ  = template_memory::PALETTE_OBJ<>;           // OBJ Palette RAM (512 Bytes)
  static const auto& VRAM         = template_memory::VRAM<>;                  // VRAM - Video RAM (96 KBytes)
  static const auto& OAM          = template_memory::OAM<>;                   // OAM - OBJ Attributes (1 Kbyte)
  static const auto& OAM_ROTSCALE = template_memory::OAM<OBJRotScaleAttr>;    // OAM - OBJ Attributes (1 Kbyte)

  // VRAM
  inline namespace vram {
    template<unsigned int CharacterBaseBlock>                                 // BG Mode 0, 1, 2 : Tile address (cf. BGCNT)
    static const auto& VRAM_BGTILE = template_memory::VRAM_BGTILE<CharacterBaseBlock>;

    template<unsigned int ScreenBaseBlock>                                    // BG Mode 0, 1, 2 : Map address (cf. BGCNT)
    static const auto& VRAM_BGMAP = template_memory::VRAM_BGMAP<ScreenBaseBlock>;

    static const auto& VRAM_FRAME0 = template_memory::VRAM_FRAME0<>;          // BG Mode 3, 4, 5: 80 KBytes (Mode 3) / 40 KBytes (Mode 4, 5) Frame 0 buffer (only 75K actually used in Mode 3, only 37.5K used in Mode 4)
    static const auto& VRAM_FRAME1 = template_memory::VRAM_FRAME1<>;          // BG Mode 4, 5: 40 KBytes Frame 1 buffer (only 37.5K used in Mode 4)

    static const auto& VRAM_OBJTILE32 = template_memory::VRAM_OBJTILE32<>;    // BG Mode 0, 1, 2: 32 KBytes OBJ Tiles
    static const auto& VRAM_OBJTILE16 = template_memory::VRAM_OBJTILE16<>;    // BG Mode 3, 4, 5: 16 KBytes OBJ Tiles
  }

  // External Memory (Game Pak)
  // TODO
} // namespace gba::pointer_memory

#endif
