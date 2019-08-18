#pragma once

#include <cstddef>
#include <cstdint>

#include <image/bg.hpp>
#include <image/obj.hpp>


namespace GameTetra::Config {
#ifndef RELEASE_BUILD
  namespace Debug {
    enum class DebugBoardType {
      None = 0,
      DoubleQuad,
      QuadTST,
      DTPC,
      REN,
    };

    constexpr DebugBoardType DebugBoard = DebugBoardType::None;
  }   // namespace Debug
#endif


  namespace Board {
    constexpr unsigned int Border = 1;

    constexpr unsigned int Width = 10;
    constexpr unsigned int Height = 40;
    constexpr unsigned int BaseY = 20;
    constexpr unsigned int VisibleHeight = Height - BaseY;

    constexpr unsigned int WidthIncludingBorder = Width + Border * 2;
    constexpr unsigned int HeightIncludingBorder = Height + Border * 2;
    constexpr unsigned int BaseYIncludingBorder = BaseY + Border;

    constexpr std::size_t NumNexts = 6;

    // ライン消去アニメの全コマ数
    constexpr unsigned int NumLineClearAnimationFrames = 3;

    // 接地してから強制的に固定するまでの（LockDelayが作用しなくなるまでの）操作回数
    constexpr unsigned int MaxOperationsAfterLand = 15;

    // 最大レベル
    constexpr unsigned int MaxLevel = 15;

    // 次のレベルに到達するのに必要なレベル
    constexpr unsigned int LinesToNextLevel = 10;
  }   // namespace Board

  constexpr std::uint16_t CharBase = 0;

  namespace ScrBase {
    // 0 ~ 7 are reserved for Tile 0
    // 8 ~ 23 are reserved for Tile 1
    // 24 is reserved for BG3

    constexpr std::uint16_t BG0Score  = 25;
    constexpr std::uint16_t BG0Result = 25;
    constexpr std::uint16_t BG1Game   = 26;
    constexpr std::uint16_t BG1Ready  = 27;
    constexpr std::uint16_t BG1Pause  = 28;
    constexpr std::uint16_t BG2       = 29;   // Frame
  }   // namespace ScrBase

  namespace ObjId {
    // 10 ~ 13 : Perfect Clear, Back To Back, Tetris

    constexpr std::uint16_t PerfectClearLeft  = 10;
    constexpr std::uint16_t PerfectClearRight = 11;
    constexpr std::uint16_t BackToBack        = 12;
    constexpr std::uint16_t Tetris            = 13;

    // 15 ~ 18 : REN
    namespace Ren {
      constexpr std::uint16_t Ren     = 15;
      constexpr std::uint16_t Digit1  = 16;
      constexpr std::uint16_t Digit21 = 17;
      constexpr std::uint16_t Digit22 = 18;
    }   // namespace Ren

    // 20 ~ 22 : T-Spin

    namespace TSpin {
      constexpr std::uint16_t TSpin   = 20;
      constexpr std::uint16_t Mini    = 21;
      constexpr std::uint16_t SDT     = 22;
    }   // namespace TSpin

    // 30 ~ 62 : LineClear Animation

    constexpr std::uint16_t LineClearFirstLeft        = 30;
    constexpr std::uint16_t LineClearFirstRight       = 31;
    constexpr std::uint16_t LineClearFirstMiddleFirst = 32;
    constexpr std::uint16_t NumLineClearMiddle        = 5;
    constexpr std::uint16_t LineClearDiff             = 8;

    // 80 : Hold Mino

    constexpr std::uint16_t HoldMino      = 80;

    // 90 ~ 95 : Next Minos

    constexpr std::uint16_t NextMinoFirst = 90;
  }   // namespace ObjId

  namespace Priority {
    inline namespace BG {
      constexpr std::uint16_t BG0 = gba::BGCNT::PRIORITY0;
      constexpr std::uint16_t BG1 = gba::BGCNT::PRIORITY1;
      constexpr std::uint16_t BG2 = gba::BGCNT::PRIORITY2;
      constexpr std::uint16_t BG3 = gba::BGCNT::PRIORITY3;
    }   // inline namespace BG

    namespace Object {
      constexpr std::uint16_t Hold           = gba::OBJATTR2::PRIORITY1;
      constexpr std::uint16_t Nexts          = gba::OBJATTR2::PRIORITY1;
      constexpr std::uint16_t Effects        = gba::OBJATTR2::PRIORITY1;
      constexpr std::uint16_t LineClearAnime = gba::OBJATTR2::PRIORITY0;
    }   // namespace Object
  }   // namespace Priority

