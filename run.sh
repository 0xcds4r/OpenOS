#!/bin/bash
set -e

OUT=build
KERNEL_ELF=$OUT/kernel.elf
ISO_IMG=os.iso

rm -rf $OUT
mkdir -p $OUT/boot/grub

echo "[*] assembling multiboot header + boot code"
nasm -f elf64 stage1/boot.asm -o $OUT/boot.o

echo "[*] compiling kernel"
gcc -m64 -ffreestanding -mcmodel=kernel -mno-red-zone -fno-stack-protector \
    -fno-pic -fno-pie \
    -c kernel/kernel.c -o "$OUT/kernel.o" -O2 -Wall -Wextra

echo "[*] linking kernel"
ld -n -o $KERNEL_ELF -T linker.ld \
    $OUT/boot.o $OUT/kernel.o \
    --oformat elf64-x86-64 \
    -z max-page-size=0x1000 \
    --build-id=none \
    -z noexecstack

echo "[*] copying for GRUB"
cp $KERNEL_ELF $OUT/boot/kernel.elf

cat > $OUT/boot/grub/grub.cfg <<EOF
set timeout=0
set default=0

menuentry "Open OS" {
    multiboot /boot/kernel.elf
    set gfxmode=1024x768x32
    boot
}
EOF

echo "[*] building ISO"
grub-mkrescue -o $ISO_IMG $OUT -d /usr/lib/grub/i386-pc

echo "[*] running QEMU"
qemu-system-x86_64 -cdrom $ISO_IMG -m 8192M -serial stdio \
    -vga std -display gtk,show-cursor=on \
    -usb -device usb-host,vendorid=0x1c4f,productid=0x0026