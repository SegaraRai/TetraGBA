#pragma once

#include <cstdint>


class MusicManager {
  bool mPlaying;
  unsigned int mDivisor;
  unsigned int mTickPerDiv;
  unsigned int mCurrentTick;
  unsigned int mNextTick;
  const std::uint16_t* mFrequencyTable;
  const std::uint16_t* mEnvelopeTable;
  const std::uint16_t* mChannel3ValueTable;
  const std::uint8_t* mPtrCurrent;
  const std::uint8_t* mPtrEnd;
  const std::uint8_t* mPtrLoopPoint;

  std::uint_fast8_t Read8();
  std::uint_fast16_t Read16LE();

  MusicManager();

public:
  MusicManager(const MusicManager&) = delete;
  MusicManager(MusicManager&&) = delete;
  MusicManager& operator=(const MusicManager&) = delete;
  MusicManager& operator=(MusicManager&&) = delete;

  static MusicManager& GetInstance();

  void Play(const std::uint32_t* data);
  void Stop();
  void Resume();

  void Step();
};
