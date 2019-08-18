#include "GameScene.hpp"
#include "Config.hpp"
#include "Ghost.hpp"
#include "Tetra/Game.hpp"
#include "../DbgPrintf.hpp"
#include "../GameConfig.hpp"
#include "../SceneManager.hpp"
#include "../Song.hpp"
#include "../Sound.hpp"
#include "../TilePrint.hpp"
#include "../UpdateFromConfig.hpp"
#include "../Signal/SignalBase.hpp"
#include "../Signal/KeyInputSignal.hpp"
#include "../Signal/DelaySignalDecorator.hpp"
#include "../Signal/OneShotSignalDecorator.hpp"
#include "../Signal/RepeatSignalDecorator.hpp"
#include "../Sound/MusicManager.hpp"
#include "../Sound/SoundManager.hpp"
#include <image/bg.hpp>
#include <image/obj.hpp>

#include <array>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <vector>

#include <gba.hpp>


namespace GameTetra {
  namespace {
    constexpr auto BlockTypeToMap = ([]() constexpr {
      std::array<std::uint16_t, Tetra::NumMinoTypes + 3> map{};
      map[0] = gba::BGMAP::TEXT::TILE(0) | gba::BGMAP::TEXT::PALETTE(0);
      map[8] = gba::BGMAP::TEXT::TILE(0) | gba::BGMAP::TEXT::PALETTE(0);
      map[9] = gba::BGMAP::TEXT::TILE(0) | gba::BGMAP::TEXT::PALETTE(0);
      for (unsigned int i = 0; i < Tetra::NumMinoTypes; i++) {
        map[static_cast<unsigned int>(Tetra::MinoTypeToBlockTypeTable[i])] = gba::BGMAP::TEXT::TILE(Tile::bg::Stacked::FirstTileIndex) | gba::BGMAP::TEXT::PALETTE(i + 1);
      }
      return map;
    })();

    constexpr auto MinoTypeToMap = ([]() constexpr {
      std::array<std::uint16_t, Tetra::NumMinoTypes> map{};
      for (unsigned int i = 0; i < Tetra::NumMinoTypes; i++) {
        map[i] = gba::BGMAP::TEXT::TILE(Tile::bg::Active::FirstTileIndex) | gba::BGMAP::TEXT::PALETTE(i + 1);
      }
      return map;
    })();


    inline void DisableObject(unsigned int objId) {
      volatile auto& objAttr = gba::pointer_memory::OAM[objId];
      objAttr.attr0 = gba::OBJATTR0::OBJ_DISABLE;
    }


    inline void SetBlockTile(unsigned int x, unsigned int y, std::uint16_t tile) {
      gba::pointer_memory::VRAM_BGMAP<Config::ScrBase::BG1Game>[(y + Config::Position::GameScreenY) * 32 + (x + Config::Position::GameScreenX)] = tile;
    }


    inline bool PlaySound(const std::uint32_t* sound, int priority) {
      constexpr std::array<SoundManager::Channel, 2> ChannelIds{
        SoundManager::Channel::A,
        SoundManager::Channel::B,
      };

      static int channelSoundPriorities[2] = {0, 0};

      auto& soundManager = SoundManager::GetInstance();

      //DbgPrintf("PlaySound %p, %d (A: %d %d, B: %d %d)\n", sound, priority, soundManager.IsPlaying(SoundManager::Channel::A), channelSoundPriorities[0], soundManager.IsPlaying(SoundManager::Channel::B), channelSoundPriorities[1]);

      // 再生していないチャンネルがあればそれを使う
      int minPriority = std::numeric_limits<int>::max();
      unsigned int minPriorityIndex = 0;
      for (unsigned int i = 0; i < 2; i++) {
        if (!soundManager.IsPlaying(ChannelIds[i])) {
          soundManager.Play(ChannelIds[i], sound);
          channelSoundPriorities[i] = priority;
          return true;
        }
        if (channelSoundPriorities[i] < minPriority) {
          minPriority = channelSoundPriorities[i];
          minPriorityIndex = i;
        }
      }

      // 再生していないチャンネルがなければ最も優先度の低い音を再生しているチャンネルを使う
      if (minPriority <= priority) {
        soundManager.Play(ChannelIds[minPriorityIndex], sound);
        channelSoundPriorities[minPriorityIndex] = priority;
        return true;
      }

      return false;
    }
  }


  // ## SimpleEffectObject
  ////////////////////////////////////////////////////////////////////////////////


  SimpleEffectObject::SimpleEffectObject(std::uint_fast16_t objId, unsigned int x, unsigned int y, unsigned int aliveFrame) :
    mObjId(objId),
    mX(x),
    mY(y),
    mAliveFrame(aliveFrame),
    mPtrObjAttr(&gba::pointer_memory::OAM[mObjId]),
    mFrameCount(0)
  {}


  SimpleEffectObject::SimpleEffectObject(std::uint_fast16_t objId, unsigned int x, unsigned int y, unsigned int aliveFrame, std::uint_fast16_t attr0Base, std::uint_fast16_t attr1Base, std::uint_fast16_t attr2Base) :
    SimpleEffectObject(objId, x, y, aliveFrame)
  {
    mPtrObjAttr->attr0 = attr0Base | gba::OBJATTR0::Y(y);
    mPtrObjAttr->attr1 = attr1Base | gba::OBJATTR1::X(x);
    mPtrObjAttr->attr2 = attr2Base;
  }


  void SimpleEffectObject::Erase() {
    //mPtrObjAttr->attr0 = gba::OBJATTR0::OBJ_DISABLE;
    gba::pointer_memory::OAM[mObjId].attr0 = gba::OBJATTR0::OBJ_DISABLE;
  }


  SimpleEffectObject::~SimpleEffectObject() {
    Erase();
  }


  bool SimpleEffectObject::Step() {
    //DbgPrintf("Step: %d, %d / %d\n", mObjId, mFrameCount, mAliveFrame);

    if (!mAliveFrame) {
      return false;
    }

    if (mFrameCount == mAliveFrame) {
      Erase();
      return true;
    }

    mFrameCount++;

    return false;
  }


  // ## GameScene
  ////////////////////////////////////////////////////////////////////////////////


