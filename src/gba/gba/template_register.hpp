#ifndef _gba_template_register_hpp_
#define _gba_template_register_hpp_

#include <cstdint>

#include "type.hpp"


namespace gba::template_reg {
  // BIOS RAM
  template<typename T=isr>                                              // 3007FFCh  4    R/W  ISRAD     Pointer to user IRQ handler (32bit ARM code)
  inline volatile T& ISRAD       = *template_pointer::ISRAD<T>;
  template<typename T=std::uint16_t>                                    // 3007FF8h  2    R/W  INTCHK    Interrupt Check Flag (for IntrWait/VBlankIntrWait functions)
  inline volatile T& INTCHK      = *template_pointer::INTCHK<T>;
  template<typename T=std::uint32_t>                                    // 3007FF4h  4    R/W  ALLOCA    Allocated Area
  inline volatile T& ALLOCA      = *template_pointer::ALLOCA<T>;
  template<typename T=void*>                                            // 3007FF0h  4    R/W  SNDBUFAD  Pointer to Sound Buffer
  inline volatile T& SNDBUFAD    = *template_pointer::SNDBUFAD<T>;

  // LCD I/O Registers
  template<typename T=std::uint16_t>                                    // 4000000h  2    R/W  DISPCNT   LCD Control
  inline volatile T& DISPCNT     = *template_pointer::DISPCNT<T>;
  //                                                                    // 4000002h  2    R/W  -         Undocumented - Green Swap
  template<typename T=std::uint16_t>                                    // 4000004h  2    R/W  DISPSTAT  General LCD Status (STAT,LYC)
  inline volatile T& DISPSTAT    = *template_pointer::DISPSTAT<T>;
  template<typename T=std::uint16_t>                                    // 4000006h  2    R    VCOUNT    Vertical Counter (LY)
  inline volatile T& VCOUNT      = *template_pointer::VCOUNT<T>;
  template<typename T=std::uint16_t>                                    // 4000008h  2    R/W  BG0CNT    BG0 Control
  inline volatile T& BG0CNT      = *template_pointer::BG0CNT<T>;
  template<typename T=std::uint16_t>                                    // 400000Ah  2    R/W  BG1CNT    BG1 Control
  inline volatile T& BG1CNT      = *template_pointer::BG1CNT<T>;
  template<typename T=std::uint16_t>                                    // 400000Ch  2    R/W  BG2CNT    BG2 Control
  inline volatile T& BG2CNT      = *template_pointer::BG2CNT<T>;
  template<typename T=std::uint16_t>                                    // 400000Eh  2    R/W  BG3CNT    BG3 Control
  inline volatile T& BG3CNT      = *template_pointer::BG3CNT<T>;
  template<typename T=std::uint16_t>                                    // 4000010h  2    W    BG0HOFS   BG0 X-Offset
  inline volatile T& BG0HOFS     = *template_pointer::BG0HOFS<T>;
  template<typename T=std::uint16_t>                                    // 4000012h  2    W    BG0VOFS   BG0 Y-Offset
  inline volatile T& BG0VOFS     = *template_pointer::BG0VOFS<T>;
  template<typename T=std::uint16_t>                                    // 4000014h  2    W    BG1HOFS   BG1 X-Offset
  inline volatile T& BG1HOFS     = *template_pointer::BG1HOFS<T>;
  template<typename T=std::uint16_t>                                    // 4000016h  2    W    BG1VOFS   BG1 Y-Offset
  inline volatile T& BG1VOFS     = *template_pointer::BG1VOFS<T>;
  template<typename T=std::uint16_t>                                    // 4000018h  2    W    BG2HOFS   BG2 X-Offset
  inline volatile T& BG2HOFS     = *template_pointer::BG2HOFS<T>;
  template<typename T=std::uint16_t>                                    // 400001Ah  2    W    BG2VOFS   BG2 Y-Offset
  inline volatile T& BG2VOFS     = *template_pointer::BG2VOFS<T>;
  template<typename T=std::uint16_t>                                    // 400001Ch  2    W    BG3HOFS   BG3 X-Offset
  inline volatile T& BG3HOFS     = *template_pointer::BG3HOFS<T>;
  template<typename T=std::uint16_t>                                    // 400001Eh  2    W    BG3VOFS   BG3 Y-Offset
  inline volatile T& BG3VOFS     = *template_pointer::BG3VOFS<T>;
  template<typename T=std::uint16_t>                                    // 4000020h  2    W    BG2PA     BG2 Rotation/Scaling Parameter A (dx)
  inline volatile T& BG2PA       = *template_pointer::BG2PA<T>;
  template<typename T=std::uint16_t>                                    // 4000022h  2    W    BG2PB     BG2 Rotation/Scaling Parameter B (dmx)
  inline volatile T& BG2PB       = *template_pointer::BG2PB<T>;
  template<typename T=std::uint16_t>                                    // 4000024h  2    W    BG2PC     BG2 Rotation/Scaling Parameter C (dy)
  inline volatile T& BG2PC       = *template_pointer::BG2PC<T>;
  template<typename T=std::uint16_t>                                    // 4000026h  2    W    BG2PD     BG2 Rotation/Scaling Parameter D (dmy)
  inline volatile T& BG2PD       = *template_pointer::BG2PD<T>;
  template<typename T=std::uint32_t>                                    // 4000028h  4    W    BG2X      BG2 Reference Point X-Coordinate
  inline volatile T& BG2X        = *template_pointer::BG2X<T>;
  template<typename T=std::uint32_t>                                    // 400002Ch  4    W    BG2Y      BG2 Reference Point Y-Coordinate
  inline volatile T& BG2Y        = *template_pointer::BG2Y<T>;
  template<typename T=std::uint16_t>                                    // 4000030h  2    W    BG3PA     BG3 Rotation/Scaling Parameter A (dx)
  inline volatile T& BG3PA       = *template_pointer::BG3PA<T>;
  template<typename T=std::uint16_t>                                    // 4000032h  2    W    BG3PB     BG3 Rotation/Scaling Parameter B (dmx)
  inline volatile T& BG3PB       = *template_pointer::BG3PB<T>;
  template<typename T=std::uint16_t>                                    // 4000034h  2    W    BG3PC     BG3 Rotation/Scaling Parameter C (dy)
  inline volatile T& BG3PC       = *template_pointer::BG3PC<T>;
  template<typename T=std::uint16_t>                                    // 4000036h  2    W    BG3PD     BG3 Rotation/Scaling Parameter D (dmy)
  inline volatile T& BG3PD       = *template_pointer::BG3PD<T>;
  template<typename T=std::uint32_t>                                    // 4000038h  4    W    BG3X      BG3 Reference Point X-Coordinate
  inline volatile T& BG3X        = *template_pointer::BG3X<T>;
  template<typename T=std::uint32_t>                                    // 400003Ch  4    W    BG3Y      BG3 Reference Point Y-Coordinate
  inline volatile T& BG3Y        = *template_pointer::BG3Y<T>;
  template<typename T=std::uint16_t>                                    // 4000040h  2    W    WIN0H     Window 0 Horizontal Dimensions
  inline volatile T& WIN0H       = *template_pointer::WIN0H<T>;
  template<typename T=std::uint16_t>                                    // 4000042h  2    W    WIN1H     Window 1 Horizontal Dimensions
  inline volatile T& WIN1H       = *template_pointer::WIN1H<T>;
  template<typename T=std::uint16_t>                                    // 4000044h  2    W    WIN0V     Window 0 Vertical Dimensions
  inline volatile T& WIN0V       = *template_pointer::WIN0V<T>;
  template<typename T=std::uint16_t>                                    // 4000046h  2    W    WIN1V     Window 1 Vertical Dimensions
  inline volatile T& WIN1V       = *template_pointer::WIN1V<T>;
  template<typename T=std::uint16_t>                                    // 4000048h  2    R/W  WININ     Inside of Window 0 and 1
  inline volatile T& WININ       = *template_pointer::WININ<T>;
  template<typename T=std::uint16_t>                                    // 400004Ah  2    R/W  WINOUT    Inside of OBJ Window & Outside of Windows
  inline volatile T& WINOUT      = *template_pointer::WINOUT<T>;
  template<typename T=std::uint16_t>                                    // 400004Ch  2    W    MOSAIC    Mosaic Size
  inline volatile T& MOSAIC      = *template_pointer::MOSAIC<T>;
  //                                                                    // 400004Eh       -    -         Not used
  template<typename T=std::uint16_t>                                    // 4000050h  2    R/W  BLDCNT    Color Special Effects Selection
  inline volatile T& BLDCNT      = *template_pointer::BLDCNT<T>;
  template<typename T=std::uint16_t>                                    // 4000052h  2    R/W  BLDALPHA  Alpha Blending Coefficients
  inline volatile T& BLDALPHA    = *template_pointer::BLDALPHA<T>;
  template<typename T=std::uint16_t>                                    // 4000054h  2    W    BLDY      Brightness (Fade-In/Out) Coefficient
  inline volatile T& BLDY        = *template_pointer::BLDY<T>;
  //                                                                    // 4000056h       -    -         Not used

