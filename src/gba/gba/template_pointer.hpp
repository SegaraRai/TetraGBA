#ifndef _gba_template_pointer_hpp_
#define _gba_template_pointer_hpp_

#include <cstdint>

#include "address.hpp"
#include "type.hpp"


namespace gba::template_pointer {
  // BIOS RAM
  template<typename T=isr>                                                                // 3007FFCh  4    R/W  ISRAD     Pointer to user IRQ handler (32bit ARM code)
  static const auto ISRAD       = reinterpret_cast<volatile T*>(address::ISRAD);
  template<typename T=std::uint16_t>                                                      // 3007FF8h  2    R/W  INTCHK    Interrupt Check Flag (for IntrWait/VBlankIntrWait functions)
  static const auto INTCHK      = reinterpret_cast<volatile T*>(address::INTCHK);
  template<typename T=std::uint32_t>                                                      // 3007FF4h  4    R/W  ALLOCA    Allocated Area
  static const auto ALLOCA      = reinterpret_cast<volatile T*>(address::ALLOCA);
  template<typename T=void*>                                                              // 3007FF0h  4    R/W  SNDBUFAD  Pointer to Sound Buffer
  static const auto SNDBUFAD    = reinterpret_cast<volatile T*>(address::SNDBUFAD);

  // LCD I/O Registers
  template<typename T=std::uint16_t>                                                      // 4000000h  2    R/W  DISPCNT   LCD Control
  static const auto DISPCNT     = reinterpret_cast<volatile T*>(address::DISPCNT);
  //                                                                                      // 4000002h  2    R/W  -         Undocumented - Green Swap
  template<typename T=std::uint16_t>                                                      // 4000004h  2    R/W  DISPSTAT  General LCD Status (STAT,LYC)
  static const auto DISPSTAT    = reinterpret_cast<volatile T*>(address::DISPSTAT);
  template<typename T=std::uint16_t>                                                      // 4000006h  2    R    VCOUNT    Vertical Counter (LY)
  static const auto VCOUNT      = reinterpret_cast<volatile T*>(address::VCOUNT);
  template<typename T=std::uint16_t>                                                      // 4000008h  2    R/W  BG0CNT    BG0 Control
  static const auto BG0CNT      = reinterpret_cast<volatile T*>(address::BG0CNT);
  template<typename T=std::uint16_t>                                                      // 400000Ah  2    R/W  BG1CNT    BG1 Control
  static const auto BG1CNT      = reinterpret_cast<volatile T*>(address::BG1CNT);
  template<typename T=std::uint16_t>                                                      // 400000Ch  2    R/W  BG2CNT    BG2 Control
  static const auto BG2CNT      = reinterpret_cast<volatile T*>(address::BG2CNT);
  template<typename T=std::uint16_t>                                                      // 400000Eh  2    R/W  BG3CNT    BG3 Control
  static const auto BG3CNT      = reinterpret_cast<volatile T*>(address::BG3CNT);
  template<typename T=std::uint16_t>                                                      // 4000010h  2    W    BG0HOFS   BG0 X-Offset
  static const auto BG0HOFS     = reinterpret_cast<volatile T*>(address::BG0HOFS);
  template<typename T=std::uint16_t>                                                      // 4000012h  2    W    BG0VOFS   BG0 Y-Offset
  static const auto BG0VOFS     = reinterpret_cast<volatile T*>(address::BG0VOFS);
  template<typename T=std::uint16_t>                                                      // 4000014h  2    W    BG1HOFS   BG1 X-Offset
  static const auto BG1HOFS     = reinterpret_cast<volatile T*>(address::BG1HOFS);
  template<typename T=std::uint16_t>                                                      // 4000016h  2    W    BG1VOFS   BG1 Y-Offset
  static const auto BG1VOFS     = reinterpret_cast<volatile T*>(address::BG1VOFS);
  template<typename T=std::uint16_t>                                                      // 4000018h  2    W    BG2HOFS   BG2 X-Offset
  static const auto BG2HOFS     = reinterpret_cast<volatile T*>(address::BG2HOFS);
  template<typename T=std::uint16_t>                                                      // 400001Ah  2    W    BG2VOFS   BG2 Y-Offset
  static const auto BG2VOFS     = reinterpret_cast<volatile T*>(address::BG2VOFS);
  template<typename T=std::uint16_t>                                                      // 400001Ch  2    W    BG3HOFS   BG3 X-Offset
  static const auto BG3HOFS     = reinterpret_cast<volatile T*>(address::BG3HOFS);
  template<typename T=std::uint16_t>                                                      // 400001Eh  2    W    BG3VOFS   BG3 Y-Offset
  static const auto BG3VOFS     = reinterpret_cast<volatile T*>(address::BG3VOFS);
  template<typename T=std::uint16_t>                                                      // 4000020h  2    W    BG2PA     BG2 Rotation/Scaling Parameter A (dx)
  static const auto BG2PA       = reinterpret_cast<volatile T*>(address::BG2PA);
  template<typename T=std::uint16_t>                                                      // 4000022h  2    W    BG2PB     BG2 Rotation/Scaling Parameter B (dmx)
  static const auto BG2PB       = reinterpret_cast<volatile T*>(address::BG2PB);
  template<typename T=std::uint16_t>                                                      // 4000024h  2    W    BG2PC     BG2 Rotation/Scaling Parameter C (dy)
  static const auto BG2PC       = reinterpret_cast<volatile T*>(address::BG2PC);
  template<typename T=std::uint16_t>                                                      // 4000026h  2    W    BG2PD     BG2 Rotation/Scaling Parameter D (dmy)
  static const auto BG2PD       = reinterpret_cast<volatile T*>(address::BG2PD);
  template<typename T=std::uint32_t>                                                      // 4000028h  4    W    BG2X      BG2 Reference Point X-Coordinate
  static const auto BG2X        = reinterpret_cast<volatile T*>(address::BG2X);
  template<typename T=std::uint32_t>                                                      // 400002Ch  4    W    BG2Y      BG2 Reference Point Y-Coordinate
  static const auto BG2Y        = reinterpret_cast<volatile T*>(address::BG2Y);
  template<typename T=std::uint16_t>                                                      // 4000030h  2    W    BG3PA     BG3 Rotation/Scaling Parameter A (dx)
  static const auto BG3PA       = reinterpret_cast<volatile T*>(address::BG3PA);
  template<typename T=std::uint16_t>                                                      // 4000032h  2    W    BG3PB     BG3 Rotation/Scaling Parameter B (dmx)
  static const auto BG3PB       = reinterpret_cast<volatile T*>(address::BG3PB);
  template<typename T=std::uint16_t>                                                      // 4000034h  2    W    BG3PC     BG3 Rotation/Scaling Parameter C (dy)
  static const auto BG3PC       = reinterpret_cast<volatile T*>(address::BG3PC);
  template<typename T=std::uint16_t>                                                      // 4000036h  2    W    BG3PD     BG3 Rotation/Scaling Parameter D (dmy)
  static const auto BG3PD       = reinterpret_cast<volatile T*>(address::BG3PD);
  template<typename T=std::uint32_t>                                                      // 4000038h  4    W    BG3X      BG3 Reference Point X-Coordinate
  static const auto BG3X        = reinterpret_cast<volatile T*>(address::BG3X);
  template<typename T=std::uint32_t>                                                      // 400003Ch  4    W    BG3Y      BG3 Reference Point Y-Coordinate
  static const auto BG3Y        = reinterpret_cast<volatile T*>(address::BG3Y);
  template<typename T=std::uint16_t>                                                      // 4000040h  2    W    WIN0H     Window 0 Horizontal Dimensions
  static const auto WIN0H       = reinterpret_cast<volatile T*>(address::WIN0H);
  template<typename T=std::uint16_t>                                                      // 4000042h  2    W    WIN1H     Window 1 Horizontal Dimensions
  static const auto WIN1H       = reinterpret_cast<volatile T*>(address::WIN1H);
  template<typename T=std::uint16_t>                                                      // 4000044h  2    W    WIN0V     Window 0 Vertical Dimensions
  static const auto WIN0V       = reinterpret_cast<volatile T*>(address::WIN0V);
  template<typename T=std::uint16_t>                                                      // 4000046h  2    W    WIN1V     Window 1 Vertical Dimensions
  static const auto WIN1V       = reinterpret_cast<volatile T*>(address::WIN1V);
  template<typename T=std::uint16_t>                                                      // 4000048h  2    R/W  WININ     Inside of Window 0 and 1
  static const auto WININ       = reinterpret_cast<volatile T*>(address::WININ);
  template<typename T=std::uint16_t>                                                      // 400004Ah  2    R/W  WINOUT    Inside of OBJ Window & Outside of Windows
  static const auto WINOUT      = reinterpret_cast<volatile T*>(address::WINOUT);
  template<typename T=std::uint16_t>                                                      // 400004Ch  2    W    MOSAIC    Mosaic Size
  static const auto MOSAIC      = reinterpret_cast<volatile T*>(address::MOSAIC);
  //                                                                                      // 400004Eh       -    -         Not used
  template<typename T=std::uint16_t>                                                      // 4000050h  2    R/W  BLDCNT    Color Special Effects Selection
  static const auto BLDCNT      = reinterpret_cast<volatile T*>(address::BLDCNT);
  template<typename T=std::uint16_t>                                                      // 4000052h  2    R/W  BLDALPHA  Alpha Blending Coefficients
  static const auto BLDALPHA    = reinterpret_cast<volatile T*>(address::BLDALPHA);
  template<typename T=std::uint16_t>                                                      // 4000054h  2    W    BLDY      Brightness (Fade-In/Out) Coefficient
  static const auto BLDY        = reinterpret_cast<volatile T*>(address::BLDY);
  //                                                                                      // 4000056h       -    -         Not used

