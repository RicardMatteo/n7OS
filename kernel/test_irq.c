#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <stdio.h>

#define HANDLER_ENTRIES_CUSTOM 25

extern unsigned long handler_IT[HANDLER_ENTRIES_CUSTOM];
extern void handler_IT2();

void init_irq() {
   for (int i = 32; i < 56; i++) {
       init_irq_entry(i, (uint32_t) handler_IT[i-32]);
       //init_irq_entry(i, (uint32_t) &handler_IT2); // old
   }
}

void handler_en_C_old() {
    printf("Handler en C\n");    
}

void handler_en_C(unsigned irq_num) {
    printf("Handler en C %d\n", irq_num);
}