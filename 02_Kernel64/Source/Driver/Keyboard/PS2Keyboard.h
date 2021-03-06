#ifndef __PA2KEYBOARD_H_
#define __PA2KEYBOARD_H_
#include <Types.h>

/*
	This file implementation Keyboard Port Driver
	PS/2 0x60, 0x64 Keyboard Port Data 
*/


BOOL PS2ActivationKeyboard();

void PS2EnableA20Gate();

BOOL PS2SetKeyboardLED(BOOL _CapsLockOn, BOOL _NumLockOn,  BOOL _ScrollLockOn);

BOOL PS2CheckInputBufferNotEmpty();
BOOL WaitACKWithScanCodePushQueue();
BYTE PS2GetKeyboardScanCode();
BOOL PS2CheckOutputBufferNotEmpty();
void PS2Reboot();


#endif /* __PA2KEYBOARD_H_ */
