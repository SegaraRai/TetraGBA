#ifndef _gba_address_hpp_
#define _gba_address_hpp_

#include <cstdint>


namespace gba::address {
  // BIOS RAM
  constexpr std::uintptr_t ISRAD       = 0x03007FFC;    // 3007FFCh  4    R/W  ISRAD     Pointer to user IRQ handler (32bit ARM code)
  constexpr std::uintptr_t INTCHK      = 0x03007FF8;    // 3007FF8h  2    R/W  INTCHK    Interrupt Check Flag (for IntrWait/VBlankIntrWait functions)
  constexpr std::uintptr_t ALLOCA      = 0x03007FF4;    // 3007FF4h  4    R/W  ALLOCA    Allocated Area
  constexpr std::uintptr_t SNDBUFAD    = 0x03007FF0;    // 3007FF0h  4    R/W  SNDBUFAD  Pointer to Sound Buffer

  // LCD I/O Registers
  constexpr std::uintptr_t DISPCNT     = 0x04000000;    // 4000000h  2    R/W  DISPCNT   LCD Control
  //                                                    // 4000002h  2    R/W  -         Undocumented - Green Swap
  constexpr std::uintptr_t DISPSTAT    = 0x04000004;    // 4000004h  2    R/W  DISPSTAT  General LCD Status (STAT,LYC)
  constexpr std::uintptr_t VCOUNT      = 0x04000006;    // 4000006h  2    R    VCOUNT    Vertical Counter (LY)
  constexpr std::uintptr_t BG0CNT      = 0x04000008;    // 4000008h  2    R/W  BG0CNT    BG0 Control
  constexpr std::uintptr_t BG1CNT      = 0x0400000A;    // 400000Ah  2    R/W  BG1CNT    BG1 Control
  constexpr std::uintptr_t BG2CNT      = 0x0400000C;    // 400000Ch  2    R/W  BG2CNT    BG2 Control
  constexpr std::uintptr_t BG3CNT      = 0x0400000E;    // 400000Eh  2    R/W  BG3CNT    BG3 Control
  constexpr std::uintptr_t BG0HOFS     = 0x04000010;    // 4000010h  2    W    BG0HOFS   BG0 X-Offset
  constexpr std::uintptr_t BG0VOFS     = 0x04000012;    // 4000012h  2    W    BG0VOFS   BG0 Y-Offset
  constexpr std::uintptr_t BG1HOFS     = 0x04000014;    // 4000014h  2    W    BG1HOFS   BG1 X-Offset
  constexpr std::uintptr_t BG1VOFS     = 0x04000016;    // 4000016h  2    W    BG1VOFS   BG1 Y-Offset
  constexpr std::uintptr_t BG2HOFS     = 0x04000018;    // 4000018h  2    W    BG2HOFS   BG2 X-Offset
  constexpr std::uintptr_t BG2VOFS     = 0x0400001A;    // 400001Ah  2    W    BG2VOFS   BG2 Y-Offset
  constexpr std::uintptr_t BG3HOFS     = 0x0400001C;    // 400001Ch  2    W    BG3HOFS   BG3 X-Offset
  constexpr std::uintptr_t BG3VOFS     = 0x0400001E;    // 400001Eh  2    W    BG3VOFS   BG3 Y-Offset
  constexpr std::uintptr_t BG2PA       = 0x04000020;    // 4000020h  2    W    BG2PA     BG2 Rotation/Scaling Parameter A (dx)
  constexpr std::uintptr_t BG2PB       = 0x04000022;    // 4000022h  2    W    BG2PB     BG2 Rotation/Scaling Parameter B (dmx)
  constexpr std::uintptr_t BG2PC       = 0x04000024;    // 4000024h  2    W    BG2PC     BG2 Rotation/Scaling Parameter C (dy)
  constexpr std::uintptr_t BG2PD       = 0x04000026;    // 4000026h  2    W    BG2PD     BG2 Rotation/Scaling Parameter D (dmy)
  constexpr std::uintptr_t BG2X        = 0x04000028;    // 4000028h  4    W    BG2X      BG2 Reference Point X-Coordinate
  constexpr std::uintptr_t BG2Y        = 0x0400002C;    // 400002Ch  4    W    BG2Y      BG2 Reference Point Y-Coordinate
  constexpr std::uintptr_t BG3PA       = 0x04000030;    // 4000030h  2    W    BG3PA     BG3 Rotation/Scaling Parameter A (dx)
  constexpr std::uintptr_t BG3PB       = 0x04000032;    // 4000032h  2    W    BG3PB     BG3 Rotation/Scaling Parameter B (dmx)
  constexpr std::uintptr_t BG3PC       = 0x04000034;    // 4000034h  2    W    BG3PC     BG3 Rotation/Scaling Parameter C (dy)
  constexpr std::uintptr_t BG3PD       = 0x04000036;    // 4000036h  2    W    BG3PD     BG3 Rotation/Scaling Parameter D (dmy)
  constexpr std::uintptr_t BG3X        = 0x04000038;    // 4000038h  4    W    BG3X      BG3 Reference Point X-Coordinate
  constexpr std::uintptr_t BG3Y        = 0x0400003C;    // 400003Ch  4    W    BG3Y      BG3 Reference Point Y-Coordinate
  constexpr std::uintptr_t WIN0H       = 0x04000040;    // 4000040h  2    W    WIN0H     Window 0 Horizontal Dimensions
  constexpr std::uintptr_t WIN1H       = 0x04000042;    // 4000042h  2    W    WIN1H     Window 1 Horizontal Dimensions
  constexpr std::uintptr_t WIN0V       = 0x04000044;    // 4000044h  2    W    WIN0V     Window 0 Vertical Dimensions
  constexpr std::uintptr_t WIN1V       = 0x04000046;    // 4000046h  2    W    WIN1V     Window 1 Vertical Dimensions
  constexpr std::uintptr_t WININ       = 0x04000048;    // 4000048h  2    R/W  WININ     Inside of Window 0 and 1
  constexpr std::uintptr_t WINOUT      = 0x0400004A;    // 400004Ah  2    R/W  WINOUT    Inside of OBJ Window & Outside of Windows
  constexpr std::uintptr_t MOSAIC      = 0x0400004C;    // 400004Ch  2    W    MOSAIC    Mosaic Size
  //                                                    // 400004Eh       -    -         Not used
  constexpr std::uintptr_t BLDCNT      = 0x04000050;    // 4000050h  2    R/W  BLDCNT    Color Special Effects Selection
  constexpr std::uintptr_t BLDALPHA    = 0x04000052;    // 4000052h  2    R/W  BLDALPHA  Alpha Blending Coefficients
  constexpr std::uintptr_t BLDY        = 0x04000054;    // 4000054h  2    W    BLDY      Brightness (Fade-In/Out) Coefficient
  //                                                    // 4000056h       -    -         Not used

