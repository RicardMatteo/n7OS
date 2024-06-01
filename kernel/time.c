#include "n7OS/console.h"
#include "n7OS/time.h"

int timer_ticks = 0;

// Tableau des prochaines fin de sleep
time_process_t sleep_table[MAX_PROCESS];

void init_timer()
{
    outb(0x34, 0x43);
    outb(FREQUENCE & 0xFF, 0x40);
    outb(FREQUENCE >> 8, 0x40);
    outb(inb(0x20) & 0xFE, 0x21);
    printf("Timer initialized\n");

    // On initialise le tableau des sleep
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        sleep_table[i].pid = -1;
        sleep_table[i].time = -1;
    }

    timer_ticks = 0;
    console_put_time(time_converter(timer_ticks));
}

void sleep(int time)
{
    // On récupère le pid du processus actif
    pid_t pid = getpid();
    // On récupère le temps actuel
    int current_time = get_internal_time();
    // On ajoute le temps de sleep au temps actuel
    int end_time = current_time + time * 1000;
    // On ajoute le pid et le temps de fin de sleep dans le tableau
    // tout en le gardant trié par ordre croissant de temps
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (sleep_table[i].time == -1)
        {
            sleep_table[i].pid = pid;
            sleep_table[i].time = end_time;
            break;
        }
        if (sleep_table[i].time > end_time)
        {
            for (int j = MAX_PROCESS - 1; j > i; j--)
            {
                sleep_table[j] = sleep_table[j - 1];
            }
            sleep_table[i].pid = pid;
            sleep_table[i].time = end_time;
            break;
        }
    }
    bloquer();
}

void timer_IT_handler()
{

    acquitter_IRQ();
    timer_ticks++;

    if (timer_ticks % TICK_TIME_UPDATE == 0)
    {
        console_put_time(time_converter(timer_ticks));
    }

    if (timer_ticks % TICK_GAME_UPDATE == 0)
    {
        update_game();
    }

    if (timer_ticks % TICK_SCHEDULER_UPDATE == 0)
    {
        scheduler();
    }

    // On vérifie si un processus doit se réveiller
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (sleep_table[i].time != -1 && sleep_table[i].time <= timer_ticks)
        {
            // On débloque le processus
            debloquer(sleep_table[i].pid);
            // On enlève le processus du tableau
            sleep_table[i].pid = -1;
            sleep_table[i].time = -1;

            // On décale les processus suivants
            for (int j = i; j < MAX_PROCESS - 1; j++)
            {
                sleep_table[j] = sleep_table[j + 1];
            }
        }
    }
}

time_t time_converter(int time)
{
    time_t time_struct;
    // the time is in milliseconds
    time_struct.hours = (time / 3600000) % 100; // 100 hours max
    time_struct.minutes = (time % 3600000) / 60000;
    time_struct.seconds = (time % 60000) / 1000;
    return time_struct;
}

void masquer_IRQ(uint8_t num_IRQ)
{
    outb(inb(0x21) | (1 << num_IRQ), 0x21);
}

void demasquer_IRQ(uint8_t num_IRQ)
{
    outb(inb(0x21) & ~(1 << num_IRQ), 0x21);
}

void acquitter_IRQ()
{
    outb(0x20, 0x20);
}

int get_internal_time()
{
    return timer_ticks;
}