  // Sound Registers
  template<typename T=std::uint16_t>                                                      // 4000060h  2  R/W  SOUND1CNT_L Channel 1 Sweep register       (NR10)
  static const auto SOUND1CNT_L = reinterpret_cast<volatile T*>(address::SOUND1CNT_L);
  template<typename T=std::uint16_t>                                                      // 4000062h  2  R/W  SOUND1CNT_H Channel 1 Duty/Length/Envelope (NR11, NR12)
  static const auto SOUND1CNT_H = reinterpret_cast<volatile T*>(address::SOUND1CNT_H);
  template<typename T=std::uint16_t>                                                      // 4000064h  2  R/W  SOUND1CNT_X Channel 1 Frequency/Control    (NR13, NR14)
  static const auto SOUND1CNT_X = reinterpret_cast<volatile T*>(address::SOUND1CNT_X);
  //                                                                                      // 4000066h     -    -           Not used
  template<typename T=std::uint16_t>                                                      // 4000068h  2  R/W  SOUND2CNT_L Channel 2 Duty/Length/Envelope (NR21, NR22)
  static const auto SOUND2CNT_L = reinterpret_cast<volatile T*>(address::SOUND2CNT_L);
  //                                                                                      // 400006Ah     -    -           Not used
  template<typename T=std::uint16_t>                                                      // 400006Ch  2  R/W  SOUND2CNT_H Channel 2 Frequency/Control    (NR23, NR24)
  static const auto SOUND2CNT_H = reinterpret_cast<volatile T*>(address::SOUND2CNT_H);
  //                                                                                      // 400006Eh     -    -           Not used
  template<typename T=std::uint16_t>                                                      // 4000070h  2  R/W  SOUND3CNT_L Channel 3 Stop/Wave RAM select (NR30)
  static const auto SOUND3CNT_L = reinterpret_cast<volatile T*>(address::SOUND3CNT_L);
  template<typename T=std::uint16_t>                                                      // 4000072h  2  R/W  SOUND3CNT_H Channel 3 Length/Volume        (NR31, NR32)
  static const auto SOUND3CNT_H = reinterpret_cast<volatile T*>(address::SOUND3CNT_H);
  template<typename T=std::uint16_t>                                                      // 4000074h  2  R/W  SOUND3CNT_X Channel 3 Frequency/Control    (NR33, NR34)
  static const auto SOUND3CNT_X = reinterpret_cast<volatile T*>(address::SOUND3CNT_X);
  //                                                                                      // 4000076h     -    -           Not used
  template<typename T=std::uint16_t>                                                      // 4000078h  2  R/W  SOUND4CNT_L Channel 4 Length/Envelope      (NR41, NR42)
  static const auto SOUND4CNT_L = reinterpret_cast<volatile T*>(address::SOUND4CNT_L);
  //                                                                                      // 400007Ah     -    -           Not used
  template<typename T=std::uint16_t>                                                      // 400007Ch  2  R/W  SOUND4CNT_H Channel 4 Frequency/Control    (NR43, NR44)
  static const auto SOUND4CNT_H = reinterpret_cast<volatile T*>(address::SOUND4CNT_H);
  //                                                                                      // 400007Eh     -    -           Not used
  template<typename T=std::uint16_t>                                                      // 4000080h  2  R/W  SOUNDCNT_L  Control Stereo/Volume/Enable   (NR50, NR51)
  static const auto SOUNDCNT_L  = reinterpret_cast<volatile T*>(address::SOUNDCNT_L);
  template<typename T=std::uint16_t>                                                      // 4000082h  2  R/W  SOUNDCNT_H  Control Mixing/DMA Control
  static const auto SOUNDCNT_H  = reinterpret_cast<volatile T*>(address::SOUNDCNT_H);
  template<typename T=std::uint16_t>                                                      // 4000084h  2  R/W  SOUNDCNT_X  Control Sound on/off           (NR52)
  static const auto SOUNDCNT_X  = reinterpret_cast<volatile T*>(address::SOUNDCNT_X);
  //                                                                                      // 4000086h     -    -           Not used
  template<typename T=std::uint16_t>                                                      // 4000088h  2  BIOS SOUNDBIAS   Sound PWM Control
  static const auto SOUNDBIAS   = reinterpret_cast<volatile T*>(address::SOUNDBIAS);
  //                                                                                      // 400008Ah  ..   -    -         Not used
  //                                                                                      // 4000090h 2x10h R/W  WAVE_RAM  Channel 3 Wave Pattern RAM (2 banks!!)
  template<typename T=std::uint16_t>                                                      // 4000090h  2  R/W  WAVE_RAM0_L Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM0_L = reinterpret_cast<volatile T*>(address::WAVE_RAM0_L);
  template<typename T=std::uint16_t>                                                      // 4000092h  2  R/W  WAVE_RAM0_H Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM0_H = reinterpret_cast<volatile T*>(address::WAVE_RAM0_H);
  template<typename T=std::uint16_t>                                                      // 4000094h  2  R/W  WAVE_RAM1_L Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM1_L = reinterpret_cast<volatile T*>(address::WAVE_RAM1_L);
  template<typename T=std::uint16_t>                                                      // 4000096h  2  R/W  WAVE_RAM1_H Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM1_H = reinterpret_cast<volatile T*>(address::WAVE_RAM1_H);
  template<typename T=std::uint16_t>                                                      // 4000098h  2  R/W  WAVE_RAM2_L Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM2_L = reinterpret_cast<volatile T*>(address::WAVE_RAM2_L);
  template<typename T=std::uint16_t>                                                      // 400009Ah  2  R/W  WAVE_RAM2_H Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM2_H = reinterpret_cast<volatile T*>(address::WAVE_RAM2_H);
  template<typename T=std::uint16_t>                                                      // 400009Ch  2  R/W  WAVE_RAM3_L Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM3_L = reinterpret_cast<volatile T*>(address::WAVE_RAM3_L);
  template<typename T=std::uint16_t>                                                      // 400009Eh  2  R/W  WAVE_RAM3_H Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM3_H = reinterpret_cast<volatile T*>(address::WAVE_RAM3_H);
  template<typename T=void*>                                                              // 40000A0h  4    W    FIFO_A    Channel A FIFO, Data 0-3
  static const auto FIFO_A      = reinterpret_cast<volatile T*>(address::FIFO_A);
  template<typename T=void*>                                                              // 40000A4h  4    W    FIFO_B    Channel B FIFO, Data 0-3
  static const auto FIFO_B      = reinterpret_cast<volatile T*>(address::FIFO_B);
  //                                                                                      // 40000A8h       -    -         Not used

