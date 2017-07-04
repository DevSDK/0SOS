[BITS 64]


global PortIO_InByte, PortIO_OutByte

SECTION .text

PortIO_InByte:
	push rdx
	mov rdx, rdi
	mov rax, 0
	in al, dx		
 	pop rdx
	ret
PortIO_OutByte:
	push rdx
	push rax
	mov rdx, rdi
	mov rax, rsi	
	out dx, al
	pop rax
	pop rdx
	ret	
