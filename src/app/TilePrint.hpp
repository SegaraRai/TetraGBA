#pragma once

#include <gba.hpp>
#include <image/bg.hpp>


template<unsigned int ScreenBaseBlock, unsigned int MapWidth = 32>
void TilePrint(const char* string, unsigned int x, unsigned int y) {
  unsigned int currentX = x;
  unsigned int currentY = y;

  auto ptr = string;
  while (*ptr != '\0') {
    if (*ptr == '\n') {
      currentX = x;
      currentY++;
    } else {
      //
      gba::pointer_memory::VRAM_BGMAP<ScreenBaseBlock>[currentY * MapWidth + currentX] = gba::BGMAP::TEXT::PALETTE(Tile::bg::Font::Palette) | gba::BGMAP::TEXT::TILE(Tile::bg::Font::FirstTileIndex + static_cast<unsigned char>(*ptr));
      currentX++;
    }
    ptr++;
  }
}
