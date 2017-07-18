
#include "Types.h"
#include "Driver/Keyboard/Keyboard.h"

#include "Driver/Keyboard/PS2Keyboard.h"
#include <Descriptor/IDT.h>
#include <Descriptor/GDT.h>
#include <Descriptor/Descriptor.h>
#include <Driver/PIC/PIC.h>
#include <Interrupt/Interrupt.h>
#include <Console/Console.h>
#include <Console/Shell.h>

void __KERNEL_ENTRY()
{

	_PrintStringXY(5,12, 0x0F,"Initialize GDT........................................");
	InitializeGDTWithTSS();
	LoadGDTR(GDTR_POINTER);
	_PrintStringXY(60,12,0x0A,"[SUCCESS]");
	_PrintStringXY(5,13, 0x0F,"Load TSS Segment .....................................");
	LoadTR(GDT_TSS_SEGMENT);
	_PrintStringXY(60,13,0x0A,"[SUCCESS]");
	_PrintStringXY(5,14, 0x0F,"Initialize IDT .......................................");
	InitializeIDTTables();
	LoadIDTR(IDTR_POINTER);
	_PrintStringXY(60,14,0x0A,"[SUCCESS]");
	_PrintStringXY(5,15, 0x0F,"PIC Driver And Interrupt Service Initalize.............");
	InitializePIC();
	MaskPICInterrupt(0);
	EnableInterrupt();
	_PrintStringXY(60,15,0x0A,"[SUCCESS]");

	_PrintStringXY(5,16, 0x0F,"Keyboard Input Buffer Initialize.......................");
	InitalizeKeyboardBuffer();

	_PrintStringXY(60,16,0x0A,"[SUCCESS]");


	BYTE flags;
	int i = 14;
	char temps[2] = {0,};

	if(PS2ActivationKeyboard() == FALSE)
	{
		_PrintStringXY(5,15, 0x0F,"Keyboard Error.");	
		while(1);
	}

	StartShell();

	
}


