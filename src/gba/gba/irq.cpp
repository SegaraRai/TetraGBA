#include <cstdint>

#include "register.hpp"
#include "type.hpp"


namespace gba::irq {
  namespace {
    isr gIsr;

    __attribute__((target("arm"))) void armIsr() {
      gIsr();
    }
  }   // namespace

  __attribute__((target("arm"))) void SetISR(isr isr) {
    gIsr = isr;
    gba::reg::ISRAD = armIsr;
  }
}   // namespace gba::irq
