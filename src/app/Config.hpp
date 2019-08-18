#pragma once

#include <cstddef>
#include <cstdint>

#include <image/bg.hpp>
#include <image/bg_background.hpp>
#include <image/obj.hpp>

#include "GameConfig.hpp"
#include "MergePalette.hpp"


namespace Root::Config {
  constexpr auto MergedPaletteData = MergePalette(Tile::bg::PaletteData, Tile::bg_background::PaletteData);

  // 背景画像用でないBGタイルが512タイル以下しか消費していないことを確認する
  // 背景画像用のタイルが0x06004000から配置されるため
  static_assert(Tile::bg::TileData.size() * sizeof(Tile::bg::TileData[0]) <= 0x4000);

  constexpr std::uint16_t CharBase    = 0;
  constexpr std::uint16_t BG3CharBase = 1;

  namespace ScrBase {
    // 0 ~ 7 are reserved for Tile 0
    // 8 ~ 23 are reserved for Tile 1
    // 24 is reserved for BG3

    constexpr std::uint16_t BG0Title  = 25;
    constexpr std::uint16_t BG0Config = 25;
    constexpr std::uint16_t BG3       = 24;
  }   // namespace ScrBase

  namespace Priority {
    inline namespace BG {
      constexpr std::uint16_t BG0 = gba::BGCNT::PRIORITY0;
      constexpr std::uint16_t BG3 = gba::BGCNT::PRIORITY3;
    }   // inline namespace BG
  }   // namespace Priority

  constexpr unsigned int ArrowKeyDelay = 20;
  constexpr unsigned int ArrowKeyInterval = 6;

  namespace Title {
    constexpr unsigned int MenuTextScreenX = 9;
    constexpr unsigned int MenuTextScreenY = 12;
  }   // namespace Title

  namespace Config {
    constexpr unsigned int MenuTextScreenX = 6;
    constexpr unsigned int MenuTextScreenY = 4;
  }   // namespace Title

  constexpr GameConfig DefaultConfig{
    GameConfig::Mode::Line150,
    GameConfig::Music::Tetris99,
    GameConfig::Background::Gray,
    1,
    false,
    false,
  };
}   // namespace GameTetra::Config
