#ifndef _gba_register_hpp_
#define _gba_register_hpp_

#include <cstdint>

#include "address.hpp"
#include "type.hpp"


namespace gba::reg {
  template<typename T, std::uintptr_t Address>
  volatile auto& byAddress = *reinterpret_cast<volatile T*>(Address);

  // BIOS RAM
  static volatile auto& ISRAD       = *reinterpret_cast<isr volatile*>(address::ISRAD);                   // 3007FFCh  4    R/W  ISRAD     Pointer to user IRQ handler (32bit ARM code)
  static volatile auto& INTCHK      = *reinterpret_cast<std::uint16_t volatile*>(address::INTCHK);        // 3007FF8h  2    R/W  INTCHK    Interrupt Check Flag (for IntrWait/VBlankIntrWait functions)
  static volatile auto& ALLOCA      = *reinterpret_cast<std::uint32_t volatile*>(address::ALLOCA);        // 3007FF4h  4    R/W  ALLOCA    Allocated Area
  static volatile auto& SNDBUFAD    = *reinterpret_cast<void* volatile*>(address::SNDBUFAD);              // 3007FF0h  4    R/W  SNDBUFAD  Pointer to Sound Buffer

  // LCD I/O Registers
  static volatile auto& DISPCNT     = *reinterpret_cast<std::uint16_t volatile*>(address::DISPCNT);       // 4000000h  2    R/W  DISPCNT   LCD Control
  //                                                                                                      // 4000002h  2    R/W  -         Undocumented - Green Swap
  static volatile auto& DISPSTAT    = *reinterpret_cast<std::uint16_t volatile*>(address::DISPSTAT);      // 4000004h  2    R/W  DISPSTAT  General LCD Status (STAT,LYC)
  static volatile auto& VCOUNT      = *reinterpret_cast<std::uint16_t volatile*>(address::VCOUNT);        // 4000006h  2    R    VCOUNT    Vertical Counter (LY)
  static volatile auto& BG0CNT      = *reinterpret_cast<std::uint16_t volatile*>(address::BG0CNT);        // 4000008h  2    R/W  BG0CNT    BG0 Control
  static volatile auto& BG1CNT      = *reinterpret_cast<std::uint16_t volatile*>(address::BG1CNT);        // 400000Ah  2    R/W  BG1CNT    BG1 Control
  static volatile auto& BG2CNT      = *reinterpret_cast<std::uint16_t volatile*>(address::BG2CNT);        // 400000Ch  2    R/W  BG2CNT    BG2 Control
  static volatile auto& BG3CNT      = *reinterpret_cast<std::uint16_t volatile*>(address::BG3CNT);        // 400000Eh  2    R/W  BG3CNT    BG3 Control
  static volatile auto& BG0HOFS     = *reinterpret_cast<std::uint16_t volatile*>(address::BG0HOFS);       // 4000010h  2    W    BG0HOFS   BG0 X-Offset
  static volatile auto& BG0VOFS     = *reinterpret_cast<std::uint16_t volatile*>(address::BG0VOFS);       // 4000012h  2    W    BG0VOFS   BG0 Y-Offset
  static volatile auto& BG1HOFS     = *reinterpret_cast<std::uint16_t volatile*>(address::BG1HOFS);       // 4000014h  2    W    BG1HOFS   BG1 X-Offset
  static volatile auto& BG1VOFS     = *reinterpret_cast<std::uint16_t volatile*>(address::BG1VOFS);       // 4000016h  2    W    BG1VOFS   BG1 Y-Offset
  static volatile auto& BG2HOFS     = *reinterpret_cast<std::uint16_t volatile*>(address::BG2HOFS);       // 4000018h  2    W    BG2HOFS   BG2 X-Offset
  static volatile auto& BG2VOFS     = *reinterpret_cast<std::uint16_t volatile*>(address::BG2VOFS);       // 400001Ah  2    W    BG2VOFS   BG2 Y-Offset
  static volatile auto& BG3HOFS     = *reinterpret_cast<std::uint16_t volatile*>(address::BG3HOFS);       // 400001Ch  2    W    BG3HOFS   BG3 X-Offset
  static volatile auto& BG3VOFS     = *reinterpret_cast<std::uint16_t volatile*>(address::BG3VOFS);       // 400001Eh  2    W    BG3VOFS   BG3 Y-Offset
  static volatile auto& BG2PA       = *reinterpret_cast<std::uint16_t volatile*>(address::BG2PA);         // 4000020h  2    W    BG2PA     BG2 Rotation/Scaling Parameter A (dx)
  static volatile auto& BG2PB       = *reinterpret_cast<std::uint16_t volatile*>(address::BG2PB);         // 4000022h  2    W    BG2PB     BG2 Rotation/Scaling Parameter B (dmx)
  static volatile auto& BG2PC       = *reinterpret_cast<std::uint16_t volatile*>(address::BG2PC);         // 4000024h  2    W    BG2PC     BG2 Rotation/Scaling Parameter C (dy)
  static volatile auto& BG2PD       = *reinterpret_cast<std::uint16_t volatile*>(address::BG2PD);         // 4000026h  2    W    BG2PD     BG2 Rotation/Scaling Parameter D (dmy)
  static volatile auto& BG2X        = *reinterpret_cast<std::uint32_t volatile*>(address::BG2X);          // 4000028h  4    W    BG2X      BG2 Reference Point X-Coordinate
  static volatile auto& BG2Y        = *reinterpret_cast<std::uint32_t volatile*>(address::BG2Y);          // 400002Ch  4    W    BG2Y      BG2 Reference Point Y-Coordinate
  static volatile auto& BG3PA       = *reinterpret_cast<std::uint16_t volatile*>(address::BG3PA);         // 4000030h  2    W    BG3PA     BG3 Rotation/Scaling Parameter A (dx)
  static volatile auto& BG3PB       = *reinterpret_cast<std::uint16_t volatile*>(address::BG3PB);         // 4000032h  2    W    BG3PB     BG3 Rotation/Scaling Parameter B (dmx)
  static volatile auto& BG3PC       = *reinterpret_cast<std::uint16_t volatile*>(address::BG3PC);         // 4000034h  2    W    BG3PC     BG3 Rotation/Scaling Parameter C (dy)
  static volatile auto& BG3PD       = *reinterpret_cast<std::uint16_t volatile*>(address::BG3PD);         // 4000036h  2    W    BG3PD     BG3 Rotation/Scaling Parameter D (dmy)
  static volatile auto& BG3X        = *reinterpret_cast<std::uint32_t volatile*>(address::BG3X);          // 4000038h  4    W    BG3X      BG3 Reference Point X-Coordinate
  static volatile auto& BG3Y        = *reinterpret_cast<std::uint32_t volatile*>(address::BG3Y);          // 400003Ch  4    W    BG3Y      BG3 Reference Point Y-Coordinate
  static volatile auto& WIN0H       = *reinterpret_cast<std::uint16_t volatile*>(address::WIN0H);         // 4000040h  2    W    WIN0H     Window 0 Horizontal Dimensions
  static volatile auto& WIN1H       = *reinterpret_cast<std::uint16_t volatile*>(address::WIN1H);         // 4000042h  2    W    WIN1H     Window 1 Horizontal Dimensions
  static volatile auto& WIN0V       = *reinterpret_cast<std::uint16_t volatile*>(address::WIN0V);         // 4000044h  2    W    WIN0V     Window 0 Vertical Dimensions
  static volatile auto& WIN1V       = *reinterpret_cast<std::uint16_t volatile*>(address::WIN1V);         // 4000046h  2    W    WIN1V     Window 1 Vertical Dimensions
  static volatile auto& WININ       = *reinterpret_cast<std::uint16_t volatile*>(address::WININ);         // 4000048h  2    R/W  WININ     Inside of Window 0 and 1
  static volatile auto& WINOUT      = *reinterpret_cast<std::uint16_t volatile*>(address::WINOUT);        // 400004Ah  2    R/W  WINOUT    Inside of OBJ Window & Outside of Windows
  static volatile auto& MOSAIC      = *reinterpret_cast<std::uint16_t volatile*>(address::MOSAIC);        // 400004Ch  2    W    MOSAIC    Mosaic Size
  //                                                                                                      // 400004Eh       -    -         Not used
  static volatile auto& BLDCNT      = *reinterpret_cast<std::uint16_t volatile*>(address::BLDCNT);        // 4000050h  2    R/W  BLDCNT    Color Special Effects Selection
  static volatile auto& BLDALPHA    = *reinterpret_cast<std::uint16_t volatile*>(address::BLDALPHA);      // 4000052h  2    R/W  BLDALPHA  Alpha Blending Coefficients
  static volatile auto& BLDY        = *reinterpret_cast<std::uint16_t volatile*>(address::BLDY);          // 4000054h  2    W    BLDY      Brightness (Fade-In/Out) Coefficient
  //                                                                                                      // 4000056h       -    -         Not used

