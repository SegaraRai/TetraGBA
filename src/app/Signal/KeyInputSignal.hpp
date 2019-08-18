#pragma once

#include <cstdint>

#include "SignalBase.hpp"


class KeyInputSignal : public SignalBase {
  std::uint16_t mKeyMask;

protected:
  bool StepImpl() override;

public:
  KeyInputSignal(std::uint16_t keyMask);
};
