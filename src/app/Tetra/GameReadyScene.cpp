#include "GameReadyScene.hpp"
#include "Config.hpp"
#include "GameScene.hpp"
#include "SceneManager.hpp"
#include "../CopyMap.hpp"
#include "../CopyVRAM.hpp"
#include "../DbgPrintf.hpp"
#include "../Sound.hpp"
#include "../SceneManager.hpp"
#include "../Sound/MusicManager.hpp"
#include "../Sound/SoundManager.hpp"

#include <gba.hpp>


namespace GameTetra {
  namespace {
    constexpr auto TargetFrameCount1 = Config::Ready::ReadyFrame;
    constexpr auto TargetFrameCount2 = TargetFrameCount1 + Config::Ready::IntervalFrame;
    constexpr auto TargetFrameCount3 = TargetFrameCount2 + Config::Ready::GoFrame;
  }


  GameReadyScene::GameReadyScene(SceneManager& sceneManager) :
    Scene(sceneManager),
    mFrameCount(0)
  {
    DbgPrintf("ctor of GameTetra::GameReadyScene\n");

    BGClearMap<Config::ScrBase::BG1Ready>();

    gba::reg::BG1CNT = gba::BGCNT::COLOR16 | gba::BGCNT::SIZE0 | gba::BGCNT::CHARBASE(Config::CharBase) | gba::BGCNT::SCRBASE(Config::ScrBase::BG1Ready) | Config::Priority::BG1;

    MusicManager::GetInstance().Stop();
  }


  GameReadyScene::~GameReadyScene() {
    DbgPrintf("dtor of GameTetra::GameReadyScene\n");

    gba::reg::BG1CNT = gba::BGCNT::COLOR16 | gba::BGCNT::SIZE0 | gba::BGCNT::CHARBASE(Config::CharBase) | gba::BGCNT::SCRBASE(Config::ScrBase::BG1Game) | Config::Priority::BG1;

    MusicManager::GetInstance().Resume();
  }


  void GameReadyScene::Render() {
    sceneManager.GameRender();

    // 盤面で操作中のミノがまだNEXTにあるように見せたいので、NEXTミノの表示を上書きする
    const auto& boardInfo = sceneManager.GetGameScene().GetGame().GetBoardInfo();
    for (unsigned int i = 0; i < Config::Board::NumNexts; i++) {
      const auto mino = i ? boardInfo.nextMinos[i - 1] : boardInfo.currentMino;
      const unsigned int minoTileIndex = Tile::obj::Mino::TileIndex + static_cast<unsigned int>(mino) * Tile::obj::Mino::MapWidth * Tile::obj::Mino::MapHeight;

      volatile auto& objAttr = gba::pointer_memory::OAM[Config::ObjId::NextMinoFirst + i];

      objAttr.attr0 = Tile::obj::Mino::Attribute0Base | gba::OBJATTR0::Y(Config::Position::NextY + Config::Position::NextH * i);
      objAttr.attr1 = Tile::obj::Mino::Attribute1Base | gba::OBJATTR1::X(Config::Position::NextX);
      objAttr.attr2 = gba::OBJATTR2::TILE(minoTileIndex) | Config::Priority::Object::Nexts | gba::OBJATTR2::PALETTE(Tile::obj::Mino::Palette);
    }

    if (mFrameCount == 0) {
      BGCopyMap<Config::ScrBase::BG1Ready, 32, Config::Ready::MapX, Config::Ready::MapY, Config::Ready::MapW>(Tile::bg::Ready::MapData);
    } else if (mFrameCount == TargetFrameCount1) {
      BGClearMap<Config::ScrBase::BG1Ready, 32, Config::Ready::MapX, Config::Ready::MapY, Config::Ready::MapW, Config::Ready::MapH>();
    } else if (mFrameCount == TargetFrameCount2) {
      BGCopyMap<Config::ScrBase::BG1Ready, 32, Config::Ready::MapX, Config::Ready::MapY, Config::Ready::MapW>(Tile::bg::Go::MapData);
    }
  }


  void GameReadyScene::Update() {
    if (mFrameCount == 0) {
      SoundManager::GetInstance().Play(SoundManager::Channel::B, Sound::tetra_ready);
    } else if (mFrameCount == TargetFrameCount2) {
      SoundManager::GetInstance().Play(SoundManager::Channel::B, Sound::tetra_go);
    }

    if (mFrameCount >= TargetFrameCount3) {
      SetScene(SceneId::Game);
    }

    mFrameCount++;
  }
}   // namespace GameTetra
