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
; 
; ;---------------------------- witch pcmpestrI
; 
; ; Immediate byte constants for SSE4.2 string instructions
; %idefine EQUAL_ANY	           0000b
; %idefine RANGES		           0100b
; %idefine EQUAL_EACH	           1000b
; %idefine EQUAL_ORDERED	       1100b
; %idefine NEGATIVE_POLARITY   010000b
; %idefine BYTE_MASK	        1000000b
; 
; strstr_sse42:
;     push rbp
;     mov rbp,rsp
; 
;     %idefine argx [rdi]
;     %idefine argy [rsi]
; 
;     dec rdi
; LOOP_2:
;     inc rdi
; 
;     mov rcx, 0x7fffffff
; 
;     push rdi
;     push rsi
;     repz cmpsb
;     pop rsi
;     pop rdi
; 
;     mov rbx, 0x7fffffff
;     sub rbx, rcx
; 
;     mov al, [rdi + rbx] ; symbol from text
;     mov ah, [rsi + rbx] ; symbol from key
; 
;     cmp ah, 0
;     je END_LOOP2 ; found
; 
;     cmp al, 0 ; end of text
;     je END_LOOP2 ; no match
; 
;     jmp LOOP_2
; END_LOOP2:
; 
;     mov rax, rdi
; 
;     pop rbp
;     ret
