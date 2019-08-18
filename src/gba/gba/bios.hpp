#ifndef _gba_bios_hpp_
#define _gba_bios_hpp_

#include <cstdint>


// https://problemkaputt.de/gbatek.htm#biosfunctions
// When invoking SWIs from inside of ARM state specify SWI NN * 10000h, instead of SWI NN as in THUMB state.


namespace gba::bios {
  static_assert(sizeof(int) == 4);
  static_assert(sizeof(unsigned int) == 4);


  // Basic Functions

  // BIOS Reset Functions

  /*
  SWI 00h (GBA/NDS7/NDS9) - SoftReset
  Clears 200h bytes of RAM (containing stacks, and BIOS IRQ vector/flags), initializes system, supervisor, and irq stack pointers, sets R0-R12, LR_svc, SPSR_svc, LR_irq, and SPSR_irq to zero, and enters system mode.
  Note that the NDS9 stack registers are hardcoded (the DTCM base should be set to the default setting of 0800000h). The NDS9 function additionally flushes caches and write buffer, and sets the CP15 control register to 12078h.
    Host  sp_svc    sp_irq    sp_sys    zerofilled area       return address
    GBA   3007FE0h  3007FA0h  3007F00h  [3007E00h..3007FFFh]  Flag[3007FFAh]
    NDS7  380FFDCh  380FFB0h  380FF00h  [380FE00h..380FFFFh]  Addr[27FFE34h]
    NDS9  0803FC0h  0803FA0h  0803EC0h  [DTCM+3E00h..3FFFh]   Addr[27FFE24h]
  The NDS7/NDS9 return addresses at [27FFE34h/27FFE24h] are usually containing copies of Cartridge Header [034h/024h] entry points, which may select ARM/THUMB state via bit0. The GBA return address 8bit flag is interpreted as 00h=8000000h (ROM), or 01h-FFh=2000000h (RAM), entered in ARM state.
  Note: The reset is applied only to the CPU that has executed the SWI (ie. on the NDS, the other CPU will remain unaffected).
  Return: Does not return to calling procedure, instead, loads the above return address into R14, and then jumps to that address by a "BX R14" opcode.
  */

  void SoftReset();

  /*
  SWI 01h (GBA) - RegisterRamReset
  Resets the I/O registers and RAM specified in ResetFlags. However, it does not clear the CPU internal RAM area from 3007E00h-3007FFFh.
    r0  ResetFlags
         Bit   Expl.
         0     Clear 256K on-board WRAM  ;-don't use when returning to WRAM
         1     Clear 32K on-chip WRAM    ;-excluding last 200h bytes
         2     Clear Palette
         3     Clear VRAM
         4     Clear OAM              ;-zerofilled! does NOT disable OBJs!
         5     Reset SIO registers    ;-switches to general purpose mode!
         6     Reset Sound registers
         7     Reset all other registers (except SIO, Sound)
  Return: No return value.
  Bug: LSBs of SIODATA32 are always destroyed, even if Bit5 of R0 was cleared.
  The function always switches the screen into forced blank by setting DISPCNT=0080h (regardless of incoming R0, screen becomes white).
  */

  void RegisterRamReset(unsigned char resetFlags);

  // BIOS Halt Functions

  /*
  SWI 02h (GBA) or SWI 06h (NDS7/NDS9/DSi7/DSi9) - Halt
  Halts the CPU until an interrupt request occurs. The CPU is switched into low-power mode, all other circuits (video, sound, timers, serial, keypad, system clock) are kept operating.
  Halt mode is terminated when any enabled interrupts are requested, that is when (IE AND IF) is not zero, the GBA locks up if that condition doesn't get true. However, the state of CPUs IRQ disable bit in CPSR register, and the IME register are don't care, Halt passes through even if either one has disabled interrupts.
  On GBA and NDS7/DSi7, Halt is implemented by writing to HALTCNT, Port 4000301h. On NDS9/DSi9, Halt is implemted by writing to System Control Coprocessor (mov p15,0,c7,c0,4,r0 opcode), this opcode hangs if IME=0.
  No parameters, no return value.
  (GBA/NDS7/DSi7: all registers unchanged, NDS9/DSi9: R0 destroyed)
  */