  // Sound Registers
  constexpr std::uintptr_t SOUND1CNT_L = 0x04000060;    // 4000060h  2  R/W  SOUND1CNT_L Channel 1 Sweep register       (NR10)
  constexpr std::uintptr_t SOUND1CNT_H = 0x04000062;    // 4000062h  2  R/W  SOUND1CNT_H Channel 1 Duty/Length/Envelope (NR11, NR12)
  constexpr std::uintptr_t SOUND1CNT_X = 0x04000064;    // 4000064h  2  R/W  SOUND1CNT_X Channel 1 Frequency/Control    (NR13, NR14)
  //                                                    // 4000066h     -    -           Not used
  constexpr std::uintptr_t SOUND2CNT_L = 0x04000068;    // 4000068h  2  R/W  SOUND2CNT_L Channel 2 Duty/Length/Envelope (NR21, NR22)
  //                                                    // 400006Ah     -    -           Not used
  constexpr std::uintptr_t SOUND2CNT_H = 0x0400006C;    // 400006Ch  2  R/W  SOUND2CNT_H Channel 2 Frequency/Control    (NR23, NR24)
  //                                                    // 400006Eh     -    -           Not used
  constexpr std::uintptr_t SOUND3CNT_L = 0x04000070;    // 4000070h  2  R/W  SOUND3CNT_L Channel 3 Stop/Wave RAM select (NR30)
  constexpr std::uintptr_t SOUND3CNT_H = 0x04000072;    // 4000072h  2  R/W  SOUND3CNT_H Channel 3 Length/Volume        (NR31, NR32)
  constexpr std::uintptr_t SOUND3CNT_X = 0x04000074;    // 4000074h  2  R/W  SOUND3CNT_X Channel 3 Frequency/Control    (NR33, NR34)
  //                                                    // 4000076h     -    -           Not used
  constexpr std::uintptr_t SOUND4CNT_L = 0x04000078;    // 4000078h  2  R/W  SOUND4CNT_L Channel 4 Length/Envelope      (NR41, NR42)
  //                                                    // 400007Ah     -    -           Not used
  constexpr std::uintptr_t SOUND4CNT_H = 0x0400007C;    // 400007Ch  2  R/W  SOUND4CNT_H Channel 4 Frequency/Control    (NR43, NR44)
  //                                                    // 400007Eh     -    -           Not used
  constexpr std::uintptr_t SOUNDCNT_L  = 0x04000080;    // 4000080h  2  R/W  SOUNDCNT_L  Control Stereo/Volume/Enable   (NR50, NR51)
  constexpr std::uintptr_t SOUNDCNT_H  = 0x04000082;    // 4000082h  2  R/W  SOUNDCNT_H  Control Mixing/DMA Control
  constexpr std::uintptr_t SOUNDCNT_X  = 0x04000084;    // 4000084h  2  R/W  SOUNDCNT_X  Control Sound on/off           (NR52)
  //                                                    // 4000086h     -    -           Not used
  constexpr std::uintptr_t SOUNDBIAS   = 0x04000088;    // 4000088h  2  BIOS SOUNDBIAS   Sound PWM Control
  //                                                    // 400008Ah  ..   -    -         Not used
  //                                                    // 4000090h 2x10h R/W  WAVE_RAM  Channel 3 Wave Pattern RAM (2 banks!!)
  constexpr std::uintptr_t WAVE_RAM0_L = 0x04000090;    // 4000090h  2  R/W  WAVE_RAM0_L Channel 3 Wave Pattern RAM
  constexpr std::uintptr_t WAVE_RAM0_H = 0x04000092;    // 4000092h  2  R/W  WAVE_RAM0_H Channel 3 Wave Pattern RAM
  constexpr std::uintptr_t WAVE_RAM1_L = 0x04000094;    // 4000094h  2  R/W  WAVE_RAM1_L Channel 3 Wave Pattern RAM
  constexpr std::uintptr_t WAVE_RAM1_H = 0x04000096;    // 4000096h  2  R/W  WAVE_RAM1_H Channel 3 Wave Pattern RAM
  constexpr std::uintptr_t WAVE_RAM2_L = 0x04000098;    // 4000098h  2  R/W  WAVE_RAM2_L Channel 3 Wave Pattern RAM
  constexpr std::uintptr_t WAVE_RAM2_H = 0x0400009A;    // 400009Ah  2  R/W  WAVE_RAM2_H Channel 3 Wave Pattern RAM
  constexpr std::uintptr_t WAVE_RAM3_L = 0x0400009C;    // 400009Ch  2  R/W  WAVE_RAM3_L Channel 3 Wave Pattern RAM
  constexpr std::uintptr_t WAVE_RAM3_H = 0x0400009E;    // 400009Eh  2  R/W  WAVE_RAM3_H Channel 3 Wave Pattern RAM
  constexpr std::uintptr_t FIFO_A      = 0x040000A0;    // 40000A0h  4    W    FIFO_A    Channel A FIFO, Data 0-3
  constexpr std::uintptr_t FIFO_B      = 0x040000A4;    // 40000A4h  4    W    FIFO_B    Channel B FIFO, Data 0-3
  //                                                    // 40000A8h       -    -         Not used

