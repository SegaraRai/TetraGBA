#pragma once

#include <cstdio>


#ifdef RELEASE_BUILD

# define DbgPrintf(...)

#else

template<typename... Args>
void DbgPrintf(Args... args) {
  static char buffer[128];
  std::snprintf(buffer, sizeof(buffer) / sizeof(char), args...);
  asm volatile("mov r2, %0; ldr r0, =0xc0ded00d; and r0, r0" : : "r"(buffer) : "r2", "r0");
}

#endif
