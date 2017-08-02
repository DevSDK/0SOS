#include "PIT.h"
#include <Driver/IO/PortIO.h>

void InitializePIT(WORD _Count, BOOL _IsInterval)
{
    //만약 주기적으로 실행되어야 한다면 모드 2 아니면 모드 0
    if(_IsInterval == TRUE)
        PortIO_OutByte(PIT_PORT_CONTROL, PIT_COUNTER0_FLAG_INTERVAL);
    else
        PortIO_OutByte(PIT_PORT_CONTROL, PIT_COUNTER0_FLAG_ONCE);
    //카운터의 하위 비트와 상위 비트를 설정
    PortIO_OutByte(PIT_PORT_COUNTER0, _Count);
    PortIO_OutByte(PIT_PORT_COUNTER0, _Count >> 8);
}

WORD ReadTimerCount0()
{
    BYTE high, low;
    WORD ret = 0x0000;
    //Counter 0 의 데이터를 읽어온다.
    PortIO_OutByte(PIT_PORT_CONTROL, PIT_COUNTER0_FLAG_LATCH);
    low = PortIO_InByte(PIT_PORT_COUNTER0);
    high = PortIO_InByte(PIT_PORT_COUNTER0);
    //상위 비트 하위 비트 묶어서 2바이트로 변환
    ret = high;
    ret = (ret << 8) | low;
    return ret;
}

void WaitUsingPITCounter0(WORD _Count)
{
    //0~0xFFFF 까지 반복

    WORD CurrentCounter = 0;
    InitializePIT(0, TRUE);
    const WORD LastCounter0 = ReadTimerCount0();
    while(TRUE)
    {
        //Get Counter 0 Value
        CurrentCounter = ReadTimerCount0();
        //시간이 흐름에 따라 값이 커짐  
        if(((LastCounter0 - CurrentCounter) & 0xFFFF) >= _Count)
            break;    
    }

}