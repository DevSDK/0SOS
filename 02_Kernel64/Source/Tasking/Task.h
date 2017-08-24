#ifndef __TASK_H__
#define __TASK_H__

#include <Types.h>

//SS, RSP, RFLAGS, CS, RIP.. 등등 Context
#define CONTEXT_REGISTER_COUNT     25
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

#pragma pack(push,1)
typedef struct __CONTEXT_STRUCT
{
    QWORD Registers[CONTEXT_REGISTER_COUNT];
} CONTEXT;

typedef struct __TCB_STRUCT
{
    CONTEXT Context;
    QWORD ID;
    QWORD Flags;

    void* StackAddress;
    QWORD StackSize;
} TCB;
#pragma pack(pop)

void InitTask(TCB* _Tcb, QWORD _ID, QWORD _Flags, QWORD _EntryPointAddress, void* _StackAddress, QWORD _StackSize);

//Link Assembly File
void ContextSwitch(CONTEXT* _CurrentContext, CONTEXT* _NextContext);
#endif /*__TASK_H__*/