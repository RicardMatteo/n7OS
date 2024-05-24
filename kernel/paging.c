#include <n7OS/paging.h>
#include <stddef.h> // nécessaire pour NULL

void initialise_paging() {
    // Allouer une zone mémoire pour le répertoire de pages
    PageTable *pgtab = (PageTable *) kmalloc_a(sizeof(PageTable));

    // Initialiser les tables de pages

    // Charger le répertoire de pages dans le registre CR3

    // Lancez la pagination en activant le bit PG du registre CR0
}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    //PageTable pgtab = 
//
    //page_entry_t *entry = 
    //entry->addr_low = address & 0xF;
    //entry->addr_high = (address >> 4) & 0xFFFF;
    //entry->flags = (1 - is_kernel) << 2 | is_writeable << 1 | 1;
    //entry->zero = 0;
    //return pgtab;
}
