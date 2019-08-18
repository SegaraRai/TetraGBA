#ifndef _gba_pointer_hpp_
#define _gba_pointer_hpp_

#include <cstdint>

#include "template_pointer.hpp"


namespace gba::pointer {
  // BIOS RAM
  static const auto ISRAD       = template_pointer::ISRAD<>;          // 3007FFCh  4    R/W  ISRAD     Pointer to user IRQ handler (32bit ARM code)
  static const auto INTCHK      = template_pointer::INTCHK<>;         // 3007FF8h  2    R/W  INTCHK    Interrupt Check Flag (for IntrWait/VBlankIntrWait functions)
  static const auto ALLOCA      = template_pointer::ALLOCA<>;         // 3007FF4h  4    R/W  ALLOCA    Allocated Area
  static const auto SNDBUFAD    = template_pointer::SNDBUFAD<>;       // 3007FF0h  4    R/W  SNDBUFAD  Pointer to Sound Buffer

  // LCD I/O Registers
  static const auto DISPCNT     = template_pointer::DISPCNT<>;        // 4000000h  2    R/W  DISPCNT   LCD Control
  //                                                                  // 4000002h  2    R/W  -         Undocumented - Green Swap
  static const auto DISPSTAT    = template_pointer::DISPSTAT<>;       // 4000004h  2    R/W  DISPSTAT  General LCD Status (STAT,LYC)
  static const auto VCOUNT      = template_pointer::VCOUNT<>;         // 4000006h  2    R    VCOUNT    Vertical Counter (LY)
  static const auto BG0CNT      = template_pointer::BG0CNT<>;         // 4000008h  2    R/W  BG0CNT    BG0 Control
  static const auto BG1CNT      = template_pointer::BG1CNT<>;         // 400000Ah  2    R/W  BG1CNT    BG1 Control
  static const auto BG2CNT      = template_pointer::BG2CNT<>;         // 400000Ch  2    R/W  BG2CNT    BG2 Control
  static const auto BG3CNT      = template_pointer::BG3CNT<>;         // 400000Eh  2    R/W  BG3CNT    BG3 Control
  static const auto BG0HOFS     = template_pointer::BG0HOFS<>;        // 4000010h  2    W    BG0HOFS   BG0 X-Offset
  static const auto BG0VOFS     = template_pointer::BG0VOFS<>;        // 4000012h  2    W    BG0VOFS   BG0 Y-Offset
  static const auto BG1HOFS     = template_pointer::BG1HOFS<>;        // 4000014h  2    W    BG1HOFS   BG1 X-Offset
  static const auto BG1VOFS     = template_pointer::BG1VOFS<>;        // 4000016h  2    W    BG1VOFS   BG1 Y-Offset
  static const auto BG2HOFS     = template_pointer::BG2HOFS<>;        // 4000018h  2    W    BG2HOFS   BG2 X-Offset
  static const auto BG2VOFS     = template_pointer::BG2VOFS<>;        // 400001Ah  2    W    BG2VOFS   BG2 Y-Offset
  static const auto BG3HOFS     = template_pointer::BG3HOFS<>;        // 400001Ch  2    W    BG3HOFS   BG3 X-Offset
  static const auto BG3VOFS     = template_pointer::BG3VOFS<>;        // 400001Eh  2    W    BG3VOFS   BG3 Y-Offset
  static const auto BG2PA       = template_pointer::BG2PA<>;          // 4000020h  2    W    BG2PA     BG2 Rotation/Scaling Parameter A (dx)
  static const auto BG2PB       = template_pointer::BG2PB<>;          // 4000022h  2    W    BG2PB     BG2 Rotation/Scaling Parameter B (dmx)
  static const auto BG2PC       = template_pointer::BG2PC<>;          // 4000024h  2    W    BG2PC     BG2 Rotation/Scaling Parameter C (dy)
  static const auto BG2PD       = template_pointer::BG2PD<>;          // 4000026h  2    W    BG2PD     BG2 Rotation/Scaling Parameter D (dmy)
  static const auto BG2X        = template_pointer::BG2X<>;           // 4000028h  4    W    BG2X      BG2 Reference Point X-Coordinate
  static const auto BG2Y        = template_pointer::BG2Y<>;           // 400002Ch  4    W    BG2Y      BG2 Reference Point Y-Coordinate
  static const auto BG3PA       = template_pointer::BG3PA<>;          // 4000030h  2    W    BG3PA     BG3 Rotation/Scaling Parameter A (dx)
  static const auto BG3PB       = template_pointer::BG3PB<>;          // 4000032h  2    W    BG3PB     BG3 Rotation/Scaling Parameter B (dmx)
  static const auto BG3PC       = template_pointer::BG3PC<>;          // 4000034h  2    W    BG3PC     BG3 Rotation/Scaling Parameter C (dy)
  static const auto BG3PD       = template_pointer::BG3PD<>;          // 4000036h  2    W    BG3PD     BG3 Rotation/Scaling Parameter D (dmy)
  static const auto BG3X        = template_pointer::BG3X<>;           // 4000038h  4    W    BG3X      BG3 Reference Point X-Coordinate
  static const auto BG3Y        = template_pointer::BG3Y<>;           // 400003Ch  4    W    BG3Y      BG3 Reference Point Y-Coordinate
  static const auto WIN0H       = template_pointer::WIN0H<>;          // 4000040h  2    W    WIN0H     Window 0 Horizontal Dimensions
  static const auto WIN1H       = template_pointer::WIN1H<>;          // 4000042h  2    W    WIN1H     Window 1 Horizontal Dimensions
  static const auto WIN0V       = template_pointer::WIN0V<>;          // 4000044h  2    W    WIN0V     Window 0 Vertical Dimensions
  static const auto WIN1V       = template_pointer::WIN1V<>;          // 4000046h  2    W    WIN1V     Window 1 Vertical Dimensions
  static const auto WININ       = template_pointer::WININ<>;          // 4000048h  2    R/W  WININ     Inside of Window 0 and 1
  static const auto WINOUT      = template_pointer::WINOUT<>;         // 400004Ah  2    R/W  WINOUT    Inside of OBJ Window & Outside of Windows
  static const auto MOSAIC      = template_pointer::MOSAIC<>;         // 400004Ch  2    W    MOSAIC    Mosaic Size
  //                                                                  // 400004Eh       -    -         Not used
  static const auto BLDCNT      = template_pointer::BLDCNT<>;         // 4000050h  2    R/W  BLDCNT    Color Special Effects Selection
  static const auto BLDALPHA    = template_pointer::BLDALPHA<>;       // 4000052h  2    R/W  BLDALPHA  Alpha Blending Coefficients
  static const auto BLDY        = template_pointer::BLDY<>;           // 4000054h  2    W    BLDY      Brightness (Fade-In/Out) Coefficient
  //                                                                  // 4000056h       -    -         Not used

