
#include "Types.h"
#include "Driver/Keyboard/Keyboard.h"

#include "Driver/Keyboard/PS2Keyboard.h"
#include <Descriptor/IDT.h>
#include <Descriptor/GDT.h>
#include <Descriptor/Descriptor.h>
#include <Utility/Console.h>
#include <Driver/PIC/PIC.h>
#include <Interrupt/Interrupt.h>
#include <Utility/Console.h>

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
	
	_Printf("Test %d \n", 100);

	_Printf("Hello My Name is Seok Ho Song\n", 100);

	char buf[200];
	_SPrintf(buf, "\n \n This is Sprintf it Work! \n yes! %d",10);

	_Printf("%s",buf);

	for(int i = 0; i< 30; i++)
		_Printf("Scroll Test %d\n",i);

	_Printf("\n",buf);
	_Printf("t\n",buf);


	KEYDATA keydata;
	while(1)
	{
			if(GetKeyData(&keydata) == TRUE)
			{
				if(keydata.Flags & KEY_DOWN)
				{
					temps[0] = keydata.ASCIICode;
					_PrintStringXY(i++, 17, 0x0C, temps);
				}
			}


		
	}
	
}