  GameScene::GameScene(SceneManager& sceneManager) :
    Scene(sceneManager),
    //
    mExtreme(Root::GameConfig::GetGlobalConfig().extreme),
    mLevel(Root::GameConfig::GetGlobalConfig().level),
    mScore(0),
    mLinesToNextLevel(mExtreme || mLevel == Config::Board::MaxLevel ? 0 : Config::Board::LinesToNextLevel),
    mLinesToGameClear(GetLinesFromMode(Root::GameConfig::GetGlobalConfig().mode)),
    //
    mKeyInputA(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYCNT::A))),
    mKeyInputB(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYCNT::B))),
    mKeyInputSelect(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYCNT::SELECT))),
    mKeyInputStart(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYCNT::START))),
    mKeyInputRight(
      std::make_unique<RepeatSignalDecorator>(
        std::make_unique<DelaySignalDecorator>(
          std::make_unique<KeyInputSignal>(gba::KEYCNT::RIGHT), Config::Frame::Key::ArrowKeyDelay), Config::Frame::Key::HorizontalMoveDelay, Config::Frame::Key::HorizontalMoveInterval)),
    mKeyInputLeft(
      std::make_unique<RepeatSignalDecorator>(
        std::make_unique<DelaySignalDecorator>(
          std::make_unique<KeyInputSignal>(gba::KEYCNT::LEFT), Config::Frame::Key::ArrowKeyDelay), Config::Frame::Key::HorizontalMoveDelay, Config::Frame::Key::HorizontalMoveInterval)),
    mKeyInputUp(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<DelaySignalDecorator>(std::make_unique<KeyInputSignal>(gba::KEYCNT::UP), Config::Frame::Key::ArrowKeyDelay))),
    mKeyInputDown(
      std::make_unique<RepeatSignalDecorator>(
        std::make_unique<DelaySignalDecorator>(
          std::make_unique<KeyInputSignal>(gba::KEYCNT::DOWN), Config::Frame::Key::ArrowKeyDelay), Config::Frame::Key::VerticalMoveDelay, Config::Frame::Key::VerticalMoveInterval)),
    mKeyInputR(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYCNT::R))),
    mKeyInputL(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYCNT::L))),
    //
    mMinoChanged(false),
    mMinoLanded(false),
    mMinoLocked(false),
    mLineCleared(false),
    mHasBlockAboveClearedLine(false),
    mTSpin(Tetra::TSpin::None),
    //
    mBackToBackCount(0),
    mPtrLastLineClearInfo(nullptr),
    //
    mMinoWaitState(MinoWaitState::None),
    mPrevMinoWaitState(MinoWaitState::None),
    mLineClearAnimeState(0),
    //
    mFrameCount(0),
    mNextLockFrame(0),
    mNextLineClearAnimeFrame(0),
    mNextMinoShowFrame(0),
    mLastMinoShowFrame(0),
    //
    mFallCounter(0),
    //
    mHardDropEffectInfo{},
    //
    mPerfectClearLeftEffect(),
    mPerfectClearRightEffect(),
    mTetrisEffect(),
    mTSpinEffect(),
    mTSpinSDTEffect(),
    mTSpinMiniEffect(),
    mRenEffect(),
    mRenDigit1Effect(),
    mRenDigit21Effect(),
    mRenDigit22Effect(),
    mBackToBackEffect(),
    //
    mBaggedMinoFactory(),
    mGame(Tetra::Game::Game::InitializeInfo{
      Config::Board::WidthIncludingBorder,
      Config::Board::HeightIncludingBorder,
      Config::Board::BaseYIncludingBorder,
      Config::Board::NumNexts,
      [this] (const Tetra::Game::Game& game) {
        return mBaggedMinoFactory(game);
      },
    })
  {
    //DbgPrintf("ctor of GameTetra::GameScene\n");

    InitializeEventListeners();

    ResetNextFallFrame();

#ifndef RELEASE_BUILD
    InitializeDebugBoard();
#endif

    InitializeObjects();

    Root::PlayMusicFromConfig();
  }


  GameScene::~GameScene() {
    //DbgPrintf("dtor of GameTetra::GameScene\n");

    MusicManager::GetInstance().Stop();

    for (unsigned int i = 0; i < Tetra::MaxMinoSize; i++) {
      const auto objIdDiff = Config::ObjId::LineClearDiff * i;

      DisableObject(Config::ObjId::LineClearFirstLeft + objIdDiff);

      DisableObject(Config::ObjId::LineClearFirstRight + objIdDiff);

      for (unsigned int j = 0; j < Config::ObjId::NumLineClearMiddle; j++) {
        DisableObject(Config::ObjId::LineClearFirstMiddleFirst + objIdDiff + j);
      }
    }

    DisableObject(Config::ObjId::HoldMino);

    for (unsigned int i = 0; i < Config::Board::NumNexts; i++) {
      DisableObject(Config::ObjId::NextMinoFirst + i);
    }
  }


  // ## GameScene/Initialization
  ////////////////////////////////////////////////////////////////////////////////


  void GameScene::InitializeEventListeners() {
    using namespace Tetra::Game::Event;


    mGame.NewMino::AddEventListener([this] ([[maybe_unused]] Tetra::MinoType mino) {
      mMinoChanged = true;
    });

    mGame.Land::AddEventListener([this] ([[maybe_unused]] bool first) {
      mMinoLanded = true;
    });

    mGame.Lock::AddEventListener([this] () {
      mMinoLocked = true;
    });

    mGame.LineClear::AddEventListener([this] (const Tetra::Game::LineClearInfo& lineClearInfo) {
      mLineCleared = true;
      mPtrLastLineClearInfo = &lineClearInfo;
      mBackToBackCount = lineClearInfo.backToBack;

      // 消えた行より上にブロックが積まれているか調べる
      // 積まれている場合はライン消去後落とす（切り詰める）ときに効果音を再生するため
      // このゲームにおいて空の列は存在しないので、落下後の盤面の消去された最下列にブロックが存在するかで確認できる
      mHasBlockAboveClearedLine = false;
      const unsigned int y = lineClearInfo.clearedLines[lineClearInfo.numLines - 1];
      for (unsigned int x = Config::Board::Border; x < Config::Board::WidthIncludingBorder - Config::Board::Border; x++) {
        if (lineClearInfo.blocksAfterFall[y * Config::Board::WidthIncludingBorder + x] != Tetra::BlockType::None) {
          mHasBlockAboveClearedLine = true;
          break;
        }
      }
    });

    mGame.TSpin::AddEventListener([this] (Tetra::TSpin tSpin, unsigned long backToBackCount) {
      mTSpin = tSpin;
      mBackToBackCount = backToBackCount;
    });
  }


