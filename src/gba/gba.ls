STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : 0x00004000;
HEAP_SIZE = DEFINED(HEAP_SIZE) ? HEAP_SIZE : 0x00004000;

OUTPUT_ARCH(arm)

ENTRY(_start)

MEMORY {
  WRAM (rwx) : ORIGIN = 0x02000000, LENGTH = 0x00040000
}

SECTIONS {
  .text : {
    KEEP(*(._entrypoint.rom ._entrypoint.rom* ._entrypoint.rom.*))
    KEEP(*(._binary_header.rom ._binary_header.rom* ._binary_header.rom.*))
    KEEP(*(._entrypoint.multiboot ._entrypoint.multiboot* ._entrypoint.multiboot.*))
    KEEP(*(._binary_header.multiboot ._binary_header.multiboot* ._binary_header.multiboot.*))
    KEEP(*(._entrypoint.joybus ._entrypoint.joybus* ._entrypoint.joybus.*))

    *(.text._entrypoint)
    *(.text._start_rom)
    *(.text._start_multiboot)
    *(.text._start_joybus)
    *(.text._start)

    *(.text)

    . = ALIGN(4);
  } > WRAM

  .data : {
    *(.data)
    . = ALIGN(4);
  } > WRAM

  .rodata : {
    *(.rodata*)
    . = ALIGN(4);
  } > WRAM

  __bss_start = .;
  __bss_start__ = .;
  .bss : {
   *(.bss .bss.*)
   . = ALIGN(4);
  } > WRAM
  __bss_end__ = .;
  _bss_end__ = .;

  __libc_IO_vtables : {
    *(__libc_IO_vtables*)
    . = ALIGN(4);
  } > WRAM

  .ARM.extab : {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } > WRAM

  __exidx_start = .;
  .ARM.exidx : {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > WRAM
  __exidx_end = .;

  /*
  .stack (NOLOAD) : {
    _sstack = .;
    . = . + STACK_SIZE;
    _estack = .;
  } > WRAM
  */

  . = ALIGN(4);
  _end = .;
  end = _end;

  .heap (NOLOAD) : {
    _heap_start = .;
    . = . + HEAP_SIZE;
    _heap_end = .;
  } > WRAM
}
