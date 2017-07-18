#include "Keyboard.h"
#include <Types.h>

void InitalizeKeyboardBuffer()
{
	InitializeQueue(&g_KeyBufferQueue, g_KeyBufferMemory, KEY_BUFFER_SIZE, sizeof(KEYDATA));
}

BOOL IsAlphabetScanCode(BYTE _ScanCode)
{
	if( ('a' <= g_KeyMapScanTable[_ScanCode].NormalCode)&&
		(g_KeyMapScanTable[_ScanCode].NormalCode <= 'z') )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL IsNumberOrSymbolScanCode(BYTE _ScanCode)
{
	if( (2 <= _ScanCode) && (_ScanCode <= 53) && (IsAlphabetScanCode(_ScanCode) == FALSE))
		return TRUE;
		
	return FALSE;
}

BOOL IsNumberPadScanCode(BYTE _ScanCode)
{
	if( (71 <= _ScanCode) && (_ScanCode <= 83))
		return TRUE;
	
	return FALSE;
}

//For Shift, Caps Lock , ETC Combined Code
BOOL IsUseCombinedCode(BOOL _ScanCode)
{
	BYTE DownScanCode = _ScanCode & 0x7F;
	BOOL IsCombinedKey = FALSE;

	if(IsAlphabetScanCode(_ScanCode) == TRUE)
	{
		if(g_KeyboardStatus.isShiftKeyDown ^ g_KeyboardStatus.isCapsLockOn) 
			IsCombinedKey = TRUE;
		else
			IsCombinedKey = FALSE;
	}
	else if( IsNumberOrSymbolScanCode(_ScanCode) == TRUE)
	{
		if(g_KeyboardStatus.isShiftKeyDown == TRUE)
			IsCombinedKey = TRUE;
		else
			IsCombinedKey = FALSE;
	}
	else if((IsNumberPadScanCode(_ScanCode) == TRUE )&&
			(g_KeyboardStatus.isExtendCode) == FALSE)
	{
		if(g_KeyboardStatus.isNumLockOn == TRUE)
			IsCombinedKey = TRUE;
		else
			IsCombinedKey = FALSE;

	}
	return IsCombinedKey;
}

//Sync Status , Keyboard LED 
void SyncKeyStatusAndLED(BYTE _ScanCode)
{
	BOOL IsDown = FALSE;
	BYTE DownScanCode;
	BOOL IsLedStatusUpdate = FALSE;

	if(_ScanCode & 0x80)
	{
		IsDown 			= FALSE;
		DownScanCode	= _ScanCode & 0x7F;
	}
	else
	{
		IsDown 			= TRUE;
		DownScanCode 	= _ScanCode;
	}

	if( (DownScanCode == 42) || (DownScanCode == 54) )
	{	
		g_KeyboardStatus.isShiftKeyDown = IsDown;
	}
	else if ((DownScanCode == 58) && (IsDown==TRUE))
	{
		g_KeyboardStatus.isCapsLockOn ^= TRUE;
		IsLedStatusUpdate = TRUE;				
	}
	else if((DownScanCode == 69) && (IsDown == TRUE))
	{
		g_KeyboardStatus.isNumLockOn  ^= TRUE;
		IsLedStatusUpdate = TRUE;
	}
	else if((DownScanCode == 70 ) && (IsDown == TRUE))
	{
		g_KeyboardStatus.isScrollLockOn ^= TRUE;
		IsLedStatusUpdate = TRUE;
	}
	
	if(IsLedStatusUpdate == TRUE)
	{	
		PS2SetKeyboardLED(g_KeyboardStatus.isCapsLockOn, g_KeyboardStatus.isNumLockOn, g_KeyboardStatus.isScrollLockOn);
	}
}


//Convert Scan Code To ASCII
BOOL ConvertScancodeToASCII(BYTE _ScanCode, BYTE* _ASCIICode, BOOL* _flags)
{
	
	
	if(g_KeyboardStatus.SkipPauseCount > 0)
	{
		g_KeyboardStatus.SkipPauseCount--;
		return FALSE;
	}

	if(_ScanCode == 0xE1)
	{
		(*_ASCIICode) 	= KEY_PAUSE;
		(*_flags)		= (BOOL)KEY_DOWN;
		g_KeyboardStatus.SkipPauseCount = KEY_SKIPCOUNT_PAUSE;
		return TRUE;	
	}
	else if( _ScanCode == 0xE0)
	{
		g_KeyboardStatus.isExtendCode = TRUE;
		return FALSE;
	}

	BOOL IsUseCombinedKey = IsUseCombinedCode(_ScanCode);

	if(IsUseCombinedKey == TRUE)
		*_ASCIICode = g_KeyMapScanTable[ _ScanCode & 0x7F].CombinedCode;	
	else
		*_ASCIICode	= g_KeyMapScanTable[ _ScanCode & 0x7F].NormalCode;

	if(g_KeyboardStatus.isExtendCode == TRUE)
	{
		*_flags = KEY_EXTEND;
		g_KeyboardStatus.isExtendCode = FALSE;	
	}
	else
		*_flags = 0;

	if((_ScanCode & 0x80) == 0)
		*_flags |=	 KEY_DOWN;

	SyncKeyStatusAndLED(_ScanCode);
	return TRUE;

}


BOOL ConvertScanCodeWithPushKeyQueue(BYTE _ScanCode)
{
	KEYDATA keydata;
	keydata.ScanCode = _ScanCode;
	BOOL result = FALSE;
	if(ConvertScancodeToASCII(_ScanCode, &keydata.ASCIICode, &keydata.Flags) == TRUE)
	{
		BOOL interrupt_status = SetInterruptFlag(FALSE);
		result = PushQueue(&g_KeyBufferQueue, &keydata);
		SetInterruptFlag(interrupt_status);
	}
	return result;
}



BOOL GetKeyData(KEYDATA* _data)
{
	BOOL result = FALSE;

	if(IsQueueEmpty(&g_KeyBufferQueue))
		return FALSE;
	
	BOOL interruptstatus = SetInterruptFlag(FALSE);
	result = PopQueue(&g_KeyBufferQueue,_data);
	SetInterruptFlag(interruptstatus);
	return result;
}

void Reboot()
{
	PS2Reboot();
}