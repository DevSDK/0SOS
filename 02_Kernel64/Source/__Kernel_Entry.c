
#include "Types.h"
#include "Driver/Keyboard/Keyboard.h"

#include "Driver/Keyboard/PS2Keyboard.h"
#include <Descriptor/IDT.h>
#include <Descriptor/GDT.h>
#include <Descriptor/Descriptor.h>
#include <Driver/PIC/PIC.h>
#include <Utility/Memory.h>
#include <Interrupt/Interrupt.h>
#include <Console/Console.h>
#include <Console/Shell.h>
#include <Scheduling/Scheduler.h>


/*
	2MB 영역에 위치한 EntryPoint.s 에서 호출하는 시작 지점
	GDT, TSS, IDT를 초기화 하고, 프로세서에 등록
	그 후 인터럽트 PIC 드라이버 및 인터럽트 서비스를 초기화 한다.
	키보드 인풋 버퍼 초기화 후, 셸의 엔트리포인트로 진입
*/
void __KERNEL_ENTRY()
{

	_PrintStringXY(5,12, 0x0F,"Initialize GDT........................................");
	InitializeGDTWithTSS();
	LoadGDTR(GDTR_POINTER);
	_PrintStringXY(60,12,0x0A,"[SUCCESS]");
	_PrintStringXY(5,13, 0x0F,"Load TSS Segment .....................................");
	LoadTR(GDT_TSS_SEGMENT);
	_PrintStringXY(60,13,0x0A,"[SUCCESS]");
	_PrintStringXY(5,14, 0x0F,"Initialize IDT .......................................");
	InitializeIDTTables();
	LoadIDTR(IDTR_POINTER);
	_PrintStringXY(60,14,0x0A,"[SUCCESS]");

	_PrintStringXY(5,15, 0x0F,"Initialize Scheduler & Task Pool ......................");	
	InitializeScheduler();
	InitializePIT(MS_TO_COUNT(1),1);
	
	_PrintStringXY(60,15,0x0A,"[SUCCESS]");	
	
	_PrintStringXY(5,16, 0x0F,"PIC Driver And Interrupt Service Initialize............");
	InitializePIC();
	MaskPICInterrupt(0);
	EnableInterrupt();
	_PrintStringXY(60,16,0x0A,"[SUCCESS]");

	_PrintStringXY(5,17, 0x0F,"Keyboard Input Buffer Initialize.......................");
	InitializeKeyboardBuffer();
	_PrintStringXY(60,17,0x0A,"[SUCCESS]");
    __InitializeMemoryCheck();

	BYTE flags;
	int i = 14;
	char temps[2] = {0,};

	if(PS2ActivationKeyboard() == FALSE)
	{
		_PrintStringXY(5,15, 0x0F,"Keyboard Error.");	
		while(1);
	}

	StartShell();
}