#ifndef RELEASE_BUILD
  void GameScene::InitializeDebugBoard() {
    [[maybe_unused]] constexpr auto N = Tetra::BlockType::None;
    [[maybe_unused]] constexpr auto I = Tetra::BlockType::I;
    [[maybe_unused]] constexpr auto O = Tetra::BlockType::O;
    [[maybe_unused]] constexpr auto S = Tetra::BlockType::S;
    [[maybe_unused]] constexpr auto Z = Tetra::BlockType::Z;
    [[maybe_unused]] constexpr auto J = Tetra::BlockType::J;
    [[maybe_unused]] constexpr auto L = Tetra::BlockType::L;
    [[maybe_unused]] constexpr auto T = Tetra::BlockType::T;

    const auto SetBlocks = [this] (auto blocks) {
      assert(blocks.size() % Config::Board::Width == 0);

      auto& boardInfo = const_cast<Tetra::Game::BoardInfo&>(mGame.GetBoardInfo());

      const auto boardBlocks = const_cast<Tetra::BlockType*>(boardInfo.blocks);

      const unsigned int baseY = Config::Board::Height - blocks.size() / Config::Board::Width + Config::Board::Border;

      unsigned int blockCount = 0;
      for (std::size_t i = 0; i < blocks.size(); i++) {
        const unsigned int y = i / Config::Board::Width + baseY;
        const unsigned int x = i % Config::Board::Width;

        boardBlocks[y * Config::Board::WidthIncludingBorder + x + Config::Board::Border] = blocks[i];

        if (blocks[i] != Tetra::BlockType::None) {
          blockCount++;
        }
      }

      boardInfo.blockCount = blockCount;

      // update ghost position
      mGame.MoveLeft();
      mGame.MoveRight();
    };

    switch (Config::Debug::DebugBoard) {
      case Config::Debug::DebugBoardType::DoubleQuad:
        SetBlocks(std::vector<Tetra::BlockType>{
          O, O, O, O, O, O, O, O, O, N,
          O, O, O, O, O, O, O, O, O, N,
          O, O, O, O, O, O, O, O, O, N,
          O, O, O, O, O, O, O, O, O, N,
          O, O, O, O, O, O, O, O, O, N,
          O, O, O, O, O, O, O, O, O, N,
          O, O, O, O, O, O, O, O, O, N,
          O, O, O, O, O, O, O, O, O, N,
        });
        break;

      case Config::Debug::DebugBoardType::QuadTST:
        SetBlocks(std::vector<Tetra::BlockType>{
          O, O, O, O, O, O, N, O, O, O,
          O, O, O, O, O, O, N, O, O, O,
          O, O, O, O, O, O, N, O, O, O,
          O, O, O, O, O, O, N, O, O, O,
          O, O, O, O, O, O, O, N, N, O,
          O, O, O, O, O, O, O, N, N, O,
          O, O, O, O, O, O, N, N, N, O,
          O, O, O, O, O, O, N, O, O, O,
          O, O, O, O, O, O, N, N, O, O,
          O, O, O, O, O, O, N, O, O, O,
        });
        break;

      case Config::Debug::DebugBoardType::DTPC:
        SetBlocks(std::vector<Tetra::BlockType>{
          N, N, N, O, O, O, O, O, O, O,
          N, N, O, O, O, O, O, O, O, O,
          N, N, O, O, O, O, O, O, O, O,
          N, N, O, O, O, O, O, O, O, O,
          N, N, N, O, O, O, O, O, O, O,
          O, O, N, O, O, O, O, O, O, O,
          O, N, N, O, O, O, O, O, O, O,
          O, N, N, N, O, O, O, O, O, O,
          O, O, N, O, O, O, O, O, O, O,
          O, O, N, O, O, O, O, O, O, O,
        });
        break;

      case Config::Debug::DebugBoardType::REN:
        const_cast<Tetra::Game::BoardInfo&>(mGame.GetBoardInfo()).currentPosition.y -= 2;
        SetBlocks(std::vector<Tetra::BlockType>{
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, N, N, N, O, O, O, O, O, O,
          N, O, O, O, O, O, O, O, O, O,
        });
        ResetNextLockFrame();
        break;

      default:
        // do nothing; for suppressing warning
        break;
    }
  }