  // Sound Registers
  static const auto SOUND1CNT_L = template_pointer::SOUND1CNT_L<>;    // 4000060h  2  R/W  SOUND1CNT_L Channel 1 Sweep register       (NR10)
  static const auto SOUND1CNT_H = template_pointer::SOUND1CNT_H<>;    // 4000062h  2  R/W  SOUND1CNT_H Channel 1 Duty/Length/Envelope (NR11, NR12)
  static const auto SOUND1CNT_X = template_pointer::SOUND1CNT_X<>;    // 4000064h  2  R/W  SOUND1CNT_X Channel 1 Frequency/Control    (NR13, NR14)
  //                                                                  // 4000066h     -    -           Not used
  static const auto SOUND2CNT_L = template_pointer::SOUND2CNT_L<>;    // 4000068h  2  R/W  SOUND2CNT_L Channel 2 Duty/Length/Envelope (NR21, NR22)
  //                                                                  // 400006Ah     -    -           Not used
  static const auto SOUND2CNT_H = template_pointer::SOUND2CNT_H<>;    // 400006Ch  2  R/W  SOUND2CNT_H Channel 2 Frequency/Control    (NR23, NR24)
  //                                                                  // 400006Eh     -    -           Not used
  static const auto SOUND3CNT_L = template_pointer::SOUND3CNT_L<>;    // 4000070h  2  R/W  SOUND3CNT_L Channel 3 Stop/Wave RAM select (NR30)
  static const auto SOUND3CNT_H = template_pointer::SOUND3CNT_H<>;    // 4000072h  2  R/W  SOUND3CNT_H Channel 3 Length/Volume        (NR31, NR32)
  static const auto SOUND3CNT_X = template_pointer::SOUND3CNT_X<>;    // 4000074h  2  R/W  SOUND3CNT_X Channel 3 Frequency/Control    (NR33, NR34)
  //                                                                  // 4000076h     -    -           Not used
  static const auto SOUND4CNT_L = template_pointer::SOUND4CNT_L<>;    // 4000078h  2  R/W  SOUND4CNT_L Channel 4 Length/Envelope      (NR41, NR42)
  //                                                                  // 400007Ah     -    -           Not used
  static const auto SOUND4CNT_H = template_pointer::SOUND4CNT_H<>;    // 400007Ch  2  R/W  SOUND4CNT_H Channel 4 Frequency/Control    (NR43, NR44)
  //                                                                  // 400007Eh     -    -           Not used
  static const auto SOUNDCNT_L  = template_pointer::SOUNDCNT_L<>;     // 4000080h  2  R/W  SOUNDCNT_L  Control Stereo/Volume/Enable   (NR50, NR51)
  static const auto SOUNDCNT_H  = template_pointer::SOUNDCNT_H<>;     // 4000082h  2  R/W  SOUNDCNT_H  Control Mixing/DMA Control
  static const auto SOUNDCNT_X  = template_pointer::SOUNDCNT_X<>;     // 4000084h  2  R/W  SOUNDCNT_X  Control Sound on/off           (NR52)
  //                                                                  // 4000086h     -    -           Not used
  static const auto SOUNDBIAS   = template_pointer::SOUNDBIAS<>;      // 4000088h  2  BIOS SOUNDBIAS   Sound PWM Control
  //                                                                  // 400008Ah  ..   -    -         Not used
  //                                                                  // 4000090h 2x10h R/W  WAVE_RAM  Channel 3 Wave Pattern RAM (2 banks!!)
  static const auto WAVE_RAM0_L = template_pointer::WAVE_RAM0_L<>;    // 4000090h  2  R/W  WAVE_RAM0_L Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM0_H = template_pointer::WAVE_RAM0_H<>;    // 4000092h  2  R/W  WAVE_RAM0_H Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM1_L = template_pointer::WAVE_RAM1_L<>;    // 4000094h  2  R/W  WAVE_RAM1_L Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM1_H = template_pointer::WAVE_RAM1_H<>;    // 4000096h  2  R/W  WAVE_RAM1_H Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM2_L = template_pointer::WAVE_RAM2_L<>;    // 4000098h  2  R/W  WAVE_RAM2_L Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM2_H = template_pointer::WAVE_RAM2_H<>;    // 400009Ah  2  R/W  WAVE_RAM2_H Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM3_L = template_pointer::WAVE_RAM3_L<>;    // 400009Ch  2  R/W  WAVE_RAM3_L Channel 3 Wave Pattern RAM
  static const auto WAVE_RAM3_H = template_pointer::WAVE_RAM3_H<>;    // 400009Eh  2  R/W  WAVE_RAM3_H Channel 3 Wave Pattern RAM
  static const auto FIFO_A      = template_pointer::FIFO_A<>;         // 40000A0h  4    W    FIFO_A    Channel A FIFO, Data 0-3
  static const auto FIFO_B      = template_pointer::FIFO_B<>;         // 40000A4h  4    W    FIFO_B    Channel B FIFO, Data 0-3
  //                                                                  // 40000A8h       -    -         Not used

