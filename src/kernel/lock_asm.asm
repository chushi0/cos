[bits 32]

section .text

global tryLock
tryLock:
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp + 8]
    mov cl, 1
    cmpxchg byte [edx], cl

    leave
    ret