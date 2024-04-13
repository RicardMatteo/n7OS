#include <n7OS/rand.h>

int rand() {
    unsigned int seed = get_internal_time();
    static unsigned int call_count = 0;
    seed = seed * 1103515245 + 12345 + call_count++;
    return (int)(seed/65536) % RANDMAX;
}