  // DMA Transfer Channels
  static const auto DMA0SAD     = template_pointer::DMA0SAD<>;        // 40000B0h  4    W    DMA0SAD   DMA 0 Source Address
  static const auto DMA0DAD     = template_pointer::DMA0DAD<>;        // 40000B4h  4    W    DMA0DAD   DMA 0 Destination Address
  static const auto DMA0CNT_L   = template_pointer::DMA0CNT_L<>;      // 40000B8h  2    W    DMA0CNT_L DMA 0 Word Count
  static const auto DMA0CNT_H   = template_pointer::DMA0CNT_H<>;      // 40000BAh  2    R/W  DMA0CNT_H DMA 0 Control
  static const auto DMA1SAD     = template_pointer::DMA1SAD<>;        // 40000BCh  4    W    DMA1SAD   DMA 1 Source Address
  static const auto DMA1DAD     = template_pointer::DMA1DAD<>;        // 40000C0h  4    W    DMA1DAD   DMA 1 Destination Address
  static const auto DMA1CNT_L   = template_pointer::DMA1CNT_L<>;      // 40000C4h  2    W    DMA1CNT_L DMA 1 Word Count
  static const auto DMA1CNT_H   = template_pointer::DMA1CNT_H<>;      // 40000C6h  2    R/W  DMA1CNT_H DMA 1 Control
  static const auto DMA2SAD     = template_pointer::DMA2SAD<>;        // 40000C8h  4    W    DMA2SAD   DMA 2 Source Address
  static const auto DMA2DAD     = template_pointer::DMA2DAD<>;        // 40000CCh  4    W    DMA2DAD   DMA 2 Destination Address
  static const auto DMA2CNT_L   = template_pointer::DMA2CNT_L<>;      // 40000D0h  2    W    DMA2CNT_L DMA 2 Word Count
  static const auto DMA2CNT_H   = template_pointer::DMA2CNT_H<>;      // 40000D2h  2    R/W  DMA2CNT_H DMA 2 Control
  static const auto DMA3SAD     = template_pointer::DMA3SAD<>;        // 40000D4h  4    W    DMA3SAD   DMA 3 Source Address
  static const auto DMA3DAD     = template_pointer::DMA3DAD<>;        // 40000D8h  4    W    DMA3DAD   DMA 3 Destination Address
  static const auto DMA3CNT_L   = template_pointer::DMA3CNT_L<>;      // 40000DCh  2    W    DMA3CNT_L DMA 3 Word Count
  static const auto DMA3CNT_H   = template_pointer::DMA3CNT_H<>;      // 40000DEh  2    R/W  DMA3CNT_H DMA 3 Control
  //                                                                  // 40000E0h       -    -         Not used