  void Halt();

  /*
  SWI 03h (GBA) - Stop
  Switches the GBA into very low power mode (to be used similar as a screen-saver). The CPU, System Clock, Sound, Video, SIO-Shift Clock, DMAs, and Timers are stopped.
  Stop state can be terminated by the following interrupts only (as far as enabled in IE register): Joypad, Game Pak, or General-Purpose-SIO.
  "The system clock is stopped so the IF flag is not set."
  Preparation for Stop:
  Disable Video before implementing Stop (otherwise Video just freezes, but still keeps consuming battery power). Possibly required to disable Sound also? Obviously, it'd be also recommended to disable any external hardware (such like Rumble or Infra-Red) as far as possible.
  No parameters, no return value.
  */

  void Stop();

  /*
  SWI 04h (GBA/NDS7/NDS9/DSi7/DSi9) - IntrWait ;DSi7/DSi9=bugged?
  Continues to wait in Halt state until one (or more) of the specified interrupt(s) do occur. The function forcefully sets IME=1. When using multiple interrupts at the same time, this function is having less overhead than repeatedly calling the Halt function.
    r0    0=Return immediately if an old flag was already set (NDS9: bugged!)
          1=Discard old flags, wait until a NEW flag becomes set
    r1    Interrupt flag(s) to wait for (same format as IE/IF registers)
    r2    DSi7 only: Extra flags (same format as DSi7's IE2/IF2 registers)
  Caution: When using IntrWait or VBlankIntrWait, the user interrupt handler MUST update the BIOS Interrupt Flags value in RAM; when acknowleding processed interrupt(s) by writing a value to the IF register, the same value should be also ORed to the BIOS Interrupt Flags value, at following memory location:
    Host     GBA (16bit)  NDS7 (32bit)  NDS9 (32bit)  DSi7-IF2 (32bit)
    Address  [3007FF8h]   [380FFF8h]    [DTCM+3FF8h]  [380FFC0h]
  NDS9: BUG: No Discard (r0=0) doesn't work. The function always waits for at least one IRQ to occur (no matter which, including IRQs that are not selected in r1), even if the desired flag was already set. NB. the same bug is also found in the GBA/NDS7 functions, but it's compensated by a second bug, ie. the GBA/NDS7 functions are working okay because their "bug doesn't work".
  Return: No return value, the selected flag(s) are automatically reset in BIOS Interrupt Flags value in RAM upon return.
  DSi9: BUG: The function tries to enter Halt state via Port 4000301h (which would be okay on ARM7, but it's probably ignored on ARM9, which should normally use CP15 to enter Halt state; if Port 4000301h is really ignored, then the function will "successfully" wait for interrupts, but without actually entering any kind of low power mode).
  DSi7: BUG: The function tries to wait for IF and IF2 interrupts, but it does accidently ignore the old IF interrupts, and works only with new IF2 ones.
  */

  void IntrWait(bool discard, unsigned int interruptFlags);

  /*
  SWI 05h (GBA/NDS7/NDS9/DSi7/DSi9) - VBlankIntrWait ;DSi7/DSi9=bugged?
  Continues to wait in Halt status until a new V-Blank interrupt occurs.
  The function sets r0=1 and r1=1 (plus r2=0 on DSi7) and does then execute IntrWait (SWI 04h), see IntrWait for details.
  No parameters, no return value.
  */

  void VBlankIntrWait();

  // BIOS Arithmetic Functions

  /*
  SWI 06h (GBA) or SWI 09h (NDS7/NDS9/DSi7/DSi9) - Div
  Signed Division, r0/r1.
    r0  signed 32bit Number
    r1  signed 32bit Denom
  Return:
    r0  Number DIV Denom ;signed
    r1  Number MOD Denom ;signed
    r3  ABS (Number DIV Denom) ;unsigned
  */

