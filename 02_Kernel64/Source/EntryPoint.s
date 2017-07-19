[BITS 64]

SECTION .text

extern __KERNEL_ENTRY
;링커 스크립터에 의해 2MB 영역에 로드되는 코드
START:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	;스택 초기화
	mov ss,  ax
	mov rsp, 0x6FFFF8
	mov rbp, 0x6FFFF8

	call __KERNEL_ENTRY
	
	jmp $