  // DMA Transfer Channels
  constexpr std::uintptr_t DMA0SAD     = 0x040000B0;    // 40000B0h  4    W    DMA0SAD   DMA 0 Source Address
  constexpr std::uintptr_t DMA0DAD     = 0x040000B4;    // 40000B4h  4    W    DMA0DAD   DMA 0 Destination Address
  constexpr std::uintptr_t DMA0CNT_L   = 0x040000B8;    // 40000B8h  2    W    DMA0CNT_L DMA 0 Word Count
  constexpr std::uintptr_t DMA0CNT_H   = 0x040000BA;    // 40000BAh  2    R/W  DMA0CNT_H DMA 0 Control
  constexpr std::uintptr_t DMA1SAD     = 0x040000BC;    // 40000BCh  4    W    DMA1SAD   DMA 1 Source Address
  constexpr std::uintptr_t DMA1DAD     = 0x040000C0;    // 40000C0h  4    W    DMA1DAD   DMA 1 Destination Address
  constexpr std::uintptr_t DMA1CNT_L   = 0x040000C4;    // 40000C4h  2    W    DMA1CNT_L DMA 1 Word Count
  constexpr std::uintptr_t DMA1CNT_H   = 0x040000C6;    // 40000C6h  2    R/W  DMA1CNT_H DMA 1 Control
  constexpr std::uintptr_t DMA2SAD     = 0x040000C8;    // 40000C8h  4    W    DMA2SAD   DMA 2 Source Address
  constexpr std::uintptr_t DMA2DAD     = 0x040000CC;    // 40000CCh  4    W    DMA2DAD   DMA 2 Destination Address
  constexpr std::uintptr_t DMA2CNT_L   = 0x040000D0;    // 40000D0h  2    W    DMA2CNT_L DMA 2 Word Count
  constexpr std::uintptr_t DMA2CNT_H   = 0x040000D2;    // 40000D2h  2    R/W  DMA2CNT_H DMA 2 Control
  constexpr std::uintptr_t DMA3SAD     = 0x040000D4;    // 40000D4h  4    W    DMA3SAD   DMA 3 Source Address
  constexpr std::uintptr_t DMA3DAD     = 0x040000D8;    // 40000D8h  4    W    DMA3DAD   DMA 3 Destination Address
  constexpr std::uintptr_t DMA3CNT_L   = 0x040000DC;    // 40000DCh  2    W    DMA3CNT_L DMA 3 Word Count
  constexpr std::uintptr_t DMA3CNT_H   = 0x040000DE;    // 40000DEh  2    R/W  DMA3CNT_H DMA 3 Control
  //                                                    // 40000E0h       -    -         Not used

