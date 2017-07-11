#ifndef __IDT_H__
#define __IDT_H__
#include "GDT.h"

#define IDT_TYPE_INTERRUPT      0x0E    //0b00001110
#define IDT_TYPE_TRAP           0x0F    //0b00001111
#define IDT_ENTRY_DPL0          0x00    //0b00000000
#define IDT_ENTRY_DPL1          0x20    //0b00100000
#define IDT_ENTRY_DPL2          0x40    //0b01000000
#define IDT_ENTRY_DPL3          0x60    //0b01100000
#define IDT_ENTRY_P             0x80    //0b10000000
#define IDT_ENTRY_IST1          1
#define IDT_ENTRY_IST0          0

#define IDT_ENTRY_KERNEL    (IDT_ENTRY_DPL0 | IDT_ENTRY_P)
#define IDT_ENTRY_USER      (IDT_ENTRY_DPL3 | IDT_ENTRY_P)

#define IDT_MAX_ENTRY_COUNT     100
#define IDTR_POINTER            (sizeof(GDTR) + GDTR_POINTER \
                                + GDT_TABLE_SIZE + TSS_SEGMENT_SIZE)
#define IDT_POINTER            (sizeof(IDTR) + IDTR_POINTER)
#define IDT_TABLE_SIZE          (IDT_MAX_ENTRY_COUNT * sizeof(IDT_ENTRY))


#define IST_POINTER         0x700000
#define IST_SIZE            0x100000


#pragma pack(push, 1)

typedef struct _Struct_IDT_Entry
{
    WORD Low_BaseAddress;
    WORD SegmentSelector;

    BYTE IST;
    //3Bit IST, 5Bit set 0
    BYTE FlagsAndType;
    // 4Bit Type, 1 Bit set 0, 2Bit DPL, 1Bit P
    WORD Mid_BaseAddress;
    DWORD High_BaseAddres;
    DWORD Reserved;

}IDT_ENTRY;

#pragma pack(pop)

void InitializeIDTTables();
void SetIDTEntry(IDT_ENTRY* _entry, void* _handler, WORD _Selector, 
                    BYTE _IST, BYTE _Flags, BYTE _Type);


void Pt(int _x, int _y, BYTE _Attribute ,const char* _str);

void DummyHandler();

#endif /* __IDT_H__ */