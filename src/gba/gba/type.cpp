#include "type.hpp"

#include <cstdint>


namespace gba {
  inline namespace type {
    volatile OBJAttr& OBJAttr::operator= (const OBJAttr& other) volatile {
      attr0 = other.attr0;
      attr1 = other.attr1;
      attr2 = other.attr2;
      return *this;
    }


    volatile OBJRotScaleAttr& OBJRotScaleAttr::operator= (const OBJRotScaleAttr& other) volatile {
      pa = other.pa;
      pb = other.pb;
      pc = other.pc;
      pd = other.pd;
      return *this;
    }
  }   // inline namespace type
}   // namespace gba
