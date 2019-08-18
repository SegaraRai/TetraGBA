#ifndef _gba_const_keypad_hpp_
#define _gba_const_keypad_hpp_

#include <cassert>
#include <cstdint>


namespace gba {
  inline namespace constant {
    // ## GBA Keypad Input
    // https://problemkaputt.de/gbatek.htm#gbakeypadinput

    // KEYINPUT - Key Status (R)
    namespace KEYINPUT {
      constexpr std::uint16_t A      = 1 << 0;    // Button A (0=Pressed, 1=Released)
      constexpr std::uint16_t B      = 1 << 1;    // Button B (0=Pressed, 1=Released)
      constexpr std::uint16_t SELECT = 1 << 2;    // Select   (0=Pressed, 1=Released)
      constexpr std::uint16_t START  = 1 << 3;    // Start    (0=Pressed, 1=Released)
      constexpr std::uint16_t RIGHT  = 1 << 4;    // Right    (0=Pressed, 1=Released)
      constexpr std::uint16_t LEFT   = 1 << 5;    // Left     (0=Pressed, 1=Released)
      constexpr std::uint16_t UP     = 1 << 6;    // Up       (0=Pressed, 1=Released)
      constexpr std::uint16_t DOWN   = 1 << 7;    // Down     (0=Pressed, 1=Released)
      constexpr std::uint16_t R      = 1 << 8;    // Button R (0=Pressed, 1=Released)
      constexpr std::uint16_t L      = 1 << 9;    // Button L (0=Pressed, 1=Released)
    }

    // KEYCNT - Key Interrupt Control (R/W)
    namespace KEYCNT {
      constexpr std::uint16_t A        = 1 <<  0;   // Button A (0=Ignore, 1=Select)
      constexpr std::uint16_t B        = 1 <<  1;   // Button B (0=Ignore, 1=Select)
      constexpr std::uint16_t SELECT   = 1 <<  2;   // Select   (0=Ignore, 1=Select)
      constexpr std::uint16_t START    = 1 <<  3;   // Start    (0=Ignore, 1=Select)
      constexpr std::uint16_t RIGHT    = 1 <<  4;   // Right    (0=Ignore, 1=Select)
      constexpr std::uint16_t LEFT     = 1 <<  5;   // Left     (0=Ignore, 1=Select)
      constexpr std::uint16_t UP       = 1 <<  6;   // Up       (0=Ignore, 1=Select)
      constexpr std::uint16_t DOWN     = 1 <<  7;   // Down     (0=Ignore, 1=Select)
      constexpr std::uint16_t R        = 1 <<  8;   // Button R (0=Ignore, 1=Select)
      constexpr std::uint16_t L        = 1 <<  9;   // Button L (0=Ignore, 1=Select)

      constexpr std::uint16_t MASTER   = 1 << 14;   // IRQ Enable Flag (Enable)

      constexpr std::uint16_t MODE_OR  = 0 << 15;   // IRQ Condition (Logical OR):  an interrupt is requested when at least one of the selected buttons is pressed.
      constexpr std::uint16_t MODE_AND = 1 << 15;   // IRQ Condition (Logical AND): an interrupt is requested when ALL of the selected buttons are pressed.
    }
  }   // inline namespace constant
}   // namespace gba

#endif
