#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <Types.h>
#include <Tasking/Task.h>


void InitializeScheduler();
void SetCurrentRunningTask(TCB* _Tcb);
TCB* GetCurrentRunningTask();
TCB* GetNextTask();
void AddTaskToScheduler(TCB* _Tcb);
void Schedule();
BOOL ScheduleInInterrupt();
void DecreaseProcessorTime();
BOOL IsProcessorTimeExpired();


#endif /*__SCHEDULER_H__*/