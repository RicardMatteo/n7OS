#ifndef _PROCESS_H
#define _PROCESS_H

#include <inttypes.h>
#include <n7OS/fifoqueue.h>

#define MAX_PROCESS 32
#define STACK_SIZE 1024
#define NUM_REGS 5

// Enumération des états d'un processus
enum process_state_t
{
    ELU,
    ACTIF,
    BLOQUE,
};

// Enumération des registres
enum reg_t
{
    EBX,
    ESP,
    EBP,
    ESI,
    EDI
};

// Définition des types
typedef enum process_state_t process_state_t;

typedef struct ressource_t
{
    int id;
    int value;
} ressource_t;

typedef uint32_t pid_t;

typedef void fnptr();

typedef struct process_t
{
    process_state_t state;
    pid_t pid;
    uint32_t stack[STACK_SIZE];
    uint32_t regs[5];
    const char *name;
    int argc;
    char **args;
    fnptr *f;
} process_t;

pid_t getpid();

void init_process();

pid_t creer(const char *name, fnptr f, int argc, char *argv[]);

void arreter();

void bloquer();

void debloquer(pid_t pid);

void scheduler();

void terminer();

void liberer_pid(pid_t pid);

pid_t allouer_pid();

void envelopper();

void callWithArgs(fnptr f, int argc, char *argv[]);

void addProcess(pid_t pid);

void process_test();

void print_process();

#endif