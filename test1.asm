section .text

global calc
global calcFast

;-----------------------------
calc:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]
    %idefine argy [rsi]
    %idefine argn rdx

    mov rcx, argn
loop1:
    mov al, [rdi+rcx-1]
    ;add al, 1
    mov byte [rsi+rcx-1], al
    loop loop1

    pop rbp
    ret

;-----------------------------
calcFast:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]
    %idefine argy [rsi]
    %idefine argn rdx

    xchg rdi, rsi
    rep movsb
    xchg rdi, rsi

    pop rbp
    ret
