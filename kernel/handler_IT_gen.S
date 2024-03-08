#define CREATION_TRAITANT_IT(id2) \
    .text; \
identifiant_traitant_##id2: \
    pushl %eax; \
    pushl %edx; \
    pushl %ecx; \
    pushl $(id2); \
    call handler_en_C; \
    addl $4, %esp; \
    popl %ecx; \
    popl %edx; \
    popl %eax; \
    iret; \
    jmp identifiant_traitant_##id2; \
    .data; \
    .long identifiant_traitant_##id2

    .data  
.globl handler_IT
handler_IT:
CREATION_TRAITANT_IT(32)
CREATION_TRAITANT_IT(33)
CREATION_TRAITANT_IT(34)
CREATION_TRAITANT_IT(35)
CREATION_TRAITANT_IT(36)
CREATION_TRAITANT_IT(37)
CREATION_TRAITANT_IT(38)
CREATION_TRAITANT_IT(39)
CREATION_TRAITANT_IT(40)
CREATION_TRAITANT_IT(41)
CREATION_TRAITANT_IT(42)
CREATION_TRAITANT_IT(43)
CREATION_TRAITANT_IT(44)
CREATION_TRAITANT_IT(45)
CREATION_TRAITANT_IT(46)
CREATION_TRAITANT_IT(47)
CREATION_TRAITANT_IT(48)
CREATION_TRAITANT_IT(49)
CREATION_TRAITANT_IT(50)


