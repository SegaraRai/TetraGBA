#pragma once

#include <cstdint>
#include <gba.hpp>


class SoundManager {
public:
  enum class Channel {
    A,    // SOUNDA; use Timer 0 and DMA 1
    B,    // SOUNDB; use Timer 1 and DMA 2
  };

private:
  struct ChannelInfo {
    std::uint_fast32_t dataCount;
  };

  ChannelInfo mChannelInfoA;
  ChannelInfo mChannelInfoB;

  SoundManager();

public:
  SoundManager(const SoundManager&) = delete;
  SoundManager(SoundManager&&) = delete;
  SoundManager& operator=(const SoundManager&) = delete;
  SoundManager& operator=(SoundManager&&) = delete;

  static SoundManager& GetInstance();

  bool IsPlaying(Channel channel) const;

  void Play(Channel channel, const std::uint32_t* data);
  void Stop(Channel channel);
  void Stop();

  std::uint_fast16_t IRQ(std::uint_fast16_t flag = gba::reg::IE);
};
