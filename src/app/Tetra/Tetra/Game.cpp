#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>

#include "Game.hpp"
#include "../../DbgPrintf.hpp"



namespace Tetra {
  namespace Game {
    namespace {
      constexpr std::array<Rotation, NumRotationPatterns> CalcNextRotationTable(unsigned int d) {
        std::array<Rotation, NumRotationPatterns> nextRotationTable{};
        for (Rotation rotation = 0; rotation < NumRotationPatterns; rotation++) {
          nextRotationTable[rotation] = (rotation + d) % NumRotationPatterns;
        }
        return nextRotationTable;
      }

      constexpr auto NextRotationTableRight = CalcNextRotationTable(1);
      constexpr auto NextRotationTableLeft = CalcNextRotationTable(NumRotationPatterns - 1);
    }


    bool Game::Collide(std::size_t boardWidth, std::size_t boardHeight, const BlockType* blocks, MinoType minoType, const Point2D& position, Rotation rotation) {
      const auto minoIndex = static_cast<std::size_t>(minoType);
      const auto& minoInfo = Mino[minoIndex].minos[rotation];
      if (const auto minPoint = position + minoInfo.minPoint; minPoint.x < 0 || minPoint.y < 0) {
        return true;
      }
      if (const auto maxPoint = position + minoInfo.maxPoint; maxPoint.x >= static_cast<int>(boardWidth) || maxPoint.y >= static_cast<int>(boardHeight)) {
        return true;
      }
      for (const auto& point : minoInfo.points) {
        const auto absolutePointPosition = position + point;
        if (blocks[absolutePointPosition.y * boardWidth + absolutePointPosition.x] != BlockType::None) {
          return true;
        }
      }
      return false;
    }



    Game::Game(const InitializeInfo& initializeInfo) :
      mInitialPositions(CalcInitialPositions(initializeInfo.boardWidth, initializeInfo.baseY)),
      mBlocks(std::make_unique<BlockType[]>(initializeInfo.boardWidth * initializeInfo.boardHeight)),
      mBlocksBeforeClear(std::make_unique<BlockType[]>(initializeInfo.boardWidth* initializeInfo.boardHeight)),
      mBlocksAfterClear(std::make_unique<BlockType[]>(initializeInfo.boardWidth* initializeInfo.boardHeight)),
      mNextMinos{},
      mBoardInfo{
        false,
        initializeInfo.boardWidth,
        initializeInfo.boardHeight,
        initializeInfo.baseY,
        mBlocks.get(),
        0,
        mNextMinos,
        std::nullopt,
        false,
        false,
        false,
        0,
        MinoType::I,      // dummy; will be updated later [1]
        Point2D{0, 0},    // dummy; will be updated later [1]
        0,                // dummy; will be updated later [1]
        Point2D{0, 0},    // dummy; will be updated later [1]
      },
      mGameStatistics{},
      mLastLineClearInfo{},
      mRenCount(0),
      mRenLineCount(0),
      mBackToBackCount(0),
      mLastOperationRotation(false),
      mLastRotationWallKickOffsetIndex(0),
      mMinoFactory(initializeInfo.minoFactory)
    {
      static_assert(static_cast<unsigned int>(BlockType::None) == 0);
      std::memset(mBlocks.get(), 0, mBoardInfo.boardWidth * mBoardInfo.boardHeight * sizeof(BlockType));
      for (unsigned int y = 0; y < mBoardInfo.boardHeight; y++) {
        GetBlockRef(Point2D{0, static_cast<int>(y)}) = BlockType::Wall;
        GetBlockRef(Point2D{static_cast<int>(mBoardInfo.boardWidth - 1), static_cast<int>(y)}) = BlockType::Wall;
      }
      for (unsigned int x = 0; x < mBoardInfo.boardWidth; x++) {
        GetBlockRef(Point2D{static_cast<int>(x), static_cast<int>(mBoardInfo.boardHeight - 1)}) = BlockType::Wall;
      }

      for (std::size_t i = 0; i < initializeInfo.numNexts; i++) {
        mNextMinos.push_back(mMinoFactory(*this));
      }

      // [1]
      ConsumeNextMino();
      InitializeNextMino();
    }

    BlockType& Game::GetBlockRef(const Point2D& position) {
      return mBlocks[position.y * mBoardInfo.boardWidth + position.x];
    }


    BlockType Game::GetBlock(const Point2D& position) const {
      return mBlocks[position.y * mBoardInfo.boardWidth + position.x];
    }