  // DMA Transfer Channels
  template<typename T=const void*>                                                        // 40000B0h  4    W    DMA0SAD   DMA 0 Source Address
  static const auto DMA0SAD     = reinterpret_cast<volatile T*>(address::DMA0SAD);
  template<typename T=void*>                                                              // 40000B4h  4    W    DMA0DAD   DMA 0 Destination Address
  static const auto DMA0DAD     = reinterpret_cast<volatile T*>(address::DMA0DAD);
  template<typename T=std::uint16_t>                                                      // 40000B8h  2    W    DMA0CNT_L DMA 0 Word Count
  static const auto DMA0CNT_L   = reinterpret_cast<volatile T*>(address::DMA0CNT_L);
  template<typename T=std::uint16_t>                                                      // 40000BAh  2    R/W  DMA0CNT_H DMA 0 Control
  static const auto DMA0CNT_H   = reinterpret_cast<volatile T*>(address::DMA0CNT_H);
  template<typename T=const void*>                                                        // 40000BCh  4    W    DMA1SAD   DMA 1 Source Address
  static const auto DMA1SAD     = reinterpret_cast<volatile T*>(address::DMA1SAD);
  template<typename T=void*>                                                              // 40000C0h  4    W    DMA1DAD   DMA 1 Destination Address
  static const auto DMA1DAD     = reinterpret_cast<volatile T*>(address::DMA1DAD);
  template<typename T=std::uint16_t>                                                      // 40000C4h  2    W    DMA1CNT_L DMA 1 Word Count
  static const auto DMA1CNT_L   = reinterpret_cast<volatile T*>(address::DMA1CNT_L);
  template<typename T=std::uint16_t>                                                      // 40000C6h  2    R/W  DMA1CNT_H DMA 1 Control
  static const auto DMA1CNT_H   = reinterpret_cast<volatile T*>(address::DMA1CNT_H);
  template<typename T=const void*>                                                        // 40000C8h  4    W    DMA2SAD   DMA 2 Source Address
  static const auto DMA2SAD     = reinterpret_cast<volatile T*>(address::DMA2SAD);
  template<typename T=void*>                                                              // 40000CCh  4    W    DMA2DAD   DMA 2 Destination Address
  static const auto DMA2DAD     = reinterpret_cast<volatile T*>(address::DMA2DAD);
  template<typename T=std::uint16_t>                                                      // 40000D0h  2    W    DMA2CNT_L DMA 2 Word Count
  static const auto DMA2CNT_L   = reinterpret_cast<volatile T*>(address::DMA2CNT_L);
  template<typename T=std::uint16_t>                                                      // 40000D2h  2    R/W  DMA2CNT_H DMA 2 Control
  static const auto DMA2CNT_H   = reinterpret_cast<volatile T*>(address::DMA2CNT_H);
  template<typename T=const void*>                                                        // 40000D4h  4    W    DMA3SAD   DMA 3 Source Address
  static const auto DMA3SAD     = reinterpret_cast<volatile T*>(address::DMA3SAD);
  template<typename T=void*>                                                              // 40000D8h  4    W    DMA3DAD   DMA 3 Destination Address
  static const auto DMA3DAD     = reinterpret_cast<volatile T*>(address::DMA3DAD);
  template<typename T=std::uint16_t>                                                      // 40000DCh  2    W    DMA3CNT_L DMA 3 Word Count
  static const auto DMA3CNT_L   = reinterpret_cast<volatile T*>(address::DMA3CNT_L);
  template<typename T=std::uint16_t>                                                      // 40000DEh  2    R/W  DMA3CNT_H DMA 3 Control
  static const auto DMA3CNT_H   = reinterpret_cast<volatile T*>(address::DMA3CNT_H);
  //                                                                                      // 40000E0h       -    -         Not used

