#include <n7OS/process.h>
#include <n7OS/cpu.h>

struct process_t process_table[MAX_PROCESS];

extern void envelopper_asm(fnptr f);

extern void ctx_sw(uint32_t *old, uint32_t *new);
extern void init_ctx(uint32_t *regs);
extern void idle();
extern void processus1();
extern void processus2();
extern void processus3();
extern void processus4();
extern void processus5();
extern void processus6();
extern void processus7();
extern void processus8();
extern void processus9();
extern void processusX(int x);

pid_t pid_actif = -1;

pid_t getpid() {
    return pid_actif;
}


void init_process() {
    initQueue();
    for (int i = 0; i < MAX_PROCESS; i++) {
        process_table[i].pid = -1;
        process_table[i].state = -1;
        process_table[i].name = 0;
        for (int j = 0; j < NUM_REGS; j++) {
            process_table[i].regs[j] = 0;
        }
    }
    ;
    pid_t pid_idle = creer("idle", idle, 0, 0);
    pid_actif = dequeue();
    process_table[pid_idle].state = ELU;

    char *argv1[] = {"11"};
    //pid_t pid_processusX = creer("processusX", processusX, 2, argv1);
    pid_t pid_processus1 = creer("processus1", processus1, 0, 0);
    pid_t pid_processus2 = creer("processus2", processus2, 0, 0);
    // processus X with 2 arguments
    pid_t pid_processusX = creer("processusX", processusX, 1, argv1);
    idle();
}


void envelopper() {
    int argc;
    void *argv[argc];

    // Récupérer les autres arguments à partir de la structure de processus actuelle
    pid_t pid = getpid();
    fnptr *f = (fnptr *)process_table[pid].f;
    argc = process_table[pid].argc;
    for (int i = 0; i < argc; i++) {
        argv[i] = process_table[pid].args[i];
    }

    
    // Appeler la fonction du processus avec les arguments
    callWithArgs(*f, argc, argv);
    terminer();
}


void callWithArgs(fnptr f, int argc, char *argv[]) {
    if (argc == 0) {
        f();
    } else {
        f(argc, argv);
    }
}



pid_t fork(const char *name, fnptr f, int argc, char *argv[]) {
    return creer(name, f, argc, argv);
}


/* Passage de l'état élu à prêt actif */
void arreter(){
    pid_t pid = getpid();
    if (pid == -1) {
        // Il n'y a pas de processus actif
        return;
    }
    if (process_table[pid].state == ELU){
        // On le passe de l'état ELU à ACTIF
        process_table[pid].state = ACTIF; 
        // On ajoute le processus en cours d'exécution à la liste des processus actifs
        addProcess(pid);
    }
}


void bloquer(){
    // On récupère le pid du processus en cours
    pid_t pid = getpid();
    if (process_table[pid].state == ELU) {
        process_table[pid].state = BLOQUE;
        scheduler();
    } 
}


void debloquer(pid_t pid){
    if (process_table[pid].state == BLOQUE) {
        process_table[pid].state = ACTIF;
        // On ajoute le processus débloqué à la file des processus actifs
        addProcess(pid);
    }
}

void addProcess(pid_t pid) {
    enqueue(pid);
}

void removeProcess(pid_t pid) {
    dequeue(pid);
}

void scheduler() {
    // On récupère le pid du processus élu
    pid_t pid = getpid();
    
    // On arrête le processus élu
    if (pid != -1) {
        arreter();
    }

    // On récupère le pid du processus suivant
    pid_t next_pid = dequeue();
    if (next_pid == 0) {
        // On ne veut pas élire idle tant qu'il reste des processus actifs
        if (isEmpty()) {
            next_pid = 0;
        } else {
            addProcess(next_pid);
            next_pid = dequeue();
        }
    }
    //printf("pid suivant : %d\n", next_pid);

    pid_actif = next_pid;
    
    if (next_pid == -1) {
        // Il n'y a pas de processus actif
        return;
    }

    // On passe le processus suivant à l'état ELU
    process_table[next_pid].state = ELU;
    
    if (pid == -1) {
        return;
    }

    // On coupe les interruptions le temps de changer de contexte
    sti();
    // On charge le contexte du processus suivant
    ctx_sw(process_table[pid].regs, process_table[next_pid].regs);
    
}

void terminer() {
    pid_t pid = getpid();
    // On libère le pid du processus
    liberer_pid(pid);
    // On libère la pile du processus
    free(process_table[pid].stack);
    // On met à jour l'état du processus
    process_table[pid].state = -1;
    // On appelle le scheduler
    printf("Processus %d termine\n", pid);
    scheduler();

}


void liberer_pid(pid_t pid) {
    // On libère le pid du processus
    process_table[pid].pid = -1;
}

pid_t allouer_pid() {
    // On cherche un pid libre
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (process_table[i].pid == -1) {
            return i;
        }
    }
    return -1;
}


pid_t creer(const char *name, fnptr f, int argc, char *argv[]) {
    pid_t pid = allouer_pid();
    if (pid == -1) {
        printf("Pas de pid disponible\n");
        return -1;
    }
    process_table[pid].f = f;
    // Copier les arguments dans la structure de processus
    for (int i = 0; i < argc; i++) {
        process_table[pid].args[i] = argv[i];
    }
    process_table[pid].argc = argc;

    // Réserver de l'espace sur la pile pour l'adresse de la fonction
    process_table[pid].stack[STACK_SIZE-1] = (uint32_t)f;

    process_table[pid].pid = pid;
    process_table[pid].state = ACTIF;
    addProcess(pid);
    
    process_table[pid].name = name;

    // Initialisation de la pile du processus
    uint32_t *stack_top = (uint32_t *)(process_table[pid].stack + STACK_SIZE);
    
    // Réserver de l'espace sur la pile pour l'enveloppe
    *(--stack_top) = (uint32_t)envelopper;

    // Initialiser le contexte du processus
    for (int i = 0; i < NUM_REGS; i++) {
        process_table[pid].regs[i] = 0;
    }
    process_table[pid].regs[ESP] = (uint32_t)stack_top;
    
    printf("Processus %s cree avec le pid %d\n", name, pid);
    return pid;
}
