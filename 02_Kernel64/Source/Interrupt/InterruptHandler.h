#ifndef __INTERRUPT_HANDLER_H__
#define __INTERRUPT_HANDLER_H__

#include <Types.h>

void DefaultExceptionHandler(int _Vector, QWORD _ErrorCode);
void DefaultInterruptHandler(int _Vector);
void KeyboardInterruptHandler(int _Vector);


#endif /*__INTERRUPT_HANDLER_H__*/