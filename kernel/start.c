#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>

void kernel_start(void)
{
    init_console();
    setup_base(0 /* la memoire virtuelle n'est pas encore definie */);

    // lancement des interruptions
    sti();

    // test de la console
    printf("Hello, kernel World!\n");
    for (int i = 0; i < 100; i++) {
        printf("Hello, kernel World! %d\n", i);
    }

    // test des interruptions
    init_irq();
    

    // on genere une interruption
    __asm__("int $0x50");
    __asm__("int $0x51");
    __asm__("int $0x52");
    __asm__("int $0x53");

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
