#include "Memory.h"



void _MemSet(void* _Dst, BYTE _Data, int _Size)
{
    for(int i =0; i< _Size; i++)
    {
        ((char*)_Dst)[i] = _Data;
    }
}
int  _MemCpy(void* _Dst, const void* _Src, int _Size)
{
    for(int i = 0; i< _Size; i++)
    {
        ((char*)_Dst)[i]= ((char*)_Src)[i];
    }
}
int  _MemCmp(const void* _Dst, const void* _Src, int _Size)
{
    for(int i =0; i<_Size; i++)
    {
        char tmp =  ((char*)_Dst)[i] - ((char*)_Src)[i];
        
        if(tmp != 0)
            return tmp;
    }
    return 0;
}
