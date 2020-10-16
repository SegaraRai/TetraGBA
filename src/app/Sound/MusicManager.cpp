#include "MusicManager.hpp"
#include "../DbgPrintf.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <gba.hpp>


namespace {
  namespace CommandId {
    constexpr std::uint_fast8_t NOP               = 0x00;
    constexpr std::uint_fast8_t RESTLONG          = 0x01;
    constexpr std::uint_fast8_t BPM               = 0x02;
    constexpr std::uint_fast8_t SOUNDCNT_L        = 0x04;
    constexpr std::uint_fast8_t SOUNDCNT_H        = 0x05;
    constexpr std::uint_fast8_t SOUNDCNT_X        = 0x06;
    constexpr std::uint_fast8_t SOUNDBIAS         = 0x07;
    constexpr std::uint_fast8_t WAVE_RAM          = 0x08;
    constexpr std::uint_fast8_t SOUND1CNT_L       = 0x0C;
    constexpr std::uint_fast8_t SOUND1CNT_L_RESET = 0x0D;
    constexpr std::uint_fast8_t SOUND1ON_BASE     = 0x10;
    constexpr std::uint_fast8_t SOUND2ON_BASE     = 0x14;
    constexpr std::uint_fast8_t SOUND3ON_BASE     = 0x18;
    constexpr std::uint_fast8_t SOUND4ON_BASE     = 0x1C;
    constexpr std::uint_fast8_t SOUND1OFF         = 0x20;
    constexpr std::uint_fast8_t SOUND2OFF         = 0x21;
    constexpr std::uint_fast8_t SOUND3OFF         = 0x22;
    constexpr std::uint_fast8_t SOUND4OFF         = 0x23;
    constexpr std::uint_fast8_t SETREG_BASE       = 0x40;
    constexpr std::uint_fast8_t SETREG_END        = 0x4F;
    constexpr std::uint_fast8_t RESTSHORT_BASE    = 0x80;
    constexpr std::uint_fast8_t RESTSHORT_END     = 0xFF;
  }
}


std::uint_fast8_t MusicManager::Read8() {
  const auto ret = *mPtrCurrent++;
  return ret;
}


std::uint_fast16_t MusicManager::Read16LE() {
  const auto a = *mPtrCurrent++;
  const auto b = *mPtrCurrent++;
  return (b << 8) | a;
}


MusicManager::MusicManager() :
  mPlaying(false),
  mDivisor(0),
  mTickPerDiv(0),
  mCurrentTick(0),
  mNextTick(0),
  mFrequencyTable(nullptr),
  mEnvelopeTable(nullptr),
  mChannel3ValueTable(nullptr),
  mPtrCurrent(nullptr),
  mPtrEnd(nullptr),
  mPtrLoopPoint(nullptr)
{
  gba::reg::SOUNDCNT_X = gba::SOUNDCNT_X::MASTER_ENABLE;

  gba::reg::SOUNDCNT_L = gba::SOUNDCNT_L::SOUND1_RL | gba::SOUNDCNT_L::SOUND2_RL | gba::SOUNDCNT_L::SOUND3_RL | gba::SOUNDCNT_L::SOUND4_RL | gba::SOUNDCNT_L::VOLUMERL(7);
  gba::reg::SOUNDCNT_H |= gba::SOUNDCNT_H::SOUND1234_VOLUME50;
  //gba::reg::SOUNDBIAS = gba::SOUNDBIAS::AMPCYCLE_6BIT;

  gba::reg::SOUND3CNT_L = gba::SOUND3CNT_L::SOUND3_ON | gba::SOUND3CNT_L::BANK1;
  gba::reg::SOUND3CNT_H = gba::SOUND3CNT_H::VOLUME50;
}


MusicManager& MusicManager::GetInstance() {
  static MusicManager musicManager;
  return musicManager;
}