  int Div(int num, int denom, int& mod, unsigned int& divAbs);
  int Div(int num, int denom, int& mod);
  int Div(int num, int denom, unsigned int& divAbs);
  int Div(int num, int denom);

  /*
  SWI 07h (GBA) - DivArm
  Same as above (SWI 06h Div), but incoming parameters are exchanged, r1/r0 (r0=Denom, r1=number). For compatibility with ARM's library. Slightly slower (3 clock cycles) than SWI 06h.
  */

  int DivArm(int denom, int num, int& mod, unsigned int& divAbs);
  int DivArm(int denom, int num, int& mod);
  int DivArm(int denom, int num, unsigned int& divAbs);
  int DivArm(int denom, int num);

  /*
  SWI 08h (GBA) or SWI 0Dh (NDS7/NDS9/DSi7/DSi9) - Sqrt
  Calculate square root.
    r0   unsigned 32bit number
  Return:
    r0   unsigned 16bit number
  The result is an integer value, so Sqrt(2) would return 1, to avoid this inaccuracy, shift left incoming number by 2*N as much as possible (the result is then shifted left by 1*N). Ie. Sqrt(2 shl 30) would return 1.41421 shl 15.
  Note: The NDS9 and DSi9 additionally support hardware square root calculation, by math coprocessor, accessed via I/O Ports, however, the SWI function is a raw software calculation.
  */

  unsigned short Sqrt(unsigned int num);

  /*
  SWI 09h (GBA) - ArcTan
  Calculates the arc tangent.
    r0   Tan, 16bit (1bit sign, 1bit integral part, 14bit decimal part)
  Return:
    r0   "-PI/2<THETA/<PI/2" in a range of C000h-4000h.
  Note: there is a problem in accuracy with "THETA<-PI/4, PI/4<THETA".
  */

  unsigned short ArcTan(unsigned short num);

  /*
  SWI 0Ah (GBA) - ArcTan2
  Calculates the arc tangent after correction processing.
  Use this in normal situations.
    r0   X, 16bit (1bit sign, 1bit integral part, 14bit decimal part)
    r1   Y, 16bit (1bit sign, 1bit integral part, 14bit decimal part)
  Return:
    r0   0000h-FFFFh for 0<=THETA<2PI.
  */

  unsigned short ArcTan2(unsigned short x, unsigned short y);

  // BIOS Memory Copy

  /*
  SWI 0Ch (GBA/NDS7/NDS9/DSi7/DSi9) - CpuFastSet
  Memory copy/fill in units of 32 bytes. Memcopy is implemented as repeated LDMIA/STMIA [Rb]!,r2-r9 instructions. Memfill as single LDR followed by repeated STMIA [Rb]!,r2-r9.
  After processing all 32-byte-blocks, the NDS/DSi additonally processes the remaining words as 4-byte blocks. BUG: The NDS/DSi uses the fast 32-byte-block processing only for the first N bytes (not for the first N words), so only the first quarter of the memory block is FAST, the remaining three quarters are SLOWLY copied word-by-word.
  The length is specifed as wordcount, ie. the number of bytes divided by 4.
  On the GBA, the length should be a multiple of 8 words (32 bytes) (otherwise the GBA is forcefully rounding-up the length). On NDS/DSi, the length may be any number of words (4 bytes).
    r0    Source address        (must be aligned by 4)
    r1    Destination address   (must be aligned by 4)
    r2    Length/Mode
            Bit 0-20  Wordcount (GBA: rounded-up to multiple of 8 words)
            Bit 24    Fixed Source Address (0=Copy, 1=Fill by WORD[r0])
  Return: No return value, Data written to destination address.

  Note: On GBA, NDS7 and DSi7, these two functions will silently reject to do anything if the source start or end addresses are reaching into the BIOS area. The NDS9 and DSi9 don't have such read-proctections.
  */