    bool Game::Collide(MinoType minoType, const Point2D& position, Rotation rotation) const {
      return Collide(mBoardInfo.boardWidth, mBoardInfo.boardHeight, mBoardInfo.blocks, minoType, position, rotation);
    }


    bool Game::Collide(const Point2D& position, Rotation rotation) const {
      return Collide(mBoardInfo.currentMino, position, rotation);
    }


    void Game::DispatchBoardUpdateEvent() {
      Event::BoardUpdate::DispatchEvent(mBoardInfo);
    }


    void Game::DispatchStatisticsUpdateEvent() {
      Event::StatisticsUpdate::DispatchEvent(mGameStatistics);
    }


    bool Game::IsLanded(const Point2D& position, Rotation rotation) const {
      assert(!Collide(position, rotation));
      return Collide(position + Offset2D{0, 1}, rotation);
    }


    bool Game::IsLanded() const {
      return IsLanded(mBoardInfo.currentPosition, mBoardInfo.currentRotation);
    }


    void Game::GameOver() {
      assert(!mBoardInfo.gameOver);
      mBoardInfo.gameOver = true;

      // update statistics
      if (mBackToBackCount) {
        mBackToBackCount--;
      }
      mGameStatistics.numTotalBackToBacks += mBackToBackCount;
      mGameStatistics.numMaxBackToBacks = std::max(mGameStatistics.numMaxBackToBacks, mBackToBackCount);

      mBackToBackCount = 0;

      if (mRenCount) {
        mRenCount--;
      }
      mGameStatistics.numTotalRens += mRenCount;
      mGameStatistics.numMaxRens = std::max<GameStatistics::Count>(mGameStatistics.numMaxRens, mRenCount);

      mGameStatistics.numMaxRenLines = std::max<GameStatistics::Count>(mGameStatistics.numMaxRenLines, mRenLineCount);

      Event::GameOver::DispatchEvent();
      DispatchBoardUpdateEvent();
      DispatchStatisticsUpdateEvent();
    }


    void Game::ConsumeNextMino() {
      if (mNextMinos.size()) {
        mBoardInfo.currentMino = mNextMinos[0];
        mNextMinos.pop_front();
        mNextMinos.push_back(mMinoFactory(*this));
      } else {
        mBoardInfo.currentMino = mMinoFactory(*this);
      }

      mGameStatistics.numMinos++;

      Event::NextMino::DispatchEvent(mNextMinos);
    }


    void Game::UpdatePosition() {
      // update ghost position
      // bit inefficient
      Point2D ghostPosition = mBoardInfo.currentPosition;
      do {
        ghostPosition.y++;
      } while (!Collide(ghostPosition, mBoardInfo.currentRotation));
      ghostPosition.y--;
      mBoardInfo.ghostPosition = ghostPosition;

      // set land information
      const auto landing = ghostPosition == mBoardInfo.currentPosition;
      const auto prevLanding = mBoardInfo.landing;

      mBoardInfo.landing = landing;
      if (landing) {
        if (!prevLanding) {
          Event::Land::DispatchEvent(!mBoardInfo.onceLanded);
        }
        mBoardInfo.onceLanded = true;
      }
    }


    void Game::InitializeNextMino() {
      mLastOperationRotation = false;
      mBoardInfo.holdUsed = false;
      mBoardInfo.landing = false;
      mBoardInfo.onceLanded = false;
      mBoardInfo.numOperationsAfterLand = 0;
      mBoardInfo.currentPosition = mInitialPositions[static_cast<std::size_t>(mBoardInfo.currentMino)];
      for (unsigned int i = 0; i < 2; i++) {
        if (!Collide(mBoardInfo.currentPosition, mBoardInfo.currentRotation)) {
          break;
        }
        mBoardInfo.currentPosition.y--;
      }
      mBoardInfo.currentRotation = 0;
      UpdatePosition();
      Event::NewMino::DispatchEvent(mBoardInfo.currentMino);
      if (Collide(mBoardInfo.currentPosition, mBoardInfo.currentRotation)) {
        GameOver();
        return;
      }
    }


