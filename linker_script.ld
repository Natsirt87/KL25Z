/* Linker script for the MKL25Z128 microcontroller */

MEMORY
{
    /* Define the memory regions */
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 128K
    RAM (rwx) : ORIGIN = 0x1FFFF000, LENGTH = 16K
}

/* Define the entry point */
ENTRY(Reset_Handler)

/* Define the stack size */
_stack_size = 1024;

SECTIONS
{
    /* Define the section for the initial stack pointer */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* Interrupt vector table */
        . = ALIGN(4);
    } > FLASH

    /* Define the section for code (text) */
    .text :
    {
        . = ALIGN(4);
        *(.text) /* Code */
        *(.text*) /* Additional text sections */
        *(.rodata) /* Read-only data */
        *(.rodata*) /* Additional read-only data sections */
        . = ALIGN(4);
    } > FLASH

    /* Define the section for read-only data (const) */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata) /* Read-only data */
        *(.rodata*) /* Additional read-only data sections */
        . = ALIGN(4);
    } > FLASH

    /* Define the section for the C runtime initialization code (preinit_array, init_array, fini_array) */
    .preinit_array :
    {
        PROVIDE_HIDDEN(__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN(__preinit_array_end = .);
    } > FLASH

    .init_array :
    {
        PROVIDE_HIDDEN(__init_array_start = .);
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        PROVIDE_HIDDEN(__init_array_end = .);
    } > FLASH

    .fini_array :
    {
        PROVIDE_HIDDEN(__fini_array_start = .);
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN(__fini_array_end = .);
    } > FLASH

    /* Define the section for data that should be copied to RAM on startup */
    .data : AT (__etext)
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data) /* Initialized data */
        *(.data*) /* Additional data sections */
        . = ALIGN(4);
        __data_end__ = .;
    } > RAM

    /* Define the section for zero-initialized data (BSS) */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss) /* Uninitialized data */
        *(.bss*) /* Additional bss sections */
        *(COMMON) /* Common symbols */
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    /* Define the section for the stack */
    .stack (NOLOAD) :
    {
        . = ALIGN(4);
        _estack = ORIGIN(RAM) + LENGTH(RAM);
        . = _estack - _stack_size;
        . = ALIGN(4);
    } > RAM

    /* Define end symbols */
    _edata = __data_end__;
    _sbss = __bss_start__;
    _ebss = __bss_end__;
    _sdata = __data_start__;
    _sidata = __etext;
}

/* Provide the end of the text section */
__etext = .;
