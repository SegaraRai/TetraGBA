#ifndef _gba_const_timer_hpp_
#define _gba_const_timer_hpp_

#include <cassert>
#include <cstdint>


namespace gba {
  inline namespace constant {
    // ## GBA Timers
    // https://problemkaputt.de/gbatek.htm#gbatimers

    // TM0CNT_H - Timer 0 Control (R/W)
    // TM1CNT_H - Timer 1 Control (R/W)
    // TM2CNT_H - Timer 2 Control (R/W)
    // TM3CNT_H - Timer 3 Control (R/W)
    namespace TMCNT_H {
      inline namespace TIMING {
        constexpr std::uint16_t TIMING_1    = 0 << 0;   // 16.78 / 1 MHz
        constexpr std::uint16_t TIMING_64   = 1 << 0;   // 16.78 / 64 MHz
        constexpr std::uint16_t TIMING_256  = 2 << 0;   // 16.78 / 256 MHz
        constexpr std::uint16_t TIMING_1024 = 3 << 0;   // 16.78 / 1024 MHz
      }

      constexpr std::uint16_t COUNTUP       = 1 << 2;   // Use Count-up Timing (the time is incremented each time when the previous counter overflows regardless of TIMING); cannot be used in TM0CNT_H
      constexpr std::uint16_t IRQ           = 1 << 6;   // Timer IRQ Enable (IRQ on Timer overflow)
      constexpr std::uint16_t OPERATE       = 1 << 7;   // Timer Start/Stop
    }
  }   // inline namespace constant
}   // namespace gba

#endif
