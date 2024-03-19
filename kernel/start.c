#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <stdio.h>

void kernel_start(void)
{
    init_console();
    setup_base(0 /* la memoire virtuelle n'est pas encore definie */);

    // test des interruptions
    init_irq();
    
    // lancement des interruptions
    sti();

    init_timer();
    // initialisation du gestionnaire de mémoire
    //init_kheap();

    // test de la console
    printf("Hello, kernel World!\n");


    // on genere des interruptions
    __asm__("int $40");
    __asm__("int $41");
    __asm__("int $42");
    __asm__("int $43");
    __asm__("int $43");
    __asm__("int $32");

    


    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
