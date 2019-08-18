#ifndef _gba_const_lcd_hpp_
#define _gba_const_lcd_hpp_

#include <cassert>
#include <cstdint>


namespace gba {
  inline namespace constant {
    // ## GBA LCD Video Controller
    // https://problemkaputt.de/gbatek.htm#gbalcdvideocontroller


    // ### LCD I/O Display Control
    // https://problemkaputt.de/gbatek.htm#lcdiodisplaycontrol

    namespace DISPCNT {
      // BG Mode (0-5=Video Mode 0-5, 6-7=Prohibited)
      inline namespace BGMODE {
        constexpr std::uint16_t BGMODE0   = 0 <<  0;
        constexpr std::uint16_t BGMODE1   = 1 <<  0;
        constexpr std::uint16_t BGMODE2   = 2 <<  0;
        constexpr std::uint16_t BGMODE3   = 3 <<  0;
        constexpr std::uint16_t BGMODE4   = 4 <<  0;
        constexpr std::uint16_t BGMODE5   = 5 <<  0;
      }

      constexpr std::uint16_t GBA         = 0 <<  3;    // Reserved / CGB Mode (GBA; can be set only by BIOS opcodes)
      constexpr std::uint16_t CGB         = 1 <<  3;    // Reserved / CGB Mode (CGB; can be set only by BIOS opcodes)

      constexpr std::uint16_t FRAME0      = 0 <<  4;    // Display Frame Select (Frame 0) (for BG Modes 4, 5 only)
      constexpr std::uint16_t FRAME1      = 1 <<  4;    // Display Frame Select (Frame 1) (for BG Modes 4, 5 only)

      constexpr std::uint16_t HBLANK_FREE = 1 <<  5;    // H-Blank Interval Free (Allow access to OAM during H-Blank)

      constexpr std::uint16_t OBJMAP1D    = 1 <<  6;    // OBJ Character VRAM Mapping (Two dimensional)
      constexpr std::uint16_t OBJMAP2D    = 0 <<  6;    // OBJ Character VRAM Mapping (One dimensional)

      constexpr std::uint16_t FORCE_BLANK = 1 <<  7;    // Forced Blank (Allow FAST access to VRAM, Palette, OAM)

      constexpr std::uint16_t BG0         = 1 <<  8;    // Screen Display BG0 (On)
      constexpr std::uint16_t BG1         = 1 <<  9;    // Screen Display BG1 (On)
      constexpr std::uint16_t BG2         = 1 << 10;    // Screen Display BG2 (On)
      constexpr std::uint16_t BG3         = 1 << 11;    // Screen Display BG3 (On)
      constexpr std::uint16_t OBJ         = 1 << 12;    // Screen Display OBJ (On)
      constexpr std::uint16_t WIN0        = 1 << 13;    // Window 0 Display Flag (On)
      constexpr std::uint16_t WIN1        = 1 << 14;    // Window 1 Display Flag (On)
      constexpr std::uint16_t WINOBJ      = 1 << 15;    // OBJ Window Display Flag (On)
    }


    // ### LCD I/O Interrupts and Status
    // https://problemkaputt.de/gbatek.htm#lcdiointerruptsandstatus

    namespace DISPSTAT {
      constexpr std::uint16_t IS_VBLANK      = 1 << 0;    // V-Blank flag (Read only) (set in line 160..226; not 227)
      constexpr std::uint16_t IS_HBLANK      = 1 << 1;    // H-Blank flag (Read only) (toggled in all lines, 0..227)
      constexpr std::uint16_t VCOUNTER_MATCH = 1 << 2;    // V-Counter match flag (Read only) (set in selected line)
      constexpr std::uint16_t VBLANK_IRQ     = 1 << 3;    // V-Blank IRQ Enable
      constexpr std::uint16_t HBLANK_IRQ     = 1 << 4;    // H-Blank IRQ Enable
      constexpr std::uint16_t VCOUNTER_IRQ   = 1 << 5;    // V-Counter IRQ Enable

      // V-Count Setting (LYC) (0..227)
      constexpr std::uint16_t VCOUNT(std::uint_fast16_t vCount) {
        assert(vCount < 228);
        return static_cast<std::uint16_t>(vCount & 255) << 8;
      }

      // V-Count Setting (LYC) (0..227)
      constexpr std::uint16_t GET_VCOUNT(std::uint16_t value) {
        return value >> 8;
      }
    }


    // ### LCD I/O BG Control
    // https://problemkaputt.de/gbatek.htm#lcdiobgcontrol

    namespace BGCNT {
      // BG Priority (0-3, 0=Highest)
      inline namespace PRIORITY {
        constexpr std::uint16_t PRIORITY0 = 0;    // Highest priority
        constexpr std::uint16_t PRIORITY1 = 1;    // Higher priority
        constexpr std::uint16_t PRIORITY2 = 2;    // Lower priority
        constexpr std::uint16_t PRIORITY3 = 3;    // Lowest priority
      }   // namespace PRIORITY

      // Character Base Block(0-3, in units of 16 KBytes) (=BG Tile Data)
      constexpr std::uint16_t CHARBASE(std::uint_fast16_t characterBaseBlock) {
        assert(characterBaseBlock < 4);
        return static_cast<std::uint16_t>(characterBaseBlock & 3) << 2;
      }

      // Character Base Block(0-3, in units of 16 KBytes) (=BG Tile Data)
      constexpr std::uint16_t GET_CHARBASE(std::uint16_t value) {
        return (value >> 2) & 3;
      }

      constexpr std::uint16_t MOSAIC   = 1 << 6;    // Mosaic (Enable)

      constexpr std::uint16_t COLOR16  = 0 << 7;    // Colors =  16, Palettes = 16
      constexpr std::uint16_t COLOR256 = 1 << 7;    // Colors = 256, Palettes =  1

      // Screen Base Block (0-31, in units of 2 KBytes) (=BG Map Data)
      constexpr std::uint16_t SCRBASE(std::uint_fast16_t screenBaseBlock) {
        assert(screenBaseBlock < 32);
        return static_cast<std::uint16_t>(screenBaseBlock & 31) << 8;
      }

      // Screen Base Block (0-31, in units of 2 KBytes) (=BG Map Data)
      constexpr std::uint16_t GET_SCRBASE(std::uint16_t value) {
        return (value >> 8) & 31;
      }

      constexpr std::uint16_t WRAP     = 1 << 13;   // BG2/BG3: Display Area Overflow (0=Transparent, 1=Wraparound)

      // Value  Text Mode      Rotation/Scaling Mode
      // 0      256x256 (2K)   128x128   (256 bytes)
      // 1      512x256 (4K)   256x256   (1K)
      // 2      256x512 (4K)   512x512   (4K)
      // 3      512x512 (8K)   1024x1024 (16K)
      inline namespace SIZE {
        constexpr std::uint16_t SIZE0 = 0 << 14;    // Text Mode = 256x256 (2K), Rotation/Scaling Mode = 128x128   (256 bytes)
        constexpr std::uint16_t SIZE1 = 1 << 14;    // Text Mode = 512x256 (4K), Rotation/Scaling Mode = 256x256   (1K)
        constexpr std::uint16_t SIZE2 = 2 << 14;    // Text Mode = 256x512 (4K), Rotation/Scaling Mode = 512x512   (4K)
        constexpr std::uint16_t SIZE3 = 3 << 14;    // Text Mode = 512x512 (8K), Rotation/Scaling Mode = 1024x1024 (16K)
      }
    }   // namespace BGCNT


    // ### LCD I/O BG Scrolling
    // https://problemkaputt.de/gbatek.htm#lcdiobgscrolling


    // ### LCD I/O BG Rotation/Scaling
    // https://problemkaputt.de/gbatek.htm#lcdiobgrotationscaling


    // ### LCD I/O Window Feature
    // https://problemkaputt.de/gbatek.htm#lcdiowindowfeature

    // TODO
    namespace WINHV {}
    namespace WININ {}
    namespace WINOUT {}


    // ### LCD I/O Mosaic Function
    // https://problemkaputt.de/gbatek.htm#lcdiomosaicfunction

    // TODO
    namespace MOSAIC {}


    // ### LCD I/O Color Special Effects
    // https://problemkaputt.de/gbatek.htm#lcdiocolorspecialeffects

    namespace BLDCNT {
      constexpr std::uint16_t FIRST_BG0  = 1 <<  0;   // BG0 1st Target Pixel (Background 0)
      constexpr std::uint16_t FIRST_BG1  = 1 <<  1;   // BG1 1st Target Pixel (Background 1)
      constexpr std::uint16_t FIRST_BG2  = 1 <<  2;   // BG2 1st Target Pixel (Background 2)
      constexpr std::uint16_t FIRST_BG3  = 1 <<  3;   // BG3 1st Target Pixel (Background 3)
      constexpr std::uint16_t FIRST_OBJ  = 1 <<  4;   // OBJ 1st Target Pixel (Top-most OBJ pixel)
      constexpr std::uint16_t FIRST_BD   = 1 <<  5;   // BD  1st Target Pixel (Backdrop)

      // Color Special Effect (0-3)
      inline namespace EFFECT {
        constexpr std::uint16_t NONE  = 0 << 6;       // None (Special effects disabled)
        constexpr std::uint16_t ALPHA = 1 << 6;       // Alpha Blending (1st + 2nd Target mixed)
        constexpr std::uint16_t BRINC = 2 << 6;       // Brightness Increase (1st Target becomes whiter)
        constexpr std::uint16_t BRDEC = 3 << 6;       // Brightness Decrease (1st Target becomes blacker)
      }

      constexpr std::uint16_t SECOND_BG0 = 1 <<  8;   // BG0 2nd Target Pixel (Background 0)
      constexpr std::uint16_t SECOND_BG1 = 1 <<  9;   // BG1 2nd Target Pixel (Background 1)
      constexpr std::uint16_t SECOND_BG2 = 1 << 10;   // BG2 2nd Target Pixel (Background 2)
      constexpr std::uint16_t SECOND_BG3 = 1 << 11;   // BG3 2nd Target Pixel (Background 3)
      constexpr std::uint16_t SECOND_OBJ = 1 << 12;   // OBJ 2nd Target Pixel (Top-most OBJ pixel)
      constexpr std::uint16_t SECOND_BD  = 1 << 13;   // BD  2nd Target Pixel (Backdrop)
    }

    namespace BLDALPHA {
      // EVA Coefficient (1st Target) (0..16 = 0/16..16/16, 17..31=16/16)
      constexpr std::uint16_t EVACOEF(std::uint_fast16_t evaCoefficient) {
        assert(evaCoefficient < 32);
        return static_cast<std::uint16_t>(evaCoefficient & 31);
      }

      // EVA Coefficient (1st Target) (0..16 = 0/16..16/16, 17..31=16/16)
      constexpr std::uint16_t GET_EVACOEF(std::uint16_t value) {
        return value & 31;
      }

      // EVB Coefficient (2nd Target) (0..16 = 0/16..16/16, 17..31=16/16)
      constexpr std::uint16_t EVBCOEF(std::uint_fast16_t evbCoefficient) {
        assert(evbCoefficient < 32);
        return static_cast<std::uint16_t>(evbCoefficient & 31) << 8;
      }

      // EVB Coefficient (2nd Target) (0..16 = 0/16..16/16, 17..31=16/16)
      constexpr std::uint16_t GET_EVBCOEF(std::uint16_t value) {
        return (value & 31) >> 8;
      }
    }

    namespace BLDY {
      // EVY Coefficient (Brightness) (0..16 = 0/16..16/16, 17..31=16/16)
      constexpr std::uint16_t EVYCOEF(std::uint_fast16_t evyCoefficient) {
        assert(evyCoefficient < 32);
        return static_cast<std::uint16_t>(evyCoefficient & 31);
      }

      // EVY Coefficient (Brightness) (0..16 = 0/16..16/16, 17..31=16/16)
      constexpr std::uint16_t GET_EVYCOEF(std::uint16_t value) {
        return value & 31;
      }
    }


    // ### LCD VRAM BG Screen Data Format (BG Map)
    // https://problemkaputt.de/gbatek.htm#lcdvrambgscreendataformatbgmap

    namespace BGMAP {
      // Text BG Screen (2 bytes per entry)
      namespace TEXT {
        // Tile Number (0-1023) (a bit less in 256 color mode, because there'd be otherwise no room for the bg map)
        constexpr std::uint16_t TILE(std::uint_fast16_t tileNumber) {
          assert(tileNumber < 1024);
          return static_cast<std::uint16_t>(tileNumber & 1023);
        }

        // Tile Number (0-1023) (a bit less in 256 color mode, because there'd be otherwise no room for the bg map)
        constexpr std::uint16_t GET_TILE(std::uint16_t value) {
          return value & 1023;
        }

        const std::uint16_t FLIP_H = 1 << 10;   // Horizontal Flip (Mirrored)
        const std::uint16_t FLIP_V = 1 << 11;   // Vertical Flip (Mirrored)

        // Palette Number (0-15) (Not used in 256 color/1 palette mode)
        constexpr std::uint16_t PALETTE(std::uint_fast16_t paletteNumber) {
          assert(paletteNumber < 16);
          return static_cast<std::uint16_t>(paletteNumber & 15) << 12;
        }

        // Tile Number (0-1023) (a bit less in 256 color mode, because there'd be otherwise no room for the bg map)
        constexpr std::uint16_t GET_PALETTE(std::uint16_t value) {
          return (value >> 12) & 15;
        }
      }
    }


    // ### LCD OBJ - OAM Attributes
    // https://problemkaputt.de/gbatek.htm#lcdobjoamattributes

    namespace OBJATTR0 {
      // Y-Coordinate (0-255)
      constexpr std::uint16_t Y(std::uint_fast16_t y) {
        assert(y < 256);
        return static_cast<std::uint16_t>(y & 255);
      }

      // Y-Coordinate (0-255)
      constexpr std::uint16_t GET_Y(std::uint16_t value) {
        return value & 255;
      }

      constexpr std::uint16_t ROTSCALE     = 1 << 8;    // Rotation/Scaling Flag (On)

      constexpr std::uint16_t DOUBLE       = 1 << 9;    // (When Rotation/Scaling used) Double-Size Flag (Double)
      constexpr std::uint16_t OBJ_DISABLE  = 1 << 9;    // (When Rotation/Scaling not used) OBJ Disable (Not displayed)

      // OBJ Mode (0=Normal, 1=Semi-Transparent, 2=OBJ Window, 3=Prohibited)
      inline namespace OBJMODE {
        constexpr std::uint16_t NORMAL     = 0 << 10;   // Normal
        constexpr std::uint16_t SEMITRANSP = 1 << 10;   // Semi-Transparent
        constexpr std::uint16_t OBJWIN     = 2 << 10;   // OBJ Window
      }

      constexpr std::uint16_t OBJMOSAIC    = 1 << 12;   // OBJ Mosaic (On)

      constexpr std::uint16_t COLOR16      = 0 << 13;   // Colors =  16, Palettes = 16
      constexpr std::uint16_t COLOR256     = 1 << 13;   // Colors = 256, Palettes =  1

      // OBJ Shape (0=Square, 1=Horizontal, 2=Vertical, 3=Prohibited)
      inline namespace OBJSHAPE {
        constexpr std::uint16_t SQUARE     = 0 << 14;   // Square
        constexpr std::uint16_t HORIZONTAL = 1 << 14;   // Horizontal
        constexpr std::uint16_t VERTICAL   = 2 << 14;   // Vertical
      }
    }

    namespace OBJATTR1 {
      // X-Coordinate (0-511)
      constexpr std::uint16_t X(std::uint_fast16_t x) {
        assert(x < 512);
        return static_cast<std::uint16_t>(x & 511);
      }

      // X-Coordinate (0-511)
      constexpr std::uint16_t GET_X(std::uint16_t value) {
        return value & 511;
      }

      constexpr std::uint16_t FLIP_H = 1 << 12;   // (When Rotation/Scaling not used) Horizontal Flip (Mirrored)
      constexpr std::uint16_t FLIP_V = 1 << 13;   // (When Rotation/Scaling not used) Vertical Flip (Mirrored)

      // OBJ Size (0..3, depends on OBJ Shape, see Attr 0)
      // Size  Square   Horizontal  Vertical
      // 0     8x8      16x8        8x16
      // 1     16x16    32x8        8x32
      // 2     32x32    32x16       16x32
      // 3     64x64    64x32       32x64
      inline namespace OBJSIZE {
        constexpr std::uint16_t SIZE0 = 0 << 14;   // Square =   8x8, Horizontal =  16x8, Vertical =  8x16
        constexpr std::uint16_t SIZE1 = 1 << 14;   // Square = 16x16, Horizontal =  32x8, Vertical =  8x32
        constexpr std::uint16_t SIZE2 = 2 << 14;   // Square = 32x32, Horizontal = 32x16, Vertical = 16x32
        constexpr std::uint16_t SIZE3 = 3 << 14;   // Square = 64x64, Horizontal = 64x32, Vertical = 32x64
      }
    }

    namespace OBJATTR2 {
      // Character Name (0-1023=Tile Number)
      constexpr std::uint16_t TILE(std::uint_fast16_t tileNumber) {
        assert(tileNumber < 1024);
        return static_cast<std::uint16_t>(tileNumber & 1023);
      }

      // Character Name (0-1023=Tile Number)
      constexpr std::uint16_t GET_TILE(std::uint16_t value) {
        return value & 1023;
      }

      // Priority relative to BG (0-3; 0=Highest)
      inline namespace PRIORITY {
        constexpr std::uint16_t PRIORITY0 = 0 << 10;    // Highest priority
        constexpr std::uint16_t PRIORITY1 = 1 << 10;    // Higher priority
        constexpr std::uint16_t PRIORITY2 = 2 << 10;    // Lower priority
        constexpr std::uint16_t PRIORITY3 = 3 << 10;    // Lowest priority
      }   // namespace PRIORITY

      // Palette Number (0-15) (Not used in 256 color/1 palette mode)
      constexpr std::uint16_t PALETTE(std::uint_fast16_t paletteNumber) {
        assert(paletteNumber < 16);
        return static_cast<std::uint16_t>(paletteNumber & 15) << 12;
      }

      // Tile Number (0-1023) (a bit less in 256 color mode, because there'd be otherwise no room for the bg map)
      constexpr std::uint16_t GET_PALETTE(std::uint16_t value) {
        return (value >> 12) & 15;
      }
    }
  }   // inline namespace constant
}   // namespace gba

#endif
