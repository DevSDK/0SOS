#include "Flags.h"
#include <Interrupt/Interrupt.h>
BOOL SetInterruptFlag(BOOL _flag)
{
    QWORD flag;
    flag = ReadFlags();
    if(_flag == TRUE)
        EnableInterrupt();
    else
        DisableInterrupt();

    if(flag & 0x0200)
        return TRUE;
    else
        return FALSE;
}