  // Timer Registers
  template<typename T=std::uint16_t>                                                      // 4000100h  2    R/W  TM0CNT_L  Timer 0 Counter/Reload
  static const auto TM0CNT_L    = reinterpret_cast<volatile T*>(address::TM0CNT_L);
  template<typename T=std::uint16_t>                                                      // 4000102h  2    R/W  TM0CNT_H  Timer 0 Control
  static const auto TM0CNT_H    = reinterpret_cast<volatile T*>(address::TM0CNT_H);
  template<typename T=std::uint16_t>                                                      // 4000104h  2    R/W  TM1CNT_L  Timer 1 Counter/Reload
  static const auto TM1CNT_L    = reinterpret_cast<volatile T*>(address::TM1CNT_L);
  template<typename T=std::uint16_t>                                                      // 4000106h  2    R/W  TM1CNT_H  Timer 1 Control
  static const auto TM1CNT_H    = reinterpret_cast<volatile T*>(address::TM1CNT_H);
  template<typename T=std::uint16_t>                                                      // 4000108h  2    R/W  TM2CNT_L  Timer 2 Counter/Reload
  static const auto TM2CNT_L    = reinterpret_cast<volatile T*>(address::TM2CNT_L);
  template<typename T=std::uint16_t>                                                      // 400010Ah  2    R/W  TM2CNT_H  Timer 2 Control
  static const auto TM2CNT_H    = reinterpret_cast<volatile T*>(address::TM2CNT_H);
  template<typename T=std::uint16_t>                                                      // 400010Ch  2    R/W  TM3CNT_L  Timer 3 Counter/Reload
  static const auto TM3CNT_L    = reinterpret_cast<volatile T*>(address::TM3CNT_L);
  template<typename T=std::uint16_t>                                                      // 400010Eh  2    R/W  TM3CNT_H  Timer 3 Control
  static const auto TM3CNT_H    = reinterpret_cast<volatile T*>(address::TM3CNT_H);
  //                                                                                      // 4000110h       -    -         Not used

