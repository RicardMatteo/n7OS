#include <n7OS/mem.h>
#include <stdio.h>

uint32_t free_page_bitmap_table[((LAST_MEMORY_INDEX + 1) / PAGE_SIZE) / 32];

/**
 * @brief Marque la page allouée
 *
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 *
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
    free_page_bitmap_table[(addr >> 12) / 32] |= (1 << (addr % 32));
}

/**
 * @brief Désalloue la page
 *
 * Libère la page allouée.
 *
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    free_page_bitmap_table[(addr >> 12) / 32] &= ~(1 << (addr % 32));
}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage() {
    for (int i = 0; i < (LAST_MEMORY_INDEX + 1) / PAGE_SIZE / 32; i++) {
        if (free_page_bitmap_table[i] != 0xFFFFFFFF) {
            for (int j = 0; j < 32; j++) {
                if ((free_page_bitmap_table[i] & (1 << j)) == 0) {
                    free_page_bitmap_table[i] |= (1 << j);
                    return ((uint32_t)i * 32 + j);
                }
            }
        }
    }
    return 0;
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 */
void init_mem() {
    for (int i = 0; i < (LAST_MEMORY_INDEX + 1) / PAGE_SIZE / 32; i++) {
        free_page_bitmap_table[i] = 0;
    }
}

/**
 * @brief Affiche l'état de la mémoire physique
 */
void print_mem() {
    // nombre de pages à print
    int nb_page_print = 64;

    // stolen and adapted from Nathan
    printf("Mem dump : \n");
    for (int i = 0; i < nb_page_print; i++) {
        uint32_t liste_page = free_page_bitmap_table[i];
        printf("%d ", liste_page);
        if (i % 16 == 15) {
            printf("\n");
        }
    }
    if (nb_page_print < (LAST_MEMORY_INDEX + 1) / PAGE_SIZE / 32) {
        printf("...\n");
    }
}