#pragma once

#include <array>
#include <cstdint>


namespace Tetra {
  // Rotation:
  //   0: initial rotation
  //   1: right rotation of 0
  //   2: half rotation of 0
  //   3: left rotation of 0
  using Rotation = unsigned int;


  struct Offset2D {
    int x;
    int y;


    constexpr Offset2D(int x, int y) noexcept :
      x(x),
      y(y)
    {}

    constexpr Offset2D(int xy = 0) noexcept :
      Offset2D(xy, xy)
    {}


    constexpr Offset2D& operator=(int v) noexcept {
      x = v;
      y = v;
      return *this;
    }

    constexpr Offset2D operator+() const noexcept {
      return *this;
    }

    constexpr Offset2D operator-() const noexcept {
      return Offset2D{
        -x,
        -y,
      };
    }

    constexpr Offset2D& operator+=(int v) noexcept {
      x += v;
      y += v;
      return *this;
    }

    constexpr Offset2D& operator+=(const Offset2D& o) noexcept {
      x += o.x;
      y += o.y;
      return *this;
    }

    constexpr Offset2D& operator-=(int v) noexcept {
      x -= v;
      y -= v;
      return *this;
    }

    constexpr Offset2D& operator-=(const Offset2D& o) noexcept {
      x -= o.x;
      y -= o.y;
      return *this;
    }

    constexpr Offset2D& operator*=(int v) noexcept {
      x *= v;
      y *= v;
      return *this;
    }

    constexpr Offset2D& operator/=(int v) noexcept {
      x /= v;
      y /= v;
      return *this;
    }


    friend constexpr bool operator==(const Offset2D& o1, const Offset2D& o2) noexcept {
      return o1.x == o2.x && o1.y == o2.y;
    }

    friend constexpr bool operator!=(const Offset2D& o1, const Offset2D& o2) noexcept {
      return !(o1 == o2);
    }

    friend constexpr Offset2D operator+(Offset2D o, int v) noexcept {
      return o += v;
    }

    friend constexpr Offset2D operator+(Offset2D o1, const Offset2D& o2) noexcept {
      return o1 += o2;
    }

    friend constexpr Offset2D operator-(Offset2D o, int v) noexcept {
      return o -= v;
    }

    friend constexpr Offset2D operator-(Offset2D o1, const Offset2D& o2) noexcept {
      return o1 -= o2;
    }

    friend constexpr Offset2D operator*(Offset2D o, int v) noexcept {
      return o *= v;
    }

    friend constexpr Offset2D operator/(Offset2D o, int v) noexcept {
      return o /= v;
    }
  };


  struct Point2D {
    int x;
    int y;


    constexpr Point2D(int x, int y) noexcept :
      x(x),
      y(y)
    {}

    constexpr Point2D(int xy) noexcept :
      Point2D(xy, xy)
    {}


    constexpr Point2D& operator=(int v) noexcept {
      x = v;
      y = v;
      return *this;
    }

    constexpr Point2D& operator+=(int v) noexcept {
      x += v;
      y += v;
      return *this;
    }

    constexpr Point2D& operator+=(const Offset2D& o) noexcept {
      x += o.x;
      y += o.y;
      return *this;
    }

    constexpr Point2D& operator-=(int v) noexcept {
      x -= v;
      y -= v;
      return *this;
    }

    constexpr Point2D& operator-=(const Offset2D& o) noexcept {
      x -= o.x;
      y -= o.y;
      return *this;
    }


    friend constexpr bool operator==(const Point2D& p1, const Point2D& p2) noexcept {
      return p1.x == p2.x && p1.y == p2.y;
    }

    friend constexpr bool operator!=(const Point2D& p1, const Point2D& p2) noexcept {
      return !(p1 == p2);
    }

    friend constexpr Point2D operator+(Point2D p, int v) noexcept {
      return p += v;
    }

    friend constexpr Point2D operator+(Point2D p, const Offset2D& o) noexcept {
      return p += o;
    }

    friend constexpr Point2D operator-(Point2D p, int v) noexcept {
      return p -= v;
    }

    friend constexpr Point2D operator-(Point2D p, const Offset2D& o) noexcept {
      return p -= o;
    }

    friend constexpr Offset2D operator-(const Point2D& p1, const Point2D& p2) noexcept {
      return Offset2D{
        p1.x - p2.x,
        p1.y - p2.y,
      };
    }
  };



  enum class MinoType {
    I,
    O,
    S,
    Z,
    J,
    L,
    T,
  };


  enum class TSpin {
    None,         // not a T-Spin
    Zero,         // T-Spin (not mini) without clearing line
    MiniZero,     // T-Spin mini without clearing line
    Single,       // T-Spin (not mini) with clearing one line
    MiniSingle,   // T-Spin mini with clearing one line
    Double,       // T-Spin with clearing two lines
    MiniDouble,   // T-Spin mini with clearing two lines
    Triple,       // T-Spin with clearing three lines
  };


  enum class RotationDirection {
    Right,    // clockwise
    Left,     // counter-clockwise
  };


  enum class BlockType : std::uint8_t {
    None,
    I,
    O,
    S,
    Z,
    J,
    L,
    T,
    Wall,
    Garbage,    // for multiplay
  };


  constexpr unsigned int NumMinoTypes = 7;

  constexpr Rotation NumRotationPatterns = 4;

  constexpr unsigned int NumTSpinTypes = 8;   // including TSpin::None

  constexpr auto MinoTypeToBlockTypeTable = ([]() constexpr {
    std::array<BlockType, NumMinoTypes> minoTypeToBlockTypeTable{
      BlockType::None,    // dummy
      BlockType::None,    // dummy
      BlockType::None,    // dummy
      BlockType::None,    // dummy
      BlockType::None,    // dummy
      BlockType::None,    // dummy
      BlockType::None,    // dummy
    };
    for (unsigned int i = 0; i < NumMinoTypes; i++) {
      minoTypeToBlockTypeTable[i] = ([](MinoType minoType) constexpr {
        switch (minoType) {
          case MinoType::I:
            return BlockType::I;

          case MinoType::O:
            return BlockType::O;

          case MinoType::S:
            return BlockType::S;

          case MinoType::Z:
            return BlockType::Z;

          case MinoType::J:
            return BlockType::J;

          case MinoType::L:
            return BlockType::L;

          case MinoType::T:
            return BlockType::T;
        }
        //assert(false);
      })(static_cast<MinoType>(i));
    }
    return minoTypeToBlockTypeTable;
  })();
}