  // Timer Registers
  static const auto TM0CNT_L    = template_pointer::TM0CNT_L<>;       // 4000100h  2    R/W  TM0CNT_L  Timer 0 Counter/Reload
  static const auto TM0CNT_H    = template_pointer::TM0CNT_H<>;       // 4000102h  2    R/W  TM0CNT_H  Timer 0 Control
  static const auto TM1CNT_L    = template_pointer::TM1CNT_L<>;       // 4000104h  2    R/W  TM1CNT_L  Timer 1 Counter/Reload
  static const auto TM1CNT_H    = template_pointer::TM1CNT_H<>;       // 4000106h  2    R/W  TM1CNT_H  Timer 1 Control
  static const auto TM2CNT_L    = template_pointer::TM2CNT_L<>;       // 4000108h  2    R/W  TM2CNT_L  Timer 2 Counter/Reload
  static const auto TM2CNT_H    = template_pointer::TM2CNT_H<>;       // 400010Ah  2    R/W  TM2CNT_H  Timer 2 Control
  static const auto TM3CNT_L    = template_pointer::TM3CNT_L<>;       // 400010Ch  2    R/W  TM3CNT_L  Timer 3 Counter/Reload
  static const auto TM3CNT_H    = template_pointer::TM3CNT_H<>;       // 400010Eh  2    R/W  TM3CNT_H  Timer 3 Control
  //                                                                  // 4000110h       -    -         Not used

