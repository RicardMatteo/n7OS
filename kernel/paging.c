#include "n7OS/paging.h"
#include "n7OS/kheap.h"
#include "string.h"
#include "n7OS/mem.h"
#include "stdio.h"

PageDirectory pageDirectory;

/*
void handler_page_fault(registers_t reg) {
       // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    __asm__ __volatile__("mov %%cr2, %0" : "=r" (faulting_address));
    
    // The error code gives us details of what happened.
    int present = !(reg.err_code & 0x1); // Page not present
    int rw = reg.err_code & 0x2;           // Write operation?
    int us = reg.err_code & 0x4;           // Processor was in user-mode?
    int reserved = reg.err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = reg.err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.
    printf("Page fault! ( ");
    if (present) {printf("present ");}
    if (rw) {printf("read-only ");}
    if (us) {printf("user-mode ");}
    if (reserved) {printf("reserved ");}
    if (id) {printf("instruction ");}
    printf(") at 0x%x\n", faulting_address);
    PANIC("Page fault");
}*/

void setPageEntry(PTE *page_table_entry, uint32_t new_page, int is_writeable, int is_kernel) {
    page_table_entry->page_entry.present= 1;
    page_table_entry->page_entry.accessed= 0;
    page_table_entry->page_entry.dirty= 0;
    page_table_entry->page_entry.rw= is_writeable;
    page_table_entry->page_entry.user= ~is_kernel;
    page_table_entry->page_entry.page= new_page>>12;
}

void initialise_paging() {

    init_kheap();

    uint32_t index= 0;
    
    init_mem();

    pageDirectory= (PageDirectory) kmalloc_a (sizeof(PDE)*1024);
    //memset(pageDirectory, 0, sizeof(PDE)*1024);

    for (int i= 0; i<1024; ++i) {
        PageTable new_page_table= (PageTable) kmalloc_a(sizeof(PTE)*1024);
        //memset(new_page_table, 0, sizeof(PTE)*1024);
        //pageDirectory[i].value= (uint32_t) new_page_table|PAGE_PRESENT|PAGE_RW;
        pageDirectory[i].dir_entry.page_table= (uint32_t) new_page_table >> 12;
        pageDirectory[i].dir_entry.present= 1;
        pageDirectory[i].dir_entry.rw= 1;
        pageDirectory[i].dir_entry.user= 1;
        
        index= (uint32_t) new_page_table + sizeof(PTE) * 1024;
    }

    for (int i= 0; i<index; i += PAGE_SIZE) {
        alloc_page_entry(i, 1, 1);
    }

    // register_interrupt_handler(14, handler_page_fault);

    //loadPageDirectory((unsigned int *)pageDirectory);
    enablePaging();
    //return 0;
    setup_base((uint32_t) pageDirectory);
    //setup_base((uint32_t) 0);

}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel ) {
    // address = adresse virtuelle à allouer 
    // address = idx_PDE | idx_PTE | offset
    //             10    |    10   |   12

    int idx_pagedir = address >> 22; // calcul de la position de la table de page dans le répertoire de page
    PageTable page_table;

    // on recupere l'entree dans le répertoire de page
    // une entree contient l'adresse de la table de page + bits de controle
    PDE page_dir_entry = pageDirectory[idx_pagedir]; 
    
    // page_table= ...; // on recupere l'adresse de la page de table dans le répertoire de page
    page_table = (PageTable) (page_dir_entry.dir_entry.page_table << 12);     
    //uint32_t phy_page= ...; // recherche d'une page libre dans la memoire physique
    // uint32_t phy_page = findfreePage(); // recherche d'une page libre dans la memoire physique
    
    //int idx_pagetab= ...; // calcul de la position de la page dans la table de page
    int idx_pagetab = (address >> 12) & 0x3FF; // calcul de la position de la page dans la table de page        

    // mise a jour de l'entree dans la page de table
    setPageEntry(&page_table[idx_pagetab], address, is_writeable, is_kernel);
    setPage(address);

    return page_table;
}

void enablePaging() {
    __asm__ __volatile__("mov %0, %%cr3":: "r"(pageDirectory));
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0":: "r"(cr0));
}

/*
void loadPageDirectory(unsigned int* dir) {
    __asm__ __volatile__("mov %0, %%cr3":: "r"(dir));
}
*/


