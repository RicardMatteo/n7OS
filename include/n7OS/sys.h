#ifndef __SYS_H__
#define __SYS_H__

#include <n7OS/process.h>
#include <n7OS/syscall_defs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <unistd.h>
#include <stddef.h>
#include "n7OS/mem.h"

void init_syscall();

// code de la fonction de traitement de l'appel systeme example
int sys_example();

// int sys_shutdown(int n);
int sys_shutdown();

int sys_write(const char *s, int len);

int sys_snake();

int sys_clear();

pid_t sys_fork(const char *name, fnptr f, int argc, char *argv[]);

int sys_print_memory();

int sys_help();

int sys_process_test();

int sys_ps();

#endif
