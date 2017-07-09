#include "GDT.h"
#include "IDT.h"
#include <Utility/Memory.h>

void InitializeGDTWithTSS()
{
    GDTR* _gdtr = (GDTR*)GDTR_POINTER;

    GDT_ENTRY8* _gdt_entry      = (GDT_ENTRY8*)(GDTR_POINTER + sizeof(GDTR)); 
    TSS_SEGMENT* _tss_segment    = (TSS_SEGMENT*)((QWORD)_gdt_entry+GDT_TABLE_SIZE);    
    _gdtr->Size         = GDT_TABLE_SIZE - 1;
    _gdtr->BaseAddress  = (QWORD)_gdt_entry;

    SetGDT_Entry8((&_gdt_entry[0]), 0, 0, 0, 0, 0);
    SetGDT_Entry8((&_gdt_entry[1]),0,0xFFFFF, GDT_ENTRY_HIGH_CODE, GDT_ENTRY_LOW_KERNEL_CODE, GDT_TYPE_CODE);
    SetGDT_Entry8((&_gdt_entry[2]),0,0xFFFFF, GDT_ENTRY_HIGH_DATA, GDT_ENTRY_LOW_KERNEL_DATA, GDT_TYPE_DATA);
    
    SetGDT_Entry16(((GDT_ENTRY16*)(&_gdt_entry[3])), (QWORD)_tss_segment, sizeof(TSS_SEGMENT)-1, GDT_ENTRY_HIGH_TSS,
                    GDT_ENTRY_LOW_TSS, GDT_TYPE_TSS);

    InitializeTSSSegment(_tss_segment);
    
}
void SetGDT_Entry8(GDT_ENTRY8* _entry, DWORD _BaseAddress,
                     DWORD _Size, BYTE _HighFlags, BYTE _LowFlags, BYTE _Type)
{
    _entry->Low_Size            =   _Size & 0xFFFF;
    _entry->Low_BaseAddress     = _BaseAddress & 0xFFFF;
    _entry->Low_BaseAddress1    = ( _BaseAddress >> 16 ) & 0xFF;
    _entry->Low_Flags           = _LowFlags | _Type;
    _entry->High_FlagsAndSize   = ((_Size>>16) & 0xFF) | _HighFlags;
    _entry->High_BaseAddress    = (_BaseAddress>>24) & 0xFF;
}

void SetGDT_Entry16(GDT_ENTRY16* _entry, QWORD _BaseAddress,
                     DWORD _Size, BYTE _HighFlags, BYTE _LowFlags, BYTE _Type)
{
    _entry->Low_Size            = _Size & 0xFFFF;
    _entry->Low_BaseAddress     = _BaseAddress & 0xFFFF;
    _entry->Mid_BaseAddress     = (_BaseAddress >> 16 ) & 0xFF;
    _entry->Low_Flags           = _LowFlags | _Type;
    _entry->High_FlagsAndSize   = ((_Size >> 16) & 0xFF) | _HighFlags;
    _entry->High_BaseAddress    = (_BaseAddress  >> 24) & 0xFF;
    _entry->High_BaseAddress2   = (_BaseAddress>>32);
    _entry->Reserved            = 0;
}

void InitializeTSSSegment(TSS_SEGMENT* _tss)
{
    _MemSet(_tss, 0, sizeof(TSS_SEGMENT));    
    _tss->IST[0] = IST_POINTER + IST_SIZE;
    _tss->IOMapBaseAddress  =  0xFFFF;
}