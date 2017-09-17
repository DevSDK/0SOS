#include "Task.h"
#include <Utility/Memory.h>
#include <Descriptor/GDT.h>
#include <Console/Console.h>

static TCB_POOL_MANAGER g_TcbPoolManager;

void InitTask(TCB* _Tcb, QWORD _Flags, QWORD _EntryPointAddress, void* _StackAddress, QWORD _StackSize)
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
    _Tcb->StackAddress  = _StackAddress;
    _Tcb->StackSize     = _StackSize;
    _Tcb->Flags         = _Flags;
 }

 void InitializeTCBPool()
 {
    _MemSet(&(g_TcbPoolManager), 0, sizeof(g_TcbPoolManager));
    g_TcbPoolManager.StartAddress = (TCB*)TASK_TCBPOOL_ADDRESS;
    _MemSet(TASK_TCBPOOL_ADDRESS,0,sizeof(TCB) * TASK_TCBPOOL_COUNT);
    for(int i = 0; i < TASK_TCBPOOL_COUNT; i++)
        g_TcbPoolManager.StartAddress[i].list_header.ID = i;

    g_TcbPoolManager.MaxCount = TASK_TCBPOOL_COUNT;
    g_TcbPoolManager.AllocatedCount = 1;
}
 //       STATE
 //63 ------------ 55 -------0
 // 0 0 0 0 0 0 0 0 | address|

 TCB* AllocateTCB()
 {
    TCB* EmptyTCB;
    if(g_TcbPoolManager.Count == g_TcbPoolManager.MaxCount)
        return NULL;
    for(int i = 0; i < g_TcbPoolManager.MaxCount; i++)
    {

        if((g_TcbPoolManager.StartAddress[i].list_header.ID & TASK_STATE_MASK) == TASK_FREE )
        {
            EmptyTCB = &(g_TcbPoolManager.StartAddress[i]);
            break;
        }
    }
    QWORD id = (QWORD) (g_TcbPoolManager.AllocatedCount);
    EmptyTCB->list_header.ID = (( id | TASK_ALLOCATED));
    g_TcbPoolManager.Count++;
    g_TcbPoolManager.AllocatedCount++;

    if(g_TcbPoolManager.AllocatedCount ==0)
        g_TcbPoolManager.AllocatedCount = 1;
    
    return EmptyTCB;
}
void FreeTCB(QWORD _ID)
{
    QWORD index = _ID & TASK_INDEX_MASK;
    _MemSet(&(g_TcbPoolManager.StartAddress[index].Context), 0, sizeof(CONTEXT));
    g_TcbPoolManager.StartAddress[index].list_header.ID = index;
    g_TcbPoolManager.Count--;
}
TCB* CreateTask(QWORD _Flags, QWORD _EntryPointAddress)
{
    TCB* task = AllocateTCB();

    if(task == NULL)
        return NULL;
        
    void* StackAddress = (void*)(TASK_STACK_ADRESS + 
        (TASK_STACK_SIZE * (task->list_header.ID & TASK_INDEX_MASK)));
        InitTask(task,  _Flags, _EntryPointAddress, StackAddress, TASK_STACK_SIZE);

    AddTaskToScheduler(task);
    return task;
}
 