    bool Game::Move(const Offset2D& offset, bool hardDrop) {
      if (offset == Offset2D{}) {
        assert(hardDrop);
        return true;
      }

      assert(offset.y >= 0);
      assert(-1 <= offset.x && offset.x <= 1);

      const auto newPosition = mBoardInfo.currentPosition + offset;

      if (Collide(newPosition, mBoardInfo.currentRotation)) {
        return false;
      }

      mLastOperationRotation = false;

      mBoardInfo.currentPosition = newPosition;
      UpdatePosition();

      if (offset.x && mBoardInfo.onceLanded && mBoardInfo.landing) {
        mBoardInfo.numOperationsAfterLand++;
      }

      if (hardDrop) {
        mGameStatistics.numHardDrops++;
      }
      if (offset.x > 0) {
        mGameStatistics.numRightMoves += offset.x;
      } else {
        mGameStatistics.numLeftMoves += -offset.x;
      }
      mGameStatistics.numDropDistance += offset.y;

      DispatchBoardUpdateEvent();
      DispatchStatisticsUpdateEvent();

      return true;
    }


    bool Game::Rotate(RotationDirection rotationDirection) {
      assert(rotationDirection == RotationDirection::Right || rotationDirection == RotationDirection::Left);

      const Rotation newRotation = (rotationDirection == RotationDirection::Right ? NextRotationTableRight : NextRotationTableLeft)[mBoardInfo.currentRotation];

      const auto minoIndex = static_cast<std::size_t>(mBoardInfo.currentMino);
      const auto& currentMinoInfo = Mino[minoIndex].minos[mBoardInfo.currentRotation];

      const auto& wallKickOffsets = rotationDirection == RotationDirection::Right ? currentMinoInfo.wallKickOffsetsRight : currentMinoInfo.wallKickOffsetsLeft;
      for (unsigned int wallKickOffsetIndex = 0; wallKickOffsetIndex < wallKickOffsets.size(); wallKickOffsetIndex++) {
        const auto& wallKickOffset = wallKickOffsets[wallKickOffsetIndex];

        const auto newPosition = mBoardInfo.currentPosition + wallKickOffset;

        //DbgPrintf("Wallkick: Mino = %d, Index = %d (%d, %d, R %d) -> (%d, %d, R %d) [%d, %d]\n", static_cast<int>(mBoardInfo.currentMino), wallKickOffsetIndex, mBoardInfo.currentPosition.x, mBoardInfo.currentPosition.y, mBoardInfo.currentRotation, newPosition.x, newPosition.y, newRotation, wallKickOffset.x, wallKickOffset.y);

        if (Collide(mBoardInfo.boardWidth, mBoardInfo.boardHeight, mBoardInfo.blocks, mBoardInfo.currentMino, newPosition, newRotation)) {
          continue;
        }

        //DbgPrintf("Wallkick ok\n");

        //DbgPrintf("Wallkick: Mino = %d, Index = %d (%d, %d, R %d) -> (%d, %d, R %d) [%d, %d]\n", static_cast<int>(mBoardInfo.currentMino), wallKickOffsetIndex, mBoardInfo.currentPosition.x, mBoardInfo.currentPosition.y, mBoardInfo.currentRotation, newPosition.x, newPosition.y, newRotation, wallKickOffset.x, wallKickOffset.y);

        mBoardInfo.currentRotation = newRotation;
        mBoardInfo.currentPosition = newPosition;
        UpdatePosition();
        if (mBoardInfo.onceLanded && mBoardInfo.landing) {
          mBoardInfo.numOperationsAfterLand++;
        }

        if (rotationDirection == RotationDirection::Right) {
          mGameStatistics.numRightRotations++;
        } else {
          mGameStatistics.numLeftRotations++;
        }

        DispatchBoardUpdateEvent();
        DispatchStatisticsUpdateEvent();

        mLastOperationRotation = true;
        mLastRotationWallKickOffsetIndex = wallKickOffsetIndex;

        return true;
      }

      return false;
    }


    const BoardInfo& Game::GetBoardInfo() const {
      return mBoardInfo;
    }


    const GameStatistics& Game::GetGameStatistics() const {
      return mGameStatistics;
    }


