#pragma once

#include "Scene.hpp"
#include "BaggedMinoFactory.hpp"
#include "Tetra/Game.hpp"
#include "../GameConfig.hpp"
#include "../Signal/SignalBase.hpp"

#include <array>
#include <cassert>
#include <memory>
#include <optional>
#include <gba.hpp>


namespace GameTetra {
  class SimpleEffectObject {
  protected:
    std::uint_fast16_t mObjId;
    unsigned int mX;
    unsigned int mY;
    unsigned int mAliveFrame;
    volatile gba::OBJAttr* mPtrObjAttr;
    unsigned int mFrameCount;

    SimpleEffectObject(std::uint_fast16_t objId, unsigned int x, unsigned int y, unsigned int aliveFrame);

    void Erase();

  public:
    // to suppress GCC warnings
    SimpleEffectObject(const SimpleEffectObject&) = default;
    SimpleEffectObject& operator=(const SimpleEffectObject&) = default;

    SimpleEffectObject(std::uint_fast16_t objId, unsigned int x, unsigned int y, unsigned int aliveFrame, std::uint_fast16_t attr0Base, std::uint_fast16_t attr1Base, std::uint_fast16_t attr2Base);
    virtual ~SimpleEffectObject();

    virtual bool Step();
  };


  class GameScene : public Scene {
    // frame count, must be unsigned
    using Frame = unsigned int;

    enum class MinoWaitState {
      None,
      Wait,
      WaitByLineClear,
      GameEnd,
    };

    struct HardDropEffectInfo {
      unsigned int numColumns;                            // 列数、0ならエフェクトなし
      Frame endFrame;                                     // ハードドロップエフェクト終了フレーム
      unsigned int length;                                // 長さ
      unsigned int x;                                     // 左端
      std::array<unsigned int, Tetra::MaxMinoSize> ys;    // 上端Y座標
    };

    enum class UserOperation {
      None,
      Rotate,
      MoveHorizontal,
      MoveDown,
      HardDrop,
      Hold,
    };


    static constexpr unsigned int GetLinesFromMode(Root::GameConfig::Mode mode) {
      switch (mode) {
        case Root::GameConfig::Mode::Line150:
          return 150;

        case Root::GameConfig::Mode::Line999:
          return 999;

        default:
          assert(false);
          return 0;
      }
    }


    bool mExtreme;                    // エクストリームモード
    unsigned int mLevel;              // レベル
    std::uint_fast32_t mScore;        // スコア
    unsigned int mLinesToNextLevel;   // 次のレベルアップ時のトータル消去ライン数（0ならこれ以上レベルアップしない）
    unsigned int mLinesToGameClear;   // ゲームクリアになるライン数

    std::unique_ptr<SignalBase> mKeyInputA;
    std::unique_ptr<SignalBase> mKeyInputB;
    std::unique_ptr<SignalBase> mKeyInputSelect;
    std::unique_ptr<SignalBase> mKeyInputStart;
    std::unique_ptr<SignalBase> mKeyInputRight;
    std::unique_ptr<SignalBase> mKeyInputLeft;
    std::unique_ptr<SignalBase> mKeyInputUp;
    std::unique_ptr<SignalBase> mKeyInputDown;
    std::unique_ptr<SignalBase> mKeyInputR;
    std::unique_ptr<SignalBase> mKeyInputL;

    bool mMinoChanged;
    bool mMinoLanded;
    bool mMinoLocked;
    bool mLineCleared;
    bool mHasBlockAboveClearedLine;
    Tetra::TSpin mTSpin;

    std::uint_fast32_t mBackToBackCount;
    const Tetra::Game::LineClearInfo* mPtrLastLineClearInfo;

    MinoWaitState mMinoWaitState;         // 次のミノ出現までの小休止を表す状態
    MinoWaitState mPrevMinoWaitState;     // 前フレームのmMinoWaitState
    unsigned int mLineClearAnimeState;    // 0 = ライン消去状態でない / 1 ～ NumLineClearAnimationFrames = ライン消去アニメ

    Frame mFrameCount;                    // フレームカウント
    Frame mNextLockFrame;                 // 次のミノ固定フレーム
    Frame mNextLineClearAnimeFrame;       // 次のライン消去アニメ進行フレーム
    Frame mNextMinoShowFrame;             // 次のミノ出現フレーム
    Frame mLastMinoShowFrame;             // ミノ出現フレーム

    unsigned int mFallCounter;            // 落下フレーム計算用カウンタ（落下間隔が分数なためmFrameCountを用いれない）

    HardDropEffectInfo mHardDropEffectInfo;

    std::optional<SimpleEffectObject> mPerfectClearLeftEffect;
    std::optional<SimpleEffectObject> mPerfectClearRightEffect;
    std::optional<SimpleEffectObject> mTetrisEffect;
    std::optional<SimpleEffectObject> mTSpinEffect;
    std::optional<SimpleEffectObject> mTSpinSDTEffect;
    std::optional<SimpleEffectObject> mTSpinMiniEffect;
    std::optional<SimpleEffectObject> mRenEffect;
    std::optional<SimpleEffectObject> mRenDigit1Effect;
    std::optional<SimpleEffectObject> mRenDigit21Effect;
    std::optional<SimpleEffectObject> mRenDigit22Effect;
    std::optional<SimpleEffectObject> mBackToBackEffect;

    BaggedMinoFactory mBaggedMinoFactory;
    Tetra::Game::Game mGame;

    void InitializeEventListeners();
#ifndef RELEASE_BUILD
    void InitializeDebugBoard();
#endif
    void InitializeObjects();

    void ResetNextFallFrame();
    void ResetNextLockFrame();
    void AddScore(unsigned int score);

    void RenderBoardTile();
    void RenderHoldMino();
    void RenderNextMinos();
    void RenderLineClearAnime();
    void RenderEffects();

    void UpdateGameScore();
    void UpdateGameEffects(Tetra::MinoType prevMino, Tetra::Point2D prevPosition, Tetra::Rotation prevRotation, UserOperation userOperation,  int hardDropDistance);
    void UpdateGameSounds(UserOperation userOperation, bool userOperationSucceeded, bool levelUp);
    void UpdateGame();
    void UpdateSignals();
    void UpdateAnimeAndEffects();
    void UpdateMinoWaitState();

  public:
    // to suppress GCC warnings
    GameScene(const GameScene&) = delete;
    GameScene& operator=(const GameScene&) = delete;

    GameScene(SceneManager& sceneManager);
    ~GameScene();

    const Tetra::Game::Game& GetGame() const;
    bool GetExtremeMode() const;
    unsigned int GetLevel() const;
    std::uint_fast32_t GetScore() const;

    void Render();
    void Update();
  };
}   // namespace GameTetra
