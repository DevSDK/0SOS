#include "IO_VGA.h"
#include <Console/Console.h>

void _SetCursor(int _x, int _y)
{
    int offset = _y * CONSOLE_WIDTH + _x;
    PortIO_OutByte(VGA_PORT_INDEX, VGA_INDEX_HIGH_CURSOR);
    PortIO_OutByte(VGA_PORT_DATA, offset >> 8);

    PortIO_OutByte(VGA_PORT_INDEX, VGA_INDEX_LOW_CURSOR);
    PortIO_OutByte(VGA_PORT_DATA, offset & 0xFF);
    
    CONSOLESYSTEM console = __GetConsole_System();
    console.cursor_offset = offset;
    __SetConsole_System(console);
}

void _GetCursor(int* _x_out, int* _y_out)
{
    CONSOLESYSTEM console = __GetConsole_System();
    *_x_out = console.cursor_offset % CONSOLE_WIDTH;
    *_y_out = console.cursor_offset / CONSOLE_WIDTH;
}