    void Game::Lock() {
      if (mBoardInfo.gameOver) {
        return;
      }

      if (!mBoardInfo.onceLanded || !mBoardInfo.landing) {
        // cannot lock if not landed
        assert(false);
        return;
      }

      assert(!Collide(mBoardInfo.currentPosition, mBoardInfo.currentRotation));

      // mobile (can block move up) ?
      const auto mobile = !Collide(mBoardInfo.currentPosition + Offset2D{0, -1}, mBoardInfo.currentRotation);

      Event::Lock::DispatchEvent();

      mBoardInfo.blockCount += NumMinoCells;

      const auto minoIndex = static_cast<std::size_t>(mBoardInfo.currentMino);
      const auto& minoInfo = Mino[minoIndex].minos[mBoardInfo.currentRotation];

      for (const auto& relativePointPosition : minoInfo.points) {
        const auto pointPosition = mBoardInfo.currentPosition + relativePointPosition;
        GetBlockRef(pointPosition) = MinoTypeToBlockTypeTable[minoIndex];
      }

      unsigned int clearedLines[4] = {};
      unsigned int numClearedLines = 0;
      for (unsigned int minoY = 0; minoY < minoInfo.height; minoY++) {
        const int y = mBoardInfo.currentPosition.y + minoInfo.minPoint.y + minoY;
        assert(y >= 0);

        bool filled = true;
        for (unsigned int x = 1; x < mBoardInfo.boardWidth - 1; x++) {
          if (GetBlock(Point2D{static_cast<int>(x), static_cast<int>(y)}) == BlockType::None) {
            filled = false;
            break;
          }
        }

        if (!filled) {
          continue;
        }

        clearedLines[numClearedLines] = y;
        numClearedLines++;
      }

      assert(mBoardInfo.blockCount >= (mBoardInfo.boardWidth - 2) * numClearedLines);
      mBoardInfo.blockCount -= (mBoardInfo.boardWidth - 2) * numClearedLines;

      // check T-Spin

      TSpin tSpin = TSpin::None;
      if (mBoardInfo.currentMino == MinoType::T && mLastOperationRotation) {
        constexpr unsigned int MinCornerBlocksForTSpin = 3;

        // T-Spin recognition with 3-corner T
        // points around T
        //
        // X T X
        // T T T
        // X * X    point of X in the left figure
        //
        // 0 T 1
        // T T T
        // 2 * 3
        constexpr std::array<Offset2D, 4> TCornerBlockOffsets = {
          Offset2D{0, 0},
          Offset2D{0, 2},
          Offset2D{2, 0},
          Offset2D{2, 2},
        };

        // point behind T for each rotations
        // also used for TOJ T-Spin Mini recognition
        //
        // * T *
        // T T T
        //   X     point of X in the left figure
        //
        // * T *   * T *   * 2 *   * T *
        // T T T   1 T T   T T T   T T 3
        // * 0 *   * T *   * T *   * T *
        constexpr std::array<Offset2D, NumRotationPatterns> TBehindBlockOffsets = {
          Offset2D{1, 2},
          Offset2D{0, 1},
          Offset2D{1, 0},
          Offset2D{2, 1},
        };

        // point side of T for each rotations
        // used for T-Spin Mini recognition
        //
        // X T X
        // T T T    point of X in the left figure
        //
        // [0]     [1]     [2]     [3]
        // 0 T 1   * T 0   * * *   0 T *
        // T T T   * T T   T T T   T T *
        // * * *   * T 1   0 T 1   1 T *
        constexpr std::array<std::array<Offset2D, 2>, NumRotationPatterns> TSideBlockOffsets = {
          std::array<Offset2D, 2>{Offset2D{0, 0}, Offset2D{2, 0}},
          std::array<Offset2D, 2>{Offset2D{2, 0}, Offset2D{2, 2}},
          std::array<Offset2D, 2>{Offset2D{0, 2}, Offset2D{2, 2}},
          std::array<Offset2D, 2>{Offset2D{0, 0}, Offset2D{0, 2}},
        };

        unsigned int cornerBlockCount = 0;
        for (const auto& relativeBlockPosition : TCornerBlockOffsets) {
          if (GetBlock(mBoardInfo.currentPosition + relativeBlockPosition) != BlockType::None) {
            cornerBlockCount++;
          }
        }

        if (cornerBlockCount >= MinCornerBlocksForTSpin) {
          // T-Spin

          const auto behindBlockPosition = mBoardInfo.currentPosition + TBehindBlockOffsets[mBoardInfo.currentRotation];
          const std::array<Point2D, 2> sideBlockPositions = {
            mBoardInfo.currentPosition + TSideBlockOffsets[mBoardInfo.currentRotation][0],
            mBoardInfo.currentPosition + TSideBlockOffsets[mBoardInfo.currentRotation][1],
          };

          // check T-Spin Mini

          bool tSpinMini;
          /*
          // TOJ style T-Spin Mini recognition
          tSpinMini = GetBlock(behindBlockPosition) != BlockType::None;
          /*/
          // https://harddrop.com/wiki/T-Spin#T-Spin_Mini

          // check for some exceptions
          if (numClearedLines == 2 && mobile) {
            // T-Spin Mini Double
            // https://harddrop.com/wiki/T-Spin_Mini_Double
            tSpinMini = true;
          } else if (mLastRotationWallKickOffsetIndex == 4) {
            // case B (Mini = false): A T-Spin Single achieved with a T-Spin Triple twist (Offset 5)
            // NOTE: don't check for the number of cleared lines (TODO: confirm)
            tSpinMini = false;
          } else if (GetBlock(behindBlockPosition) == BlockType::None) {
            // case A / case C
            // case A (Mini = false): Standard T-Spin Single with a wall kick
            // case C (Mini = true):  A T-Spin Single with a hole behind the T
            // NOTE: don't check for the number of cleared lines (confirmed on Tetris 99)
            // NOTE: Recognize as T-Spin Mini only when the hole behind T is away from the hole around T
            //       A T B
            //       T T T
            //       C   D    T-Spin Mini if A or B == ' '
            tSpinMini = GetBlock(sideBlockPositions[0]) == BlockType::None || GetBlock(sideBlockPositions[1]) == BlockType::None;
          } else {
            // normal T-Spin Mini: T-Spin with no lines or with one line clear achieved with a wall kick
            tSpinMini = numClearedLines <= 1 && mLastRotationWallKickOffsetIndex != 0;
          }
          //*/

          switch (numClearedLines) {
            case 0:
              tSpin = tSpinMini ? TSpin::MiniZero : TSpin::Zero;
              break;

            case 1:
              tSpin = tSpinMini ? TSpin::MiniSingle : TSpin::Single;
              break;

            case 2:
              tSpin = tSpinMini ? TSpin::MiniDouble : TSpin::Double;
              break;

            case 3:
              tSpin = TSpin::Triple;
              break;

            default:
              assert(false);
              break;
          }
        }
      }

      // update statistics
      switch (numClearedLines) {
        case 1:
          mGameStatistics.numSingles++;
          break;

        case 2:
          mGameStatistics.numDoubles++;
          break;

        case 3:
          mGameStatistics.numTriples++;
          break;

        case 4:
          mGameStatistics.numQuadruples++;
          break;
      }

      // update statistics
      if (tSpin != TSpin::None) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"

        mGameStatistics.numAllTSpins++;
        switch (tSpin) {
          case TSpin::Zero:
            mGameStatistics.numTSpinZeros++;
            break;

          case TSpin::MiniZero:
            mGameStatistics.numTSpinMiniZeros++;
            break;

          case TSpin::Single:
            mGameStatistics.numTSpinSingles++;
            break;

          case TSpin::MiniSingle:
            mGameStatistics.numTSpinMiniSingles++;
            break;

          case TSpin::Double:
            mGameStatistics.numTSpinDoubles++;
            break;

          case TSpin::MiniDouble:
            mGameStatistics.numTSpinMiniDoubles++;
            break;

          case TSpin::Triple:
            mGameStatistics.numTSpinTriples++;
            break;
        }

#pragma GCC diagnostic pop
      }


