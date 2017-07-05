#include <Driver/IO/PortIO.h>


BOOL PS2CheckOutputBufferNotEmpty()
{
	
	if(PortIO_InByte(0x64) & 0x01)
		return TRUE;
	return FALSE;
}

BOOL PS2CheckInputBufferNotEmpty()
{
	if(PortIO_InByte(0x64) & 0x02)	
		return TRUE;
	return FALSE;
}

BOOL PS2ActivationKeyboard()
{

	PortIO_OutByte( 0x64, 0xAE );
	
	for(int i=0; i< 0xFFFF; i++)
	{
		if(PS2CheckInputBufferNotEmpty() == FALSE)
		{
			break;
		}
	}

	PortIO_OutByte(0x60, 0xF4);

	for(int i=0; i<123; i++)
	{
		for(int j = 0; j < 0xFFFF; j++)
		{
			if(PS2CheckOutputBufferNotEmpty() == TRUE)
			{
				break;
		
			}
		}

		if(PortIO_InByte(0x60) == 0xFA)
			return TRUE;
	}

	return FALSE;

}

void PS2EnableA20Gate()
{
	PortIO_OutByte(0x64, 0xD0);

	for(int i = 0; i < 0xFFFF; i++)
	{
		if(PS2CheckOutputBufferNotEmpty() == TRUE)
			break;
	}
	
	BYTE OutPutData = PortIO_InByte(0x60);

	OutPutData		|= 0x01;


	for(int i=0; i < 0xFFFF; i++)
	{
		if(PS2CheckInputBufferNotEmpty() == FALSE)
				break;
	}
	
	PortIO_OutByte(0x64, 0xD1);

	PortIO_OutByte(0x60, OutPutData);
}



BOOL PS2SetKeyboardLED(BOOL _CapsLockOn, BOOL _NumLockOn, BOOL _ScrollLockOn)
{

	//Busy Waiting 0xFFFF Iteration.
	for(int i = 0; i< 0xFFFF; i++)
	{
		if(PS2CheckInputBufferNotEmpty() == FALSE)
			break;
	}
	
	
	
	PortIO_OutByte(0x60, 0xED);

	for(int i = 0; i < 0xFFFF; i++)
	{
		if(PS2CheckInputBufferNotEmpty() == FALSE)
			break;
	}
	
	int ic=0;	

	for(ic = 0; ic< 125; ic++)
	{
		for(int j=0; j< 0xFFFF; j++)
		{
			if(PS2CheckOutputBufferNotEmpty()== TRUE)
				break;
		}
		
		if(PortIO_InByte(0x60) == 0xFA)
			break;
	}
	
	if(ic  >= 125)
		return FALSE;

		
	PortIO_OutByte( 0x60, (_CapsLockOn << 2)|(_NumLockOn << 1)|(_ScrollLockOn));

	for(int i = 0; i< 0xFFFF; i++)
	{
		if(PS2CheckInputBufferNotEmpty() == FALSE)
			break;
	}

	for(ic = 0; ic < 125; ic++)
	{
		for(int j = 0; j < 0xFFFF; j++)
		{
			if(PS2CheckOutputBufferNotEmpty() == TRUE)
				break;
		}

		if(PortIO_InByte(0x60) == 0xFA)
			break;

	}
	
	if(ic >= 125)
		return FALSE;
	
	return TRUE;
	
}

void PS2Reboot()
{
	for(int i = 0; i < 0xFFFF; i++)
	{
		if(PS2CheckInputBufferNotEmpty() == FALSE)
			break;
	}
	PortIO_OutByte(0x64, 0xD1);
	PortIO_OutByte(0x60, 0x00);
	
	while(1)
	{
 	 	 	 

	}
}

BYTE PS2GetKeyboardScanCode()
{
	while(PS2CheckOutputBufferNotEmpty() == FALSE);

	return PortIO_InByte(0x60);
}