  void CpuSet(const void* src, void* dest, unsigned int modeAndLength);

  /*
  SWI 0Bh (GBA/NDS7/NDS9/DSi7/DSi9) - CpuSet
  Memory copy/fill in units of 4 bytes or 2 bytes. Memcopy is implemented as repeated LDMIA/STMIA [Rb]!,r3 or LDRH/STRH r3,[r0,r5] instructions. Memfill as single LDMIA or LDRH followed by repeated STMIA [Rb]!,r3 or STRH r3,[r0,r5].
  The length must be a multiple of 4 bytes (32bit mode) or 2 bytes (16bit mode). The (half)wordcount in r2 must be length/4 (32bit mode) or length/2 (16bit mode), ie. length in word/halfword units rather than byte units.
    r0    Source address        (must be aligned by 4 for 32bit, by 2 for 16bit)
    r1    Destination address   (must be aligned by 4 for 32bit, by 2 for 16bit)
    r2    Length/Mode
            Bit 0-20  Wordcount (for 32bit), or Halfwordcount (for 16bit)
            Bit 24    Fixed Source Address (0=Copy, 1=Fill by {HALF}WORD[r0])
            Bit 26    Datasize (0=16bit, 1=32bit)
  Return: No return value, Data written to destination address.

  Note: On GBA, NDS7 and DSi7, these two functions will silently reject to do anything if the source start or end addresses are reaching into the BIOS area. The NDS9 and DSi9 don't have such read-proctections.
  */

  void CpuFastSet(const void* src, void* dest, unsigned int modeAndLength);

  // BIOS Misc Functions

  /*
  SWI 0Dh (GBA) - GetBiosChecksum (Undocumented)
  Calculates the checksum of the BIOS ROM (by reading in 32bit units, and adding up these values). IRQ and FIQ are disabled during execution.
  The checksum is BAAE187Fh (GBA and GBA SP), or BAAE1880h (DS in GBA mode, whereas the only difference is that the byte at [3F0Ch] is changed from 00h to 01h, otherwise the BIOS is 1:1 same as GBA BIOS, it does even include multiboot code).
  Parameters: None. Return: r0=Checksum.
  */

  unsigned int GetBiosChecksum();

  // BIOS Rotation/Scaling Functions

  /*
  SWI 0Eh (GBA) - BgAffineSet
  Used to calculate BG Rotation/Scaling parameters.
    r0   Pointer to Source Data Field with entries as follows:
          s32  Original data's center X coordinate (8bit fractional portion)
          s32  Original data's center Y coordinate (8bit fractional portion)
          s16  Display's center X coordinate
          s16  Display's center Y coordinate
          s16  Scaling ratio in X direction (8bit fractional portion)
          s16  Scaling ratio in Y direction (8bit fractional portion)
          u16  Angle of rotation (8bit fractional portion) Effective Range 0-FFFF
    r1   Pointer to Destination Data Field with entries as follows:
          s16  Difference in X coordinate along same line
          s16  Difference in X coordinate along next line
          s16  Difference in Y coordinate along same line
          s16  Difference in Y coordinate along next line
          s32  Start X coordinate
          s32  Start Y coordinate
    r2   Number of Calculations
  Return: No return value, Data written to destination address.
  */

  struct BgAffineSetParam1 {
    std::int32_t orgCenterX;      // Original data's center X coordinate (8bit fractional portion)
    std::int32_t orgCenterY;      // Original data's center Y coordinate (8bit fractional portion)
    std::int16_t dispCenterX;     // Display's center X coordinate
    std::int16_t dispCenterY;     // Display's center Y coordinate
    std::int16_t scalingRatioX;   // Display's center X coordinate
    std::int16_t scalingRatioY;   // Display's center Y coordinate
    std::uint16_t angle;          // Angle of rotation (8bit fractional portion) Effective Range 0-FFFF
  } __attribute__((__packed__));

