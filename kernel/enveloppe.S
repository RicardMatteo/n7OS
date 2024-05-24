.global envelopper_asm

envelopper_asm:
    pushl %ebp            # Sauvegarder l'ancien EBP
    movl %esp, %ebp       # Mettre à jour EBP
    pushl 8(%ebp)         # Stocker l'adresse de retour sur la pile
    call *12(%ebp)        # Appeler la fonction
    addl $4, %esp         # Nettoyer l'adresse de retour de la pile
    popl %ebp             # Restaurer l'ancien EBP
    ret