#endif


  void GameScene::InitializeObjects() {
    // initialize line clear anime attributes
    for (unsigned int i = 0; i < Tetra::MaxMinoSize; i++) {
      const auto objIdDiff = Config::ObjId::LineClearDiff * i;

      // Left
      {
        volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::LineClearFirstLeft + objIdDiff];
        objAttr.attr0 = gba::OBJATTR0::OBJ_DISABLE;
        objAttr.attr1 = Tile::obj::LineClearEffectLeft::Attribute1Base | gba::OBJATTR1::X(Config::Position::LineClearLeftX);
        objAttr.attr2 = Tile::obj::LineClearEffectLeft::Attribute2Base | Config::Priority::Object::LineClearAnime;
      }

      // Right
      {
        volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::LineClearFirstRight + objIdDiff];
        objAttr.attr0 = gba::OBJATTR0::OBJ_DISABLE;
        objAttr.attr1 = Tile::obj::LineClearEffectRight::Attribute1Base | gba::OBJATTR1::X(Config::Position::LineClearRightX);
        objAttr.attr2 = Tile::obj::LineClearEffectRight::Attribute2Base | Config::Priority::Object::LineClearAnime;
      }

      // Middle
      for (unsigned int j = 0; j < Config::ObjId::NumLineClearMiddle; j++) {
        volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::LineClearFirstMiddleFirst + objIdDiff + j];
        objAttr.attr0 = gba::OBJATTR0::OBJ_DISABLE;
        objAttr.attr1 = Tile::obj::LineClearEffectMiddle::Attribute1Base | gba::OBJATTR1::X(Config::Position::LineClearMiddleStartX + j * Tile::obj::LineClearEffectMiddle::Width * 8);
        objAttr.attr2 = Tile::obj::LineClearEffectMiddle::Attribute2Base | Config::Priority::Object::LineClearAnime;
      }
    }

    // initialize hold mino attributes
    {
      volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::HoldMino];
      objAttr.attr0 = Tile::obj::Mino::Attribute0Base | gba::OBJATTR0::Y(Config::Position::HoldY);
      objAttr.attr1 = Tile::obj::Mino::Attribute1Base | gba::OBJATTR1::X(Config::Position::HoldX);
      objAttr.attr2 = gba::OBJATTR2::TILE(Tile::obj::Empty::TileIndex) | Config::Priority::Object::Hold | gba::OBJATTR2::PALETTE(Tile::obj::Mino::Palette);
    }

    // initialize next mino attributes
    for (unsigned int i = 0; i < Config::Board::NumNexts; i++) {
      volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::NextMinoFirst + i];
      objAttr.attr0 = Tile::obj::Mino::Attribute0Base | gba::OBJATTR0::Y(Config::Position::NextY + Config::Position::NextH * i);
      objAttr.attr1 = Tile::obj::Mino::Attribute1Base | gba::OBJATTR1::X(Config::Position::NextX);
      objAttr.attr2 = gba::OBJATTR2::TILE(Tile::obj::Empty::TileIndex) | Config::Priority::Object::Nexts | gba::OBJATTR2::PALETTE(Tile::obj::Mino::Palette);
    }
  }


  // ## GameScene/Utilities
  ////////////////////////////////////////////////////////////////////////////////


  void GameScene::ResetNextFallFrame() {
    mFallCounter = 0;
  }


  void GameScene::ResetNextLockFrame() {
    mNextLockFrame = mGame.GetBoardInfo().numOperationsAfterLand >= Config::Board::MaxOperationsAfterLand
      ? mFrameCount + 1
      : mFrameCount + Config::Frame::LockDelay + 1;
  }


  void GameScene::AddScore(unsigned int score) {
    mScore += score;
  }


  // ## GameScene/Rendering
  ////////////////////////////////////////////////////////////////////////////////


  void GameScene::RenderBoardTile() {
    const auto& boardInfo = mGame.GetBoardInfo();

    const auto blocks = mMinoWaitState == MinoWaitState::WaitByLineClear ? mPtrLastLineClearInfo->blocksAfterClear : boardInfo.blocks;

    // render board
    for (unsigned int y = 0; y < Config::Board::VisibleHeight; y++) {
      for (unsigned int x = 0; x < Config::Board::Width; x++) {
        SetBlockTile(x, y, BlockTypeToMap[static_cast<unsigned int>(blocks[(y + Config::Board::BaseYIncludingBorder) * Config::Board::WidthIncludingBorder + x + Config::Board::Border])]);
      }
    }

    // render hard drop effects
    if (mHardDropEffectInfo.numColumns) {
      for (unsigned int i = 0; i < mHardDropEffectInfo.numColumns; i++) {
        for (unsigned int ry = 0; ry < mHardDropEffectInfo.length; ry++) {
          const int x = mHardDropEffectInfo.x + static_cast<int>(i);
          const int y = mHardDropEffectInfo.ys[i] + static_cast<int>(ry);

          if (blocks[(y + Config::Board::BaseYIncludingBorder) * Config::Board::WidthIncludingBorder + x + Config::Board::Border] != Tetra::BlockType::None) {
            continue;
          }

          const auto tile = Tile::bg::HardDropEffect::MapData[Tile::bg::HardDropEffect::Height - mHardDropEffectInfo.length + ry];
          SetBlockTile(x, y, tile);
        }
      }
    }

    if (mMinoWaitState == MinoWaitState::None) {
      const auto currentMinoIndex = static_cast<unsigned int>(boardInfo.currentMino);
      const auto currentRotation = boardInfo.currentRotation;

      // render ghost mino
      const auto& ghostMinoTile = Ghost::GhostMinoTile[currentMinoIndex][currentRotation];
      for (const auto& cell : ghostMinoTile) {
        const auto x = boardInfo.ghostPosition.x + cell.x - static_cast<int>(Config::Board::Border);
        const auto y = boardInfo.ghostPosition.y + cell.y - static_cast<int>(Config::Board::BaseYIncludingBorder);
        if (y < 0) {
          continue;
        }
        SetBlockTile(x, y, cell.tile);
      }

      // render current mino
      const auto& currentMinoPoints = Tetra::Mino[currentMinoIndex].minos[currentRotation].points;
      const auto currentMinoTile = MinoTypeToMap[currentMinoIndex];
      for (const auto& point : currentMinoPoints) {
        const auto x = boardInfo.currentPosition.x + point.x - static_cast<int>(Config::Board::Border);
        const auto y = boardInfo.currentPosition.y + point.y - static_cast<int>(Config::Board::BaseYIncludingBorder);
        if (y < 0) {
          continue;
        }
        SetBlockTile(x, y, currentMinoTile);
      }
    }
  }


  void GameScene::RenderHoldMino() {
    if (mMinoWaitState != MinoWaitState::None) {
      return;
    }

    volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::HoldMino];

    const auto& boardInfo = mGame.GetBoardInfo();

    if (!boardInfo.holdMino) {
      objAttr.attr2 = gba::OBJATTR2::TILE(Tile::obj::Empty::TileIndex) | Config::Priority::Object::Hold | gba::OBJATTR2::PALETTE(Tile::obj::Mino::Palette);
      return;
    }

    const unsigned int holdMinoTileIndexOffset = static_cast<unsigned int>(boardInfo.holdMino.value()) * Tile::obj::Mino::MapWidth * Tile::obj::Mino::MapHeight;

    if (boardInfo.holdUsed) {
      objAttr.attr2 = gba::OBJATTR2::TILE(Tile::obj::GrayMino::TileIndex + holdMinoTileIndexOffset) | Config::Priority::Object::Hold | gba::OBJATTR2::PALETTE(Tile::obj::GrayMino::Palette);
    } else {
      objAttr.attr2 = gba::OBJATTR2::TILE(Tile::obj::Mino::TileIndex + holdMinoTileIndexOffset) | Config::Priority::Object::Hold | gba::OBJATTR2::PALETTE(Tile::obj::Mino::Palette);
    }
  }


  void GameScene::RenderNextMinos() {
    if (mMinoWaitState != MinoWaitState::None) {
      return;
    }

    const auto& boardInfo = mGame.GetBoardInfo();

    for (unsigned int i = 0; i < Config::Board::NumNexts; i++) {
      const auto mino = boardInfo.nextMinos[i];
      const unsigned int minoTileIndex = Tile::obj::Mino::TileIndex + static_cast<unsigned int>(mino) * Tile::obj::Mino::MapWidth * Tile::obj::Mino::MapHeight;

      volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::NextMinoFirst + i];

      objAttr.attr0 = Tile::obj::Mino::Attribute0Base | gba::OBJATTR0::Y(Config::Position::NextY + Config::Position::NextH * i);
      objAttr.attr1 = Tile::obj::Mino::Attribute1Base | gba::OBJATTR1::X(Config::Position::NextX);
      objAttr.attr2 = gba::OBJATTR2::TILE(minoTileIndex) | Config::Priority::Object::Nexts | gba::OBJATTR2::PALETTE(Tile::obj::Mino::Palette);
    }
  }


  void GameScene::RenderLineClearAnime() {
    if (mLineClearAnimeState == 0) {
      for (unsigned int i = 0; i < Tetra::MaxMinoSize; i++) {
        const auto objIdDiff = Config::ObjId::LineClearDiff * i;

        // Left
        {
          volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::LineClearFirstLeft + objIdDiff];
          objAttr.attr0 = gba::OBJATTR0::OBJ_DISABLE;
        }

        // Right
        {
          volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::LineClearFirstRight + objIdDiff];
          objAttr.attr0 = gba::OBJATTR0::OBJ_DISABLE;
        }

        // Middle
        for (unsigned int j = 0; j < Config::ObjId::NumLineClearMiddle; j++) {
          volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::LineClearFirstMiddleFirst + objIdDiff + j];
          objAttr.attr0 = gba::OBJATTR0::OBJ_DISABLE;
        }
      }
    } else {
      const auto lines = mPtrLastLineClearInfo->numLines;
      for (unsigned int i = 0; i < lines; i++) {
        const auto objIdDiff = Config::ObjId::LineClearDiff * i;
        const auto line = mPtrLastLineClearInfo->clearedLines[i];
        const auto y = (Config::Position::GameScreenY + line - Config::Board::BaseYIncludingBorder) * 8;

        // Left
        {
          using namespace Tile::obj::LineClearEffectLeft;

          volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::LineClearFirstLeft + objIdDiff];
          objAttr.attr0 = Tile::obj::LineClearEffectLeft::Attribute0Base | gba::OBJATTR0::Y(y);
          objAttr.attr2 = gba::OBJATTR2::TILE(TileIndex + MapWidth * MapHeight * (mLineClearAnimeState - 1)) | Config::Priority::Object::LineClearAnime | gba::OBJATTR2::PALETTE(Palette);
        }

        // Right
        {
          using namespace Tile::obj::LineClearEffectRight;

          volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::LineClearFirstRight + objIdDiff];
          objAttr.attr0 = Tile::obj::LineClearEffectRight::Attribute0Base | gba::OBJATTR0::Y(y);
          objAttr.attr2 = gba::OBJATTR2::TILE(TileIndex + MapWidth * MapHeight * (mLineClearAnimeState - 1)) | Config::Priority::Object::LineClearAnime | gba::OBJATTR2::PALETTE(Palette);
        }

        // Middle
        for (unsigned int j = 0; j < Config::ObjId::NumLineClearMiddle; j++) {
          using namespace Tile::obj::LineClearEffectMiddle;

          volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::LineClearFirstMiddleFirst + objIdDiff + j];
          objAttr.attr0 = Tile::obj::LineClearEffectMiddle::Attribute0Base | gba::OBJATTR0::Y(y);
          objAttr.attr2 = gba::OBJATTR2::TILE(TileIndex + MapWidth * MapHeight * (mLineClearAnimeState - 1)) | Config::Priority::Object::LineClearAnime | gba::OBJATTR2::PALETTE(Palette);
        }
      }
    }
  }


  void GameScene::RenderEffects() {
    constexpr auto stepEffect = [] (auto& effect) {
      if (!effect) {
        return;
      }
      effect.value().Step();
    };

    stepEffect(mPerfectClearLeftEffect);
    stepEffect(mPerfectClearRightEffect);
    stepEffect(mTetrisEffect);
    stepEffect(mTSpinEffect);
    stepEffect(mTSpinSDTEffect);
    stepEffect(mTSpinMiniEffect);
    stepEffect(mRenEffect);
    stepEffect(mRenDigit1Effect);
    stepEffect(mRenDigit21Effect);
    stepEffect(mRenDigit22Effect);
    stepEffect(mBackToBackEffect);
  }


  void GameScene::Render() {
    //DbgPrintf("render %d (%d)\n", mFrameCount, gba::reg::VCOUNT);

    static char str[32];

    snprintf(str, sizeof(str), "SCORE\n%9d", mScore);
    //snprintf(str, sizeof(str), "SCORE\n%09d", mScore);
    TilePrint<Config::ScrBase::BG0Score>(str, Config::Position::ScoreScreenX, Config::Position::ScoreScreenY);

    if (mExtreme) {
      snprintf(str, sizeof(str), "LEVEL  EX");
    } else{
      snprintf(str, sizeof(str), "LEVEL %3d", mLevel);
    }
    TilePrint<Config::ScrBase::BG0Score>(str, Config::Position::ScoreScreenX, Config::Position::ScoreScreenY + 3);

    snprintf(str, sizeof(str), "LINES %3d", mGame.GetGameStatistics().numClearedLines);
    TilePrint<Config::ScrBase::BG0Score>(str, Config::Position::ScoreScreenX, Config::Position::ScoreScreenY + 5);

    RenderBoardTile();
    RenderHoldMino();
    RenderNextMinos();
    RenderLineClearAnime();
    RenderEffects();
  }


  // ## GameScene/Updating
  ////////////////////////////////////////////////////////////////////////////////


  void GameScene::UpdateGameScore() {
    unsigned int score = 0;

    // normal line clear
    if (mLineCleared && mTSpin == Tetra::TSpin::None) {
      switch (mPtrLastLineClearInfo->numLines) {
        case 1:
          score += Config::Score::Single;
          break;

        case 2:
          score += Config::Score::Double;
          break;

        case 3:
          score += Config::Score::Triple;
          break;

        case 4:
          score += Config::Score::BackToBackBonus(Config::Score::Tetris, mBackToBackCount);
          break;
      }
    }

    // T-Spin
    if (mTSpin != Tetra::TSpin::None) {
      constexpr auto Scores = ([]() constexpr {
        std::array<unsigned int, Tetra::NumTSpinTypes> array{};

        array[static_cast<unsigned int>(Tetra::TSpin::None)] = 0;
        array[static_cast<unsigned int>(Tetra::TSpin::Zero)] = Config::Score::TSpin::Zero;
        array[static_cast<unsigned int>(Tetra::TSpin::MiniZero)] = Config::Score::TSpin::MiniZero;
        array[static_cast<unsigned int>(Tetra::TSpin::Single)] = Config::Score::TSpin::Single;
        array[static_cast<unsigned int>(Tetra::TSpin::MiniSingle)] = Config::Score::TSpin::MiniSingle;
        array[static_cast<unsigned int>(Tetra::TSpin::Double)] = Config::Score::TSpin::Double;
        array[static_cast<unsigned int>(Tetra::TSpin::MiniDouble)] = Config::Score::TSpin::MiniDouble;
        array[static_cast<unsigned int>(Tetra::TSpin::Triple)] = Config::Score::TSpin::Triple;

        return array;
      })();

      score += Config::Score::BackToBackBonus(Scores[static_cast<unsigned int>(mTSpin)], mBackToBackCount);
    }

    // REN
    if (mLineCleared) {
      score += Config::Score::REN(mPtrLastLineClearInfo->ren);
    }

    //
    AddScore(Config::Score::LevelBonus(score, mExtreme ? Config::Score::ExtremeLevelCoef : mLevel));
  }


  void GameScene::UpdateGameEffects(Tetra::MinoType prevMino, Tetra::Point2D prevPosition, Tetra::Rotation prevRotation, UserOperation userOperation, int hardDropDistance) {
    // effects

    // hard drop effect
    // 距離によらず表示する
    if (userOperation == UserOperation::HardDrop) {
      constexpr auto MinoHardDropInfo = ([]() constexpr {
        struct HardDropInfo {
          unsigned int numColumns;
          std::array<unsigned int, 4> ys;
        };

        using RotatedInfo = std::array<HardDropInfo, Tetra::NumRotationPatterns>;

        std::array<RotatedInfo, Tetra::NumMinoTypes> ret{};
        for (unsigned int minoIndex = 0; minoIndex < Tetra::NumMinoTypes; minoIndex++) {
          for (unsigned int rotation = 0; rotation < Tetra::NumRotationPatterns; rotation++) {
            const auto minoInfo = Tetra::Mino[minoIndex].minos[rotation];

            HardDropInfo info{
              static_cast<unsigned int>(minoInfo.maxPoint.x - minoInfo.minPoint.x + 1),
              {Tetra::MaxMinoSize, Tetra::MaxMinoSize, Tetra::MaxMinoSize, Tetra::MaxMinoSize},
            };
            for (const auto& point : minoInfo.points) {
              const auto x = point.x - minoInfo.minPoint.x;
              info.ys[x] = std::min<unsigned int>(info.ys[x], point.y);
            }

            for (auto& y : info.ys) {
              if (y == Tetra::MaxMinoSize) {
                y = 0;
              }
            }

            ret[minoIndex][rotation] = info;
          }
        }

        return ret;
      })();

      const auto& minoInfo = Tetra::Mino[static_cast<unsigned int>(prevMino)].minos[prevRotation];
      const auto& minoHardDropInfo = MinoHardDropInfo[static_cast<unsigned int>(prevMino)][prevRotation];

      const int currentY = prevPosition.y + hardDropDistance;
      const int length = std::max(0, std::min<int>(currentY + minoInfo.minPoint.y - static_cast<int>(Config::Board::BaseYIncludingBorder), Tile::bg::HardDropEffect::Height));

      std::array<unsigned int, Tetra::MaxMinoSize> ys{};
      for (unsigned int i = 0; i < minoHardDropInfo.numColumns; i++) {
        ys[i] = currentY + minoHardDropInfo.ys[i] - Config::Board::BaseYIncludingBorder - length;
      }

      mHardDropEffectInfo = HardDropEffectInfo{
        minoHardDropInfo.numColumns,
        mFrameCount + Config::Frame::HardDropEffect + 1,
        static_cast<unsigned int>(length),
        static_cast<unsigned int>(prevPosition.x + minoInfo.minPoint.x - Config::Board::Border),
        ys,
      };

      //DbgPrintf("mHardDropEffectInfo = %d, %d, %d (%d, %d, %d)\n", mHardDropEffectInfo.numColumns, mHardDropEffectInfo.length, mHardDropEffectInfo.x, hardDropDistance, currentY + minoInfo.minPoint.y - static_cast<int>(Config::Board::BaseYIncludingBorder), Tile::bg::HardDropEffect::Height);

      if (length == 0) {
        mHardDropEffectInfo.numColumns = 0;
      }
    }

    // T-Spin effect
    if (mTSpin != Tetra::TSpin::None) {
      constexpr auto IsMiniArray = ([]() constexpr {
        std::array<bool, Tetra::NumTSpinTypes> array{};

        array[static_cast<unsigned int>(Tetra::TSpin::MiniZero)] = true;
        array[static_cast<unsigned int>(Tetra::TSpin::MiniSingle)] = true;
        array[static_cast<unsigned int>(Tetra::TSpin::MiniDouble)] = true;

        return array;
      })();

      // 0 = no line clear, 1 = Single, 2 = Double, 3 = Triple
      constexpr auto SDTArray = ([]() constexpr {
        std::array<unsigned int, Tetra::NumTSpinTypes> array{};

        array[static_cast<unsigned int>(Tetra::TSpin::Single)] = 1;
        array[static_cast<unsigned int>(Tetra::TSpin::MiniSingle)] = 1;

        array[static_cast<unsigned int>(Tetra::TSpin::Double)] = 2;
        array[static_cast<unsigned int>(Tetra::TSpin::MiniDouble)] = 2;

        array[static_cast<unsigned int>(Tetra::TSpin::Triple)] = 3;

        return array;
      })();

      //DbgPrintf("T-Spin %d\n", static_cast<unsigned int>(mTSpin));

      // erase Tetris effect because it puts on
      mTetrisEffect.reset();

      if (IsMiniArray[static_cast<unsigned int>(mTSpin)]) {
        mTSpinMiniEffect.emplace(Config::ObjId::TSpin::Mini, Config::Position::Effect::MiniX, Config::Position::Effect::MiniY, Config::Frame::Effect::TSpinAlive, Tile::obj::Mini::Attribute0Base, Tile::obj::Mini::Attribute1Base, Tile::obj::Mini::Attribute2Base | Config::Priority::Object::Effects);
      }

      if (const auto type = SDTArray[static_cast<unsigned int>(mTSpin)]; type) {
        constexpr std::array<std::uint_fast16_t, 4> Attribute2Array{
          0,
          Tile::obj::Single::Attribute2Base | Config::Priority::Object::Effects,
          Tile::obj::Double::Attribute2Base | Config::Priority::Object::Effects,
          Tile::obj::Triple::Attribute2Base | Config::Priority::Object::Effects,
        };

        mTSpinSDTEffect.emplace(Config::ObjId::TSpin::SDT, Config::Position::Effect::TSpinSDTX, Config::Position::Effect::TSpinSDTY, Config::Frame::Effect::TSpinAlive, Tile::obj::Single::Attribute0Base, Tile::obj::Single::Attribute1Base, Attribute2Array[type]);
      }

      mTSpinEffect.emplace(Config::ObjId::TSpin::TSpin, Config::Position::Effect::TSpinX, Config::Position::Effect::TSpinY, Config::Frame::Effect::TSpinAlive, Tile::obj::TSpin::Attribute0Base, Tile::obj::TSpin::Attribute1Base, Tile::obj::TSpin::Attribute2Base | Config::Priority::Object::Effects);
    }

    // tetris effect
    if (mLineCleared && mPtrLastLineClearInfo->numLines == 4) {
      // erase T-Spin effects because they put on
      mTSpinEffect.reset();
      mTSpinMiniEffect.reset();
      mTSpinSDTEffect.reset();

      mTetrisEffect.emplace(Config::ObjId::Tetris, Config::Position::Effect::TetrisX, Config::Position::Effect::TetrisY, Config::Frame::Effect::TetrisAlive, Tile::obj::Tetris::Attribute0Base, Tile::obj::Tetris::Attribute1Base, Tile::obj::Tetris::Attribute2Base | Config::Priority::Object::Effects);
    }

    // REN effect
    if (mMinoLocked) {
      mRenEffect.reset();
      mRenDigit1Effect.reset();
      mRenDigit21Effect.reset();
      mRenDigit22Effect.reset();
    }

    if (mLineCleared) {
      if (const auto ren = mPtrLastLineClearInfo->ren; ren) {
        constexpr auto DigitAttribute2Array = ([]() constexpr {
          std::array<std::uint_fast16_t, 10> array{};
          for (unsigned int i = 0; i < 10; i++) {
            const auto tileIndex = Tile::obj::Number::TileIndex + (Tile::obj::Number::MapWidth * Tile::obj::Number::MapHeight) * i;
            array[i] = gba::OBJATTR2::TILE(tileIndex) | Config::Priority::Object::Effects | gba::OBJATTR2::PALETTE(Tile::obj::Number::Palette);
          }
          return array;
        })();

        mRenEffect.emplace(Config::ObjId::Ren::Ren, Config::Position::Effect::RenX, Config::Position::Effect::RenY, Config::Frame::Effect::RenAlive, Tile::obj::Ren::Attribute0Base, Tile::obj::Ren::Attribute1Base, Tile::obj::Ren::Attribute2Base | Config::Priority::Object::Effects);

        if (mPtrLastLineClearInfo->ren < 10) {
          mRenDigit1Effect.emplace(Config::ObjId::Ren::Digit1, Config::Position::Effect::RenDigit1X, Config::Position::Effect::RenDigitY, Config::Frame::Effect::RenAlive, Tile::obj::Number::Attribute0Base, Tile::obj::Number::Attribute1Base, DigitAttribute2Array[ren]);
        } else {
          mRenDigit21Effect.emplace(Config::ObjId::Ren::Digit21, Config::Position::Effect::RenDigit21X, Config::Position::Effect::RenDigitY, Config::Frame::Effect::RenAlive, Tile::obj::Number::Attribute0Base, Tile::obj::Number::Attribute1Base, DigitAttribute2Array[ren / 10]);
          mRenDigit22Effect.emplace(Config::ObjId::Ren::Digit22, Config::Position::Effect::RenDigit22X, Config::Position::Effect::RenDigitY, Config::Frame::Effect::RenAlive, Tile::obj::Number::Attribute0Base, Tile::obj::Number::Attribute1Base, DigitAttribute2Array[ren % 10]);
        }
      }
    }

    // back to back effect
    if ((mLineCleared || mTSpin != Tetra::TSpin::None) && mBackToBackCount) {
      mBackToBackEffect.emplace(Config::ObjId::BackToBack, Config::Position::Effect::BackToBackX, Config::Position::Effect::BackToBackY, Config::Frame::Effect::BackToBackAlive, Tile::obj::BackToBack::Attribute0Base, Tile::obj::BackToBack::Attribute1Base, Tile::obj::BackToBack::Attribute2Base | Config::Priority::Object::Effects);
    }

    // perfect clear effect
    if (mLineCleared && mPtrLastLineClearInfo->perfectClear) {
      mPerfectClearLeftEffect.emplace(Config::ObjId::PerfectClearLeft, Config::Position::Effect::PerfectClearLeftX, Config::Position::Effect::PerfectClearLeftY, Config::Frame::Effect::PerfectClearAlive, Tile::obj::PerfectClearLeft::Attribute0Base, Tile::obj::PerfectClearLeft::Attribute1Base, Tile::obj::PerfectClearLeft::Attribute2Base | Config::Priority::Object::Effects);
      mPerfectClearRightEffect.emplace(Config::ObjId::PerfectClearRight, Config::Position::Effect::PerfectClearRightX, Config::Position::Effect::PerfectClearRightY, Config::Frame::Effect::PerfectClearAlive, Tile::obj::PerfectClearRight::Attribute0Base, Tile::obj::PerfectClearRight::Attribute1Base, Tile::obj::PerfectClearRight::Attribute2Base | Config::Priority::Object::Effects);
    }
  }


  void GameScene::UpdateGameSounds(UserOperation userOperation, bool userOperationSucceeded, bool levelUp) {
    // operations (move, rotate, harddrop, hold)
    if (userOperationSucceeded) {
      switch (userOperation) {
        case UserOperation::Rotate:
          PlaySound(Sound::tetra_rotate, 100);
          break;

        case UserOperation::MoveHorizontal:
          PlaySound(Sound::tetra_move_horizontal, 100);
          break;

        case UserOperation::MoveDown:
          PlaySound(Sound::tetra_move_down, 100);
          break;

        case UserOperation::HardDrop:
          // ハードドロップの距離によらず再生する
          PlaySound(Sound::tetra_harddrop, 100);
          break;

        case UserOperation::Hold:
          PlaySound(Sound::tetra_hold, 150);
          break;

        case UserOperation::None:
          // do nothing; just for supressing warning
          break;
      }
    }

    // land
    if (!mMinoLocked && mMinoLanded) {
      PlaySound(Sound::tetra_land, 100);
    }

    // lock
    if (mMinoLocked) {
      PlaySound(Sound::tetra_lock, 120);
    }

    // line clear
    if (mLineCleared) {
      const std::uint32_t* lineClearSounds[] = {
        nullptr,
        Sound::tetra_lineclear_single,
        Sound::tetra_lineclear_double,
        Sound::tetra_lineclear_triple,
        Sound::tetra_lineclear_tetris,
      };

      PlaySound(lineClearSounds[mPtrLastLineClearInfo->numLines], 200);
    }

    // T-Spin
    if (mTSpin != Tetra::TSpin::None) {
      PlaySound(Sound::tetra_tspin, 250);
    }

    // perfect clear
    if (mLineCleared && mPtrLastLineClearInfo->perfectClear) {
      PlaySound(Sound::tetra_perfectclear, 300);
    }

    // ライン消去後の落下音
    if (mPrevMinoWaitState == MinoWaitState::WaitByLineClear && mHasBlockAboveClearedLine) {
      PlaySound(Sound::tetra_falldown, 200);
    }

    // level up
    if (levelUp) {
      PlaySound(Sound::tetra_levelup, 400);
    }
  }


  void GameScene::UpdateGame() {
    if (mMinoWaitState != MinoWaitState::None) {
      return;
    }


    const auto prevMino = mGame.GetBoardInfo().currentMino;
    const auto prevPosition = mGame.GetBoardInfo().currentPosition;
    const auto prevRotation = mGame.GetBoardInfo().currentRotation;

    int hardDropDistance = 0;
    UserOperation userOperation = UserOperation::None;
    bool userOperationSucceeded = false;

    mMinoChanged = false;
    mMinoLanded = false;
    mMinoLocked = false;
    mLineCleared = false;
    mTSpin = Tetra::TSpin::None;

    // ******** BEGIN mGame Update ********

    // auto mino fall
    //DbgPrintf("fc: %d\n", mFallCounter);
    const auto& fallInfo = mExtreme ? Config::Frame::ExtremeFall : Config::Frame::Fall[mLevel];

    mFallCounter += fallInfo.second;
    while (mFallCounter >= fallInfo.first) {
      mFallCounter -= fallInfo.first;
      if (mGame.MoveDown()) {
        ResetNextLockFrame();
      }
    }

    // auto lock
    if (mFrameCount == mNextLockFrame && mGame.GetBoardInfo().onceLanded) {
      if (mGame.GetBoardInfo().landing) {
        mGame.Lock();
      } else {
        mNextLockFrame++;
      }
    }

    if (mKeyInputR->GetState() || mKeyInputL->GetState()) {
      userOperation = UserOperation::Hold;
      userOperationSucceeded = mGame.Hold();
    } else {
      if (const auto stateRight = mKeyInputRight->GetState(), stateLeft = mKeyInputLeft->GetState(); stateRight || stateLeft) {
        userOperation = UserOperation::MoveHorizontal;
        userOperationSucceeded = stateRight ? mGame.MoveRight() : mGame.MoveLeft();
        if (userOperationSucceeded) {
          ResetNextLockFrame();
        }
      }
      if (mKeyInputDown->GetState()) {
        userOperation = UserOperation::MoveDown;
        userOperationSucceeded = mGame.MoveDown();
        if (userOperationSucceeded) {
          //ResetNextFallFrame();
          ResetNextLockFrame();
          AddScore(1 * Config::Score::SoftDropPerCell);
        }
      }
      if (const auto stateA = mKeyInputA->GetState(), stateB = mKeyInputB->GetState(); (stateA || stateB) && mFrameCount - mLastMinoShowFrame >= Config::Frame::Key::HoldEnableWait) {
        userOperation = UserOperation::Rotate;
        userOperationSucceeded = stateA ? mGame.RotateRight() : mGame.RotateLeft();
        if (userOperationSucceeded) {
          ResetNextLockFrame();
        }
      }
      if (mKeyInputUp->GetState() && mFrameCount - mLastMinoShowFrame >= Config::Frame::Key::HardDropEnableWait) {
        userOperation = UserOperation::HardDrop;
        const auto prevY = mGame.GetBoardInfo().currentPosition.y;
        // NOTE: DropBottom returns `true` normally
        userOperationSucceeded = mGame.DropBottom(false);
        hardDropDistance = mGame.GetBoardInfo().currentPosition.y - prevY;
        assert(hardDropDistance >= 0);
        mGame.Lock();
        AddScore(static_cast<unsigned int>(hardDropDistance) * Config::Score::HardDropPerCell);
      }
    }

    // ******** END mGame Update ********

    // update states and frames

    if (mMinoChanged) {
      mLastMinoShowFrame = mFrameCount;
      ResetNextFallFrame();
      ResetNextLockFrame();
    }

    if (mLineCleared) {
      mNextMinoShowFrame = mFrameCount + Config::Frame::LineClearWait + 1;
      mNextLineClearAnimeFrame = mFrameCount + Config::Frame::LineClearAnime + 1;
      mLineClearAnimeState = 1;
      mMinoWaitState = MinoWaitState::WaitByLineClear;
    } else if (mMinoLocked) {
      mNextMinoShowFrame = mFrameCount + Config::Frame::NextMino + 1;
      mMinoWaitState = MinoWaitState::Wait;
    }

    // level up
    bool levelUp = false;
    if (mLineCleared && mLinesToNextLevel) {
      const auto numClearedLines = mGame.GetGameStatistics().numClearedLines;
      if (numClearedLines >= mLinesToNextLevel) {
        levelUp = true;
        mLevel++;

        if (mLevel != Config::Board::MaxLevel) {
          mLinesToNextLevel += Config::Board::LinesToNextLevel;
        } else{
          mLinesToNextLevel = 0;
        }
      }
    }

    // update score
    UpdateGameScore();

    // update effects
    UpdateGameEffects(prevMino, prevPosition, prevRotation, userOperation, hardDropDistance);

    // update sounds
    UpdateGameSounds(userOperation, userOperationSucceeded, levelUp);
  }


  void GameScene::UpdateAnimeAndEffects() {
    // hard drop effect
    if (mHardDropEffectInfo.numColumns) {
      if (mFrameCount == mHardDropEffectInfo.endFrame) {
        mHardDropEffectInfo.numColumns = 0;
      }
    }

    // line clear anime
    if (mLineClearAnimeState) {
      if (mFrameCount == mNextLineClearAnimeFrame) {
        mNextLineClearAnimeFrame += Config::Frame::LineClearAnime;
        mLineClearAnimeState++;
      }

      if (mLineClearAnimeState > Config::Board::NumLineClearAnimationFrames) {
        mLineClearAnimeState = 0;
      }
    }
  }


  void GameScene::UpdateMinoWaitState() {
    if (mMinoWaitState != MinoWaitState::None && mFrameCount == mNextMinoShowFrame) {
      mMinoWaitState = MinoWaitState::None;
      ResetNextFallFrame();
    }
  }


  void GameScene::UpdateSignals() {
    mKeyInputA->Step();
    mKeyInputB->Step();
    mKeyInputSelect->Step();
    mKeyInputStart->Step();
    mKeyInputRight->Step();
    mKeyInputLeft->Step();
    mKeyInputUp->Step();
    mKeyInputDown->Step();
    mKeyInputR->Step();
    mKeyInputL->Step();
  }


  void GameScene::Update() {
    //DbgPrintf("update %d : %d, %d\n", mFrameCount, static_cast<int>(mMinoWaitState), mNextMinoShowFrame);

    if (Root::SceneManager::GetInstance().oneShotKeyInputStart->GetState()) {
      SetScene(SceneId::GamePause);
      return;
    }


    UpdateSignals();

    mPrevMinoWaitState = mMinoWaitState;

    UpdateAnimeAndEffects();
    UpdateMinoWaitState();
    UpdateGame();

    if (mPrevMinoWaitState != MinoWaitState::None && mMinoWaitState == MinoWaitState::None && mGame.GetGameStatistics().numClearedLines >= mLinesToGameClear) {
      mMinoWaitState = MinoWaitState::GameEnd;
      SetScene(SceneId::GameClear);
      return;
    }

    if (mGame.GetBoardInfo().gameOver) {
      mMinoWaitState = MinoWaitState::GameEnd;
      SetScene(SceneId::GameOver);
      return;
    }

    mFrameCount++;
  }


  // ## GameScene/Public Methods
  ////////////////////////////////////////////////////////////////////////////////


  const Tetra::Game::Game& GameScene::GetGame() const {
    return mGame;
  }


  bool GameScene::GetExtremeMode() const {
    return mExtreme;
  }


  unsigned int GameScene::GetLevel() const {
    return mLevel;
  }


  std::uint_fast32_t GameScene::GetScore() const {
    return mScore;
  }
}   // namespace GameTetra
