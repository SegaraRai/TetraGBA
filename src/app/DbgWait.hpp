#pragma once

#include <gba.hpp>


#ifdef RELEASE_BUILD

# define DbgWait(...)

#else

inline static void DbgWait(int wait) {
  for (int i = 0; i < wait; i++) {
    while (gba::reg::DISPSTAT & gba::DISPSTAT::IS_VBLANK);
    while (!(gba::reg::DISPSTAT & gba::DISPSTAT::IS_VBLANK));
  }
}

#endif