  struct BgAffineSetParam2 {
    std::int16_t diffX;       // Difference in X coordinate along same line
    std::int16_t diffXNext;   // Difference in X coordinate along next line
    std::int16_t diffY;       // Difference in Y coordinate along same line
    std::int16_t diffYNext;   // Difference in Y coordinate along next line
    std::int32_t startX;      // Start X coordinate
    std::int32_t startY;      // Start Y coordinate
  } __attribute__((__packed__));

  void BgAffineSet(const BgAffineSetParam1& p1, const BgAffineSetParam2& p2, unsigned int num);

  /*
  SWI 0Fh (GBA) - ObjAffineSet
  Calculates and sets the OBJ's affine parameters from the scaling ratio and angle of rotation.
  The affine parameters are calculated from the parameters set in Srcp.
  The four affine parameters are set every Offset bytes, starting from the Destp address.
  If the Offset value is 2, the parameters are stored contiguously. If the value is 8, they match the structure of OAM.
  When Srcp is arrayed, the calculation can be performed continuously by specifying Num.
    r0   Source Address, pointing to data structure as such:
          s16  Scaling ratio in X direction (8bit fractional portion)
          s16  Scaling ratio in Y direction (8bit fractional portion)
          u16  Angle of rotation (8bit fractional portion) Effective Range 0-FFFF
    r1   Destination Address, pointing to data structure as such:
          s16  Difference in X coordinate along same line
          s16  Difference in X coordinate along next line
          s16  Difference in Y coordinate along same line
          s16  Difference in Y coordinate along next line
    r2   Number of calculations
    r3   Offset in bytes for parameter addresses (2=continuous, 8=OAM)
  Return: No return value, Data written to destination address.
  */

  struct ObjAffineSetParam1 {
    std::int16_t scalingRatioX;   // Display's center X coordinate
    std::int16_t scalingRatioY;   // Display's center Y coordinate
    std::uint16_t angle;          // Angle of rotation (8bit fractional portion) Effective Range 0-FFFF
  } __attribute__((__packed__));

  struct ObjAffineSetParam2 {
    std::int16_t diffX;       // Difference in X coordinate along same line
    std::int16_t diffXNext;   // Difference in X coordinate along next line
    std::int16_t diffY;       // Difference in Y coordinate along same line
    std::int16_t diffYNext;   // Difference in Y coordinate along next line
  } __attribute__((__packed__));

  void ObjAffineSet(const ObjAffineSetParam1& p1, const ObjAffineSetParam2& p2, unsigned int num, unsigned int offset);


  // Decompression Functions
  // BIOS Decompression Functions

  /*
  BitUnPack - SWI 10h (GBA/NDS7/NDS9/DSi7/DSi9)
  Used to increase the color depth of bitmaps or tile data. For example, to convert a 1bit monochrome font into 4bit or 8bit GBA tiles. The Unpack Info is specified separately, allowing to convert the same source data into different formats.
    r0  Source Address      (no alignment required)
    r1  Destination Address (must be 32bit-word aligned)
    r2  Pointer to UnPack information:
         16bit  Length of Source Data in bytes     (0-FFFFh)
         8bit   Width of Source Units in bits      (only 1,2,4,8 supported)
         8bit   Width of Destination Units in bits (only 1,2,4,8,16,32 supported)
         32bit  Data Offset (Bit 0-30), and Zero Data Flag (Bit 31)
        The Data Offset is always added to all non-zero source units.
        If the Zero Data Flag was set, it is also added to zero units.
  Data is written in 32bit units, Destination can be Wram or Vram. The size of unpacked data must be a multiple of 4 bytes. The width of source units (plus the offset) should not exceed the destination width.
  Return: No return value, Data written to destination address.
  */

  struct BitUnPackParam {
    std::uint16_t srcSize;
    std::uint8_t srcWidth;
    std::uint8_t destWidth;
    std::uint32_t offsetAndFlag;
  } __attribute__((__packed__));

  void BitUnPack(const void* src, void* dest, const BitUnPackParam& param);

