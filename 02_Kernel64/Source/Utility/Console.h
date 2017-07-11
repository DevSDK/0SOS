#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include <Types.h>

#define CONSOLE_VIDEO_MEMORY    0xB8000


void PrintVideoMemory(int _x, int _y, BYTE _Attribute ,const char* _str);



#endif /*__CONSOLE_H__*/