  // Serial Communication (1)
  template<typename T=std::uint32_t>                                                      // 4000120h  4    R/W  SIODATA32 SIO Data (Normal-32bit Mode; shared with below)
  static const auto SIODATA32   = reinterpret_cast<volatile T*>(address::SIODATA32);
  template<typename T=std::uint16_t>                                                      // 4000120h  2    R/W  SIOMULTI0 SIO Data 0 (Parent)    (Multi-Player Mode)
  static const auto SIOMULTI0   = reinterpret_cast<volatile T*>(address::SIOMULTI0);
  template<typename T=std::uint16_t>                                                      // 4000122h  2    R/W  SIOMULTI1 SIO Data 1 (1st Child) (Multi-Player Mode)
  static const auto SIOMULTI1   = reinterpret_cast<volatile T*>(address::SIOMULTI1);
  template<typename T=std::uint16_t>                                                      // 4000124h  2    R/W  SIOMULTI2 SIO Data 2 (2nd Child) (Multi-Player Mode)
  static const auto SIOMULTI2   = reinterpret_cast<volatile T*>(address::SIOMULTI2);
  template<typename T=std::uint16_t>                                                      // 4000126h  2    R/W  SIOMULTI3 SIO Data 3 (3rd Child) (Multi-Player Mode)
  static const auto SIOMULTI3   = reinterpret_cast<volatile T*>(address::SIOMULTI3);
  template<typename T=std::uint16_t>                                                      // 4000128h  2    R/W  SIOCNT    SIO Control Register
  static const auto SIOCNT      = reinterpret_cast<volatile T*>(address::SIOCNT);
  template<typename T=std::uint16_t>                                                      // 400012Ah  2    R/W  SIOMLT_SEND SIO Data (Local of MultiPlayer; shared below)
  static const auto SIOMLT_SEND = reinterpret_cast<volatile T*>(address::SIOMLT_SEND);
  template<typename T=std::uint16_t>                                                      // 400012Ah  2    R/W  SIODATA8  SIO Data (Normal-8bit and UART Mode)
  static const auto SIODATA8    = reinterpret_cast<volatile T*>(address::SIODATA8);
  //                                                                                      // 400012Ch       -    -         Not used

