#include "Types.h"


void PrintVideoMemory(int x, int y, const char* _str);


void __Kernel__Entry()
{

	PrintVideoMemory(0,3, "Now C Language Binary.");
	
	while(1);
}

void PrintVideoMemory(int x, int y, const char* _str)
{
	CHARACTER_MEMORY* Adress = ( CHARACTER_MEMORY* ) 0xB8000;
	
	int i = 0;
	
	Adress+= ( y * 80 ) + x;

	for ( i = 0; _str[i] != 0; i++)
	{
		Adress[i].bCharactor = _str[i];
	}


}
