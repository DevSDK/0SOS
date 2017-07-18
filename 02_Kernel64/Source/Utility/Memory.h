#ifndef __UTILITY_MEMORY_H__
#define __UTILITY_MEMORY_H__
#include <Types.h>
void _MemSet(void* _Dst, BYTE _Data, int _Size);
int  _MemCpy(void* _Dst, const void* _Src, int _Size);
int  _MemCmp(const void* _Dst, const void* _Src, int _Size);

void __InitializeMemoryCheck();
QWORD __GetTotalRamSize();
#endif /*__UTILITY_MEMORY_H__*/