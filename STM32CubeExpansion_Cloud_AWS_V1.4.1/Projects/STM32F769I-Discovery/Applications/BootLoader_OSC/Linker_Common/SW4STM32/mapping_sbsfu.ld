/*
*****************************************************************************
**

**  File        : LinkerScript.ld
**
**  Abstract    : Linker script for STM32F769NIHx Device with
**                2048KByte FLASH, 512KByte RAM
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

/* SE Code region protected by protected area */
VECTOR_SIZE = 0x400;
__ICFEDIT_SE_Code_region_ROM_start__       = 0x08000000 + VECTOR_SIZE;
__ICFEDIT_SE_CallGate_region_ROM_start__   = __ICFEDIT_SE_Code_region_ROM_start__ + 0x4;
__ICFEDIT_SE_CallGate_region_ROM_end__     = __ICFEDIT_SE_Code_region_ROM_start__ + 0xFF;

/* SE key region protected by protected area */
__ICFEDIT_SE_Key_region_ROM_start__        = __ICFEDIT_SE_CallGate_region_ROM_end__ + 0x1;
__ICFEDIT_SE_Key_region_ROM_end__          = __ICFEDIT_SE_Key_region_ROM_start__ + 0xFF;

/* SE Startup: call before enabling protected area*/
__ICFEDIT_SE_Startup_region_ROM_start__    = __ICFEDIT_SE_Key_region_ROM_end__ + 0x1;
__ICFEDIT_SE_Code_nokey_region_ROM_start__ = __ICFEDIT_SE_Startup_region_ROM_start__ + 0x100;
/* Aligned SE End at the end of the 1st 32Kbytes of flash, MPU protection isolation constraints */
__ICFEDIT_SE_Code_region_ROM_end__         =  0x0800FFFF;

/* SE IF ROM: used to locate Secure Engine interface code out of protected area */
__ICFEDIT_SE_IF_region_ROM_start__         = __ICFEDIT_SE_Code_region_ROM_end__ + 1;
__ICFEDIT_SE_IF_region_ROM_end__           = __ICFEDIT_SE_IF_region_ROM_start__ + 0x8FF;

/* SBSFU Code region */
__ICFEDIT_SB_region_ROM_start__            = __ICFEDIT_SE_IF_region_ROM_end__ + 0x1;
/* Aligned SBSFU  end at the end of the 1st 128Kbytes of FLASH, MPU protection isolation constraints */
__ICFEDIT_SB_region_ROM_end__              = 0x0801FFFF;

SE_Entry_Secure_ROM_Region_Length = __ICFEDIT_SE_CallGate_region_ROM_end__ - __ICFEDIT_SE_CallGate_region_ROM_start__ + 1;
SE_Key_region_ROM_Length          = __ICFEDIT_SE_Key_region_ROM_end__ - __ICFEDIT_SE_Key_region_ROM_start__ + 1;
SE_Startup_region_ROM_Length      = __ICFEDIT_SE_Code_nokey_region_ROM_start__ - __ICFEDIT_SE_Startup_region_ROM_start__ ;
SE_ROM_region_Length              = __ICFEDIT_SE_Code_region_ROM_end__ - __ICFEDIT_SE_Code_nokey_region_ROM_start__ + 1;
SE_IF_region_ROM_Length           = __ICFEDIT_SE_IF_region_ROM_end__ - __ICFEDIT_SE_IF_region_ROM_start__ + 1;
SB_ROM_region_Length              = __ICFEDIT_SB_region_ROM_end__ - __ICFEDIT_SB_region_ROM_start__ + 1;

/* RAM section */
/* SE RAM1 region protected by protected area */
/* SE stack is placed 1st in RAM, stack overflow does not write on other RAM area */
__ICFEDIT_SE_region_SRAM1_start__     = 0x20020000;
__ICFEDIT_SE_region_SRAM1_stack_top__ = 0x20020A00;
__ICFEDIT_SE_region_SRAM1_end__       = 0x200227FF;

/* SBSFU RAM1 region */
__ICFEDIT_SB_region_SRAM1_start__     = __ICFEDIT_SE_region_SRAM1_end__ + 0x1;
__ICFEDIT_SB_region_SRAM1_end__       = 0x2007BFFF;

SE_SRAM1_region_Length = __ICFEDIT_SE_region_SRAM1_end__ - __ICFEDIT_SE_region_SRAM1_stack_top__ + 1;
SB_SRAM1_region_Length = __ICFEDIT_SB_region_SRAM1_end__ - __ICFEDIT_SB_region_SRAM1_start__ + 1;

MEMORY
{
 SE_Entry_Secure_ROM_Region (rx)     : ORIGIN = __ICFEDIT_SE_CallGate_region_ROM_start__, LENGTH = SE_Entry_Secure_ROM_Region_Length
 SE_Key_region_ROM (rx)              : ORIGIN = __ICFEDIT_SE_Key_region_ROM_start__, LENGTH = SE_Key_region_ROM_Length
 SE_Startup_region_ROM (rx)          : ORIGIN = __ICFEDIT_SE_Startup_region_ROM_start__, LENGTH = SE_Startup_region_ROM_Length
 SE_ROM_region (rx)                  : ORIGIN = __ICFEDIT_SE_Code_nokey_region_ROM_start__, LENGTH = SE_ROM_region_Length
 SE_IF_region_ROM (rx)               : ORIGIN = __ICFEDIT_SE_IF_region_ROM_start__, LENGTH = SE_IF_region_ROM_Length
 SB_ROM_region (rx)                  : ORIGIN = __ICFEDIT_SB_region_ROM_start__, LENGTH = SB_ROM_region_Length
 SE_SRAM1_region (xrw)               : ORIGIN = __ICFEDIT_SE_region_SRAM1_stack_top__, LENGTH = SE_SRAM1_region_Length
 SB_SRAM1_region (xrw)               : ORIGIN = __ICFEDIT_SB_region_SRAM1_start__, LENGTH = SB_SRAM1_region_Length
}

