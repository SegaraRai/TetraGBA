#ifndef _gba_const_sound_hpp_
#define _gba_const_sound_hpp_

#include <cassert>
#include <cstdint>


namespace gba {
  inline namespace constant {
    // ## GBA Sound Controller
    // https://problemkaputt.de/gbatek.htm#gbasoundcontroller


    // ### GBA Sound Channel 1 - Tone & Sweep
    // https://problemkaputt.de/gbatek.htm#gbasoundchannel1tonesweep

    // SOUND1CNT_L (NR10) - Channel 1 Sweep register (R/W)
    namespace SOUND1CNT_L {
      // Number of sweep shift (n=0-7)
      constexpr std::uint16_t SHIFT(std::uint_fast16_t sweepShift) {
        assert(sweepShift < 8);
        return static_cast<std::uint16_t>(sweepShift & 7);
      }

      // Number of sweep shift (n=0-7)
      constexpr std::uint16_t GET_SHIFT(std::uint16_t value) {
        return value & 7;
      }

      constexpr std::uint16_t FREQ_INC = 0 << 3;    // Sweep Frequency Direction (Increase)
      constexpr std::uint16_t FREQ_DEC = 1 << 3;    // Sweep Frequency Direction (Decrease)

      // Sweep Time; units of 7.8ms (0-7, min=7.8ms, max=54.7ms)
      constexpr std::uint16_t TIME(std::uint_fast16_t sweepTime) {
        assert(sweepTime < 8);
        return static_cast<std::uint16_t>(sweepTime & 7) << 4;
      }

      // Sweep Time; units of 7.8ms (0-7, min=7.8ms, max=54.7ms)
      constexpr std::uint16_t GET_TIME(std::uint16_t value) {
        return (value >> 4) & 7;
      }
    }

    // SOUND1CNT_H (NR11, NR12) - Channel 1 Duty/Len/Envelope (R/W)
    namespace SOUND1CNT_H {
      // Sound length; units of (64 - n) / 256 sec. (0-63)
      // used only if Bit 6 in NR14 (SOUND1CNT_X::USE_LENGTH or SOUND2CNT_H::USE_LENGTH) is set.
      constexpr std::uint16_t LENGTH(std::uint_fast16_t length) {
        assert(length < 64);
        return static_cast<std::uint16_t>(length & 63);
      }

      // Sound length; units of (64 - n) / 256 sec. (0-63)
      // used only if Bit 6 in NR14 (SOUND1CNT_X::USE_LENGTH or SOUND2CNT_H::USE_LENGTH) is set.
      constexpr std::uint16_t GET_LENGTH(std::uint16_t value) {
        return value & 7;
      }

      // Wave Pattern Duty (0-3)
      inline namespace DUTY {
        constexpr std::uint16_t DUTY125 = 0 << 6;    // 12.5% ( -_______-_______-_______ )
        constexpr std::uint16_t DUTY250 = 1 << 6;    // 25%   ( --______--______--______ )
        constexpr std::uint16_t DUTY500 = 2 << 6;    // ( ----____----____----____ ) (normal)
        constexpr std::uint16_t DUTY750 = 3 << 6;    // ( ------__------__------__ )
      }

      // Envelope Step-Time; units of n / 64 sec. (1-7, 0=No Envelope)
      constexpr std::uint16_t ENVSTEP(std::uint_fast16_t envelopeStepTime) {
        assert(envelopeStepTime < 8);
        return static_cast<std::uint16_t>(envelopeStepTime & 7) << 8;
      }

      // Envelope Step-Time; units of n / 64 sec. (1-7, 0=No Envelope)
      constexpr std::uint16_t GET_ENVSTEP(std::uint16_t value) {
        return (value >> 8) & 7;
      }

      constexpr std::uint16_t ENV_DEC = 0 << 14;    // Envelope Direction (Decrease)
      constexpr std::uint16_t ENV_INC = 1 << 14;    // Envelope Direction (Increase)

      // Initial Volume of envelope (1-15, 0=No Sound)
      constexpr std::uint16_t VOLUME(std::uint_fast16_t envelopeInitialVolume) {
        assert(envelopeInitialVolume < 16);
        return static_cast<std::uint16_t>(envelopeInitialVolume & 15) << 12;
      }

      // Initial Volume of envelope (1-15, 0=No Sound)
      constexpr std::uint16_t GET_VOLUME(std::uint16_t value) {
        return (value >> 12) & 15;
      }
    }

    // SOUND1CNT_X (NR13, NR14) - Channel 1 Frequency/Control (R/W)
    namespace SOUND1CNT_X {
      // Frequency; 131072 / (2048 - n) Hz (0-2047)
      constexpr std::uint16_t FREQ(std::uint_fast16_t frequency) {
        assert(frequency < 2048);
        return static_cast<std::uint16_t>(frequency & 2047);
      }

      // Frequency; 131072 / (2048 - n) Hz (0-2047)
      constexpr std::uint16_t GET_FREQ(std::uint16_t value) {
        return value & 2047;
      }

      constexpr std::uint16_t USE_LENGTH = 1 << 14;   // Length Flag (1=Stop output when length in NR11 (SOUND1CNT_H) / NR21 (SOUND2CNT_L) expires)
      constexpr std::uint16_t RESTART    = 1 << 15;   // Initial (Restart Sound)
    }


    // ### GBA Sound Channel 2 - Tone
    // https://problemkaputt.de/gbatek.htm#gbasoundchannel2tone

    // SOUND2CNT_L (NR21, NR22) - Channel 2 Duty/Length/Envelope (R/W)
    namespace SOUND2CNT_L {
      using namespace SOUND1CNT_H;
    }

    // SOUND2CNT_H (NR23, NR24) - Channel 2 Frequency/Control (R/W)
    namespace SOUND2CNT_H {
      using namespace SOUND1CNT_X;
    }


    // ### GBA Sound Channel 3 - Wave Output
    // https://problemkaputt.de/gbatek.htm#gbasoundchannel3waveoutput

    // SOUND3CNT_L (NR30) - Channel 3 Stop/Wave RAM select (R/W)
    namespace SOUND3CNT_L {
      constexpr std::uint16_t ONEBANK    = 0 << 5;    // Wave RAM Dimension (One bank/32 digits)
      constexpr std::uint16_t TWOBANK    = 1 << 5;    // Wave RAM Dimension (Two banks/64 digits)

      constexpr std::uint16_t BANK0      = 0 << 6;    // Wave RAM Bank Number (0)
      constexpr std::uint16_t BANK1      = 1 << 6;    // Wave RAM Bank Number (1)

      constexpr std::uint16_t SOUND3_OFF = 0 << 7;    // Sound Channel 3 Off (Stop)
      constexpr std::uint16_t SOUND3_ON  = 1 << 7;    // Sound Channel 3 Off (Playback)
    }

    // SOUND3CNT_H (NR31, NR32) - Channel 3 Length/Volume (R/W)
    namespace SOUND3CNT_H {
      // Sound length; units of (256-n)/256s  (0-255)
      // used only if Bit 6 in NR34 (SOUND3CNT_X::USE_LENGTH) is set.
      constexpr std::uint16_t LENGTH(std::uint_fast16_t length) {
        assert(length < 256);
        return static_cast<std::uint16_t>(length & 255);
      }

      // Sound length; units of (256 - n) / 256 sec.  (0-255)
      // used only if Bit 6 in NR34 (SOUND3CNT_X::USE_LENGTH) is set.
      constexpr std::uint16_t GET_LENGTH(std::uint16_t value) {
        return value & 255;
      }

      // Sound Volume
      inline namespace VOLUME {
        constexpr std::uint16_t VOLUME0      = 0 << 13;   // Sound Volume (Mute/Zero)
        constexpr std::uint16_t VOLUME100    = 1 << 13;   // Sound Volume (100%)
        constexpr std::uint16_t VOLUME50     = 2 << 13;   // Sound Volume (50%)
        constexpr std::uint16_t VOLUME25     = 3 << 13;   // Sound Volume (25%)
      }

      constexpr std::uint16_t FORCE_VOLUME75 = 1 << 15;   // Force Volume (Force 75% regardless of VOLUME)
    }

    // SOUND3CNT_X (NR33, NR34) - Channel 3 Frequency/Control (R/W)
    namespace SOUND3CNT_X {
      // Sample Rate; 2097152 / (2048 - n) Hz (0-2047)
      constexpr std::uint16_t SAMPLERATE(std::uint_fast16_t sampleRate) {
        assert(sampleRate < 2048);
        return static_cast<std::uint16_t>(sampleRate & 2047);
      }

      // Sample Rate; 2097152 / (2048 - n) Hz (0-2047)
      constexpr std::uint16_t GET_SAMPLERATE(std::uint16_t value) {
        return value & 2047;
      }

      constexpr std::uint16_t USE_LENGTH = 1 << 14;   // Length Flag (1=Stop output when length in NR31 (SOUND3CNT_H) expires)
      constexpr std::uint16_t RESTART    = 1 << 15;   // Initial (Restart Sound)
    }


    // ### GBA Sound Channel 4 - Noise
    // https://problemkaputt.de/gbatek.htm#gbasoundchannel4noise

    // SOUND4CNT_L (NR41, NR42) - Channel 4 Length/Envelope (R/W)
    namespace SOUND4CNT_L {
      // Sound length; units of (64 - n) / 256 sec. (0-63)
      // used only if Bit 6 in NR14 (SOUND1CNT_X::USE_LENGTH or SOUND2CNT_H::USE_LENGTH) is set.
      constexpr std::uint16_t LENGTH(std::uint_fast16_t length) {
        assert(length < 64);
        return static_cast<std::uint16_t>(length & 63);
      }

      // Sound length; units of (64 - n) / 256 sec. (0-63)
      // used only if Bit 6 in NR14 (SOUND1CNT_X::USE_LENGTH or SOUND2CNT_H::USE_LENGTH) is set.
      constexpr std::uint16_t GET_LENGTH(std::uint16_t value) {
        return value & 7;
      }

      // Envelope Step-Time; units of n / 64 sec. (1-7, 0=No Envelope)
      constexpr std::uint16_t ENVSTEP(std::uint_fast16_t envelopeStepTime) {
        assert(envelopeStepTime < 8);
        return static_cast<std::uint16_t>(envelopeStepTime & 7) << 8;
      }

      // Envelope Step-Time; units of n / 64 sec. (1-7, 0=No Envelope)
      constexpr std::uint16_t GET_ENVSTEP(std::uint16_t value) {
        return (value >> 8) & 7;
      }

      constexpr std::uint16_t ENV_DEC = 0 << 14;    // Envelope Direction (Decrease)
      constexpr std::uint16_t ENV_INC = 1 << 14;    // Envelope Direction (Increase)

      // Initial Volume of envelope (1-15, 0=No Sound)
      constexpr std::uint16_t VOLUME(std::uint_fast16_t envelopeInitialVolume) {
        assert(envelopeInitialVolume < 16);
        return static_cast<std::uint16_t>(envelopeInitialVolume & 15) << 12;
      }

      // Initial Volume of envelope (1-15, 0=No Sound)
      constexpr std::uint16_t GET_VOLUME(std::uint16_t value) {
        return (value >> 12) & 15;
      }
    }

    // SOUND4CNT_H (NR43, NR44) - Channel 4 Frequency/Control (R/W)
    namespace SOUND4CNT_H {
      // Dividing Ratio of Frequencies (r)
      // Frequency = 524288 / r / 2^(s + 1) Hz; For r=0 assume r=0.5 instead
      constexpr std::uint16_t RATIO(std::uint_fast16_t r) {
        assert(r < 8);
        return static_cast<std::uint16_t>(r & 7);
      }

      // Dividing Ratio of Frequencies (r)
      // Frequency = 524288 / r / 2^(s + 1) Hz; For r=0 assume r=0.5 instead
      constexpr std::uint16_t GET_RATIO(std::uint16_t value) {
        return value & 7;
      }

      constexpr std::uint16_t STEP_15BIT = 0 << 3;    // Counter Step/Width (15 bits); X=X SHR 1, IF carry THEN Out=HIGH, X=X XOR 6000h ELSE Out=LOW
      constexpr std::uint16_t STEP_7BIT  = 1 << 3;    // Counter Step/Width ( 7 bits); X=X SHR 1, IF carry THEN Out=HIGH, X=X XOR 60h ELSE Out=LOW

      // Shift Clock Frequency (s)
      // Frequency = 524288 / r / 2^(s + 1) Hz; For r=0 assume r=0.5 instead
      constexpr std::uint16_t SHIFTFREQ(std::uint_fast16_t s) {
        assert(s < 16);
        return static_cast<std::uint16_t>(s & 15) << 4;
      }

      // Shift Clock Frequency (s)
      // Frequency = 524288 / r / 2^(s + 1) Hz; For r=0 assume r=0.5 instead
      constexpr std::uint16_t GET_SHIFTFREQ(std::uint16_t value) {
        return (value >> 4) & 15;
      }

      constexpr std::uint16_t USE_LENGTH = 1 << 14;   // Length Flag (Stop output when length in NR31 (SOUND3CNT_H) expires)
      constexpr std::uint16_t RESTART    = 1 << 15;   // Initial (Restart Sound)
    }


    // ### GBA Sound Channel A and B - DMA Sound
    // https://problemkaputt.de/gbatek.htm#gbasoundchannelaandbdmasound


    // ### GBA Sound Control Registers
    // https://problemkaputt.de/gbatek.htm#gbasoundcontrolregisters

    // SOUNDCNT_L (NR50, NR51) - Channel L/R Volume/Enable (R/W)
    namespace SOUNDCNT_L {
      // Sound 1-4 Master Volume RIGHT (0-7)
      constexpr std::uint16_t VOLUMER(std::uint_fast16_t volume) {
        assert(volume < 8);
        return static_cast<std::uint16_t>(volume & 7);
      }

      // Sound 1-4 Master Volume RIGHT (0-7)
      constexpr std::uint16_t GET_VOLUMER(std::uint16_t value) {
        return value & 7;
      }

      // Sound 1-4 Master Volume LEFT (0-7)
      constexpr std::uint16_t VOLUMEL(std::uint_fast16_t volume) {
        assert(volume < 8);
        return static_cast<std::uint16_t>(volume & 7) << 4;
      }

      // Sound 1-4 Master Volume LEFT (0-7)
      constexpr std::uint16_t GET_VOLUMEL(std::uint16_t value) {
        return (value >> 4) & 7;
      }


      // below are utilities

      constexpr std::uint16_t SOUND1_R = 1 <<  8;   // Sound 1 Enable RIGHT
      constexpr std::uint16_t SOUND2_R = 1 <<  9;   // Sound 2 Enable RIGHT
      constexpr std::uint16_t SOUND3_R = 1 << 10;   // Sound 3 Enable RIGHT
      constexpr std::uint16_t SOUND4_R = 1 << 11;   // Sound 4 Enable RIGHT

      constexpr std::uint16_t SOUND1_L = 1 << 12;   // Sound 1 Enable LEFT
      constexpr std::uint16_t SOUND2_L = 1 << 13;   // Sound 2 Enable LEFT
      constexpr std::uint16_t SOUND3_L = 1 << 14;   // Sound 3 Enable LEFT
      constexpr std::uint16_t SOUND4_L = 1 << 15;   // Sound 4 Enable LEFT

      // Sound 1-4 Master Volume RIGHT and LEFT (0-7)
      constexpr std::uint16_t VOLUMERL(std::uint_fast16_t volume) {
        assert(volume < 8);
        return VOLUMER(volume) | VOLUMEL(volume);
      }

      constexpr std::uint16_t SOUND1_RL = SOUND1_R | SOUND1_L;    // Sound 1 Enable RIGHT and LEFT
      constexpr std::uint16_t SOUND2_RL = SOUND2_R | SOUND2_L;    // Sound 2 Enable RIGHT and LEFT
      constexpr std::uint16_t SOUND3_RL = SOUND3_R | SOUND3_L;    // Sound 3 Enable RIGHT and LEFT
      constexpr std::uint16_t SOUND4_RL = SOUND4_R | SOUND4_L;    // Sound 4 Enable RIGHT and LEFT
    }

    // SOUNDCNT_H (GBA only) - DMA Sound Control/Mixing (R/W)
    namespace SOUNDCNT_H {
      inline namespace SOUND1234_VOLUME {
        constexpr std::uint16_t SOUND1234_VOLUME25  = 0 <<  0;    // Sound # 1-4 Volume (25%)
        constexpr std::uint16_t SOUND1234_VOLUME50  = 1 <<  0;    // Sound # 1-4 Volume (50%)
        constexpr std::uint16_t SOUND1234_VOLUME100 = 2 <<  0;    // Sound # 1-4 Volume (100%)
      }

      constexpr std::uint16_t SOUNDA_VOLUME50       = 0 <<  2;    // DMA Sound A Volume (50%)
      constexpr std::uint16_t SOUNDA_VOLUME100      = 1 <<  2;    // DMA Sound A Volume (100%)

      constexpr std::uint16_t SOUNDB_VOLUME50       = 0 <<  3;    // DMA Sound B Volume (50%)
      constexpr std::uint16_t SOUNDB_VOLUME100      = 1 <<  3;    // DMA Sound B Volume (100%)

      constexpr std::uint16_t SOUNDA_R              = 1 <<  8;    // DMA Sound A Enable RIGHT
      constexpr std::uint16_t SOUNDA_L              = 1 <<  9;    // DMA Sound A Enable LEFT
      constexpr std::uint16_t SOUNDA_TIMER0         = 0 << 10;    // DMA Sound A Timer Select (Timer 0)
      constexpr std::uint16_t SOUNDA_TIMER1         = 1 << 10;    // DMA Sound A Timer Select (Timer 1)
      constexpr std::uint16_t SOUNDA_RESET          = 1 << 11;    // DMA Sound A Reset FIFO

      constexpr std::uint16_t SOUNDB_R              = 1 << 12;    // DMA Sound B Enable RIGHT
      constexpr std::uint16_t SOUNDB_L              = 1 << 13;    // DMA Sound B Enable LEFT
      constexpr std::uint16_t SOUNDB_TIMER0         = 0 << 14;    // DMA Sound B Timer Select (Timer 0)
      constexpr std::uint16_t SOUNDB_TIMER1         = 1 << 14;    // DMA Sound B Timer Select (Timer 1)
      constexpr std::uint16_t SOUNDB_RESET          = 1 << 15;    // DMA Sound B Reset FIFO
    }

    // SOUNDCNT_X (NR52) - Sound on/off (R/W)
    namespace SOUNDCNT_X {
      constexpr std::uint16_t SOUND1_ON     = 1 << 0;   // Sound 1 ON flag (Read Only)
      constexpr std::uint16_t SOUND2_ON     = 1 << 1;   // Sound 2 ON flag (Read Only)
      constexpr std::uint16_t SOUND3_ON     = 1 << 2;   // Sound 3 ON flag (Read Only)
      constexpr std::uint16_t SOUND4_ON     = 1 << 3;   // Sound 4 ON flag (Read Only)

      constexpr std::uint16_t MASTER_ENABLE = 1 << 7;   // PSG/FIFO Master Enable (Read/Write)
    }

    // SOUNDBIAS - Sound PWM Control (R/W, see below)
    namespace SOUNDBIAS {
      // Bias Level (Default=100h, converting signed samples into unsigned)
      constexpr std::uint16_t LEVEL(std::uint_fast16_t level) {
        assert(level < 512);
        return static_cast<std::uint16_t>(level & 511);
      }

      // Bias Level (Default=100h, converting signed samples into unsigned)
      constexpr std::uint16_t GET_LEVEL(std::uint16_t value) {
        return value & 511;
      }

      // Amplitude Resolution/Sampling Cycle (Default=0)
      inline namespace AMPCYCLE {
        constexpr std::uint16_t AMPCYCLE_9BIT = 0 << 14;    // 9bit / 32.768kHz (Default, best for DMA channels A,B)
        constexpr std::uint16_t AMPCYCLE_8BIT = 1 << 14;    // 8bit / 65.536kHz
        constexpr std::uint16_t AMPCYCLE_7BIT = 2 << 14;    // 7bit / 131.072kHz
        constexpr std::uint16_t AMPCYCLE_6BIT = 3 << 14;    // 6bit / 262.144kHz (Best for PSG channels 1-4)
      }
    }
  }   // inline namespace constant
}   // namespace gba

#endif
