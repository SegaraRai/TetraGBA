#include "SceneManager.hpp"
#include "ConfigScene.hpp"
#include "Config.hpp"
#include "CopyVRAM.hpp"
#include "GameConfig.hpp"
#include "GameTetraScene.hpp"
#include "GameTetraRestartScene.hpp"
#include "MergePalette.hpp"
#include "Scene.hpp"
#include "Sound.hpp"
#include "TitleScene.hpp"
#include "UpdateFromConfig.hpp"
#include "Signal/KeyInputSignal.hpp"
#include "Signal/OneShotSignalDecorator.hpp"
#include "Signal/RepeatSignalDecorator.hpp"
#include "Signal/SignalBase.hpp"
#include "Sound/SoundManager.hpp"

#include <gba.hpp>
#include <image/bg.hpp>
#include <image/bg_background.hpp>
#include <image/obj.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <memory>


namespace Root {
  SceneManager::~SceneManager() = default;


  std::unique_ptr<Scene> SceneManager::CreateSceneFromSceneId(SceneId sceneId) {
    switch (sceneId) {
      case SceneId::Title:
        return std::make_unique<TitleScene>(*this);

      case SceneId::Config:
        return std::make_unique<ConfigScene>(*this);

      case SceneId::GameTetra:
        return std::make_unique<GameTetraScene>(*this);

      case SceneId::GameTetraRestart:
        return std::make_unique<GameTetraRestartScene>(*this);
    }
    assert(false);
    return nullptr;
  }


  SceneManager& SceneManager::GetInstance() {
    static SceneManager sceneManager(InitialSceneId);
    return sceneManager;
  }


  SceneManager::SceneManager(SceneId initialSceneId) :
    mSceneId(initialSceneId),
    mPtrScene(),
    oneShotKeyInputA(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYINPUT::A))),
    oneShotKeyInputB(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYINPUT::B))),
    oneShotKeyInputSelect(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYINPUT::SELECT))),
    oneShotKeyInputStart(
      std::make_unique<OneShotSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYINPUT::START))),
    repeatKeyInputRight(
      std::make_unique<RepeatSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYINPUT::RIGHT), Config::ArrowKeyDelay, Config::ArrowKeyInterval)),
    repeatKeyInputLeft(
      std::make_unique<RepeatSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYINPUT::LEFT), Config::ArrowKeyDelay, Config::ArrowKeyInterval)),
    repeatKeyInputUp(
      std::make_unique<RepeatSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYINPUT::UP), Config::ArrowKeyDelay, Config::ArrowKeyInterval)),
    repeatKeyInputDown(
      std::make_unique<RepeatSignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYINPUT::DOWN), Config::ArrowKeyDelay, Config::ArrowKeyInterval)),
    konamiCommandSignal()
  {
    gba::reg::TM2CNT_L = 0;
    gba::reg::TM3CNT_L = 0;

    gba::reg::TM2CNT_H = gba::TMCNT_H::OPERATE | gba::TMCNT_H::TIMING_1;
    gba::reg::TM3CNT_H = gba::TMCNT_H::OPERATE | gba::TMCNT_H::COUNTUP;

    gba::reg::DISPCNT = gba::DISPCNT::FORCE_BLANK;

    BGCopyPalette(Config::MergedPaletteData);
    BGCopyTile<Config::CharBase>(Tile::bg::TileData);
    BGCopyTile<Config::BG3CharBase>(Tile::bg_background::TileData);
    BGCopyMap<Config::ScrBase::BG3>(Tile::bg_background::BGBackgroundGray::MapData);

    OBJCopyPalette(Tile::obj::PaletteData);
    OBJCopyTile(Tile::obj::TileData);

    gba::reg::BG3CNT = gba::BGCNT::COLOR16 | gba::BGCNT::SIZE0 | gba::BGCNT::CHARBASE(Config::BG3CharBase) | gba::BGCNT::SCRBASE(Config::ScrBase::BG3) | Config::Priority::BG3;

    // delay initialization
    mPtrScene = CreateSceneFromSceneId(initialSceneId);

    // enable interrupts
    gba::reg::IME = gba::IME::ENABLE;
  }


  void SceneManager::SetScene(SceneId sceneId) {
    // ensure that the old scene is destructed before the new scene is created
    mPtrScene.reset();
    mSceneId = sceneId;
    mPtrScene = CreateSceneFromSceneId(sceneId);
  }


  void SceneManager::Render() {
    mPtrScene->Render();
  }


  void SceneManager::Update() {
    mPtrScene->Update();

    if (mSceneId != SceneId::GameTetra  && konamiCommandSignal.GetState()) {
      if (GameConfig::GetGlobalConfig().easterEgg) {
        GameConfig::GetGlobalConfig() = Config::DefaultConfig;
        SetBGFromConfig();
        SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_secret_disable);
      } else {
        GameConfig::GetGlobalConfig().easterEgg = true;
        GameConfig::GetGlobalConfig().extreme = true;
        GameConfig::GetGlobalConfig().background = GameConfig::Background::Black;
        SetBGFromConfig();
        SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_secret_enable);
      }
    }

    oneShotKeyInputA->Step();
    oneShotKeyInputB->Step();
    oneShotKeyInputSelect->Step();
    oneShotKeyInputStart->Step();
    repeatKeyInputRight->Step();
    repeatKeyInputLeft->Step();
    repeatKeyInputUp->Step();
    repeatKeyInputDown->Step();

    konamiCommandSignal.Step();

    gba::bios::VBlankIntrWait();
  }
}   // namespace Root