void MusicManager::Play(const std::uint32_t* data) {
  Stop();

  const auto ptrBegin = reinterpret_cast<const std::uint8_t*>(data);
  mPtrCurrent = ptrBegin;

  const auto divisor = Read16LE();
  const auto offsetToEnd = Read16LE();
  const auto offsetToLoopPoint = Read16LE();
  const auto numFrequencies = Read16LE();
  const auto numEnvelopes = Read16LE();
  const auto numChannel3Values = Read16LE();

  mDivisor = divisor;

  mPtrEnd = ptrBegin + offsetToEnd;
  mPtrLoopPoint = ptrBegin + offsetToLoopPoint;

  mFrequencyTable = reinterpret_cast<const std::uint16_t*>(mPtrCurrent);
  mPtrCurrent += numFrequencies * sizeof(std::uint16_t) / sizeof(std::uint8_t);

  mEnvelopeTable = reinterpret_cast<const std::uint16_t*>(mPtrCurrent);
  mPtrCurrent += numEnvelopes * sizeof(std::uint16_t) / sizeof(std::uint8_t);

  mChannel3ValueTable = reinterpret_cast<const std::uint16_t*>(mPtrCurrent);
  mPtrCurrent += numChannel3Values * sizeof(std::uint16_t) / sizeof(std::uint8_t);

  //DbgPrintf("%p %p %p\n", mFrequencyTable, mEnvelopeTable, mChannel3ValueTable);

  mTickPerDiv = 0;
  mCurrentTick = 0;
  mNextTick = 0;

  mPlaying = true;
}


void MusicManager::Stop() {
  mPlaying = false;

  gba::reg::SOUND1CNT_L = 0;

  gba::reg::SOUND1CNT_H = 0;
  gba::reg::SOUND2CNT_L = 0;
  gba::reg::SOUND3CNT_H = 0;
  gba::reg::SOUND4CNT_L = 0;
}


void MusicManager::Resume() {
  if (mPtrEnd) {
    mPlaying = true;
  }
}


