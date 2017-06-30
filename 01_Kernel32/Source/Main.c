#include "Types.h"
#include "Page.h"
#include "SwitchMode.h"
void PrintVideoMemory(int x, int y,BYTE Attribute, const char* _str);
BOOL CheckMemorySize();
BOOL Initalization64KernelMemoryArea();

void __Kernel__Entry()
{
		
	PrintVideoMemory(5,5,0x0F,"Initalization IA-32e Memory Area......................");
	PrintVideoMemory(5,6,0x0F,"Check Memory Size > 64MB .............................");
	
	if(CheckMemorySize())
	{
		PrintVideoMemory(60,6,0x0A,"[SUCCESS]");
	}
	else
	{	
		PrintVideoMemory(60,6,0x0C,"[ERROR]");
		while(1); //Maybe need Error Handler Function 
	}

	if(Initalization64KernelMemoryArea())
	{
		PrintVideoMemory(60,5,0x0A,"[SUCCESS]");
	}
	else
	{
		PrintVideoMemory(60,5,0x0C,"[ERROR]");
		while(1);
	}

	PrintVideoMemory(5,7, 0x0F,"Initalization PML4, PDPT, PD .........................");
	InitializePageTable();
	PrintVideoMemory(60,7,0x0A,"[SUCCESS]");
	
	
	PrintVideoMemory(5,8, 0x0F,"CPU Company .......................................... [            ]");
	
	DWORD out_eax, out_ebx, out_ecx, out_edx;
	GetCPUID(0x00000000, &out_eax, &out_ebx, &out_ecx, &out_edx);

	char CPU_Name[13] = {0};
	*( DWORD*)CPU_Name 		= out_ebx;
	*((DWORD*)CPU_Name+1)	= out_edx;
	*((DWORD*)CPU_Name+2)	= out_ecx;

	PrintVideoMemory(61,8, 0x0F, CPU_Name);
		
	GetCPUID(0x80000001, &out_eax, &out_ebx, &out_ecx, &out_edx);
	PrintVideoMemory(5,9, 0x0F,"Check CPU Support 64Bit ..............................");
	if(out_edx & ( 1 << 29))
	{	
		PrintVideoMemory(60,9,0x0A,"[SUCCESS]");
	}
	else
	{	
		PrintVideoMemory(60,9,0x0C,"[ERROR]");
		while(1);
	}
	
	PrintVideoMemory(5,10, 0x0F,"Now 64 Bit Mode.");
	ModeSwitchAndJumpKernel64();
		
	while(1);
}

void PrintVideoMemory(int _x, int _y, BYTE _Attribute ,const char* _str)
{
	CHARACTER_MEMORY* Adress = ( CHARACTER_MEMORY* ) 0xB8000;
	
	int i = 0;
	
	Adress+= ( _y * 80 ) + _x;

	for ( i = 0; _str[i] != 0; i++)
	{
		Adress[i].bCharactor = _str[i];
		Adress[i].bAttribute = _Attribute;
	
	}
}

BOOL Initalization64KernelMemoryArea()
{
	DWORD* Adress = (DWORD*) 0x100000;
	
	while((DWORD) Adress < 0x600000)
	{
		*Adress = 0x00;

		if( *Adress != 0 )
			return FALSE;
		Adress++;
	}
	
		
	return TRUE;
}


BOOL CheckMemorySize()
{
	DWORD* Adress = (DWORD*) 0x100000;
	
	while( (DWORD) Adress< 0x4000000 )
	{
		*Adress = 0x12345678;

		if( *Adress != 0x12345678)
			return FALSE;

		Adress += (0x100000 / 4);
	}
	return TRUE;
	
}

