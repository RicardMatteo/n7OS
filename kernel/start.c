#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <stdio.h>
#include <n7OS/snake.h>

void kernel_start(void)
{
    // initialisation de la console
    init_console();

    // initialisation de la gestion de la memoire
    setup_base(0 /* la memoire virtuelle n'est pas encore definie */);

    // initialisation des appels systeme
    init_syscall();

    // test des interruptions
    init_irq();
    
    // lancement des interruptions
    sti();

    // test de l'appel systeme example
    if ( example() == 1) {
        printf("Appel systeme example OK\n" );
    }

    if (write("Hello, kernel World!\n", 21) == 21) {
        printf("Appel systeme write ok \n" );
    }



    init_timer();
    init_keyboard();
    // initialisation du gestionnaire de mémoire
    //init_kheap();

    // test de la console
    printf("Hello, kernel World!\n");

    // test du snake
    init_game();

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
    // wait 10 seconds
    for (int i = 0; i < 1000000000; i++) {
        // do nothing
    }

    if ( shutdown(1) == -1) {
        printf("Appel systeme shutdown ok \n " );
    }
    */


    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
