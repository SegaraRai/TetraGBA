#include <cstdint>

#include <gba.hpp>


#ifndef GBA_USE_CRT

int main();

extern "C" __attribute__((target("arm"))) void _start() {
  main();

  gba::bios::Stop();
}

#endif
