#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "n7OS/time_t.h"

#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <string.h>
#include <n7OS/syscall_defs.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define SCREEN_ADDR 0xB8000

#define PORT_CMD 0x3D4
#define PORT_DATA 0x3D5

#define CMD_HIGH 0xE
#define CMD_LOW 0xF

#define BLACK 0x0
#define BLUE 0x1
#define GREEN 0x2
#define CYAN 0x3
#define RED 0x4
#define PURPLE 0x5
#define BROWN 0x6
#define GRAY 0x7
#define D_GRAY 0x8
#define L_BLUE 0x9
#define L_GREEN 0xA
#define L_CYAN 0xB
#define L_RED 0xC
#define L_PURPLE 0xD
#define YELLOW 0xE
#define WHITE 0xF

// Character color: blink|back|text
#define BLINK 0 << 7

#define BACK BLACK << 4
#define H_BACK YELLOW << 4

#define TEXT WHITE

#define CHAR_COLOR BLINK | BACK | TEXT
#define HEADER_COLOR BLINK | BACK | L_CYAN

void init_console();

/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */
void console_putbytes(const char *s, int len);

/*
 * This function is called by the kernel to clear the screen.
 */
void console_clear();

/*
 * This function is called by the kernel to move the cursor to the given position.
 * It calls outb to use the GPU.
 */
void console_cursor(int pos);

void console_put_cursor(int pos);

void console_put_score(int score);

void console_full_clear();

void console_put_arbitrary(int pos, char c);

struct time_t;

void console_put_time(time_t time);

void exec_line();

void print_help();

#endif