  // Keypad Input
  template<typename T=std::uint16_t>                                                      // 4000130h  2    R    KEYINPUT  Key Status
  static const auto KEYINPUT    = reinterpret_cast<volatile T*>(address::KEYINPUT);
  template<typename T=std::uint16_t>                                                      // 4000132h  2    R/W  KEYCNT    Key Interrupt Control
  static const auto KEYCNT      = reinterpret_cast<volatile T*>(address::KEYCNT);

  // Serial Communication (2)
  template<typename T=std::uint16_t>                                                      // 4000134h  2    R/W  RCNT      SIO Mode Select/General Purpose Data
  static const auto RCNT        = reinterpret_cast<volatile T*>(address::RCNT);
  //                                                                                      // 4000136h  -    -    IR        Ancient - Infrared Register (Prototypes only)
  //                                                                                      // 4000138h       -    -         Not used
  template<typename T=std::uint16_t>                                                      // 4000140h  2    R/W  JOYCNT    SIO JOY Bus Control
  static const auto JOYCNT      = reinterpret_cast<volatile T*>(address::JOYCNT);
  //                                                                                      // 4000142h       -    -         Not used
  template<typename T=std::uint32_t>                                                      // 4000150h  4    R/W  JOY_RECV  SIO JOY Bus Receive Data
  static const auto JOY_RECV    = reinterpret_cast<volatile T*>(address::JOY_RECV);
  template<typename T=std::uint32_t>                                                      // 4000154h  4    R/W  JOY_TRANS SIO JOY Bus Transmit Data
  static const auto JOY_TRANS   = reinterpret_cast<volatile T*>(address::JOY_TRANS);
  template<typename T=std::uint16_t>                                                      // 4000158h  2    R/?  JOYSTAT   SIO JOY Bus Receive Status
  static const auto JOYSTAT     = reinterpret_cast<volatile T*>(address::JOYSTAT);
  //                                                                                      // 400015Ah       -    -         Not used

