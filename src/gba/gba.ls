STACK_SIZE = DEFINED(STACK_SIZE) ? STACK_SIZE : 0x00004000;
HEAP_SIZE = DEFINED(HEAP_SIZE) ? HEAP_SIZE : 0x00004000;

OUTPUT_ARCH(arm)

ENTRY(_start)

MEMORY {
  WRAM (rwx) : ORIGIN = 0x02000000, LENGTH = 0x00040000
}

SECTIONS {
  .text : {
    *(.text._start)

    KEEP(*(._binary_header ._binary_header* ._binary_header.*))

    /*
    BYTE(0x00);
    BYTE(0x00);
    BYTE(0x47);
    BYTE(0x42);
    BYTE(0x41);
    BYTE(0x2E);
    BYTE(0x4C);
    BYTE(0x53);
    BYTE(0x00);
    BYTE(0x00);
    */

    *(.text)
  } > WRAM

  .data : {
    *(.data)
  } > WRAM

  .rodata : {
    *(.rodata*)
  } > WRAM

  __bss_start = .;
  __bss_start__ = .;
  .bss : {
   *(.bss .bss.*)
   . = ALIGN(. != 0 ? 32 / 8 : 1);
  } > WRAM
  __bss_end__ = .;
  _bss_end__ = .;

  __libc_IO_vtables : {
    *(__libc_IO_vtables*)
  } > WRAM

  .ARM.extab : {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
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
