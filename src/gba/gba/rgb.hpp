#ifndef _gba_rgb_hpp_
#define _gba_rgb_hpp_

#include <cassert>
#include <cstdint>


namespace gba {
  inline namespace rgb {
    constexpr std::uint16_t CalcRGB(unsigned int r, unsigned int g, unsigned int b) {
      assert(r < 32);
      assert(g < 32);
      assert(b < 32);
      return (b << 10) | (g << 5) | r;
    }

    template<unsigned int R, unsigned int G, unsigned int B>
    constexpr std::uint16_t RGB = CalcRGB(R, G, B);
  }
}

#endif
