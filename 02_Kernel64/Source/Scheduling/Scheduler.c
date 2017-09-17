#include "Scheduler.h"
#include <Tasking/Task.h>
#include <Interrupt/Interrupt.h>
#include <Descriptor/IDT.h>
#include <Utility/Memory.h>
#include <Console/Console.h>
static SCHEDULER g_Scheduler;
void InitializeScheduler()
{
    InitializeTCBPool();
    InitializeLList(&(g_Scheduler.task_list));
    g_Scheduler.Current_Runing_Task = AllocateTCB();
}
void SetCurrentRunningTask(TCB* _Tcb)
{
    g_Scheduler.Current_Runing_Task = _Tcb;
}
TCB* GetCurrentRunningTask()
{
    return g_Scheduler.Current_Runing_Task;
}
TCB* GetNextTask()
{
    if(g_Scheduler.task_list.Count == 0)
        return NULL;

    return (TCB*) Remove_Front_LList(&g_Scheduler.task_list);
}
void AddTaskToScheduler(TCB* _Tcb)
{
    Push_Back_LList( &(g_Scheduler.task_list), _Tcb);
}   
void Schedule()
{
    if(g_Scheduler.task_list.Count == 0)
        return;
    BOOL interrupt_status = SetInterruptFlag(FALSE);
    TCB* task = GetNextTask(); 
    if(task == NULL)
    {
        SetInterruptFlag(interrupt_status);
        return;
    }
    
    TCB* pre_task = g_Scheduler.Current_Runing_Task;
    AddTaskToScheduler(pre_task);   
    
    g_Scheduler.Current_Runing_Task = task;
    ContextSwitch(&(pre_task->Context), &(task->Context));
    g_Scheduler.CpuTime = TASK_TIME;
       
    SetInterruptFlag(interrupt_status);

}

BOOL ScheduleInInterrupt()
{
    TCB* task = GetNextTask();
    if(task == NULL)
    {
        return FALSE;
    }

    
 
    void* ContextAddress = IST_POINTER + IST_SIZE - sizeof(CONTEXT);

    TCB* running_task = g_Scheduler.Current_Runing_Task;
    _MemCpy(&(running_task->Context), ContextAddress, sizeof(CONTEXT));
    g_Scheduler.Current_Runing_Task = task;
    AddTaskToScheduler(running_task);
 
    _MemCpy(ContextAddress, &(task->Context),sizeof(CONTEXT));
    g_Scheduler.CpuTime = TASK_TIME;



    return TRUE;
}
void DecreaseProcessorTime()
{
    if(g_Scheduler.CpuTime > 0)
        g_Scheduler.CpuTime--;
}
BOOL IsProcessorTimeExpired()
{
    if(g_Scheduler.CpuTime  <= 0)
        return TRUE;
    return FALSE;   
}
