#ifndef __TIME_H__
#define __TIME_H__

#include <inttypes.h>
#include <n7OS/cpu.h>
#include "n7OS/snake.h"
#include "n7OS/process.h"

#define FREQUENCE 1190

#define TICK_TIME_UPDATE 1000

#define TICK_GAME_UPDATE 250

#define TICK_SCHEDULER_UPDATE 100

typedef struct {
    int hours;
    int minutes;
    int seconds;
} time_t;

void init_timer();

void handler_IT_timer();

time_t time_converter(int time);

void masquer_IRQ(uint8_t num_IRQ);

void demasquer_IRQ(uint8_t num_IRQ);

void acquitter_IRQ();

int get_internal_time();

#endif