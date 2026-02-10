#pragma once

#include <stdint.h>

#define OS_NAME      "OpenOS"
#define OS_VERSION   "v0.1"

#define VGA_WIDTH    80
#define VGA_HEIGHT   25
#define VGA_BUFFER   ((volatile uint16_t*)0xB8000)

#define COLOR_WHITE_ON_BLACK 0x0F
#define COLOR_GREEN_ON_BLACK 0x0A
#define COLOR_RED_ON_BLACK   0x0C

#define MAX_COMMANDS 16
#define INPUT_BUFFER_SIZE 128

typedef void (*CommandCallback)(void);

typedef struct {
    const char* name;
    CommandCallback callback;
} Command;

struct KernelAPI {
    void (*print)(const char*, uint8_t);
    void (*register_command)(const char*, void(*)(void));
};

void kernel_main(uint64_t magic, uint64_t mb_addr);