  /*
  LZ77UnCompReadNormalWrite8bit (Wram) - SWI 11h (GBA/NDS7/NDS9/DSi7/DSi9)
  LZ77UnCompReadNormalWrite16bit (Vram) - SWI 12h (GBA)
  LZ77UnCompReadByCallbackWrite8bit - SWI 01h (DSi7/DSi9)
  LZ77UnCompReadByCallbackWrite16bit - SWI 12h (NDS), SWI 02h or 19h (DSi)
  Expands LZ77-compressed data. The Wram function is faster, and writes in units of 8bits. For the Vram function the destination must be halfword aligned, data is written in units of 16bits.
  CAUTION: Writing 16bit units to [dest-1] instead of 8bit units to [dest] means that reading from [dest-1] won't work, ie. the "Vram" function works only with disp=001h..FFFh, but not with disp=000h.
  If the size of the compressed data is not a multiple of 4, please adjust it as much as possible by padding with 0. Align the source address to a 4-Byte boundary.
    r0  Source address, pointing to data as such:
         Data header (32bit)
           Bit 0-3   Reserved
           Bit 4-7   Compressed type (must be 1 for LZ77)
           Bit 8-31  Size of decompressed data
         Repeat below. Each Flag Byte followed by eight Blocks.
         Flag data (8bit)
           Bit 0-7   Type Flags for next 8 Blocks, MSB first
         Block Type 0 - Uncompressed - Copy 1 Byte from Source to Dest
           Bit 0-7   One data byte to be copied to dest
         Block Type 1 - Compressed - Copy N+3 Bytes from Dest-Disp-1 to Dest
           Bit 0-3   Disp MSBs
           Bit 4-7   Number of bytes to copy (minus 3)
           Bit 8-15  Disp LSBs
    r1  Destination address
    r2  Callback parameter        ;\for NDS/DSi "ReadByCallback" variants only
    r3  Callback structure        ;/(see Callback notes below)
  Return: No return value.
  */

  void LZ77UnCompReadNormalWrite8bit(const void* src, void* dest);    // WRAM
  void LZ77UnCompReadNormalWrite16bit(const void* src, void* dest);   // VRAM

  /*
  HuffUnCompReadNormal - SWI 13h (GBA)
  HuffUnCompReadByCallback - SWI 13h (NDS/DSi)
  The decoder starts in root node, the separate bits in the bitstream specify if the next node is node0 or node1, if that node is a data node, then the data is stored in memory, and the decoder is reset to the root node. The most often used data should be as close to the root node as possible. For example, the 4-byte string "Huff" could be compressed to 6 bits: 10-11-0-0, with root.0 pointing directly to data "f", and root.1 pointing to a child node, whose nodes point to data "H" and data "u".
  Data is written in units of 32bits, if the size of the compressed data is not a multiple of 4, please adjust it as much as possible by padding with 0.
  Align the source address to a 4Byte boundary.
    r0  Source Address, aligned by 4, pointing to:
         Data Header (32bit)
           Bit0-3   Data size in bit units (normally 4 or 8)
           Bit4-7   Compressed type (must be 2 for Huffman)
           Bit8-31  24bit size of decompressed data in bytes
         Tree Size (8bit)
           Bit0-7   Size of Tree Table/2-1 (ie. Offset to Compressed Bitstream)
         Tree Table (list of 8bit nodes, starting with the root node)
          Root Node and Non-Data-Child Nodes are:
           Bit0-5   Offset to next child node,
                    Next child node0 is at (CurrentAddr AND NOT 1)+Offset*2+2
                    Next child node1 is at (CurrentAddr AND NOT 1)+Offset*2+2+1
           Bit6     Node1 End Flag (1=Next child node is data)
           Bit7     Node0 End Flag (1=Next child node is data)
          Data nodes are (when End Flag was set in parent node):
           Bit0-7   Data (upper bits should be zero if Data Size is less than 8)
         Compressed Bitstream (stored in units of 32bits)
           Bit0-31  Node Bits (Bit31=First Bit)  (0=Node0, 1=Node1)
    r1  Destination Address
    r2  Callback temp buffer      ;\for NDS/DSi "ReadByCallback" variants only
    r3  Callback structure        ;/(see Callback notes below)
  Return: No return value, Data written to destination address.
  */

