section .text

global strlen_normal
global strlen_scas
global strlen_sse42

;----------------------------- witchout special instructions
strlen_normal:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]

    mov rsi, rdi
loop1:
    mov al, [rdi]
    cmp al, 0
    je endloop1
    inc rdi
    jmp loop1
endloop1:
    mov rax, rdi
    sub rax, rsi

    pop rbp
    ret

;---------------------------- witch cmpsb
strlen_scas:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]

    mov rcx, 0x7fffffff
    mov rax, 0
    mov rsi, rdi
    repnz scasb

    mov rax, 0x7fffffff
    sub rax, rcx
    dec rax

    pop rbp
    ret

;---------------------------- witch pcmpestrI

; Immediate byte constants for SSE4.2 string instructions
%idefine EQUAL_ANY	           0000b
%idefine RANGES		           0100b
%idefine EQUAL_EACH	           1000b
%idefine EQUAL_ORDERED	       1100b
%idefine NEGATIVE_POLARITY   010000b
%idefine BYTE_MASK	        1000000b

strlen_sse42:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]

    pxor xmm0, xmm0
    mov rax, -16

STRLEN_LOOP:
    add rax, 16
    PcmpIstrI xmm0, [rdi + rax], EQUAL_EACH
    jnz STRLEN_LOOP

    add rax, rcx

    pop rbp
    ret
