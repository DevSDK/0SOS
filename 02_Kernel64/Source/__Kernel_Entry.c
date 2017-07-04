
#include "Types.h"
#include "Driver/Keyboard/Keyboard.h"

#include "Driver/Keyboard/PS2Keyboard.h"
void PrintVideoMemory(int _x, int _y, BYTE _Attribute, const char* _str);




void __KERNEL_ENTRY()
{

	PrintVideoMemory(5,12, 0x0F,"64 bit C Language Kernel.");	
	BYTE flags;
	int i = 14;
	char temps[2] = {0,};

	if(PS2ActivationKeyboard() == FALSE)
	{
		PrintVideoMemory(5,15, 0x0F,"Keyboard Error	.");	
		while(1);
	}
	
	
	while(1)
	{
		if(PS2CheckOutputBufferNotEmpty() == TRUE)
		{

			BYTE temp = PS2GetKeyboardScanCode();
			if(ConvertScancodeToASCII( temp, &temps[0], &flags) == TRUE)
				if(flags & KEY_DOWN )
					PrintVideoMemory(i++, 15, 0x0C, temps);
			
		
		}


		
	}
	
}




void PrintVideoMemory(int _x, int _y, BYTE _Attribute ,const char* _str)
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

void EnableA20Gate()
{

}

