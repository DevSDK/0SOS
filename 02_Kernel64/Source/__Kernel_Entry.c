
#include "Types.h"

void PrintVideoMemory(int _x, int _y, BYTE _Attribute, const char* _str);

void __KERNEL_ENTRY()
{
	PrintVideoMemory(5,12, 0x0F,"64 bit C Language Kernel.");	
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

