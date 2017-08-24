#include "Console.h"
#include <Utility/String.h>
#include <Driver/Keyboard/Keyboard.h>
#include <Driver/VGA/IO_VGA.h>
//Output Format Helper Function.

void __SetConsole_System(CONSOLESYSTEM value)
{
	g_console_system.cursor_offset = value.cursor_offset;
	g_console_system.current_attribute = value.current_attribute;	
}

CONSOLESYSTEM __GetConsole_System()
{	
	return g_console_system;
}

//커서를 업데이트 하고, 스크롤 체크를 함
void __UpdateWithCheckConsoleCursor()
{
	if(g_console_system.cursor_offset + 1 >=
		CONSOLE_HEIGHT * CONSOLE_WIDTH)	
		{
			__NextScroll();	
			g_console_system.cursor_offset = CONSOLE_WIDTH*(CONSOLE_HEIGHT - 1);	
		}
		else
		{
			g_console_system.cursor_offset++;	
		}
}
//한 글자 출력
void __putch(char _ch, BYTE _attribute)
{
	_PrintChar_offset(g_console_system.cursor_offset, _attribute, _ch);
	__UpdateWithCheckConsoleCursor();
}
//숫자 출력(_base에 따라 다름)
void  __PrintOutInteger(long _value, char* _buffer, int _base)
{
	_itoa(_value, _buffer, _base);
	for (int i = 0; _buffer[i] != '\0'; i++)
	{
		__putch(_buffer[i], g_console_system.current_attribute);
	}
}

void  __U_PrintOutInteger(QWORD _value, char* _buffer, unsigned int _base)
{
	_u_itoa(_value, _buffer, _base);
	for (int i = 0; _buffer[i] != '\0'; i++)
	{
		__putch(_buffer[i], g_console_system.current_attribute);
	}
}

char* __U_PrintMemInteger(QWORD _value, char* _dst, int _base)
{
	char Buffer[FORMAT_BUFFER_SIZE];
	_u_itoa(_value, Buffer, _base);
	int length = __StringLength(Buffer);
	_MemCpy(_dst, Buffer, length);
	return _dst + length;

} 

//메모리 대상 숫자 출력
char* __PrintMemInteger(long _value, char* _dst, int _base)
{
	char Buffer[FORMAT_BUFFER_SIZE];
	_itoa(_value, Buffer, _base);
	int length = __StringLength(Buffer);
	_MemCpy(_dst, Buffer, length);
	return _dst + length;

} 

//_type에 따라 동작하는 Printf 함수
void __VSPrintf(BYTE _type, const void* _dst, char* str, va_list _arg)
{
	char* ptr = str;
	char Buffer[FORMAT_BUFFER_SIZE];
	char* dst = (char*)_dst;
	//입력받은 문자열의 끝까지 반복
	while (*ptr != '\0')
	{
		char output = *ptr;	
		int value = 0;
		char ch = 0;
		char* str = 0;
		QWORD qvalue = 0;
		long ldate = 0;
		//%토큰을 만나면
		if (*ptr == '%')
		{
			ptr++;
			//뒤에 나오는 문자에 따라 다른 처리(_type에 따라 다름)
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

			case 'q':
				qvalue = (QWORD)(va_arg(_arg, QWORD));
				if (_type == PRINT_OUTPUT)
					__U_PrintOutInteger(qvalue, Buffer, 10);
				else if (_type == PRINT_MEMORY)
					dst = __U_PrintMemInteger(qvalue, dst, 10);
				break;
				
				//case 'f':
				//case 'g':
				//TODO: Floating Point

			case 'p':
				qvalue = (QWORD)(va_arg(_arg, void*));
				//포인터의 경우 0x를 붙임
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
				ch = (char)(va_arg(_arg,int));
				if (_type == PRINT_OUTPUT)
					__putch(ch, g_console_system.current_attribute);
				else if (_type == PRINT_MEMORY)
				{
					dst[0] = ch;
					dst++;
				}
				break;
			case 's':
				//입력받은 문자열에 따른 또다른 포멧 출력을 위해
				// 타입에 따라 함수 재 호출
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
		else if (*ptr == '\t') //\t 처리
		{
			if(_type == PRINT_OUTPUT)
				g_console_system.cursor_offset += (8 - g_console_system.cursor_offset % 8);
			else if(_type == PRINT_MEMORY)
			{
				dst[0] = '\t';
				dst++;
			}
 		}
		else if (*ptr == '\n')//\n 처리
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
		//특수문자가 아니라면, 일반적인 출력임
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
	_SetCursor(g_console_system.cursor_offset % CONSOLE_WIDTH, 
				g_console_system.cursor_offset / CONSOLE_WIDTH);
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
	Address+= _offset;
	Address[0].bCharactor = _ch;
	Address[0].bAttribute = _attribute;
}

void __NextLine()
{
	int addoffset =CONSOLE_WIDTH - (g_console_system.cursor_offset%CONSOLE_WIDTH);
	//커서의 위치가 화면 밖으로 넘어간다면
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
	//비디오 메모리를 한줄 밈
	_MemCpy(CONSOLE_VIDEO_MEMORY, 
		CONSOLE_VIDEO_MEMORY + CONSOLE_WIDTH* sizeof(CHARACTER_MEMORY), 
			(CONSOLE_HEIGHT-1) * CONSOLE_WIDTH * sizeof(CHARACTER_MEMORY)  );

	CHARACTER_MEMORY* target = (CHARACTER_MEMORY*)(CONSOLE_VIDEO_MEMORY) + (CONSOLE_HEIGHT-1) * CONSOLE_WIDTH ;

	for(int i = 0; i <CONSOLE_WIDTH; i++)
		target[i].bCharactor = ' ';

}


char _GetCh()
{
	KEYDATA keydata;
	while(1)
	{
		if(GetKeyData(&keydata) == TRUE)
		{
			if(keydata.Flags & KEY_DOWN)
			{
				return keydata.ASCIICode;
			}
		}
	}
}