  // Sound Registers
  template<typename T=std::uint16_t>                                    // 4000060h  2  R/W  SOUND1CNT_L Channel 1 Sweep register       (NR10)
  inline volatile T& SOUND1CNT_L = *template_pointer::SOUND1CNT_L<T>;
  template<typename T=std::uint16_t>                                    // 4000062h  2  R/W  SOUND1CNT_H Channel 1 Duty/Length/Envelope (NR11, NR12)
  inline volatile T& SOUND1CNT_H = *template_pointer::SOUND1CNT_H<T>;
  template<typename T=std::uint16_t>                                    // 4000064h  2  R/W  SOUND1CNT_X Channel 1 Frequency/Control    (NR13, NR14)
  inline volatile T& SOUND1CNT_X = *template_pointer::SOUND1CNT_X<T>;
  //                                                                    // 4000066h     -    -           Not used
  template<typename T=std::uint16_t>                                    // 4000068h  2  R/W  SOUND2CNT_L Channel 2 Duty/Length/Envelope (NR21, NR22)
  inline volatile T& SOUND2CNT_L = *template_pointer::SOUND2CNT_L<T>;
  //                                                                    // 400006Ah     -    -           Not used
  template<typename T=std::uint16_t>                                    // 400006Ch  2  R/W  SOUND2CNT_H Channel 2 Frequency/Control    (NR23, NR24)
  inline volatile T& SOUND2CNT_H = *template_pointer::SOUND2CNT_H<T>;
  //                                                                    // 400006Eh     -    -           Not used
  template<typename T=std::uint16_t>                                    // 4000070h  2  R/W  SOUND3CNT_L Channel 3 Stop/Wave RAM select (NR30)
  inline volatile T& SOUND3CNT_L = *template_pointer::SOUND3CNT_L<T>;
  template<typename T=std::uint16_t>                                    // 4000072h  2  R/W  SOUND3CNT_H Channel 3 Length/Volume        (NR31, NR32)
  inline volatile T& SOUND3CNT_H = *template_pointer::SOUND3CNT_H<T>;
  template<typename T=std::uint16_t>                                    // 4000074h  2  R/W  SOUND3CNT_X Channel 3 Frequency/Control    (NR33, NR34)
  inline volatile T& SOUND3CNT_X = *template_pointer::SOUND3CNT_X<T>;
  //                                                                    // 4000076h     -    -           Not used
  template<typename T=std::uint16_t>                                    // 4000078h  2  R/W  SOUND4CNT_L Channel 4 Length/Envelope      (NR41, NR42)
  inline volatile T& SOUND4CNT_L = *template_pointer::SOUND4CNT_L<T>;
  //                                                                    // 400007Ah     -    -           Not used
  template<typename T=std::uint16_t>                                    // 400007Ch  2  R/W  SOUND4CNT_H Channel 4 Frequency/Control    (NR43, NR44)
  inline volatile T& SOUND4CNT_H = *template_pointer::SOUND4CNT_H<T>;
  //                                                                    // 400007Eh     -    -           Not used
  template<typename T=std::uint16_t>                                    // 4000080h  2  R/W  SOUNDCNT_L  Control Stereo/Volume/Enable   (NR50, NR51)
  inline volatile T& SOUNDCNT_L  = *template_pointer::SOUNDCNT_L<T>;
  template<typename T=std::uint16_t>                                    // 4000082h  2  R/W  SOUNDCNT_H  Control Mixing/DMA Control
  inline volatile T& SOUNDCNT_H  = *template_pointer::SOUNDCNT_H<T>;
  template<typename T=std::uint16_t>                                    // 4000084h  2  R/W  SOUNDCNT_X  Control Sound on/off           (NR52)
  inline volatile T& SOUNDCNT_X  = *template_pointer::SOUNDCNT_X<T>;
  //                                                                    // 4000086h     -    -           Not used
  template<typename T=std::uint16_t>                                    // 4000088h  2  BIOS SOUNDBIAS   Sound PWM Control
  inline volatile T& SOUNDBIAS   = *template_pointer::SOUNDBIAS<T>;
  //                                                                    // 400008Ah  ..   -    -         Not used
  //                                                                    // 4000090h 2x10h R/W  WAVE_RAM  Channel 3 Wave Pattern RAM (2 banks!!)
  template<typename T=std::uint16_t>                                    // 4000090h  2  R/W  WAVE_RAM0_L Channel 3 Wave Pattern RAM
  inline volatile T& WAVE_RAM0_L = *template_pointer::WAVE_RAM0_L<T>;
  template<typename T=std::uint16_t>                                    // 4000092h  2  R/W  WAVE_RAM0_H Channel 3 Wave Pattern RAM
  inline volatile T& WAVE_RAM0_H = *template_pointer::WAVE_RAM0_H<T>;
  template<typename T=std::uint16_t>                                    // 4000094h  2  R/W  WAVE_RAM1_L Channel 3 Wave Pattern RAM
  inline volatile T& WAVE_RAM1_L = *template_pointer::WAVE_RAM1_L<T>;
  template<typename T=std::uint16_t>                                    // 4000096h  2  R/W  WAVE_RAM1_H Channel 3 Wave Pattern RAM
  inline volatile T& WAVE_RAM1_H = *template_pointer::WAVE_RAM1_H<T>;
  template<typename T=std::uint16_t>                                    // 4000098h  2  R/W  WAVE_RAM2_L Channel 3 Wave Pattern RAM
  inline volatile T& WAVE_RAM2_L = *template_pointer::WAVE_RAM2_L<T>;
  template<typename T=std::uint16_t>                                    // 400009Ah  2  R/W  WAVE_RAM2_H Channel 3 Wave Pattern RAM
  inline volatile T& WAVE_RAM2_H = *template_pointer::WAVE_RAM2_H<T>;
  template<typename T=std::uint16_t>                                    // 400009Ch  2  R/W  WAVE_RAM3_L Channel 3 Wave Pattern RAM
  inline volatile T& WAVE_RAM3_L = *template_pointer::WAVE_RAM3_L<T>;
  template<typename T=std::uint16_t>                                    // 400009Eh  2  R/W  WAVE_RAM3_H Channel 3 Wave Pattern RAM
  inline volatile T& WAVE_RAM3_H = *template_pointer::WAVE_RAM3_H<T>;
  template<typename T=void*>                                            // 40000A0h  4    W    FIFO_A    Channel A FIFO, Data 0-3
  inline volatile T& FIFO_A      = *template_pointer::FIFO_A<T>;
  template<typename T=void*>                                            // 40000A4h  4    W    FIFO_B    Channel B FIFO, Data 0-3
  inline volatile T& FIFO_B      = *template_pointer::FIFO_B<T>;
  //                                                                    // 40000A8h       -    -         Not used

