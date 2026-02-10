#include "kernel.h"

static int cursor_x, cursor_y;
static char input_buffer[INPUT_BUFFER_SIZE];
static int input_pos;

static const char scancode_to_ascii[128] = {
    0, 0, '1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',0,
};

static volatile uint16_t* vga = VGA_BUFFER;

static Command commands[MAX_COMMANDS];
static int command_count;

static struct KernelAPI kernel_api;

static inline uint8_t inb(uint16_t port) 
{
    uint8_t val;
    __asm__ volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline void outb(uint16_t port, uint8_t val) 
{
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static void scroll(void) 
{
    for (int y = 1; y < VGA_HEIGHT; y++) 
    {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga[(y-1) * VGA_WIDTH + x] = vga[y * VGA_WIDTH + x];
        }
    }

    for (int x = 0; x < VGA_WIDTH; x++) {
        vga[(VGA_HEIGHT-1) * VGA_WIDTH + x] = ' ' | (COLOR_WHITE_ON_BLACK << 8);
    }

    cursor_y = VGA_HEIGHT - 1;
    cursor_x = 0;
}

static void put_char(char c, uint8_t color) 
{
    if (c == '\n') 
    { 
        cursor_x = 0; 
        cursor_y++; 
    }

    else if (c == '\b') 
    {
        if (cursor_x) 
        { 
            cursor_x--; 
            vga[cursor_y * VGA_WIDTH + cursor_x] = ' ' | (color<<8); 
        }
    }
    else 
    {
        vga[cursor_y * VGA_WIDTH + cursor_x++] = c | (color<<8);
    }

    if (cursor_x >= VGA_WIDTH) 
    { 
        cursor_x = 0; 
        cursor_y++; 
    }

    if (cursor_y >= VGA_HEIGHT) {
        scroll();
    }
}

static void print(const char* s, uint8_t color) {
    while (*s) put_char(*s++, color);
}

static void clear_screen(void) 
{
    for (int i = 0; i<VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i]=' '|(COLOR_WHITE_ON_BLACK<<8);
    }

    cursor_x = cursor_y = 0;
}

static void register_command(const char* name, CommandCallback cb) 
{
    if (command_count < MAX_COMMANDS) {
        commands[command_count++] = (Command){name, cb};
    }
}

static void handle_key(uint8_t scancode) 
{
    if (scancode & 0x80) return;

    char c = scancode_to_ascii[scancode];
    if (c == 0) return;

    if (c == '\b') 
    {
        if (input_pos > 0) 
        {
            input_pos--;
            put_char('\b', COLOR_WHITE_ON_BLACK);
        }
    } 
    else if (c == '\n') 
    {
        input_buffer[input_pos] = '\0';
        put_char('\n', COLOR_WHITE_ON_BLACK);

        int found = 0;

        for (int i = 0; i < command_count; i++) 
        {
            const char* cmd = commands[i].name;
            int len = 0;

            while (cmd[len] && input_buffer[len] == cmd[len]) len++;

            if (cmd[len] == '\0' && (input_buffer[len] == '\0' || input_buffer[len] == ' ')) 
            {
                commands[i].callback();
                found = 1;
                break;
            }
        }

        if (!found) {
            print("Unknown command\n", COLOR_RED_ON_BLACK);
        }

        input_pos = 0;
        print("> ", COLOR_WHITE_ON_BLACK);
    } 
    else 
    {
        if (input_pos < INPUT_BUFFER_SIZE - 1) 
        {
            input_buffer[input_pos++] = c;
            put_char(c, COLOR_WHITE_ON_BLACK);
        }
    }
}

static void poll_keyboard(void) 
{
    if (inb(0x64) & 0x01) 
    {
        uint8_t scancode = inb(0x60);
        handle_key(scancode);
    }
}

static void disable_hw_cursor(void) {
    outb(0x3D4,0x0A); outb(0x3D5,0x20);
}

void initialize_api() 
{
    kernel_api.print = print;
    kernel_api.register_command = register_command;
}

static void cmd_clear(void) { clear_screen(); }

static void cmd_echo(void)
{
    if (input_pos > 5) { 
        print(input_buffer + 5, COLOR_WHITE_ON_BLACK);
    }

    put_char('\n', COLOR_WHITE_ON_BLACK);
}

void initialize_cmd() 
{
    register_command("clear", cmd_clear);
    register_command("echo", cmd_echo);
}

void kernel_main(uint64_t magic, uint64_t mb_addr) 
{
    (void)magic; (void)mb_addr;

    disable_hw_cursor();
    clear_screen();

    initialize_cmd();
    initialize_api();

    print("OpenOS v0.1\nLong mode active\nBasic terminal ready\nCommands: clear, echo\n\n", COLOR_GREEN_ON_BLACK);
    print("> ", COLOR_WHITE_ON_BLACK);

    while(1) poll_keyboard();
}