  // Sound Registers
  static volatile auto& SOUND1CNT_L = *reinterpret_cast<std::uint16_t volatile*>(address::SOUND1CNT_L);   // 4000060h  2  R/W  SOUND1CNT_L Channel 1 Sweep register       (NR10)
  static volatile auto& SOUND1CNT_H = *reinterpret_cast<std::uint16_t volatile*>(address::SOUND1CNT_H);   // 4000062h  2  R/W  SOUND1CNT_H Channel 1 Duty/Length/Envelope (NR11, NR12)
  static volatile auto& SOUND1CNT_X = *reinterpret_cast<std::uint16_t volatile*>(address::SOUND1CNT_X);   // 4000064h  2  R/W  SOUND1CNT_X Channel 1 Frequency/Control    (NR13, NR14)
  //                                                                                                      // 4000066h     -    -           Not used
  static volatile auto& SOUND2CNT_L = *reinterpret_cast<std::uint16_t volatile*>(address::SOUND2CNT_L);   // 4000068h  2  R/W  SOUND2CNT_L Channel 2 Duty/Length/Envelope (NR21, NR22)
  //                                                                                                      // 400006Ah     -    -           Not used
  static volatile auto& SOUND2CNT_H = *reinterpret_cast<std::uint16_t volatile*>(address::SOUND2CNT_H);   // 400006Ch  2  R/W  SOUND2CNT_H Channel 2 Frequency/Control    (NR23, NR24)
  //                                                                                                      // 400006Eh     -    -           Not used
  static volatile auto& SOUND3CNT_L = *reinterpret_cast<std::uint16_t volatile*>(address::SOUND3CNT_L);   // 4000070h  2  R/W  SOUND3CNT_L Channel 3 Stop/Wave RAM select (NR30)
  static volatile auto& SOUND3CNT_H = *reinterpret_cast<std::uint16_t volatile*>(address::SOUND3CNT_H);   // 4000072h  2  R/W  SOUND3CNT_H Channel 3 Length/Volume        (NR31, NR32)
  static volatile auto& SOUND3CNT_X = *reinterpret_cast<std::uint16_t volatile*>(address::SOUND3CNT_X);   // 4000074h  2  R/W  SOUND3CNT_X Channel 3 Frequency/Control    (NR33, NR34)
  //                                                                                                      // 4000076h     -    -           Not used
  static volatile auto& SOUND4CNT_L = *reinterpret_cast<std::uint16_t volatile*>(address::SOUND4CNT_L);   // 4000078h  2  R/W  SOUND4CNT_L Channel 4 Length/Envelope      (NR41, NR42)
  //                                                                                                      // 400007Ah     -    -           Not used
  static volatile auto& SOUND4CNT_H = *reinterpret_cast<std::uint16_t volatile*>(address::SOUND4CNT_H);   // 400007Ch  2  R/W  SOUND4CNT_H Channel 4 Frequency/Control    (NR43, NR44)
  //                                                                                                      // 400007Eh     -    -           Not used
  static volatile auto& SOUNDCNT_L  = *reinterpret_cast<std::uint16_t volatile*>(address::SOUNDCNT_L);    // 4000080h  2  R/W  SOUNDCNT_L  Control Stereo/Volume/Enable   (NR50, NR51)
  static volatile auto& SOUNDCNT_H  = *reinterpret_cast<std::uint16_t volatile*>(address::SOUNDCNT_H);    // 4000082h  2  R/W  SOUNDCNT_H  Control Mixing/DMA Control
  static volatile auto& SOUNDCNT_X  = *reinterpret_cast<std::uint16_t volatile*>(address::SOUNDCNT_X);    // 4000084h  2  R/W  SOUNDCNT_X  Control Sound on/off           (NR52)
  //                                                                                                      // 4000086h     -    -           Not used
  static volatile auto& SOUNDBIAS   = *reinterpret_cast<std::uint16_t volatile*>(address::SOUNDBIAS);     // 4000088h  2  BIOS SOUNDBIAS   Sound PWM Control
  //                                                                                                      // 400008Ah  ..   -    -         Not used
  //                                                                                                      // 4000090h 2x10h R/W  WAVE_RAM  Channel 3 Wave Pattern RAM (2 banks!!)
  static volatile auto& WAVE_RAM0_L = *reinterpret_cast<std::uint16_t volatile*>(address::WAVE_RAM0_L);   // 4000090h  2  R/W  WAVE_RAM0_L Channel 3 Wave Pattern RAM
  static volatile auto& WAVE_RAM0_H = *reinterpret_cast<std::uint16_t volatile*>(address::WAVE_RAM0_H);   // 4000092h  2  R/W  WAVE_RAM0_H Channel 3 Wave Pattern RAM
  static volatile auto& WAVE_RAM1_L = *reinterpret_cast<std::uint16_t volatile*>(address::WAVE_RAM1_L);   // 4000094h  2  R/W  WAVE_RAM1_L Channel 3 Wave Pattern RAM
  static volatile auto& WAVE_RAM1_H = *reinterpret_cast<std::uint16_t volatile*>(address::WAVE_RAM1_H);   // 4000096h  2  R/W  WAVE_RAM1_H Channel 3 Wave Pattern RAM
  static volatile auto& WAVE_RAM2_L = *reinterpret_cast<std::uint16_t volatile*>(address::WAVE_RAM2_L);   // 4000098h  2  R/W  WAVE_RAM2_L Channel 3 Wave Pattern RAM
  static volatile auto& WAVE_RAM2_H = *reinterpret_cast<std::uint16_t volatile*>(address::WAVE_RAM2_H);   // 400009Ah  2  R/W  WAVE_RAM2_H Channel 3 Wave Pattern RAM
  static volatile auto& WAVE_RAM3_L = *reinterpret_cast<std::uint16_t volatile*>(address::WAVE_RAM3_L);   // 400009Ch  2  R/W  WAVE_RAM3_L Channel 3 Wave Pattern RAM
  static volatile auto& WAVE_RAM3_H = *reinterpret_cast<std::uint16_t volatile*>(address::WAVE_RAM3_H);   // 400009Eh  2  R/W  WAVE_RAM3_H Channel 3 Wave Pattern RAM
  static volatile auto& FIFO_A      = *reinterpret_cast<void* volatile*>(address::FIFO_A);                // 40000A0h  4    W    FIFO_A    Channel A FIFO, Data 0-3
  static volatile auto& FIFO_B      = *reinterpret_cast<void* volatile*>(address::FIFO_B);                // 40000A4h  4    W    FIFO_B    Channel B FIFO, Data 0-3
  //                                                                                                      // 40000A8h       -    -         Not used