  // Timer Registers
  constexpr std::uintptr_t TM0CNT_L    = 0x04000100;    // 4000100h  2    R/W  TM0CNT_L  Timer 0 Counter/Reload
  constexpr std::uintptr_t TM0CNT_H    = 0x04000102;    // 4000102h  2    R/W  TM0CNT_H  Timer 0 Control
  constexpr std::uintptr_t TM1CNT_L    = 0x04000104;    // 4000104h  2    R/W  TM1CNT_L  Timer 1 Counter/Reload
  constexpr std::uintptr_t TM1CNT_H    = 0x04000106;    // 4000106h  2    R/W  TM1CNT_H  Timer 1 Control
  constexpr std::uintptr_t TM2CNT_L    = 0x04000108;    // 4000108h  2    R/W  TM2CNT_L  Timer 2 Counter/Reload
  constexpr std::uintptr_t TM2CNT_H    = 0x0400010A;    // 400010Ah  2    R/W  TM2CNT_H  Timer 2 Control
  constexpr std::uintptr_t TM3CNT_L    = 0x0400010C;    // 400010Ch  2    R/W  TM3CNT_L  Timer 3 Counter/Reload
  constexpr std::uintptr_t TM3CNT_H    = 0x0400010E;    // 400010Eh  2    R/W  TM3CNT_H  Timer 3 Control
  //                                                    // 4000110h       -    -         Not used

