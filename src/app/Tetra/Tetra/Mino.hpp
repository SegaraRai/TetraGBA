#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>

#include "Common.hpp"


namespace Tetra {
  constexpr unsigned int NumWallKickPatterns = 5;
  constexpr unsigned int NumMinoCells = 4;
  constexpr unsigned int MaxMinoSize = 4;


  using WallKickOffsets = std::array<Offset2D, NumWallKickPatterns>;

  struct RotatedMinos {
    struct Mino {
      unsigned int width = 0;
      unsigned int height = 0;
      Offset2D minPoint{};
      Offset2D maxPoint{};
      std::uint_fast16_t bitPattern = 0;
      std::array<Offset2D, NumMinoCells> points{};
      WallKickOffsets wallKickOffsetsRight{};
      WallKickOffsets wallKickOffsetsLeft{};
    };

    unsigned int size = 0;
    std::array<Mino, NumRotationPatterns> minos{};
  };


  namespace InternalImpl {
    constexpr unsigned int FlipFlagX = 1 << 0;
    constexpr unsigned int FlipFlagY = 1 << 1;

    constexpr WallKickOffsets WallKickOffsetsBasic{
      // Y-axis flipped
      Offset2D{ 0,  0},
      Offset2D{-1,  0},
      Offset2D{-1, -1},
      Offset2D{ 0,  2},
      Offset2D{-1,  2},
    };

    constexpr WallKickOffsets WallKickOffsetsIMino{
      // Y-axis flipped
      Offset2D{ 0,  0},
      Offset2D{-2,  0},
      Offset2D{ 1,  0},
      Offset2D{-2,  1},
      Offset2D{ 1, -2},
    };


    constexpr Offset2D RotateOffset2D(const Offset2D& point, int rotOriginDoubled, Rotation rotation) {
      constexpr std::array<std::array<int, 4>, NumRotationPatterns> RotMatrix{
        // 2D rotation matrix:
        //   /   cos(theta)  -sin(theta)  |
        //   |   sin(theta)   cos(theta)  /

        // theta = (2 * PI / 4) * 0 = 0
        // sin(theta) = 0, cos(theta) = 1
        std::array<int, 4>({
           1,  0,
           0,  1,
        }),

        // theta = (2 * PI / 4) * 1 = (1 / 2) * PI
        // sin(theta) = 1, cos(theta) = 0
        std::array<int, 4>({
           0, -1,
           1,  0,
        }),

        // theta = (2 * PI / 4) * 2 = PI
        // sin(theta) = 0, cos(theta) = -1
        std::array<int, 4>({
          -1,  0,
           0, -1,
        }),

        // theta = (2 * PI / 4) * 3 = (3 / 2) * PI
        // sin(theta) = -1, cos(theta) = 0
        std::array<int, 4>({
           0,  1,
          -1,  0,
        }),
      };


      const auto pointDoubled = point * 2;
      const auto pointDoubledMoved = pointDoubled - rotOriginDoubled;
      const Offset2D rotatedPointDoubledMoved{
        RotMatrix[rotation][0] * pointDoubledMoved.x + RotMatrix[rotation][1] * pointDoubledMoved.y,
        RotMatrix[rotation][2] * pointDoubledMoved.x + RotMatrix[rotation][3] * pointDoubledMoved.y,
      };
      const Offset2D rotatedPointDoubled = rotatedPointDoubledMoved + rotOriginDoubled;
      const auto rotatedPoint = rotatedPointDoubled / 2;

      assert(rotatedPointDoubled.x % 2 == 0);
      assert(rotatedPointDoubled.y % 2 == 0);

      return rotatedPoint;
    }


    constexpr WallKickOffsets FlipWallKickOffsets(WallKickOffsets wallKickOffsets, unsigned int flipFlags) {
      constexpr std::array<int, 1 << 2> Kx = {1, -1,  1, -1};
      constexpr std::array<int, 1 << 2> Ky = {1,  1, -1, -1};

      const int kx = Kx[flipFlags];
      const int ky = Ky[flipFlags];
      for (auto& offset : wallKickOffsets) {
        offset.x *= kx;
        offset.y *= ky;
      }
      return wallKickOffsets;
    }


