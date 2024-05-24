#include <n7OS/mem.h>

/**
 * @brief Tableau de bitmap pour la gestion de la mémoire physique   
*/
uint32_t free_page_bitmap_table[NB_PAGE/32];


/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
    free_page_bitmap_table[addr/PAGE_SIZE/32] |= (1 << (addr/PAGE_SIZE%32));
}

/**
 * @brief Désalloue la page
 * 
 * Libère la page allouée.
 * 
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    free_page_bitmap_table[addr/PAGE_SIZE/32] &= ~(1 << (addr/PAGE_SIZE%32));
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * 
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage() {
    uint32_t adresse= 0x0;
    for (uint32_t i = 0; i < NB_PAGE/32; i++) {
        if (free_page_bitmap_table[i] != 0xFFFFFFFF) {
            for (uint32_t j = 0; j < 32; j++) {
                if ((free_page_bitmap_table[i] & (1 << j)) == 0) {
                    adresse = (i*32+j)*PAGE_SIZE;
                    setPage(adresse);
                    return adresse;
                }
            }
        }
    }

    return adresse;
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 * 
 */
void init_mem() {
    // Initialisation de la bitmap avec toutes les pages libres
    for (uint32_t i = 0; i < NB_PAGE/32; i++) {
        free_page_bitmap_table[i] = 0;
    }
}

/**
 * @brief Affiche l'état de la mémoire physique
 * 
 */
void print_mem() {
    for (uint32_t i = 0; i < NB_PAGE/32; i++) {
        for (uint32_t j = 0; j < 32; j++) {
            if ((free_page_bitmap_table[i] & (1 << j)) == 0) {
                printf("0");
            } else {
                printf("1");
            }
        }
    }
}