  // DMA Transfer Channels
  static volatile auto& DMA0SAD     = *reinterpret_cast<const void* volatile*>(address::DMA0SAD);         // 40000B0h  4    W    DMA0SAD   DMA 0 Source Address
  static volatile auto& DMA0DAD     = *reinterpret_cast<void* volatile*>(address::DMA0DAD);               // 40000B4h  4    W    DMA0DAD   DMA 0 Destination Address
  static volatile auto& DMA0CNT_L   = *reinterpret_cast<std::uint16_t volatile*>(address::DMA0CNT_L);     // 40000B8h  2    W    DMA0CNT_L DMA 0 Word Count
  static volatile auto& DMA0CNT_H   = *reinterpret_cast<std::uint16_t volatile*>(address::DMA0CNT_H);     // 40000BAh  2    R/W  DMA0CNT_H DMA 0 Control
  static volatile auto& DMA1SAD     = *reinterpret_cast<const void* volatile*>(address::DMA1SAD);         // 40000BCh  4    W    DMA1SAD   DMA 1 Source Address
  static volatile auto& DMA1DAD     = *reinterpret_cast<void* volatile*>(address::DMA1DAD);               // 40000C0h  4    W    DMA1DAD   DMA 1 Destination Address
  static volatile auto& DMA1CNT_L   = *reinterpret_cast<std::uint16_t volatile*>(address::DMA1CNT_L);     // 40000C4h  2    W    DMA1CNT_L DMA 1 Word Count
  static volatile auto& DMA1CNT_H   = *reinterpret_cast<std::uint16_t volatile*>(address::DMA1CNT_H);     // 40000C6h  2    R/W  DMA1CNT_H DMA 1 Control
  static volatile auto& DMA2SAD     = *reinterpret_cast<const void* volatile*>(address::DMA2SAD);         // 40000C8h  4    W    DMA2SAD   DMA 2 Source Address
  static volatile auto& DMA2DAD     = *reinterpret_cast<void* volatile*>(address::DMA2DAD);               // 40000CCh  4    W    DMA2DAD   DMA 2 Destination Address
  static volatile auto& DMA2CNT_L   = *reinterpret_cast<std::uint16_t volatile*>(address::DMA2CNT_L);     // 40000D0h  2    W    DMA2CNT_L DMA 2 Word Count
  static volatile auto& DMA2CNT_H   = *reinterpret_cast<std::uint16_t volatile*>(address::DMA2CNT_H);     // 40000D2h  2    R/W  DMA2CNT_H DMA 2 Control
  static volatile auto& DMA3SAD     = *reinterpret_cast<const void* volatile*>(address::DMA3SAD);         // 40000D4h  4    W    DMA3SAD   DMA 3 Source Address
  static volatile auto& DMA3DAD     = *reinterpret_cast<void* volatile*>(address::DMA3DAD);               // 40000D8h  4    W    DMA3DAD   DMA 3 Destination Address
  static volatile auto& DMA3CNT_L   = *reinterpret_cast<std::uint16_t volatile*>(address::DMA3CNT_L);     // 40000DCh  2    W    DMA3CNT_L DMA 3 Word Count
  static volatile auto& DMA3CNT_H   = *reinterpret_cast<std::uint16_t volatile*>(address::DMA3CNT_H);     // 40000DEh  2    R/W  DMA3CNT_H DMA 3 Control
  //                                                                                                      // 40000E0h       -    -         Not used

