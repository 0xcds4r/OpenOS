# OpenOS

**OpenOS** is a minimalist x86_64 operating system with its own kernel, supporting long mode and a basic text terminal. The project aims to explore low-level programming, x86_64 architecture, and kernel development.

---

## Features

- Runs in 64-bit long mode.
- Simple terminal with basic command support.
- Built-in commands:  
  - `clear` - clear the screen  
  - `echo` - print text back to the terminal
- VGA scrolling and manual keyboard input.
- Minimal Multiboot 1.0 implementation.
- Fully freestanding kernel without standard library dependencies.

---

## Project Structure

OpenOS/

├─ kernel/ # Kernel source code

│ ├─ kernel.c # Main kernel code (terminal, commands)

│ └─ kernel.h # Constants, structures, kernel API

├─ stage1/ # Stage 1 bootloader (32-bit)

│ └─ boot.asm # Multiboot header and protected/long mode setup

├─ stage2/ # Stage 2 bootloader (long mode setup)

│ ├─ longmode.asm

│ └─ setup.asm

├─ run.sh # Build and run script (QEMU)

├─ linker.ld # Kernel linker script

└─ README.md

---

## Build and Run

### Requirements

- `nasm` — assembler
- `gcc` — x86_64 cross-compiler (`-m64`)
- `ld` — linker
- `grub-mkrescue` — to create a bootable ISO
- `qemu-system-x86_64` — emulator

### Build and Run

Run the `run.sh` script:

```bash
chmod +x run.sh
./run.sh