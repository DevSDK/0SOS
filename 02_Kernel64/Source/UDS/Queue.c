#include "Queue.h"
#include <Utility/Memory.h>
void InitializeQueue(QUEUE* _QD, void* _QueueBuffer, int _MaxDataCount, int _DataSize)
{
    _QD->MaxDataCount = _MaxDataCount;
    _QD->DataSize = _DataSize;
    _QD->MemoryArray = _QueueBuffer;
    _QD->Front = 0;
    _QD->Rear = 0;
    _QD->isLastPut = FALSE;
}
BOOL IsQueueFull(const QUEUE* _QD)
{
    if((_QD->Rear == _QD->Front) && (_QD->isLastPut == TRUE))
        return TRUE;
    return FALSE;
}
BOOL IsQueueEmpty(const QUEUE* _QD)
{
    if((_QD->Rear == _QD->Front) && (_QD->isLastPut == FALSE))
        return TRUE;
    return FALSE;
}

BOOL PushQueue(QUEUE* _QD, const void* _Src)
{
    if(IsQueueFull(_QD))
        return FALSE;
    _MemCpy(((char*)_QD->MemoryArray) + (_QD->DataSize * _QD->Front),  _Src, _QD->DataSize);

    _QD->Front = (_QD->Front + 1) % _QD->MaxDataCount;
    _QD->isLastPut = TRUE;
    return TRUE;
}
BOOL PopQueue(QUEUE* _QD, void* _Dst)
{
    if(IsQueueEmpty(_QD))
        return FALSE;
    _MemCpy(_Dst, (char*)_QD->MemoryArray + (_QD->DataSize * _QD->Rear ) , _QD->DataSize);

    _QD->Rear = (_QD->Rear + 1) % _QD->MaxDataCount;
    _QD->isLastPut = FALSE;

    return TRUE;
}