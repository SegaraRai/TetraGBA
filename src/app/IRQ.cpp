#include "IRQ.hpp"
#include "DbgPrintf.hpp"
#include "Sound/MusicManager.hpp"
#include "Sound/SoundManager.hpp"

#include <cstdint>
#include <gba.hpp>


void CommonISR() {
  gba::reg::IME = 0;

  const auto flag = gba::reg::IF;

  //DbgPrintf("IRQ: %d\n");

  std::uint_fast16_t ackFlag = 0;
  std::uint_fast16_t biosAckFlag = 0;

  if (flag & gba::IF::VBLANK) {
    ackFlag |= gba::IF::VBLANK;
    biosAckFlag |= gba::IF::VBLANK;

    MusicManager::GetInstance().Step();
  }

  ackFlag |= SoundManager::GetInstance().IRQ(flag);

  gba::reg::IF = ackFlag;
  gba::reg::INTCHK = biosAckFlag;

  gba::reg::IME = gba::IME::ENABLE;
}


void SetCommonIRQFlags() {
  gba::reg::DISPSTAT |= gba::DISPSTAT::VBLANK_IRQ;
  gba::reg::IE = gba::IE::VBLANK | gba::IE::DMA1 | gba::IE::DMA2;   // DMA is for SoundManager

  // gba::reg::IME should be set by the caller
}
