#ifndef __PIC_H__
#define __PIC_H__
#include <Types.h>

#define PIC_MASTER_PORT_1   0x20
#define PIC_MASTER_PORT_2   0x21

#define PIC_SLAVE_PORT_1    0xA0
#define PIC_SLAVE_PORT_2    0xA1

#define PIC_IRQ_VECTOR      0x20

void InitializePIC();
void MaskPICInterrupt(WORD _IRQ_Mask);
void SendPIC_EOI(int _IRQ_Number);

#endif /* __PIC_H__ */