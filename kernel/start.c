#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <stdio.h>
#include <n7OS/snake.h>
#include <n7OS/process.h>
#include <n7OS/paging.h>
#include <n7OS/mem.h>
#include <n7OS/syscall_defs.h>
#include <n7OS/sys.h>
#include <n7OS/keyboard.h>

extern void idle();
extern void processus1();

void kernel_start(void)
{
    // initialisation de la console
    init_console();
    // printf("> console initialisée\n");

    // initialisation de la gestion de la memoire
    initialise_paging();
    // printf("> gestion de la mémoire initialisée\n");

    // initialisation des appels systeme
    init_syscall();
    printf("> syscalls initialisées\n");

    // test des interruptions
    init_irq();

    // lancement des interruptions
    sti();
    printf("> interruptions initialisées\n");

    // test de l'appel systeme example
    if (sys_example() == 1)
    {
        printf("Appel systeme example OK\n");
    }

    if (sys_write("Hello, kernel World!\n", 21) == 21)
    {
        printf("Appel systeme write ok \n");
    }

    // initialisation du timer
    init_timer();
    printf("> timer initialisé\n");

    // initialisation du clavier
    init_keyboard();
    printf("> clavier initialisé\n");

    // test de la console
    // printf("Hello, kernel World!\n");

    // test du snake
    // init_game();

    /*
    // test des interruptions
    // on genere des interruptions
    __asm__("int $40");
    __asm__("int $41");
    __asm__("int $42");
    __asm__("int $43");
    __asm__("int $43");
    __asm__("int $32");
    */

    /*


    if ( shutdown(1) == -1) {
        printf("Appel systeme shutdown ok \n " );
    }
    */

    init_process();
    printf("> gestion des processus initialisée\n");

    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        // cette fonction arrete le processeur
        hlt();
    }
}
