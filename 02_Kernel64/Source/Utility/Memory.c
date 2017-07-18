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


static QWORD g_TotalRAM_MBSIze = 0;

void __InitializeMemoryCheck()
{
        DWORD* current_address = (DWORD*) 0x4000000;
        DWORD* previous_value;
        while(1)
        {
            previous_value = *current_address;
            *current_address = 0x12345678;
            if(*current_address != 0x12345678)
                break;
            *current_address = previous_value;
            current_address += 0x400000 / 4;

        }

        g_TotalRAM_MBSIze = (QWORD) current_address / 0x100000;
}

QWORD __GetTotalRamSize()
{
    return g_TotalRAM_MBSIze;
}