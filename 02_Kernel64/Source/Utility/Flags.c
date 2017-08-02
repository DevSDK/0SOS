#include "Flags.h"
#include <Interrupt/Interrupt.h>
//인터럽트를 _flag로 설정하면서, 원래의 인터럽트
//설정 여부를 반환
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