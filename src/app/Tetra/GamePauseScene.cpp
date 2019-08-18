#include "GamePauseScene.hpp"
#include "SceneManager.hpp"
#include "Config.hpp"
#include "../DbgPrintf.hpp"
#include "../SceneManager.hpp"
#include "../Signal/DelaySignalDecorator.hpp"
#include "../Signal/KeyInputSignal.hpp"

#include <gba.hpp>


namespace GameTetra {
  GamePauseScene::GamePauseScene(SceneManager& sceneManager) :
    Scene(sceneManager),
    mSOUNDCNT_L(gba::reg::SOUNDCNT_L),
    mSelectKeyInput(
      std::make_unique<DelaySignalDecorator>(
        std::make_unique<KeyInputSignal>(gba::KEYINPUT::SELECT), Config::Pause::SelectHoldFrame))
  {
    DbgPrintf("ctor of GameTetra::GamePauseScene\n");

    gba::reg::SOUNDCNT_L = (gba::reg::SOUNDCNT_L & 0xFF00) | gba::SOUNDCNT_L::VOLUMERL(Config::Pause::SoundVolume);
    gba::reg::BG1CNT = gba::BGCNT::COLOR16 | gba::BGCNT::SIZE0 | gba::BGCNT::CHARBASE(Config::CharBase) | gba::BGCNT::SCRBASE(Config::ScrBase::BG1Pause) | Config::Priority::BG1;
    gba::reg::DISPCNT &= ~gba::DISPCNT::OBJ;

    gba::reg::BLDY = gba::BLDY::EVYCOEF(Config::Pause::BrightnessCoef);
    gba::reg::BLDCNT = gba::BLDCNT::FIRST_BG0 | gba::BLDCNT::FIRST_BG2 | gba::BLDCNT::FIRST_BG3 | gba::BLDCNT::FIRST_OBJ | gba::BLDCNT::BRDEC;

    //Render();
  }


  GamePauseScene::~GamePauseScene() {
    DbgPrintf("dtor of GameTetra::GamePauseScene\n");

    gba::reg::SOUNDCNT_L = mSOUNDCNT_L;
    gba::reg::BG1CNT = gba::BGCNT::COLOR16 | gba::BGCNT::SIZE0 | gba::BGCNT::CHARBASE(Config::CharBase) | gba::BGCNT::SCRBASE(Config::ScrBase::BG1Game) | Config::Priority::BG1;
    gba::reg::DISPCNT |= gba::DISPCNT::OBJ;
    gba::reg::BLDCNT = gba::BLDCNT::NONE;
  }


  void GamePauseScene::Render() {}


  void GamePauseScene::Update() {
    // press START to restart game
    // press SELECT to go menu

    if (Root::SceneManager::GetInstance().oneShotKeyInputStart->GetState()) {
      // resume
      SetScene(SceneId::Game);
      return;
    }

    if (mSelectKeyInput->GetState()) {
      // go to title
      Root::SceneManager::GetInstance().SetScene(Root::SceneId::Title);
      return;
    }

    mSelectKeyInput->Step();
  }
}   // namespace GameTetra
