[ORG 0x00]
[BITS 16]

SECTION .text

jmp 0x1000:START


SECTOR: dw 0x0000
TOTALSECTOR: equ 1024

START:
	mov ax, cs
	mov ds, ax
	mov ax, 0xB800

	mov es, ax

	%assign i 0
	%rep TOTALSECTOR
		%assign i i + 1

		mov ax, 2
		mul word[SECTOR]
		mov si, ax

		mov byte[es:si + 160*2], '0' + (i % 10)
		add word[SECTOR], 1

		%if i == TOTALSECTOR
			jmp $
		%else
			jmp (0x1000 + i * 0x20):0x0000
		%endif
		
		times (512 - ($ - $$) % 512 ) db 0x00


	%endrep