  // Interrupt, Waitstate, and Power-Down Control
  template<typename T=std::uint16_t>                                                      // 4000200h  2    R/W  IE        Interrupt Enable Register
  static const auto IE          = reinterpret_cast<volatile T*>(address::IE);
  template<typename T=std::uint16_t>                                                      // 4000202h  2    R/W  IF        Interrupt Request Flags / IRQ Acknowledge
  static const auto IF          = reinterpret_cast<volatile T*>(address::IF);
  template<typename T=std::uint16_t>                                                      // 4000204h  2    R/W  WAITCNT   Game Pak Waitstate Control
  static const auto WAITCNT     = reinterpret_cast<volatile T*>(address::WAITCNT);
  //                                                                                      // 4000206h       -    -         Not used
  template<typename T=std::uint16_t>                                                      // 4000208h  2    R/W  IME       Interrupt Master Enable Register
  static const auto IME         = reinterpret_cast<volatile T*>(address::IME);
  //                                                                                      // 400020Ah       -    -         Not used
  //                                                                                      // 4000300h  1    R/W  POSTFLG   Undocumented - Post Boot Flag
  //                                                                                      // 4000301h  1    W    HALTCNT   Undocumented - Power Down Control
  //                                                                                      // 4000302h       -    -         Not used
  //                                                                                      // 4000410h  ?    ?    ?         Undocumented - Purpose Unknown / Bug ??? 0FFh
  //                                                                                      // 4000411h       -    -         Not used
  //                                                                                      // 4000800h  4    R/W  ?         Undocumented - Internal Memory Control (R/W)
  //                                                                                      // 4000804h       -    -         Not used
  //                                                                                      // 4xx0800h  4    R/W  ?         Mirrors of 4000800h (repeated each 64K)
}   // namespace gba::template_pointer

#endif