  // DMA Transfer Channels
  template<typename T=const void*>                                      // 40000B0h  4    W    DMA0SAD   DMA 0 Source Address
  inline volatile T& DMA0SAD     = *template_pointer::DMA0SAD<T>;
  template<typename T=void*>                                            // 40000B4h  4    W    DMA0DAD   DMA 0 Destination Address
  inline volatile T& DMA0DAD     = *template_pointer::DMA0DAD<T>;
  template<typename T=std::uint16_t>                                    // 40000B8h  2    W    DMA0CNT_L DMA 0 Word Count
  inline volatile T& DMA0CNT_L   = *template_pointer::DMA0CNT_L<T>;
  template<typename T=std::uint16_t>                                    // 40000BAh  2    R/W  DMA0CNT_H DMA 0 Control
  inline volatile T& DMA0CNT_H   = *template_pointer::DMA0CNT_H<T>;
  template<typename T=const void*>                                      // 40000BCh  4    W    DMA1SAD   DMA 1 Source Address
  inline volatile T& DMA1SAD     = *template_pointer::DMA1SAD<T>;
  template<typename T=void*>                                            // 40000C0h  4    W    DMA1DAD   DMA 1 Destination Address
  inline volatile T& DMA1DAD     = *template_pointer::DMA1DAD<T>;
  template<typename T=std::uint16_t>                                    // 40000C4h  2    W    DMA1CNT_L DMA 1 Word Count
  inline volatile T& DMA1CNT_L   = *template_pointer::DMA1CNT_L<T>;
  template<typename T=std::uint16_t>                                    // 40000C6h  2    R/W  DMA1CNT_H DMA 1 Control
  inline volatile T& DMA1CNT_H   = *template_pointer::DMA1CNT_H<T>;
  template<typename T=const void*>                                      // 40000C8h  4    W    DMA2SAD   DMA 2 Source Address
  inline volatile T& DMA2SAD     = *template_pointer::DMA2SAD<T>;
  template<typename T=void*>                                            // 40000CCh  4    W    DMA2DAD   DMA 2 Destination Address
  inline volatile T& DMA2DAD     = *template_pointer::DMA2DAD<T>;
  template<typename T=std::uint16_t>                                    // 40000D0h  2    W    DMA2CNT_L DMA 2 Word Count
  inline volatile T& DMA2CNT_L   = *template_pointer::DMA2CNT_L<T>;
  template<typename T=std::uint16_t>                                    // 40000D2h  2    R/W  DMA2CNT_H DMA 2 Control
  inline volatile T& DMA2CNT_H   = *template_pointer::DMA2CNT_H<T>;
  template<typename T=const void*>                                      // 40000D4h  4    W    DMA3SAD   DMA 3 Source Address
  inline volatile T& DMA3SAD     = *template_pointer::DMA3SAD<T>;
  template<typename T=void*>                                            // 40000D8h  4    W    DMA3DAD   DMA 3 Destination Address
  inline volatile T& DMA3DAD     = *template_pointer::DMA3DAD<T>;
  template<typename T=std::uint16_t>                                    // 40000DCh  2    W    DMA3CNT_L DMA 3 Word Count
  inline volatile T& DMA3CNT_L   = *template_pointer::DMA3CNT_L<T>;
  template<typename T=std::uint16_t>                                    // 40000DEh  2    R/W  DMA3CNT_H DMA 3 Control
  inline volatile T& DMA3CNT_H   = *template_pointer::DMA3CNT_H<T>;
  //                                                                    // 40000E0h       -    -         Not used

