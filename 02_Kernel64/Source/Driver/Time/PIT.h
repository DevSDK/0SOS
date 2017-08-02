#ifndef __PIT_H__
#define __PIT_H__

#include <Types.h>
//PIT의 동작 속도 1.193182MHz 
#define PIT_FREQUENCY 1193182

#define MS_TO_COUNT(x) (PIT_FREQUENCY * (x)/1000)
#define US_TO_COUNT(x) (PIT_FREQUENCY * (x)/1000000)

#define PIT_PORT_CONTROL    0x43
#define PIT_PORT_COUNTER0   0x40
#define PIT_PORT_COUNTER1   0x41
#define PIT_PORT_COUNTER2   0x42

#define PIT_CONTROL_SC_COUNTER0   0x00
#define PIT_CONTROL_SC_COUNTER1   0x40
#define PIT_CONTROL_SC_COUNTER2   0x80

#define PIT_CONTROL_RW_BIT11      0x30
#define PIT_CONTROL_RW_BIT00      0x00

#define PIT_CONTROL_MODE_0        0x00
#define PIT_CONTROL_MODE_2        0x04

#define PIT_CONTROL_BCD_FALSE     0x00
#define PIT_CONTROL_BCD_TRUE      0x01

#define PIT_COUNTER0_FLAG_ONCE  (PIT_CONTROL_SC_COUNTER0 | PIT_CONTROL_RW_BIT11 | PIT_CONTROL_MODE_0 | PIT_CONTROL_BCD_FALSE)
#define PIT_COUNTER0_FLAG_INTERVAL  (PIT_CONTROL_SC_COUNTER0 | PIT_CONTROL_RW_BIT11 | PIT_CONTROL_MODE_2 | PIT_CONTROL_BCD_FALSE)
#define PIT_COUNTER0_FLAG_LATCH  (PIT_CONTROL_SC_COUNTER0 | PIT_CONTROL_RW_BIT00 )




// SET PIT Counter 0 = _Count and Cange state Interval
void InitializePIT(WORD _Count, BOOL _IsInterval);

WORD ReadTimerCount0();


/*
    Should be disable Interrupt
    Parameter _Count : < 50ms
*/

void WaitUsingPITCounter0(WORD _Count);



#endif /*__PIT_H__*/