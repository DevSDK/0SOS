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


#define CONSOLE_BACKGROUND_BLACK    0x00
#define CONSOLE_BACKGROUND_BLUE     0x10
#define CONSOLE_BACKGROUND_GREEN    0x20
#define CONSOLE_BACKGROUND_CYAN     0x30
#define CONSOLE_BACKGROUND_RED      0x40
#define CONSOLE_BACKGROUND_MAGENTA  0x50
#define CONSOLE_BACKGROUND_BROWN    0x60
#define CONSOLE_BACKGROUND_WHITE    0x70
#define CONSOLE_BACKGROUND_BLINK    0x80


#define CONSOLE_FOREGROUND_DARKBLACK    0x00
#define CONSOLE_FOREGROUND_DARKBLUE     0x01
#define CONSOLE_FOREGROUND_DARKGREEN    0x02
#define CONSOLE_FOREGROUND_DARKCYAN     0x03
#define CONSOLE_FOREGROUND_DARKRED      0x04
#define CONSOLE_FOREGROUND_DARKMAGENTA  0x05
#define CONSOLE_FOREGROUND_DARKBROWN    0x06
#define CONSOLE_FOREGROUND_DARKWHITE    0x07
#define CONSOLE_FOREGROUND_BLACK        0x08
#define CONSOLE_FOREGROUND_BLUE         0x09
#define CONSOLE_FOREGROUND_GREEN        0x0A
#define CONSOLE_FOREGROUND_CYAN         0x0B
#define CONSOLE_FOREGROUND_RED          0x0C
#define CONSOLE_FOREGROUND_MAGENTA      0x0D
#define CONSOLE_FOREGROUND_BROWN        0x0E
#define CONSOLE_FOREGROUND_WHITE        0x0F


#define COLOR_DEFAULT (CONSOLE_BACKGROUND_BLACK|CONSOLE_FOREGROUND_WHITE)

#pragma pack(push, 1)
typedef struct _Struct_ConsoleCursor
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
void __SetConsole_System(CONSOLESYSTEM value);
CONSOLESYSTEM __GetConsole_System();


#endif /*__CONSOLE_H__*/