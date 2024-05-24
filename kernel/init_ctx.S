    .text
    .globl init_ctx
    
    init_ctx:
        movl 4(%esp), %eax
        movl (%eax), %ebx
        movl 4(%eax), %esp
        movl 8(%eax), %ebp
        movl 12(%eax), %esi
        movl 16(%eax), %edi
        # on passe la main au nouveau processus
        ret