  // Timer Registers
  template<typename T=std::uint16_t>                                    // 4000100h  2    R/W  TM0CNT_L  Timer 0 Counter/Reload
  inline volatile T& TM0CNT_L    = *template_pointer::TM0CNT_L<T>;
  template<typename T=std::uint16_t>                                    // 4000102h  2    R/W  TM0CNT_H  Timer 0 Control
  inline volatile T& TM0CNT_H    = *template_pointer::TM0CNT_H<T>;
  template<typename T=std::uint16_t>                                    // 4000104h  2    R/W  TM1CNT_L  Timer 1 Counter/Reload
  inline volatile T& TM1CNT_L    = *template_pointer::TM1CNT_L<T>;
  template<typename T=std::uint16_t>                                    // 4000106h  2    R/W  TM1CNT_H  Timer 1 Control
  inline volatile T& TM1CNT_H    = *template_pointer::TM1CNT_H<T>;
  template<typename T=std::uint16_t>                                    // 4000108h  2    R/W  TM2CNT_L  Timer 2 Counter/Reload
  inline volatile T& TM2CNT_L    = *template_pointer::TM2CNT_L<T>;
  template<typename T=std::uint16_t>                                    // 400010Ah  2    R/W  TM2CNT_H  Timer 2 Control
  inline volatile T& TM2CNT_H    = *template_pointer::TM2CNT_H<T>;
  template<typename T=std::uint16_t>                                    // 400010Ch  2    R/W  TM3CNT_L  Timer 3 Counter/Reload
  inline volatile T& TM3CNT_L    = *template_pointer::TM3CNT_L<T>;
  template<typename T=std::uint16_t>                                    // 400010Eh  2    R/W  TM3CNT_H  Timer 3 Control
  inline volatile T& TM3CNT_H    = *template_pointer::TM3CNT_H<T>;
  //                                                                    // 4000110h       -    -         Not used