  // 60 FPS
  namespace Frame {
    // 落下間隔（分数、分子→分母の順）
    // Tetris 99 基準
    constexpr std::array<std::pair<unsigned int, unsigned int>, Board::MaxLevel + 1> Fall{
      std::make_pair(0u,  0u),    // レベル0:   未使用
      std::make_pair(60u, 1u),    // レベル1:   60フレームに1マス
      std::make_pair(48u, 1u),    // レベル2:   48フレームに1マス
      std::make_pair(38u, 1u),    // レベル3:   38フレームに1マス
      std::make_pair(29u, 1u),    // レベル4:   29フレームに1マス
      std::make_pair(22u, 1u),    // レベル5:   22フレームに1マス
      std::make_pair(16u, 1u),    // レベル6:   16フレームに1マス
      std::make_pair(12u, 1u),    // レベル7:   12フレームに1マス
      std::make_pair(9u,  1u),    // レベル8:    9フレームに1マス
      std::make_pair(6u,  1u),    // レベル9:    6フレームに1マス
      std::make_pair(4u,  1u),    // レベル10:   4フレームに1マス
      std::make_pair(8u,  3u),    // レベル11: 8/3フレームに1マス
      std::make_pair(2u,  1u),    // レベル12:   2フレームに1マス
      std::make_pair(2u,  1u),    // レベル13:   2フレームに1マス  レベル12との違いが不明
      std::make_pair(1u,  1u),    // レベル14:   1フレームに1マス
      std::make_pair(1u,  2u),    // レベル15: 1/2フレームに1マス  つまり1フレームに2マス
    };

    // 20G
    constexpr auto ExtremeFall = std::make_pair(1u, 20u);

    // ライン消去アニメの1フレームあたりの表示時間
    constexpr unsigned int LineClearAnime = 3;

    // ライン消去から次のミノが出現するまでの時間（NextMinoの分を含む）
    // Tetris DS based (https://harddrop.com/wiki/Tetris_DS)
    constexpr unsigned int LineClearWait = 40;

    // ミノを置いてから次のミノが出現するまでの時間
    // a.k.a ARE (c.f. https://harddrop.com/wiki/ARE)
    // Tetris DS based (https://harddrop.com/wiki/Tetris_DS)
    constexpr unsigned int NextMino = 0;

    // ハードドロップエフェクトの表示時間
    // タイルで表示しているため、場合によっては変になる
    constexpr unsigned int HardDropEffect = 10;

    // 接地してから固定されるまでの遊び時間
    // Tetris DS based (https://harddrop.com/wiki/Tetris_DS)
    constexpr unsigned int LockDelay = 30;

    namespace Key {
      // 十字キー用の入力遅延時間（誤入力防止）
      constexpr unsigned int ArrowKeyDelay = 1;

      // Left / Right
      // a.k.a DAS (c.f. https://harddrop.com/wiki/DAS)
      constexpr unsigned int HorizontalMoveDelay    = 10;
      constexpr unsigned int HorizontalMoveInterval = 2;

      // Down
      constexpr unsigned int VerticalMoveDelay    = 4;
      constexpr unsigned int VerticalMoveInterval = VerticalMoveDelay;

      // ミノが出現してしばらくはハードドロップ入力を無視する（誤入力防止）
      constexpr unsigned int HardDropEnableWait = 10;

      // ミノが出現してしばらくはホールド入力を無視する（誤入力防止）
      constexpr unsigned int HoldEnableWait = 10;

      // Up (HardDrop), A/B (Rotation) and L/R (Hold) are one-shot
    }   // namespace Key

    namespace Effect {
      constexpr unsigned int TSpinAlive = 120;

      constexpr unsigned int TetrisAlive = 120;

      // RENが途切れるまで表示しっぱなしにする
      constexpr unsigned int RenAlive = 0;

      constexpr unsigned int BackToBackAlive = 120;

      constexpr unsigned int PerfectClearAlive = 180;
    }   // namespace Effect
  }   // namespace Frame

  namespace Position {
    // スコアテキストの開始（左上）座標X、8x8単位
    constexpr unsigned int ScoreScreenX = 1;
    // スコアテキストの開始（左上）座標Y、8x8単位
    constexpr unsigned int ScoreScreenY = 14;

    // スコアBGのスクロールX
    constexpr unsigned int ScoreScrollX = 4;
    // スコアBGのスクロールY
    constexpr unsigned int ScoreScrollY = 4;

    constexpr unsigned int HoldX = 84;
    constexpr unsigned int HoldY = 17;

    constexpr unsigned int NextX = 195;
    constexpr unsigned int NextY = 17;
    constexpr unsigned int NextH = 24;

    // 盤面の開始（左上）座標X、8x8単位
    constexpr unsigned int GameScreenX = 14;
    // 盤面の開始（左上）座標Y、8x8単位
    constexpr unsigned int GameScreenY = 0;

    constexpr unsigned int LineClearLeftX = (GameScreenX - 2) * 8;
    constexpr unsigned int LineClearMiddleStartX = GameScreenX * 8;
    constexpr unsigned int LineClearRightX = (GameScreenX + Board::Width) * 8;

    namespace Effect {
      constexpr unsigned int PerfectClearLeftX = 117;
      constexpr unsigned int PerfectClearLeftY = 61;

