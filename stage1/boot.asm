section .multiboot
align 4
multiboot_header:
    dd 0x1BADB002
    dd 0x00000000
    dd -(0x1BADB002 + 0x00000000)

section .text
global _start
extern kernel_main

BITS 32
_start:
    cli
    cmp eax, 0x2BADB002
    jne .error

    push ebx
    push eax

    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, pdpt
    or eax, 3
    mov [pml4], eax

    mov eax, pd
    or eax, 3
    mov [pdpt], eax

    mov dword [pd], 0x00000083

    mov eax, pml4
    mov cr3, eax

    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    lgdt [gdt64_pointer]
    jmp 0x08:long_mode

.error:
    mov byte [0xB8000], 'E'
    mov byte [0xB8001], 0x4F
    hlt
    jmp .error

BITS 64
long_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov rsp, 0x90000

    pop rdi
    pop rsi

    call kernel_main

    cli
.hang: hlt
    jmp .hang

align 8
gdt64:
    dq 0
    dq 0x00AF9A000000FFFF
    dq 0x00AF92000000FFFF

gdt64_pointer:
    dw gdt64_pointer - gdt64 - 1
    dq gdt64

section .bss
align 4096
pml4: resq 512
pdpt: resq 512
pd:   resq 512