#include "PIC.h"
#include <Driver/IO/PortIO.h>

void InitializePIC()
{

    //Initalize Master PIC
    //ICW1 IC4 = 1
    PortIO_OutByte(PIC_MASTER_PORT_1, 0x11);
    //ICW2 Interrupt Vector Offset = 0x20 (32)
    PortIO_OutByte(PIC_MASTER_PORT_2, PIC_IRQ_VECTOR);
    //ICW3 Master-Slave Pin S2=1   |  0x04
    PortIO_OutByte(PIC_MASTER_PORT_2, 0x04);
    //ICW4 SFNM = 0 BUF = 0 M/S = 0 AEOI = 0 uPM = 1 
    PortIO_OutByte(PIC_MASTER_PORT_2, 0x01);
    
    //Initalize Slave PIC
    //ICW1 IC4 = 1
    PortIO_OutByte(PIC_SLAVE_PORT_1, 0x11);
    //ICW2 Interrupt Vector Offset = 0x28
    PortIO_OutByte(PIC_SLAVE_PORT_2, PIC_IRQ_VECTOR + 8);
    //ICW3 Master-Slave Pin Number = 2 | 0x2
    PortIO_OutByte(PIC_SLAVE_PORT_2, 0x02);
    //ICW4 SFNM = 0 BUF = 0 M/S = 0 AEOI = 0 uPM = 1
    PortIO_OutByte(PIC_SLAVE_PORT_2, 0x01);


}   


void MaskPICInterrupt(WORD _IRQ_Mask)
{
    //OCW1 IRQ0~IRQ7
    PortIO_OutByte(PIC_MASTER_PORT_2, _IRQ_Mask);
    //OCW1 IRQ8~IRQ15
    PortIO_OutByte(PIC_SLAVE_PORT_2 , (BYTE)(_IRQ_Mask>>8));
}
void SendPIC_EOI(int _IRQ_Number)
{
    //OCW2 NonSpecific EOI Command.
    PortIO_OutByte(PIC_MASTER_PORT_1, 0X20);

    //If Slave Interrupt.
    if(_IRQ_Number>=8)
    {
        PortIO_OutByte(PIC_SLAVE_PORT_1, 0x20);
    }
}