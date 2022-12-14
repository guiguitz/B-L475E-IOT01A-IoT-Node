/*
*****************************************************************************
**

**  File        : LinkerScript.ld
**
**  Abstract    : Linker script for STM32L496AGIx Device with
**                512KByte FLASH, 128KByte RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**
**  Distribution: The file is distributed as is, without any warranty
**                of any kind.
**
**  (c)Copyright Ac6.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Ac6 permit registered System Workbench for MCU users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the System Workbench for MCU toolchain.
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

INCLUDE mapping_fwimg.ld
INCLUDE mapping_sbsfu.ld

EndOfRam = 0x2007C000;
__Credential__ = __ICFEDIT_SB_region_ROM_end__ + 1;
CredentialLen = 12K;


/* Highest address of the user mode stack */
_estack = EndOfRam;    /* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x18000;      /* required amount of heap  */
_Min_Stack_Size = 0x2000; /* required amount of stack */

/* Specify the memory areas */
/* SRAM2 : [0x2007C000, 0x2007FFFF] : 16 KBytes                                    */
/*         [0x2007C000,0x2007C1FF]  : 512 Bytes     : Ethernet dma descripteurs    */
/*         [0x2007C200,0x2007FFFF]  : 15.5 KBytes   : Ethernet Buffers             */


/* Specific ROM/RAM UserApp definition */
__ICFEDIT_intvec_start__  = __ICFEDIT_region_SLOT_0_start__ + 0x400;
APPLI_region_ROM_start    = __ICFEDIT_region_SLOT_0_start__ + VECTOR_SIZE + 0x400;
APPLI_region_ROM_length   = __ICFEDIT_region_SLOT_0_end__ - APPLI_region_ROM_start + 1;

APPLI_region_whole_ROM_length = __ICFEDIT_region_SLOT_0_end__ - __ICFEDIT_intvec_start__ + 1;

APPLI_region_RAM_start    = __ICFEDIT_SE_region_SRAM1_end__ + 1;
APPLI_region_RAM_length    = EndOfRam - APPLI_region_RAM_start;

/* Specify the memory areas */
MEMORY
{
 ISR_VECTOR (rx)   : ORIGIN = __ICFEDIT_intvec_start__, LENGTH = VECTOR_SIZE
 APPLI_region_ROM  : ORIGIN = APPLI_region_ROM_start, LENGTH = APPLI_region_ROM_length
 APPLI_region_RAM  : ORIGIN = APPLI_region_RAM_start, LENGTH = APPLI_region_RAM_length
 FLASH_UC (r)    : ORIGIN = __Credential__, LENGTH = CredentialLen		/* Fixed-location area */
 RAM_NO_CACHE (xrw)             : ORIGIN = 0x2007C000, LENGTH = 16K
 RAM_NO_CACHE_DEVICE (xrw)      : ORIGIN = 0x2007C000, LENGTH = 512
 RAM_NO_CACHE_NORMAL (xrw)      : ORIGIN = 0x2007C200, LENGTH = 15872
}


/* Define output sections */
SECTIONS
{
  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    KEEP(*(.isr_vector)) /* Startup code */
 
    FILL(0);
    . = ORIGIN(ISR_VECTOR) + LENGTH(ISR_VECTOR) - 1;
    BYTE(0)
  } > ISR_VECTOR
  
  /* Uninited regions must be placed after inited regions, otherwise they are erased while programming. */
  UNINIT_FIXED_LOC (NOLOAD) :
  {
    *(UNINIT_FIXED_LOC)
  } >FLASH_UC

 .SE_IF_Code : {
  KEEP(*se_interface_app.o (.text .text*))
  } >SE_IF_region_ROM

   /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(8);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(8);
    _etext = .;        /* define a global symbols at end of code */
  } >APPLI_region_ROM

  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(8);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(8);
  } >APPLI_region_ROM

  .ARM.extab   : 
  { 
  . = ALIGN(8);
  *(.ARM.extab* .gnu.linkonce.armextab.*)
  . = ALIGN(8);
  } >APPLI_region_ROM
  .ARM : {
	. = ALIGN(8);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
	. = ALIGN(8);
  } >APPLI_region_ROM

  .preinit_array     :
  {
	. = ALIGN(8);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
	. = ALIGN(8);
  } >APPLI_region_ROM
  
  .init_array :
  {
	. = ALIGN(8);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
	. = ALIGN(8);
  } >APPLI_region_ROM
  .fini_array :
  {
	. = ALIGN(8);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
	. = ALIGN(8);
  } >APPLI_region_ROM

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : 
  {
    . = ALIGN(8);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(8);
    _edata = .;        /* define a global symbol at data end */
  } >APPLI_region_RAM AT>APPLI_region_ROM

   /* Extra ROM section (last one) to make sure the binary size is a multiple of the AES block size (16 bytes) */
  .align16 :
  {
    . = . + 1;         /* _edata=. is aligned on 8 bytes so could be aligned on 16 bytes: add 1 byte gap */
    . = ALIGN(16) - 1; /* increment the location counter until next 16 bytes aligned address (-1 byte)   */
    BYTE(0);           /* allocate 1 byte (value is 0) to be a multiple of 16 bytes                      */
  } > APPLI_region_ROM

  
  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >APPLI_region_RAM

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
	PROVIDE ( _user_heap_base = . );
    . = . + _Min_Heap_Size;
	PROVIDE ( _user_heap_end = . );
    PROVIDE ( _user_stack_base = . );
    . = . + _Min_Stack_Size;
	PROVIDE ( _user_stack_end = . );

    . = ALIGN(8);
  } >APPLI_region_RAM

  

  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
  .sram2_non_cached_device : { *(.sram2_non_cached_device) } >RAM_NO_CACHE_DEVICE AT> APPLI_region_ROM
  .sram2_non_cached_normal : { *(.sram2_non_cached_normal) } >RAM_NO_CACHE_NORMAL AT> APPLI_region_ROM
}


  
 