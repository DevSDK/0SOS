#include "InterruptHandler.h"
#include <Driver/PIC/PIC.h>
#include <Utility/Console.h>
void __DebugIntOutput(int _Vector, int _Count)
{
    char Buffer[] = "[INT:  ,  ]";
    Buffer[5] = _Vector/10 + '0';
    Buffer[6] = _Vector%10 + '0';
    Buffer[8] = _Count/10  + '0';
    Buffer[9] = _Count%10  + '0';  
    PrintVideoMemory(69,0,0x0F,Buffer);
}

void DefaultExceptionHandler(int _Vector, QWORD _ErrorCode)
{
    char Buffer[3] = {0,};
    Buffer[0] = '0' + _Vector/10;
    Buffer[1] = '0' + _Vector%10;
    PrintVideoMemory(0,0,0xF,"================================================================================");
    PrintVideoMemory(0,1,0xF,"Exception:                                                                      ");
    PrintVideoMemory(0,2,0xF,"                                                                                ");
    PrintVideoMemory(0,3,0xF,"================================================================================");
    PrintVideoMemory(11,1,0xF, Buffer);

    while(1);
}
void DefaultInterruptHandler(int _Vector)
{
    
    static int g_DefaultInterruptCounter = 0;

    g_DefaultInterruptCounter = (g_DefaultInterruptCounter + 1)%100;
    __DebugIntOutput(_Vector, g_DefaultInterruptCounter);
    SendPIC_EOI(_Vector - PIC_IRQ_VECTOR);
}
void KeyboardInterruptHandler(int _Vector)
{
    
    static int g_KeyboardInterruptCounter = 0;
    g_KeyboardInterruptCounter = (g_KeyboardInterruptCounter + 1)%100;
    __DebugIntOutput(_Vector, g_KeyboardInterruptCounter);
    
    if(PS2CheckOutputBufferNotEmpty() == TRUE)
    {
        BYTE scancode = PS2GetKeyboardScanCode();
        ConvertScanCodeWithPushKeyQueue(scancode);
    }
    
    SendPIC_EOI(_Vector - PIC_IRQ_VECTOR);
}