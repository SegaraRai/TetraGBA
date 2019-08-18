#include "GameEndSceneBase.hpp"
#include "Config.hpp"
#include "GameScene.hpp"
#include "SceneManager.hpp"
#include "../CopyMap.hpp"
#include "../CopyVRAM.hpp"
#include "../DbgPrintf.hpp"
#include "../SceneManager.hpp"
#include "../Song.hpp"
#include "../Sound.hpp"
#include "../TilePrint.hpp"
#include "../Signal/KeyInputSignal.hpp"
#include "../Signal/OneShotSignalDecorator.hpp"
#include "../Signal/SignalBase.hpp"
#include "../Sound/MusicManager.hpp"
#include "../Sound/SoundManager.hpp"

#include <memory>
#include <gba.hpp>
#include <image/bg.hpp>
#include <image/obj.hpp>


namespace GameTetra {
  GameEndSceneBase::GameEndSceneBase(SceneManager& sceneManager, Type type) :
    Scene(sceneManager),
    mType(type),
    mPage(0),
    mFrameCountForKeyInput(0),
    mKeyInputRight(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYINPUT::RIGHT))),
    mKeyInputLeft(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYINPUT::LEFT)))
  {
    DbgPrintf("ctor of GameTetra::GameEndSceneBase\n");

    gba::reg::BLDY = gba::BLDY::EVYCOEF(Config::ClearOver::BrightnessCoef);
    gba::reg::BLDCNT = gba::BLDCNT::FIRST_BG1 | gba::BLDCNT::FIRST_BG2 | gba::BLDCNT::FIRST_BG3 | gba::BLDCNT::FIRST_OBJ | gba::BLDCNT::BRDEC;

    BGClearMap<Config::ScrBase::BG0Result>();
    BGCopyMap<Config::ScrBase::BG0Result, 32, Config::ClearOver::GameClearOverX, Config::ClearOver::GameClearOverY, Tile::bg::GameOver::MapWidth>(type == Type::GameClear ? Tile::bg::GameClear::MapData : Tile::bg::GameOver::MapData);

    gba::reg::BG0HOFS = 0;
    gba::reg::BG0VOFS = 0;
    gba::reg::BG0CNT = gba::BGCNT::COLOR16 | gba::BGCNT::SIZE0 | gba::BGCNT::CHARBASE(Config::CharBase) | gba::BGCNT::SCRBASE(Config::ScrBase::BG0Result) | Config::Priority::BG0;

    MusicManager::GetInstance().Stop();
    SoundManager::GetInstance().Stop();

    MusicManager::GetInstance().Play(Song::gameover);

    sceneManager.GameRender();
  }


  GameEndSceneBase::~GameEndSceneBase() {
    DbgPrintf("dtor of GameTetra::GameEndSceneBase\n");

    gba::reg::BLDCNT = gba::BLDCNT::NONE;

    MusicManager::GetInstance().Stop();
    SoundManager::GetInstance().Stop();
  }


  void GameEndSceneBase::Render() {
    const auto& statistics = sceneManager.GetGameScene().GetGame().GetGameStatistics();

    unsigned int y = Config::ClearOver::StatsY;
    const auto PrintResultLine = [&y] (const char* key, Tetra::Game::GameStatistics::Count value, const char* spec = "%-13s %4d") {
      static char buffer[32];
      std::snprintf(buffer, sizeof(buffer), spec, key, static_cast<unsigned int>(value));
      TilePrint<Config::ScrBase::BG0Result>(buffer, Config::ClearOver::StatsX, y);
      y++;
    };

    BGClearMap<Config::ScrBase::BG0Result, 32, 0, Config::ClearOver::StatsY, gba::lcd::TileVirtualWidth, gba::lcd::TileVirtualHeight - Config::ClearOver::StatsY>();

    PrintResultLine("Score", sceneManager.GetGameScene().GetScore(), "%s %12d");
    if (sceneManager.GetGameScene().GetExtremeMode()) {
      PrintResultLine("Level", 0, "%-10s EXTREME");
    } else {
      PrintResultLine("Level", sceneManager.GetGameScene().GetLevel());
    }
    PrintResultLine("Total Lines", statistics.numClearedLines);
    y += 2;

    switch (mPage) {
      case 0:
        PrintResultLine("Single", statistics.numSingles);
        PrintResultLine("Double", statistics.numDoubles);
        PrintResultLine("Triple", statistics.numTriples);
        PrintResultLine("Tetris", statistics.numQuadruples);
        y++;

        PrintResultLine("Max REN", statistics.numMaxRens);
        PrintResultLine("Back-to-Back", statistics.numTotalBackToBacks);
        PrintResultLine("Perfect Clear", statistics.numPerfectClears);
        y++;
        break;

      case 1:
        PrintResultLine("T-Spin", statistics.numTSpinZeros);
        PrintResultLine("T-Spin Mini", statistics.numTSpinMiniZeros + statistics.numTSpinMiniSingles + statistics.numTSpinMiniDoubles);
        PrintResultLine("T-Spin Single", statistics.numTSpinSingles);
        PrintResultLine("T-Spin Double", statistics.numTSpinDoubles);
        PrintResultLine("T-Spin Triple", statistics.numTSpinTriples);
        y++;

        PrintResultLine("T-Spin Total", statistics.numAllTSpins);
        y++;
        break;
    }
  }


  void GameEndSceneBase::Update() {
    // press START to restart game
    // press SELECT to go menu

    if (mFrameCountForKeyInput < Config::ClearOver::KeyInputEnableDelay) {
      mFrameCountForKeyInput++;
    } else {
      if (Root::SceneManager::GetInstance().oneShotKeyInputSelect->GetState()) {
        Root::SceneManager::GetInstance().SetScene(Root::SceneId::Title);
        return;
      }

      if (Root::SceneManager::GetInstance().oneShotKeyInputStart->GetState()) {
        Root::SceneManager::GetInstance().SetScene(Root::SceneId::GameTetraRestart);
        return;
      }

      if (mKeyInputRight->GetState()) {
        mPage++;
        if (mPage == NumPages) {
          mPage = 0;
        }
      }

      if (mKeyInputLeft->GetState()) {
        if (mPage == 0) {
          mPage = NumPages - 1;
        } else {
          mPage--;
        }
      }
    }

    mKeyInputRight->Step();
    mKeyInputLeft->Step();
  }
}   // namespace GameTetra