  void HuffUnCompReadNormal(const void* src, void* dest);

  /*
  RLUnCompReadNormalWrite8bit (Wram) - SWI 14h (GBA/NDS7/NDS9/DSi7/DSi9)
  RLUnCompReadNormalWrite16bit (Vram) - SWI 15h (GBA)
  RLUnCompReadByCallbackWrite16bit - SWI 15h (NDS7/NDS9/DSi7/DSi9)
  Expands run-length compressed data. The Wram function is faster, and writes in units of 8bits. For the Vram function the destination must be halfword aligned, data is written in units of 16bits.
  If the size of the compressed data is not a multiple of 4, please adjust it as much as possible by padding with 0. Align the source address to a 4Byte boundary.
    r0  Source Address, pointing to data as such:
         Data header (32bit)
           Bit 0-3   Reserved
           Bit 4-7   Compressed type (must be 3 for run-length)
           Bit 8-31  Size of decompressed data
         Repeat below. Each Flag Byte followed by one or more Data Bytes.
         Flag data (8bit)
           Bit 0-6   Expanded Data Length (uncompressed N-1, compressed N-3)
           Bit 7     Flag (0=uncompressed, 1=compressed)
         Data Byte(s) - N uncompressed bytes, or 1 byte repeated N times
    r1  Destination Address
    r2  Callback parameter        ;\for NDS/DSi "ReadByCallback" variants only
    r3  Callback structure        ;/(see Callback notes below)
  Return: No return value, Data written to destination address.
  */

  void RLUnCompReadNormalWrite8bit(const void* src, void* dest);    // WRAM
  void RLUnCompReadNormalWrite16bit(const void* src, void* dest);   // VRAM

  /*
  Diff8bitUnFilterWrite8bit (Wram) - SWI 16h (GBA/NDS9/DSi9)
  Diff8bitUnFilterWrite16bit (Vram) - SWI 17h (GBA)
  Diff16bitUnFilter - SWI 18h (GBA/NDS9/DSi9)
  These aren't actually real decompression functions, destination data will have exactly the same size as source data. However, assume a bitmap or wave form to contain a stream of increasing numbers such like 10..19, the filtered/unfiltered data would be:
    unfiltered:   10  11  12  13  14  15  16  17  18  19
    filtered:     10  +1  +1  +1  +1  +1  +1  +1  +1  +1
  In this case using filtered data (combined with actual compression algorithms) will obviously produce better compression results.
  Data units may be either 8bit or 16bit used with Diff8bit or Diff16bit functions respectively.
    r0  Source address (must be aligned by 4) pointing to data as follows:
         Data Header (32bit)
           Bit 0-3   Data size (must be 1 for Diff8bit, 2 for Diff16bit)
           Bit 4-7   Type (must be 8 for DiffFiltered)
           Bit 8-31  24bit size after decompression
         Data Units (each 8bit or 16bit depending on used SWI function)
           Data0          ;original data
           Data1-Data0    ;difference data
           Data2-Data1    ;...
           Data3-Data2
           ...
    r1  Destination address
  Return: No return value, Data written to destination address.
  */

  void Diff8bitUnFilterWrite8bit(const void* src, void* dest);    // WRAM
  void Diff8bitUnFilterWrite16bit(const void* src, void* dest);   // VRAM
  void Diff16bitUnFilter(const void* src, void* dest);


  // Sound (and Multiboot/HardReset/CustomHalt)

  /*
  // TODO

  void SoundBias();

  void SoundDriverInit();

  void SoundDriverMode();

  void SoundDriverMain();

  void SoundDriverVSync();

  void SoundChannelClear();

  void MidiKey2Freq();

  void SoundWhatever0();

  void SoundWhatever1();

  void SoundWhatever2();

  void SoundWhatever3();

  void SoundWhatever4();
  //*/

