#include "Task.h"
#include <Utility/Memory.h>
#include <Descriptor/GDT.h>
void InitTask(TCB* _Tcb, QWORD _ID, QWORD _Flags, QWORD _EntryPointAddress, void* _StackAddress, QWORD _StackSize)
{
    _MemSet(_Tcb->Context.Registers, 0, sizeof(_Tcb->Context.Registers));
    
    //초기화 과정의 RSP, RBP 해당 Task의 Stack Pointer 기 떄문에 + Size 
    _Tcb->Context.Registers[CONTEXT_OFFSET_RSP] = (QWORD)_StackAddress + _StackSize;
    _Tcb->Context.Registers[CONTEXT_OFFSET_RBP] = (QWORD)_StackAddress + _StackSize;
    
    //Segment Register Setup in Context 
    _Tcb->Context.Registers[CONTEXT_OFFSET_CS] = GDT_KERNEL_CODE_SEGMENT;
    _Tcb->Context.Registers[CONTEXT_OFFSET_DS] = GDT_KERNEL_DATA_SEGMENT;
    _Tcb->Context.Registers[CONTEXT_OFFSET_ES] = GDT_KERNEL_DATA_SEGMENT;
    _Tcb->Context.Registers[CONTEXT_OFFSET_FS] = GDT_KERNEL_DATA_SEGMENT;
    _Tcb->Context.Registers[CONTEXT_OFFSET_GS] = GDT_KERNEL_DATA_SEGMENT;
    _Tcb->Context.Registers[CONTEXT_OFFSET_SS] = GDT_KERNEL_DATA_SEGMENT;
    
    //Next run instruction setup
    _Tcb->Context.Registers[CONTEXT_OFFSET_RIP] = _EntryPointAddress;

    // 0 NT  IOPL  OF DF IF TF SF ZF 0  AF 0  PF 1  CF
    // 0  0  0  0  0  0  1  0  0  0  0  0  0  0  0  0
    _Tcb->Context.Registers[CONTEXT_OFFSET_RFLAG] |= 0x0200;   
    //Setup TCB Block
    _Tcb->ID            = _ID;
    _Tcb->StackAddress  = _StackAddress;
    _Tcb->StackSize     = _StackSize;
    _Tcb->Flags         = _Flags;
 }