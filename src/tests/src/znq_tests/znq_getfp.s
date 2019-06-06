    .file "znq_getfp.s"
    .text
    .align 2
    .global get_fp
    .type   get_fp, %function
get_fp:
    mov rp, fp
    mov pc, lr

