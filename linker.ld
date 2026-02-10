ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .multiboot : {
        KEEP(*(.multiboot))
    }

    .text ALIGN(0x1000) : {
        *(.text .text.*)
        . = ALIGN(0x1000);
    }

    .rodata ALIGN(0x1000) : {
        *(.rodata .rodata.*)
        . = ALIGN(0x1000);
    }

    .data ALIGN(0x1000) : {
        *(.data .data.*)
        . = ALIGN(0x1000);
    }

    .bss ALIGN(0x1000) : {
        *(COMMON)
        *(.bss .bss.*)
        . = ALIGN(0x1000);
    }

    /DISCARD/ : {
        *(.comment*)
        *(.eh_frame*)
        *(.eh_frame_hdr)
        *(.note*)
        *(.rela*)
        *(.rel*)
        *(.gcc_except_table*)
        *(.got)
        *(.got.plt)
        *(.iplt)
        *(.igot.plt)
        *(.plt)
        *(.plt.got)
    }
}