#include "n7OS/snake.h"

char map[MAP_WIDTH * MAP_HEIGHT];

int snake_length = 1;

int snake_x = 5;
int snake_y = 7;

int apple_x = 0;
int apple_y = 0;

enum Direction direction = RIGHT;

enum GameState game_state = INIT;

snake_t *snake;

void init_snake()
{
    snake = (snake_t *)malloc(sizeof(snake_t));
    snake_cell_t *cell = (snake_cell_t *)malloc(sizeof(snake_cell_t));
    cell->x = snake_x;
    cell->y = snake_y;
    cell->next = NULL;
    cell->prev = NULL;
    snake->head = cell;
    snake->tail = cell;
}

void move_snake(snake_t *snake)
{
    /* create a cell at the next posistion */
    snake_cell_t *cell = (snake_cell_t *)malloc(sizeof(snake_cell_t));
    switch (direction)
    {
    case UP:
        cell->x = snake->head->x;
        cell->y = snake->head->y - 1;
        break;
    case DOWN:
        cell->x = snake->head->x;
        cell->y = snake->head->y + 1;
        break;
    case LEFT:
        cell->x = snake->head->x - 1;
        cell->y = snake->head->y;
        break;
    case RIGHT:
        cell->x = snake->head->x + 1;
        cell->y = snake->head->y;
        break;
    }
    cell->next = snake->head;
    cell->prev = NULL;
    snake->head->prev = cell;
    snake->head = cell;

    /* check if we hit a wall or ourself */
    if (map[cell->x + cell->y * MAP_WIDTH] == WALL || map[cell->x + cell->y * MAP_WIDTH] == SNAKE)
    {
        game_state = GAME_OVER;
        set_focus(CONSOLE);
        // set the cursor to the bottom of the game map
        console_put_cursor((MAP_HEIGHT + CONSOLE_OFFSET) * VGA_WIDTH + 2);
        printf("GGame Over, your score is: %d\n>", snake_length);
        return;
    }

    /* and remove the last cell if we didn't eat */
    if (map[cell->x + cell->y * MAP_WIDTH] != APPLE)
    {

        /* update map */
        map[snake->tail->x + snake->tail->y * MAP_WIDTH] = VOID;
        /* draw only the diff */
        console_put_arbitrary(Y_RATIO * (snake->tail->y + CONSOLE_OFFSET) * VGA_WIDTH + X_RATIO * snake->tail->x, VOID);

        /* update tail */
        snake_cell_t *last = snake->tail;
        snake->tail = last->prev;
        snake->tail->next = NULL;

        // free(last);
    }
    else
    {
        snake_length++;
        console_put_score(snake_length);
        // printf("Score: %d\n", snake_length);
        spawn_apple();
    }

    /* update map */
    map[cell->x + cell->y * MAP_WIDTH] = SNAKE;
    /* draw only the diff */
    console_put_arbitrary(Y_RATIO * (cell->y + CONSOLE_OFFSET) * VGA_WIDTH + X_RATIO * cell->x, SNAKE);
}

void init_map()
{
    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
    {
        map[i] = VOID;
    }
    /* create walls */
    for (int i = 0; i < MAP_WIDTH; i++)
    {
        map[i] = WALL;
        map[i + (MAP_HEIGHT - 1) * MAP_WIDTH] = WALL;
    }
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        map[i * MAP_WIDTH] = WALL;
        map[(MAP_WIDTH - 1) + i * MAP_WIDTH] = WALL;
    }

    /* draw snake */
    map[snake_x + snake_y * MAP_WIDTH] = SNAKE;

    /* create apple */
    spawn_apple();
}

void spawn_apple()
{
    do
    {
        apple_x = rand() % MAP_WIDTH;
        apple_y = rand() % MAP_HEIGHT;
    } while (map[apple_x + apple_y * MAP_WIDTH] != VOID);
    map[apple_x + apple_y * MAP_WIDTH] = APPLE;
    console_put_arbitrary(Y_RATIO * (apple_y + CONSOLE_OFFSET) * VGA_WIDTH + X_RATIO * apple_x, APPLE);
}

void draw_map()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            console_put_arbitrary(Y_RATIO * (i + CONSOLE_OFFSET) * VGA_WIDTH + X_RATIO * j, map[j + i * MAP_WIDTH]);
        }
    }
}

void init_game()
{
    console_clear();
    set_focus(GAME);
    init_snake();
    init_map();
    game_state = PLAYING;
    draw_map();
    // set_focus(GAME);
}

void update_game()
{
    if (game_state == PLAYING)
    {
        move_snake(snake);
        // draw_map();
    }
}

void set_direction(enum Direction dir)
{
    if (dir == UP && direction != DOWN)
    {
        direction = dir;
    }
    else if (dir == DOWN && direction != UP)
    {
        direction = dir;
    }
    else if (dir == LEFT && direction != RIGHT)
    {
        direction = dir;
    }
    else if (dir == RIGHT && direction != LEFT)
    {
        direction = dir;
    }
}