  // Timer Registers
  static volatile auto& TM0CNT_L    = *reinterpret_cast<std::uint16_t volatile*>(address::TM0CNT_L);      // 4000100h  2    R/W  TM0CNT_L  Timer 0 Counter/Reload
  static volatile auto& TM0CNT_H    = *reinterpret_cast<std::uint16_t volatile*>(address::TM0CNT_H);      // 4000102h  2    R/W  TM0CNT_H  Timer 0 Control
  static volatile auto& TM1CNT_L    = *reinterpret_cast<std::uint16_t volatile*>(address::TM1CNT_L);      // 4000104h  2    R/W  TM1CNT_L  Timer 1 Counter/Reload
  static volatile auto& TM1CNT_H    = *reinterpret_cast<std::uint16_t volatile*>(address::TM1CNT_H);      // 4000106h  2    R/W  TM1CNT_H  Timer 1 Control
  static volatile auto& TM2CNT_L    = *reinterpret_cast<std::uint16_t volatile*>(address::TM2CNT_L);      // 4000108h  2    R/W  TM2CNT_L  Timer 2 Counter/Reload
  static volatile auto& TM2CNT_H    = *reinterpret_cast<std::uint16_t volatile*>(address::TM2CNT_H);      // 400010Ah  2    R/W  TM2CNT_H  Timer 2 Control
  static volatile auto& TM3CNT_L    = *reinterpret_cast<std::uint16_t volatile*>(address::TM3CNT_L);      // 400010Ch  2    R/W  TM3CNT_L  Timer 3 Counter/Reload
  static volatile auto& TM3CNT_H    = *reinterpret_cast<std::uint16_t volatile*>(address::TM3CNT_H);      // 400010Eh  2    R/W  TM3CNT_H  Timer 3 Control
  //                                                                                                      // 4000110h       -    -         Not used

