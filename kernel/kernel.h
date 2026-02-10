#pragma once

#include <stdint.h>

#define OS_NAME      "OpenOS"
#define OS_VERSION   "v0.1"

#define VGA_WIDTH    80
#define VGA_HEIGHT   25
#define VGA_BUFFER   ((volatile uint16_t*)0xB8000)

#define COLOR_BLACK         0
#define COLOR_BLUE          1
#define COLOR_GREEN         2
#define COLOR_CYAN          3
#define COLOR_RED           4
#define COLOR_MAGENTA       5
#define COLOR_BROWN         6
#define COLOR_LIGHT_GRAY    7
#define COLOR_DARK_GRAY     8
#define COLOR_LIGHT_BLUE    9
#define COLOR_LIGHT_GREEN   10
#define COLOR_LIGHT_CYAN    11
#define COLOR_LIGHT_RED     12
#define COLOR_LIGHT_MAGENTA 13
#define COLOR_YELLOW        14
#define COLOR_WHITE         15

#define MAKE_COLOR(fg, bg) ((bg) << 4 | (fg))
#define COLOR_BLACK_ON_WHITE     MAKE_COLOR(COLOR_BLACK, COLOR_WHITE)
#define COLOR_WHITE_ON_BLACK     MAKE_COLOR(COLOR_WHITE, COLOR_BLACK)
#define COLOR_RED_ON_BLACK       MAKE_COLOR(COLOR_RED, COLOR_BLACK)
#define COLOR_GREEN_ON_BLACK     MAKE_COLOR(COLOR_GREEN, COLOR_BLACK)
#define COLOR_YELLOW_ON_BLACK    MAKE_COLOR(COLOR_YELLOW, COLOR_BLACK)
#define COLOR_BLUE_ON_BLACK      MAKE_COLOR(COLOR_BLUE, COLOR_BLACK)
#define COLOR_MAGENTA_ON_BLACK   MAKE_COLOR(COLOR_MAGENTA, COLOR_BLACK)
#define COLOR_CYAN_ON_BLACK      MAKE_COLOR(COLOR_CYAN, COLOR_BLACK)
#define COLOR_LIGHT_GRAY_ON_BLACK MAKE_COLOR(COLOR_LIGHT_GRAY, COLOR_BLACK)

#define MAX_COMMANDS 512
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
