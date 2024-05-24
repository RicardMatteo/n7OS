#include <n7OS/process.h>
#include <n7OS/cpu.h>

struct process_t process_table[MAX_PROCESS];

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

    // On initialise la table des processus
    for (int i = 0; i < MAX_PROCESS; i++) {
        process_table[i].pid = -1;
        process_table[i].state = -1;
        process_table[i].name = 0;
        process_table[i].regs[EBX] = 0;
        process_table[i].regs[ESP] = 0;
        process_table[i].regs[EBP] = 0;
        process_table[i].regs[ESI] = 0;
        process_table[i].regs[EDI] = 0;
    }
    char *argv[] = {"11"};

    pid_t pid_idle = creer("idle", idle, 0, argv);
    pid_actif = dequeue();
    process_table[pid_idle].state = ELU;

    //pid_t pid_processus1 = creer("processus1", processus1, 0, argv);
    //pid_processus1 = creer("processus1", processus1, 0, argv);
    //pid_processus1 = creer("processus1", processus1, 0, argv);
    pid_t pid_processus2 = creer("processus2", processus2, 0, argv);
    pid_t pid_processus3 = creer("processus3", processus3, 0, argv);
    pid_t pid_processus4 = creer("processus4", processus4, 0, argv);
    //pid_t pid_processus5 = creer("processus5", processus5, 0, argv);
    //pid_t pid_processusX = creer("processusX", processusX, 1, argv);
    /*
    pid_t pid_processus6 = creer("processus6", processus6, 0, argv);
    pid_t pid_processus7 = creer("processus7", processus7, 0, argv);
    pid_t pid_processus8 = creer("processus8", processus8, 0, argv);
    pid_t pid_processus9 = creer("processus9", processus9, 0, argv);
    */
    
    idle();


    //init_ctx(process_table[pid_idle].regs);
    //printf("Contexte idle initialise\n");
}


// todo sleep and fork


pid_t fork(const char *name, fnptr f, int argc, char *argv[]) {
    return creer(name, f, argc, argv);
}

void envelopper(fnptr f) {
    printf("Envelopper %d\n", getpid());
    // print esp
    printf("esp envelopper : %d\n", process_table[getpid()].regs[ESP]);
    f();
    terminer();
}

/* Création d'un processus */
pid_t creer(const char *name, fnptr f, int argc, char *argv[]){
    pid_t pid = allouer_pid();
    // On alloue un pid un processus
    if (pid == -1) {
        // pauvre processus :(
        printf("Pas de pid disponible\n");
        return -1;
    }
    
    // On l'ajoute à la table des processus
    process_table[pid].pid = pid;
    // On l'ajoute à la file des processus actifs
    process_table[pid].state = ACTIF;
    addProcess(pid);
    
    // On lui donne un nom
    process_table[pid].name = name;
    /*

    // On y met les arguments de la fonction enveloppée
    for (int i=0; i<argc;i++){
        process_table[pid].stack[STACK_SIZE-i-1] = argv[argc-i-1];
    }
    process_table[pid].stack[STACK_SIZE-argc-2] = (uint32_t) envelopper;
    process_table[pid].stack[STACK_SIZE-argc-1] = (uint32_t) f;
    
    // print the adress of the stack elements
    printf("stack : %d\n", process_table[pid].stack);
    printf("stack[STACK_SIZE-argc-2] : %d\n", process_table[pid].stack[STACK_SIZE-argc-2]);
    printf("stack[STACK_SIZE-argc-1] : %d\n", process_table[pid].stack[STACK_SIZE-argc-1]);
    
    // print the stack elements
    printf("stack[STACK_SIZE-argc-2] : %d\n", &(process_table[pid].stack[STACK_SIZE-argc-2]));
    printf("stack[STACK_SIZE-argc-1] : %d\n", &(process_table[pid].stack[STACK_SIZE-argc-1]));
    printf("stack[STACK_SIZE-argc-2] : %d\n", process_table[pid].stack + STACK_SIZE-argc-2);
    printf("stack[STACK_SIZE-argc-1] : %d\n", process_table[pid].stack + STACK_SIZE-argc-1);
    process_table[pid].regs[EBX] = 0;
    process_table[pid].regs[ESP] = process_table[pid].stack + STACK_SIZE-argc-1;
    printf("esp : %d\n", process_table[pid].regs[ESP]);
    process_table[pid].regs[EBP] = process_table[pid].stack + STACK_SIZE-argc-2;
    process_table[pid].regs[ESI] = 0;
    process_table[pid].regs[EDI] = 0;
    */
    // on initialise le contexte du processus
    argc = 1;
    argv[0] = f;
    init_context(&process_table[pid].regs, envelopper, process_table[pid].stack, argc, argv);

    if (pid == 0) {
        process_table[pid].stack[STACK_SIZE-1] = (uint32_t) f;
        process_table[pid].regs[ESP] = process_table[pid].stack + STACK_SIZE-1;
    }
    printf("Processus %s cree avec le pid %d\n", name, pid);

    // Process correctement créé
    return pid;
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
    
    /*
    printf("Changement de contexte\n");
    printf("pid : %d\n", pid);
    printf("next_pid : %d\n", next_pid);
    // print les esp
    printf("esp pid : %d\n", process_table[pid].regs[ESP]);
    printf("esp next_pid : %d\n", process_table[next_pid].regs[ESP]);
    */
    
    // Print les esp
    printf("esp pid : %d\n", process_table[pid].regs[ESP]);
    printf("esp next_pid : %d\n", process_table[next_pid].regs[ESP]);

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

// Fonction pour initialiser le contexte d'un processus avec des arguments
void init_context(uint32_t *ctx, void (*func)(int, int*), uint8_t *stack, int num_args, int *args) {
    uint32_t *stack_top = (uint32_t *)(stack + STACK_SIZE);

    // Initialiser la pile pour contenir les arguments et l'adresse de retour
    *(--stack_top) = (uint32_t)func;         // Adresse de retour (en fait, l'adresse de la fonction de processus)
    *(--stack_top) = 0;                      // EBP initial (aucune chaîne d'appel précédente)
    
    // Placer les arguments sur la pile
    for (int i = num_args - 1; i >= 0; i--) {
        *(--stack_top) = args[i];
    }

    // Placer l'adresse du tableau des arguments et le nombre d'arguments sur la pile
    *(--stack_top) = (uint32_t)(stack_top + 1);  // Adresse du tableau des arguments (dans la pile elle-même)
    *(--stack_top) = num_args;                 // Nombre d'arguments
    sti();
    // Initialiser le contexte
    memset(ctx, 0, sizeof(uint32_t) * 5);
    ctx[ESP] = (uint32_t)stack_top;            // ESP pointe vers le sommet de la pile initialisée
    //ctx[ebp] = 0;                              // EBP initial
}
