#include "KonamiCommandSignal.hpp"
#include "../DbgPrintf.hpp"

#include <cstdint>

#include <gba.hpp>


KonamiCommandSignal::KonamiCommandSignal() :
  mCommandIndex(InitialIndex)
{}


bool KonamiCommandSignal::StepImpl() {
  const auto key = (~gba::reg::KEYINPUT) & ((1 << 10) - 1);

  if (key == KonamiCommand[mCommandIndex]) {
    mCommandIndex++;
    //DbgPrintf("KONAMI Command %d\n", mCommandIndex);
  } else if (key != KonamiCommand[mCommandIndex - 1]) {
    mCommandIndex = InitialIndex;
  }

  if (mCommandIndex == KonamiCommand.size()) {
    DbgPrintf("KONAMI Command!\n");
    mCommandIndex = InitialIndex;
    return true;
  }

  return false;
}
