#include "TitleScene.hpp"
#include "Config.hpp"
#include "CopyVRAM.hpp"
#include "DbgPrintf.hpp"
#include "RotateEnum.hpp"
#include "SceneManager.hpp"
#include "Sound.hpp"
#include "TilePrint.hpp"
#include "Signal/KeyInputSignal.hpp"
#include "Signal/OneShotSignalDecorator.hpp"
#include "Signal/RepeatSignalDecorator.hpp"
#include "Signal/SignalBase.hpp"
#include "Sound/SoundManager.hpp"

#include <gba.hpp>
#include <image/bg.hpp>

#include <cassert>
#include <cstdio>


namespace Root {
  TitleScene::TitleScene(SceneManager& sceneManager) :
    Scene(sceneManager),
    mSelection(Item::NewGame)
  {
    gba::reg::DISPCNT = gba::DISPCNT::FORCE_BLANK;

    BGCopyMap<Config::ScrBase::BG0Title>(Tile::bg::BGTitle::MapData);

    gba::reg::BG0CNT = gba::BGCNT::COLOR16 | gba::BGCNT::SIZE0 | gba::BGCNT::CHARBASE(Config::CharBase) | gba::BGCNT::SCRBASE(Config::ScrBase::BG0Title) | Config::Priority::BG0;

    gba::reg::DISPCNT = gba::DISPCNT::BGMODE0 | gba::DISPCNT::BG0 | gba::DISPCNT::BG3;
  }


  void TitleScene::Render() {
    BGCopyMap<Config::ScrBase::BG0Title>(Tile::bg::BGTitle::MapData);

    unsigned int y = Config::Title::MenuTextScreenY;
    unsigned int itemCount = 0;
    const auto PrintMenuLine = [this, &y, &itemCount] (const char* key) {
      const auto selectArrow = itemCount == static_cast<unsigned int>(mSelection) ? '>' : ' ';

      static char buffer[16];
      std::snprintf(buffer, sizeof(buffer), "%c %s", selectArrow, key);
      TilePrint<Config::ScrBase::BG0Title>(buffer, Config::Title::MenuTextScreenX, y);
      itemCount++;
      y += 2;
    };

    PrintMenuLine("NEW GAME");
    PrintMenuLine("CONFIG");
  }


  void TitleScene::Update() {
    if (sceneManager.oneShotKeyInputStart->GetState()) {
      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_decide);
      SetScene(SceneId::GameTetra);
      return;
    }

    if (sceneManager.oneShotKeyInputSelect->GetState()) {
      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_decide);
      SetScene(SceneId::Config);
      return;
    }

    if (sceneManager.oneShotKeyInputA->GetState()) {
      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_decide);
      switch (mSelection) {
        case Item::NewGame:
          SetScene(SceneId::GameTetra);
          return;

        case Item::Config:
          SetScene(SceneId::Config);
          return;

        default:
          assert(false);
          break;
      }
    }

    if (sceneManager.repeatKeyInputUp->GetState()) {
      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_cursor);
      RotateEnumLeft(mSelection, Item::End);
    }

    if (sceneManager.repeatKeyInputDown->GetState()) {
      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_cursor);
      RotateEnumRight(mSelection, Item::End);
    }
  }
}   // namespace Root
