#ifndef __PORT_IO_H_
#define __PORT_IO_H_

#include <Types.h>

/*
	Execute From Asam Code.
	Param:
		_PortNo : The Port Number for Port IO Memory
	Return:
		Get Data From Port	
*/
BYTE PortIO_InByte(WORD _PortNo);

/*
	Execute From Asam Code.

	Param:
		_PortNo : The Port Number for Port IO Memory
		_Data

*/
void PortIO_OutByte(WORD _PortNo, BYTE _data);




#endif