void MusicManager::Step() {
  //DbgPrintf("c: %08x, l: %08x, e: %08x, p: %d, t: %d -> %d\n", (std::uintptr_t)mPtrCurrent, (std::uintptr_t)mPtrLoopPoint, (std::uintptr_t)mPtrEnd, mPlaying ? 1 : 0, mCurrentTick, mNextTick);
  if (!mPlaying) {
    return;
  }

  while (mCurrentTick == mNextTick) {
    const auto command = Read8();

    //DbgPrintf("C: %02X (vc = %d)\n", command, gba::reg::VCOUNT);

    switch (command) {
      case CommandId::NOP:
        break;

      case CommandId::RESTLONG:
        mNextTick += Read16LE() * mTickPerDiv;
        break;

      case CommandId::BPM:
      {
        const auto bpm = Read16LE();
        //mTickPerDiv = 60 /*sec. (1 min.)*/ * 60 /*Hz*/ / bpm / mDivisor;
        mTickPerDiv = (60 * 60 + (bpm * mDivisor + 1) / 2) / (bpm * mDivisor);
        //DbgPrintf("BPM set: %d, %d, %d\n", bpm, mTickPerDiv, mDivisor);
        break;
      }

      case CommandId::SOUNDCNT_L:
        gba::reg::SOUNDCNT_L = Read16LE();
        break;

      case CommandId::SOUNDCNT_H:
        gba::reg::SOUNDCNT_H = Read16LE();
        break;

      case CommandId::SOUNDCNT_X:
        gba::reg::SOUNDCNT_X = Read16LE();
        break;

      case CommandId::SOUNDBIAS:
        gba::reg::SOUNDBIAS = Read16LE();
        break;

      case CommandId::WAVE_RAM:
        gba::reg::SOUND3CNT_L = 0;

        gba::reg::WAVE_RAM0_L = Read16LE();
        gba::reg::WAVE_RAM0_H = Read16LE();
        gba::reg::WAVE_RAM1_L = Read16LE();
        gba::reg::WAVE_RAM1_H = Read16LE();
        gba::reg::WAVE_RAM2_L = Read16LE();
        gba::reg::WAVE_RAM2_H = Read16LE();
        gba::reg::WAVE_RAM3_L = Read16LE();
        gba::reg::WAVE_RAM3_H = Read16LE();

        gba::reg::SOUND3CNT_L = gba::SOUND3CNT_L::SOUND3_ON | gba::SOUND3CNT_L::BANK1;
        break;

      case CommandId::SOUND1CNT_L:
        gba::reg::SOUND1CNT_L = Read16LE();
        break;

      case CommandId::SOUND1CNT_L_RESET:
        gba::reg::SOUND1CNT_L = 0;
        break;

      case CommandId::SOUND1ON_BASE | 0x00:
      case CommandId::SOUND1ON_BASE | 0x01:
      case CommandId::SOUND1ON_BASE | 0x02:
      case CommandId::SOUND1ON_BASE | 0x03:
        if (command & 0x01) {
          gba::reg::SOUND1CNT_H = Read16LE();
        } else {
          gba::reg::SOUND1CNT_H = mEnvelopeTable[Read8()];
        }
        if (command & 0x02) {
          gba::reg::SOUND1CNT_X = Read16LE();
        } else {
          const auto value = Read8();
          gba::reg::SOUND1CNT_X = mFrequencyTable[value & 0x7F] | ((value & 0x80) ? gba::SOUND1CNT_X::USE_LENGTH : 0) | gba::SOUND1CNT_X::RESTART;
        }
        break;

      case CommandId::SOUND2ON_BASE | 0x00:
      case CommandId::SOUND2ON_BASE | 0x01:
      case CommandId::SOUND2ON_BASE | 0x02:
      case CommandId::SOUND2ON_BASE | 0x03:
        if (command & 0x01) {
          gba::reg::SOUND2CNT_L = Read16LE();
        } else {
          gba::reg::SOUND2CNT_L = mEnvelopeTable[Read8()];
        }
        if (command & 0x02) {
          gba::reg::SOUND2CNT_H = Read16LE();
        } else {
          const auto value = Read8();
          gba::reg::SOUND2CNT_H = mFrequencyTable[value & 0x7F] | ((value & 0x80) ? gba::SOUND2CNT_H::USE_LENGTH : 0) | gba::SOUND2CNT_H::RESTART;
        }
        break;

      case CommandId::SOUND3ON_BASE | 0x00:
      case CommandId::SOUND3ON_BASE | 0x01:
      case CommandId::SOUND3ON_BASE | 0x02:
      case CommandId::SOUND3ON_BASE | 0x03:
        if (command & 0x01) {
          gba::reg::SOUND3CNT_H = Read16LE();
        } else {
          gba::reg::SOUND3CNT_H = mChannel3ValueTable[Read8()];
        }
        if (command & 0x02) {
          gba::reg::SOUND3CNT_X = Read16LE();
        } else {
          const auto value = Read8();
          gba::reg::SOUND3CNT_X = mFrequencyTable[value & 0x7F] | ((value & 0x80) ? gba::SOUND3CNT_X::USE_LENGTH : 0) | gba::SOUND3CNT_X::RESTART;
        }
        break;

      case CommandId::SOUND4ON_BASE | 0x00:
      case CommandId::SOUND4ON_BASE | 0x01:
      case CommandId::SOUND4ON_BASE | 0x02:
      case CommandId::SOUND4ON_BASE | 0x03:
        if (command & 0x01) {
          gba::reg::SOUND4CNT_L = Read16LE();
        } else {
          gba::reg::SOUND4CNT_L = mEnvelopeTable[Read8()];
        }
        gba::reg::SOUND4CNT_H = Read8() | ((command & 0x02) ? gba::SOUND4CNT_H::USE_LENGTH : 0) | gba::SOUND4CNT_H::RESTART;
        break;

      case CommandId::SOUND1OFF:
        gba::reg::SOUND1CNT_H = 0;
        break;

      case CommandId::SOUND2OFF:
        gba::reg::SOUND2CNT_L = 0;
        break;

      case CommandId::SOUND3OFF:
        gba::reg::SOUND3CNT_H = 0;
        break;

      case CommandId::SOUND4OFF:
        gba::reg::SOUND4CNT_L = 0;
        break;

      default:
        if (CommandId::SETREG_BASE <= command && command <= CommandId::SETREG_END) {
          *reinterpret_cast<volatile std::uint16_t*>(0x04000060 + (command - CommandId::SETREG_BASE) * 2) = Read16LE();
        } else if (CommandId::RESTSHORT_BASE <= command && command <= CommandId::RESTSHORT_END) {
          mNextTick += (command - CommandId::RESTSHORT_BASE + 1) * mTickPerDiv;
        } else {
          DbgPrintf("MusicManager::Step() : undefined command %02X\n", command);
        }
        break;
    }


    // loop
    if (mPtrCurrent == mPtrEnd) {
      mPtrCurrent = mPtrLoopPoint;
    }
  }

  mCurrentTick++;
}
