#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <stdio.h>

extern void handler_IT();


void init_irq() {
   for (int i = 32; i < 256; i++) {
       init_irq_entry(i, (uint32_t) &handler_IT);
   }
}

void handler_en_C() {
    printf("Handler en C\n");    
}
