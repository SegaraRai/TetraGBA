#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Config.hpp"
#include "GameScene.hpp"
#include "GameClearScene.hpp"
#include "GameOverScene.hpp"
#include "GamePauseScene.hpp"
#include "GameReadyScene.hpp"
#include "../CopyVRAM.hpp"
#include "../DbgPrintf.hpp"
#include "../Sound/MusicManager.hpp"
#include "../Sound/SoundManager.hpp"
#include <image/bg.hpp>
#include <image/bg_background.hpp>
#include <image/obj.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>

#include <gba.hpp>


namespace GameTetra {
  std::unique_ptr<Scene> SceneManager::CreateOverrideSceneFromSceneId(SceneId sceneId) {
    switch (sceneId) {
      case SceneId::Game:
        return nullptr;

      case SceneId::GameClear:
        return std::make_unique<GameClearScene>(*this);

      case SceneId::GameOver:
        return std::make_unique<GameOverScene>(*this);

      case SceneId::GamePause:
        return std::make_unique<GamePauseScene>(*this);

      case SceneId::GameReady:
        return std::make_unique<GameReadyScene>(*this);
    }
    assert(false);
    return nullptr;
  }


  SceneManager::SceneManager() :
    SceneManager(InitialSceneId)
  {}


  SceneManager::SceneManager(SceneId initialSceneId) :
    mPtrGameScene(),
    mPtrOverrideScene()
  {
    DbgPrintf("ctor of GameTetra::SceneManager\n");

    gba::reg::DISPCNT = gba::DISPCNT::FORCE_BLANK;

    BGClearMap<Config::ScrBase::BG0Score>();
    BGClearMap<Config::ScrBase::BG1Game>();
    BGCopyMap<Config::ScrBase::BG1Pause>(Tile::bg::BGPause::MapData);
    BGCopyMap<Config::ScrBase::BG2>(Tile::bg::BGFrame::MapData);

    gba::reg::BG0CNT = gba::BGCNT::COLOR16 | gba::BGCNT::SIZE0 | gba::BGCNT::CHARBASE(Config::CharBase) | gba::BGCNT::SCRBASE(Config::ScrBase::BG0Score) | Config::Priority::BG0;
    gba::reg::BG1CNT = gba::BGCNT::COLOR16 | gba::BGCNT::SIZE0 | gba::BGCNT::CHARBASE(Config::CharBase) | gba::BGCNT::SCRBASE(Config::ScrBase::BG1Game) | Config::Priority::BG1;
    gba::reg::BG2CNT = gba::BGCNT::COLOR16 | gba::BGCNT::SIZE0 | gba::BGCNT::CHARBASE(Config::CharBase) | gba::BGCNT::SCRBASE(Config::ScrBase::BG2) | Config::Priority::BG2;
    gba::reg::DISPCNT = gba::DISPCNT::BGMODE0 | gba::DISPCNT::BG0 | gba::DISPCNT::BG1 | gba::DISPCNT::BG2 | gba::DISPCNT::BG3 | gba::DISPCNT::OBJ | gba::DISPCNT::OBJMAP1D;

    gba::reg::BG0HOFS = Config::Position::ScoreScrollX;
    gba::reg::BG0VOFS = Config::Position::ScoreScrollY;

    mPtrGameScene = std::make_unique<GameScene>(*this);

    SetScene(initialSceneId);
  }


  SceneManager::~SceneManager() {
    DbgPrintf("dtor of GameTetra::SceneManager\n");

    mPtrOverrideScene.reset();
    mPtrGameScene.reset();

    gba::reg::BG0HOFS = 0;
    gba::reg::BG0VOFS = 0;
  }


  void SceneManager::SetScene(SceneId sceneId) {
    // ensure that the old scene is destructed before the new scene is created
    mPtrOverrideScene.reset();
    mPtrOverrideScene = CreateOverrideSceneFromSceneId(sceneId);
  }


  GameScene& SceneManager::GetGameScene() {
    return *mPtrGameScene;
  }


  void SceneManager::GameRender() {
    mPtrGameScene->Render();
  }


  void SceneManager::GameUpdate() {
    mPtrGameScene->Update();
  }


  void SceneManager::Render() {
    if (mPtrOverrideScene) {
      mPtrOverrideScene->Render();
    } else {
      GameRender();
    }
  }


  void SceneManager::Update() {
    if (mPtrOverrideScene) {
      mPtrOverrideScene->Update();
    } else {
      GameUpdate();
    }
  }
}   // namespace GameTetra
