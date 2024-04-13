#include "n7OS/time.h"
#include "n7OS/snake.h"

int timer_ticks = 0;

void init_timer() {
    outb(0x34, 0x43);
    outb(FREQUENCE&0xFF, 0x40);
    outb(FREQUENCE>>8, 0x40);
    outb(inb(0x20) & 0xFE, 0x21);
    printf("Timer initialized\n");
}

void timer_IT_handler() {

    acquitter_IRQ();
    console_put_time(time_converter(timer_ticks++));
    

    if (timer_ticks % 1000 == 0) {
        console_put_time(time_converter(timer_ticks));
    }

    if (timer_ticks % 250 == 0) {
        update_game();
    }

}

time_t time_converter(int time) {
    time_t time_struct;
    // the time is in milliseconds
    time_struct.hours = (time / 3600000) % 100; // 100 hours max
    time_struct.minutes = (time % 3600000) / 60000;
    time_struct.seconds = (time % 60000) / 1000;
    return time_struct;
}

void masquer_IRQ(uint8_t num_IRQ) {
    outb(inb(0x21) | (1 << num_IRQ), 0x21);
}

void demasquer_IRQ(uint8_t num_IRQ) {
    outb(inb(0x21) & ~(1 << num_IRQ), 0x21);
}

void acquitter_IRQ() {
    outb(0x20, 0x20);
}

int get_internal_time() {
    return timer_ticks;
}