  /*
  SWI 25h (GBA) - MultiBoot
  This function uploads & starts program code to slave GBAs, allowing to launch programs on slave units even if no cartridge is inserted into the slaves (this works because all GBA BIOSes contain built-in download procedures in ROM).
  However, the SWI 25h BIOS upload function covers only 45% of the required Transmission Protocol, the other 55% must be coded in the master cartridge (see Transmission Protocol below).
    r0  Pointer to MultiBootParam structure
    r1  Transfer Mode (undocumented)
         0=256KHz, 32bit, Normal mode    (fast and stable)
         1=115KHz, 16bit, MultiPlay mode (default, slow, up to three slaves)
         2=2MHz,   32bit, Normal mode    (fastest but maybe unstable)
    Note: HLL-programmers that are using the MultiBoot(param_ptr) macro cannot
    specify the transfer mode and will be forcefully using MultiPlay mode.
  Return:
    r0  0=okay, 1=failed

  Multiboot Parameter Structure
  Size of parameter structure should be 4Ch bytes (the current GBA BIOS uses only first 44h bytes though). The following entries must be set before calling SWI 25h:
    Addr Size Name/Expl.
    14h  1    handshake_data (entry used for normal mode only)
    19h  3    client_data[1,2,3]
    1Ch  1    palette_data
    1Eh  1    client_bit (Bit 1-3 set if child 1-3 detected)
    20h  4    boot_srcp  (typically 8000000h+0C0h)
    24h  4    boot_endp  (typically 8000000h+0C0h+length)
  The transfer length (excluding header data) should be a multiple of 10h, minimum length 100h, max 3FF40h (ca. 256KBytes). Set palette_data as "81h+color*10h+direction*8+speed*2", or as "0f1h+color*2" for fixed palette, whereas color=0..6, speed=0..3, direction=0..1. The other entries (handshake_data, client_data[1-3], and client_bit) must be same as specified in Transmission Protocol (see below hh,cc,y).

  https://problemkaputt.de/gbatek.htm#biosmultibootsinglegamepak
  */

  struct MultiBootParam {
    std::uint8_t dummy1[20];        // 0x00
    std::uint8_t handshake_data;    // 0x14
    std::uint8_t dummy2[4];         // 0x15
    std::uint8_t client_data[3];    // 0x19
    std::uint8_t palette_data;      // 0x1C
    std::uint8_t dummy3[1];         // 0x1D
    std::uint8_t client_bit;        // 0x1E
    std::uint8_t dummy4[1];         // 0x1F
    std::uint32_t boot_srcp;        // 0x20
    std::uint32_t boot_endp;        // 0x24
    std::uint8_t dummy5[36];        // 0x28
    // 0x4C
  } __attribute__((__packed__));

  bool MultiBoot(const MultiBootParam& param, unsigned int transferMode);

  /*
  SWI 26h (GBA) - HardReset (Undocumented)
  This function reboots the GBA (including for getting through the time-consuming nintendo intro, which is making the function particularly useless and annoying).
  Parameters: None. Return: Never/Reboot.
  Execution Time: About 2 seconds (!)
  */

  void HardReset();

  /*
  SWI 27h (GBA) or SWI 1Fh (NDS7/DSi7) - CustomHalt (Undocumented)
  Writes the 8bit parameter value to HALTCNT, below values are equivalent to Halt and Stop/Sleep functions, other values reserved, purpose unknown.
    r2  8bit parameter (GBA: 00h=Halt, 80h=Stop) (NDS7/DSi7: 80h=Halt, C0h=Sleep)
  No return value.
  */

  /*
  // TODO

  void CustomHalt(unsigned char param);

  void SoundDriverVSyncOff();

  void SoundDriverVSyncOn();

  void SoundGetJumpList();
  //*/
}   // namespace gba::bios


#endif
