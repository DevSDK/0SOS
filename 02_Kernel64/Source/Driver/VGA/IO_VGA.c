#include "IO_VGA.h"
#include <Console/Console.h>

//Port IO 메모리 맵핑을 이용해 커서의 위치를 지정한다.
void _SetCursor(int _x, int _y)
{
    int offset = _y * CONSOLE_WIDTH + _x;
    PortIO_OutByte(VGA_PORT_INDEX, VGA_INDEX_HIGH_CURSOR);
    //2바이트 출력을 위해 상위 비트, 하위 비트 나눠서 저장
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