#include "Keyboard.h"
#include <Types.h>

void InitializeKeyboardBuffer()
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
	// 각 문자 별 조합 코드 설정 여부
	// 1. 문자는 쉬프트키와 캡스락 키의 영향을 받음
	// 2. 숫자는 쉬프트의 영향을 받음
	// 3. 넘버패드는 넘버락키의 영향을 받음
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
	//스캔코드의 최상위 비트가 1이면 떨어짐을 의미하고, 0이면 눌림을 의미
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
	// 쉬프트 스캔코드에 따른 쉬프트 상태 업데이트
	if( (DownScanCode == 42) || (DownScanCode == 54) )
	{	
		g_KeyboardStatus.isShiftKeyDown = IsDown;
	}
	// Caps Lcok 스캔코드면 캡스락 업데이트
	else if ((DownScanCode == 58) && (IsDown==TRUE))
	{
		g_KeyboardStatus.isCapsLockOn ^= TRUE;
		IsLedStatusUpdate = TRUE;				
	}
	// Num Lock 키의 스캔코드
	else if((DownScanCode == 69) && (IsDown == TRUE))
	{
		g_KeyboardStatus.isNumLockOn  ^= TRUE;
		IsLedStatusUpdate = TRUE;
	}
	// 스크롤 락키의 스캔코드
	else if((DownScanCode == 70 ) && (IsDown == TRUE))
	{
		g_KeyboardStatus.isScrollLockOn ^= TRUE;
		IsLedStatusUpdate = TRUE;
	}
	// LED 상태 업데이트
	if(IsLedStatusUpdate == TRUE)
	{	
		PS2SetKeyboardLED(g_KeyboardStatus.isCapsLockOn, g_KeyboardStatus.isNumLockOn, g_KeyboardStatus.isScrollLockOn);
	}
}


//Convert Scan Code To ASCII
BOOL ConvertScancodeToASCII(BYTE _ScanCode, BYTE* _ASCIICode, BOOL* _flags)
{
	
	// 이전에 Pause 키가 수신되었다면 Pause의 남은 코드를 무시
	if(g_KeyboardStatus.SkipPauseCount > 0)
	{
		g_KeyboardStatus.SkipPauseCount--;
		return FALSE;
	}
	// Pause 키는 특수처리
	if(_ScanCode == 0xE1)
	{
		(*_ASCIICode) 	= KEY_PAUSE;
		(*_flags)		= (BOOL)KEY_DOWN;
		g_KeyboardStatus.SkipPauseCount = KEY_SKIPCOUNT_PAUSE;
		return TRUE;	
	}
	//확장 키 코드일경우 키 데이터는 다음에 들어옴(읽고 버림)
	else if( _ScanCode == 0xE0)
	{
		g_KeyboardStatus.isExtendCode = TRUE;
		return FALSE;
	}
	// 조합 키인가?
	BOOL IsUseCombinedKey = IsUseCombinedCode(_ScanCode);


	if(IsUseCombinedKey == TRUE)
		*_ASCIICode = g_KeyMapScanTable[ _ScanCode & 0x7F].CombinedCode;	
	else
		*_ASCIICode	= g_KeyMapScanTable[ _ScanCode & 0x7F].NormalCode;

	//확장 키 여부 설정
	if(g_KeyboardStatus.isExtendCode == TRUE)
	{
		*_flags = KEY_EXTEND;
		g_KeyboardStatus.isExtendCode = FALSE;	
	}
	else
		*_flags = 0;

	if((_ScanCode & 0x80) == 0)
		*_flags |=	 KEY_DOWN;
	// 키 상태 업데이트
	SyncKeyStatusAndLED(_ScanCode);
	return TRUE;

}

// 키 데이터를 컨버팅하고, 큐에 삽입
BOOL ConvertScanCodeWithPushKeyQueue(BYTE _ScanCode)
{
	KEYDATA keydata;
	keydata.ScanCode = _ScanCode;
	BOOL result = FALSE;
	if(ConvertScancodeToASCII(_ScanCode, &keydata.ASCIICode, &keydata.Flags) == TRUE)
	{
		// 인터럽트 조정
		BOOL interrupt_status = SetInterruptFlag(FALSE);
		result = PushQueue(&g_KeyBufferQueue, &keydata);
		SetInterruptFlag(interrupt_status);
	}
	return result;
}


//큐에서 데이터를 뽑아옴
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
// Port IO 기능 사용, Reboot
void Reboot()
{
	PS2Reboot();
}