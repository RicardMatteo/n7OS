#ifndef __SYSCALL_DEFS_H__
#define __SYSCALL_DEFS_H__

#include <debug.h>
#include <string.h>

#define NB_SYSCALL 10

int sys_example();
// int sys_shutdown(int n);
int sys_shutdown();
int sys_write(const char *s, int len);
int sys_snake();
int sys_clear();
int sys_print_memory();
int sys_help();
int sys_process_test();
int sys_ps();

typedef int (*fn_ptr)();
extern fn_ptr syscall_table[NB_SYSCALL];

void add_syscall(int num, fn_ptr function, char *name);

int strcmpk(const char *s1, const char *s2, int n);

void print_syscall_table();

int syscall_id(char *name, int len);

fn_ptr syscall_function(char *name, int len);

int syscall(char *name, int len);

#endif