  // Serial Communication (1)
  static volatile auto& SIODATA32   = *reinterpret_cast<std::uint32_t volatile*>(address::SIODATA32);     // 4000120h  4    R/W  SIODATA32 SIO Data (Normal-32bit Mode; shared with below)
  static volatile auto& SIOMULTI0   = *reinterpret_cast<std::uint16_t volatile*>(address::SIOMULTI0);     // 4000120h  2    R/W  SIOMULTI0 SIO Data 0 (Parent)    (Multi-Player Mode)
  static volatile auto& SIOMULTI1   = *reinterpret_cast<std::uint16_t volatile*>(address::SIOMULTI1);     // 4000122h  2    R/W  SIOMULTI1 SIO Data 1 (1st Child) (Multi-Player Mode)
  static volatile auto& SIOMULTI2   = *reinterpret_cast<std::uint16_t volatile*>(address::SIOMULTI2);     // 4000124h  2    R/W  SIOMULTI2 SIO Data 2 (2nd Child) (Multi-Player Mode)
  static volatile auto& SIOMULTI3   = *reinterpret_cast<std::uint16_t volatile*>(address::SIOMULTI3);     // 4000126h  2    R/W  SIOMULTI3 SIO Data 3 (3rd Child) (Multi-Player Mode)
  static volatile auto& SIOCNT      = *reinterpret_cast<std::uint16_t volatile*>(address::SIOCNT);        // 4000128h  2    R/W  SIOCNT    SIO Control Register
  static volatile auto& SIOMLT_SEND = *reinterpret_cast<std::uint16_t volatile*>(address::SIOMLT_SEND);   // 400012Ah  2    R/W  SIOMLT_SEND SIO Data (Local of MultiPlayer; shared below)
  static volatile auto& SIODATA8    = *reinterpret_cast<std::uint16_t volatile*>(address::SIODATA8);      // 400012Ah  2    R/W  SIODATA8  SIO Data (Normal-8bit and UART Mode)
  //                                                                                                      // 400012Ch       -    -         Not used

