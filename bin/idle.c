#include <stdio.h>
#include <n7OS/process.h>
#include <n7OS/cpu.h>

void idle() {
    printf("Idle process\n");
    scheduler();
    while (1) {
        hlt();
    }
}