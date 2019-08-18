#ifndef _gba_const_interrupt_hpp_
#define _gba_const_interrupt_hpp_

#include <cassert>
#include <cstdint>


namespace gba {
  inline namespace constant {
    // ## GBA Interrupt Control
    // https://problemkaputt.de/gbatek.htm#gbainterruptcontrol

    // IME - Interrupt Master Enable Register (R/W)
    namespace IME {
      constexpr std::uint16_t ENABLE = 1 << 0;
    }

    // IE - Interrupt Enable Register
    namespace IE {
      constexpr std::uint16_t VBLANK   = 1 <<  0;   // LCD V-Blank
      constexpr std::uint16_t HBLANK   = 1 <<  1;   // LCD H-Blank
      constexpr std::uint16_t VCOUNTER = 1 <<  2;   // LCD V-Counter Match
      constexpr std::uint16_t TIMER0   = 1 <<  3;   // Timer 0 Overflow
      constexpr std::uint16_t TIMER1   = 1 <<  4;   // Timer 1 Overflow
      constexpr std::uint16_t TIMER2   = 1 <<  5;   // Timer 2 Overflow
      constexpr std::uint16_t TIMER3   = 1 <<  6;   // Timer 3 Overflow
      constexpr std::uint16_t SIO      = 1 <<  7;   // Serial Communication
      constexpr std::uint16_t DMA0     = 1 <<  8;   // DMA 0
      constexpr std::uint16_t DMA1     = 1 <<  9;   // DMA 1
      constexpr std::uint16_t DMA2     = 1 << 10;   // DMA 2
      constexpr std::uint16_t DMA3     = 1 << 11;   // DMA 3
      constexpr std::uint16_t KEYPAD   = 1 << 12;   // Keypad
      constexpr std::uint16_t GAMEPAK  = 1 << 13;   // Game Pak (external IRQ source)
    }

    // IF - Interrupt Request Flags / IRQ Acknowledge
    namespace IF {
      using namespace IE;
    }
  }   // inline namespace constant
}   // namespace gba

#endif