      constexpr unsigned int PerfectClearRightX = PerfectClearLeftX + Tile::obj::PerfectClearLeft::Width * 8;
      constexpr unsigned int PerfectClearRightY = PerfectClearLeftY;

      constexpr unsigned int MiniX = 28;
      constexpr unsigned int MiniY = 36;

      constexpr unsigned int TSpinX = 20;
      constexpr unsigned int TSpinY = 46;

      constexpr unsigned int TSpinSDTX = 24;
      constexpr unsigned int TSpinSDTY = 61;

      constexpr unsigned int TetrisX = 22;
      constexpr unsigned int TetrisY = 46;

      constexpr unsigned int RenX = 27;
      constexpr unsigned int RenY = 16;

      constexpr unsigned int RenDigit1X  = 37;
      constexpr unsigned int RenDigit21X = 32;    // 先頭の桁は1（または2）なので、見栄えが良いようにRenDigit1Xから-4ではなく-5としておく
      constexpr unsigned int RenDigit22X = 42;
      constexpr unsigned int RenDigitY   = 4;

      constexpr unsigned int BackToBackX = 14;
      constexpr unsigned int BackToBackY = 86;
    }   // namespace Effect
  }   // namespace Position

  namespace Score {
    // cf. "Points(2009)" on https://harddrop.com/wiki/Scoring#Guideline_scoring_system
    // BackToBackBonusとLevelBonusの適用順は、BackToBackBonusが先とする

    constexpr unsigned int BackToBackBonus(unsigned int score) {
      // Back to Back Tetris/T-Spin = * 1.5
      return score + score / 2;
    }

    constexpr unsigned int BackToBackBonus(unsigned int score, bool backToBack) {
      return backToBack ? BackToBackBonus(score) : score;
    }

    constexpr unsigned int LevelBonus(unsigned int score, unsigned int level) {
      return score * level;
    }

    constexpr unsigned int REN(unsigned int ren) {
      // 50 * combo count
      return ren * 50;
    }


    constexpr unsigned int ExtremeLevelCoef = 20;

    constexpr unsigned int SoftDropPerCell = 1;
    constexpr unsigned int HardDropPerCell = 2;

    constexpr unsigned int Single = 100;
    constexpr unsigned int Double = 300;
    constexpr unsigned int Triple = 500;
    constexpr unsigned int Tetris = 800;

    namespace TSpin {
      constexpr unsigned int MiniZero = 100;
      constexpr unsigned int Zero = 400;
      constexpr unsigned int MiniSingle = 200;
      constexpr unsigned int Single = 800;
      constexpr unsigned int MiniDouble = 1200;
      constexpr unsigned int Double = 1200;
      constexpr unsigned int Triple = 1600;
    }

    namespace PerfectClear {
      // Tetris 99 のものを独自に調べた値
      // これもレベルで乗じる
      // ライン消去やRENボーナスとは別に加算する

      constexpr unsigned int Single = 800;
      constexpr unsigned int Double = 1200;
      constexpr unsigned int Triple = 1800;
      constexpr unsigned int Tetris = 2000;
    }
  }   // namespace Score

  // Ready画面
  namespace Ready {
    constexpr unsigned int MapX = Position::GameScreenX;
    constexpr unsigned int MapY = 9;
    constexpr unsigned int MapW = Tile::bg::Ready::MapWidth;
    constexpr unsigned int MapH = Tile::bg::Ready::MapHeight;

    static_assert(MapW == Board::Width);

    static_assert(Tile::bg::Ready::MapWidth == Tile::bg::Go::MapWidth);
    static_assert(Tile::bg::Ready::MapHeight == Tile::bg::Go::MapHeight);

    constexpr unsigned int ReadyFrame    = 60;
    constexpr unsigned int IntervalFrame = 6;
    constexpr unsigned int GoFrame       = 60;
  }   // namespace Ready

  // ポーズ画面
  namespace Pause {
    // ゲーム画面を暗くする係数
    constexpr std::uint_fast16_t BrightnessCoef = 7;

    // ボリューム
    constexpr unsigned int SoundVolume = 2;

    // タイトル画面に戻るのに必要なSELECTの押下時間（フレーム単位）
    constexpr unsigned int SelectHoldFrame = 120;
  }   // namespace Pause

  // ゲームクリア・ゲームオーバー画面
  namespace ClearOver {
    // ゲーム画面を暗くする係数
    constexpr std::uint_fast16_t BrightnessCoef = 11;

    constexpr unsigned int GameClearOverX = 7;
    constexpr unsigned int GameClearOverY = 1;

    constexpr unsigned int StatsX = 6;
    constexpr unsigned int StatsY = 5;

    // ゲームクリアまたはゲームオーバーにシーンが切り替わってからキー入力の受け付けを開始するまでの時間（誤入力防止）
    constexpr unsigned int KeyInputEnableDelay = 60;
  }   // namespace ClearOver
}   // namespace GameTetra::Config