    constexpr RotatedMinos CreateRotatedMinos(const std::array<int, 16>& minos, const WallKickOffsets& baseWallKickOffsets) {
      /*
      1 0 0 0   0 1 1 0   0 0 0 0   0 1 0 0
      1 1 1 0   0 1 0 0   1 1 1 0   0 1 0 0
      0 0 0 0   0 1 0 0   0 0 1 0   1 1 0 0
      0 0 0 0   0 0 0 0   0 0 0 0   0 0 0 0

      =>
         0b1000'1110'0000'0000,
         0b0110'0100'0100'0000,
         0b0000'1110'0010'0000,
         0b0100'0100'1100'0000,
      */

      // right (clockwise) rotation
      constexpr std::array<unsigned int, NumRotationPatterns> WallKickOffsetsFlipFlagsRight = {
        0,                        // 0 -> 1
        FlipFlagX | FlipFlagY,    // 1 -> 2
        FlipFlagX,                // 2 -> 3
        FlipFlagY,                // 3 -> 0
      };

      // left (counter-clockwise) rotation
      constexpr std::array<unsigned int, NumRotationPatterns> WallKickOffsetsFlipFlagsLeft = {
        FlipFlagX,                // 0 -> 3; WallKickOffsetsFlipFlagsRight[2]
        FlipFlagX | FlipFlagY,    // 1 -> 0; WallKickOffsetsFlipFlagsRight[1]
        0,                        // 2 -> 1; WallKickOffsetsFlipFlagsRight[0]
        FlipFlagY,                // 3 -> 2; WallKickOffsetsFlipFlagsRight[3]
      };


      unsigned int size = 0;
      {
        unsigned int minX = MaxMinoSize - 1;
        unsigned int maxX = 0;
        unsigned int minY = MaxMinoSize - 1;
        unsigned int maxY = 0;
        for (unsigned int i = 0; i < MaxMinoSize * MaxMinoSize; i++) {
          if (!minos[i]) {
            continue;
          }
          const unsigned int x = i % MaxMinoSize;
          const unsigned int y = i / MaxMinoSize;
          minX = std::min(minX, x);
          maxX = std::max(maxX, x);
          minY = std::min(minY, y);
          maxY = std::max(maxY, y);
        }

        const unsigned int width = maxX - minX + 1;
        const unsigned int height = maxY - minY + 1;

        size = std::max(width, height);
      }

      const int rotOriginDoubled = size - 1;    // (0 + size * 2 - 2) / 2

      std::array<RotatedMinos::Mino, NumRotationPatterns> rotatedMinos{};
      for (Rotation i = 0; i < NumRotationPatterns; i++) {
        unsigned int minX = MaxMinoSize - 1;
        unsigned int maxX = 0;
        unsigned int minY = MaxMinoSize - 1;
        unsigned int maxY = 0;
        std::uint_fast16_t bitPattern = 0;
        std::array<Offset2D, NumMinoCells> points{};
        for (unsigned int j = 0, k = 0; j < MaxMinoSize * MaxMinoSize; j++) {
          if (!minos[j]) {
            continue;
          }
          const Offset2D point{
            static_cast<int>(j % MaxMinoSize),
            static_cast<int>(j / MaxMinoSize),
          };
          const auto rotatedPoint = RotateOffset2D(point, rotOriginDoubled, i);
          //*
          assert(rotatedPoint.x >= 0);
          assert(rotatedPoint.y >= 0);
          assert(rotatedPoint.x < static_cast<int>(size));
          assert(rotatedPoint.y < static_cast<int>(size));
          assert(k < points.size());
          //*/
          minX = std::min<unsigned int>(minX, rotatedPoint.x);
          maxX = std::max<unsigned int>(maxX, rotatedPoint.x);
          minY = std::min<unsigned int>(minY, rotatedPoint.y);
          maxY = std::max<unsigned int>(maxY, rotatedPoint.y);
          bitPattern |= 1 << (MaxMinoSize * MaxMinoSize - (rotatedPoint.y * MaxMinoSize + rotatedPoint.x) - 1);
          points[k] = rotatedPoint;
          k++;
        }

        const unsigned int width = maxX - minX + 1;
        const unsigned int height = maxY - minY + 1;

        rotatedMinos[i] = RotatedMinos::Mino{
          width,
          height,
          Offset2D{
            static_cast<int>(minX),
            static_cast<int>(minY),
          },
          Offset2D{
            static_cast<int>(maxX),
            static_cast<int>(maxY),
          },
          bitPattern,
          points,
          FlipWallKickOffsets(baseWallKickOffsets, WallKickOffsetsFlipFlagsRight[i]),
          FlipWallKickOffsets(baseWallKickOffsets, WallKickOffsetsFlipFlagsLeft[i]),
        };
      }

      return RotatedMinos{
        size,
        rotatedMinos,
      };
    }
  }



  constexpr std::array<RotatedMinos, NumMinoTypes> Mino{
    // I
    InternalImpl::CreateRotatedMinos({
      0, 0, 0, 0,
      1, 1, 1, 1,
      0, 0, 0, 0,
      0, 0, 0, 0,
    }, InternalImpl::WallKickOffsetsIMino),
    // O
    InternalImpl::CreateRotatedMinos({
      1, 1, 0, 0,
      1, 1, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
    }, InternalImpl::WallKickOffsetsBasic),
    // S
    InternalImpl::CreateRotatedMinos({
      0, 1, 1, 0,
      1, 1, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
    }, InternalImpl::WallKickOffsetsBasic),
    // Z
    InternalImpl::CreateRotatedMinos({
      1, 1, 0, 0,
      0, 1, 1, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
    }, InternalImpl::WallKickOffsetsBasic),
    // J
    InternalImpl::CreateRotatedMinos({
      1, 0, 0, 0,
      1, 1, 1, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
    }, InternalImpl::WallKickOffsetsBasic),
    // L
    InternalImpl::CreateRotatedMinos({
      0, 0, 1, 0,
      1, 1, 1, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
    }, InternalImpl::WallKickOffsetsBasic),
    // T
    InternalImpl::CreateRotatedMinos({
      0, 1, 0, 0,
      1, 1, 1, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
    }, InternalImpl::WallKickOffsetsBasic),
  };
}
