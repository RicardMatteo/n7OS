#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/snake.h>

#define FREQUENCE 1190

enum Focus {
    CONSOLE,
    GAME
};

enum Focus focus = GAME;

unsigned char keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',     /* 9 */
  '9', '0', '-', '=', '\b',     /* Backspace */
  '\t',                 /* Tab */
  'a', 'z', 'e', 'r',   /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,                  /* 29   - Control */
  'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm',     /* 39 */
 '\'', '`',   0,                /* Left shift */
 '\\', 'w', 'x', 'c', 'v', 'b', 'n',                    /* 49 */
  ',', ';', ':', '!',   0,                              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};



void init_keyboard() {
    //outb(0x20, 0x21);
    outb(inb(0x21) & 0xFD, 0x21);

    //    outb(inb(0x21) & ~(1 << 0x21), 0x21);

    //outb(inb(0x21) & 0xFE, 0x21);
    printf("Keyboard initialized\n");
}

void keyboard_IT_handler() {
    char scancode = inb(0x60);
    if (scancode > 0) {
        //printf("Scancode: %d\n", scancode);
        if (focus == CONSOLE) {
          printf("%c", keyboard_map[scancode]);
        }
        if (focus == GAME) {
            switch (scancode) {
                case 0x48:
                    set_direction(UP);
                    break;
                case 0x50:
                    set_direction(DOWN);
                    break;
                case 0x4B:
                    set_direction(LEFT);
                    break;
                case 0x4D:
                    set_direction(RIGHT);
                    break;
                default:
                    break;
            }
        }
    }
    outb(0x20, 0x20);

}

void acquitter_IRQ_keyboard() {
    outb(0x20, 0x21);
}

void set_focus(enum Focus new_focus) {
    focus = new_focus;
}