section .text

global strstr_normal
global strstr_xxx
global strstr_sse42

;----------------------------- witchout special instructions
strstr_normal:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]
    %idefine argy [rsi]

    dec rdi
loop1:
    inc rdi
    xor rbx, rbx
    matchLoop:
        mov al, [rdi + rbx] ; symbol from text
        mov ah, [rsi + rbx] ; symbol from key
        cmp ah, 0
        je endloop1 ; found

        cmp al, ah
        jne notFound ; no match

        cmp al, 0 ; end of text
        je endloop1 ; no match


        inc rbx
        jmp matchLoop
    notFound:

    jmp loop1
endloop1:

    xor rbx, rbx
    mov rax, rdi

    pop rbp
    ret


;---------------------------- witch cmpsb
strstr_xxx:
    push rbp
    mov rbp,rsp

    %idefine argx [rdi]
    %idefine argy [rsi]

    dec rdi
LOOP_1:
    inc rdi

    mov rcx, 0x7fffffff

    ;push rdi
    mov r8, rdi
    ;push rsi
    mov r9, rsi

    repz cmpsb

    ;pop rsi
    mov rsi, r9
    ;pop rdi
    mov rdi, r8

    mov rdx, 0x7fffffff
    sub rdx, rcx
    dec rdx

    mov al, [rdi + rdx] ; symbol from text
    mov ah, [rsi + rdx] ; symbol from key

    cmp ah, 0
    je END_LOOP1 ; found

    cmp al, 0 ; end of text
    je END_LOOP1 ; no match

    jmp LOOP_1
END_LOOP1:

    ;xor rbx, rbx
    mov rax, rdi

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


strstr_sse42:

    mov rcx, rdi
    mov rdx, rsi

    ; rcx = haystack, rdx = needle

    push rsi
    push rdi
    MovDqU xmm2, [rdx] ; load the first 16 bytes of neddle
    Pxor xmm3, xmm3
    lea rax, [rcx - 16]

  ; find the first possible match of 16-byte fragment in haystack
STRSTR_MAIN_LOOP:
        add rax, 16
        PcmpIstrI xmm2, [rax], EQUAL_ORDERED
        ja STRSTR_MAIN_LOOP

    jnc STRSTR_NOT_FOUND

    add rax, rcx ; save the possible match start
    mov rdi, rdx
    mov rsi, rax
    sub rdi, rsi
    sub rsi, 16

    ; compare the strings
STRSTR_LOOP:
        add rsi, 16
        MovDqU    xmm1, [rsi + rdi] ; read from needle
        ; mask out invalid bytes in the haystack
        PcmpIstrM xmm3, xmm1, EQUAL_EACH + NEGATIVE_POLARITY + BYTE_MASK
        MovDqU xmm4, [rsi] ; read from haystack
        PAnd xmm4, xmm0
        PcmpIstrI xmm1, xmm4, EQUAL_EACH + NEGATIVE_POLARITY
        ja STRSTR_LOOP

    jnc STRSTR_FOUND

    ; continue searching from the next byte
    sub rax, 15
    jmp STRSTR_MAIN_LOOP

STRSTR_NOT_FOUND:
    xor rax, rax

STRSTR_FOUND:
    pop rdi
    pop rsi
    ret
