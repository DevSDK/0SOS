[BITS 32]

global GetCPUID, ModeSwitchAndJumpKernel64


SECTION .text


; Get Cpu ID
; Following Function Use with c language 
; void GetCPUID(DWORD in_eax, DWORD* out_eax, DWORD* out_ecx, DWORD* out_edx) 
; in_eax 	: cpuid Parameter
; out_eax 	: output eax register
; out_ecx 	: output ecx register 
; out_edx	: output edx register
GetCPUID:
	push ebp
	mov ebp, esp
	push eax
	push ebx
	push ecx
	push edx
	push esi

	mov eax, dword[ebp + 8]
	cpuid
	mov esi, dword[ebp + 12]
	mov dword[esi], eax
	
	mov esi, dword[ebp + 16]
	mov dword[esi], ebx
	
	mov esi, dword[ebp + 20]
	mov dword[esi], ecx

	mov esi, dword[ebp + 24]
	mov dword[esi], edx



	pop esi
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop ebp
	ret
	
; Switch IA-32e Mode And Jump 64 Bit Kernel 
; void ModeSwitchAndJumpKernel64() 
;
ModeSwitchAndJumpKernel64:
	
	
	; Set 1 CR4 Register PAE Bit
	mov eax, cr4
	or eax, 0x20
	mov cr4, eax

	mov eax, 0x100000
	mov cr3, eax
	
	;IA_EFER 레지스터의 LME 비트를 활성화	
	mov ecx, 0xC0000080
	rdmsr
	or eax, 0x0100
	wrmsr
	

	
	;Write Table
	mov eax, cr0
	or  eax, 0xE0000000
	xor eax, 0x60000000
	;NW(29) = 0, CD(30) = 0, PG(31) = 1 
	mov cr0, eax
	
	;IA-32e 세그먼트 설렉트 후 2MB 영역으로 점프
	jmp 0x08:0x200000
	
	;Not Entry
	jmp $
	
	

	

