[ORG 0x00]
[BITS 16]

SECTION .text


START:
	mov ax, 0x1000

	mov ds, ax
	mov es, ax
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; Enable A20 Gate

	mov ax, 0x2401
	int 0x15
	jc A20GATEENABLEERROR

	jmp A20GATEENABLESUCCESS




A20GATEENABLEERROR:
	in al, 0x92
	or al, 0x02
	and al, 0xFE
	out 0x92, al

A20GATEENABLESUCCESS:


	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	cli		;Ignore Interrupt

	lgdt[GDTR]

	;	Now Switch Protected Mode.
	;
	;   CR0 Register
	;	Summary: 
	;		Disable Paging, Disable Cache, Internal FPU, Disable Align Check
	;
	;   fields:
	;
	;	|PG|CD|NW|  |  |  |  |  |  |  |  |  |  |AM|  |WP|
	;	31---------------------------------------------16
	;	| 0| 1| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0|
	;
	;	|  |  |  |  |  |  |  |  |  |  |NE|ET|TS|EM|MP|PE| 
	;	15----------------------------------------------0
	;	| 0| 0| 0| 0| 0| 0| 0| 0| 0| 0| 1| 1| 1| 0| 1| 1|
	;
	;	0b01000000000000000000000000111011	
	; 	0x4000003B
	;

	mov eax, 0x4000003B
	mov cr0, eax

	jmp dword 0x18 : (PROTECTEDMODE - $$ + 0x10000)


	; Following Code Now Prtoected Mode

[BITS 32]

PROTECTEDMODE:
	mov ax, 0x20
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax


	mov ss, ax
	mov esp, 0XFFFE
	mov ebp, 0XFFFE
		
	push (SWITCHMESSAGE - $$ + 0x10000)
	push 0x0F
	push 4
	push 5
	call PRINT
	add  esp, 16


	push (SUCCESS - $$ + 0x10000)
	push 0x0A
	push 4
	push 60
	call PRINT
	add  esp, 16


	jmp dword 0x18:0x10200		;Let's Jump To C

PRINT:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push eax
	push ecx
	push edx

	mov eax, dword [ebp + 12]
	mov esi, 160
	mul esi
	mov edi, eax
		
	mov eax, dword[ebp + 8]
	mov esi, 2
	mul esi
	add edi, eax
	mov bl, byte[ebp + 16]
	mov esi, dword[ebp + 20]	
.PRINTLOOP:
	mov cl, byte[esi]
	
	cmp cl,0
	je PRINTEND
	
	mov byte[edi + 0xB8000], cl
	mov byte[edi + 0xB8000+1], bl	
	add esi,1
	add edi,2	
	jmp .PRINTLOOP
 
PRINTEND:
	pop edx
	pop ecx
	pop eax
	pop edi
	pop esi
	pop ebp
	ret



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; DATA AREA
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

align 8, db 0

dw 0x0000	;For Align

GDTR:
	dw GDTREND - GDT -1
	dd GDT - $$ + 0x10000

GDT:
	NULLDescriptor:
	dw 0x0000
	dw 0x0000
	db 0x00
	db 0x00
	db 0x00
	db 0x00

	IA_32eCodeDescriptor:
	dw 0xFFFF	; Limit[15:0]
	dw 0x0000	; Base[15:0]
	db 0x00		; Base[23:16] 
	db 0x9A		; P = 1, DPL = 0, Code Segment, Execute/Read
	db 0xAF		; G = 1, D = 0, L = 1, Limit [19:16]
	db 0x00		; Base[31:24]
	
	IA_32eDataDescriptor:
	dw 0xFFFF	; Limit[15:0]
	dw 0x0000	; Base[15:0]
	db 0x00		; Base[23:16] 
	db 0x92		; P = 1, DPL = 0, Code Segment, Execute/Read
	db 0xAF		; G = 1, D = 0, L = 1, Limit [19:16]
	db 0x00		; Base[31:24]
	
	CODEDescriptor:
	dw 0xFFFF	; Limit[15:0]
	dw 0x0000	; Base[15:0]
	db 0x00		; Base[23:16]
	db 0x9A		; P = 1, DPL = 0, Code Segment, Execute/Read
	db 0xCF		; G = 1, D = 1, L = 0, Limit [19:16]
	db 0x00		; Base[31:24]
	
	DATADescriptor:
	dw 0xFFFF	; Limit
	dw 0x0000	; Base
	db 0x00		; Base
	db 0x92		; P=1, DPL =0, Data Segment, Read/Write
	db 0xCF		; G= 1, D= 1, L= 0, Limit
	db 0x00		; Base
GDTREND:


SWITCHMESSAGE: db 'Switch to 32bit protected mode ....................... ',0
SUCCESS: db '[SUCCESS]',0


times 512 - ( $ - $$) db 0x00