  // Serial Communication (1)
  static const auto SIODATA32   = template_pointer::SIODATA32<>;      // 4000120h  4    R/W  SIODATA32 SIO Data (Normal-32bit Mode; shared with below)
  static const auto SIOMULTI0   = template_pointer::SIOMULTI0<>;      // 4000120h  2    R/W  SIOMULTI0 SIO Data 0 (Parent)    (Multi-Player Mode)
  static const auto SIOMULTI1   = template_pointer::SIOMULTI1<>;      // 4000122h  2    R/W  SIOMULTI1 SIO Data 1 (1st Child) (Multi-Player Mode)
  static const auto SIOMULTI2   = template_pointer::SIOMULTI2<>;      // 4000124h  2    R/W  SIOMULTI2 SIO Data 2 (2nd Child) (Multi-Player Mode)
  static const auto SIOMULTI3   = template_pointer::SIOMULTI3<>;      // 4000126h  2    R/W  SIOMULTI3 SIO Data 3 (3rd Child) (Multi-Player Mode)
  static const auto SIOCNT      = template_pointer::SIOCNT<>;         // 4000128h  2    R/W  SIOCNT    SIO Control Register
  static const auto SIOMLT_SEND = template_pointer::SIOMLT_SEND<>;    // 400012Ah  2    R/W  SIOMLT_SEND SIO Data (Local of MultiPlayer; shared below)
  static const auto SIODATA8    = template_pointer::SIODATA8<>;       // 400012Ah  2    R/W  SIODATA8  SIO Data (Normal-8bit and UART Mode)
  //                                                                  // 400012Ch       -    -         Not used

  // Keypad Input
  static const auto KEYINPUT    = template_pointer::KEYINPUT<>;       // 4000130h  2    R    KEYINPUT  Key Status
  static const auto KEYCNT      = template_pointer::KEYCNT<>;         // 4000132h  2    R/W  KEYCNT    Key Interrupt Control

  // Serial Communication (2)
  static const auto RCNT        = template_pointer::RCNT<>;           // 4000134h  2    R/W  RCNT      SIO Mode Select/General Purpose Data
  //                                                                  // 4000136h  -    -    IR        Ancient - Infrared Register (Prototypes only)
  //                                                                  // 4000138h       -    -         Not used
  static const auto JOYCNT      = template_pointer::JOYCNT<>;         // 4000140h  2    R/W  JOYCNT    SIO JOY Bus Control
  //                                                                  // 4000142h       -    -         Not used
  static const auto JOY_RECV    = template_pointer::JOY_RECV<>;       // 4000150h  4    R/W  JOY_RECV  SIO JOY Bus Receive Data
  static const auto JOY_TRANS   = template_pointer::JOY_TRANS<>;      // 4000154h  4    R/W  JOY_TRANS SIO JOY Bus Transmit Data
  static const auto JOYSTAT     = template_pointer::JOYSTAT<>;        // 4000158h  2    R/?  JOYSTAT   SIO JOY Bus Receive Status
  //                                                                  // 400015Ah       -    -         Not used

  // Interrupt, Waitstate, and Power-Down Control
  static const auto IE          = template_pointer::IE<>;             // 4000200h  2    R/W  IE        Interrupt Enable Register
  static const auto IF          = template_pointer::IF<>;             // 4000202h  2    R/W  IF        Interrupt Request Flags / IRQ Acknowledge
  static const auto WAITCNT     = template_pointer::WAITCNT<>;        // 4000204h  2    R/W  WAITCNT   Game Pak Waitstate Control
  //                                                                  // 4000206h       -    -         Not used
  static const auto IME         = template_pointer::IME<>;            // 4000208h  2    R/W  IME       Interrupt Master Enable Register
  //                                                                  // 400020Ah       -    -         Not used
  //                                                                  // 4000300h  1    R/W  POSTFLG   Undocumented - Post Boot Flag
  //                                                                  // 4000301h  1    W    HALTCNT   Undocumented - Power Down Control
  //                                                                  // 4000302h       -    -         Not used
  //                                                                  // 4000410h  ?    ?    ?         Undocumented - Purpose Unknown / Bug ??? 0FFh
  //                                                                  // 4000411h       -    -         Not used
  //                                                                  // 4000800h  4    R/W  ?         Undocumented - Internal Memory Control (R/W)
  //                                                                  // 4000804h       -    -         Not used
  //                                                                  // 4xx0800h  4    R/W  ?         Mirrors of 4000800h (repeated each 64K)
}   // namespace gba::pointer

#endif
