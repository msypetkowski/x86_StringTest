section .text

global strcmp_normal
global strcmp_cmps
global strcmp_sse42

;----------------------------- witchout special instructions
strcmp_normal:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]
    %idefine argy [rsi]
    %idefine argn rdx

    mov rcx, argn
loop1:
    mov al, [rdi]
    mov ah, [rsi]
    cmp al, ah
    jne endloop1
    inc rdi
    inc rsi
    loop loop1
endloop1:
    mov rax, argn
    sub rax, rcx

    pop rbp
    ret

;---------------------------- witch cmpsb

strcmp_cmps:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]
    %idefine argy [rsi]
    %idefine argn rdx

    mov rcx, argn
    repz cmpsb

    mov rax, argn
    sub rax, rcx
    sub rax, 1

    pop rbp
    ret

;---------------------------- witch pcmpistrI

; Immediate byte constants for SSE4.2 string instructions
%idefine EQUAL_ANY	           0000b
%idefine RANGES		           0100b
%idefine EQUAL_EACH	           1000b
%idefine EQUAL_ORDERED	       1100b
%idefine NEGATIVE_POLARITY   010000b
%idefine BYTE_MASK	        1000000b

strcmp_sse42:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]
    %idefine argy [rsi]
    %idefine argn rdx

    xor rax, rax

    sub rax, 16
    sub rdi, 16
    sub rsi, 16

STRCMP_LOOP:
    add rax, 16
    add rdi, 16
    add rsi, 16

    MovDqU  xmm0, [rdi]
    ; find the first different bytes, hence negative polarity
    PcmpIstrI xmm0, [rsi], EQUAL_EACH + NEGATIVE_POLARITY

    ja STRCMP_LOOP ; all equal

    add rax, rcx

    pop rbp
    ret
