#ifndef _gba_const_dma_hpp_
#define _gba_const_dma_hpp_

#include <cassert>
#include <cstdint>


namespace gba {
  inline namespace constant {
    // ## GBA DMA Transfers
    // https://problemkaputt.de/gbatek.htm#gbatimers

    // DMA0CNT_H - DMA 0 Control (R/W)
    // DMA1CNT_H - DMA 1 Control (R/W)
    // DMA2CNT_H - DMA 2 Control (R/W)
    // DMA3CNT_H - DMA 3 Control (R/W)
    namespace DMACNT_H {
      // Dest Address Control (0=Increment,1=Decrement,2=Fixed,3=Increment/Reload)
      namespace DESTADDR {
        constexpr std::uint16_t INC       = 0 <<  5;    // Dest Address Control (Increment)
        constexpr std::uint16_t DEC       = 1 <<  5;    // Dest Address Control (Decrement)
        constexpr std::uint16_t FIXED     = 2 <<  5;    // Dest Address Control (Fixed)
        constexpr std::uint16_t INCREL    = 3 <<  5;    // Dest Address Control (Increment/Reload)
      }

      // Source Address Control (0=Increment,1=Decrement,2=Fixed,3=Prohibited)
      namespace SRCADDR {
        constexpr std::uint16_t INC       = 0 <<  7;    // Source Address Control (Increment)
        constexpr std::uint16_t DEC       = 1 <<  7;    // Source Address Control (Decrement)
        constexpr std::uint16_t FIXED     = 2 <<  7;    // Source Address Control (Fixed)
      }

      constexpr std::uint16_t REPEAT      = 1 <<  9;    // DMA Repeat (On) (Must be zero if Bit 11 set)

      constexpr std::uint16_t TYPE_16BIT  = 0 << 10;    // DMA Transfer Type (16bit)
      constexpr std::uint16_t TYPE_32BIT  = 1 << 10;    // DMA Transfer Type (32bit)

      constexpr std::uint16_t GAMEPAKDRQ  = 1 << 11;    // Game Pak DRQ (DMA3 only, DRQ <from> Game Pak, DMA3)

      // DMA Start Timing (0=Immediately, 1=VBlank, 2=HBlank, 3=Special)
      //   The 'Special' setting (Start Timing=3) depends on the DMA channel:
      //   DMA0=Prohibited, DMA1/DMA2=Sound FIFO, DMA3=Video Capture
      inline namespace TIMING {
        constexpr std::uint16_t IMMEDIATE = 0 << 12;    // DMA Start Timing (Immediately)
        constexpr std::uint16_t VBLANK    = 1 << 12;    // DMA Start Timing (VBlank)
        constexpr std::uint16_t HBLANK    = 2 << 12;    // DMA Start Timing (HBlank)
        constexpr std::uint16_t SPECIAL   = 3 << 12;    // DMA Start Timing (Special; DMA0=Prohibited, DMA1/DMA2=Sound FIFO, DMA3=Video Capture)
      }

      constexpr std::uint16_t IRQ         = 1 << 14;    // IRQ upon end of Word Count (Enable)

      constexpr std::uint16_t ENABLE      = 1 << 15;    // DMA Enable (On)
    }
  }   // inline namespace constant
}   // namespace gba

#endif