      // dispatch T-Spin Event
      if (tSpin != TSpin::None) {
        Event::TSpin::DispatchEvent(tSpin, mBackToBackCount);
      }


      if (numClearedLines) {
        const bool backToBack = numClearedLines == 4 || tSpin != TSpin::None;

        std::memcpy(mBlocksBeforeClear.get(), mBlocks.get(), mBoardInfo.boardWidth * mBoardInfo.boardHeight * sizeof(BlockType));
        std::memcpy(mBlocksAfterClear.get(), mBlocks.get(), mBoardInfo.boardWidth * mBoardInfo.boardHeight * sizeof(BlockType));

        for (unsigned int i = 0; i < numClearedLines; i++) {
          const unsigned int y = clearedLines[i];

          /*
          for (unsigned int x = 1; x < mBoardInfo.boardWidth - 1; x++) {
            mBlocksAfterClear[y * mBoardInfo.boardWidth + x] = BlockType::None;
          }
          /*/
          static_assert(static_cast<unsigned int>(BlockType::None) == 0);
          std::memset(mBlocksAfterClear.get() + (y * mBoardInfo.boardWidth + 1), 0, (mBoardInfo.boardWidth - 2) * sizeof(BlockType));
          //*/

          std::memmove(mBlocks.get() + mBoardInfo.boardWidth, mBlocks.get(), mBoardInfo.boardWidth * y * sizeof(BlockType));
        }

        // clean top
        static_assert(static_cast<unsigned int>(BlockType::None) == 0);
        std::memset(mBlocks.get(), 0, mBoardInfo.boardWidth * numClearedLines * sizeof(BlockType));
        for (unsigned int y = 0; y < numClearedLines; y++) {
          GetBlockRef(Point2D{0, static_cast<int>(y)}) = BlockType::Wall;
          GetBlockRef(Point2D{static_cast<int>(mBoardInfo.boardWidth - 1), static_cast<int>(y)}) = BlockType::Wall;
        }

        const bool perfectClear = mBoardInfo.blockCount == 0;

        mLastLineClearInfo = LineClearInfo{
          numClearedLines,
          mRenLineCount + numClearedLines,
          {
            clearedLines[0],
            clearedLines[1],
            clearedLines[2],
            clearedLines[3],
          },
          tSpin,
          perfectClear,
          mRenCount,
          backToBack ? mBackToBackCount : 0,
          mBoardInfo.boardWidth,
          mBoardInfo.boardHeight,
          mBlocksBeforeClear.get(),
          mBlocksAfterClear.get(),
          mBlocks.get(),
        };

        Event::LineClear::DispatchEvent(mLastLineClearInfo);

        mRenCount++;
        mRenLineCount += numClearedLines;

        if (backToBack) {
          mBackToBackCount++;
        } else {
          // update statistics
          if (mBackToBackCount) {
            mBackToBackCount--;
          }
          mGameStatistics.numTotalBackToBacks += mBackToBackCount;
          mGameStatistics.numMaxBackToBacks = std::max(mGameStatistics.numMaxBackToBacks, mBackToBackCount);

          mBackToBackCount = 0;
        }

        // update statistics
        mGameStatistics.numClearedLines += numClearedLines;

        if (perfectClear) {
          mGameStatistics.numPerfectClears++;
        }
      } else {
        if (tSpin != TSpin::None) {
          mBackToBackCount++;
        }

        if (mRenCount) {
          mRenCount--;
        }
        mGameStatistics.numTotalRens += mRenCount;
        mGameStatistics.numMaxRens = std::max<GameStatistics::Count>(mGameStatistics.numMaxRens, mRenCount);

        mGameStatistics.numMaxRenLines = std::max<GameStatistics::Count>(mGameStatistics.numMaxRenLines, mRenLineCount);

        mRenCount = 0;
        mRenLineCount = 0;
      }

