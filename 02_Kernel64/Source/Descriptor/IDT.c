#include "IDT.h"
#include <Interrupt/InterruptService.h>
void InitializeIDTTables()
{
    IDTR* idtr = (IDTR*) IDTR_POINTER;
    IDT_ENTRY* entry =  (IDT_ENTRY*)(IDTR_POINTER + sizeof(IDTR));
    
    idtr->BaseAddress   = (QWORD)entry;
    idtr->Size          = IDT_TABLE_SIZE - 1;

    SetIDTEntry(&entry[0],ISRDividError,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[1],ISRDebug,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[2],ISRNMI,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[3],ISRBreakPoint,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[4],ISROverflow,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[5],ISRBoundRangeExceeded,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[6],ISRInvalidOpCode,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[7],ISRDeviceNotAvailable,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[8],ISRDoubleFault,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[9],ISRCoProcessorSegmentOverrun,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[10],ISRInvalidTSS,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[11],ISRSegmentNotPresent,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[12],ISRStackSegmentFault,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[13],ISRGeneralProtection,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[14],ISRPageFault,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[15],ISR15,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[16],ISRFloatingPointError,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[17],ISRAlignmentCheck,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[18],ISRMachineCheck,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[19],ISRSIMDError,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[20],ISRDefaultException,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);

    for(int i= 21; i < 32 ; i++)
    {
        SetIDTEntry(&entry[i],ISRDefaultException,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    }

    SetIDTEntry(&entry[32],ISRTimer,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[33],ISRPS2Keyboard,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[34],ISRSlavePIC,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[35],ISRSerialPort2,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[36],ISRSerialPort1,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[37],ISRParallel2,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[38],ISRFloppy,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[39],ISRParallel1,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[40],ISRRTC,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[41],ISRReserved,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[42],ISRNotUsed1,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[43],ISRNotUsed2,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[44],ISRPS2Mouse,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[45],ISRCoprocessor,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[46],ISRHDD1,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    SetIDTEntry(&entry[47],ISRHDD2,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    
    for(int i = 48; i < IDT_MAX_ENTRY_COUNT; i++)
    {
        SetIDTEntry(&entry[i],ISRDefaultInterrupt,0x08, IDT_ENTRY_IST1, IDT_ENTRY_KERNEL, IDT_TYPE_INTERRUPT);
    }
}
void SetIDTEntry(IDT_ENTRY* _entry, void* _handler, WORD _Selector, 
                    BYTE _IST, BYTE _Flags, BYTE _Type)
{
    _entry->Low_BaseAddress =  (QWORD) _handler & 0xFFFF;
    _entry->SegmentSelector = _Selector;
    _entry->IST             = _IST & 0x3;
    _entry->FlagsAndType   = _Flags | _Type;
    _entry->Mid_BaseAddress = ((QWORD)_handler >> 16) & 0xFFFF;
    _entry->High_BaseAddres = ((QWORD)_handler >> 32);
    _entry->Reserved        = 0;
}
