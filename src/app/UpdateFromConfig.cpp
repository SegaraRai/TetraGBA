#include "UpdateFromConfig.hpp"
#include "Config.hpp"
#include "CopyVRAM.hpp"
#include "GameConfig.hpp"
#include "Song.hpp"
#include "Sound/MusicManager.hpp"

#include <gba.hpp>
#include <image/bg_background.hpp>


namespace Root {
  void PlayMusicFromConfig() {
    auto& musicManager = MusicManager::GetInstance();

    switch (GameConfig::GetGlobalConfig().music) {
      case GameConfig::Music::Tetris99:
        musicManager.Play(Song::tetrisa2);
        break;

      case GameConfig::Music::Wataten:
        musicManager.Play(Song::kimamanatenshitachi);
        break;

      case GameConfig::Music::NkPop:
        musicManager.Play(Song::gonggyeokjeonida);
        break;

      default:
        assert(false);
        break;
    }
  }


  void SetBGFromConfig() {
    switch (GameConfig::GetGlobalConfig().background) {
      case GameConfig::Background::Black:
        BGClearMap<Config::ScrBase::BG3>();
        break;

      case GameConfig::Background::Gray:
        BGCopyMap<Config::ScrBase::BG3>(Tile::bg_background::BGBackgroundGray::MapData);
        break;

      case GameConfig::Background::Flame:
        BGCopyMap<Config::ScrBase::BG3>(Tile::bg_background::BGBackgroundFlame::MapData);
        break;

      default:
        assert(false);
        break;
    }
  }

}
