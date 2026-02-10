BITS 64
extern kernel_main

BITS 16
start:
    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE32:pmode

BITS 32
pmode:
    mov ax, DATA32
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, pml4
    mov cr3, eax

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    jmp CODE64:lmode

BITS 64
lmode:
    mov rsp, 0x90000
    mov rbp, rsp

    call kernel_main

halt:
    hlt
    jmp halt

ALIGN 8
gdt:
    dq 0
    dq 0x00AF9A000000FFFF
    dq 0x00AF92000000FFFF
    dq 0x00209A0000000000
    dq 0x0000920000000000

gdt_desc:
    dw gdt_end - gdt - 1
    dq gdt
gdt_end:

CODE32 equ 0x08
DATA32 equ 0x10
CODE64 equ 0x18
DATA64 equ 0x20

ALIGN 4096
pml4:
    dq pdpt + 3
    times 511 dq 0

ALIGN 4096
pdpt:
    dq pd + 3
    times 511 dq 0

ALIGN 4096
pd:
    dq 0x00000083
    times 511 dq 0
