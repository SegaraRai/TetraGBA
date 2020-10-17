.section ._entrypoint.rom
  b _wrap_start_rom

.section ._entrypoint.multiboot
  b _wrap_start_multiboot

.section ._entrypoint.joybus
  b _wrap_start_joybus

.section ._entrypoint.ram
  b _wrap_start_ram

@-------------------------------------------------------------------------------
@-------------------------------------------------------------------------------

.section .text._entrypoint

_wrap_start_rom:
  bl _copy_and_jump_to_ram_if_executing_rom
  b _start_rom

_wrap_start_multiboot:
  bl _copy_and_jump_to_ram_if_executing_rom
  b _start_multiboot

_wrap_start_joybus:
  bl _copy_and_jump_to_ram_if_executing_rom
  b _start_joybus

_wrap_start_ram:
  b _start_ram

@-------------------------------------------------------------------------------
@-------------------------------------------------------------------------------

_copy_and_jump_to_ram_if_executing_rom:

@@------------------------------------------------------------------------------
@@ Codes below are obtained from devkitPro
@@ https://github.com/devkitPro/devkitarm-crtls/blob/master/gba_crt0.s
@@ Licensed under the Mozilla Public License Version 2.0 : https://mozilla.org/MPL/2.0/
@@------------------------------------------------------------------------------

@-------------------------------------------------------------------------------
@ Enter Thumb mode
@-------------------------------------------------------------------------------
  add r0, pc, #1
  bx r0

.thumb
  mov r0, pc
  lsl r0, #5          @ Are we running from ROM (0x8000000 or higher) ?
  bcc .Return         @ No, so no need to do a copy.

@-------------------------------------------------------------------------------
@ We were started in ROM, silly emulators. :P
@ So we need to copy to ExWRAM.
@-------------------------------------------------------------------------------
  mov r2, #2
  lsl r2, r2, #24     @ r2= 0x02000000
  ldr r3, = _end      @ last EWRAM address
  sub r3, r2          @ r3= actual binary size
  mov r6, r2          @ r6= 0x02000000
  lsl r1, r2, #2      @ r1= 0x08000000
  bl CopyMem
  add r6, r6, #0xF0   @ r6= ._entrypoint.ram
  bx r6               @ Jump to the code to execute

.Return:
  bx lr

@-------------------------------------------------------------------------------
@ Copy memory
@-------------------------------------------------------------------------------
@ r1 = Source Address
@ r2 = Dest Address
@ r3 = Length
@-------------------------------------------------------------------------------
CopyMem:
  mov r0, #3          @ These commands are used in cases where
  add r3, r0          @ the length is not a multiple of 4,
  bic r3, r0          @ even though it should be.
  beq .CIDExit        @ Length is zero so exit
.CIDLoop:
  ldmia r1!, {r0}
  stmia r2!, {r0}
  sub r3, #4
  bne .CIDLoop
.CIDExit:
  bx lr
