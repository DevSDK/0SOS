#include "Console.h"
#include "String.h"
//Output Format Helper Function.


void gotoxy(int _x, int _y)
{
	g_console_system.cursor_offset = CONSOLE_WIDTH * _y + _x;
}





void __UpdateWithCheckConsoleCursor()
{
	if(g_console_system.cursor_offset + 1 >
		CONSOLE_HEIGHT * CONSOLE_WIDTH)	
		__NextScroll();	
	g_console_system.cursor_offset++;	
}

void __putch(char _ch, BYTE _attribute)
{
	_PrintChar_offset(g_console_system.cursor_offset, _attribute, _ch);
	__UpdateWithCheckConsoleCursor();
}


void __PrintOutInteger(long _value, char* _buffer, int _base)
{
	_itoa(_value, _buffer, _base);
	for (int i = 0; _buffer[i] != '\0'; i++)
	{
		__putch(_buffer[i], g_console_system.current_attribute);
	}
}

char* __PrintMemInteger(long _value, char* _dst, int _base)
{
	char Buffer[FORMAT_BUFFER_SIZE];
	_itoa(_value, Buffer, _base);
	int length = __StringLength(Buffer);
	_MemCpy(_dst, Buffer, length);
	return _dst + length;
}


void __VSPrintf(BYTE _type, const void* _dst, char* str, va_list _arg)
{
	char* ptr = str;
	char Buffer[FORMAT_BUFFER_SIZE];
	char* dst = (char*)_dst;
	while (*ptr != '\0')
	{
		char output = *ptr;	
		int value = 0;
		char ch = 0;
		char* str = 0;
		QWORD qvalue = 0;
		if (*ptr == '%')
		{
			ptr++;
			switch (*ptr)
			{	
			case 'd':
				value = (int)(va_arg(_arg, int));
				if (_type == PRINT_OUTPUT)
					__PrintOutInteger(value, Buffer, 10);
				else if (_type == PRINT_MEMORY)
					dst = __PrintMemInteger(value, dst, 10);
				break;
			case 'o':
				value = (int)(va_arg(_arg, int));
				if (_type == PRINT_OUTPUT)
					__PrintOutInteger(value, Buffer, 8);
				else if (_type == PRINT_MEMORY)
					dst = __PrintMemInteger(value, dst, 8);
				break;
			case 'x':
				value = (int)(va_arg(_arg, int));

				if (_type == PRINT_OUTPUT)
					__PrintOutInteger(value, Buffer, 16);
				else if (_type == PRINT_MEMORY)
					dst = __PrintMemInteger(value, dst, 16);
					
				break;
				//case 'f':
				//case 'g':
				//TODO: Floating Point

			case 'p':
				qvalue = (QWORD)(va_arg(_arg, void*));
	
				if (_type == PRINT_OUTPUT)
				{
					__putch('0', g_console_system.current_attribute);
					__putch('x', g_console_system.current_attribute);
					__PrintOutInteger(qvalue, Buffer, 16);
				}
				else if (_type == PRINT_MEMORY)
				{
					dst[0] = '0';
					dst[1] = 'x';
					dst = __PrintMemInteger(qvalue, dst+ 2, 16);
				}
					
				break;
			case 'c':
				ch = (char)(va_arg(_arg,char));
				if (_type == PRINT_OUTPUT)
					__putch(ch, g_console_system.current_attribute);
				else if (_type == PRINT_MEMORY)
				{
					dst[0] = ch;
					dst++;
				}
				break;
			case 's':
				str = (char*)(va_arg(_arg, char*));
				if (_type == PRINT_OUTPUT)
					_Printf(str);
				else if (_type == PRINT_MEMORY)
				{
					_SPrintf(dst, str);
					int len = __StringLength(str);
					dst += len;
				}
				break;
			default:
				break;
			}

		}
		else if (*ptr == '\n')
		{
			if (_type == PRINT_OUTPUT)
			{
				__NextLine();
			}
			else if (_type == PRINT_MEMORY)
			{
				dst[0] = '\n';
				dst++;
			}
		}
		else
		{
			if (_type == PRINT_OUTPUT)
				__putch(*ptr, g_console_system.current_attribute);
			else if (_type == PRINT_MEMORY)
			{
				dst[0] = *ptr;
				dst++;
			}
		}
		ptr++;
	}

	if (_type == PRINT_OUTPUT)
		return;
	else if (_type == PRINT_MEMORY)
	{
		dst[0] = '\0';
		dst++;
	}
}


void _Printf(char* _str, ...)
{
	va_list _arg;
	va_start(_arg, _str);
	__VSPrintf(PRINT_OUTPUT, 0, _str, _arg);
	va_end(_arg);
}

void _SPrintf(void* _dst, char* _str, ...)
{
	va_list _arg;
	va_start(_arg, _str);
	__VSPrintf(PRINT_MEMORY, _dst, _str, _arg);
	va_end(_arg);
}
void _PrintStringXY(int _x, int _y, BYTE _Attribute ,const char* _str)
{
	CHARACTER_MEMORY* Address = ( CHARACTER_MEMORY* ) CONSOLE_VIDEO_MEMORY;
	int i = 0;
	Address+= ( _y * 80 ) + _x;
	
	for ( i = 0; _str[i] != 0; i++)
	{
		Address[i].bCharactor = _str[i];
		Address[i].bAttribute = _Attribute;
		
	}
}
void _PrintChar_offset(int _offset, BYTE _attribute, char _ch)
{
	CHARACTER_MEMORY* Address = ( CHARACTER_MEMORY* ) CONSOLE_VIDEO_MEMORY;
	int i = 0;
	Address+= _offset;
	Address[0].bCharactor = _ch;
	Address[0].bAttribute = _attribute;
}

void __NextLine()
{

	int addoffset =CONSOLE_WIDTH - (g_console_system.cursor_offset%CONSOLE_WIDTH);
	if( g_console_system.cursor_offset + addoffset >= CONSOLE_HEIGHT * CONSOLE_WIDTH)	
		{
			__NextScroll();
			g_console_system.cursor_offset = CONSOLE_WIDTH*(CONSOLE_HEIGHT - 1);
		}
	else
		g_console_system.cursor_offset += addoffset;
}

void __NextScroll()
{
	_MemCpy(CONSOLE_VIDEO_MEMORY, 
		CONSOLE_VIDEO_MEMORY + CONSOLE_WIDTH* sizeof(CHARACTER_MEMORY), 
			(CONSOLE_HEIGHT-1) * CONSOLE_WIDTH * sizeof(CHARACTER_MEMORY)  );

	CHARACTER_MEMORY* target = (CHARACTER_MEMORY*)(CONSOLE_VIDEO_MEMORY) + (CONSOLE_HEIGHT-1) * CONSOLE_WIDTH ;

	for(int i = 0; i <CONSOLE_WIDTH; i++)
		target[i].bCharactor = ' ';

}