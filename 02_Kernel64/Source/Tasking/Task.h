#ifndef __TASK_H__
#define __TASK_H__

#include <Types.h>
#include <GDS/LinkedList.h>
//SS, RSP, RFLAGS, CS, RIP.. 등등 Context
#define CONTEXT_REGISTER_COUNT     24
#define CONTEXT_REGISTER_SIZE       8

#define CONTEXT_OFFSET_GS           0
#define CONTEXT_OFFSET_FS           1
#define CONTEXT_OFFSET_ES           2
#define CONTEXT_OFFSET_DS           3
#define CONTEXT_OFFSET_R15          4
#define CONTEXT_OFFSET_R14          5
#define CONTEXT_OFFSET_R13          6
#define CONTEXT_OFFSET_R12          7
#define CONTEXT_OFFSET_R11          8
#define CONTEXT_OFFSET_R10          9
#define CONTEXT_OFFSET_R9           10
#define CONTEXT_OFFSET_R8           11
#define CONTEXT_OFFSET_RSI          12
#define CONTEXT_OFFSET_RDI          13
#define CONTEXT_OFFSET_RDX          14
#define CONTEXT_OFFSET_RCX          15
#define CONTEXT_OFFSET_RBX          16
#define CONTEXT_OFFSET_RAX          17
#define CONTEXT_OFFSET_RBP          18
#define CONTEXT_OFFSET_RIP          19
#define CONTEXT_OFFSET_CS           20
#define CONTEXT_OFFSET_RFLAG        21
#define CONTEXT_OFFSET_RSP          22
#define CONTEXT_OFFSET_SS           23

#define TASK_TCBPOOL_ADDRESS        0x800000
#define TASK_TCBPOOL_COUNT          4096

#define TASK_STACK_ADRESS           (TASK_TCBPOOL_ADDRESS + sizeof(TCB) * TASK_TCBPOOL_COUNT)
#define TASK_STACK_SIZE             8192

#define TASK_INVALID_ID             0xFFFFFFFFFFFFFFFF

#define TASK_FREE                   0x0000000000000000
#define TASK_ALLOCATED              0x0100000000000000

#define TASK_STATE_MASK             0xFF00000000000000
#define TASK_INDEX_MASK             0x00FFFFFFFFFFFFFF


#pragma pack(push,1)
typedef struct __CONTEXT_STRUCT
{
    //RegisterData
    QWORD Registers[CONTEXT_REGISTER_COUNT];
} CONTEXT;

//Include Linked List Header.
typedef struct __TCB_STRUCT
{
    LLIST_NODE_HEADER   list_header;
    QWORD               Flags;
    CONTEXT             Context;

    void*               StackAddress;
    QWORD               StackSize;
} TCB;  

//Management TCB Poll
typedef struct __TCB_POOL_MANAGER
{
    TCB* StartAddress;
    int MaxCount;
    int Count;

    int AllocatedCount;
}TCB_POOL_MANAGER;

typedef struct __SchedulerStruct
{
    TCB* Current_Runing_Task;

    int CpuTime;
    
    LLIST task_list;
}SCHEDULER;

#pragma pack(pop)

/// TASK POOL & TASK 


void InitTask(TCB* _Tcb, QWORD _Flags, QWORD _EntryPointAddress, void* _StackAddress, QWORD _StackSize);
void InitializeTCBPool();
TCB* AllocateTCB();
void FreeTCB(QWORD _ID);
TCB* CreateTask(QWORD _Flags, QWORD _EntryPointAddress);


//Link Assembly File
void ContextSwitch(CONTEXT* _CurrentContext, CONTEXT* _NextContext);
#endif /*__TASK_H__*/