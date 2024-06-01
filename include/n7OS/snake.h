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

#define X_RATIO 2
#define Y_RATIO 1

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum GameState
{
    INIT,
    PLAYING,
    GAME_OVER
};

typedef struct snake_cell_t
{
    struct snake_cell_t *next;
    struct snake_cell_t *prev;
    int x;
    int y;
} snake_cell_t;

// typedef struct snake_cell_t snake_cell_t;

typedef struct snake_t
{
    snake_cell_t *head;
    snake_cell_t *tail;
} snake_t;

// typedef struct snake_t snake_t;*

void move_snake(snake_t *snake);

void init_map();

void spawn_apple();

void draw_map();

void init_game();

void update_game();

void set_direction(enum Direction dir);

#endif // SNAKE_H