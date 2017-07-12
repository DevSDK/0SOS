#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <Types.h>

#pragma pack(push, 1)

typedef struct _struct_QueueDescriptor
{
    int DataSize;
    int MaxDataCount;

    void* MemoryArray;
    unsigned int Front;
    unsigned int Rear;

    BOOL isLastPut;
} QUEUE;

#pragma pack(pop)

void InitializeQueue(QUEUE* _QD, void* _QueueBuffer, int _MaxDataCount, int _DataSize);
BOOL IsQueueFull(const QUEUE* _QD);
BOOL IsQueueEmpty(const QUEUE* _QD);
BOOL PushQueue(QUEUE* _QD, const void* _Src);
BOOL PopQueue(QUEUE* _QD, void* _Dst);

#endif /*__QUEUE_H__*/