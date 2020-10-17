#include <cstdint>
#include <cstring>

#include <gba.hpp>

#ifndef GBA_USE_CRT
int main();

extern "C" __attribute__((target("arm"))) void _start() {
  main();
  gba::bios::Stop();
}
#endif

extern "C" __attribute__((target("arm"))) void _start_rom() {
  _start();
}

extern "C" __attribute__((target("arm"))) void _start_multiboot() {
  _start();
}

extern "C" __attribute__((target("arm"))) void _start_joybus() {
  _start();
}

extern "C" __attribute__((target("arm"))) void _start_ram() {
  _start();
}