  // Serial Communication (1)
  template<typename T=std::uint32_t>                                    // 4000120h  4    R/W  SIODATA32 SIO Data (Normal-32bit Mode; shared with below)
  inline volatile T& SIODATA32   = *template_pointer::SIODATA32<T>;
  template<typename T=std::uint16_t>                                    // 4000120h  2    R/W  SIOMULTI0 SIO Data 0 (Parent)    (Multi-Player Mode)
  inline volatile T& SIOMULTI0   = *template_pointer::SIOMULTI0<T>;
  template<typename T=std::uint16_t>                                    // 4000122h  2    R/W  SIOMULTI1 SIO Data 1 (1st Child) (Multi-Player Mode)
  inline volatile T& SIOMULTI1   = *template_pointer::SIOMULTI1<T>;
  template<typename T=std::uint16_t>                                    // 4000124h  2    R/W  SIOMULTI2 SIO Data 2 (2nd Child) (Multi-Player Mode)
  inline volatile T& SIOMULTI2   = *template_pointer::SIOMULTI2<T>;
  template<typename T=std::uint16_t>                                    // 4000126h  2    R/W  SIOMULTI3 SIO Data 3 (3rd Child) (Multi-Player Mode)
  inline volatile T& SIOMULTI3   = *template_pointer::SIOMULTI3<T>;
  template<typename T=std::uint16_t>                                    // 4000128h  2    R/W  SIOCNT    SIO Control Register
  inline volatile T& SIOCNT      = *template_pointer::SIOCNT<T>;
  template<typename T=std::uint16_t>                                    // 400012Ah  2    R/W  SIOMLT_SEND SIO Data (Local of MultiPlayer; shared below)
  inline volatile T& SIOMLT_SEND = *template_pointer::SIOMLT_SEND<T>;
  template<typename T=std::uint16_t>                                    // 400012Ah  2    R/W  SIODATA8  SIO Data (Normal-8bit and UART Mode)
  inline volatile T& SIODATA8    = *template_pointer::SIODATA8<T>;
  //                                                                    // 400012Ch       -    -         Not used

