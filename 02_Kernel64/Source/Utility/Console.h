#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include <Types.h>
#include <stdarg.h>


#define FORMAT_BUFFER_SIZE 200    

#define PRINT_MEMORY 0x01
#define PRINT_OUTPUT 0x02

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 25

#define CONSOLE_VIDEO_MEMORY    0xB8000

#define COLOR_DEFAULT 0x0F

#pragma pack(push, 1)
typedef struct _Struct_ConsoleCurser
{
    int cursor_offset;
    DWORD current_attribute;
} CONSOLESYSTEM;

typedef struct _Charactor_Struct
{
	BYTE bCharactor;
	BYTE bAttribute;

} CHARACTER_MEMORY;

#pragma pack(pop)

static CONSOLESYSTEM g_console_system = {0,COLOR_DEFAULT};


void _PrintStringXY(int _x, int _y, BYTE _Attribute ,const char* _str);

void _PrintChar_offset(int _offset, BYTE _attribute, char _ch);

void gotoxy(int _x, int _y);

void _Printf(char* _str, ...);
void _SPrintf(void* _dst, char* _str, ...);
char _GetCh();

void __VSPrintf(BYTE _type, const void* _dst, char* str, va_list _arg);
void __NextLine();
void __NextScroll();
#endif /*__CONSOLE_H__*/