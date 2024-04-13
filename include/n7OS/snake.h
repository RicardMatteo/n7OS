#ifndef SNAKE_H
#define SNAKE_H

#include <n7OS/console.h>
#include <stddef.h>
#include <stdlib.h>
#include <n7OS/rand.h>

#define VOID ' '
#define WALL '#'
#define APPLE 'o'
#define SNAKE '*'

#define MAP_WIDTH 20
#define MAP_HEIGHT 20

#define CONSOLE_OFFSET VGA_HEIGHT - MAP_HEIGHT

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

#endif // SNAKE_H