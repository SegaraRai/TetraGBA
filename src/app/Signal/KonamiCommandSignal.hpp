#pragma once

#include "SignalBase.hpp"

#include <gba.hpp>

#include <array>
#include <cstdint>
#include <memory>


class KonamiCommandSignal : public SignalBase {
  static constexpr unsigned int InitialIndex = 1;

  static constexpr std::array<std::uint16_t, 22> KonamiCommand{
    0,
    0, gba::KEYINPUT::UP,
    0, gba::KEYINPUT::UP,
    0, gba::KEYINPUT::DOWN,
    0, gba::KEYINPUT::DOWN,
    0, gba::KEYINPUT::LEFT,
    0, gba::KEYINPUT::RIGHT,
    0, gba::KEYINPUT::LEFT,
    0, gba::KEYINPUT::RIGHT,
    0, gba::KEYINPUT::B,
    0, gba::KEYINPUT::A,
    0,
  };

  unsigned int mCommandIndex;

protected:
  bool StepImpl() override;

public:
  KonamiCommandSignal();
};
