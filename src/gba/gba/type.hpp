#ifndef _gba_type_hpp_
#define _gba_type_hpp_

#include <cstdint>


namespace gba {
  inline namespace type {
    using isr = void(*)();


    // LCD OBJ - OAM Attributes
    // https://problemkaputt.de/gbatek.htm#lcdobjoamattributes
    struct OBJAttr {
      std::uint16_t attr0;    // OBJ Attribute 0
      std::uint16_t attr1;    // OBJ Attribute 1
      std::uint16_t attr2;    // OBJ Attribute 2
      std::uint16_t unused;   // For Rotation/Scaling

      volatile OBJAttr& operator= (const OBJAttr& other) volatile;
    } __attribute__((__packed__));

    static_assert(sizeof(OBJAttr) == 8);


    // LCD OBJ - OAM Rotation/Scaling Parameters
    // https://problemkaputt.de/gbatek.htm#lcdobjoamrotationscalingparameters
    struct OBJRotScaleAttr {
      std::uint16_t unused0[3];
      std::int16_t pa;
      std::uint16_t unused1[3];
      std::int16_t pb;
      std::uint16_t unused2[3];
      std::int16_t pc;
      std::uint16_t unused3[3];
      std::int16_t pd;

      volatile OBJRotScaleAttr& operator= (const OBJRotScaleAttr& other) volatile;
    } __attribute__((__packed__));

    static_assert(sizeof(OBJRotScaleAttr) == 32);
  }   // inline namespace type
}   // namespace gba

#endif
