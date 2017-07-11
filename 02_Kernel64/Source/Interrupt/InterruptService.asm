[BITS 64]


SECTION .text

extern DefaultExceptionHandler,DefaultInterruptHandler, KeyboardInterruptHandler

global ISRDividError,ISRDebug,ISRNMI,ISRBreakPoint,ISROverflow
global ISRBoundRangeExceeded,ISRInvalidOpCode,ISRDeviceNotAvailable
global ISRDoubleFault,ISRCoProcessorSegmentOverrun,ISRInvalidTSS
global ISRSegmentNotPresent, ISRStackSegmentFault,ISRGeneralProtection
global ISRPageFault,ISR15,ISRFloatingPointError,ISRAlignmentCheck
global ISRMachineCheck,ISRDefaultException,ISRTimer,ISRPS2Keyboard
global ISRSlavePIC,ISRSerialPort1,ISRSerialPort2,ISRParallel2
global ISRFloppy,ISRParallel1,ISRRTC,ISRReserved,ISRNotUsed1,ISRSIMDError
global ISRNotUsed2,ISRPS2Mouse,ISRCoprocessor,ISRHDD1,ISRHDD2,ISRDefaultInterrupt


%macro SAVECONTEXT 0
    push rbp
    mov rbp, rsp
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

; Exception Handler

ISRDividError:
    SAVECONTEXT
    mov rdi, 0
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRDebug:
    SAVECONTEXT
    mov rdi, 1
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRNMI:
    SAVECONTEXT
    mov rdi, 2
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRBreakPoint:
    SAVECONTEXT
    mov rdi, 3
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISROverflow:
    SAVECONTEXT
    mov rdi, 4
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRBoundRangeExceeded:
    SAVECONTEXT
    mov rdi, 5
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRInvalidOpCode:
    SAVECONTEXT
    mov rdi, 6
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRDeviceNotAvailable:
    SAVECONTEXT
    mov rdi, 7
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRDoubleFault:
    SAVECONTEXT
    mov rdi, 8
    mov rsi, qword[rbp+8]
    call DefaultExceptionHandler
    LOADCONTEXT
    add rsp, 8
    iretq
ISRCoProcessorSegmentOverrun:
    SAVECONTEXT
    mov rdi, 9
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRInvalidTSS:
    SAVECONTEXT
    mov rdi, 10
    mov rsi, qword[rbp+8]
    call DefaultExceptionHandler
    LOADCONTEXT
    add rsp, 8
    iretq
ISRSegmentNotPresent:
    SAVECONTEXT
    mov rdi, 11
    mov rsi, qword[rbp+8]
    call DefaultExceptionHandler
    LOADCONTEXT
    add rsp, 8
    iretq
ISRStackSegmentFault:
    SAVECONTEXT
    mov rdi, 12
    mov rsi, qword[rbp+8]
    call DefaultExceptionHandler
    LOADCONTEXT
    add rsp,8
    iretq
ISRGeneralProtection:
    SAVECONTEXT
    mov rdi, 13
    mov rsi, qword[rbp+8]
    call DefaultExceptionHandler
    LOADCONTEXT
    add rsp,8
    iretq
ISRPageFault:
    SAVECONTEXT
    mov rdi, 14
    mov rsi, qword[rbp + 8]
    call DefaultExceptionHandler
    LOADCONTEXT
    add rsp, 8
    iretq
ISR15:
    SAVECONTEXT
    mov rdi, 15
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRFloatingPointError:
    SAVECONTEXT
    mov rdi, 16
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRAlignmentCheck:
    SAVECONTEXT
    mov rdi, 17
    mov rsi, qword[rbp + 8]
    call DefaultExceptionHandler
    LOADCONTEXT
    add rsp, 8
    iretq
ISRMachineCheck:
    SAVECONTEXT
    mov rdi, 18
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRSIMDError:
    SAVECONTEXT
    mov rdi,19
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRDefaultException:
    SAVECONTEXT
    mov rdi, 20
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq

;PIC Interrupt Handler    
ISRTimer:
    SAVECONTEXT
    mov rdi, 32
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRPS2Keyboard:
    SAVECONTEXT
    mov rdi, 33
    call KeyboardInterruptHandler
    LOADCONTEXT
    iretq
ISRSlavePIC:
    SAVECONTEXT
    mov rdi, 34
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRSerialPort1:
    SAVECONTEXT
    mov rdi, 35
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRSerialPort2:
    SAVECONTEXT
    mov rdi, 36
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRParallel2:
    SAVECONTEXT
    mov rdi, 37
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRFloppy:
    SAVECONTEXT
    mov rdi, 38
    call DefaultExceptionHandler
    LOADCONTEXT
    iretq
ISRParallel1:
    SAVECONTEXT
    mov rdi, 39
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRRTC:
    SAVECONTEXT
    mov rdi, 40
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRReserved:
    SAVECONTEXT
    mov rdi, 41
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRNotUsed1:
    SAVECONTEXT
    mov rdi, 42
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRNotUsed2:
    SAVECONTEXT
    mov rdi, 43
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRPS2Mouse:
    SAVECONTEXT
    mov rdi, 44
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRCoprocessor:
    SAVECONTEXT
    mov rdi, 45
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRHDD1:
    SAVECONTEXT
    mov rdi, 46
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRHDD2:
    SAVECONTEXT
    mov rdi, 47
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq
ISRDefaultInterrupt:
    SAVECONTEXT
    mov rdi, 48
    call DefaultInterruptHandler
    LOADCONTEXT
    iretq