  // Keypad Input
  template<typename T=std::uint16_t>                                    // 4000130h  2    R    KEYINPUT  Key Status
  inline volatile T& KEYINPUT    = *template_pointer::KEYINPUT<T>;
  template<typename T=std::uint16_t>                                    // 4000132h  2    R/W  KEYCNT    Key Interrupt Control
  inline volatile T& KEYCNT      = *template_pointer::KEYCNT<T>;

  // Serial Communication (2)
  template<typename T=std::uint16_t>                                    // 4000134h  2    R/W  RCNT      SIO Mode Select/General Purpose Data
  inline volatile T& RCNT        = *template_pointer::RCNT<T>;
  //                                                                    // 4000136h  -    -    IR        Ancient - Infrared Register (Prototypes only)
  //                                                                    // 4000138h       -    -         Not used
  template<typename T=std::uint16_t>                                    // 4000140h  2    R/W  JOYCNT    SIO JOY Bus Control
  inline volatile T& JOYCNT      = *template_pointer::JOYCNT<T>;
  //                                                                    // 4000142h       -    -         Not used
  template<typename T=std::uint32_t>                                    // 4000150h  4    R/W  JOY_RECV  SIO JOY Bus Receive Data
  inline volatile T& JOY_RECV    = *template_pointer::JOY_RECV<T>;
  template<typename T=std::uint32_t>                                    // 4000154h  4    R/W  JOY_TRANS SIO JOY Bus Transmit Data
  inline volatile T& JOY_TRANS   = *template_pointer::JOY_TRANS<T>;
  template<typename T=std::uint16_t>                                    // 4000158h  2    R/?  JOYSTAT   SIO JOY Bus Receive Status
  inline volatile T& JOYSTAT     = *template_pointer::JOYSTAT<T>;
  //                                                                    // 400015Ah       -    -         Not used

