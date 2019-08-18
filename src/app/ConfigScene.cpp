#include "ConfigScene.hpp"
#include "Config.hpp"
#include "CopyVRAM.hpp"
#include "GameConfig.hpp"
#include "RotateEnum.hpp"
#include "SceneManager.hpp"
#include "Sound.hpp"
#include "TilePrint.hpp"
#include "UpdateFromConfig.hpp"
#include "Signal/KeyInputSignal.hpp"
#include "Signal/OneShotSignalDecorator.hpp"
#include "Signal/RepeatSignalDecorator.hpp"
#include "Signal/SignalBase.hpp"
#include "Sound/MusicManager.hpp"
#include "Sound/SoundManager.hpp"
#include "Tetra/Config.hpp"

#include <gba.hpp>
#include <image/bg.hpp>
#include <image/bg_background.hpp>

#include <cassert>
#include <cstdio>


namespace Root {
  ConfigScene::ConfigScene(SceneManager& sceneManager) :
    Scene(sceneManager),
    mSelection(Item::Mode)
  {
    gba::reg::DISPCNT = gba::DISPCNT::FORCE_BLANK;

    BGClearMap<Config::ScrBase::BG0Config>();

    gba::reg::BG0CNT = gba::BGCNT::COLOR16 | gba::BGCNT::SIZE0 | gba::BGCNT::CHARBASE(Config::CharBase) | gba::BGCNT::SCRBASE(Config::ScrBase::BG0Config) | Config::Priority::BG0;

    gba::reg::DISPCNT = gba::DISPCNT::BGMODE0 | gba::DISPCNT::BG0 | gba::DISPCNT::BG3;
  }


  void ConfigScene::Render() {
    BGClearMap<Config::ScrBase::BG0Config>();

    unsigned int y = Config::Config::MenuTextScreenY;
    unsigned int itemCount = 0;
    const auto PrintConfigLine = [this, &y, &itemCount] (const char* key, const char* value) {
      const auto selectArrow = itemCount == static_cast<unsigned int>(mSelection) ? '>' : ' ';

      static char buffer[32];
      std::snprintf(buffer, sizeof(buffer), "%5s  %c%s", key, selectArrow, value);
      TilePrint<Config::ScrBase::BG0Config>(buffer, Config::Config::MenuTextScreenX, y);
      itemCount++;
      y += 2;
    };

    const auto& globalConfig = GameConfig::GetGlobalConfig();

    // MODE
    const char* modeStrings[static_cast<unsigned int>(GameConfig::Mode::End)] = {
      "150 LINES",
      "999 LINES",
    };
    PrintConfigLine("MODE", modeStrings[static_cast<unsigned int>(globalConfig.mode)]);

    // LEVEL
    char levelString[16];
    if (globalConfig.extreme) {
      std::snprintf(levelString, sizeof(levelString), "EX");
    } else {
      std::snprintf(levelString, sizeof(levelString), "%02d", globalConfig.level);
    }
    PrintConfigLine("LEVEL", levelString);

    // MUSIC
    const char* musicStrings[static_cast<unsigned int>(GameConfig::Music::End)] = {
      "TETRIS 99",
      "WATATEN",
      "NK-POP",
    };
    PrintConfigLine("MUSIC", musicStrings[static_cast<unsigned int>(globalConfig.music)]);

    // BG
    const char* bgStrings[static_cast<unsigned int>(GameConfig::Background::End)] = {
      "GRAY",
      "FLAME",
      "BLACK",
    };
    PrintConfigLine("BG", bgStrings[static_cast<unsigned int>(globalConfig.background)]);

    //
    y += 2;
    PrintConfigLine("", "RETURN");
  }


  void ConfigScene::Update() {
    if (sceneManager.oneShotKeyInputSelect->GetState()) {
      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_decide);
      SetScene(SceneId::Title);
      return;
    }

    if (sceneManager.oneShotKeyInputA->GetState() && mSelection == Item::Return) {
      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_decide);
      SetScene(SceneId::Title);
      return;
    }

    /*
    if (sceneManager.oneShotKeyInputB->GetState()) {
      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_decide);
      SetScene(SceneId::Title);
      return;
    }
    //*/


    auto& globalConfig = GameConfig::GetGlobalConfig();


    if (sceneManager.repeatKeyInputRight->GetState()) {
      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_select);

      switch (mSelection) {
        case Item::Mode:
          RotateEnumRight(globalConfig.mode, globalConfig.easterEgg ? GameConfig::Mode::End : GameConfig::Mode::HiddenSince);
          break;

        case Item::Level:
          globalConfig.level++;
          if (globalConfig.level > GameTetra::Config::Board::MaxLevel) {
            globalConfig.level = GameTetra::Config::Board::MaxLevel;
          }
          break;

        case Item::Music:
          RotateEnumRight(globalConfig.music, globalConfig.easterEgg ? GameConfig::Music::End : GameConfig::Music::HiddenSince);
          PlayMusicFromConfig();
          break;

        case Item::Background:
          RotateEnumRight(globalConfig.background, globalConfig.easterEgg ? GameConfig::Background::End : GameConfig::Background::HiddenSince);
          SetBGFromConfig();
          break;

        default:
          // do nothing; just for supressing warnings
          break;
      }
    }

    if (sceneManager.repeatKeyInputLeft->GetState()) {
      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_select);

      switch (mSelection) {
        case Item::Mode:
          RotateEnumLeft(globalConfig.mode, GameConfig::Mode::End);
          break;

        case Item::Level:
          globalConfig.level--;
          if (globalConfig.level < 1) {
            globalConfig.level = 1;
          }
          break;

        case Item::Music:
          RotateEnumLeft(globalConfig.music, globalConfig.easterEgg ? GameConfig::Music::End : GameConfig::Music::HiddenSince);
          PlayMusicFromConfig();
          break;

        case Item::Background:
          RotateEnumLeft(globalConfig.background, globalConfig.easterEgg ? GameConfig::Background::End : GameConfig::Background::HiddenSince);
          SetBGFromConfig();
          break;

        default:
          // do nothing; just for supressing warnings
          break;
      }
    }

    if (sceneManager.repeatKeyInputUp->GetState()) {
      if (mSelection == Item::Music) {
        MusicManager::GetInstance().Stop();
      }

      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_cursor);
      RotateEnumLeft(mSelection, Item::End);

      if (mSelection == Item::Music) {
        PlayMusicFromConfig();
      }
    }

    if (sceneManager.repeatKeyInputDown->GetState()) {
      if (mSelection == Item::Music) {
        MusicManager::GetInstance().Stop();
      }

      SoundManager::GetInstance().Play(SoundManager::Channel::A, Sound::common_cursor);
      RotateEnumRight(mSelection, Item::End);

      if (mSelection == Item::Music) {
        PlayMusicFromConfig();
      }
    }
  }
}   // namespace Root
