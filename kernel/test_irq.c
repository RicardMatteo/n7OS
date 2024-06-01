#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <n7OS/keyboard.h>
#include <stdio.h>

#define HANDLER_ENTRIES_CUSTOM 25

extern unsigned long handler_IT[HANDLER_ENTRIES_CUSTOM];
extern void handler_IT2();
extern void handler_syscall();

void init_irq()
{
    for (int i = 32; i < 128; i++)
    {
        init_irq_entry(i, (uint32_t)handler_IT[i - 32]);
        // init_irq_entry(i, (uint32_t) &handler_IT2); // old
    }
}

void handler_en_C_old()
{
    printf("Handler en C\n");
}

void handler_en_C(unsigned irq_num)
{
    switch (irq_num)
    {
    // 0x20 est le numéro de l'IRQ du timer
    case 0x20:
        timer_IT_handler();
        break;
    // 0x21 est le numéro de l'IRQ du clavier
    case 0x21:
        keyboard_IT_handler();
        break;
    // 0x80 est le numéro de l'IRQ des appels systèmes
    case 0x80:
        handler_syscall();
        break;
    default:
        printf("Handler en C %d\n", irq_num);
        break;
    }
}