      ConsumeNextMino();
      InitializeNextMino();

      DispatchBoardUpdateEvent();
      DispatchStatisticsUpdateEvent();
    }


    bool Game::Hold() {
      if (mBoardInfo.gameOver) {
        return false;
      }

      if (mBoardInfo.holdUsed) {
        return false;
      }

      if (mBoardInfo.holdMino) {
        const auto prevHoldMino = mBoardInfo.holdMino.value();
        mBoardInfo.holdMino = mBoardInfo.currentMino;
        mBoardInfo.currentMino = prevHoldMino;
      } else {
        mBoardInfo.holdMino = mBoardInfo.currentMino;
        ConsumeNextMino();
      }
      InitializeNextMino();
      mBoardInfo.holdUsed = true;

      mGameStatistics.numHolds++;

      DispatchBoardUpdateEvent();
      DispatchStatisticsUpdateEvent();

      return true;
    }


    bool Game::MoveRight() {
      if (mBoardInfo.gameOver) {
        return false;
      }

      return Move(Offset2D{1, 0}, false);
    }


    bool Game::MoveLeft() {
      if (mBoardInfo.gameOver) {
        return false;
      }

      return Move(Offset2D{-1, 0}, false);
    }


    bool Game::MoveDown() {
      if (mBoardInfo.gameOver) {
        return false;
      }

      return Move(Offset2D{0, 1}, false);
    }


    bool Game::DropBottom(bool hard) {
      if (mBoardInfo.gameOver) {
        return false;
      }

      if (!Move(mBoardInfo.ghostPosition - mBoardInfo.currentPosition, true)) {
        assert(false);
        return false;
      }

      if (hard) {
        Lock();
      }

      return true;
    }


    bool Game::RotateRight() {
      if (mBoardInfo.gameOver) {
        return false;
      }

      return Rotate(RotationDirection::Right);
    }


    bool Game::RotateLeft() {
      if (mBoardInfo.gameOver) {
        return false;
      }

      return Rotate(RotationDirection::Left);
    }
  }   // namespace Game
}   // namespace Tetra
