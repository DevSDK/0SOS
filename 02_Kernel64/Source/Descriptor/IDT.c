#include "IDT.h"

void InitializeIDTTables()
{
    IDTR* idtr = (IDTR*) IDTR_POINTER;
    IDT_ENTRY* entry =  (IDT_ENTRY*)IDTR_POINTER + sizeof(IDTR);
    
    idtr->BaseAddress   = (QWORD)entry;
    idtr->Size          = IDT_TABLE_SIZE - 1;

    for(int i = 0; i < IDT_MAX_ENTRY_COUNT; i++) 
    {
        SetIDTEntry(&entry[i],DummyHandler,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    }

}
void SetIDTEntry(IDT_ENTRY* _entry, void* _handler, WORD _Selector, 
                    BYTE _IST, BYTE _Flags, BYTE _Type)
{
    _entry->Low_BaseAddress =  (QWORD) _handler;
    _entry->SegmentSelector = _Selector;
    _entry->IST             = _IST;
    _entry->FlagsAndType   = _Flags | _Type;
    _entry->Mid_BaseAddress = ((QWORD)_handler >> 16) & 0xFFFF;
    _entry->High_BaseAddres = ((QWORD)_handler >> 32);
    _entry->Reserved        = 0;
}

void Pt(int _x, int _y, BYTE _Attribute ,const char* _str)
{
    CHARACTER_MEMORY* Address = ( CHARACTER_MEMORY* ) 0xB8000;
	
	int i = 0;
	
	Address+= ( _y * 80 ) + _x;
	
	for ( i = 0; _str[i] != 0; i++)
	{
		Address[i].bCharactor = _str[i];
		Address[i].bAttribute = _Attribute;
		
	}
}
void DummyHandler()
{
    Pt(0,0,0x0C ,"===============================");
    Pt(0,1,0x0C ,"===============================");
    Pt(0,2,0x0C ,"===    Test Interrupt Hander===");
    Pt(0,3,0x0C ,"===============================");
    Pt(0,4,0x0C ,"===============================");

    while(1);

}