  // Keypad Input
  static volatile auto& KEYINPUT    = *reinterpret_cast<std::uint16_t volatile*>(address::KEYINPUT);      // 4000130h  2    R    KEYINPUT  Key Status
  static volatile auto& KEYCNT      = *reinterpret_cast<std::uint16_t volatile*>(address::KEYCNT);        // 4000132h  2    R/W  KEYCNT    Key Interrupt Control

  // Serial Communication (2)
  static volatile auto& RCNT        = *reinterpret_cast<std::uint16_t volatile*>(address::RCNT);          // 4000134h  2    R/W  RCNT      SIO Mode Select/General Purpose Data
  //                                                                                                      // 4000136h  -    -    IR        Ancient - Infrared Register (Prototypes only)
  //                                                                                                      // 4000138h       -    -         Not used
  static volatile auto& JOYCNT      = *reinterpret_cast<std::uint16_t volatile*>(address::JOYCNT);        // 4000140h  2    R/W  JOYCNT    SIO JOY Bus Control
  //                                                                                                      // 4000142h       -    -         Not used
  static volatile auto& JOY_RECV    = *reinterpret_cast<std::uint32_t volatile*>(address::JOY_RECV);      // 4000150h  4    R/W  JOY_RECV  SIO JOY Bus Receive Data
  static volatile auto& JOY_TRANS   = *reinterpret_cast<std::uint32_t volatile*>(address::JOY_TRANS);     // 4000154h  4    R/W  JOY_TRANS SIO JOY Bus Transmit Data
  static volatile auto& JOYSTAT     = *reinterpret_cast<std::uint16_t volatile*>(address::JOYSTAT);       // 4000158h  2    R/?  JOYSTAT   SIO JOY Bus Receive Status
  //                                                                                                      // 400015Ah       -    -         Not used

  // Interrupt, Waitstate, and Power-Down Control
  static volatile auto& IE          = *reinterpret_cast<std::uint16_t volatile*>(address::IE);            // 4000200h  2    R/W  IE        Interrupt Enable Register
  static volatile auto& IF          = *reinterpret_cast<std::uint16_t volatile*>(address::IF);            // 4000202h  2    R/W  IF        Interrupt Request Flags / IRQ Acknowledge
  static volatile auto& WAITCNT     = *reinterpret_cast<std::uint16_t volatile*>(address::WAITCNT);       // 4000204h  2    R/W  WAITCNT   Game Pak Waitstate Control
  //                                                                                                      // 4000206h       -    -         Not used
  static volatile auto& IME         = *reinterpret_cast<std::uint16_t volatile*>(address::IME);           // 4000208h  2    R/W  IME       Interrupt Master Enable Register
  //                                                                                                      // 400020Ah       -    -         Not used
  //                                                                                                      // 4000300h  1    R/W  POSTFLG   Undocumented - Post Boot Flag
  //                                                                                                      // 4000301h  1    W    HALTCNT   Undocumented - Power Down Control
  //                                                                                                      // 4000302h       -    -         Not used
  //                                                                                                      // 4000410h  ?    ?    ?         Undocumented - Purpose Unknown / Bug ??? 0FFh
  //                                                                                                      // 4000411h       -    -         Not used
  //                                                                                                      // 4000800h  4    R/W  ?         Undocumented - Internal Memory Control (R/W)
  //                                                                                                      // 4000804h       -    -         Not used
  //                                                                                                      // 4xx0800h  4    R/W  ?         Mirrors of 4000800h (repeated each 64K)
}   // namespace gba::reg

#endif
