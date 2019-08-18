#include "SoundManager.hpp"

#include <array>
#include <cstdint>
#include <gba.hpp>


SoundManager::SoundManager() :
  mChannelInfoA{
    0,
  },
  mChannelInfoB{
    0,
  }
{
  gba::reg::SOUNDCNT_X = gba::SOUNDCNT_X::MASTER_ENABLE;

  gba::reg::SOUNDCNT_H |=
      gba::SOUNDCNT_H::SOUNDA_VOLUME100 | gba::SOUNDCNT_H::SOUNDA_R | gba::SOUNDCNT_H::SOUNDA_L | gba::SOUNDCNT_H::SOUNDA_TIMER0 | gba::SOUNDCNT_H::SOUNDA_RESET |
      gba::SOUNDCNT_H::SOUNDB_VOLUME100 | gba::SOUNDCNT_H::SOUNDB_R | gba::SOUNDCNT_H::SOUNDB_L | gba::SOUNDCNT_H::SOUNDB_TIMER1 | gba::SOUNDCNT_H::SOUNDB_RESET;
}


SoundManager& SoundManager::GetInstance() {
  static SoundManager soundManager;
  return soundManager;
}


bool SoundManager::IsPlaying(Channel channel) const {
  return (channel == Channel::A ? mChannelInfoA : mChannelInfoB).dataCount;
}


void SoundManager::Play(Channel channel, const std::uint32_t* data) {
  struct ChannelSpec {
    ChannelInfo& channelInfo;
    volatile std::uint16_t& TMCNT_L;
    volatile std::uint16_t& TMCNT_H;
    const void* volatile& DMASAD;
    void* volatile& DMADAD;
    volatile std::uint16_t& DMACNT_L;
    volatile std::uint16_t& DMACNT_H;
    std::uintptr_t FIFO;
  };


  std::array<ChannelSpec, 2> channelSpecs = {
    ChannelSpec{
      mChannelInfoA,
      gba::reg::TM0CNT_L,
      gba::reg::TM0CNT_H,
      gba::reg::DMA1SAD,
      gba::reg::DMA1DAD,
      gba::reg::DMA1CNT_L,
      gba::reg::DMA1CNT_H,
      gba::address::FIFO_A,
    },
    ChannelSpec{
      mChannelInfoB,
      gba::reg::TM1CNT_L,
      gba::reg::TM1CNT_H,
      gba::reg::DMA2SAD,
      gba::reg::DMA2DAD,
      gba::reg::DMA2CNT_L,
      gba::reg::DMA2CNT_H,
      gba::address::FIFO_B,
    },
  };


  Stop(channel);


  auto& channelSpec = channelSpecs[static_cast<unsigned int>(channel)];

  const auto length = data[0];
  const auto prescaler = data[1];

  channelSpec.channelInfo.dataCount = length;

  channelSpec.TMCNT_L = prescaler;
  channelSpec.DMASAD = &data[2];
  channelSpec.DMADAD = reinterpret_cast<void*>(channelSpec.FIFO);
  channelSpec.DMACNT_L = 0;

  channelSpec.DMACNT_H = gba::DMACNT_H::DESTADDR::FIXED | gba::DMACNT_H::SRCADDR::INC | gba::DMACNT_H::REPEAT | gba::DMACNT_H::TYPE_32BIT | gba::DMACNT_H::SPECIAL | gba::DMACNT_H::IRQ | gba::DMACNT_H::ENABLE;
  channelSpec.TMCNT_H = gba::TMCNT_H::TIMING_1 | gba::TMCNT_H::OPERATE;
}


void SoundManager::Stop(Channel channel) {
  struct ChannelSpec {
    ChannelInfo& channelInfo;
    volatile std::uint16_t& TMCNT_H;
    volatile std::uint16_t& DMACNT_H;
  };


  std::array<ChannelSpec, 2> channelSpecs{
    ChannelSpec{
      mChannelInfoA,
      gba::reg::TM0CNT_H,
      gba::reg::DMA1CNT_H,
    },
    ChannelSpec{
      mChannelInfoB,
      gba::reg::TM1CNT_H,
      gba::reg::DMA2CNT_H,
    },
  };


  auto& channelSpec = channelSpecs[static_cast<unsigned int>(channel)];


  channelSpec.channelInfo.dataCount = 0;
  channelSpec.TMCNT_H = 0;
  channelSpec.DMACNT_H = 0;
}


void SoundManager::Stop() {
  Stop(Channel::A);
  Stop(Channel::B);
}


std::uint_fast16_t SoundManager::IRQ(std::uint_fast16_t flag) {
  auto handleIRQ = [this, flag] (Channel channel) -> std::uint_fast16_t {
    struct ChannelSpec {
      std::uint_fast16_t dmaFlag;
      ChannelInfo& channelInfo;
      volatile std::uint16_t& TMCNT_H;
      volatile std::uint16_t& DMACNT_H;
    };


    std::array<ChannelSpec, 2> channelSpecs{
      ChannelSpec{
        gba::IF::DMA1,
        mChannelInfoA,
        gba::reg::TM0CNT_H,
        gba::reg::DMA1CNT_H,
      },
      ChannelSpec{
        gba::IF::DMA2,
        mChannelInfoB,
        gba::reg::TM1CNT_H,
        gba::reg::DMA2CNT_H,
      },
    };


    auto& channelSpec = channelSpecs[static_cast<unsigned int>(channel)];


    if (!(flag & channelSpec.dmaFlag)) {
      return 0;
    }

    if (!channelSpec.channelInfo.dataCount) {
      return channelSpec.dmaFlag;
    }

    channelSpec.channelInfo.dataCount--;
    if (!channelSpec.channelInfo.dataCount) {
      channelSpec.TMCNT_H = 0;
      channelSpec.DMACNT_H = 0;
    }

    return channelSpec.dmaFlag;
  };

  return handleIRQ(Channel::A) | handleIRQ(Channel::B);
}
