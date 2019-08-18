#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "Common.hpp"
#include "Mino.hpp"
#include "EventEmitter.hpp"


namespace Tetra {
  namespace Game {
    struct BoardInfo {
      bool gameOver;
      unsigned int boardWidth;
      unsigned int boardHeight;
      unsigned int baseY;
      const BlockType* blocks;
      std::size_t blockCount;
      const std::deque<MinoType>& nextMinos;
      std::optional<MinoType> holdMino;
      bool holdUsed;
      bool landing;
      bool onceLanded;
      unsigned long numOperationsAfterLand;
      MinoType currentMino;
      Point2D currentPosition;
      Rotation currentRotation;
      Point2D ghostPosition;
    };


    struct GameStatistics {
      using Count = std::uint_fast32_t;

      // they include T-Spins
      Count numSingles = 0;
      Count numDoubles = 0;
      Count numTriples = 0;
      Count numQuadruples = 0;

      Count numTSpinZeros = 0;
      Count numTSpinMiniZeros = 0;
      Count numTSpinMiniSingles = 0;
      Count numTSpinSingles = 0;
      Count numTSpinMiniDoubles = 0;
      Count numTSpinDoubles = 0;
      Count numTSpinTriples = 0;
      Count numAllTSpins = 0;         // numTSpinZeros + numTSpinMiniZeros + numTSpinMiniSingles + numTSpinSingles + numTSpinMiniDoubles + numTSpinDoubles + numTSpinTriples

      Count numMaxRens = 0;
      Count numTotalRens = 0;
      Count numMaxBackToBacks = 0;
      Count numTotalBackToBacks = 0;
      Count numMaxRenLines = 0;
      Count numPerfectClears = 0;
      Count numClearedLines = 0;      // numSingles * 1 + numDoubles * 2 + numTriples * 3 + numQuadruples * 4;
      Count numMinos = 0;

      Count numHolds = 0;
      Count numLeftMoves = 0;
      Count numRightMoves = 0;
      Count numLeftRotations = 0;
      Count numRightRotations = 0;
      Count numHardDrops = 0;
      Count numDropDistance = 0;     // includes drop distance of hard drops
    };


    struct LineClearInfo {
      unsigned int numLines = 0;
      unsigned int numRenLines = 0;
      unsigned int clearedLines[4] = {};
      TSpin tSpin = TSpin::None;
      bool perfectClear = false;
      unsigned int ren = 0;
      std::uint_fast32_t backToBack = 0;
      unsigned int boardWidth = 0;
      unsigned int boardHeight = 0;
      const BlockType* blocksBeforeClear = nullptr;
      const BlockType* blocksAfterClear = nullptr;
      const BlockType* blocksAfterFall = nullptr;
    };


    namespace Event {
      enum class EventType {
        BoardUpdate,
        StatisticsUpdate,
        NewMino,
        NextMino,
        TSpin,
        LineClear,
        Land,
        Lock,
        GameOver,
      };

      using BoardUpdate = EventEmitter<EventType::BoardUpdate, const BoardInfo&>;
      using StatisticsUpdate = EventEmitter<EventType::StatisticsUpdate, const GameStatistics&>;
      using NewMino = EventEmitter<EventType::NewMino, MinoType>;
      using NextMino = EventEmitter<EventType::NextMino, const std::deque<MinoType>&>;
      using TSpin = EventEmitter<EventType::TSpin, TSpin, unsigned long>;
      using LineClear = EventEmitter<EventType::LineClear, const LineClearInfo&>;
      using Land = EventEmitter<EventType::Land, bool>;
      using Lock = EventEmitter<EventType::Lock>;
      using GameOver = EventEmitter<EventType::GameOver>;

      // The diferrence between NewMino event and NextMino event is that NewMino event is dispatched when the current mino is changed (including the second time hold)
      // while NextMino event is dispatched when the "next" minos are updated (not including the second time hold).
    }


    class Game :
      public Event::BoardUpdate,
      public Event::StatisticsUpdate,
      public Event::NewMino,
      public Event::NextMino,
      public Event::TSpin,
      public Event::LineClear,
      public Event::Land,
      public Event::Lock,
      public Event::GameOver
    {
      static constexpr std::array<Point2D, NumMinoTypes> CalcInitialPositions(unsigned int boardWidth, unsigned int baseY) {
        std::array<Point2D, NumMinoTypes> initialPositions{
          Point2D{0, 0},    // dummy
          Point2D{0, 0},    // dummy
          Point2D{0, 0},    // dummy
          Point2D{0, 0},    // dummy
          Point2D{0, 0},    // dummy
          Point2D{0, 0},    // dummy
          Point2D{0, 0},    // dummy
        };

        assert(boardWidth >= NumMinoCells);
        for (std::size_t i = 0; i < NumMinoTypes; i++) {
          initialPositions[i] = Point2D{
            static_cast<int>((boardWidth - Mino[i].minos[0].width) / 2),
            static_cast<int>(baseY),
          };
        }

        return initialPositions;
      }

    public:
      using MinoFactory = std::function<MinoType(const Game& game)>;
      using EventListener = std::function<void(Game& game, void* data)>;

      struct InitializeInfo {
        unsigned int boardWidth;
        unsigned int boardHeight;
        unsigned int baseY;
        std::size_t numNexts;
        MinoFactory minoFactory;
      };

    private:
      std::array<Point2D, NumMinoTypes> mInitialPositions;
      std::unique_ptr<BlockType[]> mBlocks;
      std::unique_ptr<BlockType[]> mBlocksBeforeClear;
      std::unique_ptr<BlockType[]> mBlocksAfterClear;
      std::deque<MinoType> mNextMinos;
      BoardInfo mBoardInfo;
      GameStatistics mGameStatistics;
      LineClearInfo mLastLineClearInfo;
      unsigned int mRenCount;
      std::uint_fast32_t mRenLineCount;
      std::uint_fast32_t mBackToBackCount;
      bool mLastOperationRotation;
      unsigned int mLastRotationWallKickOffsetIndex;
      MinoFactory mMinoFactory;

      BlockType& GetBlockRef(const Point2D& position);
      void DispatchBoardUpdateEvent();
      void DispatchStatisticsUpdateEvent();
      void GameOver();
      void ConsumeNextMino();
      void UpdatePosition();
      void InitializeNextMino();
      bool Move(const Offset2D& offset, bool hardDrop);
      bool Rotate(RotationDirection rotationDirection);

    public:
      static bool Collide(std::size_t boardWidth, std::size_t boardHeight, const BlockType* blocks, MinoType minoType, const Point2D& position, Rotation rotation);;

      Game(const InitializeInfo& initializeInfo);

      const BoardInfo& GetBoardInfo() const;
      const GameStatistics& GetGameStatistics() const;

      BlockType GetBlock(const Point2D& position) const;

      bool Collide(MinoType minoType, const Point2D& position, Rotation rotation) const;
      bool Collide(const Point2D& position, Rotation rotation) const;

      bool IsLanded(const Point2D& position, Rotation rotation) const;
      bool IsLanded() const;

      void Lock();
      bool Hold();
      bool MoveRight();
      bool MoveLeft();
      bool MoveDown();
      bool DropBottom(bool hard);
      bool RotateRight();
      bool RotateLeft();
    };
  }
}
