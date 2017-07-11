
#include "Types.h"
#include "Driver/Keyboard/Keyboard.h"

#include "Driver/Keyboard/PS2Keyboard.h"
#include <Descriptor/IDT.h>
#include <Descriptor/GDT.h>
#include <Descriptor/Descriptor.h>
#include <Utility/Console.h>
#include <Driver/PIC/PIC.h>
#include <Interrupt/Interrupt.h>


void __KERNEL_ENTRY()
{

	PrintVideoMemory(5,12, 0x0F,"Initialize GDT........................................");
	InitializeGDTWithTSS();
	LoadGDTR(GDTR_POINTER);
	PrintVideoMemory(60,12,0x0A,"[SUCCESS]");
	PrintVideoMemory(5,13, 0x0F,"Load TSS Segment .....................................");
	LoadTR(GDT_TSS_SEGMENT);
	PrintVideoMemory(60,13,0x0A,"[SUCCESS]");
	PrintVideoMemory(5,14, 0x0F,"Initialize IDT .......................................");
	InitializeIDTTables();
	LoadIDTR(IDTR_POINTER);
	PrintVideoMemory(60,14,0x0A,"[SUCCESS]");
	PrintVideoMemory(5,15, 0x0F,"PIC Driver And Interrupt Service Initalize.............");
	InitializePIC();
	MaskPICInterrupt(0);
	EnableInterrupt();
	PrintVideoMemory(60,15,0x0A,"[SUCCESS]");
	
	BYTE flags;
	int i = 14;
	char temps[2] = {0,};

	if(PS2ActivationKeyboard() == FALSE)
	{
		PrintVideoMemory(5,15, 0x0F,"Keyboard Error.");	
		while(1);
	}
	
	
	while(1)
	{
		if(PS2CheckOutputBufferNotEmpty() == TRUE)
		{

			BYTE temp = PS2GetKeyboardScanCode();
			if(ConvertScancodeToASCII( temp, &temps[0], &flags) == TRUE)
				if(flags & KEY_DOWN )
				{
					PrintVideoMemory(i++, 16, 0x0C, temps);
				
				}
			
		
		}


		
	}
	
}


