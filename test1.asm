section .text

global memcpy_normal
global memcpy_movsb
global memcpy_movsq
global memcpy_movdqu

;-----------------------------
memcpy_normal:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]
    %idefine argy [rsi]
    %idefine argn rdx

    mov rcx, argn
loop1:
    mov al, [rdi+rcx-1]
    mov byte [rsi+rcx-1], al
    loop loop1

    pop rbp
    ret

;-----------------------------
memcpy_movsb:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]
    %idefine argy [rsi]
    %idefine argn rdx

    mov rcx, argn
    xchg rdi, rsi
    rep movsb
    xchg rdi, rsi

    pop rbp
    ret

;---------------------------------

memcpy_movsq:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]
    %idefine argy [rsi]
    %idefine argn rdx

    mov rcx, argn
    sar rcx, 3

    xchg rdi, rsi
    rep movsq

    ; remaining bytes
    mov rax, argn
    sar rax, 3
    sal rax, 3
    mov rcx, argn
    sub rcx, rax
    rep movsb

    xchg rdi, rsi


    pop rbp
    ret

;-----------------------------
memcpy_movdqu:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]
    %idefine argy [rsi]
    %idefine argn rdx

    mov rcx, rdx
    sar rcx, 4 ; block size is 2^4

    jz NOFULLBLOCKS
MEMCPYLOOP:
    MovDqU  xmm0, [rdi]
    MovDqU  [rsi], xmm0
    add rdi, 16
    add rsi, 16
    loop MEMCPYLOOP
NOFULLBLOCKS:

    ; copy remaining bytes
    mov rax, argn
    sar rax, 4
    sal rax, 4
    mov rcx, argn
    sub rcx, rax
    jnz MEMCPYLOOP_REST ; some bytes left

    pop rbp
    ret

MEMCPYLOOP_REST:
    mov al, [rdi+rcx-1]
    mov byte [rsi+rcx-1], al
    loop MEMCPYLOOP_REST

    pop rbp
    ret
