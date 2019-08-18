#ifndef _gba_lcd_hpp_
#define _gba_lcd_hpp_

namespace gba::lcd {
  constexpr unsigned int DotWidth          = 240;   // 240 dots / horizontal line
  constexpr unsigned int DotHeight         = 160;   // 160 dots / vertical line
  constexpr unsigned int TileRealWidth     =  30;   // Real width in tile
  constexpr unsigned int TileRealHeight    =  20;   // Real height in tile
  constexpr unsigned int TileVirtualWidth  =  32;   // Virtual width in tile
  constexpr unsigned int TileVirtualHeight =  32;   // Virtual height in tile
}   // namespace gba::lcd

#endif
