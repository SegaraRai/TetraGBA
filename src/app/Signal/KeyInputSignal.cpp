#include "KeyInputSignal.hpp"

#include <cstdint>

#include <gba.hpp>


KeyInputSignal::KeyInputSignal(std::uint16_t keyMask) :
  mKeyMask(keyMask)
{}


bool KeyInputSignal::StepImpl() {
  return (gba::reg::KEYINPUT & mKeyMask) != mKeyMask;
}