  // Interrupt, Waitstate, and Power-Down Control
  template<typename T=std::uint16_t>                                    // 4000200h  2    R/W  IE        Interrupt Enable Register
  inline volatile T& IE          = *template_pointer::IE<T>;
  template<typename T=std::uint16_t>                                    // 4000202h  2    R/W  IF        Interrupt Request Flags / IRQ Acknowledge
  inline volatile T& IF          = *template_pointer::IF<T>;
  template<typename T=std::uint16_t>                                    // 4000204h  2    R/W  WAITCNT   Game Pak Waitstate Control
  inline volatile T& WAITCNT     = *template_pointer::WAITCNT<T>;
  //                                                                    // 4000206h       -    -         Not used
  template<typename T=std::uint16_t>                                    // 4000208h  2    R/W  IME       Interrupt Master Enable Register
  inline volatile T& IME         = *template_pointer::IME<T>;
  //                                                                    // 400020Ah       -    -         Not used
  //                                                                    // 4000300h  1    R/W  POSTFLG   Undocumented - Post Boot Flag
  //                                                                    // 4000301h  1    W    HALTCNT   Undocumented - Power Down Control
  //                                                                    // 4000302h       -    -         Not used
  //                                                                    // 4000410h  ?    ?    ?         Undocumented - Purpose Unknown / Bug ??? 0FFh
  //                                                                    // 4000411h       -    -         Not used
  //                                                                    // 4000800h  4    R/W  ?         Undocumented - Internal Memory Control (R/W)
  //                                                                    // 4000804h       -    -         Not used
  //                                                                    // 4xx0800h  4    R/W  ?         Mirrors of 4000800h (repeated each 64K)
}   // namespace gba::template_reg

#endif