  // Serial Communication (1)
  constexpr std::uintptr_t SIODATA32   = 0x04000120;    // 4000120h  4    R/W  SIODATA32 SIO Data (Normal-32bit Mode; shared with below)
  constexpr std::uintptr_t SIOMULTI0   = 0x04000120;    // 4000120h  2    R/W  SIOMULTI0 SIO Data 0 (Parent)    (Multi-Player Mode)
  constexpr std::uintptr_t SIOMULTI1   = 0x04000122;    // 4000122h  2    R/W  SIOMULTI1 SIO Data 1 (1st Child) (Multi-Player Mode)
  constexpr std::uintptr_t SIOMULTI2   = 0x04000124;    // 4000124h  2    R/W  SIOMULTI2 SIO Data 2 (2nd Child) (Multi-Player Mode)
  constexpr std::uintptr_t SIOMULTI3   = 0x04000126;    // 4000126h  2    R/W  SIOMULTI3 SIO Data 3 (3rd Child) (Multi-Player Mode)
  constexpr std::uintptr_t SIOCNT      = 0x04000128;    // 4000128h  2    R/W  SIOCNT    SIO Control Register
  constexpr std::uintptr_t SIOMLT_SEND = 0x0400012A;    // 400012Ah  2    R/W  SIOMLT_SEND SIO Data (Local of MultiPlayer; shared below)
  constexpr std::uintptr_t SIODATA8    = 0x0400012A;    // 400012Ah  2    R/W  SIODATA8  SIO Data (Normal-8bit and UART Mode)
  //                                                    // 400012Ch       -    -         Not used

  // Keypad Input
  constexpr std::uintptr_t KEYINPUT    = 0x04000130;    // 4000130h  2    R    KEYINPUT  Key Status
  constexpr std::uintptr_t KEYCNT      = 0x04000132;    // 4000132h  2    R/W  KEYCNT    Key Interrupt Control

  // Serial Communication (2)
  constexpr std::uintptr_t RCNT        = 0x04000134;    // 4000134h  2    R/W  RCNT      SIO Mode Select/General Purpose Data
  //                                                    // 4000136h  -    -    IR        Ancient - Infrared Register (Prototypes only)
  //                                                    // 4000138h       -    -         Not used
  constexpr std::uintptr_t JOYCNT      = 0x04000140;    // 4000140h  2    R/W  JOYCNT    SIO JOY Bus Control
  //                                                    // 4000142h       -    -         Not used
  constexpr std::uintptr_t JOY_RECV    = 0x04000150;    // 4000150h  4    R/W  JOY_RECV  SIO JOY Bus Receive Data
  constexpr std::uintptr_t JOY_TRANS   = 0x04000154;    // 4000154h  4    R/W  JOY_TRANS SIO JOY Bus Transmit Data
  constexpr std::uintptr_t JOYSTAT     = 0x04000158;    // 4000158h  2    R/?  JOYSTAT   SIO JOY Bus Receive Status
  //                                                    // 400015Ah       -    -         Not used

  // Interrupt, Waitstate, and Power-Down Control
  constexpr std::uintptr_t IE          = 0x04000200;    // 4000200h  2    R/W  IE        Interrupt Enable Register
  constexpr std::uintptr_t IF          = 0x04000202;    // 4000202h  2    R/W  IF        Interrupt Request Flags / IRQ Acknowledge
  constexpr std::uintptr_t WAITCNT     = 0x04000204;    // 4000204h  2    R/W  WAITCNT   Game Pak Waitstate Control
  //                                                    // 4000206h       -    -         Not used
  constexpr std::uintptr_t IME         = 0x04000208;    // 4000208h  2    R/W  IME       Interrupt Master Enable Register
  //                                                    // 400020Ah       -    -         Not used
  //                                                    // 4000300h  1    R/W  POSTFLG   Undocumented - Post Boot Flag
  //                                                    // 4000301h  1    W    HALTCNT   Undocumented - Power Down Control
  //                                                    // 4000302h       -    -         Not used
  //                                                    // 4000410h  ?    ?    ?         Undocumented - Purpose Unknown / Bug ??? 0FFh
  //                                                    // 4000411h       -    -         Not used
  //                                                    // 4000800h  4    R/W  ?         Undocumented - Internal Memory Control (R/W)
  //                                                    // 4000804h       -    -         Not used
  //                                                    // 4xx0800h  4    R/W  ?         Mirrors of 4000800h (repeated each 64K)
}   // namespace gba::address

#endif
