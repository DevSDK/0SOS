[BITS 64]

global ContextSwitch

SECTION .text   

%macro SAVECONTEXT 0
    push rbp
    push rax
    push rbx
    push rcx
    push rdx
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov ax, ds
    push rax
    mov ax, es
    push rax
    push fs
    push gs

    mov ax, 0x10
    mov ds,ax
    mov es,ax
    mov gs,ax
    mov fs,ax

%endmacro


%macro LOADCONTEXT 0
    pop gs
    pop fs
    pop rax
    mov es, ax
    pop rax
    mov ds, ax

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    pop rbp

%endmacro

ContextSwitch:
    push rbp
    mov rbp, rsp
    ;Push RFLAGS in stack
    pushfq
    ;if _CurrentContext is NULL
    cmp rdi,0
    je .LoadContext
    popfq
    push rax    ;For use Context Offset 

    ;Save SS RSP RFLAGS CS RIP Registers
    mov ax, ss
    mov qword[rdi + 23 * 8], rax
    mov rax, rbp

    ;RSP를 Contextswitch 호출 이전으로 되돌림
    add rax, 16
    mov qword[rdi + 22 * 8], rax

    ;Push FLAGS in stack
    pushfq
    pop rax
    mov qword[rdi + 21 * 8], rax
    
    ;Save Cs Segment
    mov ax, cs
    mov qword[rdi + 20 * 8], rax

    mov rax, qword[rbp + 8]
    mov qword[rdi + 19 * 8], rax

    pop rax
    pop rbp
    ;Context->Registers를 자동으로 채워버림
    add rdi, 19*8
    mov rsp, rdi
    sub rdi, 19*8

    SAVECONTEXT

.LoadContext:
    mov rsp, rsi
    LOADCONTEXT
    iretq




