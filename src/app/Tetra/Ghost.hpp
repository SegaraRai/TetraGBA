#pragma once

#include <cstddef>
#include <cstdint>

#include "Tetra/Mino.hpp"
#include <image/bg.hpp>


namespace GameTetra::Ghost {
  constexpr std::uint16_t FirstTileIndex  = Tile::bg::Ghost::FirstTileIndex;
  constexpr std::uint16_t FirstTileIndexO = FirstTileIndex + 16;
  constexpr std::uint16_t HasLeft         = 1 << 0;
  constexpr std::uint16_t HasUp           = 1 << 1;
  constexpr std::uint16_t HasRight        = 1 << 2;
  constexpr std::uint16_t HasDown         = 1 << 3;

  struct CellTile {
    int x;
    int y;
    std::uint_fast16_t tile;
  };
  using CellTiles = std::array<CellTile, Tetra::NumMinoCells>;
  using RotatedMinoTile = std::array<CellTiles, Tetra::NumRotationPatterns>;

  constexpr auto GhostMinoTile = ([]() constexpr {
    std::array<RotatedMinoTile, Tetra::NumMinoTypes> ghostMinoTile{};

    for (std::size_t minoIndex = 0; minoIndex < Tetra::NumMinoTypes; minoIndex++) {
      for (std::size_t rotation = 0; rotation < Tetra::NumRotationPatterns; rotation++) {
        const auto points = Tetra::Mino[minoIndex].minos[rotation].points;

        auto hasPoint = [&points](int x, int y) constexpr {
          for (const auto& point : points) {
            if (point.x == x && point.y == y) {
              return true;
            }
          }
          return false;
        };

        CellTiles cellTiles{};

        for (std::size_t i = 0; i < points.size(); i++) {
          const auto& point = points[i];
          const auto x = point.x;
          const auto y = point.y;

          std::uint16_t tileIndex = 0;

          //*
          if (hasPoint(x - 1, y)) {
            tileIndex |= HasLeft;
          }
          if (hasPoint(x, y - 1)) {
            tileIndex |= HasUp;
          }
          if (hasPoint(x + 1, y)) {
            tileIndex |= HasRight;
          }
          if (hasPoint(x, y + 1)) {
            tileIndex |= HasDown;
          }
          //*/

          tileIndex += minoIndex == static_cast<unsigned int>(Tetra::MinoType::O) ? FirstTileIndexO : FirstTileIndex;

          cellTiles[i] = CellTile{
            x,
            y,
            static_cast<std::uint_fast16_t>(gba::BGMAP::TEXT::TILE(tileIndex) | gba::BGMAP::TEXT::PALETTE(minoIndex + 1)),
          };
        }

        ghostMinoTile[minoIndex][rotation] = cellTiles;
      }
    }

    return ghostMinoTile;
  })();
}   // namespace GameTetra::Ghost
