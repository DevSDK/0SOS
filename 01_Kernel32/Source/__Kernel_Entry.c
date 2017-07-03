#include "Types.h"
#include "Page.h"
#include "SwitchMode.h"
void PrintVideoMemory(int x, int y,BYTE Attribute, const char* _str);
BOOL CheckMemorySize();
BOOL Initalization64KernelMemoryArea();
void LoadKernel64ImageToMemory(DWORD _address);

void __KERNEL_ENTRY()
{
		
	PrintVideoMemory(5,5,0x0F,"Initalization IA-32e Memory Area......................");
	
	if(Initalization64KernelMemoryArea())
	{
		PrintVideoMemory(60,5,0x0A,"[SUCCESS]");
	}
	else
	{
		PrintVideoMemory(60,5,0x0C,"[ERROR]");
		while(1);
	}

	
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


	LoadKernel64ImageToMemory(0x200000);	
	
	ModeSwitchAndJumpKernel64();

		
	while(1);
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

BOOL Initalization64KernelMemoryArea()
{
	DWORD* Address = (DWORD*) 0x100000;
	
	while((DWORD) Address < 0x600000)
	{
		*Address = 0x00;

		if( *Address != 0 )
			return FALSE;
		Address++;
	}
	
		
	return TRUE;
}


BOOL CheckMemorySize()
{
	DWORD* Address = (DWORD*) 0x100000;
	
	while( (DWORD) Address< 0x4000000 )
	{
		*Address = 0x12345678;

		if( *Address != 0x12345678)
			return FALSE;

		Address += (0x100000 / 4);
	}
	return TRUE;
	
}

void LoadKernel64ImageToMemory(DWORD _address)
{
	WORD TotalKernelSector 	= *( (WORD*) 0x7C05);
	WORD Kernel32Sector 	= *( (WORD*) 0x7C07);
	
	DWORD* SourceAddress = ((DWORD*) (0x10000 + Kernel32Sector * 512));
	DWORD* DestAddress	= (DWORD*) 0x200000; 
	
	for(int i=0; i< 512 * (TotalKernelSector -Kernel32Sector) / 4; i++)
	{
		*DestAddress = *SourceAddress;
		DestAddress++;
		SourceAddress++;
	}

	
}
