#ifndef __MODESWITCH_H__
#define __MODESWITCH_H__


#include "Types.h"

/*
	Get System  CPUID
	in_eax 	: CPUID Parameter
	out_eax : Output CPUID eax register 
	out_ebx : Output CPUID ebx register
	out_ecx : Output CPUID ecx register
	out_edx : Output CPUID edx register
*/
void GetCPUID(DWORD in_eax, DWORD* out_eax,DWORD* out_ebx ,DWORD* out_ecx, DWORD* out_edx);

/*
	Switch 64Bit Kernel Mode 
	
	Set CR4 Register PAE = 1
	Set CR3 Register PML4 Table Adress 0x100000
	Set IA32_EFER.LME = 1 to Enable IA-32e
	Set CR0 Register NW = 0 CD = 0 PG 1
	 
*/
void ModeSwitchAndJumpKernel64();



#endif /*__MODESWITCH_H__*/
